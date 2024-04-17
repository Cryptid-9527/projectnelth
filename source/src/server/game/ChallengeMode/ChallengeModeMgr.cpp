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

#include "ChallengeModeMgr.h"
#include "Containers.h"
#include "Item.h"
#include "Player.h"
#include "InstanceScript.h"
#include "ScriptedCreature.h"
#include "Chat.h"
#include "..\Custom\mythic_rewards.h"



ChallengeModeMgr* ChallengeModeMgr::instance()
{
    static ChallengeModeMgr instance;
    return &instance;
}

void ChallengeModeMgr::LoadChallengeData()
{
    uint32 oldMSTime = getMSTime();

    m_levels.clear();

    QueryResult result = WorldDatabase.Query("SELECT map, timer FROM challenge_data");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 mapId = fields[0].GetUInt32();

        ChallengeModeData& levelData = m_data[mapId];
        levelData.mapId     = mapId;
        levelData.timer     = fields[1].GetUInt32();

    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %lu challenge mode data in %u ms", (unsigned long)m_levels.size(), GetMSTimeDiffToNow(oldMSTime));
}



void ChallengeModeMgr::LoadMultipliersData()
{
    uint32 oldMSTime = getMSTime();

    m_levels.clear();

    QueryResult result = WorldDatabase.Query("SELECT level, health_multiplier, damage_multiplier FROM challenge_multiplier");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint8 level = fields[0].GetUInt8();

        ChallengeModeLevel& levelData = m_levels[level];
        levelData.level            = level;
        levelData.healthMultiplier = fields[1].GetFloat();
        levelData.damageMultiplier = fields[2].GetFloat();

    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %lu challenge mode levels in %u ms", (unsigned long)m_levels.size(), GetMSTimeDiffToNow(oldMSTime));
}

uint32 ChallengeModeMgr::GetTimerForInstance(uint32 mapId, uint8 keystone_level)
{
    UNORDERED_MAP<uint32, ChallengeModeData>::const_iterator itr = m_data.find(mapId);
    if (itr == m_data.end())
        return 0;

    uint32 timer{ itr->second.timer };
    uint32 timer_difficulty_bonus{ uint32(keystone_level) * 90 };
    return (timer + timer_difficulty_bonus);
}

float ChallengeModeMgr::GetDamageMultiplier(uint8 challengeLevel)
{
    UNORDERED_MAP<uint8, ChallengeModeLevel>::const_iterator itr = m_levels.find(challengeLevel);
    if (itr == m_levels.end())
        return 0.f;

    return itr->second.damageMultiplier;
}

float ChallengeModeMgr::GetHealthMultiplier(uint8 challengeLevel)
{
    UNORDERED_MAP<uint8, ChallengeModeLevel>::const_iterator itr = m_levels.find(challengeLevel);
    if (itr == m_levels.end())
        return 0.f;

    return itr->second.healthMultiplier;
}

float ChallengeModeMgr::ApplyDamageMultiplier(Unit const* unit, float baseDamage, uint32 spellId)
{
    if (unit)
        if (Creature const* cre = unit->ToCreature())
        {
            /*
            if (cre->getFaction() == 32)
                return baseDamage;
            */

            if (cre->ToPet())
                return baseDamage;
            if (cre->isTotem())
                return baseDamage;
            if (cre->isGuardian())
                return baseDamage;
            if (cre->isHunterPet())
                return baseDamage;
            if (auto o = cre->GetOwner())
                if (auto p = o->ToPlayer())
                    return baseDamage;


                if (auto script = cre->GetInstanceScript())
                    if (script->IsChallengeModeStarted())
                    {
                        if (spellId != 0)
                        {
                            if (auto info = sSpellMgr->GetSpellInfo(spellId))
                                if (info->HasAura(SPELL_AURA_PERIODIC_DAMAGE_PERCENT) || info->HasEffect(SPELL_EFFECT_DAMAGE_FROM_MAX_HEALTH_PCT))
                                {
                                    //TC_LOG_ERROR("sql.sql", "ChallengeModeMgr::ApplyDamageMultiplier spell %u not multiplied due to having PCT based damage.", spellId);
                                    return baseDamage;
                                }

                            uint32 excludedspellIds[]{ 76002, 93668, 91397, 91037, 91016, 91005, 91717, 90912, 92008, 92011, 92007, 92042, 92203, 90250,
                                90764, 90810, 90915, 90972, 89881, 90007, 89861, 89990, 90027, 90015, 93424, 93720, 93617, 93703, 92640, 91434, 95463, 91495, 93988, 93993, 93989, 91791 };

                            for (uint16 i = 0; i < 34; ++i)
                                if (spellId == excludedspellIds[i])
                                    return baseDamage;
                        }
                        return baseDamage * GetDamageMultiplier(script->GetChallengeModeLevel());
                    }
        }

    return baseDamage;
}

