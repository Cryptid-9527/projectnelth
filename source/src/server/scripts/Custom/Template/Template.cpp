/*
* Created by Aps
*/
#include "ScriptPCH.h"
#include "Template.h"
#include "Player.h"

const uint32 MIN_ITEM_LEVEL_AVAILABLE = 232;
const uint32 MAX_ITEM_LEVEL_AVAILABLE = 416;

void sTemplate::LearnPlateMailSpells(Player* player)
{
	switch (player->getClass())
	{
	case CLASS_WARRIOR:
	case CLASS_PALADIN:
	case CLASS_DEATH_KNIGHT:
		player->learnSpell(PLATE_MAIL, true);
		break;
	case CLASS_SHAMAN:
	case CLASS_HUNTER:
		player->learnSpell(MAIL, true);
		break;
	default:
		break;
	}
}

void sTemplate::ApplyBonus(Player* player, Item* item, EnchantmentSlot slot, uint32 bonusEntry)
{
	if (!item)
		return;

	if (!bonusEntry || bonusEntry == 0)
		return;

	player->ApplyEnchantment(item, slot, false);
	item->SetEnchantment(slot, bonusEntry, 0, 0);
	player->ApplyEnchantment(item, slot, true);
}

void sTemplate::ApplyGlyph(Player* player, uint8 slot, uint32 glyphID)
{
	if (GlyphPropertiesEntry const* gp = sGlyphPropertiesStore.LookupEntry(glyphID))
	{
		if (uint32 oldGlyph = player->GetGlyph(player->GetActiveSpec(), slot))
		{
			player->RemoveAurasDueToSpell(sGlyphPropertiesStore.LookupEntry(oldGlyph)->SpellId);
			player->SetGlyph(slot, 0);
		}
		player->CastSpell(player, gp->SpellId, true);
		player->SetGlyph(slot, glyphID);
	}

	player->SendTalentsInfoData(false);
}

void sTemplate::ClearGlyphs(Player* player)
{
	for (int i = 0; i < MAX_GLYPH_SLOT_INDEX; i++)
	{
		if (uint32 oldGlyph = player->GetGlyph(player->GetActiveSpec(), i))
		{
			player->RemoveAurasDueToSpell(sGlyphPropertiesStore.LookupEntry(oldGlyph)->SpellId);
			player->SetGlyph(i, 0);
		}
	}
}

void sTemplate::LearnTemplateTalents(Player* player, PlayerTemplate playerTemplate)
{
	UpdatePrimaryTalentTree(player, playerTemplate.spec.c_str());
	for (auto& talent : playerTemplate.talents)
	{
		player->LearnTalentByForce(talent.talentId, talent.talentRank);
	}
	player->SendTalentsInfoData(false);
}

void sTemplate::LearnTemplateGlyphs(Player* player, PlayerTemplate playerTemplate)
{
	for (auto& glyph : playerTemplate.glyphs)
	{
		ApplyGlyph(player, glyph.slot, glyph.glyph);
	}
}

void sTemplate::EquipTemplateGear(Player* player, PlayerTemplate playerTemplate)
{
	GearTemplates itemSet = ChoseGearTemplate(player, playerTemplate);

	for (auto& item : itemSet)
	{
		player->EquipNewItem(item.pos, item.itemEntry, true); // Equip the item and apply enchants and gems
		ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, item.pos),
			PERM_ENCHANTMENT_SLOT, item.enchant);
		ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, item.pos),
			SOCK_ENCHANTMENT_SLOT, item.socket1);
		ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, item.pos),
			SOCK_ENCHANTMENT_SLOT_2, item.socket2);
		ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, item.pos),
			SOCK_ENCHANTMENT_SLOT_3, item.socket3);
		ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, item.pos),
			BONUS_ENCHANTMENT_SLOT, item.bonusEnchant);
		ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, item.pos),
			PRISMATIC_ENCHANTMENT_SLOT, item.prismaticEnchant);
		ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, item.pos),
			REFORGE_ENCHANTMENT_SLOT, item.reforge);
	}
}

GearTemplates sTemplate::ChoseGearTemplate(Player* player, PlayerTemplate m_template)
{
	GearTemplatesMap templates = m_template.gear;

	GearTemplates tempToUse;

	if (templates.size() < 1)
		return tempToUse;

	// if we the same race exists then we can simply use that
	if (templates.find(player->getRace()) != templates.end())
		return templates[player->getRace()];

	// if another race from our faction exists, use that
	for (auto& raceTemplate : templates)
	{
		if (player->TeamForRace(raceTemplate.first) == player->GetTeam())
		{
			return raceTemplate.second;
		}
	}

	// use any available template
	if (templates.begin() != templates.end())
		return templates.begin()->second;

	return tempToUse;
}

