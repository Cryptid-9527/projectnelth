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

#include "Battleground.h"
#include "BattlegroundRV.h"
#include "ObjectAccessor.h"
#include "Language.h"
#include "Player.h"
#include "WorldPacket.h"
#include "GameObject.h"
#include "Transport.h"
#include "ScriptMgr.h"
#include "GameObjectAI.h"

BattlegroundRV::BattlegroundRV()
{
    BgObjects.resize(BG_RV_OBJECT_MAX);

    StartDelayTimes[BG_STARTING_EVENT_FIRST]  = BG_START_DELAY_1M;
    StartDelayTimes[BG_STARTING_EVENT_SECOND] = BG_START_DELAY_30S;
    StartDelayTimes[BG_STARTING_EVENT_THIRD]  = BG_START_DELAY_15S;
    StartDelayTimes[BG_STARTING_EVENT_FOURTH] = BG_START_DELAY_NONE;
    StartMessageIds[BG_STARTING_EVENT_FIRST]  = LANG_ARENA_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_ARENA_THIRTY_SECONDS;
    StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_ARENA_FIFTEEN_SECONDS;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_ARENA_HAS_BEGUN;
}

BattlegroundRV::~BattlegroundRV() { }

void BattlegroundRV::PostUpdateImpl(uint32 diff) 
{ //Fired every 50 milliseconds, along with server-wide "update" setting.
    if (GetStatus() != STATUS_IN_PROGRESS) {
        return;   
        }
   
    if (getTimer() >= BG_RV_PILLAR_SWITCH_TIMER) 
    {
        SwapPillars();
        setTimer(0);
    }
    else 
    { 
        setTimer(getTimer() + 1); 
    }

}

void BattlegroundRV::StartingEventCloseDoors()
{
}

void BattlegroundRV::StartingEventOpenDoors()
{
    //pillars 2 and 4 should go from down to up here.
/*enum GOState //template
{
    GO_STATE_ACTIVE             = 0,                        // show in world as used and not reset (closed door open)
    GO_STATE_READY              = 1,                        // show in world as ready (closed door close)
    GO_STATE_ACTIVE_ALTERNATIVE = 2,                        // show in world as used in alt way and not reset (closed door open by cannon fire)
    GO_STATE_TRANSPORT_ACTIVE   = 24,                       // move Pillar to UP position
    GO_STATE_TRANSPORT_READY    = 25                        // move Pillar to DOWN position
};
*/


    // Buff respawn
   
    // buffs
    AddObject(BG_RV_OBJECT_BUFF_1, BG_RV_OBJECT_TYPE_BUFF_1, 735.551819f, -284.794678f, 28.276682f, 0.034906f, 0, 0, 0, 120);
    AddObject(BG_RV_OBJECT_BUFF_2, BG_RV_OBJECT_TYPE_BUFF_2, 791.224487f, -284.794464f, 28.276682f, 2.600535f, 0, 0, 0, 120);
    SpawnBGObject(BG_RV_OBJECT_BUFF_1, SHADOW_SIGHT_SPAWN_TIME);
    SpawnBGObject(BG_RV_OBJECT_BUFF_2, SHADOW_SIGHT_SPAWN_TIME);
    // Elevators
    DoorOpen(BG_RV_OBJECT_ELEVATOR_1);
    DoorOpen(BG_RV_OBJECT_ELEVATOR_2);

    setState(BG_RV_STATE_OPEN_FENCES);
    setTimer(BG_RV_FIRST_TIMER);

    // Should be false at first, TogglePillarCollision will do it.

    SetPillarCollision(true);

    SwapPillars();
    setTimer(0);
    setSwitchCount(0);
}

