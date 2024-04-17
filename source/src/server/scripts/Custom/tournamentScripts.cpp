#include "ScriptMgr.h"
#include "Player.h"
#include "Chat.h"
#include "Pet.h"

class npc_tourny_teleport : public CreatureScript
{
public:
	npc_tourny_teleport() : CreatureScript("npc_tourny_teleport") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		if (creature->GetDisplayId() == 29133)
			if (!creature->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN))
				creature->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);

		if (player->isInCombat())
		{
			player->GetSession()->SendAreaTriggerMessage("|cffff0000You are in combat!|r");
			return true;
		}
		if (sWorld->getBoolConfig(CONFIG_TOURNAMENT_ENABLE))
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Learn Riding", GOSSIP_SENDER_MAIN, 99);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Teleport to Vendors", GOSSIP_SENDER_MAIN, 1);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "Teleport to Gurubashi Arena", GOSSIP_SENDER_MAIN, 5);
		}

		if (player->GetAreaId() != 2037)//outside duel zone
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Teleport to Sanctuary Duel Zone", GOSSIP_SENDER_MAIN, 2);
		else//inside duel zone
		{
			if (player->GetTeam() == ALLIANCE)
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Teleport to Elwynn Forest Duel Zone", GOSSIP_SENDER_MAIN, 3);
			else
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Teleport to Durotar Duel Zone", GOSSIP_SENDER_MAIN, 4);
		}

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Exit", GOSSIP_SENDER_MAIN, 100);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();

		if (player->isInCombat())
		{
			player->GetSession()->SendAreaTriggerMessage("|cffff0000You are in combat!|r");
			player->PlayerTalkClass->SendCloseGossip();
		}

		switch (action)
		{
		case 1: // Vendors
			player->TeleportTo(0, 4285.60f, -2845.98f, 7.64f, 3.28f);
			break;
		case 2: // Sanctuary duel
			player->TeleportTo(0, 4297.0f, -2762.9f, 19.25f, 3.67f);
			break;
		case 3: // Elwynn
			player->TeleportTo(0, -9162.270508f, 466.839203f, 106.213104f,  5.5544f);
			break;
		case 4: // Durotar
			player->TeleportTo(1, 1289.414185f, -4338.683594f, 35.224243f,  0.1127f);
			break;
		case 5: // Gurubashi
			player->TeleportTo(0, -13242.7f, 197.6f, 32.8f, 1.126f);
			break;
		case 99:
			player->GetSession()->SendTrainerList(creature->GetGUID());
			break;
		case 100:
			player->CLOSE_GOSSIP_MENU();
			break;
		}
		return true;
	}
};

enum TournamentVendorIDs
{
	pvpMain		= 1,
	pvpOff		= 2,
	pvpJewl		= 3,
	pvpWep		= 4,
	pveMain1	= 5,
	pveMain2	= 6,
	pveCloth	= 7,
	pveLeather	= 8,
	pveMail		= 9,
	pvePlate	= 10,
	pveJewl		= 11,
	pveWep		= 12,
	redGem		= 13,
	blueGem		= 14,
	yellowGem	= 15,
	greenGem	= 16,
	orangeGem	= 17,
	purpleGem	= 18,
	metaGem		= 19
};

void SendTournamentVendor(Player* player, Creature* creature, uint8 id)
{
	if (!player || !creature || !id)
		return;

	uint32 entry = 600000;
	uint8 currSeason = 0;
	currSeason = sWorld->getIntConfig(CONFIG_TOURNAMENT_VENDOR);

	switch (id)
	{
	case 1: // pvpMain
	{
		if (currSeason == 11)
			entry = 600001;
		else if (currSeason == 10)
			entry = 600034;
		else if (currSeason == 9)
			entry = 600046;
	}
	break;
	case 2: // pvpOff
	{
		if (currSeason == 11)
			entry = 600002;
		else if (currSeason == 10)
			entry = 600035;
		else if (currSeason == 9)
			entry = 600047;
	}
	break;
	case 3: // pvpJewl
	{
		if (currSeason == 11)
			entry = 600003;
		else if (currSeason == 10)
			entry = 600036;
		else if (currSeason == 9)
			entry = 600048;
	}
	break;
	case 4: // pvpWep
	{
		if (currSeason == 11)
			entry = 600004;
		else if (currSeason == 10)
			entry = 600037;
		else if (currSeason == 9)
			entry = 600049;
	}
	break;
	case 5: // pveMain1
	{
		if (currSeason == 11)
			entry = 600005;
		else if (currSeason == 10)
			entry = 600038;
		else if (currSeason == 9)
			entry = 600050;
	}
	break;
	case 6: // pveMain2
	{
		if (currSeason == 11)
			entry = 600006;
		else if (currSeason == 10)
			entry = 600039;
	}
	break;
	case 7: // pveCloth
	{
		if (currSeason == 11)
			entry = 600007;
		else if (currSeason == 10)
			entry = 600040;
		else if (currSeason == 9)
			entry = 600051;
	}
	break;
	case 8: // pveLeather
	{
		if (currSeason == 11)
			entry = 600008;
		else if (currSeason == 10)
			entry = 600041;
		else if (currSeason == 9)
			entry = 600052;
	}
	break;
	case 9: // pveMail
	{
		if (currSeason == 11)
			entry = 600009;
		else if (currSeason == 10)
			entry = 600042;
		else if (currSeason == 9)
			entry = 600053;
	}
	break;
	case 10: // pvePlate
	{
		if (currSeason == 11)
			entry = 600010;
		else if (currSeason == 10)
			entry = 600043;
		else if (currSeason == 9)
			entry = 600054;
	}
	break;
	case 11: // pveJewl
	{
		if (currSeason == 11)
			entry = 600011;
		else if (currSeason == 10)
			entry = 600044;
		else if (currSeason == 9)
			entry = 600055;
	}
	break;
	case 12: // pveWep
	{
		if (currSeason == 11)
			entry = 600012;
		else if (currSeason == 10)
			entry = 600045;
		else if (currSeason == 9)
			entry = 600056;
	}
	break;
	case 13: // redGem
	{
		if (currSeason == 11)
			entry = 600013;
		else if (currSeason == 10 || currSeason == 9)
			entry = 600057;
	}
	break;
	case 14: // blueGem
	{
		if (currSeason == 11)
			entry = 600014;
		else if (currSeason == 10 || currSeason == 9)
			entry = 600058;
	}
	break;
	case 15: // yellowGem
	{
		if (currSeason == 11)
			entry = 600015;
		else if (currSeason == 10 || currSeason == 9)
			entry = 600059;
	}
	break;
	case 16: // greenGem
	{
		if (currSeason == 11)
			entry = 600016;
		else if (currSeason == 10 || currSeason == 9)
			entry = 600060;
	}
	break;
	case 17: // orangeGem
	{
		if (currSeason == 11)
			entry = 600017;
		else if (currSeason == 10 || currSeason == 9)
			entry = 600061;
	}
	break;
	case 18: // purpleGem
	{
		if (currSeason == 11)
			entry = 600018;
		else if (currSeason == 10 || currSeason == 9)
			entry = 600062;
	}
	break;
	case 19: // metaGem
	{
		if (currSeason == 11 || currSeason == 10 || currSeason == 9)
			entry = 600019;
	}
	break;
	default:
		break;
	}

	player->GetSession()->SendListInventory(creature->GetGUID(), entry);
}

class npc_tourny_vendor : public CreatureScript
{
public:
	npc_tourny_vendor() : CreatureScript("npc_tourny_vendor") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		if (creature->GetEntry() == 600000) // Gnomey
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Browse PvP Armor..", GOSSIP_SENDER_MAIN, 1);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Browse PvE Armor..", GOSSIP_SENDER_MAIN, 2);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Browse Gems..", GOSSIP_SENDER_MAIN, 3);
		}
		
		if (creature->GetEntry() == 599996) // Goblinjo
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "General Goods", GOSSIP_SENDER_MAIN, 4);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Consumables", GOSSIP_SENDER_MAIN, 5);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Character Services", GOSSIP_SENDER_MAIN, 6);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Mounts", GOSSIP_SENDER_MAIN, 7);
		}

		if (creature->GetEntry() == 599990) // Wufc pvp rewards
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Mounts", GOSSIP_SENDER_MAIN, 8);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Tabards", GOSSIP_SENDER_MAIN, 9);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Fun Items", GOSSIP_SENDER_MAIN, 14);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Mythic Transmog", GOSSIP_SENDER_MAIN, 15);
		}

		if (player->GetMoney() < 50000000 && player->getLevel() == 85)
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "I'm almost out of gold, give me more!", GOSSIP_SENDER_MAIN, 99);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();
		switch (action)
		{
		case 98:
		{
			player->PlayerTalkClass->ClearMenus();
			OnGossipHello(player, creature);
		}
		break;
		case 99:
		{
			player->PlayerTalkClass->SendCloseGossip();
			player->ModifyMoney(250000000, false);
		}
		break;
		case 1:
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "PvP Armor Set", GOSSIP_SENDER_MAIN, 10);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "PvP Non-Set", GOSSIP_SENDER_MAIN, 11);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "PvP Jewelry", GOSSIP_SENDER_MAIN, 12);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "PvP Weapons", GOSSIP_SENDER_MAIN, 13);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 98);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		}
		break;
		case 10:
			SendTournamentVendor(player, creature, pvpMain);
			break;
		case 11:
			SendTournamentVendor(player, creature, pvpOff);
			break;
		case 12:
			SendTournamentVendor(player, creature, pvpJewl);
			break;
		case 13:
			SendTournamentVendor(player, creature, pvpWep);
			break;
		case 2:
		{
			if (sWorld->getBoolConfig(CONFIG_TOURNAMENT_SET_2_ENABLE))
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "PvE Armor Sets", GOSSIP_SENDER_MAIN, 20);
			else
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "PvE Armor Set", GOSSIP_SENDER_MAIN, 200);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "PvE Non-set", GOSSIP_SENDER_MAIN, 21);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "PvE Jewelry", GOSSIP_SENDER_MAIN, 22);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "PvE Weapons", GOSSIP_SENDER_MAIN, 23);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 98);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		}
		break;
		case 20:
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "PvE Armor Set High", GOSSIP_SENDER_MAIN, 200);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "PvE Armor Set Low", GOSSIP_SENDER_MAIN, 201);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 2);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		}
		break;
		case 21:
		{
			switch (player->getClass())
			{
			case CLASS_PRIEST:
			case CLASS_MAGE:
			case CLASS_WARLOCK:
				SendTournamentVendor(player, creature, pveCloth);
				break;
			case CLASS_ROGUE:
			case CLASS_DRUID:
				SendTournamentVendor(player, creature, pveLeather);
				break;
			case CLASS_HUNTER:
			case CLASS_SHAMAN:
				SendTournamentVendor(player, creature, pveMail);
				break;
			case CLASS_WARRIOR:
			case CLASS_PALADIN:
			case CLASS_DEATH_KNIGHT:
				SendTournamentVendor(player, creature, pvePlate);
				break;
			}
		}
		break;
		case 22:
			SendTournamentVendor(player, creature, pveJewl);
			break;
		case 23:
			SendTournamentVendor(player, creature, pveWep);
			break;
		case 200:
			SendTournamentVendor(player, creature, pveMain1);
			break;
		case 201:
			SendTournamentVendor(player, creature, pveMain2);
			break;
		case 3:
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Red Gems", GOSSIP_SENDER_MAIN, 30);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Blue Gems", GOSSIP_SENDER_MAIN, 31);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Yellow Gems", GOSSIP_SENDER_MAIN, 32);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Green Gems", GOSSIP_SENDER_MAIN, 33);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Orange Gems", GOSSIP_SENDER_MAIN, 34);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Purple Gems", GOSSIP_SENDER_MAIN, 35);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Meta Gems", GOSSIP_SENDER_MAIN, 36);
			if (player->HasSkill(SKILL_ENGINEERING) && player->GetSkillValue(SKILL_ENGINEERING) > 450)
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "|TInterface\\icons\\trade_engineering:20|t|r [Engineering]: Cogwheel Sockets", GOSSIP_SENDER_MAIN, 366);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 98);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		}
		break;
		case 30:
			SendTournamentVendor(player, creature, redGem);
			break;
		case 31:
			SendTournamentVendor(player, creature, blueGem);
			break;
		case 32:
			SendTournamentVendor(player, creature, yellowGem);
			break;
		case 33:
			SendTournamentVendor(player, creature, greenGem);
			break;
		case 34:
			SendTournamentVendor(player, creature, orangeGem);
			break;
		case 35:
			SendTournamentVendor(player, creature, purpleGem);
			break;
		case 36:
			SendTournamentVendor(player, creature, metaGem);
			break;
		case 366:
			player->GetSession()->SendListInventory(creature->GetGUID(), 600063);
			break;
		case 4:
			player->GetSession()->SendListInventory(creature->GetGUID(), 600030);
			break;
		case 5:
			player->GetSession()->SendListInventory(creature->GetGUID(), 600031);
			break;
		case 6:
			player->GetSession()->SendListInventory(creature->GetGUID(), 600032);
			break;
		case 7:
			player->GetSession()->SendListInventory(creature->GetGUID(), 600033);
			break;
		case 8:
			player->GetSession()->SendListInventory(creature->GetGUID(), 600064);
			break;
		case 9:
			player->GetSession()->SendListInventory(creature->GetGUID(), 600066);
			break;
		case 14:
			player->GetSession()->SendListInventory(creature->GetGUID(), 600065);
			break;
		case 15:
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "One-handed Weapon", GOSSIP_SENDER_MAIN, 150);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Two-handed Weapon", GOSSIP_SENDER_MAIN, 151);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Staff and Polearm", GOSSIP_SENDER_MAIN, 152);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Ranged and Off-Hand", GOSSIP_SENDER_MAIN, 153);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Head Transmogrification", GOSSIP_SENDER_MAIN, 154);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 98);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		}
		break;
		case 150:
			player->GetSession()->SendListInventory(creature->GetGUID(), 600067);
			break;
		case 151:
			player->GetSession()->SendListInventory(creature->GetGUID(), 600068);
			break;
		case 152:
			player->GetSession()->SendListInventory(creature->GetGUID(), 600069);
			break;
		case 153:
			player->GetSession()->SendListInventory(creature->GetGUID(), 600070);
			break;
		case 154:
			player->GetSession()->SendListInventory(creature->GetGUID(), 600071);
			break;
		}
		return true;
	}
};

