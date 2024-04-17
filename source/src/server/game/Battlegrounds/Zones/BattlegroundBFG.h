/*
* Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
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

#ifndef __BATTLEGROUNDBG_H
#define __BATTLEGROUNDBG_H

#include "Battleground.h"

enum BG_BFG_Rewards
{
    BG_BFG_MAP_COMPLETE,
    BG_BFG_REWARD_NUM
};

enum BG_BFG_WorldStates
{
    BG_BFG_OP_OCCUPIED_BASES_HORDE  = 1778,
    BG_BFG_OP_OCCUPIED_BASES_ALLY   = 1779,
    BG_BFG_OP_RESOURCES_ALLY        = 1776,
    BG_BFG_OP_RESOURCES_HORDE       = 1777,
    BG_BFG_OP_RESOURCES_MAX         = 1780,
    BG_BFG_OP_RESOURCES_WARNING     = 1955
};
enum BG_BFG_EMOTE
{
    STATE_WORK_CHOPWOOD,
    STATE_WORK_MINING,
    ONESHOT_TALK_NOSHEATHE,
    STATE_USESTANDING
};

enum BG_BFG_NPC_IDs
{
    NPC_UNDEAD_MISC = 41914,
    NPC_HUMAN_MISC = 41913,
    NPC_WORGEN_MISC = 41915,
    NPC_PLAGUEBEARER_MISC = 42053
};

enum BG_BFG_NPCS
{
    BG_BFG_NODE_LIGHTHOUSE = 0,
    BG_BFG_NODE_WATERWORKS = 1,
    BG_BFG_NODE_MINE = 2,
    BG_BFG_DYNAMIC_NODES_COUNT = 3, // Dynamic nodes that can be captured
    BG_BFG_SPIRIT_ALIANCE = 3,
    BG_BFG_SPIRIT_HORDE = 4,
    BG_BFG_ALL_NODES_COUNT = 5, // All nodes (dynamic and static)
    BFG_UNK_1,
    BFG_UNK_2,
    BFG_UNK_3,
    BFG_UNK_4,
    BFG_UNK_5,

    //Lighthouse
    BG_BFG_NPC_LH_1,
    BG_BFG_NPC_LH_2,
    BG_BFG_NPC_LH_3,
    BG_BFG_NPC_LH_4,
    BG_BFG_NPC_LH_5,
    BG_BFG_NPC_LH_6,
    BG_BFG_NPC_LH_7,
    BG_BFG_NPC_LH_8,
    BG_BFG_NPC_LH_9,
    BG_BFG_NPC_LH_10,
    BG_BFG_NPC_LH_11,
    BG_BFG_NPC_LH_12,
    BG_BFG_NPC_LH_13,
    BG_BFG_NPC_LH_14,
    BG_BFG_NPC_LH_15,
    BG_BFG_NPC_LH_16,
    BG_BFG_NPC_LH_17,
    BG_BFG_NPC_LH_18,
    BG_BFG_NPC_LH_19,
    // Waterworks
    BG_BFG_NPC_WW_1,
    BG_BFG_NPC_WW_2,
    BG_BFG_NPC_WW_3,
    BG_BFG_NPC_WW_4,
    BG_BFG_NPC_WW_5,
    BG_BFG_NPC_WW_6,
    BG_BFG_NPC_WW_7,
    BG_BFG_NPC_WW_8,
    BG_BFG_NPC_WW_9,
    BG_BFG_NPC_WW_10,
    BG_BFG_NPC_WW_11,
    BG_BFG_NPC_WW_12,
    BG_BFG_NPC_WW_13,
    BG_BFG_NPC_WW_14,
    BG_BFG_NPC_WW_15,
    BG_BFG_NPC_WW_16,
    BG_BFG_NPC_WW_17,
    // Mines
    BG_BFG_NPC_MINE_1,
    BG_BFG_NPC_MINE_2,
    BG_BFG_NPC_MINE_3,
    BG_BFG_NPC_MINE_4,
    BG_BFG_NPC_MINE_5,
    BG_BFG_NPC_MINE_6,
    BG_BFG_NPC_MINE_7,
    BG_BFG_NPC_MINE_8,
    BG_BFG_NPC_MINE_9,
    BG_BFG_NPC_MINE_10,
    BG_BFG_NPC_MINE_11,
    BG_BFG_NPC_MINE_12,
    BG_BFG_NPC_MINE_13,
    BG_BFG_NPC_MINE_14,
    BG_BFG_NPC_MINE_15,
    BG_BFG_NPC_MINE_16,
    BG_BFG_NPC_MINE_17,
    BG_BFG_NPC_MINE_18,
    BG_BFG_NPC_MINE_19,
    BG_BFG_NPC_MINE_20,
    BG_BFG_NPC_MAX,
};

const uint32 BG_BFG_OP_NODESTATES[3]    = { 1767, 1782, 1772 };

const uint32 BG_BFG_OP_NODEICONS[3]     = { 1842, 1846, 1845 };

enum BG_BFG_NodeObjectId
{
    BG_BFG_OBJECTID_NODE_BANNER_0     = 208779, // Lighthouse banner
    BG_BFG_OBJECTID_NODE_BANNER_1     = 208782, // Mine banner
    BG_BFG_OBJECTID_NODE_BANNER_2     = 208785, // Waterworks banner

    BG_BFG_RBG_OBJECTID_NODE_BANNER_0 = 208779, //406009, // Lighthouse banner
    BG_BFG_RBG_OBJECTID_NODE_BANNER_1 = 208782, //406010, // Mine banner
    BG_BFG_RBG_OBJECTID_NODE_BANNER_2 = 208785, //406011  // Waterworks banner
};

enum BG_BG_ObjectType
{
    BG_BFG_OBJECT_BANNER_NEUTRAL            = 0,
    BG_BFG_OBJECT_BANNER_CONT_A             = 1,
    BG_BFG_OBJECT_BANNER_CONT_H             = 2,
    BG_BFG_OBJECT_BANNER_ALLY               = 3,
    BG_BFG_OBJECT_BANNER_HORDE              = 4,
    BG_BFG_OBJECT_AURA_ALLY                 = 5,
    BG_BFG_OBJECT_AURA_HORDE                = 6,
    BG_BFG_OBJECT_AURA_CONTESTED            = 7,
    //gates
    BG_BFG_OBJECT_GATE_A                    = 25,
    BG_BFG_OBJECT_GATE_H                    = 26,
    //buffs
    BG_BFG_OBJECT_SPEEDBUFF_LIGHTHOUSE      = 27,
    BG_BFG_OBJECT_REGENBUFF_LIGHTHOUSE      = 28,
    BG_BFG_OBJECT_BERSERKBUFF_LIGHTHOUSE    = 29,
    BG_BFG_OBJECT_SPEEDBUFF_WATERWORKS      = 30,
    BG_BFG_OBJECT_REGENBUFF_WATERWORKS      = 31,
    BG_BFG_OBJECT_BERSERKBUFF_WATERWORKS    = 32,
    BG_BFG_OBJECT_SPEEDBUFF_MINE            = 33,
    BG_BFG_OBJECT_REGENBUFF_MINE            = 34,
    BG_BFG_OBJECT_BERSERKBUFF_MINE          = 35,
    BG_BFG_OBJECT_MAX                       = 36
};

/* Object id templates from DB */
enum BG_BFG_ObjectTypes
{
    BG_BFG_OBJECTID_BANNER_A            = 208673,
    BG_BFG_OBJECTID_BANNER_CONT_A       = 208763,
    BG_BFG_OBJECTID_BANNER_H            = 208748,
    BG_BFG_OBJECTID_BANNER_CONT_H       = 208733,

