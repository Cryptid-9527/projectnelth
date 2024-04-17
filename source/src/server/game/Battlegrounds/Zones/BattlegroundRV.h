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
#ifndef __BATTLEGROUNDRV_H
#define __BATTLEGROUNDRV_H

#include "Battleground.h"

enum BattlegroundRVObjectTypes
{
    BG_RV_OBJECT_BUFF_1,
    BG_RV_OBJECT_BUFF_2,
    BG_RV_OBJECT_FIRE_1,
    BG_RV_OBJECT_FIRE_2,
    BG_RV_OBJECT_FIREDOOR_1,
    BG_RV_OBJECT_FIREDOOR_2,

    //these are the sets of "connected" objects. all of these fire at the same time.

    BG_RV_OBJECT_PILAR_1,
    BG_RV_OBJECT_PILAR_3,
    BG_RV_OBJECT_PILAR_2,
    BG_RV_OBJECT_PILAR_4,
    
    BG_RV_OBJECT_PILLAR_FALLCATCH_1,
    BG_RV_OBJECT_PILLAR_FALLCATCH_2,
    BG_RV_OBJECT_PILLAR_FALLCATCH_3,
    BG_RV_OBJECT_PILLAR_FALLCATCH_4,

    BG_RV_OBJECT_PILLAR_ELEVATED_1,
    BG_RV_OBJECT_PILLAR_ELEVATED_2,
    BG_RV_OBJECT_PILLAR_ELEVATED_3,
    BG_RV_OBJECT_PILLAR_ELEVATED_4,
    
    

    BG_RV_OBJECT_GEAR_1,
    BG_RV_OBJECT_GEAR_2,

    BG_RV_OBJECT_PULLEY_1,
    BG_RV_OBJECT_PULLEY_2,

    //These objects exist, but in the wrong way. rather than toggling them we'll use their collision object to maintain the floor underneath the pillar.


    BG_RV_OBJECT_ELEVATOR_1,
    BG_RV_OBJECT_ELEVATOR_2,
    BG_RV_OBJECT_READYMARKER_1,
    BG_RV_OBJECT_READYMARKER_2,
    BG_RV_OBJECT_MAX
};

enum battleground_rv_misc
{
    ACTION_MOVE_UP = 500,
    ACTION_MOVE_DOWN,
    ACTION_MOVE_DOWN_INITIAL_ONLY,

    DATA_CLONE_PILLAR_TOP,
    DATA_CLONE_PILLAR_BOTTOM,
};

enum BattlegroundRVObjects
{
    BG_RV_OBJECT_TYPE_BUFF_1                     = 184663,
    BG_RV_OBJECT_TYPE_BUFF_2                     = 184664,
    BG_RV_OBJECT_TYPE_FIRE_1                     = 192704,
    BG_RV_OBJECT_TYPE_FIRE_2                     = 192705,

    BG_RV_OBJECT_TYPE_FIREDOOR_2                 = 192387,
    BG_RV_OBJECT_TYPE_FIREDOOR_1                 = 192388,
    BG_RV_OBJECT_TYPE_PULLEY_1                   = 192389,
    BG_RV_OBJECT_TYPE_DOOR                       = 192390,
    BG_RV_OBJECT_TYPE_GEAR_1                     = 192393,
    BG_RV_OBJECT_TYPE_GEAR_2                     = 192394,
    BG_RV_OBJECT_TYPE_ELEVATOR_1                 = 194582,
    BG_RV_OBJECT_TYPE_ELEVATOR_2                 = 194586,

    /*
I will keep these here for historical sake, however, we will not use them. these objects are cylindrical Barriers with no top to them.
Their purpose was to provide a second layer so that nobody could glitch their way inside the pillar.
I'm going to use a different approach and create duplicate pillars that do not move, and place them squarely underneath the originals.
This second layer of pillars will catch players who do manage to glitch underneath the original, and keep them at a manageable height beneath the map.*/

