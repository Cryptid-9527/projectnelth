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

#ifndef __BATTLEGROUNDAV_H
#define __BATTLEGROUNDAV_H

#include "Battleground.h"

#define LANG_BG_AV_A_CAPTAIN_BUFF       "Begone. Uncouth scum! The Alliance shall prevail in Alterac Valley!"
#define LANG_BG_AV_H_CAPTAIN_BUFF       "Now is the time to attack! For the Horde!"
#define LANG_BG_AV_S_MINE_BOSS_CLAIMS   "Snivvle is here! Snivvle claims the Coldtooth Mine!"

#define BG_AV_CAPTIME                    240000  //4:00
#define BG_AV_SNOWFALL_FIRSTCAP          240000  //Addons line up with 4:00

#define BG_AV_SCORE_INITIAL_POINTS       600
#define SEND_MSG_NEAR_LOSE               120

#define BG_AV_KILL_BOSS                 4
#define BG_AV_REP_BOSS                  350

#define BG_AV_KILL_CAPTAIN              3
#define BG_AV_REP_CAPTAIN               125
#define BG_AV_RES_CAPTAIN               100

#define BG_AV_KILL_TOWER                3
#define BG_AV_REP_TOWER                 12
#define BG_AV_RES_TOWER                 75

#define BG_AV_GET_COMMANDER            1 //for a safely returned wingcommander
//bonushonor at the end
#define BG_AV_KILL_SURVIVING_TOWER      2
#define BG_AV_REP_SURVIVING_TOWER       12

#define BG_AV_KILL_SURVIVING_CAPTAIN    2
#define BG_AV_REP_SURVIVING_CAPTAIN     125

#define AV_EVENT_START_BATTLE           9166 // Achievement: The Alterac Blitz

const uint32 BG_AV_Factions[2] =
{
    1732, // Alliance
    1735  // Horde
};


enum BG_AV_Sounds
{ //TODO: get out if there comes a sound when neutral team captures mine

/*
8212:
    alliance grave assault
    alliance tower assault
    drek "mlanzenabschaum! In meiner Burg?! Toetet sie all" - nicht immer der sound
8333:
    galv "sterbt fuer euch ist kein platz hier"

8332:
    bal "Verschwinde, dreckiger Abschaum! Die Allianz wird im Alteractal "
8174:
    horde tower assault
    horde grave assault
    van "es Sturmlanzenklans, euer General wird angegriffen! Ich fordere Unterst"
8173:
    ally grave capture/defend
    tower destroy
    mine capture
    ally wins
8192:
    ally tower destroy(only iceblood - found a bug^^)
    ally tower  defend
    horde tower defend
8213
horde:
    grave defend/capture
    tower destroy
    mine capture
    horde wins
    */

    AV_SOUND_NEAR_VICTORY                   = 8456, //not confirmed yet

    AV_SOUND_ALLIANCE_ASSAULTS              = 8212, //tower, grave + enemy boss if someone tries to attack him
    AV_SOUND_HORDE_ASSAULTS                 = 8174,
    AV_SOUND_ALLIANCE_GOOD                  = 8173, //if something good happens for the team:  wins(maybe only through killing the boss), captures mine or grave, destroys tower and defends grave
    AV_SOUND_HORDE_GOOD                     = 8213,
    AV_SOUND_BOTH_TOWER_DEFEND              = 8192,

    AV_SOUND_ALLIANCE_CAPTAIN               = 8232, //gets called when someone attacks them and at the beginning after 3min+rand(x)*10sec (maybe buff)
    AV_SOUND_HORDE_CAPTAIN                  = 8333
};

enum BG_AV_OTHER_VALUES
{
    AV_STATICCPLACE_MAX                 = 157,
    AV_NORTH_MINE                       = 0,
    AV_SOUTH_MINE                       = 1,
    AV_MINE_TICK_TIMER                  = 45000,
    AV_MINE_RECLAIM_TIMER               = 1200000, //TODO: get the right value.. this is currently 20 minutes
    AV_NEUTRAL_TEAM                     = 0 //this is the neutral owner of snowfall
};
enum BG_AV_ObjectIds
{
    //cause the mangos-system is a bit different, we don't use the right go-ids for every node.. if we want to be 100% like another big server, we must take one object for every node
    //snowfall 4flags as eyecandy 179424 (alliance neutral)
    //Banners - stolen from battleground_AB.h ;-)
    BG_AV_OBJECTID_BANNER_A             = 178925, // can only be used by horde
    BG_AV_OBJECTID_BANNER_H             = 178943, // can only be used by alliance
    BG_AV_OBJECTID_BANNER_CONT_A        = 178940, // can only be used by horde
    BG_AV_OBJECTID_BANNER_CONT_H        = 179435, // can only be used by alliance

    BG_AV_OBJECTID_BANNER_A_B           = 178365,
    BG_AV_OBJECTID_BANNER_H_B           = 178364,
    BG_AV_OBJECTID_BANNER_CONT_A_B      = 179286,
    BG_AV_OBJECTID_BANNER_CONT_H_B      = 179287,
    BG_AV_OBJECTID_BANNER_SNOWFALL_N    = 180418,

    //snowfall eyecandy banner:
    BG_AV_OBJECTID_SNOWFALL_CANDY_A     = 179044,
    BG_AV_OBJECTID_SNOWFALL_CANDY_PA    = 179424,
    BG_AV_OBJECTID_SNOWFALL_CANDY_H     = 179064,
    BG_AV_OBJECTID_SNOWFALL_CANDY_PH    = 179425,

    //banners on top of towers:
    BG_AV_OBJECTID_TOWER_BANNER_A       = 178927, //[PH] Alliance A1 Tower Banner BIG
    BG_AV_OBJECTID_TOWER_BANNER_H       = 178955, //[PH] Horde H1 Tower Banner BIG
    BG_AV_OBJECTID_TOWER_BANNER_PA      = 179446, //[PH] Alliance H1 Tower Pre-Banner BIG
    BG_AV_OBJECTID_TOWER_BANNER_PH      = 179436, //[PH] Horde A1 Tower Pre-Banner BIG

    //Auras
    BG_AV_OBJECTID_AURA_A               = 180421,
    BG_AV_OBJECTID_AURA_H               = 180422,
    BG_AV_OBJECTID_AURA_N               = 180423,
    BG_AV_OBJECTID_AURA_A_S             = 180100,
    BG_AV_OBJECTID_AURA_H_S             = 180101,
    BG_AV_OBJECTID_AURA_N_S             = 180102,

    BG_AV_OBJECTID_GATE_A               = 180424,
    BG_AV_OBJECTID_GATE_H               = 180424,

    //mine supplies
    BG_AV_OBJECTID_MINE_N               = 178785,
    BG_AV_OBJECTID_MINE_S               = 178784,

    BG_AV_OBJECTID_FIRE                 = 179065,
    BG_AV_OBJECTID_CAMPFIRE             = 29784,
    BG_AV_OBJECTID_BONFIRE              = 208198,
    BG_AV_OBJECTID_BONFIRE_SKINNY       = 206745,
    BG_AV_OBJECTID_SMOKE                = 179066
};

enum BG_AV_Nodes
{
    BG_AV_NODES_FIRSTAID_STATION        = 0,         //alliance rear GY
    BG_AV_NODES_STORMPIKE_GRAVE         = 1,         //alli graves
    BG_AV_NODES_STONEHEART_GRAVE        = 2,         //alli mid graveyard
    BG_AV_NODES_SNOWFALL_GRAVE          = 3,         //Neutral GY
    BG_AV_NODES_ICEBLOOD_GRAVE          = 4,         //horde graves
    BG_AV_NODES_FROSTWOLF_GRAVE         = 5,         //horde rear graveyard
    BG_AV_NODES_FROSTWOLF_HUT           = 6,         //horde base
    BG_AV_NODES_DUNBALDAR_SOUTH         = 7,         //alliance rear towers
    BG_AV_NODES_DUNBALDAR_NORTH         = 8,         //alli north tower
    BG_AV_NODES_ICEWING_BUNKER          = 9,         //alli second tower
    BG_AV_NODES_STONEHEART_BUNKER       = 10,        //alli middle tower
    BG_AV_NODES_ICEBLOOD_TOWER          = 11,        //horde middle tower
    BG_AV_NODES_TOWER_POINT             = 12,        //horde second tower
    BG_AV_NODES_FROSTWOLF_ETOWER        = 13,        //horde rear towers
    BG_AV_NODES_FROSTWOLF_WTOWER        = 14,        //horde rear tower 2

    BG_AV_NODES_MAX                     = 15
};

enum BG_AV_ObjectTypes
{
    BG_AV_OBJECT_FLAG_A_FIRSTAID_STATION    = 0,
    BG_AV_OBJECT_FLAG_A_STORMPIKE_GRAVE     = 1,
    BG_AV_OBJECT_FLAG_A_STONEHEART_GRAVE    = 2,
    BG_AV_OBJECT_FLAG_A_SNOWFALL_GRAVE      = 3,
    BG_AV_OBJECT_FLAG_A_ICEBLOOD_GRAVE      = 4,
    BG_AV_OBJECT_FLAG_A_FROSTWOLF_GRAVE     = 5,
    BG_AV_OBJECT_FLAG_A_FROSTWOLF_HUT       = 6,
    BG_AV_OBJECT_FLAG_A_DUNBALDAR_SOUTH     = 7,
    BG_AV_OBJECT_FLAG_A_DUNBALDAR_NORTH     = 8,
    BG_AV_OBJECT_FLAG_A_ICEWING_BUNKER      = 9,
    BG_AV_OBJECT_FLAG_A_STONEHEART_BUNKER   = 10,

    BG_AV_OBJECT_FLAG_C_A_FIRSTAID_STATION    = 11,
    BG_AV_OBJECT_FLAG_C_A_STORMPIKE_GRAVE     = 12,
    BG_AV_OBJECT_FLAG_C_A_STONEHEART_GRAVE    = 13,
    BG_AV_OBJECT_FLAG_C_A_SNOWFALL_GRAVE      = 14,
    BG_AV_OBJECT_FLAG_C_A_ICEBLOOD_GRAVE      = 15,
    BG_AV_OBJECT_FLAG_C_A_FROSTWOLF_GRAVE     = 16,
    BG_AV_OBJECT_FLAG_C_A_FROSTWOLF_HUT       = 17,
    BG_AV_OBJECT_FLAG_C_A_ICEBLOOD_TOWER      = 18,
    BG_AV_OBJECT_FLAG_C_A_TOWER_POINT         = 19,
    BG_AV_OBJECT_FLAG_C_A_FROSTWOLF_ETOWER    = 20,
    BG_AV_OBJECT_FLAG_C_A_FROSTWOLF_WTOWER    = 21,

    BG_AV_OBJECT_FLAG_C_H_FIRSTAID_STATION    = 22,
    BG_AV_OBJECT_FLAG_C_H_STORMPIKE_GRAVE     = 23,
    BG_AV_OBJECT_FLAG_C_H_STONEHEART_GRAVE    = 24,
    BG_AV_OBJECT_FLAG_C_H_SNOWFALL_GRAVE      = 25,
    BG_AV_OBJECT_FLAG_C_H_ICEBLOOD_GRAVE      = 26,
    BG_AV_OBJECT_FLAG_C_H_FROSTWOLF_GRAVE     = 27,
    BG_AV_OBJECT_FLAG_C_H_FROSTWOLF_HUT       = 28,
    BG_AV_OBJECT_FLAG_C_H_DUNBALDAR_SOUTH     = 29,
    BG_AV_OBJECT_FLAG_C_H_DUNBALDAR_NORTH     = 30,
    BG_AV_OBJECT_FLAG_C_H_ICEWING_BUNKER      = 31,
    BG_AV_OBJECT_FLAG_C_H_STONEHEART_BUNKER   = 32,

    BG_AV_OBJECT_FLAG_H_FIRSTAID_STATION    = 33,
    BG_AV_OBJECT_FLAG_H_STORMPIKE_GRAVE     = 34,
    BG_AV_OBJECT_FLAG_H_STONEHEART_GRAVE    = 35,
    BG_AV_OBJECT_FLAG_H_SNOWFALL_GRAVE      = 36,
    BG_AV_OBJECT_FLAG_H_ICEBLOOD_GRAVE      = 37,
    BG_AV_OBJECT_FLAG_H_FROSTWOLF_GRAVE     = 38,
    BG_AV_OBJECT_FLAG_H_FROSTWOLF_HUT       = 39,
    BG_AV_OBJECT_FLAG_H_ICEBLOOD_TOWER      = 40,
    BG_AV_OBJECT_FLAG_H_TOWER_POINT         = 41,
    BG_AV_OBJECT_FLAG_H_FROSTWOLF_ETOWER    = 42,
    BG_AV_OBJECT_FLAG_H_FROSTWOLF_WTOWER    = 43,