bool sTemplate::UpdatePrimaryTalentTree(Player* player, const char* spec)
{
	// Aps: find the talent tree id
	uint32 _talentTree = 0;
	for (auto& talentTree : m_TalentTreeContainer)
	{

		if (talentTree.playerClass == GetClassString(player->getClass()).c_str() && talentTree.playerSpec == spec)
		{
			_talentTree = talentTree.talentTree;
			break;
		}
	}

	// In case a wrong class / spec combination is entered
	if (_talentTree == 0)
		return false;

	// set talent tree for player
	if (!player->GetPrimaryTalentTree(player->GetActiveSpec()))
	{
		TalentTabEntry const* talentTabInfo = sTalentTabStore.LookupEntry(_talentTree);

		player->SetPrimaryTalentTree(player->GetActiveSpec(), _talentTree);
		std::vector<uint32> const* specSpells = GetTalentTreePrimarySpells(_talentTree);
		if (specSpells)
			for (size_t i = 0; i < specSpells->size(); ++i)
				player->learnSpell(specSpells->at(i), false);

		if (player->IsMasteryLearned())
			for (uint32 i = 0; i < MAX_MASTERY_SPELLS; ++i)
				if (uint32 mastery = talentTabInfo->MasterySpells[i])
					player->learnSpell(mastery, false);
	}

	player->SetPrimaryTalentTree(player->GetActiveSpec(), _talentTree);
	return true;
}

void sTemplate::LoadTalentTrees()
{
	uint32 oldMSTime = getMSTime();
	uint32 count = 0;

	// Load the talent trees
	QueryResult result = CharacterDatabase.PQuery("SELECT `playerClass`, `playerSpec`, `talentTree` FROM `template_npc_talent_trees`;");
	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			std::string playerClass = fields[0].GetString();
			std::string playerSpec = fields[1].GetString();
			uint32 tree = fields[2].GetUInt32();
			talentTreeNames[tree] = playerSpec;

			TalentTreeTemplate newTemplate;
			newTemplate.playerClass = playerClass;
			newTemplate.playerSpec = playerSpec;
			newTemplate.talentTree = tree;
			m_TalentTreeContainer.emplace_back(newTemplate);
			count++;
		} while (result->NextRow());
	}
	//sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u talent trees in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