    BG_RV_OBJECT_TYPE_PILAR_COLLISION_1          = 194580, // axe
    BG_RV_OBJECT_TYPE_PILAR_COLLISION_2          = 194579, // wolf
    BG_RV_OBJECT_TYPE_PILAR_COLLISION_3          = 194581, // lightning
    BG_RV_OBJECT_TYPE_PILAR_COLLISION_4          = 194578, // horn
    

    /*BG_RV_OBJECT_TYPE_PILAR_COLLISION_1          = 208468, // axe
    BG_RV_OBJECT_TYPE_PILAR_COLLISION_2          = 208469, // wolf
    BG_RV_OBJECT_TYPE_PILAR_COLLISION_3          = 208470, // lightning
    BG_RV_OBJECT_TYPE_PILAR_COLLISION_4          = 208471,  // horn*/

    BG_RV_OBJECT_TYPE_PILAR_1                    = 208468, //axe
    BG_RV_OBJECT_SOLID_PILLAR_1                  = 1337001,//axe
    BG_RV_OBJECT_SOLID_PILLAR_1B                 = 1337011,//axe

    BG_RV_OBJECT_TYPE_PILAR_2                    = 208469, //wolf
    BG_RV_OBJECT_SOLID_PILLAR_2                  = 1337004, //wolf
    BG_RV_OBJECT_SOLID_PILLAR_2B                 = 1337014, //wolf

    BG_RV_OBJECT_TYPE_PILAR_3                    = 208470, //lightning
    BG_RV_OBJECT_SOLID_PILLAR_3                  = 1337002,//lightning
    BG_RV_OBJECT_SOLID_PILLAR_3B                 = 1337012,//lightning

    BG_RV_OBJECT_TYPE_PILAR_4                    = 208471,  // horn
    BG_RV_OBJECT_SOLID_PILLAR_4                  = 1337003, //horn
    BG_RV_OBJECT_SOLID_PILLAR_4B                 = 1337013, //horn

    BG_RV_OBJECT_READYMARKER                     = 940000
};

enum BattlegroundRVData
{
    BG_RV_STATE_OPEN_FENCES,
    BG_RV_STATE_SWITCH_PILLARS,
    BG_RV_STATE_CLOSE_FIRE,

    BG_RV_PILLAR_SWITCH_TIMER                    = 500, // x (50 ms)
    BG_RV_FIRE_TO_PILLAR_TIMER                   = 20000,
    BG_RV_CLOSE_FIRE_TIMER                       =  5000,
    BG_RV_FIRST_TIMER                            = 20133,
    BG_RV_WORLD_STATE_A                          = 0xe10,
    BG_RV_WORLD_STATE_H                          = 0xe11,
    BG_RV_WORLD_STATE                            = 0xe1a
};

class BattlegroundRV : public Battleground
{
    public:
        BattlegroundRV();
        ~BattlegroundRV();

        /* inherited from BattlegroundClass */
        void AddPlayer(Player* player);
        void StartingEventCloseDoors();
        void StartingEventOpenDoors();
        void Reset();
        void FillInitialWorldStates(WorldPacket &d);

        void RemovePlayer(Player* player, uint64 guid, uint32 team);
        void HandleAreaTrigger(Player* Source, uint32 Trigger);
        bool SetupBattleground();
        void HandleKillPlayer(Player* player, Player* killer);
        bool HandlePlayerUnderMap(Player* player);

    private:
        uint32 Timer;
        uint32 State;
        uint32 switchCount;
        bool   PillarCollision;
        int8 pillar_position{ 0 };

        void SwapPillars(); //new method to swap all pillars based on whether or not the "switchCount" is even.
        void PostUpdateImpl(uint32 diff);

    protected:
        uint32 getTimer() { return Timer; };
        void setTimer(uint32 timer) { Timer = timer; };

        uint32 getSwitchCount() { return switchCount; };
        void setSwitchCount(uint32 switches) { switchCount = switches; };

        uint32 getState() { return State; };
        void setState(uint32 state) { State = state; };
        bool GetPillarCollision() { return PillarCollision; }
        void SetPillarCollision(bool apply) { PillarCollision = apply; }
};
#endif