class npc_tourny_i_request : public CreatureScript
{
public:
	npc_tourny_i_request() : CreatureScript("npc_tourny_i_request") {}

	bool OnGossipHello(Player* player, Creature* creature)
	{
		player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, "Request Item ID", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1, "", 0, true);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "What's this?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
		player->PlayerTalkClass->SendGossipMenu(50002, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();
		if (action == GOSSIP_ACTION_INFO_DEF + 2)
		{
			creature->MonsterWhisper("You can request transmogrification items by entering the ID of the item you would like to have.", player->GetGUID(), false);
			creature->MonsterWhisper("One way of finding item ID's could be Wowhead.com (example: wowhead.com/item=32235)", player->GetGUID(), false);
			player->PlayerTalkClass->SendCloseGossip();
		}

		return true;
	}

	bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, char const* code)
	{
		player->PlayerTalkClass->ClearMenus();
		if (sender == GOSSIP_SENDER_MAIN)
		{
			switch (action)
			{
			case GOSSIP_ACTION_INFO_DEF + 1:
			{
				if (strlen(code) > 5)
				{
					ChatHandler(player->GetSession()).PSendSysMessage("Invalid ItemId");
					player->PlayerTalkClass->SendCloseGossip();
					return false;
				}

				uint32 itemId = atoi(code);

				if (!itemId)
				{
					ChatHandler(player->GetSession()).PSendSysMessage("Invalid ItemId");
					player->PlayerTalkClass->SendCloseGossip();
					return false;
				}

				if (itemId > 78000)
				{
					ChatHandler(player->GetSession()).PSendSysMessage("ItemId too high");
					player->PlayerTalkClass->SendCloseGossip();
					return false;
				}

				ItemTemplate const* item_proto = sObjectMgr->GetItemTemplate(itemId);

				if (!item_proto)
				{
					ChatHandler(player->GetSession()).PSendSysMessage("Item does not exist");
					player->PlayerTalkClass->SendCloseGossip();
					return false;
				}

				switch (item_proto->InventoryType)
				{
				case INVTYPE_NON_EQUIP:
				case INVTYPE_NECK:
				case INVTYPE_BODY:
				case INVTYPE_FINGER:
				case INVTYPE_TRINKET:
				case INVTYPE_BAG:
				case INVTYPE_TABARD:
				case INVTYPE_AMMO:
				case INVTYPE_QUIVER:
				case INVTYPE_RELIC:
					ChatHandler(player->GetSession()).PSendSysMessage("Item is on the exclusion list");
					player->PlayerTalkClass->SendCloseGossip();
					return false;
				}

				if (item_proto->ItemLevel > 277)
				{
					ChatHandler(player->GetSession()).PSendSysMessage("Item level too high");
					player->PlayerTalkClass->SendCloseGossip();
					return false;
				}

				if (item_proto->ItemLevel == 1)
				{
					ChatHandler(player->GetSession()).PSendSysMessage("Item level too low");
					player->PlayerTalkClass->SendCloseGossip();
					return false;

				}

				if (item_proto->Quality > 4 || item_proto->Quality < 2)
				{
					ChatHandler(player->GetSession()).PSendSysMessage("Can only request Common, Rare and Epic items");
					player->PlayerTalkClass->SendCloseGossip();
					return false;
				}
				
				if (item_proto->Flags2 & ITEM_FLAGS_EXTRA_HORDE_ONLY && player->GetTeam() == ALLIANCE)
				{
					ChatHandler(player->GetSession()).PSendSysMessage("This item is not for your faction");
					player->PlayerTalkClass->SendCloseGossip();
					return false;
				}

				if (item_proto->Flags2 & ITEM_FLAGS_EXTRA_ALLIANCE_ONLY && player->GetTeam() == HORDE)
				{
					ChatHandler(player->GetSession()).PSendSysMessage("This item is not for your faction");
					player->PlayerTalkClass->SendCloseGossip();
					return false;
				}

				// GM OUTFIT and TEST items
				if (item_proto->ItemId == 2586 || item_proto->ItemId == 11508 || item_proto->ItemId == 12064
					|| item_proto->ItemId == 26173 || item_proto->ItemId == 26174 || item_proto->ItemId == 26175
					|| item_proto->ItemId == 26180 || item_proto->ItemId == 26235 || item_proto->ItemId == 26324
					|| item_proto->ItemId == 26368 || item_proto->ItemId == 26372 || item_proto->ItemId == 26464
					|| item_proto->ItemId == 26465 || item_proto->ItemId == 26548 || item_proto->ItemId == 26655
					|| item_proto->ItemId == 26738 || item_proto->ItemId == 26792 || item_proto->ItemId == 26843
					|| item_proto->ItemId == 27196 || item_proto->ItemId == 38469 || item_proto->ItemId == 38471
					|| item_proto->ItemId == 38480 || item_proto->ItemId == 38481 || item_proto->ItemId == 45172
					|| item_proto->ItemId == 45174 || item_proto->ItemId == 45175 || item_proto->ItemId == 45575
					|| item_proto->ItemId == 54859 || item_proto->ItemId == 72069 || item_proto->ItemId == 24561
					|| item_proto->ItemId == 24567 || item_proto->ItemId == 27965 || item_proto->ItemId == 29828
					|| item_proto->ItemId == 34219 || item_proto->ItemId == 40307 || item_proto->ItemId == 40480
					|| item_proto->ItemId == 40481 || item_proto->ItemId == 40650 || item_proto->ItemId == 40762
					|| item_proto->ItemId == 40354 || item_proto->ItemId == 42755 || item_proto->ItemId == 43267
					|| item_proto->ItemId == 49706 || item_proto->ItemId == 50815 || item_proto->ItemId == 50840
					|| item_proto->ItemId == 53889 || item_proto->ItemId == 53890 || item_proto->ItemId == 53891
					|| item_proto->ItemId == 53924 || item_proto->ItemId == 61290 || item_proto->ItemId == 61291
					|| item_proto->ItemId == 61392 || item_proto->ItemId == 62032 || item_proto->ItemId == 63052
					|| item_proto->ItemId == 63270 || item_proto->ItemId == 67493 || item_proto->ItemId == 76175
					|| item_proto->ItemId == 76176)
				{
					ChatHandler(player->GetSession()).PSendSysMessage("Item is on the exclusion list");
					player->PlayerTalkClass->SendCloseGossip();
					return false;
				}

				player->AddItem(itemId, 1);
				player->PlayerTalkClass->SendCloseGossip();
				return true;
			}
			}
		}
		return false;
	}
};

class npc_tourny_trainer : public CreatureScript
{
public:
	npc_tourny_trainer() : CreatureScript("npc_tourny_trainer") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		if (creature->GetEntry() == 26332 && player->getClass() != CLASS_WARRIOR)
		{
			player->GetSession()->SendAreaTriggerMessage("|cffff0000You're not a Warrior.|r");
			return true;
		}

		if (creature->GetEntry() == 26327 && player->getClass() != CLASS_PALADIN)
		{
			player->GetSession()->SendAreaTriggerMessage("|cffff0000You're not a Paladin.|r");
			return true;
		}

		if (creature->GetEntry() == 26325 && player->getClass() != CLASS_HUNTER)
		{
			player->GetSession()->SendAreaTriggerMessage("|cffff0000You're not a Hunter.|r");
			return true;
		}

		if (creature->GetEntry() == 26329 && player->getClass() != CLASS_ROGUE)
		{
			player->GetSession()->SendAreaTriggerMessage("|cffff0000You're not a Rogue.|r");
			return true;
		}