void sTemplate::LoadNpcTemplates()
{
	// Load the max template id
	QueryResult result = CharacterDatabase.PQuery("SELECT max(`id`) FROM `template_npc_templates`;");
	if (result)
	{
		Field* fields = result->Fetch();
		uint32 id = fields[0].GetUInt32() + 1;
		nextTemplateId = id;
	}
	else
	{
		nextTemplateId = 1;
	}

	uint32 oldMSTime = getMSTime();
	uint32 count = 0;

	// Load all Templates with character names
	QueryResult tempResult = CharacterDatabase.PQuery("SELECT `id`, `class_id`, `name`, `spec`, `category` FROM `template_npc_templates`;");
	if (tempResult)
	{
		// for each table row do
		do
		{
			Field* fields = tempResult->Fetch();
			uint32 id = fields[0].GetUInt32();
			uint32 class_id = fields[1].GetUInt32();
			std::string name = fields[2].GetString();
			std::string spec = fields[3].GetString();
			uint8 category = fields[4].GetUInt8();

			PlayerTemplate npcTemplate;
			npcTemplate.id = id;
			npcTemplate.class_id = class_id;
			npcTemplate.name = name;
			npcTemplate.spec = spec;
			npcTemplate.category = category;

			// Load all talents for this template
			QueryResult talentsResult = CharacterDatabase.PQuery("SELECT `talent_id`, `talent_rank` FROM `template_npc_talents` WHERE `template_id` = %u;", id);
			if (talentsResult)
			{
				do
				{
					// Fetch fields from the table
					Field* fields = talentsResult->Fetch();
					uint32 talentId = fields[0].GetUInt32();
					uint32 talentRank = fields[1].GetUInt32();
					// Create a new talent record
					TalentTemplate curTalent;
					curTalent.talentId = talentId;
					curTalent.talentRank = talentRank;
					// Place the talent inside the current npc template
					npcTemplate.talents.emplace_back(curTalent);

				} while (talentsResult->NextRow());
			}

			// Load all glyphs for this template
			QueryResult glyphsResult = CharacterDatabase.PQuery("SELECT `slot`, `glyph` FROM `template_npc_glyphs` WHERE `template_id` = %u;", id);
			if (glyphsResult)
			{
				do
				{
					// Fetch fields from the table
					Field* fields = glyphsResult->Fetch();
					uint32 slot = fields[0].GetUInt32();
					uint32 glyph = fields[1].GetUInt32();
					// Create a new glyph record
					GlyphTemplate curGlyph;
					curGlyph.slot = slot;
					curGlyph.glyph = glyph;
					// Place the glyph inside the current npc template
					npcTemplate.glyphs.emplace_back(curGlyph);

				} while (glyphsResult->NextRow());
			}

			// Load all gear for this template
			QueryResult gearResult = CharacterDatabase.PQuery("SELECT * FROM `template_npc_gear` WHERE `template_id` = %u;", id);
			if (gearResult)
			{
				do
				{
					// Fetch fields from the table
					Field* fields = gearResult->Fetch();
					uint32 race_id = fields[1].GetUInt32();
					uint32 pos = fields[2].GetUInt32();
					uint32 itemEntry = fields[3].GetUInt32();
					uint32 enchant = fields[4].GetUInt32();
					uint32 socket1 = fields[5].GetUInt32();
					uint32 socket2 = fields[6].GetUInt32();
					uint32 socket3 = fields[7].GetUInt32();
					uint32 bonusEnchant = fields[8].GetUInt32();
					uint32 prismaticEnchant = fields[9].GetUInt32();
					uint32 reforge = fields[10].GetUInt32();

					// Create a new gear record
					GearTemplate curGear;
					curGear.race_id = race_id;
					curGear.pos = pos;
					curGear.itemEntry = itemEntry;
					curGear.enchant = enchant;
					curGear.socket1 = socket1;
					curGear.socket2 = socket2;
					curGear.socket3 = socket3;
					curGear.bonusEnchant = bonusEnchant;
					curGear.prismaticEnchant = prismaticEnchant;
					curGear.reforge = reforge;

					// Place the glyph inside the current npc template
					npcTemplate.gear[race_id].emplace_back(curGear);

				} while (gearResult->NextRow());
			}

			// Save the template inside the NpcTemplates container
			m_ServerTemplates[class_id][spec][category][name] = npcTemplate;
			count++;
		} while (tempResult->NextRow());
	}
	//sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u talent templates in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

std::string sTemplate::GetClassString(uint32 classId)
{
	switch (classId)
	{
	case CLASS_PRIEST:       return "Priest";      break;
	case CLASS_PALADIN:      return "Paladin";     break;
	case CLASS_WARRIOR:      return "Warrior";     break;
	case CLASS_MAGE:         return "Mage";        break;
	case CLASS_WARLOCK:      return "Warlock";     break;
	case CLASS_SHAMAN:       return "Shaman";      break;
	case CLASS_DRUID:        return "Druid";       break;
	case CLASS_HUNTER:       return "Hunter";      break;
	case CLASS_ROGUE:        return "Rogue";       break;
	case CLASS_DEATH_KNIGHT: return "DeathKnight"; break;
	default:
		break;
	}
	return "Unknown"; // Fix warning, this should never happen
}

bool sTemplate::OverwriteTemplateForPlayer(Player* player, std::string& playerSpecStr, std::string& name)
{
	PlayerTemplate templateToDelete;

	// Remove template from player templates
	for (auto m_template : player->m_templates)
	{
		if (m_template.second.name == name)
		{
			player->m_templates.erase(m_template.second.id);
			sTemplateMgr->playerTemplatesContainer[player->GetSession()->GetAccountId()].erase(m_template.second.id);
			templateToDelete = m_template.second;
		}
	}
	// Remove template from saving queue
	if (sTemplateMgr->newPlayerTemplatesContainer.find(player->GetSession()->GetAccountId()) != sTemplateMgr->newPlayerTemplatesContainer.end())
	{
		PlayerTemplates myTemplates = sTemplateMgr->newPlayerTemplatesContainer.find(player->GetSession()->GetAccountId())->second;

		if (myTemplates.size() > 0)
		{
			// Loop through all new templates of this player
			for (auto& playerTemplate : myTemplates)
			{
				if (playerTemplate.second.name == name)
				{
					sTemplateMgr->newPlayerTemplatesContainer[player->GetSession()->GetAccountId()].erase(playerTemplate.second.id);
					if (sTemplateMgr->updatedPlayerTemplatesContainer.find(player->GetSession()->GetAccountId()) != sTemplateMgr->updatedPlayerTemplatesContainer.end())
					{
						sTemplateMgr->updatedPlayerTemplatesContainer[player->GetSession()->GetAccountId()].erase(playerTemplate.second.id);
					}
				}
			}
		}
	}
	// If it's not in the saving queue then it should be in the db
	else
	{
		// check if it's already queued to be deleted
		bool alreadyDeleted = false;
		if (sTemplateMgr->deletedPlayerTemplatesContainer.find(player->GetSession()->GetAccountId()) != sTemplateMgr->deletedPlayerTemplatesContainer.end())
		{
			PlayerTemplates deletedTemplates = sTemplateMgr->deletedPlayerTemplatesContainer.find(player->GetSession()->GetAccountId())->second;

			if (deletedTemplates.size() > 0)
			{
				// Loop through all new templates of this player
				for (auto& deletedTemplate : deletedTemplates)
				{
					if (deletedTemplate.second.name == name)
					{
						alreadyDeleted = true;
					}
				}
			}
		}
		// if it's not already deleted then we need to delete it
		if (!alreadyDeleted)
		{
			sTemplateMgr->deletedPlayerTemplatesContainer[player->GetSession()->GetAccountId()][templateToDelete.id] = templateToDelete;
		}
		// remove the update queue if one exists
		if (sTemplateMgr->updatedPlayerTemplatesContainer.find(player->GetSession()->GetAccountId()) != sTemplateMgr->updatedPlayerTemplatesContainer.end())
		{
			sTemplateMgr->updatedPlayerTemplatesContainer[player->GetSession()->GetAccountId()].erase(templateToDelete.id);
		}
	}
	return true;
}

bool sTemplate::IsAllowedToStoreOrEquip(uint32 itemId)
{
	ItemTemplate const* itemProto = sObjectMgr->GetItemTemplate(itemId);

	if (!itemProto)
		return false;

	if (itemProto->ItemLevel > MAX_ITEM_LEVEL_AVAILABLE ||
		itemProto->ItemLevel < MIN_ITEM_LEVEL_AVAILABLE)
		return false;

	if (itemProto->Quality > 4)
		return false;

	// Tabards and shirts are items with level lower
	// than 232, so we don't have to implement a separate
	// check for gladiator's tabards or any shirts which
	// players won't be able to copy

	return true;
}

void sTemplate::ExtractNamedTemplateToDB(Player* player, uint8 category, std::string name)
{
	PlayerTemplate gearOnlyTemplate;
	bool gearOnly = false;
	std::string spec = sTemplateMgr->GetSpecNameFromTree(player->GetPrimaryTalentTree(player->GetActiveSpec()));

	// Aps: First I check if this name already exist for this spec
	for (auto& specTemplate : sTemplateMgr->m_ServerTemplates[player->getClass()][spec][category])
	{
		if (specTemplate.second.name == name)
		{
			gearOnlyTemplate.id = specTemplate.second.id;
			gearOnlyTemplate.class_id = specTemplate.second.class_id;
			gearOnlyTemplate.spec = specTemplate.second.spec;
			gearOnly = true;
		}
	}
	// Aps: If it exists then we only need to extract new gear for this template
	// Gears are stored based on the race of the character
	// This allows us to extract specific gear for humans for example
	if (gearOnly)
	{
		// Aps: Check if a template for this race already exists
		if (gearOnlyTemplate.gear.find(player->getRace()) != gearOnlyTemplate.gear.end())
		{
			ChatHandler(player->GetSession()).PSendSysMessage("A gear template for this race already exists. You need to delete the template before you can extract a new one.");
			return;
		}
		else
		{
			gearOnlyTemplate.gear[player->getRace()] = ExtractGearTemplate(player);
			StoreTemplateToDB(gearOnlyTemplate, true);
			// Store in runtime
			sTemplateMgr->m_ServerTemplates[gearOnlyTemplate.class_id][gearOnlyTemplate.spec][category][name].gear = gearOnlyTemplate.gear;
		}
	}
	else
	{
		PlayerTemplate npcTemplate;
		npcTemplate.id = sTemplateMgr->nextTemplateId;
		npcTemplate.class_id = player->getClass();
		npcTemplate.name = name;
		npcTemplate.spec = spec;
		npcTemplate.category = category;

		npcTemplate.gear[player->getRace()] = ExtractGearTemplate(player);
		npcTemplate.talents = ExtractTalentTemplate(player);
		npcTemplate.glyphs = ExtractGlyphTemplate(player);

		StoreTemplateToDB(npcTemplate);

		// Store in runtime
		sTemplateMgr->m_ServerTemplates[npcTemplate.class_id][npcTemplate.spec][category][name] = npcTemplate;
		// Increment the next template id
		sTemplateMgr->nextTemplateId++;
	}
}

GearTemplates sTemplate::ExtractGearTemplate(Player* player)
{
	GearTemplates gear;

	for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i)
	{
		if (Item* equippedItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
		{
			if (!IsAllowedToStoreOrEquip(equippedItem->GetEntry()))
				continue;

			GearTemplate item;
			item.race_id = player->getRace();
			item.pos = equippedItem->GetSlot();
			item.itemEntry = equippedItem->GetEntry();
			item.enchant = equippedItem->GetEnchantmentId(PERM_ENCHANTMENT_SLOT);
			item.socket1 = equippedItem->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT);
			item.socket2 = equippedItem->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT_2);
			item.socket3 = equippedItem->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT_3);
			item.bonusEnchant = equippedItem->GetEnchantmentId(BONUS_ENCHANTMENT_SLOT);
			item.prismaticEnchant = equippedItem->GetEnchantmentId(PRISMATIC_ENCHANTMENT_SLOT);
			item.reforge = equippedItem->GetEnchantmentId(REFORGE_ENCHANTMENT_SLOT);

			gear.emplace_back(item);
		}
	}

	return gear;
}

