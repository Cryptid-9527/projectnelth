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

#ifndef __BATTLEGROUNDAB_H
#define __BATTLEGROUNDAB_H

#include "Battleground.h"

enum BG_AB_WorldStates
{
    BG_AB_OP_OCCUPIED_BASES_HORDE       = 1778,
    BG_AB_OP_OCCUPIED_BASES_ALLY        = 1779,
    BG_AB_OP_RESOURCES_ALLY             = 1776,
    BG_AB_OP_RESOURCES_HORDE            = 1777,
    BG_AB_OP_RESOURCES_MAX              = 1780,
    BG_AB_OP_RESOURCES_WARNING          = 1955
/*
    BG_AB_OP_STABLE_ICON                = 1842,             //Stable map icon (NONE)
    BG_AB_OP_STABLE_STATE_ALIENCE       = 1767,             //Stable map state (ALIENCE)
    BG_AB_OP_STABLE_STATE_HORDE         = 1768,             //Stable map state (HORDE)
    BG_AB_OP_STABLE_STATE_CON_ALI       = 1769,             //Stable map state (CON ALIENCE)
    BG_AB_OP_STABLE_STATE_CON_HOR       = 1770,             //Stable map state (CON HORDE)
    BG_AB_OP_FARM_ICON                  = 1845,             //Farm map icon (NONE)
    BG_AB_OP_FARM_STATE_ALIENCE         = 1772,             //Farm state (ALIENCE)
    BG_AB_OP_FARM_STATE_HORDE           = 1773,             //Farm state (HORDE)
    BG_AB_OP_FARM_STATE_CON_ALI         = 1774,             //Farm state (CON ALIENCE)
    BG_AB_OP_FARM_STATE_CON_HOR         = 1775,             //Farm state (CON HORDE)

    BG_AB_OP_BLACKSMITH_ICON            = 1846,             //Blacksmith map icon (NONE)
    BG_AB_OP_BLACKSMITH_STATE_ALIENCE   = 1782,             //Blacksmith map state (ALIENCE)
    BG_AB_OP_BLACKSMITH_STATE_HORDE     = 1783,             //Blacksmith map state (HORDE)
    BG_AB_OP_BLACKSMITH_STATE_CON_ALI   = 1784,             //Blacksmith map state (CON ALIENCE)
    BG_AB_OP_BLACKSMITH_STATE_CON_HOR   = 1785,             //Blacksmith map state (CON HORDE)
    BG_AB_OP_LUMBERMILL_ICON            = 1844,             //Lumber Mill map icon (NONE)
    BG_AB_OP_LUMBERMILL_STATE_ALIENCE   = 1792,             //Lumber Mill map state (ALIENCE)
    BG_AB_OP_LUMBERMILL_STATE_HORDE     = 1793,             //Lumber Mill map state (HORDE)
    BG_AB_OP_LUMBERMILL_STATE_CON_ALI   = 1794,             //Lumber Mill map state (CON ALIENCE)
    BG_AB_OP_LUMBERMILL_STATE_CON_HOR   = 1795,             //Lumber Mill map state (CON HORDE)
    BG_AB_OP_GOLDMINE_ICON              = 1843,             //Gold Mine map icon (NONE)
    BG_AB_OP_GOLDMINE_STATE_ALIENCE     = 1787,             //Gold Mine map state (ALIENCE)
    BG_AB_OP_GOLDMINE_STATE_HORDE       = 1788,             //Gold Mine map state (HORDE)
    BG_AB_OP_GOLDMINE_STATE_CON_ALI     = 1789,             //Gold Mine map state (CON ALIENCE
    BG_AB_OP_GOLDMINE_STATE_CON_HOR     = 1790,             //Gold Mine map state (CON HORDE)
*/
};

const uint32 BG_AB_OP_NODESTATES[5] =    {1767, 1782, 1772, 1792, 1787};

const uint32 BG_AB_OP_NODEICONS[5]  =    {1842, 1846, 1845, 1844, 1843};

/* Note: code uses that these IDs follow each other */
enum BG_AB_NodeObjectId
{
    BG_AB_OBJECTID_NODE_BANNER_0        = 180087,       // Stables banner
    BG_AB_OBJECTID_NODE_BANNER_1        = 180088,       // Blacksmith banner
    BG_AB_OBJECTID_NODE_BANNER_2        = 180089,       // Farm banner
    BG_AB_OBJECTID_NODE_BANNER_3        = 180090,       // Lumber mill banner
    BG_AB_OBJECTID_NODE_BANNER_4        = 180091,        // Gold mine banner

    BG_AB_RBG_OBJECTID_NODE_BANNER_0    = 406000,       // Stables banner
    BG_AB_RBG_OBJECTID_NODE_BANNER_1    = 406001,       // Blacksmith banner
    BG_AB_RBG_OBJECTID_NODE_BANNER_2    = 406002,       // Farm banner
    BG_AB_RBG_OBJECTID_NODE_BANNER_3    = 406003,       // Lumber mill banner
    BG_AB_RBG_OBJECTID_NODE_BANNER_4    = 406004        // Gold mine banner
};