		if (creature->GetEntry() == 26328 && player->getClass() != CLASS_PRIEST)
		{
			player->GetSession()->SendAreaTriggerMessage("|cffff0000You're not a Priest.|r");
			return true;
		}

		if (creature->GetEntry() == 33251 && player->getClass() != CLASS_DEATH_KNIGHT)
		{
			player->GetSession()->SendAreaTriggerMessage("|cffff0000You're not a Death Knight.|r");
			return true;
		}

		if (creature->GetEntry() == 26330 && player->getClass() != CLASS_SHAMAN)
		{
			player->GetSession()->SendAreaTriggerMessage("|cffff0000You're not a Shaman.|r");
			return true;
		}

		if (creature->GetEntry() == 26326 && player->getClass() != CLASS_MAGE)
		{
			player->GetSession()->SendAreaTriggerMessage("|cffff0000You're not a Mage.|r");
			return true;
		}

		if (creature->GetEntry() == 26331 && player->getClass() != CLASS_WARLOCK)
		{
			player->GetSession()->SendAreaTriggerMessage("|cffff0000You're not a Warlock.|r");
			return true;
		}

		if (creature->GetEntry() == 26324 && player->getClass() != CLASS_DRUID)
		{
			player->GetSession()->SendAreaTriggerMessage("|cffff0000You're not a Druid.|r");
			return true;
		}
		
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Learn Class Spells", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Reset Talent Points", GOSSIP_SENDER_MAIN, 2);
		if (player->GetSpecsCount() == 1 && creature->isCanTrainingAndResetTalentsOf(player) && player->getLevel() >= sWorld->getIntConfig(CONFIG_MIN_DUALSPEC_LEVEL))
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Learn Dual Talent Specialization", GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "Purchase Glyphs", GOSSIP_SENDER_MAIN, 4);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();
		switch (action)
		{
		case 1:
			player->GetSession()->SendTrainerList(creature->GetGUID());
			break;
		case 2:
			player->CLOSE_GOSSIP_MENU();
			player->SendTalentWipeConfirm(creature->GetGUID());
			break;
		case 3:
			if (player->GetSpecsCount() == 1 && !(player->getLevel() < sWorld->getIntConfig(CONFIG_MIN_DUALSPEC_LEVEL)))
			{
				player->CastSpell(player, 63680, true, NULL, NULL, player->GetGUID());
				player->CastSpell(player, 63624, true, NULL, NULL, player->GetGUID());
				player->PlayerTalkClass->SendCloseGossip();
			}
			break;
		case 4:
			switch (player->getClass())
			{
			case CLASS_WARRIOR:
				player->GetSession()->SendListInventory(creature->GetGUID(), 600020);
				break;
			case CLASS_PALADIN:
				player->GetSession()->SendListInventory(creature->GetGUID(), 600021);
				break;
			case CLASS_HUNTER:
				player->GetSession()->SendListInventory(creature->GetGUID(), 600022);
				break;
			case CLASS_ROGUE:
				player->GetSession()->SendListInventory(creature->GetGUID(), 600023);
				break;
			case CLASS_PRIEST:
				player->GetSession()->SendListInventory(creature->GetGUID(), 600024);
				break;
			case CLASS_DEATH_KNIGHT:
				player->GetSession()->SendListInventory(creature->GetGUID(), 600025);
				break;
			case CLASS_SHAMAN:
				player->GetSession()->SendListInventory(creature->GetGUID(), 600026);
				break;
			case CLASS_MAGE:
				player->GetSession()->SendListInventory(creature->GetGUID(), 600027);
				break;
			case CLASS_WARLOCK:
				player->GetSession()->SendListInventory(creature->GetGUID(), 600028);
				break;
			case CLASS_DRUID:
				player->GetSession()->SendListInventory(creature->GetGUID(), 600029);
				break;
			}
		break;
		}
		return true;
	}
};

class npc_tourny_beastmaster : public CreatureScript
{
public:
	npc_tourny_beastmaster() : CreatureScript("npc_tourny_beastmaster") { }

	enum GossipOptions
	{
		GOSSIP_HELLO,
		GOSSIP_MAIN,

		GOSSIP_NORMAL_PETS,
		GOSSIP_EXOTIC_PETS,
		GOSSIP_STABLE_PETS,
		GOSSIP_CUSTOM_PETS,

		GOSSIP_CREATE_PET
	};

	bool OnGossipHello(Player* player, Creature* creature)
	{
		if (player->getClass() != CLASS_HUNTER)
		{
			player->GetSession()->SendAreaTriggerMessage("|cffff0000You're not a Hunter.|r");
			return true;
		}

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "New Normal Pet", GOSSIP_NORMAL_PETS, 0);
		player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, "Tame a Custom Pet (Insert Npc entry)", GOSSIP_SENDER_MAIN, 1, "", 0, true);
		if (player->CanTameExoticPets())
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "New Exotic Pet", GOSSIP_EXOTIC_PETS, 0);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Stable your pet", GOSSIP_STABLE_PETS, 0);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();

		switch (sender)
		{
		case GOSSIP_HELLO:
			OnGossipHello(player, creature);
			return true;
		case GOSSIP_NORMAL_PETS:
			ShowNormalPets(player, creature, action);
			break;
		case GOSSIP_EXOTIC_PETS:
			ShowExoticPets(player, creature, action);
			break;
		case GOSSIP_STABLE_PETS:
			player->GetSession()->SendStablePet(creature->GetGUID());
			break;
		case GOSSIP_CREATE_PET:
			CreatePet(player, creature, action);
			break;
		default:
			player->GetSession()->SendAreaTriggerMessage("Something went wrong. Error code: %u.%u.", sender, action);
		}
		return true;
	}

	bool OnGossipSelectCode(Player* player, Creature* creature, uint32 /*sender*/, uint32 action, const char* code)
	{
		player->PlayerTalkClass->ClearMenus();
		uint32 creatureID = atoi((char*)code);

		if (!creatureID)
		{
			ShowErrorScreen(player, creature, GOSSIP_HELLO, 0, "Please enter a proper entry ID.");
			return false;
		}

		const CreatureTemplate* beast = sObjectMgr->GetCreatureTemplate(creatureID);
		if (action == 1)
		{
			if (beast)
			{
				uint32 type = beast->type;
				uint32 type_flags = beast->type_flags;

				if (type == CREATURE_TYPE_BEAST && type_flags == CREATURE_TYPEFLAGS_TAMEABLE) //creature is beast AND tameable
				{
					CreatePet(player, creature, creatureID);
				}
				else
				{
					ShowErrorScreen(player, creature, GOSSIP_HELLO, 0, "This creature is not tameable.");
				}
			}
			else
			{
				ShowErrorScreen(player, creature, GOSSIP_HELLO, 0, "This creature entry does not exist!");
			}
		}
		return true;
	}

	void ShowNormalPets(Player* player, Creature* creature, uint32 page)
	{
		switch (page)
		{
		case 0:
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\ability_hunter_pet_bat:20:20:-10:0|tBat", GOSSIP_CREATE_PET, 28233);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\ability_hunter_pet_bear:20:20:-10:0|tBear", GOSSIP_CREATE_PET, 29319);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\ability_hunter_pet_boar:20:20:-10:0|tBoar", GOSSIP_CREATE_PET, 29996);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\ability_hunter_pet_cat:20:20:-10:0|tCat", GOSSIP_CREATE_PET, 28097);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\ability_hunter_pet_vulture:20:20:-10:0|tCarrion Bird", GOSSIP_CREATE_PET, 51760);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\ability_hunter_pet_crab:20:20:-10:0|tCrab", GOSSIP_CREATE_PET, 39418);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\ability_hunter_pet_crocolisk:20:20:-10:0|tCrocolisk", GOSSIP_CREATE_PET, 1417);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\ability_hunter_pet_dragonhawk:20:20:-10:0|tDragonhawk", GOSSIP_CREATE_PET, 42710);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\ability_hunter_pet_gorilla:20:20:-10:0|tGorilla", GOSSIP_CREATE_PET, 1108);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\ability_hunter_pet_wolf:20:20:-10:0|tHound", GOSSIP_CREATE_PET, 29452);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\ability_hunter_pet_hyena:20:20:-10:0|tHyena", GOSSIP_CREATE_PET, 13036);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\ability_hunter_pet_moth:20:20:-10:0|tMoth", GOSSIP_CREATE_PET, 27421);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\ability_hunter_aspectofthemonkey:20:20:-10:0|tMonkey", GOSSIP_CREATE_PET, 43050);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\ability_hunter_pet_owl:20:20:-10:0|tOwl", GOSSIP_CREATE_PET, 46162);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Next Page ->", GOSSIP_NORMAL_PETS, page + 1);
			break;
		case 1:
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\ability_hunter_pet_raptor:20:20:-10:0|tRaptor", GOSSIP_CREATE_PET, 42721);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\ability_hunter_pet_ravager:20:20:-10:0|tRavager", GOSSIP_CREATE_PET, 17199);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\ability_hunter_pet_tallstrider:20:20:-10:0|tTallstrider", GOSSIP_CREATE_PET, 22807);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\ability_hunter_pet_scorpid:20:20:-10:0|tScorpid", GOSSIP_CREATE_PET, 9698);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\ability_hunter_pet_spider:20:20:-10:0|tSpider", GOSSIP_CREATE_PET, 30);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\spell_nature_guardianward:20:20:-10:0|tSerpent", GOSSIP_CREATE_PET, 28358);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\ability_hunter_pet_turtle:20:20:-10:0|tTurtle", GOSSIP_CREATE_PET, 25482);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\ability_hunter_pet_wasp:20:20:-10:0|tWasp", GOSSIP_CREATE_PET, 28085);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\icons\\ability_hunter_pet_wolf:20:20:-10:0|tWolf", GOSSIP_CREATE_PET, 2680);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "<- Previous Page", GOSSIP_NORMAL_PETS, page - 1);
			break;
		default:
			player->GetSession()->SendAreaTriggerMessage("Something went wrong. sender: Error code: %u.%u.", GOSSIP_NORMAL_PETS, page);
			break;
		}
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_HELLO, 0);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
	}

	void ShowExoticPets(Player* player, Creature* creature, uint32 page)
	{
		if (!player->CanTameExoticPets())
		{
			ShowErrorScreen(player, creature, GOSSIP_HELLO, 0, "Only Beast Mastery Hunters tame exotic pets.");
			return;
		}
		
		switch (page)
		{
		case 0:
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Arcturis", GOSSIP_CREATE_PET, 38453);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Chimaera", GOSSIP_CREATE_PET, 21879);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Core Hound", GOSSIP_CREATE_PET, 21108);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Devilsaur", GOSSIP_CREATE_PET, 20931);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Rhino", GOSSIP_CREATE_PET, 30445);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Silithid", GOSSIP_CREATE_PET, 5460);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Worm", GOSSIP_CREATE_PET, 30148);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Loque'nahak", GOSSIP_CREATE_PET, 32517);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Skoll", GOSSIP_CREATE_PET, 35189);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Gondria", GOSSIP_CREATE_PET, 33776);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Ban'thalos", GOSSIP_CREATE_PET, 54320);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Ghostcrawler", GOSSIP_CREATE_PET, 50051);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Core Hound", GOSSIP_CREATE_PET, 11671);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Shale Spider", GOSSIP_CREATE_PET, 44425);
			break;
		default:
			player->GetSession()->SendAreaTriggerMessage("Something went wrong. Error code: %u.%u.", GOSSIP_EXOTIC_PETS, page);
			break;
		}
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_HELLO, 0);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
	}

	void CreatePet(Player* player, Creature* creature, uint32 entry) {
		//Check if the player is a hunter
		if (player->getClass() != CLASS_HUNTER)
		{
			ShowErrorScreen(player, creature, GOSSIP_HELLO, 0, "Only Hunters can tame pets.");
			return;
		}

		//Checking if the player has a pet curretly
		if (player->GetPet())
		{
			player->GetSession()->SendNotification("You must dismiss your current pet first.");
			player->PlayerTalkClass->SendCloseGossip();
			return;
		}

		// If we have a full list we shoulden't be able to create a new one.
		PetSlot newslot = player->ToPlayer()->getSlotForNewPet();
		if (newslot == PET_SLOT_FULL_LIST)
		{
			ChatHandler(player->GetSession()).PSendSysMessage("You have too many pets.");
			return;
		}

		Creature* creatureTarget = creature->SummonCreature(entry, player->GetPositionX(), player->GetPositionY() + 2, player->GetPositionZ(), player->GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 500);

		Pet* pet = player->CreateTamedPetFrom(creatureTarget, 0);

		/*Kill the creautre and remove the corpse*/
		creatureTarget->setDeathState(JUST_DIED);
		creatureTarget->RemoveCorpse();
		creatureTarget->SetHealth(0);                       // just for nice GM-mode view

		//Pet level on tame
		uint8 level = 85; //(creatureTarget->getLevel() < (m_caster->getLevel() - 5)) ? (m_caster->getLevel() - 5) : creatureTarget->getLevel();

		// prepare visual effect for levelup
		pet->SetUInt32Value(UNIT_FIELD_LEVEL, level - 1);

		// add to world
		pet->GetMap()->AddToMap(pet->ToCreature());

		// visual effect for levelup
		pet->SetUInt32Value(UNIT_FIELD_LEVEL, level);

		// caster have pet now
		player->SetMinion(pet, true, newslot);

		pet->InitTalentForLevel();

		if (player->GetTypeId() == TYPEID_PLAYER)
		{
			pet->SavePet(newslot);
			player->ToPlayer()->PetSpellInitialize();
			player->ToPlayer()->GetSession()->SendStablePet(0);
		}

		// Inform user pet was created
		ShowErrorScreen(player, creature, GOSSIP_HELLO, 0, "Pet created successfully!");
	}

	void ShowErrorScreen(Player* player, Creature* creature, uint32 uiSender, uint32 uiAction, std::string error)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, error, uiSender, uiAction);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", uiSender, uiAction);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
	}
};