    BG_AV_OBJECT_FLAG_N_SNOWFALL_GRAVE      = 44,

    BG_AV_OBJECT_DOOR_H                     = 45,
    BG_AV_OBJECT_DOOR_A                     = 46,
//auras for graveyards (3auras per graveyard neutral, alliance, horde)
    BG_AV_OBJECT_AURA_N_FIRSTAID_STATION    = 47,
    BG_AV_OBJECT_AURA_A_FIRSTAID_STATION    = 48,
    BG_AV_OBJECT_AURA_H_FIRSTAID_STATION    = 49,
    BG_AV_OBJECT_AURA_N_STORMPIKE_GRAVE     = 50,
    BG_AV_OBJECT_AURA_A_STORMPIKE_GRAVE     = 51,
    BG_AV_OBJECT_AURA_H_STORMPIKE_GRAVE     = 52,
    BG_AV_OBJECT_AURA_N_STONEHEART_GRAVE    = 53,
    BG_AV_OBJECT_AURA_A_STONEHEART_GRAVE    = 54,
    BG_AV_OBJECT_AURA_H_STONEHEART_GRAVE    = 55,
    BG_AV_OBJECT_AURA_N_SNOWFALL_GRAVE      = 56,
    BG_AV_OBJECT_AURA_A_SNOWFALL_GRAVE      = 57,
    BG_AV_OBJECT_AURA_H_SNOWFALL_GRAVE      = 58,
    BG_AV_OBJECT_AURA_N_ICEBLOOD_GRAVE      = 59,
    BG_AV_OBJECT_AURA_A_ICEBLOOD_GRAVE      = 60,
    BG_AV_OBJECT_AURA_H_ICEBLOOD_GRAVE      = 61,
    BG_AV_OBJECT_AURA_N_FROSTWOLF_GRAVE     = 62,
    BG_AV_OBJECT_AURA_A_FROSTWOLF_GRAVE     = 63,
    BG_AV_OBJECT_AURA_H_FROSTWOLF_GRAVE     = 64,
    BG_AV_OBJECT_AURA_N_FROSTWOLF_HUT       = 65,
    BG_AV_OBJECT_AURA_A_FROSTWOLF_HUT       = 66,
    BG_AV_OBJECT_AURA_H_FROSTWOLF_HUT       = 67,

    //big flags on top of towers 2 flags on each (contested, (alliance | horde)) + 2 auras
    BG_AV_OBJECT_TFLAG_A_DUNBALDAR_SOUTH     = 67,
    BG_AV_OBJECT_TFLAG_H_DUNBALDAR_SOUTH     = 68,
    BG_AV_OBJECT_TFLAG_A_DUNBALDAR_NORTH     = 69,
    BG_AV_OBJECT_TFLAG_H_DUNBALDAR_NORTH     = 70,
    BG_AV_OBJECT_TFLAG_A_ICEWING_BUNKER      = 71,
    BG_AV_OBJECT_TFLAG_H_ICEWING_BUNKER      = 72,
    BG_AV_OBJECT_TFLAG_A_STONEHEART_BUNKER   = 73,
    BG_AV_OBJECT_TFLAG_H_STONEHEART_BUNKER   = 74,
    BG_AV_OBJECT_TFLAG_A_ICEBLOOD_TOWER      = 75,
    BG_AV_OBJECT_TFLAG_H_ICEBLOOD_TOWER      = 76,
    BG_AV_OBJECT_TFLAG_A_TOWER_POINT         = 77,
    BG_AV_OBJECT_TFLAG_H_TOWER_POINT         = 78,
    BG_AV_OBJECT_TFLAG_A_FROSTWOLF_ETOWER    = 79,
    BG_AV_OBJECT_TFLAG_H_FROSTWOLF_ETOWER    = 80,
    BG_AV_OBJECT_TFLAG_A_FROSTWOLF_WTOWER    = 81,
    BG_AV_OBJECT_TFLAG_H_FROSTWOLF_WTOWER    = 82,
    BG_AV_OBJECT_TAURA_A_DUNBALDAR_SOUTH     = 83,
    BG_AV_OBJECT_TAURA_H_DUNBALDAR_SOUTH     = 84,
    BG_AV_OBJECT_TAURA_A_DUNBALDAR_NORTH     = 85,
    BG_AV_OBJECT_TAURA_H_DUNBALDAR_NORTH     = 86,
    BG_AV_OBJECT_TAURA_A_ICEWING_BUNKER      = 87,
    BG_AV_OBJECT_TAURA_H_ICEWING_BUNKER      = 88,
    BG_AV_OBJECT_TAURA_A_STONEHEART_BUNKER   = 89,
    BG_AV_OBJECT_TAURA_H_STONEHEART_BUNKER   = 90,
    BG_AV_OBJECT_TAURA_A_ICEBLOOD_TOWER      = 91,
    BG_AV_OBJECT_TAURA_H_ICEBLOOD_TOWER      = 92,
    BG_AV_OBJECT_TAURA_A_TOWER_POINT         = 93,
    BG_AV_OBJECT_TAURA_H_TOWER_POINT         = 94,
    BG_AV_OBJECT_TAURA_A_FROSTWOLF_ETOWER    = 95,
    BG_AV_OBJECT_TAURA_H_FROSTWOLF_ETOWER    = 96,
    BG_AV_OBJECT_TAURA_A_FROSTWOLF_WTOWER    = 97,
    BG_AV_OBJECT_TAURA_H_FROSTWOLF_WTOWER    = 98,

    BG_AV_OBJECT_BURN_DUNBALDAR_SOUTH        = 99,
    BG_AV_OBJECT_BURN_DUNBALDAR_NORTH        = 109,
    BG_AV_OBJECT_BURN_ICEWING_BUNKER         = 119,
    BG_AV_OBJECT_BURN_STONEHEART_BUNKER      = 129,
    BG_AV_OBJECT_BURN_ICEBLOOD_TOWER         = 139,
    BG_AV_OBJECT_BURN_TOWER_POINT            = 149,
    BG_AV_OBJECT_BURN_FROSTWOLF_ETWOER       = 159,
    BG_AV_OBJECT_BURN_FROSTWOLF_WTOWER       = 169,
    BG_AV_OBJECT_BURN_BUILDING_ALLIANCE      = 179,
    BG_AV_OBJECT_BURN_BUILDING_HORDE         = 189,
    BG_AV_OBJECT_SNOW_EYECANDY_A             = 199,
    BG_AV_OBJECT_SNOW_EYECANDY_PA            = 203,
    BG_AV_OBJECT_SNOW_EYECANDY_H             = 207,
    BG_AV_OBJECT_SNOW_EYECANDY_PH            = 211,
    BG_AV_OBJECT_MINE_SUPPLY_N_MIN           = 215,
    BG_AV_OBJECT_MINE_SUPPLY_N_MAX           = 224,
    BG_AV_OBJECT_MINE_SUPPLY_S_MIN           = 225,
    BG_AV_OBJECT_MINE_SUPPLY_S_MAX           = 236,

    BG_AV_OBJECT_MAX                         = 239
};

enum BG_AV_OBJECTS
{
    AV_OPLACE_FIRSTAID_STATION                  = 0,
    AV_OPLACE_STORMPIKE_GRAVE                   = 1,
    AV_OPLACE_STONEHEART_GRAVE                  = 2,
    AV_OPLACE_SNOWFALL_GRAVE                    = 3,
    AV_OPLACE_ICEBLOOD_GRAVE                    = 4,
    AV_OPLACE_FROSTWOLF_GRAVE                   = 5,
    AV_OPLACE_FROSTWOLF_HUT                     = 6,
    AV_OPLACE_DUNBALDAR_SOUTH                   = 7,
    AV_OPLACE_DUNBALDAR_NORTH                   = 8,
    AV_OPLACE_ICEWING_BUNKER                    = 9,
    AV_OPLACE_STONEHEART_BUNKER                 = 10,
    AV_OPLACE_ICEBLOOD_TOWER                    = 11,
    AV_OPLACE_TOWER_POINT                       = 12,
    AV_OPLACE_FROSTWOLF_ETOWER                  = 13,
    AV_OPLACE_FROSTWOLF_WTOWER                  = 14,
    AV_OPLACE_BIGBANNER_DUNBALDAR_SOUTH         = 15,
    AV_OPLACE_BIGBANNER_DUNBALDAR_NORTH         = 16,
    AV_OPLACE_BIGBANNER_ICEWING_BUNKER          = 17,
    AV_OPLACE_BIGBANNER_STONEHEART_BUNKER       = 18,
    AV_OPLACE_BIGBANNER_ICEBLOOD_TOWER          = 19,
    AV_OPLACE_BIGBANNER_TOWER_POINT             = 20,
    AV_OPLACE_BIGBANNER_FROSTWOLF_ETOWER        = 21,
    AV_OPLACE_BIGBANNER_FROSTWOLF_WTOWER        = 22,

    AV_OPLACE_BURN_DUNBALDAR_SOUTH              = 23,
    AV_OPLACE_BURN_DUNBALDAR_NORTH              = 33,
    AV_OPLACE_BURN_ICEWING_BUNKER               = 43,
    AV_OPLACE_BURN_STONEHEART_BUNKER            = 53,
    AV_OPLACE_BURN_ICEBLOOD_TOWER               = 63,
    AV_OPLACE_BURN_TOWER_POINT                  = 73,
    AV_OPLACE_BURN_FROSTWOLF_ETOWER             = 83,
    AV_OPLACE_BURN_FROSTWOLF_WTOWER             = 93,
    AV_OPLACE_BURN_BUILDING_A                   = 103,
    AV_OPLACE_BURN_BUILDING_H                   = 113,
    AV_OPLACE_SNOW_1                            = 123,
    AV_OPLACE_SNOW_2                            = 124,
    AV_OPLACE_SNOW_3                            = 125,
    AV_OPLACE_SNOW_4                            = 126,
    AV_OPLACE_MINE_SUPPLY_N_MIN                 = 127,
    AV_OPLACE_MINE_SUPPLY_N_MAX                 = 136,
    AV_OPLACE_MINE_SUPPLY_S_MIN                 = 137,
    AV_OPLACE_MINE_SUPPLY_S_MAX                 = 148,

    AV_OPLACE_FirePit_1                         = 149,   
    AV_OPLACE_FirePit_2                         = 150,
    AV_OPLACE_FirePit_3                         = 151,
    AV_OPLACE_FirePit_4                         = 152,
    AV_OPLACE_MAX                               = 153
};

