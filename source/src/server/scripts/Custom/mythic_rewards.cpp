#include "ScriptMgr.h"
#include "mythic_rewards.h"
#include "ChallengeModeMgr.h"

class mythic_satchel : public ItemScript
{
public:
	mythic_satchel() : ItemScript("mythic_satchel") { }

	bool OnUse(Player* player, Item* item, const SpellCastTargets& /*targets*/)
	{
		if (player->HasSpellCooldown(90065))
		{
			player->GetSession()->SendNotification("|cffff0000You cannot open your Mythic Satchel yet, try again later.|r");
			return false;
		}

		if (player->isInCombat() || player->InArena() || player->InBattleground())
		{
			player->GetSession()->SendNotification("|cffff0000You cannot open your Mythic Satchel while in combat or present in an arena / battleground.|r");
			return false;
		}

		uint32 noSpaceForCount = 0;
		int count = 3;

		ItemPosCountVec dest;
		InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, item->GetEntry(), count, &noSpaceForCount);
		if (msg != EQUIP_ERR_OK)
			count -= noSpaceForCount;

		if (count == 0 || dest.empty())
		{
			player->GetSession()->SendNotification("|cffff0000Cleanup your inventory, you need atleast 1 free slot in your inventory in order to open your Mythic Satchel.|r");
			return false;
		}

		uint8 level = 0;
		uint8 currencyChance = 0;
		uint16 vpCount = 0;
		uint16 jpCount = 0;
		uint16 goldrange = 0;
		uint32 nextItem = 0;
		std::stringstream ss;
		std::string lootTableTier;
		std::string lootTableArmor;
		std::string lootTableClass;