// not used atm
enum ProfMenus
{
	MAIN_MENU = 0,
	ALCHEMY = 1,
	BLACKSMITHING = 2,
	LEATHERWROKING = 3,
	TAILORING = 4,
	ENGINEERING = 5,
	ENCHANTING = 6,
	JEWELCRAFTING = 7,
	INSCRIPTION = 8,
	FIRST_AID = 9
};

bool PlayerAlreadyHasTwoProfessions(const Player* player) /*const*/
{
	uint32 skillCount = 0;

	if (player->HasSkill(SKILL_MINING))
		skillCount++;
	if (player->HasSkill(SKILL_SKINNING))
		skillCount++;
	if (player->HasSkill(SKILL_HERBALISM))
		skillCount++;

	if (skillCount >= 2)
		return true;

	for (uint32 i = 1; i < sSkillLineStore.GetNumRows(); ++i)
	{
		SkillLineEntry const* SkillInfo = sSkillLineStore.LookupEntry(i);
		if (!SkillInfo)
			continue;

		if (SkillInfo->categoryId == SKILL_CATEGORY_SECONDARY)
			continue;

		if ((SkillInfo->categoryId != SKILL_CATEGORY_PROFESSION) || !SkillInfo->canLink)
			continue;

		const uint32 skillID = SkillInfo->id;
		if (player->HasSkill(skillID))
		{
			//TC_LOG_ERROR("sql.sql", "SKILL %u", i);
			skillCount++;
		}

		if (skillCount >= 2)
			return true;
	}
	return false;
}


void LearnSkillRecipesHelper(Player* player, uint32 skill_id)
{
	uint32 classmask = player->getClassMask();

	for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
	{
		SkillLineAbilityEntry const* skillLine = sSkillLineAbilityStore.LookupEntry(j);
		if (!skillLine)
			continue;

		// wrong skill
		if (skillLine->skillId != skill_id)
			continue;

		// not high rank
		if (skillLine->forward_spellid)
			continue;

		// skip racial skills
		if (skillLine->racemask != 0)
			continue;

		// skip wrong class skills
		if (skillLine->classmask && (skillLine->classmask & classmask) == 0)
			continue;

		if (skillLine->spellId == 20219 
			|| skillLine->spellId == 20222
			|| skillLine->spellId == 82207
			|| skillLine->spellId == 28677	//elixir master
			|| skillLine->spellId == 28675	//potion master
			|| skillLine->spellId == 105518)
			continue;

		SpellEntry const* spellInfo = sSpellStore.LookupEntry(skillLine->spellId);
		player->learnSpell(skillLine->spellId, false);
	}
}

bool LearnAllRecipesInProfession(Player* player, SkillType skill)
{
	ChatHandler handler(player->GetSession());

	SkillLineEntry const* SkillInfo = sSkillLineStore.LookupEntry(skill);

	std::string skill_name = SkillInfo->name;
	if (!SkillInfo)
		return false;

	LearnSkillRecipesHelper(player, SkillInfo->id);
	player->SetSkill(SkillInfo->id, player->GetSkillStep(SkillInfo->id), 525, 525);
	if (skill_name.empty())
		return false;

	handler.PSendSysMessage(LANG_COMMAND_LEARN_ALL_RECIPES, skill_name.c_str());
	return true;
}

bool IsSecondarySkill(SkillType skill) /* const */
{
	return skill == SKILL_COOKING || skill == SKILL_FIRST_AID || skill == SKILL_FISHING || skill == SKILL_ARCHAEOLOGY;
}

void CompleteLearnProfession(Player* player, SkillType skill)
{
	if (PlayerAlreadyHasTwoProfessions(player) && !IsSecondarySkill(skill))
		player->GetSession()->SendNotification("You already know two primary professions!");
	else
	{
		if (!LearnAllRecipesInProfession(player, skill))
			player->GetSession()->SendNotification("Error #1: Internal code failiure");
	}
}

class npc_tourny_professions : public CreatureScript
{
public: npc_tourny_professions() : CreatureScript("npc_tourny_professions") {}

	  bool OnGossipHello(Player* player, Creature* creature)
	  {
		  // Primary
		  if (!player->HasSkill(SKILL_ALCHEMY))
			  player->ADD_GOSSIP_ITEM(-1, "|TInterface/ICONS/Trade_Alchemy:20:20:-10:0|tAlchemy|r", GOSSIP_SENDER_MAIN, 1);
		  if (!player->HasSkill(SKILL_BLACKSMITHING))
			  player->ADD_GOSSIP_ITEM(-1, "|TInterface/ICONS/Trade_BlackSmithing:20:20:-10:0|tBlacksmithing|r", GOSSIP_SENDER_MAIN, 2);
		  if (!player->HasSkill(SKILL_LEATHERWORKING))
			  player->ADD_GOSSIP_ITEM(-1, "|TInterface/ICONS/INV_Misc_ArmorKit_17:20:20:-10:0|tLeatherworking|r", GOSSIP_SENDER_MAIN, 3);
		  if (!player->HasSkill(SKILL_TAILORING))
			  player->ADD_GOSSIP_ITEM(-1, "|TInterface/ICONS/Trade_Tailoring:20:20:-10:0|tTailoring|r", GOSSIP_SENDER_MAIN, 4);
		  if (!player->HasSkill(SKILL_ENGINEERING))
			  player->ADD_GOSSIP_ITEM(-1, "|TInterface/ICONS/Trade_Engineering:20:20:-10:0|tEngineering|r", GOSSIP_SENDER_MAIN, 5);
		  if (!player->HasSkill(SKILL_ENCHANTING))
			  player->ADD_GOSSIP_ITEM(-1, "|TInterface/ICONS/Trade_Engraving:20:20:-10:0|tEnchanting|r", GOSSIP_SENDER_MAIN, 6);
		  if (!player->HasSkill(SKILL_JEWELCRAFTING))
			  player->ADD_GOSSIP_ITEM(-1, "|TInterface/ICONS/INV_Misc_Gem_01:20:20:-10:0|tJewelcrafting|r", GOSSIP_SENDER_MAIN, 7);
		  if (!player->HasSkill(SKILL_INSCRIPTION))
			  player->ADD_GOSSIP_ITEM(-1, "|TInterface/ICONS/INV_Inscription_Tradeskill01:20:20:-10:0|tInscription|r", GOSSIP_SENDER_MAIN, 8);
		  if (!player->HasSkill(SKILL_HERBALISM))
			  player->ADD_GOSSIP_ITEM(-1, "|TInterface/ICONS/Spell_Nature_NatureTouchGrow:20:20:-10:0|tHerbalism|r", GOSSIP_SENDER_MAIN, 19);
		  if (!player->HasSkill(SKILL_MINING))
			  player->ADD_GOSSIP_ITEM(-1, "|TInterface/ICONS/Trade_Mining:20:20:-10:0|tMining|r", GOSSIP_SENDER_MAIN, 20);
		  if (!player->HasSkill(SKILL_SKINNING))
			  player->ADD_GOSSIP_ITEM(-1, "|TInterface/ICONS/INV_Misc_Pelt_Wolf_01:20:20:-10:0|tSkinning|r", GOSSIP_SENDER_MAIN, 21);
		  // Second
		  if (!player->HasSkill(SKILL_COOKING))
			  player->ADD_GOSSIP_ITEM(-1, "|TInterface/ICONS/inv_misc_food_15:20:20:-10:0|tCooking|r", GOSSIP_SENDER_MAIN, 22);
		  if (!player->HasSkill(SKILL_FISHING))
			  player->ADD_GOSSIP_ITEM(-1, "|TInterface/ICONS/trade_fishing:20:20:-10:0|tFishing|r", GOSSIP_SENDER_MAIN, 23);
		  if (!player->HasSkill(SKILL_FIRST_AID) || player->getClass() == CLASS_DEATH_KNIGHT && player->HasSkill(SKILL_FIRST_AID) && player->GetSkillValue(SKILL_FIRST_AID) < 500)
			  player->ADD_GOSSIP_ITEM(-1, "|TInterface/ICONS/spell_holy_sealofsacrifice:20:20:-10:0|tFirst Aid|r", GOSSIP_SENDER_MAIN, 24);
		  player->PlayerTalkClass->SendGossipMenu(907, creature->GetGUID());
		  return true;
	  }