void BattlegroundRV::SwapPillars()
{
    switch (pillar_position)
    {
    case 0:
        pillar_position = 1;
        break;
    case 1:
        pillar_position = 0;
        break;
    case 2:
        pillar_position = int8(urand(0, 1));
        break;
    }

    bool two_four_up { (pillar_position == 1 ? true : false) };

    if (auto map = GetBgMap())
    for (uint8 i = BG_RV_OBJECT_PILAR_1; i <= BG_RV_OBJECT_PILAR_4; ++i)
        if (auto gob = map->GetGameObject(BgObjects[i]))
            switch (gob->GetEntry())
            {
                case BG_RV_OBJECT_TYPE_PILAR_2:
                case BG_RV_OBJECT_TYPE_PILAR_4:
                    gob->AI()->DoAction(two_four_up ? ACTION_MOVE_UP : ACTION_MOVE_DOWN);
                    break;
                case BG_RV_OBJECT_TYPE_PILAR_1:
                case BG_RV_OBJECT_TYPE_PILAR_3:
                    gob->AI()->DoAction(two_four_up ? ACTION_MOVE_DOWN : ACTION_MOVE_UP);
                    break;
                default:
                    TC_LOG_ERROR("sql.sql", "BattlegroundRV::SwapPillars() has gameobject with an unlisted entry: %u", gob->GetEntry());
                    break;
            }


}

void BattlegroundRV::AddPlayer(Player* player)
{
    Battleground::AddPlayer(player);
    BattlegroundScore* sc = new BattlegroundScore;
    sc->BgTeam = player->GetTeam();
    sc->TalentTree = player->GetPrimaryTalentTree(player->GetActiveSpec());
    PlayerScores[player->GetGUID()] = sc;

    UpdateWorldState(BG_RV_WORLD_STATE_A, GetAlivePlayersCountByTeam(ALLIANCE));
    UpdateWorldState(BG_RV_WORLD_STATE_H, GetAlivePlayersCountByTeam(HORDE));
}

void BattlegroundRV::RemovePlayer(Player* /*player*/, uint64 /*guid*/, uint32 /*team*/)
{
    if (GetStatus() == STATUS_WAIT_LEAVE)
        return;

    UpdateWorldState(BG_RV_WORLD_STATE_A, GetAlivePlayersCountByTeam(ALLIANCE));
    UpdateWorldState(BG_RV_WORLD_STATE_H, GetAlivePlayersCountByTeam(HORDE));

    CheckArenaWinConditions();
}

void BattlegroundRV::HandleKillPlayer(Player* player, Player* killer)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    if (killer)
        if (player)
            Battleground::HandleKillPlayer(player, killer);
        else TC_LOG_ERROR("bg.battleground", "BattlegroundRV::HandleKillPlayer: Killer player not found");
    else TC_LOG_ERROR("bg.battleground", "BattlegroundRV::HandleKillPlayer: Victim player not found");

    UpdateWorldState(BG_RV_WORLD_STATE_A, GetAlivePlayersCountByTeam(ALLIANCE));
    UpdateWorldState(BG_RV_WORLD_STATE_H, GetAlivePlayersCountByTeam(HORDE));

    CheckArenaWinConditions();
}

bool BattlegroundRV::HandlePlayerUnderMap(Player* player)
{
    player->TeleportTo(GetMapId(), 763.5f, -284, 28.276f, 2.422f);
    return true;
}

void BattlegroundRV::HandleAreaTrigger(Player* player, uint32 trigger)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    switch (trigger)
    {
        case 5224:
        case 5226:
        // fire was removed in 3.2.0
        case 5473:
        case 5474:
            break;
        default:
            Battleground::HandleAreaTrigger(player, trigger);
            break;
    }
}

void BattlegroundRV::FillInitialWorldStates(WorldPacket &data)
{
    data << uint32(BG_RV_WORLD_STATE_A) << uint32(GetAlivePlayersCountByTeam(ALLIANCE));
    data << uint32(BG_RV_WORLD_STATE_H) << uint32(GetAlivePlayersCountByTeam(HORDE));
    data << uint32(BG_RV_WORLD_STATE) << uint32(1);
}

void BattlegroundRV::Reset()
{
    //call parent's class reset
    Battleground::Reset();
}