enum BG_AB_ObjectType
{
    // for all 5 node points 8*5=40 objects
    BG_AB_OBJECT_BANNER_NEUTRAL          = 0,
    BG_AB_OBJECT_BANNER_CONT_A           = 1,
    BG_AB_OBJECT_BANNER_CONT_H           = 2,
    BG_AB_OBJECT_BANNER_ALLY             = 3,
    BG_AB_OBJECT_BANNER_HORDE            = 4,
    BG_AB_OBJECT_AURA_ALLY               = 5,
    BG_AB_OBJECT_AURA_HORDE              = 6,
    BG_AB_OBJECT_AURA_CONTESTED          = 7,
    //gates
    BG_AB_OBJECT_GATE_A                  = 40,
    BG_AB_OBJECT_GATE_H                  = 41,
    //buffs
    BG_AB_OBJECT_SPEEDBUFF_STABLES       = 42,
    BG_AB_OBJECT_REGENBUFF_STABLES       = 43,
    BG_AB_OBJECT_BERSERKBUFF_STABLES     = 44,
    BG_AB_OBJECT_SPEEDBUFF_BLACKSMITH    = 45,
    BG_AB_OBJECT_REGENBUFF_BLACKSMITH    = 46,
    BG_AB_OBJECT_BERSERKBUFF_BLACKSMITH  = 47,
    BG_AB_OBJECT_SPEEDBUFF_FARM          = 48,
    BG_AB_OBJECT_REGENBUFF_FARM          = 49,
    BG_AB_OBJECT_BERSERKBUFF_FARM        = 50,
    BG_AB_OBJECT_SPEEDBUFF_LUMBER_MILL   = 51,
    BG_AB_OBJECT_REGENBUFF_LUMBER_MILL   = 52,
    BG_AB_OBJECT_BERSERKBUFF_LUMBER_MILL = 53,
    BG_AB_OBJECT_SPEEDBUFF_GOLD_MINE     = 54,
    BG_AB_OBJECT_REGENBUFF_GOLD_MINE     = 55,
    BG_AB_OBJECT_BERSERKBUFF_GOLD_MINE   = 56,
    BG_AB_OBJECT_MAX                     = 57,   //non-switching objects

    FARM_PUMPKIN_BIG_1 = 58,
    FARM_PUMPKIN_BIG_2 = 59,
    FARM_PUMPKIN_BIG_3 = 60,
    FARM_PUMPKIN_BIG_4 = 61,
    FARM_PUMPKIN_1 = 62,
    FARM_PUMPKIN_2 = 63,
    FARM_PUMPKIN_3 = 64,
    FARM_PUMPKIN_4 = 65,
    FARM_PUMPKIN_5 = 66,
    FARM_PUMPKIN_6 = 67,
    FARM_PUMPKIN_7 = 68,
    FARM_PUMPKIN_8 = 69,

    FARM_FLOWER_BIG_1 = 70,
    FARM_FLOWER_BIG_2 = 71,
    FARM_FLOWER_BIG_3 = 72,
    FARM_FLOWER_BIG_4 = 73,
    FARM_FLOWER_1 = 74,
    FARM_FLOWER_2 = 75,
    FARM_FLOWER_3 = 76,
    FARM_FLOWER_4 = 77,
    FARM_FLOWER_5 = 78,
    FARM_FLOWER_6 = 79,
    FARM_FLOWER_7 = 80,
    FARM_FLOWER_8 = 81,
    BS_ANVIL_1 = 82,
    BS_ANVIL_2 = 83,

    BG_BASE_OBJECT_MAX = 84
};

/* Object id templates from DB */
enum BG_AB_ObjectTypes
{
    BG_AB_OBJECTID_BANNER_A             = 180058,
    BG_AB_OBJECTID_BANNER_CONT_A        = 180059,
    BG_AB_OBJECTID_BANNER_H             = 180060,
    BG_AB_OBJECTID_BANNER_CONT_H        = 180061,

    BG_AB_RBG_OBJECTID_BANNER_A         = 406005,
    BG_AB_RBG_OBJECTID_BANNER_CONT_A    = 406006,
    BG_AB_RBG_OBJECTID_BANNER_H         = 406007,
    BG_AB_RBG_OBJECTID_BANNER_CONT_H    = 406008,

    BG_AB_OBJECTID_AURA_A               = 180100,
    BG_AB_OBJECTID_AURA_H               = 180101,
    BG_AB_OBJECTID_AURA_C               = 180102,

    BG_AB_OBJECTID_GATE_A               = 180255,
    BG_AB_OBJECTID_GATE_H               = 180256,

    Arathi_Basin_bone02 = 180222,
    ArathiPlant01 = 401145,
    Arathi_basin_doomweed01 = 180226,//more colorful, has a mouth at the top
    ArathiRock02 = 401645,
    Arathi_cannon_fire = 113529,
    Arathi_Basin_Pumpkin = 180218,
    Arathi_Basin_PumpkinPatch = 180219,
    Arathi_Basin_Bone01 = 180220,
    Arathi_Basin_Skull = 180223,
    Arathi_Basin_BonePile2 = 180224,
    Arathi_Basin_BonePile3 = 180225,
    Arathi_Basin_Gloomweed01 = 180227,//almost the same as doomweed
    ArathiPlant02 = 401661,
    ArathiPlant04 = 401662,
    ArathiPlant05 = 401663,
    DwarvenAnvil = 208251
};

enum BG_AB_Timers
{
    BG_AB_FLAG_CAPTURING_TIME           = 60000
};

enum BG_AB_Score
{
    BG_AB_WARNING_NEAR_VICTORY_SCORE    = 1400,
    BG_AB_MAX_TEAM_SCORE                = 1600
};


enum BG_AB_NodeStatus
{
    BG_AB_NODE_TYPE_NEUTRAL             = 0,
    BG_AB_NODE_TYPE_CONTESTED           = 1,
    BG_AB_NODE_STATUS_ALLY_CONTESTED    = 1,
    BG_AB_NODE_STATUS_HORDE_CONTESTED   = 2,
    BG_AB_NODE_TYPE_OCCUPIED            = 3,
    BG_AB_NODE_STATUS_ALLY_OCCUPIED     = 3,
    BG_AB_NODE_STATUS_HORDE_OCCUPIED    = 4
};