	  bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 uiAction)
	  {
		  player->PlayerTalkClass->ClearMenus();

		  if (uiSender == GOSSIP_SENDER_MAIN)
		  {
			  switch (uiAction)
			  {
			  case 0:
				  OnGossipHello(player, creature);
				  break;

			  case 1: CompleteLearnProfession(player, SKILL_ALCHEMY); break;
			  case 2: CompleteLearnProfession(player, SKILL_BLACKSMITHING); break;
			  case 3:	CompleteLearnProfession(player, SKILL_LEATHERWORKING); break;
			  case 4: CompleteLearnProfession(player, SKILL_TAILORING); break;
			  case 5: CompleteLearnProfession(player, SKILL_ENGINEERING); break;
			  case 6: CompleteLearnProfession(player, SKILL_ENCHANTING); break;
			  case 7: CompleteLearnProfession(player, SKILL_JEWELCRAFTING); break;
			  case 8: CompleteLearnProfession(player, SKILL_INSCRIPTION); break;
			  case 15: CompleteLearnProfession(player, SKILL_FIRST_AID); break;
			  case 16: CompleteLearnProfession(player, SKILL_FISHING); break;
			  case 17: CompleteLearnProfession(player, SKILL_COOKING); break;
			  case 18: CompleteLearnProfession(player, SKILL_ARCHAEOLOGY); break;
			  case 19: CompleteLearnProfession(player, SKILL_HERBALISM); break;
			  case 20: CompleteLearnProfession(player, SKILL_MINING); break;
			  case 21: CompleteLearnProfession(player, SKILL_SKINNING); break;
			  case 22: CompleteLearnProfession(player, SKILL_COOKING); break;
			  case 23: CompleteLearnProfession(player, SKILL_FISHING); break;
			  case 24: CompleteLearnProfession(player, SKILL_FIRST_AID); break;
			  }
		  }
		  player->CLOSE_GOSSIP_MENU();
		  return true;
	  }
};

enum Enchants
{
	ENCHANT_WEP_LANDSLIDE = 4099,
	ENCHANT_WEP_POWER_TORRENT = 4097,
	ENCHANT_WEP_WINDWALK = 4098,
	ENCHANT_WEP_HEARTSONG = 4084,
	ENCHANT_WEP_HURRICANE = 4083,
	ENCHANT_WEP_ELEMENTAL_SLAYER = 4074,
	ENCHANT_WEP_AVALANCHE = 4067,
	ENCHANT_WEP_MENDING = 4066,
	ENCHANT_WEP_PYRIUM = 4217,
	ENCHANT_WEP_MONGOOSE = 2673,
	ENCHANT_2HWEP_MIGHTY_AGILITY = 4227,

	ENCHANT_RANGED_FLINTLOCKE = 4267,
	ENCHANT_RANGED_GNOMISH = 4175,

	ENCHANT_OFF_HAND_SUPERIOR_INTELLECT = 4091,
	ENCHANT_SHIELD_MASTERY = 4085,
	ENCHANT_SHIELD_PROTECTION = 4073,
	ENCHANT_SHIELD_PYRIUM = 4216,
	ENCHANT_SHIELD_TITANIUM = 3849,

	ENCHANT_HEAD_VICIOUS_INTELLECT = 4245,
	ENCHANT_HEAD_VICIOUS_AGILITY = 4246,
	ENCHANT_HEAD_VICIOUS_STRENGTH = 4247,
	ENCHANT_HEAD_PVE_INT = 4207,
	ENCHANT_HEAD_PVE_STR = 4208,
	ENCHANT_HEAD_PVE_AGI = 4209,
	ENCHANT_HEAD_PVE_STA = 4206,

	ENCHANT_SHOULDER_VICIOUS_INTELLECT = 4248,
	ENCHANT_SHOULDER_VICIOUS_AGILITY = 4250,
	ENCHANT_SHOULDER_VICIOUS_STRENGTH = 4249,
	ENCHANT_SHOULDER_PVE_INT = 4200,
	ENCHANT_SHOULDER_PVE_STR = 4202,
	ENCHANT_SHOULDER_PVE_AGI = 4204,
	ENCHANT_SHOULDER_PVE_STA = 4198,

	ENCHANT_CLOAK_GREATER_CRITICAL_STRIKE = 4100,
	ENCHANT_CLOAK_GREATER_INTELLECT = 4096,
	ENCHANT_CLOAK_PROTECTION = 4090,
	ENCHANT_CLOAK_GREATER_SPELL_PIERCING = 4064,
	ENCHANT_CLOACK_AGILITY = 1099,

	ENCHANT_GLOVES_GREATER_MASTERY = 4107,
	ENCHANT_GLOVES_MIGHTY_STRENGTH = 4106,
	ENCHANT_GLOVES_GREATER_EXPERTISE = 4082,
	ENCHANT_GLOVES_HASTE = 4068,
	ENCHANT_GLOVES_AGILITY = 3222,
	ENCHANT_GLOVES_EXCEPTIONAL_SPELLPOWER = 3246,

	ENCHANT_BRACERS_GREATER_CRITICAL_STRIKE = 4101,
	ENCHANT_BRACERS_GREATER_SPEED = 4108,
	ENCHANT_BRACERS_AGILITY = 4258,
	ENCHANT_BRACERS_MAJOR_STRENGTH = 4256,
	ENCHANT_BRACERS_MIGHTY_INTELLECT = 4257,
	ENCHANT_BRACERS_GREATER_EXPERTISE = 4095,
	ENCHANT_BRACERS_EXCEPTIONAL_SPIRIT = 4093,
	ENCHANT_BRACERS_PRECISION = 4089,
	ENCHANT_BRACERS_SUPERIOR_DODGE = 4086,

	ENCHANT_CHEST_PEERLESS_STATS = 4102,
	ENCHANT_CHEST_GREATER_STAMINA = 4103,
	ENCHANT_CHEST_EXCEPTIONAL_SPIRIT = 4088,
	ENCHANT_CHEST_MIGHTY_RESILIENCE = 4077,

	ENCHANT_BOOTS_ASSASSINS_STEP = 4105,
	ENCHANT_BOOTS_LAVAWALKER = 4104,
	ENCHANT_BOOTS_MASTERY = 4094,
	ENCHANT_BOOTS_PRECISION = 4092,
	ENCHANT_BOOTS_MAJOR_AGILITY = 4076,
	ENCHANT_BOOTS_HASTE = 4069,

	ENCHANT_RING_AGILITY = 4079,
	ENCHANT_RING_GREATER_STAMINA = 4081,
	ENCHANT_RING_INTELLECT = 4080,
	ENCHANT_RING_STRENGTH = 4078,

	ENCHANT_LEGS_DRAGONSCALE = 4126,
	ENCHANT_LEGS_CHARSCALE = 4127,
	ENCHANT_LEGS_DRAKEHIDE = 4270,
	ENCHAANT_POWERFUL_GHOSTLY_THREAD = 4110,
	ENCHAANT_POWERFUL_ENCHANTED_THREAD = 4112,
	ENCHANT_LEGS_LEG = 71720,

	ENCHANT_WAIST_BUCKLE = 3729,

	// Blacksmithing
	ENCHANT_WRIST_SOCKET = 3717,
	ENCHANT_GLOVE_SOCKET = 3723,

	// Leatherworking
	ENCHANT_LW_BRACER_INT = 4192,
	ENCHANT_LW_BRACER_STR = 4191,
	ENCHANT_LW_BRACER_AGI = 4190,
	ENCHANT_LW_BRACER_STA = 4189,

	// Tailoring
	ENCHANT_TAILOR_DARKGLOW = 4116,
	ENCHANT_TAILOR_LIGHTWAV = 4115,
	ENCHANT_TAILOR_SWORDGUA = 4118,

	// Engineering
	ENCHANT_ENGI_SYNAPSE = 4179,
	ENCHANT_ENGI_TAZIK = 4181,
	ENCHANT_ENGI_PYRO = 3603,
	ENCHANT_ENGI_HYPER = 3604,
	ENCHANT_ENGI_NITRO = 4223,
	ENCHANT_ENGI_PARA = 3605,

	// Inscription
	ENCHANT_INSC_SHOULDER_INT = 4196,
	ENCHANT_INSC_SHOULDER_AGI = 4193,
	ENCHANT_INSC_SHOULDER_STR = 4194,
	ENCHANT_INSC_SHOULDER_STA = 4195,

	/*Runeforge*/
	RUNEFORGE_CINDERGLAICER = 3369,
	RUNEFORGE_LICHBANE = 3366,
	RUNEFORGE_RAZORICE = 3370,
	RUNEFORGE_SPELLBREAKING = 3595,
	RUNEFORGE_SPELLSHATTERING = 3367,
	RUNEFORGE_SWORDBREAKING = 3594,
	RUNEFROGE_SWORDSHATTERING = 3365,
	RUNEFORGE_FALLEN_CRUSADE = 3368,
	RUNEFORGE_NERUBIAN_CARAPACE = 3883,
	RUNEFORGE_STONESKIN_GARGYOLE = 3847


};

#define ONE_HAND_WEAPON_COST	0 
#define TWO_HAND_WEAPON_COST	0 
#define MAIN_PIECES_COST		0 
#define OFF_PIECES_COST	        0 
#define SHIELD_COST				0 

void Enchant(Player* player, Item* item, uint32 enchantid)
{
	if (!item)
	{
		player->GetSession()->SendNotification("You need to equip an item in that slot to enchant it!");
		return;
	}

	if (!enchantid)
		return;

	bool isEngineer = false;

	if (enchantid == 4179 || enchantid == 4181 || enchantid == 3603 || enchantid == 3604 || enchantid == 4223 || enchantid == 3605)
		isEngineer = true;

	if (isEngineer)
	{
		player->ApplyEnchantment(item, ENGINEERING_ENCHANT_SLOT, false);
		item->SetEnchantment(ENGINEERING_ENCHANT_SLOT, enchantid, 0, 0);
		player->ApplyEnchantment(item, ENGINEERING_ENCHANT_SLOT, true);
	}
	else
	{
		player->ApplyEnchantment(item, PERM_ENCHANTMENT_SLOT, false);
		item->SetEnchantment(PERM_ENCHANTMENT_SLOT, enchantid, 0, 0);
		player->ApplyEnchantment(item, PERM_ENCHANTMENT_SLOT, true);
	}
	std::string color = "|cff";
	switch (item->GetTemplate()->Quality)
	{
	case 0:
		color += "9d9d9d";
		break;
	case 1:
		color += "ffffff";
		break;
	case 2:
		color += "1eff00";
		break;
	case 3:
		color += "0070dd";
		break;
	case 4:
		color += "a335ee";
		break;
	case 5:
		color += "ff8000";
		break;
	}
	std::string itemname = item->GetTemplate()->Name1;
	ChatHandler(player->GetSession()).PSendSysMessage("|cffFFFFFF[%s%s|cffFFFFFF] |cffFF0000succesfully enchanted!", color.c_str(), itemname.c_str());
}