uint32 ChallengeModeMgr::GetRandomChallengeId()
{
    auto temp_data{ m_data };
    //Do Not allow Keystones to generate certain maps:
    temp_data.erase(938);//End Time
    temp_data.erase(939);//well of eternity
    temp_data.erase(940);//Hour of Twilight

    temp_data.erase(568);//Zul Aman
    temp_data.erase(859);//Zul Gurub


    return Trinity::Containers::SelectRandomContainerElement(temp_data).first;
}

uint8 ChallengeModeMgr::GetAffixesNumberForLevel(uint8 level)
{
    struct AffixesPerLevel
    {
        uint8 level;
        uint8 affixNumber;
    };

    std::vector<AffixesPerLevel> AffixesPerLevel =
    {
        { 15, 4 },
        { 7, 3 },
        { 4, 2 },
        { 2, 1 }
    };

    for (auto itr : AffixesPerLevel)
        if (level >= itr.level)
            return std::min(itr.affixNumber, uint8(MAX_KEYSTONE_AFFIXES));

    return 0;
}

std::vector<uint8> ChallengeModeMgr::GenerateAffixes(uint8 level)
{
    std::vector<uint8> affixes = { AFFIX_NONE, AFFIX_NONE, AFFIX_NONE, AFFIX_NONE };

    /*
    std::unordered_map<uint8, std::vector<uint8>> perSlotAffix =
    {
        { 0, { AFFIX_FORTIFIED, AFFIX_TYRANNICAL, AFFIX_TEEMING, } },                   //Baseline "Extra to do"
        { 1, { AFFIX_RAGING, AFFIX_BOLSTERING, AFFIX_SANGUINE, AFFIX_VOLCANIC } },      //On Death Effects
        { 2, { AFFIX_BURSTING, AFFIX_EXPLOSIVE, AFFIX_QUAKING, AFFIX_NECROTIC } },      //Something else to contend with
        { 3, { AFFIX_SKITTISH, AFFIX_GRIEVOUS, AFFIX_OVERFLOWING, AFFIX_RELENTLESS } }  //Core player weakening
    };
    */

    /*
        Updated
    */
    std::unordered_map<uint8, std::vector<uint8>> perSlotAffix =
    {
        { 0, { AFFIX_FORTIFIED, AFFIX_TYRANNICAL, AFFIX_VOLCANIC      }                                                 },  //Baseline "Extra to do"
        { 1, { AFFIX_SANGUINE,  AFFIX_AFFLICTED,    AFFIX_ENTANGLING,   AFFIX_STORMING,     AFFIX_INCORPOREAL     }     },  //On Death Effects
        { 2, { AFFIX_SPITEFUL,  AFFIX_RAGING,       AFFIX_BOLSTERING,   AFFIX_BURSTING,         }                       },  //Something Else to contend with
        { 3, { AFFIX_SKITTISH,  AFFIX_GRIEVOUS,      AFFIX_RELENTLESS,   AFFIX_NECROTIC,        }                       }   //Core player weakeners
    };

    uint8 affixesNumber = GetAffixesNumberForLevel(level);

    for (uint8 i = 0; i < affixesNumber; ++i)
        affixes[i] = Trinity::Containers::SelectRandomContainerElement(perSlotAffix[i]);


    /*
    The following affixes are not listed in retail's configuration.
    We will have them fill in as wildcard affixes, only available to a tier when the tier above them is unlocked.
    */

    std::unordered_map<uint8, std::vector<uint8>> wildcard_affixes =
    {
        {0, {AFFIX_EXPLOSIVE, AFFIX_QUAKING, AFFIX_PRIDEFUL,  AFFIX_TEEMING}},
        {0, {AFFIX_SHIELDING,   AFFIX_THUNDERING,   AFFIX_OVERFLOWING}},
    };

    for (uint8 i = 1; i < 3; ++i)
        if (affixesNumber > i)
            if (urand(0, 5) == 0)
            {
                auto selected_wildcard_affix = Trinity::Containers::SelectRandomContainerElement(wildcard_affixes[i]);
                affixes[i] = selected_wildcard_affix;
            }

    return affixes;
}