enum BG_AB_Sounds
{
    BG_AB_SOUND_NODE_CAPTURED_ALLIANCE  = 8173,
    BG_AB_SOUND_NODE_CAPTURED_HORDE     = 8213,
    BG_AB_SOUND_NODE_ASSAULTED_ALLIANCE = 8212,
    BG_AB_SOUND_NODE_ASSAULTED_HORDE    = 8174,
    BG_AB_SOUND_NEAR_VICTORY            = 8456
};

enum BG_AB_Objectives
{
    AB_OBJECTIVE_ASSAULT_BASE           = 122,
    AB_OBJECTIVE_DEFEND_BASE            = 123
};

enum BG_AB_NPCs //1 of 4 Actual NPC labels we will use in the code, master refererence.
{
    BG_AB_NODE_STABLES = 0,
    BG_AB_NODE_BLACKSMITH = 1,
    BG_AB_NODE_FARM = 2,
    BG_AB_NODE_LUMBER_MILL = 3,
    BG_AB_NODE_GOLD_MINE = 4,
    BG_AB_DYNAMIC_NODES_COUNT = 5,    // dynamic nodes that can be captured
    BG_AB_SPIRIT_ALIANCE = 5,    //safe double use because the count should never be referenced to spawn something.
    BG_AB_SPIRIT_HORDE = 6,

    BG_AB_ALL_NODES_COUNT = 7,  //safe double use because the count should never be referenced to spawn something.

    BG_AB_TRIGGER_STABLES = 7,
    BG_AB_TRIGGER_BLACKSMITH,
    BG_AB_TRIGGER_FARM,
    BG_AB_TRIGGER_LUMBER_MILL,
    BG_AB_TRIGGER_GOLD_MINE,

    BG_AB_HORSE_1_ALLIANCE = 12,//start stables
    BG_AB_HORSE_2_ALLIANCE,
    BG_AB_HORSE_3_ALLIANCE,
    BG_AB_HORSE_4_ALLIANCE,
    BG_AB_HORSE_5_ALLIANCE,
    BG_AB_HORSE_6_ALLIANCE,
    BG_AB_HORSE_7_ALLIANCE,
    BG_AB_HORSE_8_ALLIANCE,
    BG_AB_STABLEHAND_ALLIANCE,

    BG_AB_BS_NPC_1_ALLIANCE = 21,//Blacksmiths
    BG_AB_BS_NPC_2_ALLIANCE,
    BG_AB_BS_NPC_3_ALLIANCE,

    BG_AB_LUMBERJACK_1_ALLIANCE = 24,//Lumberjacks
    BG_AB_LUMBERJACK_2_ALLIANCE,
    BG_AB_LUMBERJACK_3_ALLIANCE,
    BG_AB_LUMBERJACK_4_ALLIANCE,
    BG_AB_LUMBERJACK_5_ALLIANCE,

    BG_AB_MINER_1_ALLIANCE = 29,//miners
    BG_AB_MINER_2_ALLIANCE,
    BG_AB_MINER_3_ALLIANCE,
    BG_AB_MINER_4_ALLIANCE,

    BG_AB_FARMER_1_ALLIANCE = 33,//farmers
    BG_AB_FARMER_2_ALLIANCE,
    //new set for horde

    BG_AB_HORSE_1_HORDE = 35, //stable
    BG_AB_HORSE_2_HORDE,
    BG_AB_HORSE_3_HORDE,
    BG_AB_HORSE_4_HORDE,
    BG_AB_HORSE_5_HORDE,
    BG_AB_HORSE_6_HORDE,
    BG_AB_HORSE_7_HORDE,
    BG_AB_HORSE_8_HORDE,
    BG_AB_STABLEHAND_HORDE,

    BG_AB_BS_NPC_1_HORDE = 44, //blacksmith
    BG_AB_BS_NPC_2_HORDE,
    BG_AB_BS_NPC_3_HORDE,

    BG_AB_LUMBERJACK_1_HORDE = 47, //lumberjacks
    BG_AB_LUMBERJACK_2_HORDE,
    BG_AB_LUMBERJACK_3_HORDE,
    BG_AB_LUMBERJACK_4_HORDE,
    BG_AB_LUMBERJACK_5_HORDE,

    BG_AB_MINER_1_HORDE = 52, //miners
    BG_AB_MINER_2_HORDE,
    BG_AB_MINER_3_HORDE,
    BG_AB_MINER_4_HORDE,

    BG_AB_FARMER_1_HORDE = 56, //farmers
    BG_AB_FARMER_2_HORDE,

    BG_AB_CRITTER_SET_1,
    BG_AB_CRITTER_SET_2,
    BG_AB_CRITTER_SET_3,

    BG_AB_NPC_MAX = 61
};

enum ab_npc //where NPC entry IDs are given a label
{
    BG_AB_HORSE_ALLIANCE = 15107,
    BG_AB_HORSE_HORDE = 15108,

    BG_AB_STABLENPC_ALLIANCE = 15086,
    BG_AB_STABLENPC_HORDE = 15087,

    BG_AB_BS_NPC_ALLIANCE = 15063,
    BG_AB_BS_NPC_HORDE = 15064,//

    BG_AB_LUMBERJACK_ALLIANCE = 15062,
    BG_AB_LUMBERJACK_HORDE = 15089,//

    BG_AB_MINER_ALLIANCE = 15074,
    BG_AB_MINER_HORDE = 15075,

    BG_AB_FARMER_ALLIANCE = 15045,
    BG_AB_FARMER_HORDE = 15046,

    BG_AB_Critter_1 = 14886,
    BG_AB_Critter_2 = 6653,
    BG_AB_Critter_3 = 883
};