void PrismaticEnchant(Player* player, Item* item, uint32 enchantid)
{
	if (!item)
	{
		player->GetSession()->SendNotification("You need to equip an item in that slot to enchant it!");
		return;
	}

	if (!enchantid)
		return;

	player->ApplyEnchantment(item, PRISMATIC_ENCHANTMENT_SLOT, false);
	item->SetEnchantment(PRISMATIC_ENCHANTMENT_SLOT, enchantid, 0, 0);
	player->ApplyEnchantment(item, PRISMATIC_ENCHANTMENT_SLOT, true);
	std::string color = "|cff";
	switch (item->GetTemplate()->Quality)
	{
	case 0:
		color += "9d9d9d";
		break;
	case 1:
		color += "ffffff";
		break;
	case 2:
		color += "1eff00";
		break;
	case 3:
		color += "0070dd";
		break;
	case 4:
		color += "a335ee";
		break;
	case 5:
		color += "ff8000";
		break;
	}
	std::string itemname = item->GetTemplate()->Name1;
	ChatHandler(player->GetSession()).PSendSysMessage("|cffFFFFFF[%s%s|cffFFFFFF] |cffFF0000succesfully enchanted!", color.c_str(), itemname.c_str());
}

class npc_tourny_enchants : public CreatureScript
{
	std::string iconSize = "20";
public:
	npc_tourny_enchants() : CreatureScript("npc_tourny_enchants") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\INV_Misc_Desecrated_PlateHelm:20:20:-10:0|t[Enchant Head]", GOSSIP_SENDER_MAIN, 4);
		player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\INV_Misc_Desecrated_PlateShoulder:20:20:-10:0|t[Enchant Shoulders]", GOSSIP_SENDER_MAIN, 5);
		player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\INV_Misc_Cape_11:20:20:-10:0|t[Enchant Cloak]", GOSSIP_SENDER_MAIN, 6);
		player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\INV_Misc_Desecrated_PlateChest:20:20:-10:0|t[Enchant Chest]", GOSSIP_SENDER_MAIN, 7);
		player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\INV_Misc_Desecrated_PlateBracer:20:20:-10:0|t[Enchant Bracers]", GOSSIP_SENDER_MAIN, 8);
		player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\INV_Misc_Desecrated_PlateGloves:20:20:-10:0|t[Enchant Gloves]", GOSSIP_SENDER_MAIN, 9);
		player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\INV_Misc_Desecrated_PlateBelt:20:20:-10:0|t[Enchant Waist]", GOSSIP_SENDER_MAIN, 13);
		player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\INV_Misc_Desecrated_PlatePants:20:20:-10:0|t[Enchant Legs]", GOSSIP_SENDER_MAIN, 10);
		player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\INV_Misc_Desecrated_PlateBoots:20:20:-10:0|t[Enchant Feet]", GOSSIP_SENDER_MAIN, 11);
		if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) > 450)
			player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\INV_Jewelry_Ring_01:20:20:-10:0|t[Enchanting]: [Enchant Rings]", GOSSIP_SENDER_MAIN, 12);
		player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\INV_Sword_04:20:20:-10:0|t[Enchant Weapon]", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\INV_Shield_04:20:20:-10:0|t[Enchant Off-Hand]", GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\ability_meleedamage:20:20:-10:0|t[Enchant 2H Weapon]", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\INV_Weapon_Crossbow_04:20:20:-10:0|t[Enchant Ranged]", GOSSIP_SENDER_MAIN, 999);
		if (player->getClass() == CLASS_DEATH_KNIGHT)
			player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\Spell_deathknight_frozenruneweapon:20:20:-10:0|t[Runeforge]", GOSSIP_SENDER_MAIN, 0);
		player->SEND_GOSSIP_MENU(100001, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();
		Item* mainItem;

		switch (action)
		{
		case 0: //Runefroge
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Main Hand", GOSSIP_SENDER_MAIN, 160);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Off Hand", GOSSIP_SENDER_MAIN, 161);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
			player->PlayerTalkClass->SendGossipMenu(100015, creature->GetGUID());
			return true;
			break;
		case 1: // Enchant Weapon
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Heartsong", GOSSIP_SENDER_MAIN, 102);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Hurricane", GOSSIP_SENDER_MAIN, 103);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Avalanche", GOSSIP_SENDER_MAIN, 100);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Elemental Slayer", GOSSIP_SENDER_MAIN, 101);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Landslide", GOSSIP_SENDER_MAIN, 104);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Mending", GOSSIP_SENDER_MAIN, 105);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Power Torrent", GOSSIP_SENDER_MAIN, 106);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Windwalk", GOSSIP_SENDER_MAIN, 107);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Pyrium Weapon Chain", GOSSIP_SENDER_MAIN, 511);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Mongoose", GOSSIP_SENDER_MAIN, 901);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
			player->PlayerTalkClass->SendGossipMenu(100002, creature->GetGUID());
			return true;
			break;

		case 2: // Enchant 2H Weapon
		{
			mainItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
			if (!mainItem || mainItem->GetTemplate()->InventoryType != INVTYPE_2HWEAPON)
			{
				player->GetSession()->SendAreaTriggerMessage("|cffFF0000You need a Two-Hand Weapon equipped!");
				return false;
				player->CLOSE_GOSSIP_MENU();;
			}
			if (mainItem->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Heartsong", GOSSIP_SENDER_MAIN, 102);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Hurricane", GOSSIP_SENDER_MAIN, 103);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Avalanche", GOSSIP_SENDER_MAIN, 100);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Elemental Slayer", GOSSIP_SENDER_MAIN, 101);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Landslide", GOSSIP_SENDER_MAIN, 104);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Mending", GOSSIP_SENDER_MAIN, 105);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Power Torrent", GOSSIP_SENDER_MAIN, 106);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Windwalk", GOSSIP_SENDER_MAIN, 107);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Pyrium Weapon Chain", GOSSIP_SENDER_MAIN, 511);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Mongoose", GOSSIP_SENDER_MAIN, 901);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Mighty Agility", GOSSIP_SENDER_MAIN, 108);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
				player->PlayerTalkClass->SendGossipMenu(100003, creature->GetGUID());
				return true;
			}
		}
		break;

		case 3: // Enchant Shield
		{
			mainItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
			if (!mainItem)
			{
				player->GetSession()->SendAreaTriggerMessage("|cffFF0000You need an Off-Hand or Shield equipped!");
				player->CLOSE_GOSSIP_MENU();
				return false;
			}
			if (mainItem->GetTemplate()->InventoryType == INVTYPE_SHIELD)
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+50 Mastery", GOSSIP_SENDER_MAIN, 109);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+160 Armor", GOSSIP_SENDER_MAIN, 110);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+40 Intellect", GOSSIP_SENDER_MAIN, 111);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Pyrium Shield Spike", GOSSIP_SENDER_MAIN, 903);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Titanium Plating", GOSSIP_SENDER_MAIN, 904);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
				player->PlayerTalkClass->SendGossipMenu(100004, creature->GetGUID());
				return true;
			}
			if (mainItem->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND || mainItem->GetTemplate()->InventoryType == INVTYPE_WEAPON)
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Heartsong", GOSSIP_SENDER_MAIN, 602);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Hurricane", GOSSIP_SENDER_MAIN, 603);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Avalanche", GOSSIP_SENDER_MAIN, 600);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Elemental Slayer", GOSSIP_SENDER_MAIN, 601);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Landslide", GOSSIP_SENDER_MAIN, 604);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Mending", GOSSIP_SENDER_MAIN, 605);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Power Torrent", GOSSIP_SENDER_MAIN, 606);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Windwalk", GOSSIP_SENDER_MAIN, 607);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Pyrium Weapon Chain", GOSSIP_SENDER_MAIN, 608);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Mongoose", GOSSIP_SENDER_MAIN, 902);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
				player->SEND_GOSSIP_MENU(100004, creature->GetGUID());
			}
			if (mainItem->GetTemplate()->InventoryType == INVTYPE_HOLDABLE)
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+40 Intellect", GOSSIP_SENDER_MAIN, 111);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
				player->SEND_GOSSIP_MENU(100004, creature->GetGUID());
			}

		}
		break;

		case 999: // Enchant Ranged
		{
			mainItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED);
			if (!mainItem)
			{
				player->GetSession()->SendAreaTriggerMessage("|cffFF0000You need a ranged weapon equipped!");
				player->CLOSE_GOSSIP_MENU();
				return false;
			}
			if (mainItem->GetTemplate()->Class == 2 && mainItem->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_BOW || mainItem->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_CROSSBOW || mainItem->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_GUN)
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Flintlocke's Woodchucker", GOSSIP_SENDER_MAIN, 998);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Gnomish X-Ray Scope", GOSSIP_SENDER_MAIN, 997);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
				player->PlayerTalkClass->SendGossipMenu(100004, creature->GetGUID());
				return true;
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cffFF0000You need a ranged weapon equipped!");
				player->CLOSE_GOSSIP_MENU();
				return false;
			}

		}
		break;

		case 4: // Enchant Head
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+60 Intellect & +35 Resilience", GOSSIP_SENDER_MAIN, 113);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+60 Agility & +35 Resilience", GOSSIP_SENDER_MAIN, 112);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+60 Strength & +35 Resilience", GOSSIP_SENDER_MAIN, 114);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+60 Intellect and +35 Critical Rating", GOSSIP_SENDER_MAIN, 701);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+60 Agility and +35 Haste Rating", GOSSIP_SENDER_MAIN, 702);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+60 Strength and +35 Mastery Rating", GOSSIP_SENDER_MAIN, 703);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+90 Stamina and +35 Dodge Rating", GOSSIP_SENDER_MAIN, 704);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
			player->PlayerTalkClass->SendGossipMenu(100005, creature->GetGUID());
			return true;
			break;

		case 5: // Enchant Shoulders
			if (player->HasSkill(SKILL_INSCRIPTION) && player->GetSkillValue(SKILL_INSCRIPTION) > 450)
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Inscription_Tradeskill01:" + iconSize + " |t|r [Inscription]: +130 Intellect and +25 Haste Rating", GOSSIP_SENDER_MAIN, 801);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Inscription_Tradeskill01:" + iconSize + " |t|r [Inscription]: +130 Agility and +25 Mastery Rating", GOSSIP_SENDER_MAIN, 802);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Inscription_Tradeskill01:" + iconSize + " |t|r [Inscription]: +130 Strength and +25 Critical Rating", GOSSIP_SENDER_MAIN, 803);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Inscription_Tradeskill01:" + iconSize + " |t|r [Inscription]: +195 Stamina and +25 Dodge Rating", GOSSIP_SENDER_MAIN, 804);
			}
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+50 Intellect & +25 Resilience", GOSSIP_SENDER_MAIN, 116);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+50 Agility & +25 Resilience", GOSSIP_SENDER_MAIN, 115);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+50 Strength & +25 Resilience", GOSSIP_SENDER_MAIN, 117);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+50 Intellect and +25 Haste Rating", GOSSIP_SENDER_MAIN, 705);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+50 Agility and +25 Mastery Rating", GOSSIP_SENDER_MAIN, 706);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+50 Strength and +25 Critical Rating", GOSSIP_SENDER_MAIN, 707);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+75 Stamina and +25 Dodge Rating", GOSSIP_SENDER_MAIN, 708);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
			player->PlayerTalkClass->SendGossipMenu(100006, creature->GetGUID());
			return true;
			break;

		case 6: // Enchant Cloak
			if (player->HasSkill(SKILL_ENGINEERING) && player->GetSkillValue(SKILL_ENGINEERING) > 450)
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_engineering:" + iconSize + " |t|r [Engineering]: Parachute", GOSSIP_SENDER_MAIN, 512);
			if (player->HasSkill(SKILL_TAILORING) && player->GetSkillValue(SKILL_TAILORING) > 450)
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_tailoring:" + iconSize + " |t|r [Tailoring]: 580 Spirit (proc)", GOSSIP_SENDER_MAIN, 513);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_tailoring:" + iconSize + " |t|r [Tailoring]: 580 Intellect (proc)", GOSSIP_SENDER_MAIN, 514);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_tailoring:" + iconSize + " |t|r [Tailoring]: 1000 Attack Power (proc)", GOSSIP_SENDER_MAIN, 515);
			}
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+65 Critical Strike", GOSSIP_SENDER_MAIN, 118);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+50 Intellect", GOSSIP_SENDER_MAIN, 119);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+70 Spell Pen", GOSSIP_SENDER_MAIN, 120);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+22 Agility", GOSSIP_SENDER_MAIN, 156);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+250 Armor", GOSSIP_SENDER_MAIN, 121);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
			player->PlayerTalkClass->SendGossipMenu(100007, creature->GetGUID());
			return true;
			break;

		case 7: //Enchant chest
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+40 Spirit", GOSSIP_SENDER_MAIN, 122);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+75 Stamina", GOSSIP_SENDER_MAIN, 123);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+40 Resilience", GOSSIP_SENDER_MAIN, 124);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+20 Stats", GOSSIP_SENDER_MAIN, 125);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
			player->PlayerTalkClass->SendGossipMenu(100008, creature->GetGUID());
			return true;
			break;

		case 8: //Enchant Bracers
			if (player->HasSkill(SKILL_BLACKSMITHING) && player->GetSkillValue(SKILL_BLACKSMITHING) > 450)
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_blacksmithing:" + iconSize + " |t|r [Blacksmithing]: Socket Bracer", GOSSIP_SENDER_MAIN, 500);
			if (player->HasSkill(SKILL_LEATHERWORKING) && player->GetSkillValue(SKILL_LEATHERWORKING) > 450)
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Misc_ArmorKit_17:" + iconSize + " |t|r [Leatherworking]: +130 Intellect", GOSSIP_SENDER_MAIN, 501);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Misc_ArmorKit_17:" + iconSize + " |t|r [Leatherworking]: +130 Agility", GOSSIP_SENDER_MAIN, 502);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Misc_ArmorKit_17:" + iconSize + " |t|r [Leatherworking]: +130 Strength", GOSSIP_SENDER_MAIN, 503);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Misc_ArmorKit_17:" + iconSize + " |t|r [Leatherworking]: +195 Stamina", GOSSIP_SENDER_MAIN, 504);
			}
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+50 Agility", GOSSIP_SENDER_MAIN, 126);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+50 Spirit", GOSSIP_SENDER_MAIN, 127);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+65 Critical Strike", GOSSIP_SENDER_MAIN, 128);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+50 Expertise", GOSSIP_SENDER_MAIN, 129);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+65 Haste", GOSSIP_SENDER_MAIN, 130);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+50 Strength", GOSSIP_SENDER_MAIN, 131);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+50 Intellect", GOSSIP_SENDER_MAIN, 132);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+50 Hit", GOSSIP_SENDER_MAIN, 133);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+50 Dodge", GOSSIP_SENDER_MAIN, 134);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
			player->PlayerTalkClass->SendGossipMenu(100009, creature->GetGUID());
			return true;
			break;

		case 9: //Enchant Gloves
			if (player->HasSkill(SKILL_BLACKSMITHING) && player->GetSkillValue(SKILL_BLACKSMITHING) > 450)
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_blacksmithing:" + iconSize + " |t|r [Blacksmithing]: Socket Gloves", GOSSIP_SENDER_MAIN, 505);
			if (player->HasSkill(SKILL_ENGINEERING) && player->GetSkillValue(SKILL_ENGINEERING) > 450)
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_engineering:" + iconSize + " |t|r [Engineering]: 480 Stats (use)", GOSSIP_SENDER_MAIN, 516);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_engineering:" + iconSize + " |t|r [Engineering]: 240 Haste (use)", GOSSIP_SENDER_MAIN, 507);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_engineering:" + iconSize + " |t|r [Engineering]: Tazik Shocker", GOSSIP_SENDER_MAIN, 508);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_engineering:" + iconSize + " |t|r [Engineering]: Hand-Mounted Pyro Rocket", GOSSIP_SENDER_MAIN, 509);
			}
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+50 Expertise", GOSSIP_SENDER_MAIN, 135);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+65 Mastery", GOSSIP_SENDER_MAIN, 136);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+50 Haste", GOSSIP_SENDER_MAIN, 137);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+50 Strength", GOSSIP_SENDER_MAIN, 138);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+20 Agility", GOSSIP_SENDER_MAIN, 157);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+28 Spell Power", GOSSIP_SENDER_MAIN, 159);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
			player->PlayerTalkClass->SendGossipMenu(100010, creature->GetGUID());
			return true;
			break;

		case 10: // Enchant Legs
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+190 Attack Power & +55 Critical Strike", GOSSIP_SENDER_MAIN, 151);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+145 Stamina & +55 Agility", GOSSIP_SENDER_MAIN, 152);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+145 Stamina & +55 Dodge Rating", GOSSIP_SENDER_MAIN, 153);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+95 Intellect & +55 Spirit", GOSSIP_SENDER_MAIN, 154);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+95 Intellect & +80 Stamina", GOSSIP_SENDER_MAIN, 155);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
			player->PlayerTalkClass->SendGossipMenu(100011, creature->GetGUID());
			return true;
			break;

		case 11: //Enchant feet
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+25 Agility & Movement Speed", GOSSIP_SENDER_MAIN, 139);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+35 Mastery & Movement Speed", GOSSIP_SENDER_MAIN, 141);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+50 Haste", GOSSIP_SENDER_MAIN, 140);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+35 Agility", GOSSIP_SENDER_MAIN, 142);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+50 Mastery", GOSSIP_SENDER_MAIN, 143);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+50 Hit Rating", GOSSIP_SENDER_MAIN, 144);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
			player->PlayerTalkClass->SendGossipMenu(100012, creature->GetGUID());
			return true;
			break;

		case 12: //Enchant rings
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+40 Agility", GOSSIP_SENDER_MAIN, 145);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+60 Stamina", GOSSIP_SENDER_MAIN, 146);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+40 Intellect", GOSSIP_SENDER_MAIN, 147);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "+40 Strength", GOSSIP_SENDER_MAIN, 148);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
			player->PlayerTalkClass->SendGossipMenu(100013, creature->GetGUID());
			return true;
			break;

		case 13: //Enchant waist
			if (player->HasSkill(SKILL_ENGINEERING) && player->GetSkillValue(SKILL_ENGINEERING) > 450)
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_engineering:" + iconSize + " |t|r [Engineering]: Nitro Boost", GOSSIP_SENDER_MAIN, 510);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Ebonsteel Belt Buckle", GOSSIP_SENDER_MAIN, 158);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
			player->PlayerTalkClass->SendGossipMenu(100014, creature->GetGUID());
			return true;
			break;

		case 998:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED), ENCHANT_RANGED_FLINTLOCKE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 997:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED), ENCHANT_RANGED_GNOMISH);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 100:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_AVALANCHE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 101:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_ELEMENTAL_SLAYER);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 102:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_HEARTSONG);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 103:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_HURRICANE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 104:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_LANDSLIDE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 105:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_MENDING);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 106:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_POWER_TORRENT);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 107:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_WINDWALK);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 511:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_PYRIUM);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 901:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_MONGOOSE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 902:
			mainItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
			if (mainItem->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND || INVTYPE_WEAPON)
			{
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_MONGOOSE);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cffFF0000You don't have off-hand equipped!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case 600:
			mainItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
			if (mainItem->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND || INVTYPE_WEAPON)
			{
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_AVALANCHE);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cffFF0000You don't have off-hand equipped!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case 601:
			mainItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
			if (mainItem->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND || INVTYPE_WEAPON)
			{
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_ELEMENTAL_SLAYER);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cffFF0000You don't have off-hand equipped!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case 602:
			mainItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
			if (mainItem->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND || INVTYPE_WEAPON)
			{
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_HEARTSONG);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cffFF0000You don't have off-hand equipped!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case 603:
			mainItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
			if (mainItem->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND || INVTYPE_WEAPON)
			{
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_HURRICANE);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cffFF0000You don't have off-hand equipped!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case 604:
			mainItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
			if (mainItem->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND || INVTYPE_WEAPON)
			{
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_LANDSLIDE);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cffFF0000You don't have off-hand equipped!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case 605:
			mainItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
			if (mainItem->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND || INVTYPE_WEAPON)
			{
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_MENDING);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cffFF0000You don't have off-hand equipped!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case 606:
			mainItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
			if (mainItem->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND || INVTYPE_WEAPON)
			{
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_POWER_TORRENT);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cffFF0000You don't have off-hand equipped!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case 607:
			mainItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
			if (mainItem->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND || INVTYPE_WEAPON)
			{
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_WINDWALK);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cffFF0000You don't have off-hand equipped!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case 608:
			mainItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
			if (mainItem->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND || INVTYPE_WEAPON)
			{
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_PYRIUM);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cffFF0000You don't have off-hand equipped!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case 108:
			mainItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
			if (mainItem->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
			{
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2HWEP_MIGHTY_AGILITY);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cffFF0000You don't have 2H equipped!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case 903:
			mainItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
			if (mainItem->GetTemplate()->InventoryType == INVTYPE_SHIELD)
			{
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_PYRIUM);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cffFF0000You don't have Shield equipped!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case 904:
			mainItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
			if (mainItem->GetTemplate()->InventoryType == INVTYPE_SHIELD)
			{
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_TITANIUM);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cffFF0000You don't have Shield equipped!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case 109:
			mainItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
			if (mainItem->GetTemplate()->InventoryType == INVTYPE_SHIELD)
			{
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_MASTERY);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cffFF0000You don't have Shield equipped!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case 110:
			mainItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
			if (mainItem->GetTemplate()->InventoryType == INVTYPE_SHIELD)
			{
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_PROTECTION);
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				player->GetSession()->SendAreaTriggerMessage("|cffFF0000You don't have Shield equipped!");
				player->CLOSE_GOSSIP_MENU();
			}
			break;

		case 111:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_OFF_HAND_SUPERIOR_INTELLECT);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 112:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_VICIOUS_AGILITY);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 113:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_VICIOUS_INTELLECT);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 114:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_VICIOUS_STRENGTH);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 701:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_PVE_INT);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 702:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_PVE_AGI);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 703:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_PVE_STR);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 704:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_PVE_STA);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 115:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_VICIOUS_AGILITY);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 116:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_VICIOUS_INTELLECT);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 117:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_VICIOUS_STRENGTH);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 705:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_PVE_INT);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 706:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_PVE_AGI);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 707:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_PVE_STR);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 708:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_PVE_STA);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 801:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_INSC_SHOULDER_INT);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 802:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_INSC_SHOULDER_AGI);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 803:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_INSC_SHOULDER_STR);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 804:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_INSC_SHOULDER_STA);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 118:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_GREATER_CRITICAL_STRIKE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 119:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_GREATER_INTELLECT);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 120:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_GREATER_SPELL_PIERCING);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 121:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_PROTECTION);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 513:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_TAILOR_DARKGLOW);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 514:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_TAILOR_LIGHTWAV);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 515:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_TAILOR_SWORDGUA);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 512:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_ENGI_PARA);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 122:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_EXCEPTIONAL_SPIRIT);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 123:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_GREATER_STAMINA);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 124:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_MIGHTY_RESILIENCE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 125:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_PEERLESS_STATS);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 126:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_AGILITY);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 127:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_EXCEPTIONAL_SPIRIT);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 128:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_GREATER_CRITICAL_STRIKE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 129:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_GREATER_EXPERTISE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 130:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_GREATER_SPEED);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 131:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_MAJOR_STRENGTH);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 132:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_MIGHTY_INTELLECT);
			player->CLOSE_GOSSIP_MENU();
			break;
		case 133:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_PRECISION);
			player->CLOSE_GOSSIP_MENU();
			break;
		case 134:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_SUPERIOR_DODGE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 500:
			PrismaticEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_WRIST_SOCKET);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 501:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_LW_BRACER_INT);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 506:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_LW_BRACER_STR);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 502:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_LW_BRACER_AGI);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 504:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_LW_BRACER_STA);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 135:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_GREATER_EXPERTISE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 136:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_GREATER_MASTERY);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 137:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_HASTE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 138:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_MIGHTY_STRENGTH);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 139:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_ASSASSINS_STEP);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 140:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_HASTE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 141:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_LAVAWALKER);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 142:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_MAJOR_AGILITY);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 143:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_MASTERY);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 144:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_PRECISION);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 145:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1), ENCHANT_RING_AGILITY);
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2), ENCHANT_RING_AGILITY);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 146:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1), ENCHANT_RING_GREATER_STAMINA);
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2), ENCHANT_RING_GREATER_STAMINA);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 147:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1), ENCHANT_RING_INTELLECT);
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2), ENCHANT_RING_INTELLECT);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 148:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1), ENCHANT_RING_STRENGTH);
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2), ENCHANT_RING_STRENGTH);
			player->CLOSE_GOSSIP_MENU();
			break;


		case 151:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEGS_DRAGONSCALE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 152:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEGS_CHARSCALE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 153:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEGS_DRAKEHIDE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 154:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHAANT_POWERFUL_GHOSTLY_THREAD);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 155:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHAANT_POWERFUL_ENCHANTED_THREAD);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 156:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOACK_AGILITY);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 505:
			PrismaticEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVE_SOCKET);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 516:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_ENGI_SYNAPSE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 508:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_ENGI_TAZIK);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 509:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_ENGI_PYRO);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 507:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_ENGI_HYPER);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 157:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_AGILITY);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 158:
			PrismaticEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WAIST), ENCHANT_WAIST_BUCKLE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 159:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_EXCEPTIONAL_SPELLPOWER);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 510:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WAIST), ENCHANT_ENGI_NITRO);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 149:
			if (player->GetMoney() < OFF_PIECES_COST) {
				ChatHandler(player->GetSession()).PSendSysMessage("You don't have enough money to purchase this! It costs %d gold", MAIN_PIECES_COST);
				player->CLOSE_GOSSIP_MENU();;
				break;
			}

			/*Runeforge*/
			//Main hand
		case 160:
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Rune of Cinderglacier", GOSSIP_SENDER_MAIN, 162);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Rune of Lichbane", GOSSIP_SENDER_MAIN, 163);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Rune of Razorice", GOSSIP_SENDER_MAIN, 164);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Rune of Spellbreaking", GOSSIP_SENDER_MAIN, 165);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Rune of Spellshattering", GOSSIP_SENDER_MAIN, 166);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Rune of Swordbreaking", GOSSIP_SENDER_MAIN, 167);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Rune of Swordshattering", GOSSIP_SENDER_MAIN, 168);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Rune of the Fallen Crusader", GOSSIP_SENDER_MAIN, 169);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Rune of the Nerubian Carapace", GOSSIP_SENDER_MAIN, 170);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Rune of the Stoneskin Gargoyle", GOSSIP_SENDER_MAIN, 171);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
			player->PlayerTalkClass->SendGossipMenu(100016, creature->GetGUID());
			return true;
			break;

			//Offhand
		case 161:
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Rune of Cinderglacier", GOSSIP_SENDER_MAIN, 172);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Rune of Lichbane", GOSSIP_SENDER_MAIN, 173);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Rune of Razorice", GOSSIP_SENDER_MAIN, 174);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Rune of Spellbreaking", GOSSIP_SENDER_MAIN, 175);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Rune of Spellshattering", GOSSIP_SENDER_MAIN, 176);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Rune of Swordbreaking", GOSSIP_SENDER_MAIN, 177);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Rune of Swordshattering", GOSSIP_SENDER_MAIN, 178);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Rune of the Fallen Crusader", GOSSIP_SENDER_MAIN, 179);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Rune of the Nerubian Carapace", GOSSIP_SENDER_MAIN, 180);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "Rune of the Stoneskin Gargoyle", GOSSIP_SENDER_MAIN, 181);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
			player->PlayerTalkClass->SendGossipMenu(100017, creature->GetGUID());
			return true;
			break;
			/*Runeforge Mainhand*/
		case 162:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), RUNEFORGE_CINDERGLAICER);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 163:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), RUNEFORGE_LICHBANE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 164:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), RUNEFORGE_RAZORICE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 165:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), RUNEFORGE_SPELLBREAKING);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 166:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), RUNEFORGE_SPELLSHATTERING);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 167:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), RUNEFORGE_SWORDBREAKING);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 168:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), RUNEFROGE_SWORDSHATTERING);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 169:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), RUNEFORGE_FALLEN_CRUSADE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 170:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), RUNEFORGE_NERUBIAN_CARAPACE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 171:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), RUNEFORGE_STONESKIN_GARGYOLE);
			player->CLOSE_GOSSIP_MENU();
			break;

			/*Runeforge offhand*/

		case 172:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), RUNEFORGE_CINDERGLAICER);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 173:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), RUNEFORGE_LICHBANE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 174:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), RUNEFORGE_RAZORICE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 175:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), RUNEFORGE_SPELLBREAKING);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 176:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), RUNEFORGE_SPELLSHATTERING);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 177:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), RUNEFORGE_SWORDBREAKING);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 178:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), RUNEFROGE_SWORDSHATTERING);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 179:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), RUNEFORGE_FALLEN_CRUSADE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 180:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), RUNEFORGE_NERUBIAN_CARAPACE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 181:
			Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), RUNEFORGE_STONESKIN_GARGYOLE);
			player->CLOSE_GOSSIP_MENU();
			break;

		case 300:
			player->PlayerTalkClass->ClearMenus();
			OnGossipHello(player, creature);
			break;
		}

		return true;
	}
};