std::list<TalentTemplate> sTemplate::ExtractTalentTemplate(Player* player)
{
	std::list<TalentTemplate> talents;

	for (uint32 talentEntry = 0; talentEntry < sTalentStore.GetNumRows(); ++talentEntry)
	{
		TalentEntry const* talentInfo = sTalentStore.LookupEntry(talentEntry);

		if (!talentInfo)
			continue;

		for (int8 rank = MAX_TALENT_RANK - 1; rank >= 0; --rank)
		{
			if (talentInfo->RankID[rank] == 0)
				continue;

			if (player->HasTalent(talentInfo->RankID[rank], player->GetActiveSpec()))
			{
				TalentTemplate talent;
				talent.talentId = talentEntry;
				talent.talentRank = rank;

				talents.emplace_back(talent);
			}
		}
	}

	return talents;
}

PlayerTemplate sTemplate::ExtractTalentTemplateForPlayer(Player* player, std::string& playerSpecStr, std::string& name)
{
	// Create the new Template Object
	PlayerTemplate m_template;
	m_template.id = sTemplateMgr->nextId;
	m_template.account_id = player->GetSession()->GetAccountId();
	m_template.class_id = player->getClass();
	m_template.name = name;
	m_template.spec = playerSpecStr;
	m_template.isPublic = false;
	m_template.publisher = 0;

	// Extract player talents
	for (uint32 talentEntry = 0; talentEntry < sTalentStore.GetNumRows(); ++talentEntry)
	{
		TalentEntry const* talentInfo = sTalentStore.LookupEntry(talentEntry);
		if (!talentInfo)
			continue;

		for (int8 rank = MAX_TALENT_RANK - 1; rank >= 0; --rank)
		{
			if (talentInfo->RankID[rank] == 0)
				continue;

			if (player->HasTalent(talentInfo->RankID[rank], player->GetActiveSpec()))
			{
				TalentTemplate tempTalent;
				tempTalent.talentId = talentEntry;
				tempTalent.talentRank = (int)rank;

				m_template.talents.emplace_back(tempTalent);
			}
		}
	}
	// Extract player glyphs
	for (uint8 slot = 0; slot < MAX_GLYPH_SLOT_INDEX; ++slot)
	{
		uint32 storedGlyph = player->GetGlyph(player->GetActiveSpec(), slot);
		if (storedGlyph == 0)
			continue;

		GlyphTemplate tempGlyph;
		tempGlyph.slot = (int)slot;
		tempGlyph.glyph = (int)storedGlyph;

		m_template.glyphs.emplace_back(tempGlyph);
	}
	// Save template in player's templates
	player->m_templates[m_template.id] = m_template;
	// Increment next Id;
	sTemplateMgr->nextId++;

	return m_template;
}

