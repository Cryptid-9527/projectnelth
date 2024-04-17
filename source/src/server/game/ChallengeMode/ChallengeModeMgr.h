/*
 * Copyright (C) 2017-2019 AshamaneProject <https://github.com/AshamaneProject>
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

#ifndef _CHALLENGEMODEMGR_H
#define _CHALLENGEMODEMGR_H

#define MAX_KEYSTONE_AFFIXES 4
#define CHALLENGE_MODE_QUEST_ID 90000
#define CHALLENGE_MODE_QUEST_KILL_COUNT 90001
#define AFFIX_QUAKING_TIMER 20 * IN_MILLISECONDS
#define AFFIX_INCORPOREAL_TIMER 45 * IN_MILLISECONDS
#define AFFIX_THUNDERING_TIMER 75 * IN_MILLISECONDS

enum ChallengeModeAffix : uint8
{
    AFFIX_NONE          = 0,

    // Level 2+
    AFFIX_FORTIFIED     = 1,
    AFFIX_TYRANNICAL    = 2,

    // Level 4+
    AFFIX_TEEMING       = 3,    // NYI
    AFFIX_RAGING        = 4,
    AFFIX_BOLSTERING    = 5,
    AFFIX_SANGUINE      = 6,
    AFFIX_BURSTING      = 7,

    // Level 7+
    AFFIX_NECROTIC      = 8,    // NYI
    AFFIX_EXPLOSIVE     = 9,
    AFFIX_VOLCANIC      = 10,
    AFFIX_SKITTISH      = 11,
    AFFIX_GRIEVOUS      = 12,
    AFFIX_QUAKING       = 13,
    AFFIX_OVERFLOWING   = 14,   // NYI
    AFFIX_RELENTLESS    = 15,   // NYI
    AFFIX_SPITEFUL      = 16,
    AFFIX_STORMING      = 17,
    AFFIX_PRIDEFUL      = 18,
    AFFIX_THUNDERING    = 19,
    AFFIX_ENTANGLING    = 20,
    AFFIX_AFFLICTED     = 21,
    AFFIX_INCORPOREAL   = 22,
    AFFIX_SHIELDING     = 23,
    AFFIX_MAX,
};

enum ChallengeModeAffixSpells : uint32
{
    // AFFIX_FORTIFIED
    // AFFIX_TYRANNICAL
    // AFFIX_BOLSTERING
    AFFIX_SPELL_RALLYING_CRY_HEALTH         = 97463,

    // AFFIX_FORTIFIED
    // AFFIX_TYRANNICAL
    AFFIX_SPELL_RALLYING_CRY_DAMAGE         = 108169,

    // AFFIX_RAGING
    AFFIX_SPELL_ENRAGE_100                  = 80158,

    // AFFIX_BOLSTERING
    AFFIX_SPELL_ENRAGE_20                   = 64217,

    // AFFIX_EXPLOSIVE
    AFFIX_SPELL_EXPLOSIVE_NAME              = 5255,
    AFFIX_SPELL_EXPLOSIVE_ORB_VISUAL        = 91707,
    AFFIX_SPELL_EXPLOSIVE_CAST              = 91263,
    AFFIX_SPELL_EXPLOSIVE_EXPLOSION_VISUAL  = 41918,

    // AFFIX_SANGUINE
    AFFIX_SPELL_SANGUINE_VISUAL             = 106814,

    // AFFIX_VOLCANIC
    AFFIX_SPELL_VOLCANIC_VISUAL             = 100976,

    // AFFIX_GRIEVOUS
    AFFIX_SPELL_GRIEVOUS                    = 91937,

    // AFFIX_QUAKING
    AFFIX_SPELL_QUAKING_WARNING             = 62898,
    AFFIX_SPELL_QUAKING                     = 83130,

    // AFFIX_BURSTING
    AFFIX_SPELL_BURSTING_VISUAL             = 105219,
    AFFIX_SPELL_BURSTING_DAMAGE             = 101166,

    // AFFIX ENTANGLING
    AFFIX_SPELL_ENTANGLING_PRE_AOE          = 94370,
    AFFIX_SPELL_ENTANGLING_EXPLOSION        = 43585,

    // AFFIX INCORPOREAL
    AFFIX_SPELL_INCORPOREAL_CHANNEL         = 54657,
    AFFIX_SPELL_INCORPOREAL_DEBUFF_AURA     = 72391,
};

enum ChallengeModeAffixNpcs : uint32
{
    AFFIX_NPC_EXPLOSIVE             = 90001,
    AFFIX_NPC_SANGUINE              = 90002,
    AFFIX_NPC_VOLCANIC              = 90003,
    AFFIX_NPC_QUAKING               = 1337021,


    AFFIX_NPC_SPITEFUL              = 174773,
    AFFIX_NPC_PRIDEFUL              = 173729,
    AFFIX_NPC_STORMING              = 1337020,
    AFFIX_NPC_THUNDERING_TRIGGER    = 1337022,
    AFFIX_NPC_THUNDERING_ORB        = 1337023,
    AFFIX_NPC_THUNDERING_SPAWNER    = 1337024,
    AFFIX_NPC_THUNDERING_DESPAWNER  = 1337025,

    AFFIX_NPC_ENTANGLING            = 1337028,
    AFFIX_NPC_SHIELDING             = 205746,
    AFFIX_NPC_INCORPOREAL           = 75979,
    AFFIX_NPC_AFFLICTED             = 204773,

};

struct ChallengeModeLevel
{
    uint8 level;
    float damageMultiplier = 0.f;
    float healthMultiplier = 0.f;
};

struct ChallengeModeData
{
    uint32 mapId;
    uint32 timer;
};

class ChallengeModeMgr
{
public:
    static ChallengeModeMgr* instance();

    void LoadChallengeData();
    void LoadMultipliersData();

    uint32 GetTimerForInstance(uint32 mapId, uint8 keystone_level);

    float GetDamageMultiplier(uint8 challengeLevel);
    float GetHealthMultiplier(uint8 challengeLevel);

    float ApplyDamageMultiplier(Unit const* unit, float baseDamage, uint32 spellId = 0);

    uint32 GetRandomChallengeId();

    uint8 GetAffixesNumberForLevel(uint8 level);
    std::vector<uint8> GenerateAffixes(uint8 level);

    void ApplyAffixes(Unit* unit, bool teeming = true);

    void Reward(Player* player, uint8 challengeLevel);

    static std::string GetKeystoneName(Player* player, uint32 mapId, uint8 level = 0);
    static std::string GetKeystoneNameCurrent(Player* player, uint32 mapId, uint8 level = 0);
    static std::string GenerateAfficon(uint8 size, std::string icon_text);

private:
    UNORDERED_MAP<uint8, ChallengeModeLevel> m_levels;
    UNORDERED_MAP<uint32, ChallengeModeData> m_data;
};

#define sChallengeModeMgr ChallengeModeMgr::instance()

#endif
