/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ByteBuffer.h"
#include "TargetedMovementGenerator.h"
#include "Errors.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "World.h"
#include "MoveSplineInit.h"
#include "MoveSpline.h"
#include "Player.h"
#include "Spell.h"
#include "Vehicle.h"

template<class T, typename D>
void TargetedMovementGeneratorMedium<T,D>::_setTargetLocation(T* owner, bool updateDestination)
{
    if (!i_target.isValid() || !i_target->IsInWorld())
        return;

    if (owner->HasUnitState(UNIT_STATE_NOT_MOVE))
        return;

    if (owner->GetTypeId() == TYPEID_UNIT && !i_target->isInAccessiblePlaceFor(owner->ToCreature()))
        return;

    float x, y, z;

    // to nearest contact position
    /*
        By default, this functon includes the owner's combat reach.
    */
    float distance_excluding_owner_combat_reach
    {
        (
            i_target->ToUnit()->GetMeleeReEnagementDistance(owner->ToUnit())
        - (owner->GetObjectSize() + i_target->GetObjectSize())
        )
    };
    if (updateDestination || !i_path)
    {
        if (!i_offset)
        {
            i_target->GetContactPoint(owner, x, y, z, distance_excluding_owner_combat_reach);
        }
        else
        {
            float size = DEFAULT_WORLD_OBJECT_SIZE;
            if (!owner->HasUnitState(UNIT_STATE_FOLLOW))
            {
                size = i_target->GetObjectSize();
                if (i_target->IsWithinDistInMap(owner, i_offset + size))
                    return;
            }

            // to at i_offset distance from target and i_angle from target facing
            i_target->GetClosePoint(x, y, z, size, i_offset, i_angle);
        }
    }
    else
    {
        // the destination has not changed, we just need to refresh the path (usually speed change)
        G3D::Vector3 end = i_path->GetEndPosition();
        x = end.x;
        y = end.y;
        z = end.z;
    }

    if (!i_path)
        i_path = new PathGenerator(owner);

    // allow pets to use shortcut if no path found when following their master
    bool forceDest = owner->GetTypeId() == TYPEID_UNIT && owner->HasUnitTypeMask(UNIT_MASK_MINION) &&
        owner->HasUnitState(UNIT_STATE_FOLLOW);

    bool result = i_path->CalculatePath(x, y, z, forceDest);
    if (!result || (i_path->GetPathType() & PATHFIND_NOPATH))
    {
        // Cant reach target
        i_recalculateTravel = true;
        return;
    }

    D::_addUnitStateMove(owner);
    i_targetReached = false;
    i_recalculateTravel = false;
    owner->AddUnitState(UNIT_STATE_CHASE);

    Movement::MoveSplineInit init(owner);
    init.MovebyPath(i_path->GetPath());
    init.SetWalk(static_cast<D*>(this)->EnableWalking(owner));
    static_cast<D*>(this)->_updateSpeed(owner);
    // Using the same condition for facing target as the one that is used for SetInFront on movement end
    // - applies to ChaseMovementGenerator mostly
    if (i_angle == 0.f)
        init.SetFacing(i_target.getTarget());

    init.Launch();
}

