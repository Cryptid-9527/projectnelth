/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#ifndef DEF_DEADMINES_H
#define DEF_DEADMINES_H

#define DataHeader "DM"

enum DeadminesBossData
{
    BOSS_GLUBTOK_DATA               = 0,
    BOSS_HELIX_DATA                 = 1,
    BOSS_FOE_REAPER_5000_DATA       = 2,
    BOSS_CAPTAIN_COOKIE             = 3,
    BOSS_ADMIRAL_RISNARL            = 4,
    BOSS_EVENT_VANCLEEF             = 5,
    BOSS_VANCLEEF                   = 6,
    MAX_ENCOUNTER                   = 7,

    BOSS_GLUBTOK_EVENT_DATA,
    BOSS_HELIX_EVENT_DATA,
    BOSS_REEPER_EVENT_DATA,
    DATA_START_VANCLEEF_EVENT,
    DATA_END_VANCLEEF_EVENT,
    DATA_PREPARE_VANCLEEF_EVENT,
    DATA_RESET_VANCLEEF_EVENT,
    DATA_GLUBTOK_EVENT,
    DATA_HELIX_EVENT,
    DATA_REEPER_EVENT,
    DATA_WORGEN_EVENT,
    DATA_TRAP_COMPLETE,

    DATA_FIREWALL,
    DATA_PRODIGY,
    DATA_KAGTHA,
    DATA_KAGTHA_ENTRANCE,
    DATA_SLINKY_SHARPSHIV,
    DATA_SLINKY_SHARPSHIV_ENTRANCE,
    DATA_MISS_MAYHEM,
    DATA_MISS_MAYHEM_ENTRANCE,
    DATA_MAYHEM_PROTOTYPE_L,
    DATA_MAYHEM_PROTOTYPE_L_ENTRANCE,
    DATA_MAYHEM_PROTOTYPE_R,
    DATA_MAYHEM_PROTOTYPE_R_ENTRANCE,
    DATA_LIEUTENANT_HORATIO_LAINE,
    DATA_LIEUTENANT_HORATIO_LAINE_ENTRANCE,
    DATA_STORMWIND_INVESTIGATOR,
    DATA_STORMWIND_INVESTIGATOR_ENTRANCE,
    DATA_STORMWIND_INVESTIGATOR_2,
    DATA_STORMWIND_INVESTIGATOR_2_ENTRANCE,
    DATA_CRIME_SCENE_BOT,
    DATA_CRIME_SCENE_BOT_ENTRANCE,
    DATA_CRIME_SCENE_BOT_2,
    DATA_CRIME_SCENE_BOT_2_ENTRANCE,
    DATA_CRIME_SCENE_BOT_3,
    DATA_CRIME_SCENE_BOT_3_ENTRANCE,
};

enum DeadminesBoss
{
    NPC_GLUBTOK                     = 47162,
    NPC_HELIX                       = 47296,
    NPC_CAPTAIN_COOKIE              = 47739,
    NPC_VANCLEEF                    = 49541,
    // combat npcs
    NPC_FIRE_BLOSSOM                = 48957,
    NPC_FIRE_BLOSSOM_BUNNY          = 47282,
    NPC_FROST_BLOSSOM               = 48958,
    NPC_FROST_BLOSSOM_BUNNY         = 47284,
};

enum DeadminesNPCs
{
    NPC_LUMBERING_OAF               = 47297,
    NPC_DEFIAS_CANNON               = 48266,
    NPC_KAGTHA                      = 46889,
    NPC_SLINKY_SHARPSHIV            = 46906,
    NPC_MISS_MAYHEM                 = 46902,
    NPC_MAYHEM_REAPER_PROTOTYPE     = 46903,
    NPC_LIEUTENANT_HORATIO_LAINE    = 46612,
    NPC_STORMWIND_INVESTIGATOR      = 46614,
    NPC_CRIME_SCENE_BOT             = 46613,
    NPC_FOUNDRY_GOBLIN_CRAFTSMAN    = 48280,
    NPC_FOUNDRY_GOBLIN_ENGINEER     = 48439,
    NPC_VANESSA_ANCHOR_BUNNY        = 51624,
    NPC_VANESSA_TRAP_BUNNY          = 49454,
    NPC_MAGMA_TRAP_EXIT_JUMP_TARGET = 49185,


    NPC_GENERAL_BUNNY = 45979,
    NPC_DEFIAS_ENFORCER = 49850,
    NPC_DEFIAS_SHADOWGUARD = 49852,
    NPC_ROPE = 49550,
    NPC_VANESSA_ROPE_ANCHOR = 49552,

};

enum DeadmineGOBs
{
    GO_GLUBTOK_DOOR                 = 13965,
    GO_HELIX_DOOR_ENTRANCE          = 17153,
    GO_HELIX_DOOR_EXIT              = 16400,
    GO_FOE_ENTRANCE                 = 17154,
    GO_FOE_EXIT                     = 16399,
    GO_IRON_CLAD_DOOR               = 16397,
};