#define BG_AB_NotABBGWeekendHonorTicks      260
#define BG_AB_ABBGWeekendHonorTicks         160
#define BG_AB_NotABBGWeekendReputationTicks 160
#define BG_AB_ABBGWeekendReputationTicks    120

#define AB_EVENT_START_BATTLE               9158 // Achievement: Let's Get This Done

// x, y, z, o
const float BG_AB_NodePositions[BG_AB_DYNAMIC_NODES_COUNT][4] =
{
    {1166.785f, 1200.132f, -56.70859f, 0.9075713f},         // stables
    {977.0156f, 1046.616f, -44.80923f, -2.600541f},         // blacksmith
    {806.1821f, 874.2723f, -55.99371f, -2.303835f},         // farm
    {856.1419f, 1148.902f, 11.18469f, -2.303835f},          // lumber mill
    {1146.923f, 848.1782f, -110.917f, -0.7330382f}          // gold mine
};

// x, y, z, o, rot0, rot1, rot2, rot3
const float BG_AB_DoorPositions[2][8] =
{
    {1284.597f, 1281.167f, -15.97792f, 0.7068594f, 0.012957f, -0.060288f, 0.344959f, 0.93659f},
    {708.0903f, 708.4479f, -17.8342f, -2.391099f, 0.050291f, 0.015127f, 0.929217f, -0.365784f}
};

// Tick intervals and given points: case 0, 1, 2, 3, 4, 5 captured nodes
const uint32 BG_AB_TickIntervals[6] = {0, 12000, 9000, 6000, 3000, 1000};
const uint32 BG_AB_TickPoints[6] = {0, 10, 10, 10, 10, 30};

// WorldSafeLocs ids for 5 nodes, and for ally, and horde starting location
const uint32 BG_AB_GraveyardIds[BG_AB_ALL_NODES_COUNT] = {895, 894, 893, 897, 896, 898, 899};

// x, y, z, o
const float BG_AB_BuffPositions[BG_AB_DYNAMIC_NODES_COUNT][4] =
{
    {1185.71f, 1185.24f, -56.36f, 2.56f},                   // stables
    {990.75f, 1008.18f, -42.60f, 2.43f},                    // blacksmith
    {817.66f, 843.34f, -56.54f, 3.01f},                     // farm
    {807.46f, 1189.16f, 11.92f, 5.44f},                     // lumber mill
    {1146.62f, 816.94f, -98.49f, 6.14f}                     // gold mine
};

// x, y, z, o
const float BG_AB_SpiritGuidePos[BG_AB_ALL_NODES_COUNT][4] =
{
    {1200.03f, 1171.09f, -56.47f, 5.15f},                   // stables
    {1017.43f, 960.61f, -42.95f, 4.88f},                    // blacksmith
    {833.00f, 793.00f, -57.25f, 5.27f},                     // farm
    {775.17f, 1206.40f, 15.79f, 1.90f},                     // lumber mill
    {1207.48f, 787.00f, -83.36f, 5.51f},                    // gold mine
    {1354.05f, 1275.48f, -11.30f, 4.77f},                   // alliance starting base
    {714.61f, 646.15f, -10.87f, 4.34f}                      // horde starting base
};

struct BG_AB_BannerTimer
{
    uint32      timer;
    uint8       type;
    uint8       teamIndex;
};

struct BattlegroundABScore : public BattlegroundScore
{
    BattlegroundABScore(): BasesAssaulted(0), BasesDefended(0) { }
    ~BattlegroundABScore() { }
    uint32 BasesAssaulted;
    uint32 BasesDefended;
    uint32 GetAttr1() const { return BasesAssaulted; }
    uint32 GetAttr2() const { return BasesDefended; }
};

class BattlegroundAB : public Battleground
{
    public:
        BattlegroundAB();
        ~BattlegroundAB();

        void AddPlayer(Player* player);
        void StartingEventCloseDoors();
        void StartingEventOpenDoors();
        void RemovePlayer(Player* player, uint64 guid, uint32 team);
        void HandleAreaTrigger(Player* Source, uint32 Trigger);
        bool SetupBattleground();
        void Reset();
        void EndBattleground(uint32 winner);
        WorldSafeLocsEntry const* GetClosestGraveYard(Player* player);

        /* Scorekeeping */
        void UpdatePlayerScore(Player* Source, uint32 type, uint32 value, bool doAddHonor = true);

        void FillInitialWorldStates(WorldPacket& data);

        /* Nodes occupying */
        //TODO: BEFORE THE FLAG CAST
        //after the flag cast is done
        void EventPlayerClickedOnFlag(Player* source, GameObject* target_obj);

        /* achievement req. */
        bool IsAllNodesConrolledByTeam(uint32 team) const;  // overwrited
        bool IsTeamScores500Disadvantage(uint32 team) const { return m_TeamScores500Disadvantage[GetTeamIndexByTeamId(team)]; }

        uint32 GetPrematureWinner();
    private:
        void PostUpdateImpl(uint32 diff);
        /* Gameobject spawning/despawning */
        void _CreateBanner(uint8 node, uint8 type, uint8 teamIndex, bool delay);
        void _DelBanner(uint8 node, uint8 type, uint8 teamIndex);
        void _SendNodeUpdate(uint8 node);

        /* Creature spawning/despawning */
        // TODO: working, scripted peons spawning
        void _NodeOccupied(uint8 node, Team team);
        void _NodeDeOccupied(uint8 node);

        int32 _GetNodeNameId(uint8 node);