template<class T, typename D>
bool TargetedMovementGeneratorMedium<T,D>::DoUpdate(T* owner, uint32 time_diff)
{
    if (!i_target.isValid() || !i_target->IsInWorld())
        return false;

    if (!owner || !owner->isAlive())
        return false;

    if (owner->HasUnitState(UNIT_STATE_NOT_MOVE))
    {
        D::_clearUnitStateMove(owner);
        return true;
    }

    // prevent movement while casting spells with cast time or channel time
    if (owner->IsMovementPreventedByCasting())
    {
        if (!owner->IsStopped())
            owner->StopMoving();
        return true;
    }

    // prevent crash after creature killed pet
    if (static_cast<D*>(this)->_lostTarget(owner))
    {
        D::_clearUnitStateMove(owner);
        return true;
    }

    // Check whether pet got send to cast a spell
    if (i_spell)
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(i_spell);
        Spell* spell = new Spell(owner->ToUnit(), spellInfo, TRIGGERED_NONE);
        SpellCastResult result = spell->CheckPetCast(i_target.getTarget());
        if (result == SPELL_CAST_OK)
        {
            spell->prepare(&(spell->m_targets));
            // Reset i_spell here to prevent chaincasting.
            i_spell = 0;
        }
        else
        {
            spell->finish(false);
            delete spell;
        }
    }

    bool targetMoved = false;
    i_recheckDistance.Update(time_diff);
    if (i_recheckDistance.Passed())
    {
        i_recheckDistance.Reset(100);
        // More distance gives better performance, less distance gives more sensitive reaction at target move.
        /*
            Scenario:
            "owner" is npc.
            "i_target" could be an npc or a player.
            world config is an addition.

            If "owner" reach is something absurd like 10, then using the owner reach can make the npc kite players and keep it a 1 way fight.
            This was found to be the case with young storm dragon in vortex pinnacle.
            To avoid this, we need to base the distance off the target's (possibly a player) combat reach so they can always fight back with melee.
        */
        //float allowed_dist = owner->GetMeleeRange(i_target->ToUnit());
        float allowed_dist = owner->ToUnit()->GetMeleeRange(i_target->ToUnit());

        //std::stringstream debug_report;


        if (owner->HasUnitState(UNIT_STATE_FOLLOW))
        {
            allowed_dist = owner->GetMeleeReach();
            //debug_report << ", HAS BEEN EDITED TO MELEE REACH";
        }

        Position OwnerPos;
        owner->GetPosition(&OwnerPos);
        if (owner->movespline->onTransport)
            if (TransportBase* transport = owner->GetDirectTransport())
            {
                float ori = 0.0f;
                transport->CalculatePassengerPosition(OwnerPos.m_positionX, OwnerPos.m_positionY, OwnerPos.m_positionZ, ori);
            }

        if (auto o = owner->ToUnit())
            if (auto t = i_target->ToUnit())
            {
                if ((!o->IsWithinMeleeRange(t)) || (!i_target->IsWithinLOS(OwnerPos.m_positionX, OwnerPos.m_positionY, OwnerPos.m_positionZ)))
                {
                    if (t->ToPlayer() && o->ToPet())
                        {
                            targetMoved = false;
                            if (auto p = t->ToPlayer())
                                if (auto petCreature = o->ToCreature())
                                {
                                    /*                                    
                                        uint64 following_guid;
                                        Position following_to_position;
                                        float distance_to_follow_pos;
                                    */
                                    auto info = petCreature->GetNPCFollowData();

                                    NpcFollowData new_follow_info
                                    {
                                        { t->GetGUID() },
                                        { t->GetPositionX(), t->GetPositionY(), t->GetPositionZ(), t->GetOrientation() },
                                        { o->GetExactDist2d(t) }
                                    };

                                    if (new_follow_info.following_guid != info.following_guid)                          //new follow target
                                    {
                                        //TC_LOG_ERROR("sql.sql", "targetMoved = true case 1");
                                        targetMoved = true;
                                    }
                                    else if (new_follow_info.distance_to_follow_pos < info.distance_to_follow_pos)     //follow target is getting closer
                                    {
                                        //TC_LOG_ERROR("sql.sql", "targetMoved = false case 2");
                                        targetMoved = true;
                                    }
                                    else if (!petCreature->isMoving() && !o->IsWithinMeleeRange(t))
                                    {
                                        //TC_LOG_ERROR("sql.sql", "targetMoved = true case 3");
                                        targetMoved = true;
                                    }
                                    
                                    petCreature->SetNPCFollowData(new_follow_info);
                                }
                        }
                        else
                            targetMoved = true;
                }
                else
                    targetMoved = false;
            }

        /*
        if (auto o = owner->ToUnit())
            if (auto t = i_target->ToUnit())
                if (t->ToPlayer())
                    if (o->ToPet())
                    if (targetMoved)
                    {
                        TC_LOG_ERROR("sql.sql", "targetmoved debug: %u %u", (o->GetExactDist2d(t) > allowed_dist), !i_target->IsWithinLOS(OwnerPos.m_positionX, OwnerPos.m_positionY, OwnerPos.m_positionZ));
                        debug_report << "allowed_dist (total): " << allowed_dist << ", actual distance:" << (o->GetExactDist2d(t)) << "\n-----\n";
                        sWorld->SendGMText(LANG_GM_BROADCAST, debug_report.str().c_str());
                    }
        */

    }

    if (i_recalculateTravel || targetMoved)
    {
        _setTargetLocation(owner, targetMoved);
    }

    if (!i_targetReached)
    if (owner->movespline->Finalized())
    {
        static_cast<D*>(this)->MovementInform(owner);
        if (i_angle == 0.f && !owner->HasInArc(0.01f, i_target.getTarget()))
            owner->SetInFront(i_target.getTarget());

            i_targetReached = true;
            static_cast<D*>(this)->_reachTarget(owner);
    }

    return true;
}