const float BG_AV_ObjectPos[AV_OPLACE_MAX][4] =
{
    {638.592f, -32.422f, 46.0608f, -1.62316f }, //firstaid station
    {669.007f, -294.078f, 30.2909f, 2.77507f }, //stormpike
    {77.8013f, -404.7f, 46.7549f, -0.872665f }, //stone grave
    {-202.581f, -112.73f, 78.4876f, -0.715585f }, //snowfall
    {-611.962f, -396.17f, 60.8351f, 2.53682f},  //iceblood grave
    {-1082.45f, -346.823f, 54.9219f, -1.53589f }, //frostwolf grave
    {-1402.21f, -307.431f, 89.4424f, 0.191986f }, //frostwolf hut
    {553.779f, -78.6566f, 51.9378f, -1.22173f }, //dunnbaldar south
    {674.001f, -143.125f, 63.6615f, 0.994838f }, //dunbaldar north
    {203.281f, -360.366f, 56.3869f, -0.925024f }, //icew
    {-152.437f, -441.758f, 40.3982f, -1.95477f }, //stone
    {-571.88f, -262.777f, 75.0087f, -0.802851f }, //ice tower
    {-768.907f, -363.71f, 90.8949f, 1.07991f},  //tower point
    {-1302.9f, -316.981f, 113.867f, 2.00713f }, //frostwolf etower
    {-1297.5f, -266.767f, 114.15f, 3.31044f},   //frostwolf wtower
    //bigbanner:
    {555.848f, -84.4151f, 64.4397f, 3.12414f }, //duns
    {679.339f, -136.468f, 73.9626f, -2.16421f }, //dunn
    {208.973f, -365.971f, 66.7409f, -0.244346f }, //icew
    {-155.832f, -449.401f, 52.7306f, 0.610865f }, //stone
    {-572.329f, -262.476f, 88.6496f, -0.575959f }, //icetower
    {-768.199f, -363.105f, 104.537f, 0.10472f }, //towerp
    {-1302.84f, -316.582f, 127.516f, 0.122173f }, //etower
    {-1297.87f, -266.762f, 127.796f, 0.0698132f }, //wtower
    //burning auras towers have 9*179065 captain-buildings have 5*179066+5*179065
    //dunns
    {562.632f, -88.1815f, 61.993f, 0.383972f },
    {562.523f, -74.5028f, 37.9474f, -0.0523599f },
    {558.097f, -70.9842f, 52.4876f, 0.820305f },
    {578.167f, -71.8191f, 38.1514f, 2.72271f },
    {556.028f, -94.9242f, 44.8191f, 3.05433f },
    {572.451f, -94.3655f, 37.9443f, -1.72788f },
    {549.263f, -79.3645f, 44.8191f, 0.436332f },
    {543.513f, -94.4006f, 52.4819f, 0.0349066f },
    {572.149f, -93.7862f, 52.5726f, 0.541052f },
    {582.162f, -81.2375f, 37.9216f, 0.0872665f },
    //dunn
    {664.797f, -143.65f, 64.1784f, -0.453786f},
    {664.505f, -139.452f, 49.6696f, -0.0349067f},
    {676.067f, -124.319f, 49.6726f, -1.01229f},
    {693.004f, -144.025f, 64.1755f, 2.44346f},
    {661.175f, -117.691f, 49.645f, 1.91986f},
    {684.423f, -146.582f, 63.6662f, 0.994838f},
    {682.791f, -127.769f, 62.4155f, 1.09956f},
    {674.576f, -147.101f, 56.5425f, -1.6057f},
    {655.719f, -126.673f, 49.8138f, 2.80998f},
    {0, 0, 0, 0},
    //icew
    {231.503f, -356.688f, 42.3704f, 0.296706f},
    {224.989f, -348.175f, 42.5607f, 1.50098f},
    {205.782f, -351.335f, 56.8998f, 1.01229f},
    {196.605f, -369.187f, 56.3914f, 2.46091f},
    {210.619f, -376.938f, 49.2677f, 2.86234f},
    {209.647f, -352.632f, 42.3959f, -0.698132f},
    {220.65f, -368.132f, 42.3978f, -0.2618f},
    {224.682f, -374.031f, 57.0679f, 0.541052f},
    {200.26f, -359.968f, 49.2677f, -2.89725f},
    {196.619f, -378.016f, 56.9131f, 1.01229f},
    //stone
    {-155.488f, -437.356f, 33.2796f, 2.60054f},
    {-163.441f, -454.188f, 33.2796f, 1.93732f},
    {-143.977f, -445.148f, 26.4097f, -1.8675f},
    {-135.764f, -464.708f, 26.3823f, 2.25147f},
    {-154.076f, -466.929f, 41.0636f, -1.8675f},
    {-149.908f, -460.332f, 26.4083f, -2.09439f},
    {-151.638f, -439.521f, 40.3797f, 0.436332f},
    {-131.301f, -454.905f, 26.5771f, 2.93215f},
    {-171.291f, -444.684f, 40.9211f, 2.30383f},
    {-143.591f, -439.75f, 40.9275f, -1.72788f},
    //iceblood
    {-572.667f, -267.923f, 56.8542f, 2.35619f},
    {-561.021f, -262.689f, 68.4589f, 1.37881f},
    {-572.538f, -262.649f, 88.6197f, 1.8326f},
    {-574.77f, -251.45f, 74.9422f, -1.18682f},
    {-578.625f, -267.571f, 68.4696f, 0.506145f},
    {-571.476f, -257.234f, 63.3223f, 3.10669f},
    {-566.035f, -273.907f, 52.9582f, -0.890118f},
    {-580.948f, -259.77f, 68.4696f, 1.46608f},
    {-568.318f, -267.1f, 75.0008f, 1.01229f},
    {-559.621f, -268.597f, 52.8986f, 0.0523599f},
    //towerp
    {-776.072f, -368.046f, 84.3558f, 2.63545f},
    {-777.564f, -368.521f, 90.6701f, 1.72788f},
    {-765.461f, -357.711f, 90.888f, 0.314159f},
    {-768.763f, -362.735f, 104.612f, 1.81514f},
    {-760.356f, -358.896f, 84.3558f, 2.1293f},
    {-771.967f, -352.838f, 84.3484f, 1.74533f},
    {-773.333f, -364.653f, 79.2351f, -1.64061f},
    {-764.109f, -366.069f, 70.0934f, 0.383972f},
    {-767.103f, -350.737f, 68.7933f, 2.80998f},
    {-760.115f, -353.845f, 68.8633f, 1.79769f},
    //froste
    {-1304.87f, -304.525f, 91.8366f, -0.680679f},
    {-1301.77f, -310.974f, 95.8252f, 0.907571f},
    {-1305.58f, -320.625f, 102.166f, -0.558505f},
    {-1294.27f, -323.468f, 113.893f, -1.67552f},
    {-1302.65f, -317.192f, 127.487f, 2.30383f},
    {-1293.89f, -313.478f, 107.328f, 1.6057f},
    {-1312.41f, -312.999f, 107.328f, 1.5708f},
    {-1311.57f, -308.08f, 91.7666f, -1.85005f},
    {-1314.7f, -322.131f, 107.36f, 0.645772f},
    {-1304.6f, -310.754f, 113.859f, -0.401426f},
    //frostw
    {-1308.24f, -273.26f, 92.0514f, -0.139626f},
    {-1302.26f, -262.858f, 95.9269f, 0.418879f},
    {-1297.28f, -267.773f, 126.756f, 2.23402f},
    {-1299.08f, -256.89f, 114.108f, -2.44346f},
    {-1303.41f, -268.237f, 114.151f, -1.23918f},
    {-1304.43f, -273.682f, 107.612f, 0.244346f},
    {-1309.53f, -265.951f, 92.1418f, -2.49582f},
    {-1295.55f, -263.865f, 105.033f, 0.925024f},
    {-1294.71f, -281.466f, 107.664f, -1.50098f},
    {-1289.69f, -259.521f, 107.612f, -2.19912f},

    //the two buildings of the captains
    //alliance
    {-64.4987f, -289.33f, 33.4616f, -2.82743f},
    {-5.98025f, -326.144f, 38.8538f, 0},
    {-2.67893f, -306.998f, 33.4165f, 0},
    {-60.25f, -309.232f, 50.2408f, -1.46608f},
    {-48.7941f, -266.533f, 47.7916f, 2.44346f},
    {-3.40929f, -306.288f, 33.34f, 0},
    {-48.619f, -266.917f, 47.8168f, 0},
    {-62.9474f, -286.212f, 66.7288f, 0},
    {-5.05132f, -325.323f, 38.8536f, 0},
    {-64.2677f, -289.412f, 33.469f, 0},
//horde
    {-524.276f, -199.6f, 82.8733f, -1.46608f},
    {-518.196f, -173.085f, 102.43f, 0},
    {-500.732f, -145.358f, 88.5337f, 2.44346f},
    {-501.084f, -150.784f, 80.8506f, 0},
    {-518.309f, -163.963f, 102.521f, 2.96706f},
    {-517.053f, -200.429f, 80.759f, 0},
    {-514.361f, -163.864f, 104.163f, 0},
    {-568.04f, -188.707f, 81.55f, 0},
    {-501.775f, -151.581f, 81.2027f, 0},
    {-509.975f, -191.652f, 83.2978f, 0},

//snowfall eyecandy
    {-191.153f, -129.868f, 78.5595f, -1.25664f },
    {-201.282f, -134.319f, 78.6753f, -0.942478f },
    {-215.981f, -91.4101f, 80.8702f, -1.74533f },
    {-200.465f, -96.418f, 79.7587f, 1.36136f },
    //mine supplies
    //irondeep
    {870.899f, -388.434f, 61.6406f, -1.22173f},
    {825.214f, -320.174f, 63.712f, -2.82743f},
    {837.117f, -452.556f, 47.2331f, -3.12414f},
    {869.755f, -448.867f, 52.5448f, -0.855212f},
    {949.877f, -458.198f, 56.4874f, 0.314159f},
    {900.35f, -479.024f, 58.3553f, 0.122173f},
    {854.449f, -442.255f, 50.6589f, 0.401426f},
    {886.685f, -442.358f, 54.6962f, -1.22173f},
    {817.509f, -457.331f, 48.4666f, 2.07694f},
    {793.411f, -326.281f, 63.1117f, -2.79253f},
    //coldtooth
    {-934.212f, -57.3517f, 80.277f, -0.0174535f},
    {-916.281f, -36.8579f, 77.0227f, 0.122173f},
    {-902.73f, -103.868f, 75.4378f, -1.58825f},
    {-900.514f, -143.527f, 75.9686f, 1.8675f},
    {-862.882f, -0.353299f, 72.1526f, -2.51327f},
    {-854.932f, -85.9184f, 68.6056f, -2.04204f},
    {-851.833f, -118.959f, 63.8672f, -0.0698131f},
    {-849.832f, -20.8421f, 70.4672f, -1.81514f},
    {-844.25f, -60.0374f, 72.1031f, -2.19912f},
    {-820.644f, -136.043f, 63.1977f, 2.40855f},
    {-947.642f, -208.807f, 77.0101f, 1.36136f},
    {-951.394f, -193.695f, 67.634f, 0.802851f},
    {351.033020f, -493.161011f, 71.451675f, 0.115715f},
    {-743.127808f, -397.796997f, 76.484863f, 3.025661f},//firepit 2
    {-1286.189331f, -184.110825f, 72.079803f, 3.422264f},//firepit 3
    {-1279.406616f, -219.805191f, 72.530594f, 5.864842f},//firepit 4
};

const float BG_AV_DoorPositons[2][4] =
{
    {793.807488f, -493.181902f, 99.931842f, 3.016842f},   //alliance
    {-1380.973396f, -544.725324f, 54.934431f, 0.72178f} //horde
};

//creaturestuff starts here
//is related to BG_AV_CreaturePos
enum BG_AV_CreaturePlace
{
    AV_CPLACE_SPIRIT_STORM_AID      = 0,
    AV_CPLACE_SPIRIT_STORM_GRAVE    = 1,
    AV_CPLACE_SPIRIT_STONE_GRAVE    = 2,
    AV_CPLACE_SPIRIT_SNOWFALL       = 3,
    AV_CPLACE_SPIRIT_ICE_GRAVE      = 4,
    AV_CPLACE_SPIRIT_FROSTWOLF      = 5,
    AV_CPLACE_SPIRIT_FROST_HUT      = 6,
    AV_CPLACE_SPIRIT_MAIN_ALLIANCE  = 7,
    AV_CPLACE_SPIRIT_MAIN_HORDE     = 8,
//i don't will add for all 4 positions a variable.. i think one is enough to compute the rest
    AV_CPLACE_DEFENSE_STORM_AID      = 9,
    AV_CPLACE_DEFEMSE_STORM_GRAVE    = 13,
    AV_CPLACE_DEFENSE_STONE_GRAVE    = 17,
    AV_CPLACE_DEFENSE_SNOWFALL       = 21,
    AV_CPLACE_DEFENSE_FROSTWOLF      = 25,
    AV_CPLACE_DEFENSE_ICE_GRAVE      = 29,
    AV_CPLACE_DEFENSE_FROST_HUT      = 33,

    AV_CPLACE_DEFENSE_DUN_S          = 37,
    AV_CPLACE_DEFENSE_DUN_N          = 41,
    AV_CPLACE_DEFENSE_ICEWING        = 45,
    AV_CPLACE_DEFENSE_STONE_TOWER    = 49,
    AV_CPLACE_DEFENSE_ICE_TOWER      = 53,
    AV_CPLACE_DEFENSE_TOWERPOINT     = 57,
    AV_CPLACE_DEFENSE_FROST_E        = 61,
    AV_CPLACE_DEFENSE_FROST_t        = 65,

    AV_CPLACE_A_MARSHAL_SOUTH       = 69,
    AV_CPLACE_A_MARSHAL_NORTH       = 70,
    AV_CPLACE_A_MARSHAL_ICE         = 71,
    AV_CPLACE_A_MARSHAL_STONE       = 72,
    AV_CPLACE_H_MARSHAL_ICE         = 73,
    AV_CPLACE_H_MARSHAL_TOWER       = 74,
    AV_CPLACE_H_MARSHAL_ETOWER      = 75,
    AV_CPLACE_H_MARSHAL_WTOWER      = 76,
    //irondeep
    //miner:
    AV_CPLACE_MINE_N_1_MIN      = 77,
    AV_CPLACE_MINE_N_1_MAX      = 106,
    //special types
    AV_CPLACE_MINE_N_2_MIN      = 107,
    AV_CPLACE_MINE_N_2_MAX      = 132,
    //boss
    AV_CPLACE_MINE_N_3          = 133,
    //coldtooth
    //miner:
    AV_CPLACE_MINE_S_1_MIN      = 134,
    AV_CPLACE_MINE_S_1_MAX      = 160,
    //special types
    AV_CPLACE_MINE_S_2_MIN      = 161,
    AV_CPLACE_MINE_S_2_MAX      = 179,
    //vermin
    AV_CPLACE_MINE_S_S_MIN      = 180,
    AV_CPLACE_MINE_S_S_MAX      = 189,
    //boss
    AV_CPLACE_MINE_S_3          = 190,