    BG_BFG_RBG_OBJECTID_BANNER_A        = 208673, //406012,
    BG_BFG_RBG_OBJECTID_BANNER_CONT_A   = 208763, //406013,
    BG_BFG_RBG_OBJECTID_BANNER_H        = 208748, //406014,
    BG_BFG_RBG_OBJECTID_BANNER_CONT_H   = 208733, //406015,

    BG_BFG_OBJECTID_AURA_A              = 180100,
    BG_BFG_OBJECTID_AURA_H              = 180101,
    BG_BFG_OBJECTID_AURA_C              = 180102,

    BG_BFG_OBJECTID_GATE_A              = 207177,
    BG_BFG_OBJECTID_GATE_H              = 207178
};

enum BG_BFG_Timers
{
    BG_BFG_FLAG_CAPTURING_TIME = 60000
};

enum BG_BFG_Score
{
    BG_BFG_WARNING_NEAR_VICTORY_SCORE = 1800,
    BG_BFG_MAX_TEAM_SCORE = 2000
};

/* Do not change below, or this thing will go boom! */
enum BG_BG_BattlegroundNodes
{   
};

enum BG_BFG_NodeStatus
{
    BG_BFG_NODE_TYPE_NEUTRAL = 0,
    BG_BFG_NODE_TYPE_CONTESTED = 1,
    BG_BFG_NODE_STATUS_ALLY_CONTESTED = 1,
    BG_BFG_NODE_STATUS_HORDE_CONTESTED = 2,
    BG_BFG_NODE_TYPE_OCCUPIED = 3,
    BG_BFG_NODE_STATUS_ALLY_OCCUPIED = 3,
    BG_BFG_NODE_STATUS_HORDE_OCCUPIED = 4
};

