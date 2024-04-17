/*
 * Copyright (C) 2010 - 2012 ProjectSkyfire <http://www.projectskyfire.org/>
 *
 * Copyright (C) 2011 - 2012 ArkCORE <http://www.arkania.net/>
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

#ifndef DEF_THRONE_OF_THE_TIDES
#define DEF_THRONE_OF_THE_TIDES

static const int maxEncounter = 4;
#define ThroneoftheTidesScriptName "instance_throne_of_the_tides"
#define DataHeader "TT"

enum Data64
{
    DATA_LADY_NAZJAR,
    DATA_COMMANDER_ULTHOK,
    DATA_ERUNAK_STONESPEAKER,
    DATA_MINDBENDER_GHURSHA,
    DATA_OZUMAT,
    DATA_NEPTULON,
};

enum Data
{
    DATA_LADY_NAZJAR_EVENT,
    DATA_COMMANDER_ULTHOK_EVENT,
    DATA_ERUNAK_STONESPEAKER_EVENT,
    DATA_MINDBENDER_GHURSHA_EVENT,
    DATA_OZUMAT_EVENT,
    DATA_PROGRESS_EVENT,
    DATA_EVENT_COMMANDER_ULTHOK_START,
    DATA_LADY_NAZJAR_INTRO,
};

enum CreatureIds
{
    BOSS_LADY_NAZJAR                = 40586, // HC = 49079
    NPC_SUMMONED_WATERSPOUT         = 48571,
    NPC_NAZJAR_TEMPEST_WITCH_LADY   = 44404,
    NPC_NAZJAR_TEMPEST_WITCH_LADY_HC= 49093,
    NPC_NAZJAR_HONOR_GUARD          = 40633,
    NPC_NAZJAR_HONOR_GUARD_HC       = 49084,

    // Boss Commander Ulthok
    BOSS_COMMANDER_ULTHOK           = 40765, // HC = 49064
    NPC_DARK_FISSURE                = 40784, // HC = 49065

    BOSS_ERUNAK_STONESPEAKER        = 40825,
    BOSS_MINDBENDER_GHURSHA         = 40788,
    NPC_EARTH_SHARD                 = 45469,
    NPC_MIND_FOG                    = 40861,

    // Boss Ozumat
    BOSS_RP_OZUMAT                  = 42172,
    BOSS_OZUMAT                     = 40655, // HC = 49096
    BOSS_NEPTULON                   = 40792, // HC = 49094
    NPC_DEEP_MURLOC_INVADER         = 44658, // HC = 49068
    NPC_VICIOUS_MINDLASHER          = 44715, // HC = 49107
    NPC_UNYIELDING_BEHEMOTH         = 44648, // HC = 49104
    NPC_FACELESS_SAPPER             = 44752, // HC = 49073
    NPC_BLIGHT_BEAST                = 44841, // HC = 49061
    NPC_BLIGHT_OF_OZUMAT            = 44838, // HC = 49063
    NPC_BLIGHT_OF_OZUMAT_SUMMONER   = 44801,

    NPC_WORLD_TRIGGER_INFINITE_AOI  = 361710,
    NPC_BLIGHT_OF_OZUMAT_ROOM       = 44834,
    BOSS_OZUMAT_EVENT               = 40655,
    NPC_FISSURE                     = 40789,
    NPC_CINEMATIC_TRIGGER           = 361710,

    // Others
    NPC_CAPTAIN_TAYLOR              = 50270,
    NPC_LEGIONNAIRE_NAZGRIM         = 50272,
    NPC_LADY_NAZJAR_INTRO           = 39959, // HC = 49080
    NPC_DEEP_MURLOC_DRUDGE          = 39960, // HC = 49066
    NPC_NAZJAR_SPIRITMENDER         = 41139, // HC = 49091
    NPC_NAZJAR_INVADER              = 39616, // HC = 49085
    NPC_NAZJAR_INTRO_INVADER        = 40584, // HC = 49085

    NPC_NAZJAR_TEMPEST_WITCH        = 40634, // HC = 49092
    NPC_GILGOBLIN_HUNTER            = 40935, // HC = 49078
    NPC_GILGOBLIN_AQUAMAGE          = 40943, // HC = 49077
    NPC_UNSTABLE_CORRUPTION         = 40923, // HC = 49103
    NPC_TAINTED_SENTRY              = 40925, // HC = 49102
    NPC_TOT_TELEPORTER_F1           = 51391,
    NPC_TOT_TELEPORTER_F2           = 51395,
    NPC_FACELESS_WATCHER            = 40936,
};

enum GameObjectIds
{
    GO_NEPUTOLON_CACHE              = 205216,
    GO_STRANGE_FOUNTAIN             = 207406,
    GO_ABYSSAL_MAW_01               = 204338,
    GO_ABYSSAL_MAW_02               = 204339,
    GO_ABYSSAL_MAW_03               = 204340,
    GO_ABYSSAL_MAW_04               = 204341,
    GO_CORAIL                       = 205542,
    GO_CONSOLE                      = 203199,
    GO_OZUMAT_CHEST_NORMAL          = 205216,
    GO_OZUMAT_CHEST_HEROIC          = 205215,
    GO_TENTACLE_LEFT                = 208301,
    GO_TENTACLE_RIGHT               = 208302,
    GO_WALL_TENTACLE_LEFT           = 207997,
    GO_WALL_TENTACLE_RIGHT          = 207998,
};

enum Actions
{
    ACTION_LADY_NAZJAR_INTRO_START  = 1,
    ACTION_LADY_NAZJAR_INTRO_END,
    ACTION_LADY_NAZJAR_INTRO_COUNTER,
    ACTION_NEPTULON_INTRO           = 1001,
};

enum ToT_Spells
{
    SPELL_WATER_JUMP_VISUAL = 74048,
};

enum AchievemtsIds
{
    NORMAL_OZUMAT_DEFEAT                                    = 4839,
    HEROIC_OZUMAT_DEFEAT                                    = 5061,
    ACHIEVEMENT_THRONE_OF_THE_TIDES_GUILD_GROUP_CRITERIA    = 14483,
    HEROIC_NAZJAR_GEYSER_KILLED_TRASH                       = 5285,
    HEROIC_DEFEAT_BEHEMOT_WITH_SURGE_EFFECT                 = 5286,
};
Position const teleportersPOS[2] =
{
    { -560.25f, 819.20f, 245.3970f, 4.9043f }, // Entrance
    { -14.540f, 796.32f, 808.2360f, 1.5882f }, // Second Floor
};
template<class AI>
CreatureAI* GetThroneoftheTidesAI(Creature* creature)
{
    if (InstanceMap* instance = creature->GetMap()->ToInstanceMap())
        if (instance->GetInstanceScript())
            if (instance->GetScriptId() == sObjectMgr->GetScriptId(ThroneoftheTidesScriptName))
                return new AI(creature);

    return NULL;
}

#endif