std::list<GlyphTemplate> sTemplate::ExtractGlyphTemplate(Player* player)
{
	std::list<GlyphTemplate> glyphs;

	for (uint8 slot = 0; slot < MAX_GLYPH_SLOT_INDEX; ++slot)
	{
		uint32 storedGlyph = player->GetGlyph(player->GetActiveSpec(), slot);

		if (storedGlyph == 0)
			continue;

		GlyphTemplate glyph;
		glyph.glyph = storedGlyph;
		glyph.slot = slot;

		glyphs.emplace_back(glyph);
	}

	return glyphs;
}

void sTemplate::StoreTemplateToDB(PlayerTemplate npcTemplate, bool gearOnly)
{
	// Initialise first value booleans
	bool firstItem = true;
	bool firstTalent = true;
	bool firstGlyph = true;

	// Aps: First we save the gear, in case the developer requested a gearOnly save
	// Initialise template_npc_gear query
	std::stringstream gearQuery;
	gearQuery << "INSERT INTO `template_npc_gear` (`template_id`, `race_id`, `pos`, `itemEntry`, `enchant`, `socket1`, `socket2`, `socket3`, `bonusEnchant`, `prismaticEnchant`, `reforge`) VALUES ";

	// Loop through all the gear for this template
	for (auto& thisItem : npcTemplate.gear)
	{
		for (auto& item : thisItem.second)
		{
			if (!firstItem)
				gearQuery << ", ";
			firstItem = false;
			// New item
			gearQuery << "(" << npcTemplate.id << ", " << item.race_id << ", " << item.pos << ", " << item.itemEntry << ", " << item.enchant << ", " << item.socket1 << ", " << item.socket2 << ", " << item.socket3 << ", " << item.bonusEnchant << ", " << item.prismaticEnchant << ", " << item.reforge << ")";
		}
	}
	// Run the query
	CharacterDatabase.PExecute(gearQuery.str().c_str());

	// If gearOnly was requested we stop here
	if (gearOnly)
		return;

	// Insert the template into the db
	CharacterDatabase.PExecute("INSERT INTO `template_npc_templates` (`id`, `class_id`, `name`, `spec`, `category`) VALUES (%u, %u, '%s', '%s', %u)",
		npcTemplate.id,
		npcTemplate.class_id,
		npcTemplate.name.c_str(),
		npcTemplate.spec.c_str(),
		npcTemplate.category);

	// Initialise template_npc_talents query
	std::stringstream talentQuery;
	talentQuery << "INSERT INTO `template_npc_talents` (`template_id`, `talent_id`, `talent_rank`) VALUES ";
	// Initialise template_npc_glyphs query
	std::stringstream glyphQuery;
	glyphQuery << "INSERT INTO `template_npc_glyphs` (`template_id`, `slot`, `glyph`) VALUES ";

	// Loop through all the talent points for this template
	for (auto& talent : npcTemplate.talents)
	{
		if (!firstTalent)
			talentQuery << ", ";
		firstTalent = false;
		// New talent point
		talentQuery << "(" << npcTemplate.id << ", " << talent.talentId << ", " << talent.talentRank << ")";
	}

	// Loop through all the glyphs for this template
	for (auto& glyph : npcTemplate.glyphs)
	{
		if (!firstGlyph)
			glyphQuery << ", ";
		firstGlyph = false;
		// New glyph point
		glyphQuery << "(" << npcTemplate.id << ", " << glyph.slot << ", " << glyph.glyph << ")";
	}

	// Run the queries
	if (!firstTalent)
		CharacterDatabase.PExecute(talentQuery.str().c_str());
	if (!firstGlyph)
		CharacterDatabase.PExecute(glyphQuery.str().c_str());
}