enum BG_BFG_Sounds
{
    BG_BFG_SOUND_NODE_CLAIMED = 8192,
    BG_BFG_SOUND_NODE_CAPTURED_ALLIANCE = 8173,
    BG_BFG_SOUND_NODE_CAPTURED_HORDE = 8213,
    BG_BFG_SOUND_NODE_ASSAULTED_ALLIANCE = 8212,
    BG_BFG_SOUND_NODE_ASSAULTED_HORDE = 8174,
    BG_BFG_SOUND_NEAR_VICTORY = 8456
};

enum BG_BFG_Objectives
{
    BG_OBJECTIVE_ASSAULT_BASE = 122,
    BG_OBJECTIVE_DEFEND_BASE = 123,
    BFG_OBJECTIVE_ASSAULT_BASE = 370,
    BFG_OBJECTIVE_DEFEND_BASE = 371,
};

#define BG_BFG_NotBGBGWeekendHonorTicks 330
#define BG_BFG_BGBGWeekendHonorTicks 200

#define BG_EVENT_START_BATTLE 9158 // Achievement: Newbs to Plowshares

// x, y, z, o
const float BG_BFG_NodePositions[BG_BFG_DYNAMIC_NODES_COUNT][4] =
{
    {1057.856934f, 1278.269165f, 2.91f, 1.903034f}, // Lighthouse
    {980.0635f, 948.7772f, 12.868f, 5.9015f}, // Waterwork
    {1251.010f, 958.3939f, 5.680f, 2.7698f} // Mine
};

// x, y, z, o, rot0, rot1, rot2, rot3
const float BG_BFG_DoorPositions[2][8] =
{
    {918.876f, 1336.56f, 27.6195f, 2.77481f, 0.0f, 0.0f, 0.983231f, 0.182367f},
    {1396.15f, 977.014f, 7.43169f, 6.27043f, 0.0f, 0.0f, 0.006378f, -0.99998f}
};

// Tick intervals and given points: case 0,1,2,3 captured nodes
const uint32 BG_BFG_TickIntervals[4] = {0, 12000, 6000, 1000};
const uint32 BG_BFG_TickPoints[4] = {0, 10, 10, 30};

// WorldSafeLocs ids for 3 nodes, and for ally, and horde starting location
const uint32 BG_BFG_GraveyardIds[BG_BFG_ALL_NODES_COUNT] = {1736, 1738, 1735, 1740, 1739};

// x, y, z, o
const float BG_BFG_BuffPositions[BG_BFG_DYNAMIC_NODES_COUNT][4] =
{
    {1063.57f, 1313.42f, 4.91f, 4.14f}, // Lighthouse
    {961.830f, 977.03f, 14.15f, 4.55f}, // Waterworks
    {1193.09f, 1017.46f, 7.98f, 0.24f}, // Mine
};

// x, y, z, o
const float BG_BFG_SpiritGuidePos[BG_BFG_ALL_NODES_COUNT][4] =
{
    {1034.82f, 1335.58f, 12.0095f, 5.15f}, // Lighthouse
    {887.578f, 937.337f, 23.7737f, 0.45f}, // Waterworks
    {1252.23f, 836.547f, 27.7895f, 1.60f}, // Mine
    {908.274f, 1338.6f, 27.6449f, 5.95f}, // Alliance
    {1401.38f, 977.125f, 7.44215f, 3.04f}, // Horde
};

struct BG_BFG_BannerTimer
{
    uint32 timer;
    uint8 type;
    uint8 teamIndex;
};