    //herald
    AV_CPLACE_HERALD          = 191,

    //node aura triggers
    AV_CPLACE_TRIGGER01       = 192,
    AV_CPLACE_TRIGGER02       = 193,
    AV_CPLACE_TRIGGER03       = 194,
    AV_CPLACE_TRIGGER04       = 195,
    AV_CPLACE_TRIGGER05       = 196,
    AV_CPLACE_TRIGGER06       = 197,
    AV_CPLACE_TRIGGER07       = 198,
    AV_CPLACE_TRIGGER08       = 199,
    AV_CPLACE_TRIGGER09       = 200,
    AV_CPLACE_TRIGGER10       = 201,
    AV_CPLACE_TRIGGER11       = 202,
    AV_CPLACE_TRIGGER12       = 203,
    AV_CPLACE_TRIGGER13       = 204,
    AV_CPLACE_TRIGGER14       = 205,
    AV_CPLACE_TRIGGER15       = 206,

    //boss, captain triggers
    AV_CPLACE_TRIGGER16 = 207,
    AV_CPLACE_TRIGGER17 = 208,
    AV_CPLACE_TRIGGER18 = 209,
    AV_CPLACE_TRIGGER19 = 210,

    AV_CPLACE_IVUS      = 211,
    AV_CPLACE_LOKHOLAR  = 212,
    AV_CPLACE_MAX       = 213
};

//x, y, z, o
const float BG_AV_CreaturePos[AV_CPLACE_MAX][4] =
{
    //spiritguides
    {643.000000f, 44.000000f, 69.740196f, -0.001854f},
    {676.000000f, -374.000000f, 30.000000f, -0.001854f},
    {73.417755f, -496.433105f, 48.731918f, -0.001854f},
    {-157.409195f, 31.206272f, 77.050598f, -0.001854f},
    {-531.217834f, -405.231384f, 49.551376f, -0.001854f},
    {-1090.476807f, -253.308670f, 57.672371f, -0.001854f},
    {-1496.065063f, -333.338409f, 101.134804f, -0.001854f},
    {873.001770f, -491.283630f, 96.541931f, -0.001854f},
    {-1437.670044f, -610.088989f, 51.161900f, -0.001854f},
 //grave
 //firstaid
    {635.17f, -29.5594f, 46.5056f, 4.81711f},
    {642.488f, -32.9437f, 46.365f, 4.67748f},
    {642.326f, -27.9442f, 46.9211f, 4.59022f},
    {635.945f, -33.6171f, 45.7164f, 4.97419f},
   //stormpike
    {669.272f, -297.304f, 30.291f, 4.66604f},
    {674.08f, -292.328f, 30.4817f, 0.0918785f},
    {667.01f, -288.532f, 29.8809f, 1.81583f},
    {664.153f, -294.042f, 30.2851f, 3.28531f},
  //stone
    {81.7027f, -406.135f, 47.7843f, 0.598464f},
    {78.1431f, -409.215f, 48.0401f, 5.05953f},
    {73.4135f, -407.035f, 46.7527f, 3.34736f},
    {78.2258f, -401.859f, 46.4202f, 2.05852f},
  //snowfall
    {-207.412f, -110.616f, 78.7959f, 2.43251f},
    {-197.95f, -112.205f, 78.5686f, 6.22441f},
    {-202.709f, -116.829f, 78.4358f, 5.13742f},
    {-202.059f, -108.314f, 78.5783f, 5.91968f},
  //ice
    {-615.501f, -393.802f, 60.4299f, 3.06147f},
    {-608.513f, -392.717f, 62.5724f, 2.06323f},
    {-609.769f, -400.072f, 60.7174f, 5.22367f},
    {-616.093f, -398.293f, 60.5628f, 3.73613f},
  //frost
    {-1078.209595f, -345.622284f, 55.120140f, 0.118623f},
    {-1082.588867f, -342.263153f, 55.381741f, 1.767959f},
    {-1086.708130f, -347.341187f, 54.866028f, 3.330910f},
    {-1082.402832f, -349.693390f, 54.344894f, 4.799599f},
  //frost hut
    {-1408.95f, -311.69f, 89.2536f, 4.49954f},
    {-1407.15f, -305.323f, 89.1993f, 2.86827f},
    {-1400.64f, -304.3f, 89.7008f, 1.0595f},
    {-1400.4f, -311.35f, 89.3028f, 4.99434f},
  //towers
  //dun south - OK
    {568.914246f, -100.307686f, 52.682915f, 5.440790f},
    {575.022827f, -84.309898f, 52.322861f, 0.147205f},
    {573.925964f, -92.112442f, 52.516998f, 6.018055f},
    {571.295288f, -75.376144f, 52.399460f, 0.740197f},
    //dun north - OK
    {665.633362f, -147.043579f, 64.042816f, 3.976018f},
    {662.062256f, -138.929443f, 64.181999f, 3.500852f},
    {661.588318f, -129.950333f, 64.116409f, 2.848972f},
    {666.711548f, -122.087128f, 63.991318f, 2.299192f},
    //icewing - OK
    {216.011368f, -383.417877f, 56.935673f, 5.165906f},
    {226.461517f, -368.704895f, 56.937405f, 6.198711f},
    {202.002472f, -384.578705f, 56.741547f, 4.304491f},
    {190.733704f, -370.037201f, 57.101086f, 3.271692f},
    //stone
    {-166.656372f, -435.805756f, 41.090187f, 2.177437f},
    {-174.757858f, -452.272278f, 40.719578f, 3.358144f},
    {-153.340210f, -432.720001f, 40.907352f, 1.368480f},
    {-138.427628f, -452.355286f, 40.870827f, 6.210467f},
    //ice - OK
    {-569.784912f, -254.967239f, 75.008438f, 1.219262f},
    {-563.083801f, -260.434692f, 74.907593f, 0.292486f},
    {-565.846069f, -269.414520f, 74.909058f, 5.491826f},
    {-575.057434f, -271.907104f, 74.909615f, 4.407979f},
    //towerpoint
    {-773.777405f, -372.351959f, 90.845192f, 4.282271f},
    {-762.619568f, -370.581940f, 90.798088f, 5.303292f},
    {-758.723999f, -362.354401f, 90.797760f, 0.033268f},
    {-773.558594f, -354.877625f, 90.792976f, 2.122456f},
    //frost etower
    {-1293.561279f, -317.439209f, 107.329025f, 6.167287f},
    {-1297.407104f, -309.716370f, 107.329025f, 0.967950f},
    {-1293.245483f, -317.422028f, 113.767494f, 6.171220f},
    {-1297.423340f, -308.650879f, 113.769562f, 1.015079f},
    //frost wtower
    {-1292.862793f, -273.093597f, 114.152260f, 5.538955f},
    {-1301.115234f, -275.592529f, 114.050522f, 4.404052f},
    {-1286.169678f, -264.934113f, 114.090889f, 0.225726f},
    {-1288.731201f, -264.488342f, 107.612099f, 0.186462f},

    //alliance marshall
    {721.104f, -7.64155f, 50.7046f, 3.45575f}, // south
    {723.058f, -14.1548f, 50.7046f, 3.40339f}, // north
    {715.691f, -4.72233f, 50.2187f, 3.47321f}, // icewing
    {720.046f, -19.9413f, 50.2187f, 3.36849f}, // stone
//horde  (coords not 100% ok)
    {-1376.421631f, -225.940369f, 98.426949f, 5.303f},
    {-1373.523193f, -221.817490f, 98.426964f, 5.222f}, //tower point warmaster
    {-1366.671753f, -218.841721f, 98.425537f, 4.922f},
    {-1362.239136f, -220.283508f, 98.445503f, 4.965f},

 //irondeep mine
 //Irondeep Trogg
    {955.601379f, -434.012970f, 56.015236f, 1.885757f},
    {803.144897f, -316.435516f, 53.622078f, 1.285982f},
    {958.692f, -333.477f, 63.2276f, 5.77704f},
    {817.136963f, -342.772064f, 49.459530f, 4.027038f},
    {941.054077f, -447.082245f, 58.374416f, 3.959218f},
    {834.263672f, -348.329865f, 47.604561f, 0.245344f},
    {931.751f, -403.458f, 59.6737f, 5.63741f},
    {837.157959f, -359.812561f, 48.892204f, 0.995415f},
    {929.702f, -412.401f, 56.8776f, 5.89921f},
    {848.907410f, -373.271667f, 48.654205f, 4.556008f},
    {921.972f, -358.597f, 66.4313f, 2.93215f},
    {819.556335f, -390.508057f, 49.656734f, 2.841094f},
    {912.516174f, -416.598541f, 59.262741f, 2.804666f},
    {819.020264f, -397.920746f, 49.316509f, 3.088495f},
    {913.075195f, -376.015411f, 66.668549f, 2.773252f},
    {836.229797f, -378.438263f, 48.552719f, 0.190377f},
    {913.064f, -395.773f, 60.1364f, 4.41568f},
    {850.688843f, -407.804321f, 51.765297f, 1.395984f},
    {909.246f, -474.576f, 58.2901f, 0.226893f},
    {877.268799f, -413.026642f, 53.719540f, 1.392051f},
    {905.973f, -459.528f, 58.7594f, 1.37189f},
    {883.905090f, -419.905426f, 54.035358f, 1.152504f},
    {901.809f, -457.709f, 59.0116f, 3.52557f},
    {797.457336f, -340.081268f, 63.335228f, 4.325561f},
    {897.929f, -471.742f, 59.7729f, 2.54818f},
    {809.121826f, -447.277313f, 50.460926f, 2.519077f},
    {890.584f, -406.049f, 61.1925f, 5.67232f},
    {809.966187f, -468.283112f, 48.355762f, 4.297993f},
    {887.647f, -391.537f, 61.8734f, 1.37881f},   
    //irondeep skullthumber irondeep shaman
    {820.430054f, -467.319946f, 49.042698f, 4.549321f},
    {937.378f, -377.816f, 65.3919f, 3.56047f},
    {826.394287f, -419.353668f, 48.536247f, 1.858236f},
    {922.918f, -396.634f, 60.3942f, 2.71695f},
    {870.551636f, -426.659393f, 50.865738f, 2.758605f},
    {907.893f, -388.787f, 61.7923f, 5.74213f},
    {836.748352f, -417.295807f, 47.319965f, 2.129146f},
    {884.237f, -407.597f, 61.566f, 0.820305f},
    {909.567200f, -422.668823f, 60.329109f, 2.318811f},
    {876.047f, -341.857f, 65.8743f, 4.45059f},
    {937.096863f, -391.753754f, 62.340950f, 2.958911f},
    {871.914f, -404.209f, 62.1269f, 6.06163f},
    {931.249695f, -361.074677f, 66.030922f, 4.042747f},
    {909.076538f, -436.938446f, 57.617706f, 2.839916f},
    {845.172974f, -393.921112f, 62.876377f, 4.396197f},
    {930.398438f, -342.587616f, 65.207863f, 4.304680f},
    {858.818665f, -384.684387f, 62.787159f, 1.490222f},
    {868.294f, -392.395f, 61.4772f, 4.38685f},
    {842.492126f, -367.177429f, 47.793766f, 3.110952f},
    {867.804f, -392.51f, 61.5089f, 2.30167f},
    {940.570557f, -429.530609f, 56.349491f, 3.26f},
    {858.593f, -439.614f, 50.2184f, 0.872665f},
    {973.887634f, -459.581268f, 59.718048f, 0.186473f},
    {846.939f, -347.279f, 66.2876f, 0.942478f},
    {968.620911f, -465.608063f, 59.049736f, 4.608271f},
    {838.358f, -371.212f, 63.3299f, 4.04916f},
    {868.598389f, -436.811432f, 50.972420f, 2.157690f},
    // Morloch
    {865.554f, -438.735f, 50.7333f, 2.12431f},
    //coldtooth mine
    //miner/digger
    {-917.648f, -46.8922f, 77.0872f, 5.27089f},
    {-912.689f, -45.4494f, 76.2277f, 4.60767f},
    {-905.455f, -84.5179f, 75.3642f, 3.29867f},
    {-904.332f, -111.509f, 75.5925f, 2.47837f},
    {-904.27f, -160.419f, 61.9876f, 3.61192f},
    {-904.023f, -90.4558f, 75.3706f, 3.40339f},
    {-978.678f, -37.3136f, 75.8364f, 2.84489f},
    {-973.076f, -36.5013f, 77.5047f, 1.0821f},
    {-963.951f, -87.734f, 81.5555f, 0.575959f},
    {-961.941f, -90.7252f, 81.6629f, 0.820305f},
    {-957.623f, -186.582f, 66.6021f, 1.95477f},
    {-952.476f, -179.778f, 78.6771f, 4.5204f},
    {-950.427f, -115.007f, 79.6127f, 3.68264f},
    {-956.535645f, -155.720963f, 80.429810f, 2.188055f},
    {-950.169f, -188.099f, 66.6184f, 5.55015f},
    {-949.944f, -142.977f, 80.5382f, 2.70526f},
    {-947.854f, -170.5f, 79.7618f, 0.942478f},
    {-946.738f, -139.567f, 80.0904f, 2.3911f},
    {-945.503f, -65.0654f, 79.7907f, 5.02655f},
    {-943.678f, -110.986f, 80.2557f, 0.959931f},
    {-942.993f, -56.9881f, 79.8915f, 5.65487f},
    {-938.197f, -155.838f, 61.3111f, 1.65806f},
    {-946.049927f, -194.157623f, 76.938774f, 2.148799f},
    {-929.947f, -154.449f, 61.5084f, 1.67552f},
    {-927.412f, -135.313f, 61.1987f, 3.29867f},
    {-920.677f, -156.859f, 62.8033f, 3.15306f},
    {-916.75f, -136.094f, 62.2357f, 0.0698132f},  
    //special
    {-954.622f, -110.958f, 80.7911f, 6.24828f},
    {-951.477f, -53.9647f, 80.0235f, 5.32325f},
    {-946.812f, -126.04f, 78.8601f, 5.15265f},
    {-940.689f, -140.707f, 79.9225f, 2.79253f},
    {-847.274597f, -95.510231f, 68.543884f, 3.516410f},
    {-922.537f, -130.291f, 61.3756f, 4.95674f},
    {-915.862f, -151.74f, 76.9427f, 0.942478f},
    {-888.321f, -159.831f, 62.5303f, 1.20428f},
    {-874.361f, -42.4751f, 69.4316f, 0.785398f},
    {-873.19f, -50.4899f, 70.0568f, -2.41288f},
    {-868.511f, -148.386f, 62.3547f, 3.57875f},
    {-868.44f, -121.649f, 64.5056f, 3.33358f},
    {-868.324f, -77.7196f, 71.4768f, 5.41052f},
    {-859.846f, -19.6549f, 70.7304f, 1.97222f},
    {-828.05f, -150.508f, 62.2019f, 2.14675f},
    {-826.254f, -58.6911f, 72.0041f, 3.68264f},
    {-976.086f, -44.1775f, 76.029f, 1.46608f},
    {-851.340088f, -90.151855f, 68.514587f, 4.203633f},
    {-966.551f, -74.1111f, 80.0243f, 4.2129f},
    //vermin (s.th special for this mine)
    {-953.469482f, -216.733231f, 77.830673f, 5.812748f},
    {-957.264893f, -168.975662f, 77.869316f, 1.049314f},
    {-933.494f, -209.063f, 73.7803f, 5.88176f},
    {-929.666f, -201.308f, 73.7032f, 5.02655f},
    {-978.997f, -249.356f, 65.4345f, 5.05464f},
    {-974.565f, -224.828f, 69.5858f, 4.88846f},
    {-946.514f, -259.239f, 66.0874f, 3.78132f},
    {-918.402f, -250.439f, 69.5271f, 2.21352f},
    {-910.14f, -229.959f, 72.9279f, 0.27677f},
    {-868.576965f, -108.824135f, 64.693817f, 2.046769f},
    //boss
    {-849.276733f, -92.219147f, 68.529945f, 4.002411f},
    //herald
    {-48.459f, -288.802f, 55.47f, 1.0f},
    //triggers
    {637.083f, -32.6603f, 45.9715f, 1.14353f},         //firstaid_station
    {669.007f, -294.078f, 30.2909f, 2.77507f},     //stormpike_grave
    {77.8013f, -404.7f, 46.7549f, -0.872665f},     //stoneheart_grave
    {-202.581f, -112.73f, 78.4876f, -0.715585f},   //snowfall_grave
    {-611.962f, -396.17f, 60.8351f, 2.53682f},     //iceblood_grave
    {-1082.45f, -346.823f, 54.9219f, -1.53589f},   //frostwolf_grave
    {-1402.21f, -307.431f, 89.4424f, 0.191986f},   //frostwolf_hut
    {553.779f, -78.6566f, 51.9378f, -1.22173f},    //dunbaldar_south
    {674.001f, -143.125f, 63.6615f, 0.994838f},    //dunbaldar_north
    {203.281f, -360.366f, 56.3869f, -0.925024f},    //icewing_bunker
    {-152.437f, -441.758f, 40.3982f, -1.95477f},   //stoneheart_bunker
    {-571.88f, -262.777f, 75.0087f, -0.802851f},   //iceblood_tower
    {-768.907f, -363.71f, 90.8949f, 1.07991f},     //tower_point
    {-1302.9f, -316.981f, 113.867f, 2.00713f},     //frostwolf_etower
    {-1297.5f, -266.767f, 114.15f, 3.31044f},      //frostwolf_wtower
    {-57.7891f, -286.597f, 15.6479f, 6.02139f},    //AV_NPC_A_CAPTAIN balinda
    {722.43f, -10.9982f, 50.7046f, 3.42085f},      //AV_NPC_A_BOSS vanndar
    {-545.23f, -165.35f, 57.7886f, 3.01145f},      //AV_NPC_H_CAPTAIN galvangar
    {-1370.432373f, -219.406540f, 98.427879f, 5.059814f},      //AV_NPC_H_BOSS drek thar
    {-225.569595f, -306.301392f, 6.667585f, 3.053098f},
    {-285.473816f, -292.691681f, 6.694710f, 6.124003f}
};