//-----------------------------------------------//
template<class T>
void ChaseMovementGenerator<T>::_reachTarget(T* owner)
{
    if (owner->IsWithinMeleeRange(this->i_target.getTarget(), 0.f))
    {
        if (owner->GetEntry() == 43214)
        TC_LOG_ERROR("sql.sql", "reached target.");
        owner->Attack(this->i_target.getTarget(), true);
    }
}

template<>
void ChaseMovementGenerator<Player>::DoInitialize(Player* owner)
{
    owner->AddUnitState(UNIT_STATE_CHASE | UNIT_STATE_CHASE_MOVE);
    _setTargetLocation(owner, true);
}

template<>
void ChaseMovementGenerator<Creature>::DoInitialize(Creature* owner)
{
    owner->SetWalk(false);
    owner->AddUnitState(UNIT_STATE_CHASE | UNIT_STATE_CHASE_MOVE);
    _setTargetLocation(owner, true);
}

template<class T>
void ChaseMovementGenerator<T>::DoFinalize(T* owner)
{
    owner->ClearUnitState(UNIT_STATE_CHASE | UNIT_STATE_CHASE_MOVE);
}

template<class T>
void ChaseMovementGenerator<T>::DoReset(T* owner)
{
    DoInitialize(owner);
}

template<class T>
void ChaseMovementGenerator<T>::MovementInform(T* /*unit*/)
{
}

template<>
void ChaseMovementGenerator<Creature>::MovementInform(Creature* unit)
{
    // Pass back the GUIDLow of the target. If it is pet's owner then PetAI will handle
    if (unit->AI())
        unit->AI()->MovementInform(CHASE_MOTION_TYPE, i_target.getTarget()->GetGUIDLow());
}

//-----------------------------------------------//
template<>
bool FollowMovementGenerator<Creature>::EnableWalking(Creature* owner) const
{
    return i_target.isValid() && i_target->IsWalking();
}

template<>
bool FollowMovementGenerator<Player>::EnableWalking(Player* /*owner*/) const
{
    return false;
}

template<>
void FollowMovementGenerator<Player>::_updateSpeed(Player* /*owner*/)
{
    // nothing to do for Player
}

template<>
void FollowMovementGenerator<Creature>::_updateSpeed(Creature* owner)
{
    // pet only sync speed with owner
    /// Make sure we are not in the process of a map change (IsInWorld)
    if (!owner->HasUnitTypeMask(UNIT_MASK_MINION) || !owner->IsInWorld() ||
        !i_target.isValid() || i_target->GetGUID() != owner->GetOwnerGUID())
        return;

    owner->UpdateSpeed(MOVE_RUN, true);
    owner->UpdateSpeed(MOVE_SWIM, true);
    owner->UpdateSpeed(MOVE_FLIGHT, true);
}