        /* Nodes info:
            0: neutral
            1: ally contested
            2: horde contested
            3: ally occupied
            4: horde occupied     */
        uint8               m_Nodes[BG_AB_DYNAMIC_NODES_COUNT];
        uint8               m_prevNodes[BG_AB_DYNAMIC_NODES_COUNT];
        BG_AB_BannerTimer   m_BannerTimers[BG_AB_DYNAMIC_NODES_COUNT];
        uint32              m_NodeTimers[BG_AB_DYNAMIC_NODES_COUNT];
        uint32              m_lastTick[BG_TEAMS_COUNT];
        uint32              m_HonorScoreTics[BG_TEAMS_COUNT];
        uint32              m_ReputationScoreTics[BG_TEAMS_COUNT];
        bool                m_IsInformedNearVictory;
        uint32              m_HonorTics;
        uint32              m_ReputationTics;


        //we will will switch these values from 0 to 1 to begin a 5s and 10s timer. 
        //(0=inactive, 1=pending first, 2 = pending second -> 0 to reset to inactive again)
        uint8               delayed_pop_BG_AB_NODE_LUMBER_MILL_ALLY;
        uint8               delayed_pop_BG_AB_NODE_FARM_ALLY;
        uint8               delayed_pop_BG_AB_NODE_BLACKSMITH_ALLY;
        uint8               delayed_pop_BG_AB_NODE_GOLD_MINE_ALLY;
        uint8               delayed_pop_BG_AB_NODE_STABLES_ALLY;

        uint16               delayed_pop_BG_AB_NODE_LUMBER_MILL_counter_ALLY;
        uint16               delayed_pop_BG_AB_NODE_FARM_counter_ALLY;
        uint16               delayed_pop_BG_AB_NODE_BLACKSMITH_counter_ALLY;
        uint16               delayed_pop_BG_AB_NODE_GOLD_MINE_counter_ALLY;
        uint16               delayed_pop_BG_AB_NODE_STABLES_counter_ALLY;
        //seperate set from horde.

        uint8               delayed_pop_BG_AB_NODE_LUMBER_MILL_HORDE;
        uint8               delayed_pop_BG_AB_NODE_FARM_HORDE;
        uint8               delayed_pop_BG_AB_NODE_BLACKSMITH_HORDE;
        uint8               delayed_pop_BG_AB_NODE_GOLD_MINE_HORDE;
        uint8               delayed_pop_BG_AB_NODE_STABLES_HORDE;

        uint16               delayed_pop_BG_AB_NODE_LUMBER_MILL_counter_HORDE;
        uint16               delayed_pop_BG_AB_NODE_FARM_counter_HORDE;
        uint16               delayed_pop_BG_AB_NODE_BLACKSMITH_counter_HORDE;
        uint16               delayed_pop_BG_AB_NODE_GOLD_MINE_counter_HORDE;
        uint16               delayed_pop_BG_AB_NODE_STABLES_counter_HORDE;

        // need for achievements
        bool                m_TeamScores500Disadvantage[BG_TEAMS_COUNT];

        struct NPC_DATA_FORMAT
        {
            uint32 type; //label
            uint32 entry; //entry
            uint32 team; //faction
            float x; //coordinate
            float y; //coordinate
            float z; //coordinate
            float o; //orientation
        };
        //the struct names each column of the const below. for example, AB_NPC_DATA[0].o returns 2.682593f.