enum Gauntlet_Positions
{
    NMG_POS_NOTE_SPAWN                         = 0,
    NMG_POS_PLATFORM_VANESSA_FIRST_SPAWN,
    NMG_POS_PLATFORM_VANESSA_JUMP_TO,
    NMG_POS_VANESSA_SPAWN_AT_GLUBTOK,

    NMG_POS_RP_GLUBTOK_SPAWN,
    NMG_POS_BOSS_GLUBTOK_SPAWN,
    NMG_POS_VANESSA_AT_HELIX,
    NMG_POS_SPAWN_HELIX,

    NMG_POS_VANCLEEF_START_FOE_REAPER,
    NMG_POS_VANCLEEF_START_RIPSNARL,
    NMG_POS_REAPER_RP_INTRO,
    NMG_POS_REAPER_TELETO,
    NMG_POS_RP_RIPSNARL,
};

enum Vanessas
{
    VANESSA_MONOLOGUE = 0,
    VANESSA_GLUBTOK,
    VANESSA_HELIX,
    VANESSA_FOE_REAPER,
    VANESSA_RIPSNARL,
    VANESSA_BOSSFIGHT
};
Position const nightmare_gauntlet_positions[]
{
    { -74.410118f, -819.963623f, 40.237736f, 0.130879f },   //Position const noteSpawn = ;
    { -75.54f, -819.87f, 47.15f, 0.02f },                   //Position const vancleefEventSpawn = ;      //Vanessa's spawn above the hut       
    { -226.561813f, -567.022583f, 51.229687f, 2.594636f },  //Position const platformJump = ;            // Jump to Spawn // OLD -229.03f, -576.95f, 51.3f, 0.0f
    { -233.650925f, -566.168152f, 51.235683f, 1.166317f },  //Position const platformVanessaSpawnOne = ; // Vancleef near glubtok

    { -231.512100f, -562.600342f, 51.230778f, 5.804346f },  //Position const Part1EventPos_preSpawn = ;  // Glubtok up top // OLD -235.737183f, -570.027344f, 51.247158f, 5.996501f
    { -174.955276f, -579.862671f, 19.313070f, 3.083427f },  //    Position const Part1EventPos = ;           // Glubtok Boss Spawn
    { -173.373337f, -576.778870f, 19.314102f, 4.848213f },  //Position const Part1EventPosVC = ;         // Vancleef Spawn teleport too pos next to Helix
    { -173.214478f, -579.883179f, 19.314102f, 3.123470f },  //Position const Part1EventHelixPos = ;      // Helix Spawn

    { -158.11f, -580.29f, 19.32f, 6.280226f },              //Position const Part2EventPos = ;                       // Vancleef after foundry door is open
    { -99.018616f, -662.427490f, 7.415267f, 3.257906f },    //Position const Part3EventPos = ;             // Vancleef 3rd spawn // OLD -100.72f, -663.50f, 7.42f, 1.97f
    { -154.547180f, -580.375610f, 19.315048f, 6.265645f },  //Position const Part3EventReaperSpawnPos = ;
    { -98.293228f, -663.278320f, 7.413917f, 3.656418f },   //Position const Part3EventReaperTeleportPos = ;
    { -100.872162f, -663.859802f, 7.421007f, 1.794636f }    //Position const Part3EventPosRipsnarl = ;
};

enum npc_groups
{
    NM_GROUP_BEFORE_GAUNTLET = 0,

    NM_GROUP_PAPER_ONLY,
    NM_GROUP_VANESSA_MONOLOGUE,

    NM_GROUP_TRAP,
    NM_GROUP_GLUBTOK,
    NM_GROUP_HELIX,
    NM_GROUP_REAPER,
    NM_GROUP_RIPSNARL,
    NM_GROUP_AFTER_GAUNTLET,
};

Position const PartHARRINGTON[3] =
{
    {-88.96f, -720.50f, 8.51f},
    {6.78f, -766.50f, 9.63f, 2.53f},
    {-83.86f, -775.28f, 28.38f, 1.71f},
};

Position const PartWorgen[6] =
{
    {0.85f, -764.91f, 9.72f, 5.47f},
    {7.85f, -764.41f, 9.59f, 4.35f},
    {4.84f, -770.50f, 9.63f, 1.30f},

    {-97.79166f, -717.8542f, 8.668088f, 4.520403f},
    {-94.40278f, -719.7274f, 8.598646f, 3.560472f},
    {-101.9167f, -718.7552f, 8.726379f, 5.51524f},
};

enum Achievements
{
    ACHIEVEMENT_VIGOROUS_VANCLEEF_VINDICATOR = 5371,
    ACHIEVEMENT_IM_ON_A_DIET                 = 5370,
};

enum VancCleefEventSpells
{
    SPELL_MAGMA_VEHICLE_DUMMY = 92378,

    SPELL_EJECT_ALL_PASSENGERS = 68576,
    SPELL_FORECAST_MAGMA_TRAP_THROW = 92489,
    SPELL_FORECAST_MAGMA_TRAP_JUMP = 92438,