enum BG_AV_CreatureIds
{
    AV_NPC_A_TOWERDEFENSE_0 = 0,     // stormpike bowman

    AV_NPC_A_GRAVEDEFENSE0,// stormpike Defender
    AV_NPC_A_GRAVEDEFENSE1,     // seasoned defender
    AV_NPC_A_GRAVEDEFENSE2,    // veteran defender
    AV_NPC_A_GRAVEDEFENSE3,     // champion defender
    AV_NPC_A_CAPTAIN,     // balinda
    AV_NPC_A_BOSS,     // vanndar

    AV_NPC_H_TOWERDEFENSE_0,     // frostwolf bowman

    AV_NPC_H_GRAVEDEFENSE0,     // frostwolf guardian
    AV_NPC_H_GRAVEDEFENSE1,     // seasoned guardian
    AV_NPC_H_GRAVEDEFENSE2,    // veteran guardian
    AV_NPC_H_GRAVEDEFENSE3,    // champion guardian

    AV_NPC_H_CAPTAIN,    // galvangar
    AV_NPC_H_BOSS,    // drek thar

    AV_NPC_A_MARSHAL_SOUTH,
    AV_NPC_MARSHAL_NORTH,
    AV_NPC_A_MARSHAL_ICE,
    AV_NPC_A_MARSHAL_STONE,

    AV_NPC_H_MARSHAL_ICE,
    AV_NPC_H_MARSHAL_TOWER,
    AV_NPC_MARSHAL_ETOWER,
    AV_NPC_H_MARSHAL_WTOWER,

    AV_NPC_N_MINE_N_1,
    AV_NPC_N_MINE_N_2,
    AV_NPC_N_MINE_N_3,
    AV_NPC_N_MINE_N_4,

    AV_NPC_N_MINE_A_1,
    AV_NPC_N_MINE_A_2,
    AV_NPC_N_MINE_A_3,
    AV_NPC_N_MINE_A_4,

    AV_NPC_N_MINE_H_1,
    AV_NPC_N_MINE_H_2,
    AV_NPC_N_MINE_H_3,
    AV_NPC_N_MINE_H_4,

    AV_NPC_S_MINE_N_1,
    AV_NPC_S_MINE_N_2,
    AV_NPC_S_MINE_N_3,
    AV_NPC_S_MINE_N_4,
    AV_NPC_S_MINE_N_S,

    AV_NPC_S_MINE_A_1,
    AV_NPC_S_MINE_A_2,
    AV_NPC_S_MINE_A_3,
    AV_NPC_S_MINE_A_4,

    AV_NPC_S_MINE_H_1,
    AV_NPC_S_MINE_H_2,
    AV_NPC_S_MINE_H_3,
    AV_NPC_S_MINE_H_4,

    AV_NPC_HERALD,

    AV_NPC_IVUS,
    AV_NPC_LOKHOLAR,

    AV_NPC_INFO_MAX = 50
};

//entry, team, minlevel, maxlevel
//TODO this array should be removed, the only needed things are the entrys (for spawning(?) and handlekillunit)
const uint32 BG_AV_CreatureInfo[AV_NPC_INFO_MAX][4] =
{
    { 13358, TEAM_ALLIANCE, 58, 58 }, //Stormpike Bowman //i think its 60, 61 and 69, 70.. but this is until now not possible TODO look if this is ok

    { 12050, TEAM_ALLIANCE, 58, 58 }, //Stormpike Defender
    { 13326, TEAM_ALLIANCE, 59, 59 }, //Seasoned Defender
    { 13331, TEAM_ALLIANCE, 60, 60 }, //Veteran Defender
    { 13422, TEAM_ALLIANCE, 61, 61 }, //Champion Defender
    { 11949, TEAM_ALLIANCE, 0, 0}, //not spawned with this data, but used for handlekillunit
    { 11948, TEAM_ALLIANCE, 0, 0}, //not spawned with this data, but used for handlekillunit

    { 13359, TEAM_HORDE, 58, 58 }, //Frostwolf Bowman

    { 12053, TEAM_HORDE, 58, 58 }, //Frostwolf Guardian
    { 13328, TEAM_HORDE, 59, 59 }, //Seasoned Guardian
    { 13332, TEAM_HORDE, 60, 60 }, //Veteran Guardian
    { 13421, TEAM_HORDE, 61, 61 }, //Champion Guardian

    { 11947, TEAM_HORDE, 0, 0}, //not spawned with this data, but used for handlekillunit
    { 11946, TEAM_HORDE, 0, 0}, //not spawned with this data, but used for handlekillunit

    { 14763, TEAM_ALLIANCE, 60, 60 }, //Dun Baldar South Marshal
    { 14762, TEAM_ALLIANCE, 60, 60 }, //Dun Baldar North Marshal
    { 14764, TEAM_ALLIANCE, 60, 60 }, //Icewing Marshal
    { 14765, TEAM_ALLIANCE, 60, 60 }, //Stonehearth Marshal

    { 14773, TEAM_HORDE, 60, 60 }, //Iceblood Warmaster
    { 14776, TEAM_HORDE, 60, 60 }, //Tower Point Warmaster
    { 14772, TEAM_HORDE, 60, 60 }, //East Frostwolf Warmaster
    { 14777, TEAM_HORDE, 60, 60 }, //West Frostwolf Warmaster

    { 10987, 59, 52, 53 }, //Irondeep Trogg
    { 11600, 59, 53, 54 }, //Irondeep Shaman
    { 11602, 59, 54, 55 }, //Irondeep Skullthumper
    { 11657, 59, 58, 58 }, //Morloch

    {13396, TEAM_ALLIANCE, 52, 53}, //irondeep alliance TODO: get the right ids
    {13080, TEAM_ALLIANCE, 53, 54},
    {13098, TEAM_ALLIANCE, 54, 55},
    {13078, TEAM_ALLIANCE, 58, 58},

    {13397, TEAM_HORDE, 52, 53}, //irondeep horde
    {13099, TEAM_HORDE, 53, 54},
    {13081, TEAM_HORDE, 54, 55},
    {13079, TEAM_HORDE, 58, 58},

    { 11603, 59, 52, 53 }, //south mine neutral
    { 11604, 59, 53, 54 },
    { 11605, 59, 54, 55 },
    { 11677, 59, 58, 58 },
    { 10982, 59, 52, 53 }, //vermin

    {13317, TEAM_ALLIANCE, 52, 53}, //alliance
    {13096, TEAM_ALLIANCE, 54, 55}, //explorer
    {13087, TEAM_ALLIANCE, 54, 55}, //invader
    {13086, TEAM_ALLIANCE, 58, 58},

    {13316, TEAM_HORDE, 52, 53}, //horde
    {13097, TEAM_HORDE, 54, 55}, //surveypr
    {13089, TEAM_HORDE, 54, 55}, //guard
    {13088, TEAM_HORDE, 58, 58},
    {14848, TEAM_HORDE, 58, 58},  //Herald
    {13419, TEAM_ALLIANCE, 61, 61},  //Ivus the forest lord
    {13256, TEAM_HORDE, 61, 61}      //Lokholar the Ice lord
};