        const NPC_DATA_FORMAT AB_NPC_DATA[BG_AB_NPC_MAX] = {//{LABEL, ENTRY ID, FACTION, X, Y, Z, O}
            {NULL,                          NULL,                       NULL,           NULL,         NULL,         NULL,        NULL},//these null values are empty slots to make the function line up with the index values referenced in the spawning function.
            {NULL,                          NULL,                       NULL,           NULL,         NULL,         NULL,        NULL},
            {NULL,                          NULL,                       NULL,           NULL,         NULL,         NULL,        NULL},
            {NULL,                          NULL,                       NULL,           NULL,         NULL,         NULL,        NULL},
            {NULL,                          NULL,                       NULL,           NULL,         NULL,         NULL,        NULL},
            {NULL,                          NULL,                       NULL,           NULL,         NULL,         NULL,        NULL},
            {NULL,                          NULL,                       NULL,           NULL,         NULL,         NULL,        NULL},
            {NULL,                          NULL,                       NULL,           NULL,         NULL,         NULL,        NULL},
            {NULL,                          NULL,                       NULL,           NULL,         NULL,         NULL,        NULL},
            {NULL,                          NULL,                       NULL,           NULL,         NULL,         NULL,        NULL},
            {NULL,                          NULL,                       NULL,           NULL,         NULL,         NULL,        NULL},
            {NULL,                          NULL,                       NULL,           NULL,         NULL,         NULL,        NULL},
            {BG_AB_HORSE_1_ALLIANCE,        BG_AB_HORSE_ALLIANCE,       TEAM_NEUTRAL,   1156.872803f, 1172.615234f, -56.364742f, 2.682593f},       //0, facing LM at trough
            {BG_AB_HORSE_2_ALLIANCE,        BG_AB_HORSE_ALLIANCE,       TEAM_NEUTRAL,   1163.303955f, 1139.414429f, -56.440754f, 4.076692f},       //1, facing BS at trough
            {BG_AB_HORSE_3_ALLIANCE,        BG_AB_HORSE_ALLIANCE,       TEAM_NEUTRAL,   1180.257813f, 1153.500977f, -56.368095f, 6.146216f},       //2, facing Back Wall at trough
            {BG_AB_HORSE_4_ALLIANCE,        BG_AB_HORSE_ALLIANCE,       TEAM_NEUTRAL,   1185.061279f, 1178.788574f, -56.364632f, 2.427359f},       //3, in stables, closest to BS
            {BG_AB_HORSE_5_ALLIANCE,        BG_AB_HORSE_ALLIANCE,       TEAM_NEUTRAL,   1187.603516f, 1181.055054f, -56.363232f, 2.427359f},       //4, in stables, 2nd closest to BS
            {BG_AB_HORSE_6_ALLIANCE,        BG_AB_HORSE_ALLIANCE,       TEAM_NEUTRAL,   1189.555054f, 1183.299072f, -56.363136f, 2.427359f},       //5, in stables, 3rd closest to BS
            {BG_AB_HORSE_7_ALLIANCE,        BG_AB_HORSE_ALLIANCE,       TEAM_NEUTRAL,   1186.766724f, 1190.340698f, -56.362778f, 2.305624f},       //6, in stables, 4th closest to BS
            {BG_AB_HORSE_8_ALLIANCE,        BG_AB_HORSE_ALLIANCE,       TEAM_NEUTRAL,   1169.471924f, 1159.760376f, -56.548706f, 3.216708f},       //6, middle of pen
            {BG_AB_STABLEHAND_ALLIANCE,     BG_AB_STABLENPC_ALLIANCE,   TEAM_NEUTRAL,   1154.748779f, 1163.783081f, -56.363182f, 5.890941f},       //7, Stablekeeper

            {BG_AB_BS_NPC_1_ALLIANCE,       BG_AB_BS_NPC_ALLIANCE,      TEAM_NEUTRAL,   969.554443f, 1000.256897f, -44.049091f, 2.843486f},       //8, person on anvil outside
            {BG_AB_BS_NPC_2_ALLIANCE,       BG_AB_BS_NPC_ALLIANCE,      TEAM_NEUTRAL,   997.058167f, 1002.788391f, -42.603142f, 3.958866f},       //9, person at northern anvil
            {BG_AB_BS_NPC_3_ALLIANCE,       BG_AB_BS_NPC_ALLIANCE,      TEAM_NEUTRAL,   985.678650f, 1008.797913f, -42.604332f, 3.789892f},       //10, person at southern anvil

            {BG_AB_LUMBERJACK_1_ALLIANCE,   BG_AB_LUMBERJACK_ALLIANCE,  TEAM_NEUTRAL,   815.957153f, 1089.814331f, 9.671382f, 5.537472f},          //11, lone woodcutter horde lady
            {BG_AB_LUMBERJACK_2_ALLIANCE,   BG_AB_LUMBERJACK_ALLIANCE,  TEAM_NEUTRAL,   756.144897f, 1158.086914f, 19.384485f, 2.062085f},         //12, duo cutters 1 of 2
            {BG_AB_LUMBERJACK_3_ALLIANCE,   BG_AB_LUMBERJACK_ALLIANCE,  TEAM_NEUTRAL,   757.735474f, 1164.242798f, 18.607395f, 3.719275f},         //13, duo cutters 2 of 2
            {BG_AB_LUMBERJACK_4_ALLIANCE,   BG_AB_LUMBERJACK_ALLIANCE,  TEAM_NEUTRAL,   820.423706f, 1203.587158f, 11.854614f, 3.491558f},         //14, someone can be by the mech repairing it
            {BG_AB_LUMBERJACK_5_ALLIANCE,   BG_AB_LUMBERJACK_ALLIANCE,  TEAM_NEUTRAL,   837.922363f, 1178.753662, 11.758116f, 4.551857f},          //15, someone can be by the wood pile

            {BG_AB_MINER_1_ALLIANCE,        BG_AB_MINER_ALLIANCE,       TEAM_NEUTRAL,   1210.646240f, 810.709900f, -102.552330f, 0.730908f},       //16, miner position 1
            {BG_AB_MINER_2_ALLIANCE,        BG_AB_MINER_ALLIANCE,       TEAM_NEUTRAL,   1236.392944f, 801.188965f, -102.896599f, 3.393436f},       //17, miner position 2
            {BG_AB_MINER_3_ALLIANCE,        BG_AB_MINER_ALLIANCE,       TEAM_NEUTRAL,   1248.184082f, 795.973999f, -102.698402f, 1.327840f},       //18, miner position 3
            {BG_AB_MINER_4_ALLIANCE,        BG_AB_MINER_ALLIANCE,       TEAM_NEUTRAL,   1231.205200f, 786.340332f, -102.835251f, 3.177456f},       //19, miner position 4

            {BG_AB_FARMER_1_ALLIANCE,       BG_AB_FARMER_ALLIANCE,      TEAM_NEUTRAL,   803.650513f, 810.837219f, -57.060204f, 4.241579f},         //20, farmer 1
            {BG_AB_FARMER_2_ALLIANCE,       BG_AB_FARMER_ALLIANCE,      TEAM_NEUTRAL,   848.723694f, 849.319092f, -57.961533f, 4.799200f},         //21, farmer 2
            //NEW SET FOR HORDE
            {BG_AB_HORSE_1_HORDE,           BG_AB_HORSE_HORDE,          TEAM_NEUTRAL,   1156.872803f, 1172.615234f, -56.364742f, 2.682593f},       //0, facing LM at trough
            {BG_AB_HORSE_2_HORDE,           BG_AB_HORSE_HORDE,          TEAM_NEUTRAL,   1163.303955f, 1139.414429f, -56.440754f, 4.076692f},       //1, facing BS at trough
            {BG_AB_HORSE_3_HORDE,           BG_AB_HORSE_HORDE,          TEAM_NEUTRAL,   1180.257813f, 1153.500977f, -56.368095f, 6.146216f},       //2, facing Back Wall at trough
            {BG_AB_HORSE_4_HORDE,           BG_AB_HORSE_HORDE,          TEAM_NEUTRAL,   1185.061279f, 1178.788574f, -56.364632f, 2.427359f},       //3, in stables, closest to BS
            {BG_AB_HORSE_5_HORDE,           BG_AB_HORSE_HORDE,          TEAM_NEUTRAL,   1187.603516f, 1181.055054f, -56.363232f, 2.427359f},       //4, in stables, 2nd closest to BS
            {BG_AB_HORSE_6_HORDE,           BG_AB_HORSE_HORDE,          TEAM_NEUTRAL,   1189.555054f, 1183.299072f, -56.363136f, 2.427359f},       //5, in stables, 3rd closest to BS
            {BG_AB_HORSE_7_HORDE,           BG_AB_HORSE_HORDE,          TEAM_NEUTRAL,   1186.766724f, 1190.340698f, -56.362778f, 2.305624f},       //6, in stables, 4th closest to BS
            {BG_AB_HORSE_8_HORDE,           BG_AB_HORSE_HORDE,          TEAM_NEUTRAL,   1169.471924f, 1159.760376f, -56.548706f, 3.216708f},       //6, middle of pen
            {BG_AB_STABLEHAND_HORDE,        BG_AB_STABLENPC_HORDE,      TEAM_NEUTRAL,   1154.748779f, 1163.783081f, -56.363182f, 5.890941f},       //7, Stablekeeper

            {BG_AB_BS_NPC_1_HORDE,          BG_AB_BS_NPC_HORDE,         TEAM_NEUTRAL,   969.554443f, 1000.256897f, -44.049091f, 2.843486f},        //8, person on anvil outside
            {BG_AB_BS_NPC_2_HORDE,          BG_AB_BS_NPC_HORDE,         TEAM_NEUTRAL,   997.058167f, 1002.788391f, -42.603142f, 3.958866f},        //9, person at northern anvil
            {BG_AB_BS_NPC_3_HORDE,          BG_AB_BS_NPC_HORDE,         TEAM_NEUTRAL,   985.678650f, 1008.797913f, -42.604332f, 3.789892f},        //10, person at southern anvil

            {BG_AB_LUMBERJACK_1_HORDE,      BG_AB_LUMBERJACK_HORDE,     TEAM_NEUTRAL,   815.957153f, 1089.814331f, 9.671382f, 5.537472f},          //11, lone woodcutter horde lady
            {BG_AB_LUMBERJACK_2_HORDE,      BG_AB_LUMBERJACK_HORDE,     TEAM_NEUTRAL,   756.144897f, 1158.086914f, 19.384485f, 2.062085f},         //12, duo cutters 1 of 2
            {BG_AB_LUMBERJACK_3_HORDE,      BG_AB_LUMBERJACK_HORDE,     TEAM_NEUTRAL,   757.735474f, 1164.242798f, 18.607395f, 3.719275f},         //13, duo cutters 2 of 2
            {BG_AB_LUMBERJACK_4_HORDE,      BG_AB_LUMBERJACK_HORDE,     TEAM_NEUTRAL,   820.423706f, 1203.587158f, 11.854614f, 3.491558f},         //14, someone can be by the mech repairing it
            {BG_AB_LUMBERJACK_5_HORDE,      BG_AB_LUMBERJACK_HORDE,     TEAM_NEUTRAL,   837.922363f, 1178.753662f, 11.758116f, 4.551857f},          //15, someone can be by the wood pile

            {BG_AB_MINER_1_HORDE,           BG_AB_MINER_HORDE,          TEAM_NEUTRAL,   1210.646240f, 810.709900f, -102.552330f, 0.730908f},       //16, miner position 1
            {BG_AB_MINER_2_HORDE,           BG_AB_MINER_HORDE,          TEAM_NEUTRAL,   1236.392944f, 801.188965f, -102.896599f, 3.393436f},       //17, miner position 2
            {BG_AB_MINER_3_HORDE,           BG_AB_MINER_HORDE,          TEAM_NEUTRAL,   1248.184082f, 795.973999f, -102.698402f, 1.327840f},       //18, miner position 3
            {BG_AB_MINER_4_HORDE,           BG_AB_MINER_HORDE,          TEAM_NEUTRAL,   1231.205200f, 786.340332f, -102.835251f, 3.177456f},       //19, miner position 4

            {BG_AB_FARMER_1_HORDE,          BG_AB_FARMER_HORDE,         TEAM_NEUTRAL,   803.650513f, 810.837219f, -57.060204f, 4.241579f},         //20, farmer 1
            {BG_AB_FARMER_2_HORDE,          BG_AB_FARMER_HORDE,         TEAM_NEUTRAL,   848.723694f, 849.319092f, -57.961533f, 4.799200f},          //21, farmer 2


            {BG_AB_CRITTER_SET_1,          BG_AB_Critter_1,            TEAM_NEUTRAL,   768.747986f, 775.810791f, -44.476887f, 2.419525f},          //21, Critter 1
            {BG_AB_CRITTER_SET_2,          BG_AB_Critter_2,            TEAM_NEUTRAL,   885.826599f, 1038.663940f, -67.252815f, 4.547945f},         //21, Critter 2
            {BG_AB_CRITTER_SET_3,          BG_AB_Critter_3,            TEAM_NEUTRAL,   1168.573853f, 1337.473633f, -29.598406f, 4.104183f},        //21, Critter 3
        };