bool BattlegroundRV::SetupBattleground()
{
    // elevators
    if (!AddObject(BG_RV_OBJECT_ELEVATOR_1, BG_RV_OBJECT_TYPE_DOOR, 819.438904f, -297.652557f, 28.682638f, 2.718719f, 0, 0, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_RV_OBJECT_ELEVATOR_2, BG_RV_OBJECT_TYPE_DOOR, 708.579773f, -269.124878f, 28.641962f, 5.716985f, 0, 0, 0, RESPAWN_IMMEDIATELY)
    // fire is actually a linear transport that we're not even going to touch.
        /*
        || !AddObject(BG_RV_OBJECT_FIRE_1, BG_RV_OBJECT_TYPE_FIRE_1, 743.543457f, -283.799469f, 28.286655f, 3.141593f, 0, 0, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_RV_OBJECT_FIRE_2, BG_RV_OBJECT_TYPE_FIRE_2, 782.971802f, -283.799469f, 28.286655f, 3.141593f, 0, 0, 0, RESPAWN_IMMEDIATELY)
        */
        || !AddObject(BG_RV_OBJECT_FIREDOOR_1, BG_RV_OBJECT_TYPE_FIREDOOR_1, 743.711060f, -284.099609f, 27.542587f, 3.141593f, 0, 0, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_RV_OBJECT_FIREDOOR_2, BG_RV_OBJECT_TYPE_FIREDOOR_2, 783.221252f, -284.133362f, 27.535686f, 0.000000f, 0, 0, 0, RESPAWN_IMMEDIATELY)
    // Gear
        || !AddObject(BG_RV_OBJECT_GEAR_1, BG_RV_OBJECT_TYPE_GEAR_1, 763.664551f, -261.872986f, 26.686588f, 0.000000f, 0, 0, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_RV_OBJECT_GEAR_2, BG_RV_OBJECT_TYPE_GEAR_2, 763.578979f, -306.146149f, 26.665222f, 3.141593f, 0, 0, 0, RESPAWN_IMMEDIATELY)
    // Pulley
        || !AddObject(BG_RV_OBJECT_PULLEY_1, BG_RV_OBJECT_TYPE_PULLEY_1, 700.722290f, -283.990662f, 39.517582f, 3.141593f, 0, 0, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_RV_OBJECT_PULLEY_2, BG_RV_OBJECT_TYPE_PULLEY_1, 826.303833f, -283.996429f, 39.517582f, 0.000000f, 0, 0, 0, RESPAWN_IMMEDIATELY)
    // Active-Level Pilars

        
        || !AddObject(BG_RV_OBJECT_PILAR_1,             BG_RV_OBJECT_TYPE_PILAR_1, 763.632385f, -306.162384f, 25.909504f - 0.01f, 3.141593f, 0, 0, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_RV_OBJECT_PILAR_2,             BG_RV_OBJECT_TYPE_PILAR_2, 723.644287f, -284.493256f, 24.648525f - 0.01f, 3.141593f, 0, 0, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_RV_OBJECT_PILAR_3,             BG_RV_OBJECT_TYPE_PILAR_3, 763.611145f, -261.856750f, 25.909504f - 0.01f, 0.000000f, 0, 0, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_RV_OBJECT_PILAR_4,             BG_RV_OBJECT_TYPE_PILAR_4, 802.211609f, -284.493256f, 24.648525f - 0.01f, 0.000000f, 0, 0, 0, RESPAWN_IMMEDIATELY)
        
        
    // "catch-fall" pillars
        /*
            1 and 3 are smalls
            4.730243 rise for small

            2 and 4 are bigs
            7.738 rise for big
        */
        || !AddObject(BG_RV_OBJECT_PILLAR_FALLCATCH_1,  BG_RV_OBJECT_SOLID_PILLAR_1, 763.632385f, -306.162384f, (25.909504f), 3.141593f, 0, 0, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_RV_OBJECT_PILLAR_FALLCATCH_2,  BG_RV_OBJECT_SOLID_PILLAR_2, 723.644287f, -284.493256f, (24.648525f), 3.141593f, 0, 0, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_RV_OBJECT_PILLAR_FALLCATCH_3,  BG_RV_OBJECT_SOLID_PILLAR_3, 763.611145f, -261.856750f, (25.909504f), 0.000000f, 0, 0, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_RV_OBJECT_PILLAR_FALLCATCH_4,  BG_RV_OBJECT_SOLID_PILLAR_4, 802.211609f, -284.493256f, (24.648525f), 0.000000f, 0, 0, 0, RESPAWN_IMMEDIATELY)
        
        || !AddObject(BG_RV_OBJECT_PILLAR_ELEVATED_1, BG_RV_OBJECT_SOLID_PILLAR_1B, 763.632385f, -306.162384f, (25.909504f + 4.730243f), 3.141593f, 0, 0, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_RV_OBJECT_PILLAR_ELEVATED_2, BG_RV_OBJECT_SOLID_PILLAR_2B, 723.644287f, -284.493256f, (24.648525f + 7.738f), 3.141593f, 0, 0, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_RV_OBJECT_PILLAR_ELEVATED_3, BG_RV_OBJECT_SOLID_PILLAR_3B, 763.611145f, -261.856750f, (25.909504f + 4.730243f), 0.000000f, 0, 0, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_RV_OBJECT_PILLAR_ELEVATED_4, BG_RV_OBJECT_SOLID_PILLAR_4B, 802.211609f, -284.493256f, (24.648525f + 7.738f), 0.000000f, 0, 0, 0, RESPAWN_IMMEDIATELY)
        
        )
    {
        TC_LOG_ERROR("sql.sql", "BatteGroundRV: Failed to spawn some object!");
        return false;
    }

    if (auto map = GetBgMap())
        for (uint8 i = BG_RV_OBJECT_PILAR_1; i <= BG_RV_OBJECT_PILAR_4; ++i)
            if (auto gob = map->GetGameObject(BgObjects[i]))
                if (auto gob2 = map->GetGameObject(BgObjects[(i + 4)]))
                    if (auto gob3 = map->GetGameObject(BgObjects[(i + 8)]))
                    {
                        gob->AI()->SetData64(DATA_CLONE_PILLAR_BOTTOM, gob2->GetGUID());
                        gob->AI()->SetData64(DATA_CLONE_PILLAR_TOP, gob3->GetGUID());
                    }
    


    if (auto map = GetBgMap())
        for (uint8 i = BG_RV_OBJECT_PILAR_1; i <= BG_RV_OBJECT_PILAR_4; ++i)
            if (auto gob = map->GetGameObject(BgObjects[i]))
                gob->SetGoState(GO_STATE_TRANSPORT_SPEC);

    // readymarkers
    if (sWorld->getBoolConfig(CONFIG_ARENA_READYMARK_ENABLED))
    {
        AddObject(BG_RV_OBJECT_READYMARKER_1, BG_RV_OBJECT_READYMARKER, 703.0f, -270.0f, 30.0f, 3.6f, 0, 0, 0, 0, RESPAWN_IMMEDIATELY);
        AddObject(BG_RV_OBJECT_READYMARKER_2, BG_RV_OBJECT_READYMARKER, 826.0f, -296.0f, 30.0f, 0.5f, 0, 0, 0, 0, RESPAWN_IMMEDIATELY);
    }
    return true;
}