		switch (item->GetEntry())
		{
		case SATCHEL_WHITE:
		{
			level = 1;
			currencyChance = 10;
			vpCount = 25;
			jpCount = 50;
			goldrange = urand(35, 50);
			nextItem = SATCHEL_GREEN;

			switch (player->getClass())
			{
			case CLASS_PRIEST:			lootTableClass = "T1_PRIEST";      lootTableArmor = "T1_CLOTH";   break;
			case CLASS_MAGE:			lootTableClass = "T1_MAGE";        lootTableArmor = "T1_CLOTH";   break;
			case CLASS_WARLOCK:			lootTableClass = "T1_WARLOCK";     lootTableArmor = "T1_CLOTH";   break;
			case CLASS_ROGUE:			lootTableClass = "T1_ROGUE";       lootTableArmor = "T1_LEATHER"; break;
			case CLASS_DRUID:			lootTableClass = "T1_DRUID";       lootTableArmor = "T1_LEATHER"; break;
			case CLASS_HUNTER:			lootTableClass = "T1_HUNTER";      lootTableArmor = "T1_MAIL";    break;
			case CLASS_SHAMAN:			lootTableClass = "T1_SHAMAN";	   lootTableArmor = "T1_MAIL";    break;
			case CLASS_WARRIOR:			lootTableClass = "T1_WARRIOR";	   lootTableArmor = "T1_PLATE";   break;
			case CLASS_PALADIN:			lootTableClass = "T1_PALADIN";     lootTableArmor = "T1_PLATE";   break;
			case CLASS_DEATH_KNIGHT:	lootTableClass = "T1_DEATHKNIGHT"; lootTableArmor = "T1_PLATE";   break;
			}

			lootTableTier = "T1_SATCHEL";
		}
		break;
		case SATCHEL_GREEN:
		{
			level = 2;
			currencyChance = 20;
			vpCount = 50;
			jpCount = 100;
			goldrange = urand(51, 100);
			nextItem = SATCHEL_BLUE;

			switch (player->getClass())
			{
			case CLASS_PRIEST:			lootTableClass = "T2_PRIEST";      lootTableArmor = "T2_CLOTH";   break;
			case CLASS_MAGE:			lootTableClass = "T2_MAGE";        lootTableArmor = "T2_CLOTH";   break;
			case CLASS_WARLOCK:			lootTableClass = "T2_WARLOCK";     lootTableArmor = "T2_CLOTH";   break;
			case CLASS_ROGUE:			lootTableClass = "T2_ROGUE";       lootTableArmor = "T2_LEATHER"; break;
			case CLASS_DRUID:			lootTableClass = "T2_DRUID";       lootTableArmor = "T2_LEATHER"; break;
			case CLASS_HUNTER:			lootTableClass = "T2_HUNTER";      lootTableArmor = "T2_MAIL";    break;
			case CLASS_SHAMAN:			lootTableClass = "T2_SHAMAN";	   lootTableArmor = "T2_MAIL";    break;
			case CLASS_WARRIOR:			lootTableClass = "T2_WARRIOR";	   lootTableArmor = "T2_PLATE";   break;
			case CLASS_PALADIN:			lootTableClass = "T2_PALADIN";     lootTableArmor = "T2_PLATE";   break;
			case CLASS_DEATH_KNIGHT:	lootTableClass = "T2_DEATHKNIGHT"; lootTableArmor = "T2_PLATE";   break;
			}

			lootTableTier = "T2_SATCHEL";
		}
		break;
		case SATCHEL_BLUE:
		{
			level = 3;
			currencyChance = 30;
			vpCount = 75;
			jpCount = 150;
			goldrange = urand(101, 200);
			nextItem = SATCHEL_EPIC;

			switch (player->getClass())
			{
			case CLASS_PRIEST:			lootTableClass = "T3_PRIEST";      lootTableArmor = "T3_CLOTH";   break;
			case CLASS_MAGE:			lootTableClass = "T3_MAGE";        lootTableArmor = "T3_CLOTH";   break;
			case CLASS_WARLOCK:			lootTableClass = "T3_WARLOCK";     lootTableArmor = "T3_CLOTH";   break;
			case CLASS_ROGUE:			lootTableClass = "T3_ROGUE";       lootTableArmor = "T3_LEATHER"; break;
			case CLASS_DRUID:			lootTableClass = "T3_DRUID";       lootTableArmor = "T3_LEATHER"; break;
			case CLASS_HUNTER:			lootTableClass = "T3_HUNTER";      lootTableArmor = "T3_MAIL";    break;
			case CLASS_SHAMAN:			lootTableClass = "T3_SHAMAN";	   lootTableArmor = "T3_MAIL";    break;
			case CLASS_WARRIOR:			lootTableClass = "T3_WARRIOR";	   lootTableArmor = "T3_PLATE";   break;
			case CLASS_PALADIN:			lootTableClass = "T3_PALADIN";     lootTableArmor = "T3_PLATE";   break;
			case CLASS_DEATH_KNIGHT:	lootTableClass = "T3_DEATHKNIGHT"; lootTableArmor = "T3_PLATE";   break;
			}

			lootTableTier = "T3_SATCHEL";
		}
		break;
		case SATCHEL_EPIC:
		{
			level = 4;
			currencyChance = 40;
			vpCount = 100;
			jpCount = 200;
			goldrange = urand(201, 300);
			nextItem = SATCHEL_LEGENDARY;

			switch (player->getClass())
			{
			case CLASS_PRIEST:			lootTableClass = "T4_PRIEST";      lootTableArmor = "T4_CLOTH";   break;
			case CLASS_MAGE:			lootTableClass = "T4_MAGE";        lootTableArmor = "T4_CLOTH";   break;
			case CLASS_WARLOCK:			lootTableClass = "T4_WARLOCK";     lootTableArmor = "T4_CLOTH";   break;
			case CLASS_ROGUE:			lootTableClass = "T4_ROGUE";       lootTableArmor = "T4_LEATHER"; break;
			case CLASS_DRUID:			lootTableClass = "T4_DRUID";       lootTableArmor = "T4_LEATHER"; break;
			case CLASS_HUNTER:			lootTableClass = "T4_HUNTER";      lootTableArmor = "T4_MAIL";    break;
			case CLASS_SHAMAN:			lootTableClass = "T4_SHAMAN";	   lootTableArmor = "T4_MAIL";    break;
			case CLASS_WARRIOR:			lootTableClass = "T4_WARRIOR";	   lootTableArmor = "T4_PLATE";   break;
			case CLASS_PALADIN:			lootTableClass = "T4_PALADIN";     lootTableArmor = "T4_PLATE";   break;
			case CLASS_DEATH_KNIGHT:	lootTableClass = "T4_DEATHKNIGHT"; lootTableArmor = "T4_PLATE";   break;
			}

			lootTableTier = "T4_SATCHEL";
		}
		break;
		case SATCHEL_LEGENDARY:
		{
			level = 5;
			currencyChance = 50;
			vpCount = 150;
			jpCount = 300;
			goldrange = urand(301, 400);
			nextItem = SATCHEL_ARTIFACT;

			switch (player->getClass())
			{
			case CLASS_PRIEST:			lootTableClass = "T5_PRIEST";      lootTableArmor   = "T5_CLOTH";   break;
			case CLASS_MAGE:			lootTableClass = "T5_MAGE";        lootTableArmor   = "T5_CLOTH";   break;
			case CLASS_WARLOCK:			lootTableClass = "T5_WARLOCK";     lootTableArmor   = "T5_CLOTH";   break;
			case CLASS_ROGUE:			lootTableClass = "T5_ROGUE";       lootTableArmor   = "T5_LEATHER"; break;
			case CLASS_DRUID:			lootTableClass = "T5_DRUID";       lootTableArmor   = "T5_LEATHER"; break;
			case CLASS_HUNTER:			lootTableClass = "T5_HUNTER";      lootTableArmor   = "T5_MAIL";    break;
			case CLASS_SHAMAN:			lootTableClass = "T5_SHAMAN";	   lootTableArmor   = "T5_MAIL";    break;
			case CLASS_WARRIOR:			lootTableClass = "T5_WARRIOR";	   lootTableArmor   = "T5_PLATE";   break;
			case CLASS_PALADIN:			lootTableClass = "T5_PALADIN";     lootTableArmor	= "T5_PLATE";   break;
			case CLASS_DEATH_KNIGHT:	lootTableClass = "T5_DEATHKNIGHT"; lootTableArmor	= "T5_PLATE";   break;
			}

			lootTableTier = "T5_SATCHEL";
		}
		break;
		case SATCHEL_ARTIFACT:
		{
			level = 6;
			currencyChance = 60;
			vpCount = 200;
			jpCount = 400;
			goldrange = urand(401, 1000);

			switch (player->getClass())
			{
			case CLASS_PRIEST:			lootTableClass = "T6_PRIEST";      lootTableArmor = "T6_CLOTH";   break;
			case CLASS_MAGE:			lootTableClass = "T6_MAGE";        lootTableArmor = "T6_CLOTH";   break;
			case CLASS_WARLOCK:			lootTableClass = "T6_WARLOCK";     lootTableArmor = "T6_CLOTH";   break;
			case CLASS_ROGUE:			lootTableClass = "T6_ROGUE";       lootTableArmor = "T6_LEATHER"; break;
			case CLASS_DRUID:			lootTableClass = "T6_DRUID";       lootTableArmor = "T6_LEATHER"; break;
			case CLASS_HUNTER:			lootTableClass = "T6_HUNTER";      lootTableArmor = "T6_MAIL";    break;
			case CLASS_SHAMAN:			lootTableClass = "T6_SHAMAN";	   lootTableArmor = "T6_MAIL";    break;
			case CLASS_WARRIOR:			lootTableClass = "T6_WARRIOR";	   lootTableArmor = "T6_PLATE";   break;
			case CLASS_PALADIN:			lootTableClass = "T6_PALADIN";     lootTableArmor = "T6_PLATE";   break;
			case CLASS_DEATH_KNIGHT:	lootTableClass = "T6_DEATHKNIGHT"; lootTableArmor = "T6_PLATE";   break;
			}

			lootTableTier = "T6_SATCHEL";
		}
		break;
		}