class tourney_login_first : public PlayerScript
{
public:
	tourney_login_first() : PlayerScript("tourney_login_first") {}

	void OnLogin(Player* player)
	{
		if (sWorld->getBoolConfig(CONFIG_TOURNAMENT_ENABLE))
		{
			if (player->HasAtLoginFlag(AT_LOGIN_FIRST))
			{
				if (player->GetSession()->GetSecurity() <= 1)
				{
					for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; slot++)
						player->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);

					if (player->getClass() == CLASS_DEATH_KNIGHT)
					{
						for (uint8 slot = INVENTORY_SLOT_BAG_START; slot < INVENTORY_SLOT_BAG_END; slot++)
							player->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);

						player->learnSpell(50977, false);
						player->learnSpell(53428, false);
					}

					for (uint8 slot = INVENTORY_SLOT_BAG_START; slot < INVENTORY_SLOT_BAG_END; slot++)
						player->EquipNewItem(slot, 54444, true);

					if (player->getRace() == RACE_WORGEN)
					{
						player->learnSpell(68996, false);
						player->learnSpell(68992, false);
					}

					player->GiveLevel(85);
					player->InitTalentForLevel();
					if (player->getClass() == CLASS_DEATH_KNIGHT)
					{
						player->SetFreeTalentPoints(41);
						player->SendTalentsInfoData(false);
					}
					player->SetUInt32Value(PLAYER_XP, 0);
					player->ModifyMoney(300000000, false);
					PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_LEVEL);
					stmt->setUInt8(0, uint8(85));
					stmt->setUInt32(1, GUID_LOPART(player->GetGUID()));
					CharacterDatabase.Execute(stmt);
					player->SaveToDB();