class go_rov_wild_pillar : public GameObjectScript
{
public:
    go_rov_wild_pillar() : GameObjectScript("go_rov_wild_pillar") { }

    enum rov_events
    {
        EVENT_SUMMON_CLONES = 1,
        EVENT_MOVE_DOWN_REMOVE_CLONE_TOP,
        EVENT_MOVE_DOWN_MOVE,
        EVENT_MOVE_DOWN_ADD_CLONE_BOTTOM,

        EVENT_MOVE_UP_REMOVE_CLONE_BOTTOM,
        EVENT_MOVE_UP_MOVE,
        EVENT_MOVE_UP_ADD_CLONE_TOP,


    };
    struct go_rov_wild_pillarAI : public GameObjectAI
    {

        go_rov_wild_pillarAI(GameObject* go) : GameObjectAI(go)
        {
        }


        void DoAction(const int32 param)
        {
            switch (param)
            {
                case ACTION_MOVE_UP:
                {
                    events.ScheduleEvent(EVENT_MOVE_UP_REMOVE_CLONE_BOTTOM, 100);
                    break;
                }
                case ACTION_MOVE_DOWN:
                {
                    events.ScheduleEvent(EVENT_MOVE_DOWN_REMOVE_CLONE_TOP, 100);
                    break;
                }
            }
        }