		std::vector<uint32> ItemEntries;

		ss << "SELECT `item` FROM `mythic_loot_template` WHERE `loot_table` IN ('" << lootTableTier << "','" << lootTableArmor << "','" << lootTableClass << "');";
		std::string query = ss.str();

		QueryResult result = WorldDatabase.PQuery(query.c_str());

		if (!result)
			return false;

		do
		{
			Field* fields = result->Fetch();
			uint32 item = fields[0].GetUInt32();

			ItemEntries.push_back(item);

		} while (result->NextRow());

		uint32 ItemSize = ItemEntries.size();
		uint32 randomizedItemID = rand() % ItemSize;

		uint32 mountSpell = 0;
		if (ItemSparseEntry const* findSpell = sItemSparseStore.LookupEntry(ItemEntries[randomizedItemID]))
			if (uint32 foundSpell = findSpell->SpellId[1])
				mountSpell = foundSpell;

		uint32 canGet = ItemSize;
		bool canLoot = true;

		for (canGet; player->HasItemCount(ItemEntries[randomizedItemID], 1, true) || player->GetVoidStorageItemID(ItemEntries[randomizedItemID]) || player->HasSpell(mountSpell); --canGet)
		{
			randomizedItemID = rand() % ItemSize;

			if (ItemSparseEntry const* findSpell = sItemSparseStore.LookupEntry(ItemEntries[randomizedItemID]))
				if (uint32 foundSpell = findSpell->SpellId[1])
					mountSpell = foundSpell;

			if (canGet == 0)
				break;
		}