					player->TeleportTo(0, 4325.65f, -2872.95f, 4.34f, 2.25f);
				}
			}
		}
	}
};

class ressurect_players_on_death : public PlayerScript
{
public:
	ressurect_players_on_death() : PlayerScript("ressurect_players_on_death") { }

	void OnPVPKill(Player* killer, Player* victim)
	{
		if (sWorld->getBoolConfig(CONFIG_TOURNAMENT_RESS_ON_DEATH))
		{
			if (victim->GetAreaId() == 2037)
			{
				if (!killer || !victim)
					return;

				/*if (killer->GetGUID() == victim->GetGUID())
					return;*/

				// if (!killer->InArena() || !killer->InBattleground() && !victim->InArena() || !victim->InBattleground())
				victim->ClearInCombat();
				victim->ResurrectPlayer(100, false);
				victim->ResetAllPowers();
				ChatHandler(victim->GetSession()).PSendSysMessage("|cffFFC125[Safety System]|r |cffADFF2FYou have been resurrected, as you've died outside of a combat area.|r");
			}
		}
	}
};

void AddSC_TournamentScripts()
{
	new npc_tourny_teleport();
	new npc_tourny_vendor();
	new npc_tourny_i_request();
	new npc_tourny_trainer();
	new npc_tourny_beastmaster();
	new npc_tourny_professions();
	new npc_tourny_enchants();
	new tourney_login_first();
	new ressurect_players_on_death();
}