void ChallengeModeMgr::ApplyAffixes(Unit* unit, bool teeming /*= true*/)
{
    InstanceScript* script = unit->GetInstanceScript();
    if (!script)
        return;

    Player* player = unit->ToPlayer();
    Creature* creature = unit->ToCreature();

    // AFFIX_FORTIFIED : Non-boss enemies have 20% more health and inflict up to 30% increased damage.
    if (creature && script->HasAffix(AFFIX_FORTIFIED) && creature->CanBeSubjectOfAffix(AFFIX_FORTIFIED))
        if (!creature->IsDungeonBoss())
        {
            creature->RemoveAurasDueToSpell(AFFIX_SPELL_RALLYING_CRY_HEALTH);
            creature->RemoveAurasDueToSpell(AFFIX_SPELL_RALLYING_CRY_DAMAGE);

            CustomSpellValues healthValues;
            healthValues.AddSpellMod(SPELLVALUE_BASE_POINT0, creature->CountPctFromMaxHealth(20));
            healthValues.AddSpellMod(SPELLVALUE_DURATION, 7 * DAY * IN_MILLISECONDS);
            creature->CastCustomSpell(AFFIX_SPELL_RALLYING_CRY_HEALTH, healthValues, creature, TRIGGERED_FULL_MASK);

            CustomSpellValues damageValues;
            damageValues.AddSpellMod(SPELLVALUE_BASE_POINT0, 30);
            damageValues.AddSpellMod(SPELLVALUE_BASE_POINT1, 0); // Scale
            damageValues.AddSpellMod(SPELLVALUE_DURATION, 7 * DAY * IN_MILLISECONDS);
            creature->CastCustomSpell(AFFIX_SPELL_RALLYING_CRY_DAMAGE, damageValues, creature, TRIGGERED_FULL_MASK);
        }

    // AFFIX_TYRANNICAL : Boss enemies have 40% more health and inflict up to 15% increased damage.
    if (creature && script->HasAffix(AFFIX_TYRANNICAL) && creature->CanBeSubjectOfAffix(AFFIX_TYRANNICAL))
    {
        /*if (creature->IsDungeonBoss())
        {
            creature->RemoveAurasDueToSpell(AFFIX_SPELL_RALLYING_CRY_HEALTH);
            creature->RemoveAurasDueToSpell(AFFIX_SPELL_RALLYING_CRY_DAMAGE);

            CustomSpellValues healthValues;
            healthValues.AddSpellMod(SPELLVALUE_BASE_POINT0, creature->CountPctFromMaxHealth(40));
            healthValues.AddSpellMod(SPELLVALUE_DURATION, 7 * DAY * IN_MILLISECONDS);
            creature->CastCustomSpell(AFFIX_SPELL_RALLYING_CRY_HEALTH, healthValues, creature, TRIGGERED_FULL_MASK);

            CustomSpellValues damageValues;
            damageValues.AddSpellMod(SPELLVALUE_BASE_POINT0, 15);
            damageValues.AddSpellMod(SPELLVALUE_BASE_POINT1, 0); // Scale
            damageValues.AddSpellMod(SPELLVALUE_DURATION, 7 * DAY * IN_MILLISECONDS);
            creature->CastCustomSpell(AFFIX_SPELL_RALLYING_CRY_DAMAGE, damageValues, creature, TRIGGERED_FULL_MASK);
        }*/

        if (creature->IsDungeonBoss())
        {
            creature->RemoveAurasDueToSpell(56257);
            creature->RemoveAurasDueToSpell(40318);

            CustomSpellValues healthValues;
            healthValues.AddSpellMod(SPELLVALUE_BASE_POINT0, 40);
            healthValues.AddSpellMod(SPELLVALUE_DURATION, 7 * DAY * IN_MILLISECONDS);
            creature->CastCustomSpell(56257, healthValues, creature, TRIGGERED_FULL_MASK);

            CustomSpellValues damageValues;
            damageValues.AddSpellMod(SPELLVALUE_BASE_POINT0, 0); // Scale
            damageValues.AddSpellMod(SPELLVALUE_BASE_POINT1, 15);
            damageValues.AddSpellMod(SPELLVALUE_DURATION, 7 * DAY * IN_MILLISECONDS);
            creature->CastCustomSpell(40318, damageValues, creature, TRIGGERED_FULL_MASK);
        }
    }

    if (creature && script->HasAffix(AFFIX_TEEMING) && creature->CanBeSubjectOfAffix(AFFIX_TEEMING))
        if (!creature->IsDungeonBoss() && (!creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE)) && (!creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED)) && (urand(1, 100) <= 10))
        {
            Position randomPosition;
            creature->MoveBlink(randomPosition, frand(5.f, 20.f), frand(0.f, M_PI * 2.f));
            randomPosition.m_positionZ += 10.f;
            creature->UpdateGroundPositionZ(randomPosition.GetPositionX(), randomPosition.GetPositionY(), randomPosition.m_positionZ);
            randomPosition.m_positionZ += 1.f;
            creature->SummonCreature(creature->GetEntry(), randomPosition);
        }
}