		if (canGet == 0)
		{
			canLoot = false;

			if (item->GetEntry() != SATCHEL_ARTIFACT)
			{
				player->AddItem(nextItem, 1);
				player->AddSpellCooldown(90065, item->GetEntry(), time(NULL) + 5);
				player->DestroyItemCount(item->GetEntry(), 1, true);
				player->CastSpell(player, 25465, 1);
				ChatHandler(player->GetSession()).PSendSysMessage("Congratulations! You have been rewarded with upgraded loot!");
				return false;
			}
			else
			{
				uint16 endGoldRange = urand(1500, 2500);
				player->ModifyMoney(endGoldRange * 10000);
				ChatHandler(player->GetSession()).PSendSysMessage("You loot %u Gold from the tier 6 Mythic Rewards!", endGoldRange);
			}
		}

		if (canLoot)
		{
			player->AddItem(ItemEntries[randomizedItemID], 1);
			ChatHandler(player->GetSession()).PSendSysMessage("You've received rewards from the tier %u Mythic Rewards!", level);

			bool ChanceGold = (rand() % 100) <= currencyChance;

			if (ChanceGold)
			{
				player->ModifyMoney(goldrange * 10000);
				ChatHandler(player->GetSession()).PSendSysMessage("You loot %u Gold", goldrange);
			}
		}

		bool ChanceValor = (rand() % 100) <= currencyChance;

		if (ChanceValor && player->GetCurrencyOnWeek(CURRENCY_TYPE_VALOR_POINTS, true) < 1000)
		{
			uint32 currentWeekCap = player->GetCurrencyOnWeek(CURRENCY_TYPE_VALOR_POINTS, false);
			uint32 rewardVPcount = vpCount * CURRENCY_PRECISION;

			if ((currentWeekCap + rewardVPcount) > CURRENCY_WEEK_CAP_VALOR_POINTS)
				rewardVPcount = CURRENCY_WEEK_CAP_VALOR_POINTS - currentWeekCap;

			player->ModifyCurrency(CURRENCY_TYPE_VALOR_POINTS, rewardVPcount);
		}

		bool ChanceJustice = (rand() % 100) <= currencyChance;