void sTemplate::DeleteTemplateFromDB(uint32 templateId)
{
	CharacterDatabase.PExecute("DELETE FROM `template_npc_templates` WHERE `id` = %u",
		templateId);

	CharacterDatabase.PExecute("DELETE FROM `template_npc_gear` WHERE `template_id` = %u",
		templateId);

	CharacterDatabase.PExecute("DELETE FROM `template_npc_talents` WHERE `template_id` = %u",
		templateId);

	CharacterDatabase.PExecute("DELETE FROM `template_npc_glyphs` WHERE `template_id` = %u",
		templateId);
}

bool sTemplate::TemplateExists(Player* player, uint32 template_id)
{
	if (!GetTemplate(template_id))
		return false;

	return true;
}

std::string sTemplate::GetSpecNameFromTree(uint32 tree)
{
	return sTemplateMgr->talentTreeNames[tree];
}

std::string sTemplate::GetSpecNameFromIndex(uint8 classId, uint8 index)
{
	return GetClassSpecs(classId).at(index).at(0);
}

ClassSpecs sTemplate::GetClassSpecs(uint8 classId, bool toLower /*= false*/)
{
	ClassSpecs Specs(3);
	std::string size = "30";

	switch (classId)
	{
	case CLASS_PRIEST:
	{
		Specs.at(0).emplace_back("Discipline"); // spec names
		Specs.at(0).emplace_back("TInterface\\icons\\spell_holy_wordfortitude:" + size + "|t|r"); // spec icons

		Specs.at(1).emplace_back("Holy");
		Specs.at(1).emplace_back("TInterface\\icons\\spell_holy_holybolt:" + size + "|t|r");

		Specs.at(2).emplace_back("Shadow");
		Specs.at(2).emplace_back("TInterface\\icons\\spell_shadow_shadowwordpain:" + size + "|t|r");
	}
	break;
	case CLASS_PALADIN:
	{
		Specs.at(0).emplace_back("Holy");
		Specs.at(0).emplace_back("TInterface\\icons\\spell_holy_holybolt:" + size + "|t|r");

		Specs.at(1).emplace_back("Protection");
		Specs.at(1).emplace_back("TInterface\\icons\\spell_holy_devotionaura:" + size + "|t|r");

		Specs.at(2).emplace_back("Retribution");
		Specs.at(2).emplace_back("TInterface\\icons\\spell_holy_auraoflight:" + size + "|t|r");
	}
	break;
	case CLASS_WARRIOR:
	{
		Specs.at(0).emplace_back("Fury");
		Specs.at(0).emplace_back("TInterface\\icons\\ability_warrior_innerrage:" + size + "|t|r");

		Specs.at(1).emplace_back("Arms");
		Specs.at(1).emplace_back("TInterface\\icons\\ability_rogue_eviscerate:" + size + "|t|r");

		Specs.at(2).emplace_back("Protection");
		Specs.at(2).emplace_back("TInterface\\icons\\ability_warrior_defensivestance:" + size + "|t|r");
	}
	break;
	case CLASS_MAGE:
	{
		Specs.at(0).emplace_back("Arcane");
		Specs.at(0).emplace_back("TInterface\\icons\\spell_holy_magicalsentry:" + size + "|t|r");

		Specs.at(1).emplace_back("Fire");
		Specs.at(1).emplace_back("TInterface\\icons\\spell_fire_flamebolt:" + size + "|t|r");

		Specs.at(2).emplace_back("Frost");
		Specs.at(2).emplace_back("TInterface\\icons\\spell_frost_frostbolt02:" + size + "|t|r");

	}
	break;
	case CLASS_WARLOCK:
	{
		Specs.at(0).emplace_back("Affliction");
		Specs.at(0).emplace_back("TInterface\\icons\\spell_shadow_deathcoil:" + size + "|t|r");

		Specs.at(1).emplace_back("Demonology");
		Specs.at(1).emplace_back("TInterface\\icons\\spell_shadow_metamorphosis:" + size + "|t|r");

		Specs.at(2).emplace_back("Destruction");
		Specs.at(2).emplace_back("TInterface\\icons\\spell_shadow_rainoffire:" + size + "|t|r");
	}
	break;
	case CLASS_SHAMAN:
	{
		Specs.at(0).emplace_back("Elemental");
		Specs.at(0).emplace_back("TInterface\\icons\\spell_nature_lightning:" + size + "|t|r");

		Specs.at(1).emplace_back("Enhancement");
		Specs.at(1).emplace_back("TInterface\\icons\\spell_nature_lightningshield:" + size + "|t|r");

		Specs.at(2).emplace_back("Restoration");
		Specs.at(2).emplace_back("TInterface\\icons\\spell_nature_magicimmunity:" + size + "|t|r");
	}
	break;
	case CLASS_DRUID:
	{
		Specs.at(0).emplace_back("Balance");
		Specs.at(0).emplace_back("TInterface\\icons\\spell_nature_starfall:" + size + "|t|r");

		Specs.at(1).emplace_back("Feral");
		Specs.at(1).emplace_back("TInterface\\icons\\ability_racial_bearform:" + size + "|t|r");

		Specs.at(2).emplace_back("Restoration");
		Specs.at(2).emplace_back("TInterface\\icons\\spell_nature_healingtouch:" + size + "|t|r");
	}
	break;
	case CLASS_HUNTER:
	{
		Specs.at(0).emplace_back("Marksmanship");
		Specs.at(0).emplace_back("TInterface\\icons\\ability_marksmanship:" + size + "|t|r");

		Specs.at(1).emplace_back("Beast Mastery");
		Specs.at(1).emplace_back("TInterface\\icons\\ability_hunter_beasttaming:" + size + "|t|r");

		Specs.at(2).emplace_back("Survival");
		Specs.at(2).emplace_back("TInterface\\icons\\ability_Hunter_swiftstrike:" + size + "|t|r");
	}
	break;
	case CLASS_ROGUE:
	{
		Specs.at(0).emplace_back("Assassination");
		Specs.at(0).emplace_back("TInterface\\icons\\ability_rogue_eviscerate:" + size + "|t|r");

		Specs.at(1).emplace_back("Combat");
		Specs.at(1).emplace_back("TInterface\\icons\\ability_backstab:" + size + "|t|r");

		Specs.at(2).emplace_back("Subtlety");
		Specs.at(2).emplace_back("TInterface\\icons\\ability_stealth:" + size + "|t|r");
	}
	break;
	case CLASS_DEATH_KNIGHT:
	{
		Specs.at(0).emplace_back("Blood");
		Specs.at(0).emplace_back("TInterface\\icons\\spell_deathknight_bloodpresence:" + size + "|t|r");

		Specs.at(1).emplace_back("Frost");
		Specs.at(1).emplace_back("TInterface\\icons\\spell_deathknight_frostpresence:" + size + "|t|r");

		Specs.at(2).emplace_back("Unholy");
		Specs.at(2).emplace_back("TInterface\\icons\\spell_deathknight_unholypresence:" + size + "|t|r");
	}
	break;
	}
	if (toLower)
	{
		std::transform(Specs.at(0).at(0).begin(), Specs.at(0).at(0).end(), Specs.at(0).at(0).begin(), ::tolower);
		std::transform(Specs.at(1).at(0).begin(), Specs.at(1).at(0).end(), Specs.at(1).at(0).begin(), ::tolower);
		std::transform(Specs.at(2).at(0).begin(), Specs.at(2).at(0).end(), Specs.at(2).at(0).begin(), ::tolower);
	}
	return Specs;
}