void ChallengeModeMgr::Reward(Player* player, uint8 challengeLevel)
{
    InstanceScript* script = player->GetInstanceScript();
    if (!script)
        return;


    /*
        BEGIN KEYSTONE LEVEL CHANGE
    */
    uint32 maxTime = GetTimerForInstance(script->instance->GetId(), script->GetKeystone().level);
    uint32 remainingTime = player->GetInstanceScript()->GetChallengeModeRemainingTime();

    float remainingTimePercent{ float(remainingTime) / float(maxTime) * 100.f };
    float completed_challenge_level{ float(challengeLevel) };
    float player_current_keystone_level{ float(player->GetKeystone().level) };
    float keystone_rank_change{ 0.f };
    float keystone_level_to_set{ player_current_keystone_level };

    if (remainingTime <= 0)
        keystone_rank_change = -1.f;
    else if (remainingTimePercent > 40.f)
        keystone_rank_change = 3.f;
    else if (remainingTimePercent > 20.f)
        keystone_rank_change = 2.f;
    else
        keystone_rank_change = 1.f;

    /*
        If the dungeon completion was at a loss, force the loss on the player's keystone.
        If the dungeon completion was within range of the keystone, set their keystone to the new level.
    */
    if (keystone_rank_change == -1.f)                                                                                           //Everyone's taking the L
    {
        keystone_level_to_set = player_current_keystone_level + keystone_rank_change;                                           //Subtract 1
        if (keystone_level_to_set < 1)                                                                                          //If this took you lower than 1, go back to 1
            keystone_level_to_set = 1;

        player->SetKeystone(GetRandomChallengeId(), int8(double(keystone_level_to_set)));
    }
    else
    {
        if (player_current_keystone_level < (completed_challenge_level + keystone_rank_change))                                 //Your current key would be upgraded if set to the party leader's level
        {
            if (player_current_keystone_level + keystone_rank_change >= (completed_challenge_level + keystone_rank_change))      //if you were to get the full benefit, your keystone would go higher than the leader
                keystone_level_to_set = (completed_challenge_level + keystone_rank_change);
            else
                keystone_level_to_set = (player_current_keystone_level + keystone_rank_change);                                 //You can get the whole benefit of the boost without going higher than the leader.

            player->SetKeystone(GetRandomChallengeId(), int8(double(keystone_level_to_set)));                                   //Execute
        }
    }


    /*
        END KEYSTONE LEVEL CHANGE
    */
    ChatHandler(player->GetSession()).PSendSysMessage("Mythic Challenge Completed!");
    uint8 ChanceWhite = 0;
    uint8 ChanceGreen = 0;
    uint8 ChanceBlue = 0;
    uint8 ChanceEpic = 0;
    uint8 ChanceLegendary = 0;
    uint8 ChanceArtifact = 0;
    uint8 rewardLevel = challengeLevel;

    if (remainingTime < 0)
        if (challengeLevel > 4)
            rewardLevel -= 3;

    switch (rewardLevel)
    {
    case 1: case 2: case 3: case 4:
    {
        ChanceWhite		= 90;
        ChanceGreen		= 5;
        ChanceBlue		= 3;
        ChanceEpic		= 2;
        ChanceLegendary = 0;
        ChanceArtifact	= 0;
    }
    break;
    case 5: case 6: case 7: case 8: case 9:
    {
        ChanceWhite		= 50;
        ChanceGreen		= 30;
        ChanceBlue		= 12;
        ChanceEpic		= 8;
        ChanceLegendary = 0;
        ChanceArtifact	= 0;
    }
    break;
    case 10: case 11: case 12: case 13: case 14:
    {
        ChanceWhite		= 30;
        ChanceGreen		= 30;
        ChanceBlue		= 24;
        ChanceEpic		= 15;
        ChanceLegendary = 1;
        ChanceArtifact	= 0;
    }
    break;
    case 15: case 16: case 17: case 18: case 19:
    {
        ChanceWhite		= 21;
        ChanceGreen		= 21;
        ChanceBlue		= 30;
        ChanceEpic		= 21;
        ChanceLegendary = 5;
        ChanceArtifact	= 2;
    }
    break;
    case 20: case 21: case 22: case 23: case 24:
    {
        ChanceWhite		= 5;
        ChanceGreen		= 15;
        ChanceBlue		= 27;
        ChanceEpic		= 30;
        ChanceLegendary = 16;
        ChanceArtifact	= 7;
    }
    break;
    }
    
    if (rewardLevel >= 25)
    {
        ChanceWhite		= 0;
        ChanceGreen		= 8;
        ChanceBlue		= 18;
        ChanceEpic		= 36;
        ChanceLegendary = 27;
        ChanceArtifact	= 11;
    }
    
    uint8 c_white_calc	= ChanceWhite;
    uint8 c_green_calc	= (c_white_calc + ChanceGreen);
    uint8 c_blue_calc	= (c_green_calc + ChanceBlue);
    uint8 c_epic_calc	= (c_blue_calc	+ ChanceEpic);
    uint8 c_legen_calc	= (c_epic_calc	+ ChanceLegendary);
    uint8 c_artif_calc	= (c_legen_calc + ChanceArtifact);
    
    uint32 itemId = 0;
    
    uint8 roll = (rand() % 100 + 1);
    
    if (ChanceWhite > 0)
        if (roll <= c_white_calc)
            itemId = SATCHEL_WHITE;
    if (ChanceGreen > 0)
        if (roll > c_white_calc && roll <= c_green_calc)
            itemId = SATCHEL_GREEN;
    if (ChanceBlue > 0)
        if (roll > c_green_calc && roll <= c_blue_calc)
            itemId = SATCHEL_BLUE;
    if (ChanceEpic > 0)
        if (roll > c_blue_calc && roll <= c_epic_calc)
            itemId = SATCHEL_EPIC;
    if (ChanceLegendary > 0)
        if (roll > c_epic_calc && roll <= c_legen_calc)
            itemId = SATCHEL_LEGENDARY;
    if (ChanceArtifact > 0)
        if (roll > c_legen_calc && roll <= 100)
            itemId = SATCHEL_ARTIFACT;
    
    int count = 3;
    ItemPosCountVec dest;
    uint32 noSpaceForCount = 0;
    InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemId, count, &noSpaceForCount);
    
    if (msg != EQUIP_ERR_OK)
        count -= noSpaceForCount;
    
    if (count == 0 || dest.empty())
    {
        MailSender sender(MAIL_NORMAL, 0, MAIL_STATIONERY_GM);
        MailDraft draft("Mythic Reward", "Here's your Mythic Reward.\n\nIt was sent to your mailbox because your inventory was full.");
        
        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        
        if (Item* mailItem = Item::CreateItem(itemId, 1, player->GetSession() ? player->GetSession()->GetPlayer() : 0))
        {
            mailItem->SaveToDB(trans);
            draft.AddItem(mailItem);
        }
        
        draft.SendMailTo(trans, MailReceiver(player, GUID_LOPART(player->GetGUID())), sender);
        CharacterDatabase.CommitTransaction(trans);
        ChatHandler(player->GetSession()).PSendSysMessage("Inventory full, reward was sent to your mailbox!");
    }
    else
        player->AddItem(itemId, 1);
    
    player->CastSpell(player, 18282, 1); // send new keystone info in players chat

    /* VISUAL SPELLS START */
    player->CastSpell(player, 48350, 1); // roar
    if (player->GetTeam() == ALLIANCE)
        player->CastSpell(player, 73322, 1); // blue visual
    else // if horde
        player->CastSpell(player, 70288, 1); // red visual
    /* VISUAL SPELLS END */

    if (Quest const* quest = sObjectMgr->GetQuestTemplate(CHALLENGE_MODE_QUEST_ID))
        player->RewardQuest(quest, 0, nullptr, false);

    if (Quest const* quest2 = sObjectMgr->GetQuestTemplate(CHALLENGE_MODE_QUEST_KILL_COUNT))
        player->RewardQuest(quest2, 0, nullptr, false);
}

