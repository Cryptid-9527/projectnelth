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

#include "Creature.h"
#include "MapManager.h"
#include "ConfusedMovementGenerator.h"
#include "PathGenerator.h"
#include "VMapFactory.h"
#include "MoveSplineInit.h"
#include "MoveSpline.h"
#include "Player.h"
//#include "Chat.h" //only needed for debugging to chatbox

#ifdef MAP_BASED_RAND_GEN
#define rand_norm() unit.rand_norm()
#define urand(a, b) unit.urand(a, b)
#endif

template<class T>
void ConfusedMovementGenerator<T>::DoInitialize(T* unit)
{
    unit->AddUnitState(UNIT_STATE_CONFUSED);
    unit->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_CONFUSED);
    unit->GetPosition(i_x, i_y, i_z);

    if (!unit->isAlive() || unit->IsStopped())
        return;

    unit->StopMoving();
    unit->AddUnitState(UNIT_STATE_CONFUSED_MOVE);
}

template<class T>
void ConfusedMovementGenerator<T>::DoReset(T* unit)
{
    i_nextMoveTime.Reset(0);

    if (!unit->isAlive() || unit->IsStopped())
        return;

    unit->StopMoving();
    unit->AddUnitState(UNIT_STATE_CONFUSED | UNIT_STATE_CONFUSED_MOVE);
}

template<class T>
bool ConfusedMovementGenerator<T>::DoUpdate(T* unit, uint32 diff)
{
    if (unit->HasUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED | UNIT_STATE_DISTRACTED))
        return true;

    if (i_nextMoveTime.Passed())
    {
        // currently moving, update location
        unit->AddUnitState(UNIT_STATE_CONFUSED_MOVE);

        if (unit->movespline->Finalized())
            i_nextMoveTime.Reset(urand(200, 500));
    }
    else
    {
        // waiting for next move
        i_nextMoveTime.Update(diff);
        if (i_nextMoveTime.Passed())
        {
            // start moving
            unit->AddUnitState(UNIT_STATE_CONFUSED_MOVE);
            /*
                This configuration is the result of several dozen videos being reviewed.
                There were several cases where somewhat of a triangle was recognized, but a slightly lower amount that could not recognize a triangle.
                This means that the pattern is centered around a perfect triangle, randomized enough to generate non-triangle shapes on occasion.
            */
            /*
                UPDATE: 2 JULY 2022 - CRYPTID
                - An excel spreadsheet has been made to decide these numbers. 
                The odds have been fine-tuned so that there is a 50% chance for a perfectly centered 8-second scatter shot to return to within freezing trap activation range.

                The data configuration is as follows:
                (Sample Size - Excel generated)
                Number of Movements	                                        12951
                Number of Scatters	                                        4317

                (Distances)
                Closest distance:	                                        0.00493877373953
                Average Distance:	                                        3.611239757
                Furthest Distance:	                                        9.442824281
                Distance Variation:	                                        9.43788550748308

                Movements which land within activation radius	            2881
                Activation Movements, no double-dip	                        2163

                Odds of Perfectly centered trap triggering during scatter:	67%
                Non Double-Dip Odds	                                        50%
                
                -------------------------------------------------------------------------
                (Ideal Configuration)
                2.31 M <->3.30 M Movement distance
                120 Degrees left angle, with +- 25 degrees Variation

                **NOTES FOR ADJUSTING THIS WITHOUT THE EXCEL SHEET**
                - "exact_triangle_angle" is set at 120 degrees so that 8 seconds of confused movement should base it's movement around a triangle.
                - "allowed_triangle_tolerance" has largely no effect on the average odds of the movement returning to center.

                "travel_distance" is the main variable that decides the odds of a confused movement returning to center, here are some benchmarks:
                    - (5.f, 10.f) produce a Non Double-Dip Odds of 10%
                    - (2.f, 3.f) produce a Non Double-Dip Odds of 70% 

            */
            float travel_distance               {   frand(2.33f, 3.35f)                                                                                                  };
            float exact_triangle_angle          {   ((M_PI*2.f) / 3.f)/*120 degrees to the left*/                                                                       };
            float allowed_triangle_tolerance    {   ((M_PI * 2.f) / 14.4f)/*25 degrees left or right*/                                                                    };
            float decided_movement_angle        {   frand((exact_triangle_angle - allowed_triangle_tolerance), (exact_triangle_angle + allowed_triangle_tolerance))     };

            /*
            std::ostringstream ostream;
            if (auto player = unit->ToPlayer())
                if (auto s = player->GetSession())
                {
                    LocaleConstant loc_idx = s->GetSessionDbLocaleIndex();
                    WorldPacket data(SMSG_MESSAGECHAT, 200);
                    ostream << "Target confused, destination yards: " << dest << ", current orientation" << unit->GetOrientation() << ", exact triangle angle: " << exact_triangle_angle << ", allowed tolerance: " << allowed_triangle_tolerance << ", decided angle: " << decided_movement_angle;
                    auto fullmsg{ ostream.str() };
                    ChatHandler(s).PSendSysMessage("%s", fullmsg.c_str());
                }
            */

            Position pos;
            pos.Relocate(i_x, i_y, i_z);
            unit->MoveBlink(pos, travel_distance, decided_movement_angle);
            unit->SetFacingTo(unit->GetAngle(&pos));//the movement itself doesn't seem to update the server-side orientation, so this is needed.

            PathGenerator path(unit);
            path.SetPathLengthLimit(30.0f);
            bool result = path.CalculatePath(pos.m_positionX, pos.m_positionY, pos.m_positionZ);
            if (!result || (path.GetPathType() & PATHFIND_NOPATH))
            {
                i_nextMoveTime.Reset(100);
                return true;
            }

            Movement::MoveSplineInit init(unit);
            init.MovebyPath(path.GetPath());
            init.SetVelocity(6.00f);
            init.Launch();
        }
    }

    return true;
}

template<>
void ConfusedMovementGenerator<Player>::DoFinalize(Player* unit)
{
    unit->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_CONFUSED);
    unit->ClearUnitState(UNIT_STATE_CONFUSED | UNIT_STATE_CONFUSED_MOVE);
    unit->StopMoving();
}

template<>
void ConfusedMovementGenerator<Creature>::DoFinalize(Creature* unit)
{
    unit->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_CONFUSED);
    unit->ClearUnitState(UNIT_STATE_CONFUSED | UNIT_STATE_CONFUSED_MOVE);
    if (unit->getVictim())
        unit->SetTarget(unit->getVictim()->GetGUID());
}

template void ConfusedMovementGenerator<Player>::DoInitialize(Player*);
template void ConfusedMovementGenerator<Creature>::DoInitialize(Creature*);
template void ConfusedMovementGenerator<Player>::DoReset(Player*);
template void ConfusedMovementGenerator<Creature>::DoReset(Creature*);
template bool ConfusedMovementGenerator<Player>::DoUpdate(Player*, uint32 diff);
template bool ConfusedMovementGenerator<Creature>::DoUpdate(Creature*, uint32 diff);