const uint32 BG_AV_TOTALNPCS = (AV_CPLACE_MAX + AV_STATICCPLACE_MAX);
//x, y, z, o, static_creature_info-id
const float BG_AV_StaticCreaturePos[AV_STATICCPLACE_MAX][5] =
{
    //static creatures
    {-1235.31f, -340.777f, 60.5088f, 3.31613f, 0 }, //2225 - Zora Guthrek
    {-1244.02f, -323.795f, 61.0485f, 5.21853f, 1 }, //3343 - Grelkor
    {-1235.16f, -332.302f, 60.2985f, 2.96706f, 2 }, //3625 - Rarck
    {587.303f, -42.8257f, 37.5615f, 5.23599f, 3 }, //4255 - Brogus Thunderbrew
    {643.635f, -58.3987f, 41.7405f, 4.72984f, 4 }, //4257 - Lana Thunderbrew
    {591.464f, -44.452f, 37.6166f, 5.65487f, 5 }, //5134 - Jonivera Farmountain
    {608.515f, -33.3935f, 42.0003f, 5.41052f, 6 }, //5135 - Svalbrad Farmountain
    {617.656f, -32.0701f, 42.7168f, 4.06662f, 7 }, //5139 - Kurdrum Barleybeard
    {-1183.76f, -268.295f, 72.8233f, 3.28122f, 8 }, //10364 - Yaelika Farclaw
    {-1187.86f, -275.31f, 73.0481f, 3.63028f, 9 }, //10367 - Shrye Ragefist
    {-1008.42f, -368.006f, 55.3426f, 5.95647f, 10 }, //10981 - Frostwolf
    {-1309.500854f, -501.240509f, 52.042721f, 0.158919f, 10 }, //10981 - Frostwolf
    {-558.455f, -198.768f, 58.1755f, 4.97946f, 10 }, //10981 - Frostwolf
    {-861.247f, -312.51f, 55.1427f, 3.35382f, 10 }, //10981 - Frostwolf
    {-1003.81f, -395.913f, 50.4736f, 2.85631f, 10 }, //10981 - Frostwolf
    {-904.5f, -289.815f, 65.1222f, 5.7847f, 10 }, //10981 - Frostwolf
    {-1064.41f, -438.839f, 51.3614f, 1.88857f, 10 }, //10981 - Frostwolf
    {258.814f, 76.2017f, 18.6468f, 6.19052f, 11 }, //10986 - Snowblind Harpy
    {265.838f, -315.846f, -16.5429f, 3.15917f, 11 }, //10986 - Snowblind Harpy
    {426.485f, -51.1927f, -5.66286f, 1.60347f, 11 }, //10986 - Snowblind Harpy
    {452.044f, -33.9594f, -0.044651f, 2.72815f, 11 }, //10986 - Snowblind Harpy
    {267.539978f, -166.320435f, -42.987259f, 5.274699f, 11 }, //10986 - Snowblind Harpy
    {532.64f, -54.5863f, 20.7024f, 2.93215f, 11 }, //10986 - Snowblind Harpy
    {295.183f, -299.908f, -34.6123f, 0.135851f, 12 }, //10990 - Alterac Ram
    {421.08f, -225.006f, -23.73f, 0.166754f, 12 }, //10990 - Alterac Ram
    {-55.7766f, -192.498f, 20.4352f, 6.12221f, 12 }, //10990 - Alterac Ram
    {527.887f, -477.223f, 62.3559f, 0.170935f, 12 }, //10990 - Alterac Ram
    {389.144f, -346.508f, -30.334f, 4.14117f, 12 }, //10990 - Alterac Ram
    {108.121f, -322.248f, 37.5655f, 4.46788f, 12 }, //10990 - Alterac Ram
    {507.479f, -67.9403f, 10.3571f, 3.26304f, 12 }, //10990 - Alterac Ram
    {329.071f, -185.016f, -29.1542f, 0.356943f, 12 }, //10990 - Alterac Ram
    {-108.209709, -367.361938, 16.876339, 2.448416, 12 }, //10990 - Alterac Ram
    {358.882f, -118.061f, -24.9119f, 2.29257f, 12 }, //10990 - Alterac Ram
    {487.151f, -174.229f, 14.7558f, 4.73192f, 12 }, //10990 - Alterac Ram
    {449.652f, -123.561f, 6.14273f, 6.12029f, 12 }, //10990 - Alterac Ram
    {272.419f, -261.802f, -41.8835f, 3.66559f, 12 }, //10990 - Alterac Ram
    {359.021f, -210.954f, -29.3483f, 4.31339f, 12 }, //10990 - Alterac Ram
    {450.598f, -318.048f, -37.7548f, 0.655219f, 12 }, //10990 - Alterac Ram
    {509.333f, -218.2f, 3.05439f, 3.66292f, 12 }, //10990 - Alterac Ram
    {485.771f, -223.613f, -1.53f, 2.04862f, 12 }, //10990 - Alterac Ram
    {486.636f, -452.172f, 39.6592f, 2.3341f, 12 }, //10990 - Alterac Ram
    {702.783f, -257.494f, 25.9777f, 1.68329f, 12 }, //10990 - Alterac Ram
    {460.942f, -199.263f, -6.0149f, 0.380506f, 12 }, //10990 - Alterac Ram
    {483.108f, -115.307f, 10.1056f, 3.69701f, 12 }, //10990 - Alterac Ram
    {471.601f, -154.174f, 14.0702f, 5.5807f, 12 }, //10990 - Alterac Ram
    {213.938f, -420.793f, 41.2549f, 5.71394f, 12 }, //10990 - Alterac Ram
    {289.387f, -294.685f, -33.9073f, 0.555494f, 12 }, //10990 - Alterac Ram
    {155.649f, -402.891f, 43.3915f, 5.94838f, 12 }, //10990 - Alterac Ram
    {517.184f, -295.105f, -9.78195f, 6.05668f, 12 }, //10990 - Alterac Ram
    {-109.968834f, -224.004898f, 17.976637f, 3.913121, 12 }, //10990 - Alterac Ram
    {320.244f, -107.793f, -42.6357f, -1.00311f, 12 }, //10990 - Alterac Ram
    {217.976f, 110.774f, 15.7603f, 4.56793f, 13 }, //11675 - Snowblind Windcaller
    {269.872f, 6.66684f, 20.7592f, 0.381212f, 13 }, //11675 - Snowblind Windcaller
    {313.528f, -319.041f, -27.2373f, 0.554098f, 13 }, //11675 - Snowblind Windcaller
    {435.441f, -39.9289f, -0.169651f, 0.549454f, 13 }, //11675 - Snowblind Windcaller
    {461.502563f, -210.149612f, -8.139971f, 3.637167f, 13 }, //11675 - Snowblind Windcaller
    {428.091f, -122.731f, 3.40332f, 6.05901f, 14 }, //11678 - Snowblind Ambusher
    {235.05f, 85.5705f, 18.3079f, -0.914255f, 14 }, //11678 - Snowblind Ambusher
    {-1553.04f, -344.342f, 64.4163f, 6.09933f, 15 }, //11839 - Wildpaw Brute
    {-545.23f, -165.35f, 57.7886f, 5.8923f, 16 }, //11947 - Captain Galvangar
    {722.43f, -10.9982f, 50.7046f, 3.42085f, 17 }, //11948 - Vanndar Stormpike
    {-57.7891f, -286.597f, 15.6479f, 6.02139f, 18 }, //11949 - Captain Balinda Stonehearth
    {930.498f, -520.755f, 93.7334f, 1.8326f, 19 }, //11997 - Stormpike Herald
    {-776.092f, -345.161f, 67.4092f, 1.89257f, 20 }, //12051 - Frostwolf Legionnaire  
    {587.633f, -45.9816f, 37.5438f, 5.81195f, 21 }, //12096 - Stormpike Quartermaster
    {-1293.79f, -194.407f, 72.4398f, 5.84685f, 22 }, //12097 - Frostwolf Quartermaster   
    {-1251.5f, -316.327f, 62.6565f, 5.02655f, 24 }, //13176 - Smith Regzar
    {214.505280f, -358.609039f, 56.374683f, 4.914503f, 25}, //13179 - Wing Commander Guse
    {569.983f, -94.9992f, 38.0325f, 1.39626f, 26 }, //13216 - Gaelden Hammersmith
    {-1244.92f, -308.916f, 63.2525f, 1.62316f, 27 }, //13218 - Grunnda Wolfheart
    {-1319.56f, -342.675f, 60.3404f, 1.20428f, 28 }, //13236 - Primalist Thurloga
    {647.61f, -61.1548f, 41.7405f, 4.24115f, 29 }, //13257 - Murgot Deepforge
    {-1321.64f, -343.73f, 60.4833f, 1.01229f, 30 }, //13284 - Frostwolf Shaman
    {-1317.61f, -342.853f, 60.3726f, 2.47837f, 30 }, //13284 - Frostwolf Shaman
    {-1319.31f, -344.475f, 60.3825f, 1.72788f, 30 }, //13284 - Frostwolf Shaman
    {-767.736145f, -360.551605f, 68.576851f, 1.089667f, 31 }, //13438 - Wing Commander Slidore
    {729.2f, -78.812f, 51.6335f, 3.97935f, 32 }, //13442 - Arch Druid Renferal
    {729.118f, -82.8713f, 51.6335f, 2.53073f, 33 }, //13443 - Druid of the Grove
    {725.554f, -79.4973f, 51.6335f, 5.27089f, 33 }, //13443 - Druid of the Grove
    {724.768f, -84.1642f, 51.6335f, 0.733038f, 33 }, //13443 - Druid of the Grove
    {596.68f, -83.0633f, 39.0051f, 6.24828f, 34 }, //13447 - Corporal Noreg Stormpike
    {600.032f, -2.92475f, 42.0788f, 5.00909f, 35}, //13577 - Stormpike Ram Rider Commander
    {610.239f, -21.8454f, 43.272f, 4.90438f, 36}, //13617 - Stormpike Stable Master
    {613.422f, -150.764f, 33.4517f, 5.55015f, 37}, //13797 - Mountaineer Boombellow
    {-1213.91f, -370.619f, 56.4455f, 0.837758f, 38}, //13798 - Jotek
    {-1271.24f, -335.766f, 62.3971f, 5.75959f, 39}, //14185 - Najak Hexxen
    {-1268.64f, -332.688f, 62.6171f, 5.28835f, 40}, //14186 - Ravak Grimtotem
    {648.363f, -65.2233f, 41.7405f, 3.12414f, 41}, //14187 - Athramanis
    {648.238f, -67.8931f, 41.7405f, 2.60054f, 42}, //14188 - Dirk Swindle
    {947.412f, -509.982f, 95.1098f, 2.82743f, 45}, //14284 - Stormpike Battleguard
    {939.42f, -502.777f, 94.5887f, 5.14872f, 45}, //14284 - Stormpike Battleguard
    {853.105896f, -493.013916f, 97.211029f, 5.381832f, 45}, //14284 - Stormpike Battleguard
    {776.621f, -487.775f, 99.4049f, 3.50811f, 45}, //14284 - Stormpike Battleguard-1428.914063
    {773.651f, -497.482f, 99.0408f, 2.11185f, 45}, //14284 - Stormpike Battleguard
    {949.1f, -506.913f, 95.4237f, 3.31613f, 45}, //14284 - Stormpike Battleguard
    {776.102783f, -496.945984f, 99.446594f, 2.263792f,45},
    {774.965576f, -486.022644f, 98.749657f, 3.677505f,45},
    {855.401001f, -491.748688f, 96.852287f, 5.436824f,45},
    {876.713684f, -504.857605f, 96.623032f, 0.343515f,45},
    {877.878052f, -507.154083f, 96.638657f, 0.512383f,45},
    {864.308228f, -509.850006f, 96.610756f, 1.761168f,45},
    {867.084595f, -509.181671f, 96.597649f, 1.988934f,45},
    {885.837097f, -498.436554f, 96.868538f, 2.680111f,45},
    {885.168823f, -501.027100f, 96.848793f, 3.049308f,45},
    {901.832764f, -516.835938f, 95.763077f, 1.694424f,45},
    {904.445374f, -517.022888f, 95.406853f, 1.502004f,45},
    {-1370.432373f, -219.406540f, 98.427879f, 5.059814f,46}, //drek thar
    {-1362.553955f, -537.779907f, 53.506645f, 1.992726f,50},        //Frostwolf Battleguard
    {-1367.785156f, -539.850220f, 54.776760f, 1.889680f,50},        //Frostwolf Battleguard
    {-1375.532227f, -531.220886f, 54.698044f, 5.562387f,50},        //Frostwolf Battleguard
    {-1373.048096f, -527.535278f, 53.493637f, 6.163217f,50},        //Frostwolf Battleguard
    {-1443.442627f, -588.411194f, 51.320053f, 5.825599f,50},        //Frostwolf Battleguard
    {-1473.833740f, -612.173340f, 50.140842f, 5.589973f,50},        //Frostwolf Battleguard
    {-1445.213867f, -591.943909f, 51.266773f, 6.076926f,50},         //Frostwolf Battleguard
    {-1448.148804f, -601.395264f, 51.217022f, 4.804545f,50},
    {-1451.260864f, -600.552429f, 51.270378f, 4.364783f,50},
    {-1423.110962f, -591.941956f, 52.308441f, 3.061007f,50},
    {-1424.038330f, -595.602783f, 51.653915f, 3.049218f,50},
    {-1451.336426f, -610.326599f, 51.365753f, 0.759804f,50},
    {-1449.708984f, -613.584961f, 51.391922f, 0.390633f,50},
    {-1476.114258f, -614.322144f, 49.720409f, 5.507518f,50},
    {-1376.520020f, -217.598007f, 99.371262f, 4.635f, 51},         //Drakan
    {-1367.670044f, -214.559998f, 99.371231f, 5.562f, 52},         //Duros
    {-1218.935425f, -351.874695f, 57.677719f, 4.576824f, 53},    //Vipore
    {-1302.182861f, -267.837952f, 91.907600f, 5.731381f, 54 },      //Ichman
    {320.564575f, -502.545074f, 71.147240f, 6.257573f, 55},     //Jeztor
    {673.819214f, -144.452133f, 63.651516f, 5.440706f, 56 },      //Mulverick
    {32.898331f, 47.241146f, 227.552811f, 3.78f, 57},
    {-1034.068726f, -512.926147f, 45.621620f, 1.647275f, 10}, //10981 - Frostwolf
    {-1130.748535f, -556.101501f, 51.517883f, 1.301700f, 10}, //10981 - Frostwolf
    {-1178.099243f, -586.272522f, 51.924316f, 1.164283f, 10}, //10981 - Frostwolf
    {-1000.727844f, -293.526611f, 65.995216f, 0.414219f, 10}, //10981 - Frostwolf
    {-819.048218f, -270.483612f, 59.093216f, 5.354395f, 10}, //10981 - Frostwolf
    {-954.355530f, -353.085693f, 62.327618f, 2.232444f, 10}, //10981 - Frostwolf
    {-941.931152f, -460.930511f, 43.309605f, 0.394494f, 10}, //10981 - Frostwolf
    {-889.778442f, -437.054016f, 43.437962f, 3.944494f, 10}, //10981 - Frostwolf
    {-1237.304199f, -641.522766f, 53.982742f, 1.796448f, 20}, //legionnaire
    {-1255.923340f, -631.339417f, 52.939537f, 0.347388f, 20}, //legionnaire
    {-1260.301147f, -585.861206f, 54.120270f, 1.399801f, 58}, //normal wolf rider
    {-1249.234863f, -637.869263f, 52.552696f, 0.971787f, 59}, //wolf rider commander
    {-1243.254639f, -638.627625f, 52.557941f, 2.420883f, 60}, //frostwolf stable master
        /*
        15 world triggers for base capturing effects
        7 per faction + 1 for snowfall GY        
        */
    {638.592f, -32.422f, 46.0608f, -0.62316f, 61 }, //firstaid station
    {669.007f, -294.078f, 30.2909f, 3.77507f, 61 }, //stormpike
    {77.8013f, -404.7f, 46.7549f, 1.872665f, 61 }, //stone grave
    {-202.581f, -112.73f, 78.4876f, 1.715585f, 61 }, //snowfall
    {-611.962f, -396.17f, 60.8351f, 3.53682f, 61 },  //iceblood grave
    {-1082.45f, -346.823f, 54.9219f, -0.53589f, 61 }, //frostwolf grave
    {-1402.21f, -307.431f, 89.4424f, 1.191986f, 61 }, //frostwolf hut
    {553.779f, -78.6566f, 51.9378f, -0.22173f, 61 }, //dunnbaldar south
    {674.001f, -143.125f, 63.6615f, 1.994838f, 61 }, //dunbaldar north
    {203.281f, -360.366f, 56.3869f, 1.925024f, 61 }, //icew
    {-152.437f, -441.758f, 40.3982f, -0.95477f, 61 }, //stone
    {-571.88f, -262.777f, 75.0087f, 1.802851f, 61 }, //ice tower
    {-768.907f, -363.71f, 90.8949f, 2.07991f, 61 },  //tower point
    {-1302.9f, -316.981f, 113.867f, 3.00713f, 61 }, //frostwolf etower
    {-1297.5f, -266.767f, 114.15f, 4.31044f, 61 },   //frostwolf wtower
    {-1211.649536f, -260.762604f, 73.201454f, 6.014042f, 62 } //Sergeant Yazra Bloodsnarl
};