        void SetData64(uint32 id, uint64 value)
        {
            switch (id)
            {
            case DATA_CLONE_PILLAR_TOP:
                if (auto go3 = go->GetGameObject(*go, value))
                {
                    go3->SetPhaseMask(0x2, true);
                    elevated = go3->GetGUID();
                    myElevated = go3;
                }
                break;
            case DATA_CLONE_PILLAR_BOTTOM:
                if (auto go2 = go->GetGameObject(*go, value))
                {
                    go2->SetPhaseMask(0x2, true);
                    floor = go2->GetGUID();
                    myfloor = go2;
                }
                break;
            }
        }

        void Reset()
        {
            events.ScheduleEvent(101, 5000);
        }

        void UpdateAI(uint32 diff)
        {
            if (!myfloor)
                myfloor = go->GetGameObject(*go, floor);
            if (!myElevated)
                myElevated = go->GetGameObject(*go, elevated);

            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
                switch (eventId)
                {
                    /*
                        BEGIN MOVE DOWN SEQUENCE
                    */
                case EVENT_MOVE_DOWN_REMOVE_CLONE_TOP:
                {
                    auto p = go->GetPlayersInRange(5.f, false);
                    for (auto player : p)
                        if (!player->isMoving())
                            player->SendTeleportPacket(*player);

                    myElevated->EnableCollision(false);
                    myElevated->SetPhaseMask(0x2, true);
                    events.ScheduleEvent(EVENT_MOVE_DOWN_MOVE, 1500);
                    break;
                }
                case EVENT_MOVE_DOWN_MOVE:                                                               //BEGIN 30S TIMER HERE
                    //go->MonsterSay("going down", LANG_UNIVERSAL, NULL);
                    go->SetGoState(GO_STATE_TRANSPORT_SPEC);
                    events.ScheduleEvent(EVENT_MOVE_DOWN_ADD_CLONE_BOTTOM, 1500);                            //1500 ms
                    break;
                case EVENT_MOVE_DOWN_ADD_CLONE_BOTTOM:
                    //go->MonsterSay("hardening floor", LANG_UNIVERSAL, NULL);
                    myfloor->SetPhaseMask(0x1, true);
                    myfloor->EnableCollision(true);
                    break;

                    /*
                        BEGIN MOVE UP SEQUENCE
                    */
                case EVENT_MOVE_UP_REMOVE_CLONE_BOTTOM:
                {
                    myfloor->EnableCollision(false);
                    myfloor->SetPhaseMask(0x2, true);
                    events.ScheduleEvent(EVENT_MOVE_UP_MOVE, 1500);                            //1500 ms
                    auto p = go->GetPlayersInRange(5.f, false);
                    for (auto player : p)
                        if (!player->isMoving())
                            player->SendTeleportPacket(*player);
                    break;
                }
                case EVENT_MOVE_UP_MOVE:
                    //go->MonsterSay("going up", LANG_UNIVERSAL, NULL);
                    go->SetGoState(GO_STATE_TRANSPORT_READY);
                    events.ScheduleEvent(EVENT_MOVE_UP_ADD_CLONE_TOP, 1500);
                    break;
                case EVENT_MOVE_UP_ADD_CLONE_TOP:
                    //go->MonsterSay("hardening elevated", LANG_UNIVERSAL, NULL);
                    myElevated->SetPhaseMask(0x1, true);
                    myElevated->EnableCollision(true);
                    break;
                }
        }

    private:
        EventMap events;
        uint64 floor{ 0 };
        uint64 elevated{ 0 };
        GameObject* myfloor{ nullptr };
        GameObject* myElevated{ nullptr };

    };

    GameObjectAI* GetAI(GameObject* go) const
    {
        return new go_rov_wild_pillarAI(go);
    }
};

void AddSc_bg_rov_scripts()
{
    new go_rov_wild_pillar;
}