struct BattlegroundBFGScore : public BattlegroundScore
{
    BattlegroundBFGScore(): BasesAssaulted(0), BasesDefended(0) { }
    ~BattlegroundBFGScore() { }
    uint32 BasesAssaulted;
    uint32 BasesDefended;
    uint32 GetAttr1() const final override { return BasesAssaulted; }
    uint32 GetAttr2() const final override { return BasesDefended; }
};

class BattlegroundBFG : public Battleground
{
public:
    BattlegroundBFG();
    ~BattlegroundBFG();

    void AddPlayer(Player* player);
    void StartingEventCloseDoors();
    void StartingEventOpenDoors();
    void RemovePlayer(Player* player, uint64 guid, uint32 team);
    void HandleAreaTrigger(Player* Source, uint32 Trigger);
    bool SetupBattleground();
    void Reset();
    void EndBattleground(uint32 winner);
    WorldSafeLocsEntry const* GetClosestGraveYard(Player* player);

    /* Score-keeping */
    void UpdatePlayerScore(Player* Source, uint32 type, uint32 value, bool doAddHonor = true);

    void FillInitialWorldStates(WorldPacket& data);

    /* Nodes occupying */
    void EventPlayerClickedOnFlag(Player* source, GameObject* target_obj);

    /* achievement req. */
    bool IsAllNodesConrolledByTeam(uint32 team) const;
    bool IsTeamScores500Disadvantage(uint32 team) const { return m_TeamScores500Disadvantage[GetTeamIndexByTeamId(team)]; }

    uint32 GetPrematureWinner();
    uint32 GetNPCTemplate(Team team, int8 type);
    void SetEmoteAndWeapon(Team team, int8 type, Creature* me);

private:
    void PostUpdateImpl(uint32 diff);
    /* GameObject spawning/removing */
    void _CreateBanner(uint8 node, uint8 type, uint8 teamIndex, bool delay);
    void _DelBanner(uint8 node, uint8 type, uint8 teamIndex);
    void _SendNodeUpdate(uint8 node);

    /* Creature spawning/removing */
    // TODO: need to get the peons spawns scripted
    void _NodeOccupied(uint8 node, Team team);
    void _NodeDeOccupied(uint8 node);

    int32 _GetNodeNameId(uint8 node);

    /* Nodes info:
    0: neutral
    1: ally contested
    2: horde contested
    3: ally occupied
    4: horde occupied */
    uint8 m_Nodes[BG_BFG_DYNAMIC_NODES_COUNT];
    uint8 m_prevNodes[BG_BFG_DYNAMIC_NODES_COUNT];
    BG_BFG_BannerTimer m_BannerTimers[BG_BFG_DYNAMIC_NODES_COUNT];
    uint32 m_NodeTimers[BG_BFG_DYNAMIC_NODES_COUNT];
    uint64 m_LastAssaulter[BG_BFG_DYNAMIC_NODES_COUNT];
    uint64 m_LastDefender[BG_BFG_DYNAMIC_NODES_COUNT];
    uint32 m_lastTick[BG_TEAMS_COUNT];
    uint32 m_HonorScoreTics[BG_TEAMS_COUNT];
    bool m_IsInformedNearVictory;
    uint32 m_HonorTics;
    // need for achievements
    bool m_TeamScores500Disadvantage[BG_TEAMS_COUNT];

    struct NPC_DATA_FORMAT
    {
        uint32 type; //label
        uint32 team; //faction
        float x; //coordinate
        float y; //coordinate
        float z; //coordinate
        float o; //orientation
        SheathState sheath_H;
        SheathState sheath_A;
        uint32 equipment_H;
        uint32 equipment_A;
        uint32 emote;
    };
    //the struct names each column of the const below. for example, AB_NPC_DATA[0].o returns 2.682593f.


