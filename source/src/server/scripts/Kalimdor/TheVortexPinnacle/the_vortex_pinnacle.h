/*
 * Copyright (C) 2018-2020 <http://www.projectnelth.com/>
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

#ifndef DEF_VORTEX_PINNACLE_H
#define DEF_VORTEX_PINNACLE_H

#include "ObjectMgr.h"

static const int maxEncounter = 3;
#define VortexPinnacleScriptName "instance_the_vortex_pinnacle"
#define DataHeader "VP"

enum Data
{
    DATA_GRAND_VIZIER_ERTAN,
    DATA_ALTAIRUS,
    DATA_ASAAD,
    DATA_GROUNDING_FIELD,
    DATA_GOLDEN_ORB,
};

enum Bosses
{
    BOSS_GRAND_VIZIER_ERTAN = 0,
    BOSS_ALTAIRUS,
    BOSS_ASAAD,
    MAX_BOSSES,
};

enum CreatureIds
{
    // Bosses
    NPC_GRAND_VIZIER_ERTAN      = 43878,
    NPC_ALTAIRUS                = 43873,
    NPC_ASAAD                   = 43875,

    // Trash mobs
    NPC_ARMORED_MISTRAL         = 45915,
    NPC_CLOUD_PRINCE            = 45917,
    NPC_EMPYREAN_ASSASSIN       = 45922,
    NPC_EXECUTOR_OF_THE_CALIPH  = 45928,
    NPC_SERVANT_OF_ASAAD        = 45926,
    NPC_GUST_SOLDIER            = 45477,
    NPC_HOWLING_GALE            = 45572,
    NPC_LURKING_TEMPEST         = 45704,
    NPC_MINISTER_OF_AIR         = 45930,
    NPC_TEMPLE_ADEPT            = 45935,
    NPC_YOUNG_STORM_DRAGON      = 45919,
    NPC_WILD_VORTEX             = 45912,
    NPC_SLIPSTREAM              = 45455,
    NPC_SKYFALL                 = 52019,
    NPC_WHIPPING_WIND           = 47238,

    // Boss' encounter related NPCs
    NPC_AIR_CURRENT             = 47305,
    NPC_TWISTER                 = 47342,
    NPC_GROUNDING_FIELD         = 47000,
    NPC_UNSTABLE_GROUNDING_FIELD= 46492,
    NPC_STORM_TARGET            = 46387,
    NPC_CYCLONE                 = 46007,
    NPC_VORTEX_ROTATION_FOCUS   = 438780,

    // Various NPCs
    NPC_GOLDEN_ORB              = 51157, /*Heroic Mode*/
    NPC_ITESH                   = 49943,
    NPC_GROUNDING_FIELD_CENTER  = 47085,

    NPC_WIND                    = 47305,
};

enum Misc
{
    GO_MAGIC_BRAZIER            = 207408,
};

enum WolrdStateCoT
{
    WORLDSTATE_GOLDEN_ORB_COLLECTED = 5649,
};

Position const slipstreamPos[13] = {
    {-310.458f, -29.745f, 625.0833f, 0.0f}, // Enterance Left
    {-382.441f, 42.3160f, 625.0833f, 0.0f}, // Enterance Right

    {-775.517f, -70.932f, 640.3123f, 0.0f}, // First Boss 1
    {-848.227f, -68.724f, 654.2203f, 0.0f}, // First Boss 2
    {-844.885f, -205.14f, 660.7083f, 0.0f}, // First Boss 3
    {-901.360f, -177.33f, 664.6000f, 2.9f}, // First Boss Arrival

    {-1190.88f, 125.203f, 737.6243f, 0.0f}, // Second Boss 1
    {-1138.55f, 178.524f, 711.4943f, 0.0f}, // Second Boss 2
    {-1245.21f, 230.986f, 690.6083f, 0.0f}, // Second Boss 3
    {-1282.07f, 344.856f, 660.9873f, 0.0f}, // Second Boss 4
    {-1229.64f, 412.260f, 641.2933f, 0.0f}, // Second Boss 5
    {-1188.70f, 475.900f, 635.0000f, 0.5f}, // Second Boss Arrival

    {-746.957f, 529.141f, 644.8316f, 0.0f}, // Third Boss 1
};

int GetSlipstreamData64(uint64 Guid);

template<class AI>
CreatureAI* GetVortexPinnacleAI(Creature* creature)
{
    if (InstanceMap* instance = creature->GetMap()->ToInstanceMap())
        if (instance->GetInstanceScript())
            if (instance->GetScriptId() == sObjectMgr->GetScriptId(VortexPinnacleScriptName))
                return new AI(creature);

    return NULL;
}

#endif