		if (ChanceJustice && player->GetCurrency(CURRENCY_TYPE_JUSTICE_POINTS, true) < 4000)
		{
			uint32 currentJP = player->GetCurrency(CURRENCY_TYPE_JUSTICE_POINTS, false);
			uint32 rewardJPcount = jpCount * CURRENCY_PRECISION;

			if ((currentJP + rewardJPcount) > (4000 * CURRENCY_PRECISION))
				rewardJPcount = (4000 * CURRENCY_PRECISION) - currentJP;

			player->ModifyCurrency(CURRENCY_TYPE_JUSTICE_POINTS, rewardJPcount);
		}

		player->AddSpellCooldown(90065, item->GetEntry(), time(NULL) + 5);
		player->DestroyItemCount(item->GetEntry(), 1, true);
		return true;
	}
};

class spell_item_mythic_keystone : public SpellScriptLoader
{
public:
	spell_item_mythic_keystone() : SpellScriptLoader("spell_item_mythic_keystone") { }

	class spell_item_mythic_keystone_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_item_mythic_keystone_SpellScript);

		bool Load() override
		{
			return GetCaster()->GetTypeId() == TYPEID_PLAYER;
		}

		SpellCastResult CheckRequirement()
		{
			if (GetCaster()->isInCombat())
				return SPELL_FAILED_AFFECTING_COMBAT;

			if (auto c = GetCaster())
			if (auto caster = c->ToPlayer())
			{

				if (auto i = c->GetInstanceScript())
					if (i->IsChallengeModeInProgress())
					{
						i->SendChallengeModeHint(caster);
						return SPELL_CAST_OK;
					}

				Keystone const& keystone = caster->GetKeystone();
				MapEntry const* mapEntry = sMapStore.LookupEntry(keystone.mapId);

				if (!mapEntry->IsNonRaidDungeon())
				{
					caster->GetSession()->SendNotification("|cffff0000You don't have any Mythic Keystone data.\nFind Aggra at any dungeon entrance to start your first challenge.|r");
					return SPELL_FAILED_DONT_REPORT;
				}

				std::ostringstream ostream;

				ostream << "Your next Mythic Keystone:\nDifficulty level: +" << uint32(keystone.level) << "\nDungeon: " << mapEntry->name;

				if (keystone.affixes.front() != 0)
				{
					for (uint8 i = 0; i < MAX_KEYSTONE_AFFIXES; ++i)
					{
						auto affix_info = sWorld->GetAffixDetails(keystone.affixes[i]);

						if (keystone.affixes[i] != 0)
						{
							ostream << "\nAffix " << i + 1 << ": " << ChallengeModeMgr::GenerateAfficon(25, affix_info->affix_icon) << affix_info->affix_name;
						}
					}
				}

				std::string fullmsg = ostream.str();

				ChatHandler(caster->GetSession()).PSendSysMessage("%s", fullmsg.c_str());
			}

			return SPELL_CAST_OK;
		}

		void Register() override
		{
			OnCheckCast += SpellCheckCastFn(spell_item_mythic_keystone_SpellScript::CheckRequirement);
		}
	};

	SpellScript* GetSpellScript() const override
	{
		return new spell_item_mythic_keystone_SpellScript();
	}
};

class mythic_rew_dummy : public ItemScript
{
public:
	mythic_rew_dummy() : ItemScript("mythic_rew_dummy") { }

	bool OnUse(Player* player, Item* item, const SpellCastTargets& /*targets*/)
	{
		
		uint8 currentKeystone = player->GetKeystone().level;
		
		player->GetSession()->SendAreaTriggerMessage("Mythic Challenge Completed!");
		ChatHandler(player->GetSession()).PSendSysMessage("Mythic Challenge Completed!");

		uint8 ChanceWhite = 0;
		uint8 ChanceGreen = 0;
		uint8 ChanceBlue = 0;
		uint8 ChanceEpic = 0;
		uint8 ChanceLegendary = 0;
		uint8 ChanceArtifact = 0;

		switch (currentKeystone)
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

		if (player->GetKeystone().level >= 25)
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

		return true;
	}
};

void AddSC_mythic_rewards()
{
	new mythic_satchel();
	new spell_item_mythic_keystone();
	new mythic_rew_dummy();
}