        struct OBJECT_DATA_FORMAT
        {
            uint32 label; //label
            uint32 entryID; //entry
            float x; //coordinate
            float y; //coordinate
            float z; //coordinate
            float o; //orientation
        };

        const OBJECT_DATA_FORMAT AB_Object_DATA[BG_BASE_OBJECT_MAX] = {//{LABEL, ENTRY ID, FACTION, X, Y, Z, O}
        //farm
        //big pumpkins
        {FARM_PUMPKIN_BIG_1,    Arathi_Basin_PumpkinPatch,      846.687012, 879.076843, -58.362801, 3.950928},
        {FARM_PUMPKIN_BIG_2,    Arathi_Basin_PumpkinPatch,      815.266724, 860.134583, -57.717274, 5.788746},
        {FARM_PUMPKIN_BIG_3,    Arathi_Basin_PumpkinPatch,      836.169006, 843.885498, -58.366405, 5.985096},
        {FARM_PUMPKIN_BIG_4,    Arathi_Basin_PumpkinPatch,      825.179565, 816.845825, -57.778194, 1.732178},
        //little pumpkins
        {FARM_PUMPKIN_1,        Arathi_Basin_Pumpkin,           843.748962, 869.419556, -57.526936, 2.898494},
        {FARM_PUMPKIN_2,        Arathi_Basin_Pumpkin,           842.509949, 867.658813, -57.454838, 2.348722},
        {FARM_PUMPKIN_3,        Arathi_Basin_Pumpkin,           827.419556, 856.070923, -57.991207, 1.822505},
        {FARM_PUMPKIN_4,        Arathi_Basin_Pumpkin,           861.690247, 844.396118, -58.558830, 3.149822},
        {FARM_PUMPKIN_5,        Arathi_Basin_Pumpkin,           855.125122, 822.976685, -57.772415, 2.961326},
        {FARM_PUMPKIN_6,        Arathi_Basin_Pumpkin,           812.976013, 790.727112, -57.873489, 0.514815},
        {FARM_PUMPKIN_7,        Arathi_Basin_Pumpkin,           825.320679, 870.980713, -58.045189, 4.858161},
        {FARM_PUMPKIN_8,        Arathi_Basin_Pumpkin,           849.267456, 845.432495, -58.002743, 3.036028},

        {FARM_FLOWER_BIG_1,    Arathi_basin_doomweed01,        846.687012, 879.076843, -58.362801, 3.950928},
        {FARM_FLOWER_BIG_2,    Arathi_basin_doomweed01,        815.266724, 860.134583, -57.717274, 5.788746},
        {FARM_FLOWER_BIG_3,    Arathi_basin_doomweed01,        836.169006, 843.885498, -58.366405, 5.985096},
        {FARM_FLOWER_BIG_4,    Arathi_basin_doomweed01,        825.179565, 816.845825, -57.778194, 1.732178},
        //little flowers
        {FARM_FLOWER_1,        Arathi_Basin_Gloomweed01,       843.748962, 869.419556, -57.526936, 2.898494},
        {FARM_FLOWER_2,        Arathi_Basin_Gloomweed01,       842.509949, 867.658813, -57.454838, 2.348722},
        {FARM_FLOWER_3,        Arathi_Basin_Gloomweed01,       827.419556, 856.070923, -57.991207, 1.822505},
        {FARM_FLOWER_4,        Arathi_Basin_Gloomweed01,       861.690247, 844.396118, -58.558830, 3.149822},
        {FARM_FLOWER_5,        Arathi_Basin_Gloomweed01,       855.125122, 822.976685, -57.772415, 2.961326},
        {FARM_FLOWER_6,        Arathi_Basin_Gloomweed01,       812.976013, 790.727112, -57.873489, 0.514815},
        {FARM_FLOWER_7,        Arathi_Basin_Gloomweed01,       825.320679, 870.980713, -58.045189, 4.858161},
        {FARM_FLOWER_8,        Arathi_Basin_Gloomweed01,       849.267456, 845.432495, -58.002743, 3.036028},

        {BS_ANVIL_1,            DwarvenAnvil,                  984.403320, 1007.954224, -42.605061, 5.411874},
        {BS_ANVIL_2,            DwarvenAnvil,                  996.049316, 1001.867798, -42.605061, 5.517890}
        };