const uint32 BG_AV_StaticCreatureInfo[78][4] =
{
    { 2225, TEAM_HORDE, 55, 55 }, //Zora Guthrek
    { 3343, TEAM_HORDE, 55, 55 }, //Grelkor
    { 3625, TEAM_HORDE, 55, 55 }, //Rarck
    { 4255, TEAM_ALLIANCE, 55, 55 }, //Brogus Thunderbrew
    { 4257, TEAM_ALLIANCE, 55, 55 }, //Lana Thunderbrew
    { 5134, TEAM_ALLIANCE, 55, 55 }, //Jonivera Farmountain
    { 5135, TEAM_ALLIANCE, 55, 55 }, //Svalbrad Farmountain
    { 5139, TEAM_ALLIANCE, 55, 55 }, //Kurdrum Barleybeard
    { 10364, TEAM_HORDE, 55, 55 }, //Yaelika Farclaw
    { 10367, TEAM_HORDE, 55, 55 }, //Shrye Ragefist
    { 10981, 38, 50, 51 }, //Frostwolf
    { 10986, 514, 52, 53 }, //Snowblind Harpy
    { 10990, 1274, 50, 51 }, //Alterac Ram
    { 11675, 514, 53, 53 }, //Snowblind Windcaller
    { 11678, 14, 52, 53 }, //Snowblind Ambusher
    { 11839, 39, 56, 56 }, //Wildpaw Brute
    { 11947, TEAM_HORDE, 61, 61 }, //Captain Galvangar --TODO: doubled
    { 11948, TEAM_ALLIANCE, 63, 63 }, //Vanndar Stormpike
    { 11949, TEAM_ALLIANCE, 61, 61 }, //Captain Balinda Stonehearth
    { 11997, TEAM_ALLIANCE, 60, 60 }, //Stormpike Herald
    { 12051, TEAM_HORDE, 57, 57 }, //Frostwolf Legionnaire
    { 12096, TEAM_ALLIANCE, 55, 55 }, //Stormpike Quartermaster
    { 12097, TEAM_HORDE, 55, 55 }, //Frostwolf Quartermaster
    { 12127, TEAM_ALLIANCE, 57, 57 }, //Stormpike Guardsman
    { 13176, TEAM_HORDE, 60, 60 }, //Smith Regzar
    { 13179, TEAM_HORDE, 59, 59 }, //Wing Commander Guse
    { 13216, TEAM_ALLIANCE, 58, 58 }, //Gaelden Hammersmith
    { 13218, TEAM_HORDE, 58, 58 }, //Grunnda Wolfheart
    { 13236, TEAM_HORDE, 60, 60 }, //Primalist Thurloga
    { 13257, TEAM_ALLIANCE, 60, 60 }, //Murgot Deepforge
    { 13284, TEAM_HORDE, 58, 58 }, //Frostwolf Shaman
    { 13438, TEAM_ALLIANCE, 58, 58 }, //Wing Commander Slidore
    { 13442, TEAM_ALLIANCE, 60, 60 }, //Arch Druid Renferal
    { 13443, TEAM_ALLIANCE, 60, 60 }, //Druid of the Grove
    { 13447, TEAM_ALLIANCE, 58, 58 }, //Corporal Noreg Stormpike
    { 13577, TEAM_ALLIANCE, 60, 60 }, //Stormpike Ram Rider Commander
    { 13617, TEAM_ALLIANCE, 60, 60 }, //Stormpike Stable Master
    { 13797, TEAM_ALLIANCE, 60, 61 }, //Mountaineer Boombellow
    { 13798, TEAM_HORDE, 60, 61 }, //Jotek
    { 14185, TEAM_HORDE, 59, 59 }, //Najak Hexxen
    { 14186, TEAM_HORDE, 60, 60 }, //Ravak Grimtotem
    { 14187, TEAM_ALLIANCE, 60, 60 }, //Athramanis
    { 14188, TEAM_ALLIANCE, 59, 59 }, //Dirk Swindle
    { 14282, TEAM_HORDE, 53, 54 }, //Frostwolf Bloodhound
    { 14283, TEAM_ALLIANCE, 53, 54 }, //Stormpike Owl
    { 14284, TEAM_ALLIANCE, 61, 61 }, //Stormpike Battleguard
    { 11946, TEAM_HORDE, 63, 63 }, //Drek'Thar //TODO: make the levels right (boss=0 maybe)
    { 11948, TEAM_ALLIANCE, 63, 63 }, //Vanndar Stormpike
    { 11947, TEAM_HORDE, 61, 61 }, //Captain Galvangar
    { 11949, TEAM_ALLIANCE, 61, 61 }, //Captain Balinda Stonehearth
    { 14285, TEAM_HORDE, 61, 61 }, //Frostwolf Battleguard
    { 12121, TEAM_HORDE, 61, 61 }, //Draken
    { 12122, TEAM_HORDE, 61, 61 }, //Duros
    { 13439, TEAM_ALLIANCE, 61, 61 }, //Vipore
    { 13437, TEAM_ALLIANCE, 61, 61 }, //Ichman
    { 13180, TEAM_HORDE, 61, 61 }, //Jeztor
    { 13181, TEAM_HORDE, 61, 61 }, //Mulverick, 56
    { 48918, TEAM_NEUTRAL, 61, 61 }, //57
    { 13440, TEAM_HORDE, 61, 61 }, //wolf rider 58
    { 13441, TEAM_HORDE, 61, 61 }, //wolf rider commander 59
    { 13616, TEAM_HORDE, 61, 61 }, //frostwolf stable master 60
    { 12999, TEAM_NEUTRAL, 61, 61 }, //WORLD_TRIGGER: capture point
    { 13448, TEAM_HORDE, 61, 61 } //Sergeant Yazra Bloodsnarl
};

enum BG_AV_Graveyards
{
    AV_GRAVE_STORM_AID         = 751,
    AV_GRAVE_STORM_GRAVE       = 689,
    AV_GRAVE_STONE_GRAVE       = 729,
    AV_GRAVE_SNOWFALL          = 169,
    AV_GRAVE_ICE_GRAVE         = 749,
    AV_GRAVE_FROSTWOLF         = 690,
    AV_GRAVE_FROST_HUT         = 750,
    AV_GRAVE_MAIN_ALLIANCE     = 611,
    AV_GRAVE_MAIN_HORDE        = 610
};

const uint32 BG_AV_GraveyardIds[9]=
{
    AV_GRAVE_STORM_AID,
    AV_GRAVE_STORM_GRAVE,
    AV_GRAVE_STONE_GRAVE,
    AV_GRAVE_SNOWFALL,
    AV_GRAVE_ICE_GRAVE,
    AV_GRAVE_FROSTWOLF,
    AV_GRAVE_FROST_HUT,
    AV_GRAVE_MAIN_ALLIANCE,
    AV_GRAVE_MAIN_HORDE
};

enum BG_AV_BUFF
{ //TODO add all other buffs here
    AV_BUFF_ARMOR = 21163,
    AV_BUFF_A_CAPTAIN = 23693, //the buff which the alliance captain does
    AV_BUFF_H_CAPTAIN = 22751 //the buff which the horde captain does
};
enum BG_AV_States
{
    POINT_NEUTRAL              =  0,
    POINT_ASSAULTED            =  1,
    POINT_DESTROYED            =  2,
    POINT_CONTROLED            =  3
};