std::string ChallengeModeMgr::GetKeystoneName(Player* player, uint32 mapId, uint8 level/* = 0*/)
{
    std::ostringstream ostream;

    MapEntry const* mapEntry = sMapStore.LookupEntry(mapId);

    if (!mapEntry)
        return "";

    Keystone const& keystone = player->GetKeystone();
    bool customLevel = level != 0;
    level = customLevel ? level : keystone.level;

    ostream << mapEntry->name << "\nDifficulty level: +" << uint32(level);

    return ostream.str();
}

std::string ChallengeModeMgr::GenerateAfficon(uint8 size, std::string icon_text)
{
    std::string start{ " |TInterface/ICONS/" };
    std::string end{ "|t|r " };

    std::ostringstream ostream{""};
    ostream << start << icon_text << ":" << uint32(size) << end;
    return ostream.str();
}

std::string ChallengeModeMgr::GetKeystoneNameCurrent(Player* player, uint32 mapId, uint8 level/* = 0*/)
{
    std::ostringstream ostream;

    MapEntry const* mapEntry = sMapStore.LookupEntry(mapId);

    if (!mapEntry)
        return "";

    Keystone const& keystone = player->GetKeystone();
    bool customLevel = level != 0;
    level = customLevel ? level : keystone.level;

    ostream << mapEntry->name << "\nDifficulty level: +" << uint32(level);

    if (!customLevel)
        if (keystone.affixes.front() != 0)
            ostream << " | Affixes:";

    return ostream.str();
}