        /*
        Once a base is captured, there is a 5 second delay before the first set of environmental pieces are placed.
        After a 2nd 5-sec delay, a set of "larger" npcs and objects are spawned.
        Whenever a base is assaulted, all of the objects and NPCs are stripped clean.
        */
        void SPAWN_BS_HORDE_1();
        void SPAWN_BS_HORDE_2();
        void PURGE_BS_HORDE();
        void SPAWN_BS_ALLIANCE_1();
        void SPAWN_BS_ALLIANCE_2();
        void PURGE_BS_ALLIANCE();

        void SPAWN_LM_HORDE_1();
        void SPAWN_LM_HORDE_2();
        void PURGE_LM_HORDE();
        void SPAWN_LM_ALLIANCE_1();
        void SPAWN_LM_ALLIANCE_2();
        void PURGE_LM_ALLIANCE();

        void SPAWN_ST_HORDE_1();
        void SPAWN_ST_HORDE_2();
        void PURGE_ST_HORDE();
        void SPAWN_ST_ALLIANCE_1();
        void SPAWN_ST_ALLIANCE_2();
        void PURGE_ST_ALLIANCE();

        void SPAWN_GM_HORDE_1();
        void SPAWN_GM_HORDE_2();
        void PURGE_GM_HORDE();
        void SPAWN_GM_ALLIANCE_1();
        void SPAWN_GM_ALLIANCE_2();
        void PURGE_GM_ALLIANCE();

        void SPAWN_FM_HORDE_1();
        void SPAWN_FM_HORDE_2();
        void PURGE_FM_HORDE();
        void SPAWN_FM_ALLIANCE_1();
        void SPAWN_FM_ALLIANCE_2();
        void PURGE_FM_ALLIANCE();


        void AB_creature_SpawnBetween(uint8 a, uint8 b);
        void AB_creature_PurgeBetween(uint8 a, uint8 b);
        void AB_objects_SpawnBetween(uint8 a, uint8 b);
        void AB_objects_PurgeBetween(uint8 a, uint8 b);
};
#endif