enum BG_AV_WorldStates
{
    AV_Alliance_Score               = 3127,
    AV_Horde_Score                  = 3128,
    AV_SHOW_H_SCORE                 = 3133,
    AV_SHOW_A_SCORE                 = 3134,

/*
    //the comments behind the state shows which icon overlaps the other.. but is, until now, unused and maybe not a good solution (but give few performance (:)

// Graves

    // Alliance
    //Stormpike first aid station
    AV_AID_A_C                      = 1325,
    AV_AID_A_A                      = 1326,
    AV_AID_H_C                      = 1327,
    AV_AID_H_A                      = 1328,
    //Stormpike Graveyard
    AV_PIKEGRAVE_A_C                = 1333,
    AV_PIKEGRAVE_A_A                = 1335,
    AV_PIKEGRAVE_H_C                = 1334,
    AV_PIKEGRAVE_H_A                = 1336,
    //Stoneheart Grave
    AV_STONEHEART_A_C               = 1302,
    AV_STONEHEART_A_A               = 1304, //over hc
    AV_STONEHEART_H_C               = 1301, //over ac
    AV_STONEHEART_H_A               = 1303, //over aa
    //Neutral
    //Snowfall Grave
*/
    AV_SNOWFALL_N                   = 1966 //over aa
/*
    AV_SNOWFALL_A_C                 = 1341, //over hc
    AV_SNOWFALL_A_A                 = 1343, //over ha
    AV_SNOWFALL_H_C                 = 1342,
    AV_SNOWFALL_H_A                 = 1344, //over ac
    //Horde
    //Iceblood grave
    AV_ICEBLOOD_A_C                 = 1346, //over hc
    AV_ICEBLOOD_A_A                 = 1348, //over ac
    AV_ICEBLOOD_H_C                 = 1347,
    AV_ICEBLOOD_H_A                 = 1349, //over aa
    //Frostwolf Grave
    AV_FROSTWOLF_A_C                = 1337, //over hc
    AV_FROSTWOLF_A_A                = 1339, //over ac
    AV_FROSTWOLF_H_C                = 1338,
    AV_FROSTWOLF_H_A                = 1340, //over aa
    //Frostwolf Hut
    AV_FROSTWOLFHUT_A_C             = 1329, //over hc
    AV_FROSTWOLFHUT_A_A             = 1331, //over ha
    AV_FROSTWOLFHUT_H_C             = 1330,
    AV_FROSTWOLFHUT_H_A             = 1332, //over ac

//Towers
    //Alliance
    //Dunbaldar South Bunker
    AV_DUNS_CONTROLLED              = 1361,
    AV_DUNS_DESTROYED               = 1370,
    AV_DUNS_ASSAULTED               = 1378,
    //Dunbaldar North Bunker
    AV_DUNN_CONTROLLED              = 1362,
    AV_DUNN_DESTROYED               = 1371,
    AV_DUNN_ASSAULTED               = 1379,
    //Icewing Bunker
    AV_ICEWING_CONTROLLED           = 1363,
    AV_ICEWING_DESTROYED            = 1372,
    AV_ICEWING_ASSAULTED            = 1380,
    //Stoneheart Bunker
    AV_STONEH_CONTROLLED            = 1364,
    AV_STONEH_DESTROYED             = 1373,
    AV_STONEH_ASSAULTED             = 1381,
    //Horde
    //Iceblood Tower
    AV_ICEBLOOD_CONTROLLED          = 1385,
    AV_ICEBLOOD_DESTROYED           = 1368,
    AV_ICEBLOOD_ASSAULTED           = 1390,
    //Tower Point
    AV_TOWERPOINT_CONTROLLED        = 1384,
    AV_TOWERPOINT_DESTROYED         = 1367, //goes over controlled
    AV_TOWERPOINT_ASSAULTED         = 1389, //goes over destroyed
    //Frostwolf West
    AV_FROSTWOLFW_CONTROLLED        = 1382,
    AV_FROSTWOLFW_DESTROYED         = 1365, //over controlled
    AV_FROSTWOLFW_ASSAULTED         = 1387, //over destroyed
    //Frostwolf East
    AV_FROSTWOLFE_CONTROLLED        = 1383,
    AV_FROSTWOLFE_DESTROYED         = 1366,
    AV_FROSTWOLFE_ASSAULTED         = 1388,

//mines

    AV_N_MINE_N              = 1360,
    AV_N_MINE_A              = 1358,
    AV_N_MINE_H              = 1359,

    AV_S_MINE_N                     = 1357,
    AV_S_MINE_A                     = 1355,
    AV_S_MINE_H                     = 1356,

//towers assaulted by own team (unused)
    AV_STONEH_UNUSED                = 1377,
    AV_ICEWING_UNUSED               = 1376,
    AV_DUNS_UNUSED                  = 1375,
    AV_DUNN_UNUSED                  = 1374,

    AV_ICEBLOOD_UNUSED              = 1395,
    AV_TOWERPOINT_UNUSED            = 1394,
    AV_FROSTWOLFE_UNUSED            = 1393,
    AV_FROSTWOLFW_UNUSED            = 1392
*/
};

//alliance_control neutral_control horde_control
const uint32 BG_AV_MineWorldStates[2][3] =
{
    {1358, 1360, 1359},
    {1355, 1357, 1356}
};

//alliance_control alliance_assault h_control h_assault
const uint32 BG_AV_NodeWorldStates[16][4] =
{
    //Stormpike first aid station
    {1325, 1326, 1327, 1328},
    //Stormpike Graveyard
    {1333, 1335, 1334, 1336},
    //Stoneheart Grave
    {1302, 1304, 1301, 1303},
    //Snowfall Grave
    {1341, 1343, 1342, 1344},
    //Iceblood grave
    {1346, 1348, 1347, 1349},
    //Frostwolf Grave
    {1337, 1339, 1338, 1340},
    //Frostwolf Hut
    {1329, 1331, 1330, 1332},
    //Dunbaldar South Bunker
    {1361, 1375, 1370, 1378},
    //Dunbaldar North Bunker
    {1362, 1374, 1371, 1379},
    //Icewing Bunker
    {1363, 1376, 1372, 1380},
    //Stoneheart Bunker
    {1364, 1377, 1373, 1381},
    //Iceblood Tower
    {1368, 1390, 1385, 1395},
    //Tower Point
    {1367, 1389, 1384, 1394},
    //Frostwolf East
    {1366, 1388, 1383, 1393},
    //Frostwolf West
    {1365, 1387, 1382, 1392},
};

enum BG_AV_QuestIds
{
    AV_QUEST_A_SCRAPS1 = 7223,     //"armor scraps"(A)
    AV_QUEST_A_SCRAPS2 = 6781,     //"more armor scraps"(A)
    AV_QUEST_H_SCRAPS1 = 7224,     //"enemy booty"(H)
    AV_QUEST_H_SCRAPS2 = 6741,     //"more booty"(H)
    AV_QUEST_A_COMMANDER1 = 6942,     //"Call of air"
    AV_QUEST_H_COMMANDER1 = 6825,     //"Call of air - Guse's fleet"
    AV_QUEST_A_COMMANDER2 = 6941,     //"Call of air - Vipore's fleet"
    AV_QUEST_H_COMMANDER2 = 6826,     //"call of air - Jeztor's fleet"
    AV_QUEST_A_COMMANDER3 = 6943,     //"Call of air - Ichman's fleet"
    AV_QUEST_H_COMMANDER3 = 6827,     //"Call of air - mulverick's fleet"
    AV_QUEST_A_BOSS1 = 7386,     //"Crystal Cluster"(Alliance 5)
    AV_QUEST_H_BOSS1 = 7385,     //"A gallon of blood"(Horde 5)
    AV_QUEST_A_BOSS2 = 6881,     //"Ivus the forest lord"(Alliance 1)
    AV_QUEST_H_BOSS2 = 6801,     //"Lokholar the ice lord"(Horde 1)
    AV_QUEST_A_NEAR_MINE = 5892,     //"Irondeep Supplies"(A)
    AV_QUEST_H_NEAR_MINE = 5893,     //"Coldtooth Supplies"(H)
    AV_QUEST_A_OTHER_MINE = 6982,     //"Coldtooth supplies"(A)
    AV_QUEST_H_OTHER_MINE = 6985,     //"Irondeep supplies"(H)
    AV_QUEST_A_RIDER_HIDE = 7026,     //"Ram Riding Harnesses"(A)
    AV_QUEST_H_RIDER_HIDE = 7002,     //"Ram Riding Harnesses"(H)
    AV_QUEST_A_RIDER_TAME = 7027,     //"Empty Stables"(A)
    AV_QUEST_H_RIDER_TAME = 7001      //"Empty Stables"(H)
};

enum BG_AV_Objectives
{
    AV_OBJECTIVE_ASSAULT_TOWER      = 61,
    AV_OBJECTIVE_ASSAULT_GRAVEYARD  = 63,
    AV_OBJECTIVE_DEFEND_TOWER       = 64,
    AV_OBJECTIVE_DEFEND_GRAVEYARD   = 65
};

struct BG_AV_NodeInfo
{
    BG_AV_States State;
    BG_AV_States PrevState;
    uint32       Timer;
    uint16       TotalOwner;
    uint16       Owner;
    uint16       PrevOwner;
    bool         Tower;
};

inline BG_AV_Nodes &operator++(BG_AV_Nodes &i){ return i = BG_AV_Nodes(i + 1); }

struct BattlegroundAVScore : public BattlegroundScore
{
    BattlegroundAVScore() : GraveyardsAssaulted(0), GraveyardsDefended(0), TowersAssaulted(0), TowersDefended(0), MinesCaptured(0), LeadersKilled(0), SecondaryObjectives(0) { }
    ~BattlegroundAVScore() { }
    uint32 GraveyardsAssaulted;
    uint32 GraveyardsDefended;
    uint32 TowersAssaulted;
    uint32 TowersDefended;
    uint32 MinesCaptured;
    uint32 LeadersKilled;
    uint32 SecondaryObjectives;
    uint32 GetAttr1() const { return GraveyardsAssaulted; }
    uint32 GetAttr2() const { return GraveyardsDefended; }
    uint32 GetAttr3() const { return TowersAssaulted; }
    uint32 GetAttr4() const { return TowersDefended; }
    uint32 GetAttr5() const { return MinesCaptured; }
};

class BattlegroundAV : public Battleground
{
    public:
        BattlegroundAV();
        ~BattlegroundAV();

        /* inherited from BattlegroundClass */
        void AddPlayer(Player* player);
        void StartingEventCloseDoors();
        void StartingEventOpenDoors();

        void RemovePlayer(Player* player, uint64 guid, uint32 team);
        void HandleAreaTrigger(Player* Source, uint32 Trigger);
        bool SetupBattleground();
        void ResetBGSubclass();

        /*general stuff*/
        void UpdateScore(uint16 team, int16 points);
        void UpdatePlayerScore(Player* Source, uint32 type, uint32 value, bool doAddHonor = true);

        /*handlestuff*/ //these are functions which get called from extern
        void EventPlayerClickedOnFlag(Player* source, GameObject* target_obj);
        void HandleKillPlayer(Player* player, Player* killer);
        void HandleKillUnit(Creature* unit, Player* killer);
        void HandleQuestComplete(uint32 questid, Player* player);
        bool PlayerCanDoMineQuest(int32 GOId, uint32 team);

        void EndBattleground(uint32 winner);

        WorldSafeLocsEntry const* GetClosestGraveYard(Player* player);

        /* achievement req. */
        bool IsBothMinesControlledByTeam(uint32 team) const;
        bool IsAllTowersControlledAndCaptainAlive(uint32 team) const;

        uint32 GetPrematureWinner();

    private:
        void PostUpdateImpl(uint32 diff);

        /* Nodes occupying */
        void EventPlayerAssaultsPoint(Player* player, uint32 object);
        void EventPlayerDefendsPoint(Player* player, uint32 object);
        void EventPlayerDestroyedPoint(BG_AV_Nodes node);

        void AssaultNode(BG_AV_Nodes node, uint16 team);
        void DestroyNode(BG_AV_Nodes node);
        void InitNode(BG_AV_Nodes node, uint16 team, bool tower);
        void DefendNode(BG_AV_Nodes node, uint16 team);

        void PopulateNode(BG_AV_Nodes node);
        void DePopulateNode(BG_AV_Nodes node);

        BG_AV_Nodes GetNodeThroughObject(uint32 object);
        uint32 GetObjectThroughNode(BG_AV_Nodes node);
        char const* GetNodeName(BG_AV_Nodes node);
        bool IsTower(BG_AV_Nodes node) { return m_Nodes[node].Tower; }

        /*mine*/
        void ChangeMineOwner(uint8 mine, uint32 team, bool initial=false);

        /*worldstates*/
        void FillInitialWorldStates(WorldPacket& data);
        uint8 GetWorldStateType(uint8 state, uint16 team);
        void SendMineWorldStates(uint32 mine);
        void UpdateNodeWorldState(BG_AV_Nodes node);

        /*general */
        Creature* AddAVCreature(uint16 cinfoid, uint16 type);
        uint16 GetBonusHonor(uint8 kills); //TODO remove this when the core handles this right

        /*variables */
        int32 m_Team_Scores[2];
        uint32 m_Team_QuestStatus[2][9]; //[x][y] x=team y=questcounter

        BG_AV_NodeInfo m_Nodes[BG_AV_NODES_MAX];

        uint32 m_Mine_Owner[2];
        uint32 m_Mine_PrevOwner[2]; //only for worldstates needed
        int32 m_Mine_Timer; //ticks for both teams
        uint32 m_Mine_Reclaim_Timer[2];
        uint32 m_CaptainBuffTimer[2];
        bool m_CaptainAlive[2];

        uint8 m_MaxLevel; //TODO remove this when battleground-getmaxlevel() returns something usefull
        bool m_IsInformedNearVictory[2];
        bool Ivus_has_been_spawned;
        bool Lokholar_has_been_spawned;
};

#endif