template<>
void FollowMovementGenerator<Player>::_reachTarget(Player* /*owner*/) { }

template<>
void FollowMovementGenerator<Creature>::_reachTarget(Creature* owner)
{
    if (i_target->GetTypeId() == TYPEID_PLAYER && i_target->IsWithinMeleeRange(owner))
        owner->SetFacingTo(i_target->GetOrientation());
}

template<>
void FollowMovementGenerator<Player>::DoInitialize(Player* owner)
{
    owner->AddUnitState(UNIT_STATE_FOLLOW | UNIT_STATE_FOLLOW_MOVE);
    _updateSpeed(owner);
    _setTargetLocation(owner, true);
}

template<>
void FollowMovementGenerator<Creature>::DoInitialize(Creature* owner)
{
    owner->AddUnitState(UNIT_STATE_FOLLOW | UNIT_STATE_FOLLOW_MOVE);
    _updateSpeed(owner);
    _setTargetLocation(owner, true);
}

template<class T>
void FollowMovementGenerator<T>::DoFinalize(T* owner)
{
    owner->ClearUnitState(UNIT_STATE_FOLLOW | UNIT_STATE_FOLLOW_MOVE);
    _updateSpeed(owner);
}

template<class T>
void FollowMovementGenerator<T>::DoReset(T* owner)
{
    DoInitialize(owner);
}

template<class T>
void FollowMovementGenerator<T>::MovementInform(T* /*unit*/)
{
}

template<>
void FollowMovementGenerator<Creature>::MovementInform(Creature* unit)
{
    // Pass back the GUIDLow of the target. If it is pet's owner then PetAI will handle
    if (unit->AI())
        unit->AI()->MovementInform(FOLLOW_MOTION_TYPE, i_target.getTarget()->GetGUIDLow());
}

//-----------------------------------------------//
template void TargetedMovementGeneratorMedium<Player,ChaseMovementGenerator<Player> >::_setTargetLocation(Player*, bool);
template void TargetedMovementGeneratorMedium<Player,FollowMovementGenerator<Player> >::_setTargetLocation(Player*, bool);
template void TargetedMovementGeneratorMedium<Creature,ChaseMovementGenerator<Creature> >::_setTargetLocation(Creature*, bool);
template void TargetedMovementGeneratorMedium<Creature,FollowMovementGenerator<Creature> >::_setTargetLocation(Creature*, bool);
template bool TargetedMovementGeneratorMedium<Player,ChaseMovementGenerator<Player> >::DoUpdate(Player*, uint32);
template bool TargetedMovementGeneratorMedium<Player,FollowMovementGenerator<Player> >::DoUpdate(Player*, uint32);
template bool TargetedMovementGeneratorMedium<Creature,ChaseMovementGenerator<Creature> >::DoUpdate(Creature*, uint32);
template bool TargetedMovementGeneratorMedium<Creature,FollowMovementGenerator<Creature> >::DoUpdate(Creature*, uint32);

template void ChaseMovementGenerator<Player>::_reachTarget(Player*);
template void ChaseMovementGenerator<Creature>::_reachTarget(Creature*);
template void ChaseMovementGenerator<Player>::DoFinalize(Player*);
template void ChaseMovementGenerator<Creature>::DoFinalize(Creature*);
template void ChaseMovementGenerator<Player>::DoReset(Player*);
template void ChaseMovementGenerator<Creature>::DoReset(Creature*);
template void ChaseMovementGenerator<Player>::MovementInform(Player*);

template void FollowMovementGenerator<Player>::DoFinalize(Player*);
template void FollowMovementGenerator<Creature>::DoFinalize(Creature*);
template void FollowMovementGenerator<Player>::DoReset(Player*);
template void FollowMovementGenerator<Creature>::DoReset(Creature*);
template void FollowMovementGenerator<Player>::MovementInform(Player*);