void sTemplate::ApplyTemplate(Player* player, uint32 template_id, bool learnTalents, bool equipGear)
{
	PlayerTemplate m_template = GetTemplate(template_id);
	// Check if a template exists in the db
	if (!m_template)
	{
		player->GetSession()->SendAreaTriggerMessage("There is no template for %s specialization yet.", m_template.spec.c_str());
		return;
	}

	// Aps: If gear requested make sure character is not already wearing any gear
	if (equipGear)
	{
		for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i)
		{
			if (Item* haveItemEquipped = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
			{
				if (haveItemEquipped)
				{
					// Tabard and Shirt do not need to be removed
					if (i == EQUIPMENT_SLOT_TABARD || i == EQUIPMENT_SLOT_BODY)
						continue;

					player->UnequipItem(haveItemEquipped);
				}
			}
		}
	}

	// Aps: If talents requested, check if character has spent any talent points already
	if (player->GetFreeTalentPoints() < 41 && learnTalents)
	{
		// Aps: Reset talent points and update the client
		player->ResetTalents(true);
	}

	// Aps: Plate/Mail should always be learnt
	this->LearnPlateMailSpells(player);

	if (learnTalents)
	{
		LearnTemplateTalents(player, m_template);
		// Remove old glyphs
		ClearGlyphs(player);
		// Learn new glyphs
		LearnTemplateGlyphs(player, m_template);
	}
	if (equipGear)
	{
		EquipTemplateGear(player, m_template);
	}

	player->DestroyItemCount(100005, 1, true);
	player->GetSession()->SendAreaTriggerMessage("%s %s template applied!", m_template.spec.c_str(), GetClassString(player->getClass()).c_str());
}