    SPELL_NIGHTMARE_SLOW = 92559,
    SPELL_NIGHTMARE = 92563,
    SPELL_NIGHTMARE_1 = 92113,
    SPELL_NIGHTMARE_FLAME = 92169, // entry 51594


    SPELL_FORGOTTEN_AURA = 48143, // on NPC_VANCLEEF_GAUNTLET_INTRO, NPC_GLUBTOK_EVENT
    SPELL_COSMETIC_ALPHA_50_PCT = 69676, // on NPC_VANCLEEF_GAUNTLET_INTRO, NPC_GLUBTOK_EVENT

    SPELL_NIGHTMARE_AURA_CANCEL = 92583, // casted by NPC_VANCLEEF_GAUNTLET_INTRO
    SPELL_AURA_NIGHTMARE_2 = 92565, // casted by NPC_VANCLEEF_GAUNTLET_INTRO
    SPELL_AURA_NIGHTMARE_3 = 92566, // casted by NPC_VANCLEEF_GAUNTLET_INTRO
    SPELL_AURA_NIGHTMARE_4 = 92567, // casted by NPC_VANCLEEF_GAUNTLET_INTRO
    SPELL_AURA_NIGHTMARE_5 = 92568, // casted by NPC_VANCLEEF_GAUNTLET_INTRO

    SPELL_SUMMON_ICETOMB = 92189, // casted by 45979 (bunny) summon NPC_COLLAPSING_ICICLE
    SPELL_ICETOMB_TARGET = 92210, // casted by 45979 (bunny) 1087757
    SPELL_GLACIATE = 92201, // casted by NPC_COLLAPSING_ICICLE trigger 92203 and 62463 that summon 33174
    SPELL_GLACIATE_DUMMY = 92202, // casted by NPC_COLLAPSING_ICICLE

    SPELL_HELIX_VEHICLE = 88337,
    SPELL_SHADOW_FOG = 92792, // casted by 45979 (bunny)
    SPELL_HELIX_CT_VEHICLE_DUMMY = 88349,
    SPELL_HELIX_CT_VEHICLE = 88360,

    SPELL_LIGHTING = 95519, // 49521
    SPELL_LIGHTING_2 = 92277, // 49521

    SPELL_ADRENALINE = 92604,
    SPELL_ROPE_RAY                  = 43785, // on 49552
    SPELL_VAPOR_EVENT = 92401,
    SPELL_AURA_VAPOR_EVENT = 92399,
};

enum VanCleefEvents
{
    EVENT_VANCLEEF_EVENT = 1,
    CHECK_PLAYERS_ALIVE,
    CHECK_ACH_FAILED,
    PHASE_EVENT_NOT_STARTED,
    PHASE_EVENT_IN_PROGRESS,
    PHASE_EVENT_DONE,
    PHASE_EVENT_GLUBTOK,
    PHASE_EVENT_HELIX,
    PHASE_REEPER,
    PHASE_WORGEN,
    ACTION_START_FIGHT = 7,
    ACTION_ACTIVATE,
    ACTION_DESACTIVATE,
    ACTION_INIT_EVENT,
    ACTION_EJECT_PASSENGERS,
    ACTION_EMOTE_TRAP,
};

enum CanCleefEventnpcs
{
    // vancleef evet
    NPC_MAGMA_VEHICLE       = 49454,
    NPC_VANESSA_ABOVE_HUT      = 49429,
    NPC_NIGHTMARE_FLAMES    = 51594,
    NPC_VANCLEEF_GAUNTLET_INTRO    = 49671,
    NPC_GLUBTOK_EVENT       = 49670,
    NPC_GLUBTOK_WEAK_SELF   = 42755,
    NPC_COLLAPSING_ICICLE   = 49481,
    NPC_SNOWPACKED_ICICLE   = 33174,
    NPC_GLUBTOK_ICICLE_TARGET_BUNNY = 45979,

    NPC_HELIX_EVENT         = 49674,
    NPC_REEPER_EVENT        = 49681,
    NPC_RIPSNARL_NIGHTMARE  = 49682,

    NPC_DARKWEB_DEVOURER    = 49494,
    NPC_CHATTERING_HORROR   = 49495,
    NPC_NIGHTMARE_SKITTERLING = 49493,

    NPC_REAPER_LIGHTNING_OBSTACLE = 49520,
    NPC_REAPER_LIGHTNING_OBSTACLE_PASSENGER = 49521,

    NPC_NM_GLUBTOK_FIRE_CONTROLLER = 49039,
    NPC_ENRAGED_WORGEN      = 49532,

    NPC_HERIC_HARRINGTON    = 49535,
    NPC_JAMES_HARRINGTON    = 49539,
    NPC_EMMA_HARRINGTON     = 49534,
    NPC_CALLISA_HARRINGTON  = 49536,

    NPC_EDWIN_VANCLEEF = 42697, // ???

    NPC_VANESSA_PAPER   = 49564,
    NPC_VAPOR_VALVE     = 49457,
    NPC_CHALLENGE_MODE  = 90000,


};
#endif