    const NPC_DATA_FORMAT BFG_NPC_DATA[BG_BFG_NPC_MAX] = //{LABEL, FACTION, X, Y, Z, O}
    {
        {NULL,               NULL, NULL, NULL, NULL, NULL, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, NULL},   //1-10 are not going to be used for bases.
        {NULL,               NULL, NULL, NULL, NULL, NULL, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, NULL},   //1-10 are not going to be used for bases.
        {NULL,               NULL, NULL, NULL, NULL, NULL, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, NULL},   //1-10 are not going to be used for bases.
        {NULL,               NULL, NULL, NULL, NULL, NULL, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, NULL},   //1-10 are not going to be used for bases.
        {NULL,               NULL, NULL, NULL, NULL, NULL, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, NULL},   //1-10 are not going to be used for bases.
        {NULL,               NULL, NULL, NULL, NULL, NULL, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, NULL},   //1-10 are not going to be used for bases.
        {NULL,               NULL, NULL, NULL, NULL, NULL, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, NULL},   //1-10 are not going to be used for bases.
        {NULL,               NULL, NULL, NULL, NULL, NULL, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, NULL},   //1-10 are not going to be used for bases.
        {NULL,               NULL, NULL, NULL, NULL, NULL, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, NULL},   //1-10 are not going to be used for bases.
        {NULL,               NULL, NULL, NULL, NULL, NULL, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, NULL},   //1-10 are not going to be used for bases.
        {NULL,               NULL, NULL, NULL, NULL, NULL, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, NULL},   //1-10 are not going to be used for bases.
        {BG_BFG_NPC_LH_1,    TEAM_NEUTRAL, 1055.50f, 1257.68f, 5.31234f, 3.77384f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 419141, 419131, STATE_WORK_CHOPWOOD},
        {BG_BFG_NPC_LH_2,    TEAM_NEUTRAL, 1043.55f, 1318.49f, 3.05107f, 2.6036f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 419141, 419131, STATE_WORK_CHOPWOOD},
        {BG_BFG_NPC_LH_3,    TEAM_NEUTRAL, 1034.21f, 1309.48f, 2.36764f, 1.51582f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 419141, 419131, STATE_WORK_CHOPWOOD},
        {BG_BFG_NPC_LH_4,    TEAM_NEUTRAL, 1024.4f, 1327.52f, 1.52062f, 1.06422f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 419142, 0, STATE_WORK_MINING},
        {BG_BFG_NPC_LH_5,    TEAM_NEUTRAL, 1101.39f, 1290.68f, 6.3868f, 5.82767f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 419142, 0, STATE_WORK_MINING},
        {BG_BFG_NPC_LH_6,    TEAM_NEUTRAL, 1085.91f, 1289.29f, 3.62843f, 1.54333f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 419142, 0, STATE_WORK_MINING},
        {BG_BFG_NPC_LH_7,    TEAM_NEUTRAL, 1034.84f, 1281.71f, 2.70364f, 4.21918f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 52556, 52556, STATE_USESTANDING},
        {BG_BFG_NPC_LH_8,    TEAM_NEUTRAL, 1032.35f, 1257.74f, 4.46787f, 6.07428f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 419141, 419131, STATE_WORK_CHOPWOOD},
        {BG_BFG_NPC_LH_9,    TEAM_NEUTRAL, 990.439f, 1330.63f, 6.07083f, 0.757141f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 419141, 419131, STATE_WORK_CHOPWOOD},
        {BG_BFG_NPC_LH_10,   TEAM_NEUTRAL, 1070.84f, 1258.95f, 4.76681f, 3.7322f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, ONESHOT_TALK_NOSHEATHE},
        {BG_BFG_NPC_LH_11,   TEAM_NEUTRAL, 1067.37f, 1259.23f, 4.94054f, 5.39174f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, ONESHOT_TALK_NOSHEATHE},
        {BG_BFG_NPC_LH_12,   TEAM_NEUTRAL, 1014.34f, 1278.94f, 2.91626f, 4.32123f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 52556, 52556, STATE_USESTANDING},
        {BG_BFG_NPC_LH_13,   TEAM_NEUTRAL, 1061.75f, 1301.86f, 3.72857f, 5.37369f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, ONESHOT_TALK_NOSHEATHE},
        {BG_BFG_NPC_LH_14,   TEAM_NEUTRAL, 1064.19f, 1300.89f, 3.78673f, 3.50443f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, ONESHOT_TALK_NOSHEATHE},
        {BG_BFG_NPC_LH_15,   TEAM_NEUTRAL, 1121.83f, 1268.69f, 5.24366f, 2.31849f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, ONESHOT_TALK_NOSHEATHE},
        {BG_BFG_NPC_LH_16,   TEAM_NEUTRAL, 1120.44f, 1271.47f, 5.27098f, 5.04382f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, ONESHOT_TALK_NOSHEATHE},
        {BG_BFG_NPC_LH_17,   TEAM_NEUTRAL, 1078.47f, 1271.27f, 3.74095f, 5.04776f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 52556, 52556, STATE_USESTANDING},
        {BG_BFG_NPC_LH_18,   TEAM_NEUTRAL, 1070.17f, 1284.32f, 3.62846f, 0.069092f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 52556, 52556, STATE_USESTANDING},
        {BG_BFG_NPC_LH_19,   TEAM_NEUTRAL, 1082.56f, 1302.37f, 3.09174f, 0.185361f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 52556, 52556, STATE_USESTANDING},

        {BG_BFG_NPC_WW_1,    TEAM_NEUTRAL, 995.805f, 933.199f, 13.1716, 5.60147f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 419141, 419131, STATE_WORK_CHOPWOOD},
        {BG_BFG_NPC_WW_2,    TEAM_NEUTRAL, 979.484f, 919.15f, 13.1716, 5.47579f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 419141, 419131, STATE_WORK_CHOPWOOD},
        {BG_BFG_NPC_WW_3,    TEAM_NEUTRAL, 956.069f, 922.552f, 13.9969, 4.28827f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 419141, 419131, STATE_WORK_CHOPWOOD},
        {BG_BFG_NPC_WW_4,    TEAM_NEUTRAL, 948.952f, 925.66f, 15.7308, 4.81841f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 419141, 419131, STATE_WORK_CHOPWOOD},
        {BG_BFG_NPC_WW_5,    TEAM_NEUTRAL, 936.929f, 957.868f, 13.855, 3.04655f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 419141, 419131, STATE_WORK_CHOPWOOD},
        {BG_BFG_NPC_WW_6,    TEAM_NEUTRAL, 1005.82f, 953.06f, 13.1579, 5.1891f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, ONESHOT_TALK_NOSHEATHE},
        {BG_BFG_NPC_WW_7,    TEAM_NEUTRAL, 1007.39f, 950.648f, 13.1708, 2.21637f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, ONESHOT_TALK_NOSHEATHE},
        {BG_BFG_NPC_WW_8,    TEAM_NEUTRAL, 970.093f, 968.018f, 13.1719, 5.93838f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, ONESHOT_TALK_NOSHEATHE},
        {BG_BFG_NPC_WW_9,    TEAM_NEUTRAL, 973.983f, 968.309f,13.1715, 3.32773f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, ONESHOT_TALK_NOSHEATHE},
        {BG_BFG_NPC_WW_10,   TEAM_NEUTRAL, 981.796f, 978.734f, 13.1676, 0.797949f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 52556, 52556, STATE_USESTANDING},
        {BG_BFG_NPC_WW_11,   TEAM_NEUTRAL, 947.642f, 972.191f, 13.5881, 1.68152f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 419141, 419131, STATE_WORK_CHOPWOOD},
        {BG_BFG_NPC_WW_12,   TEAM_NEUTRAL, 1020.39f, 910.274f, 13.1759, 5.8842f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 52556, 52556, STATE_USESTANDING},
        {BG_BFG_NPC_WW_13,   TEAM_NEUTRAL, 1022.84f, 918.343f, 13.1725, 2.79836f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, ONESHOT_TALK_NOSHEATHE},
        {BG_BFG_NPC_WW_14,   TEAM_NEUTRAL, 997.691f, 980.65f, 13.0361, 0.241857f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 52556, 52556, STATE_USESTANDING},
        {BG_BFG_NPC_WW_15,   TEAM_NEUTRAL, 913.923f, 889.791f, 28.8366, 4.17667f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 52556, 52556, STATE_USESTANDING},
        {BG_BFG_NPC_WW_16,   TEAM_NEUTRAL, 901.831604f, 1011.480347f, 16.922367, 4.578873f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, ONESHOT_TALK_NOSHEATHE},
        {BG_BFG_NPC_WW_17,   TEAM_NEUTRAL, 904.054260f, 1008.083069f, 16.757637, 2.88319f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, ONESHOT_TALK_NOSHEATHE},

        {BG_BFG_NPC_MINE_1,  TEAM_NEUTRAL,  1239.93f, 984.961f, 7.5027f, 6.26906f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, ONESHOT_TALK_NOSHEATHE},
        {BG_BFG_NPC_MINE_2,  TEAM_NEUTRAL,  1242.85f, 986.218f, 7.92636f, 3.77149f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, ONESHOT_TALK_NOSHEATHE},
        {BG_BFG_NPC_MINE_3,  TEAM_NEUTRAL,  1203.99f, 1019.62f, 7.10189f, 5.78447f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, ONESHOT_TALK_NOSHEATHE},
        {BG_BFG_NPC_MINE_4,  TEAM_NEUTRAL,  1207.08f, 1020.61f, 7.4612f, 3.57358f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, ONESHOT_TALK_NOSHEATHE},
        {BG_BFG_NPC_MINE_5,  TEAM_NEUTRAL,  1195.72f, 1003.66f, 6.69157f, 0.0353567f, SHEATH_STATE_RANGED, SHEATH_STATE_RANGED, 419142, 0, STATE_WORK_MINING},
        {BG_BFG_NPC_MINE_6,  TEAM_NEUTRAL,  1224.5f, 949.304f, 6.11117f, 4.95196f, SHEATH_STATE_RANGED, SHEATH_STATE_RANGED, 419142, 0, STATE_WORK_MINING},
        {BG_BFG_NPC_MINE_7,  TEAM_NEUTRAL,  1208.41f, 979.394f, 5.82021f, 2.54078f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 419141, 419131, STATE_WORK_CHOPWOOD},
        {BG_BFG_NPC_MINE_8,  TEAM_NEUTRAL,  1194.26f, 972.169f, 5.70131f, 5.63917f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 419141, 419131, STATE_WORK_CHOPWOOD},
        {BG_BFG_NPC_MINE_9,  TEAM_NEUTRAL,  1225.6f, 906.293f, 12.8993f, 2.51721f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 419141, 419131, STATE_WORK_CHOPWOOD},
        {BG_BFG_NPC_MINE_10, TEAM_NEUTRAL,  1229.19f, 889.25f, 14.574f, 4.21368f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 419141, 419131, STATE_WORK_CHOPWOOD},
        {BG_BFG_NPC_MINE_11, TEAM_NEUTRAL,  1284.34f, 895.249f, 14.4449f, 4.13513f, SHEATH_STATE_RANGED, SHEATH_STATE_RANGED, 419142, 0, STATE_WORK_MINING},
        {BG_BFG_NPC_MINE_12, TEAM_NEUTRAL,  1279.48f, 905.504f, 13.4857f, 5.17577f, SHEATH_STATE_RANGED, SHEATH_STATE_RANGED, 419142, 0, STATE_WORK_MINING},
        {BG_BFG_NPC_MINE_13, TEAM_NEUTRAL,  1269.84f, 867.935f, 13.7656f, 3.60733f, SHEATH_STATE_RANGED, SHEATH_STATE_RANGED, 419142, 0, STATE_WORK_MINING},
        {BG_BFG_NPC_MINE_14, TEAM_NEUTRAL,  1264.34f, 863.421f, 14.12f, 0.984084f, SHEATH_STATE_RANGED, SHEATH_STATE_RANGED, 419142, 0, STATE_WORK_MINING},
        {BG_BFG_NPC_MINE_15, TEAM_NEUTRAL,  1252.55f, 887.573f, 17.4064f, 4.01258f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, ONESHOT_TALK_NOSHEATHE},
        {BG_BFG_NPC_MINE_16, TEAM_NEUTRAL,  1250.61f, 885.582f, 17.5822f, 0.768888f, SHEATH_STATE_MELEE, SHEATH_STATE_MELEE, 0, 0, ONESHOT_TALK_NOSHEATHE},
        {BG_BFG_NPC_MINE_17, TEAM_NEUTRAL,  1272.99f, 883.582f, 14.1619f, 5.18676f, SHEATH_STATE_RANGED, SHEATH_STATE_RANGED, 419142, 0, STATE_WORK_MINING},
        {BG_BFG_NPC_MINE_18, TEAM_NEUTRAL, 1272.91f, 865.319f, 13.5884f, 5.98863f, SHEATH_STATE_RANGED, SHEATH_STATE_RANGED, 419142, 0, STATE_WORK_MINING},
        {BG_BFG_NPC_MINE_19, TEAM_NEUTRAL, 1284.31f, 876.804f, 13.9174f, 5.23465f, SHEATH_STATE_RANGED, SHEATH_STATE_RANGED, 419142, 0, STATE_WORK_MINING},
        {BG_BFG_NPC_MINE_20, TEAM_NEUTRAL, 1279.77f, 890.089f, 14.3737f, 0.0981368f, SHEATH_STATE_RANGED, SHEATH_STATE_RANGED, 419142, 0, STATE_WORK_MINING},
    };
};
#endif