bool sTemplate::ApplyTemplateForPlayer(Player* player, int id)
{
	// Find the template
	for (auto& m_template : player->m_templates)
	{
		if (m_template.second.id == id)
		{
			// Check if character has spent any talent points already
			if (player->GetFreeTalentPoints() < 41)
			{
				// Aps: Reset talent points and update the client
				player->ResetTalents(true);
				//player->SendTalentsInfoData(false);
			}
			// Update talent tree
			UpdatePrimaryTalentTree(player, m_template.second.spec.c_str());
			// Learn new talents
			for (auto& talent : m_template.second.talents)
			{
				player->LearnTalentByForce(talent.talentId, talent.talentRank);
			}
			// Update client with new talents
			player->SendTalentsInfoData(false);

			// Remove old glyphs
			ClearGlyphs(player);
			// Learn new glyphs
			for (auto& glyph : m_template.second.glyphs)
			{
				ApplyGlyph(player, glyph.slot, glyph.glyph);
			}
			return true;
		}
	}
	return false;
}

PlayerTemplate sTemplate::GetTemplate(uint32 templateId)
{
	// Aps: this might not seem very efficient, but considering there can ever only be 10 classes, 3 specs and 2 categories it's ok
	for (auto& classTemplates : m_ServerTemplates)
	{
		for (auto& specTemplates : classTemplates.second)
		{
			for (auto& categoryTemplates : specTemplates.second)
			{
				for (auto& templates : categoryTemplates.second)
				{
					if (templates.second.id == templateId)
						return templates.second;
				}
			}
		}
	}
	// Aps: Just in case
	PlayerTemplate null_template;
	return null_template;
}