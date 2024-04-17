#include "Common.h"
#include "Creature.h"
#include "DBCStructure.h"
#include "Define.h"
#include "GossipDef.h"
#include "Language.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "SharedDefines.h"
#include "WorldSession.h"
#include <string>

typedef Player::FakeMorphData FakeMorphData;
struct MorphData
{
	int index;
	const char* name;
	FakeMorphData data;
	const char* icon;
};

static std::vector<MorphData> FakeMorphs;

int64 goldCost = 0;

const char* are_you_sure{ "Are you sure you want to morph into\n" };

MorphData* GetMorphData(int index)
{
	return &FakeMorphs[index];
}

class SelfFakeMorph_world : public WorldScript
{
	public:
		SelfFakeMorph_world() : WorldScript("World_FakeMorph") { }
		
		void OnStartup() override
		{
			goldCost = sWorld->getIntConfig(CONFIG_MORPHER_NPC_COST);
			FakeMorphs = {
				{ 0, "Human male",					FakeMorphData(37925, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\achievement_character_human_male:20:20:-10:0|t"},
				{ 1, "Human female",				FakeMorphData(37926, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\achievement_character_human_female:20:20:-10:0|t"},
				{ 2, "Dwarf male",					FakeMorphData(20317, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\achievement_character_dwarf_male:20:20:-10:0|t" },
				{ 3, "Dwarf female",				FakeMorphData(37918, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\achievement_character_dwarf_female:20:20:-10:0|t" },
				{ 4, "Night elf male",				FakeMorphData(20318, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\achievement_character_nightelf_male:20:20:-10:0|t" },
				{ 5, "Night elf female",			FakeMorphData(37919, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\achievement_character_nightelf_female:20:20:-10:0|t" },
				{ 6, "Gnome male",					FakeMorphData(37913, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\achievement_character_gnome_male:20:20:-10:0|t" },
				{ 7, "Gnome female",				FakeMorphData(20320, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\achievement_character_gnome_female:20:20:-10:0|t" },
				{ 8, "Draenei male",				FakeMorphData(37916, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\achievement_character_draenei_male:20:20:-10:0|t" },
				{ 9, "Draenei female",				FakeMorphData(20323, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\achievement_character_draenei_female:20:20:-10:0|t" },
				{ 10, "Worgen male",				FakeMorphData(37915, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\inv_mask_08:20:20:-10:0|t" },
				{ 11, "Worgen female",				FakeMorphData(37914, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\inv_mask_09:20:20:-10:0|t" },

				{ 12, "Orc male",					FakeMorphData(37920, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\achievement_character_orc_male:20:20:-10:0|t"},
				{ 13, "Orc female",					FakeMorphData(20316, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\achievement_character_orc_female:20:20:-10:0|t"},
				{ 14, "Undead male",				FakeMorphData(37923, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\achievement_character_undead_male:20:20:-10:0|t"},
				{ 15, "Undead female",				FakeMorphData(37924, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\achievement_character_undead_female:20:20:-10:0|t"},
				{ 16, "Tauren male",				FakeMorphData(20585, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\achievement_character_tauren_male:20:20:-10:0|t"},
				{ 17, "Tauren female",				FakeMorphData(37921, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\achievement_character_tauren_female:20:20:-10:0|t"},
				{ 18, "Troll male",					FakeMorphData(20321, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\achievement_character_troll_male:20:20:-10:0|t"},
				{ 19, "Troll female",				FakeMorphData(37922, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\achievement_character_troll_female:20:20:-10:0|t"},
				{ 20, "Blood elf male",				FakeMorphData(30087, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\achievement_character_bloodelf_male:20:20:-10:0|t"},
				{ 21, "Blood elf female",			FakeMorphData(20579, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\achievement_character_bloodelf_female:20:20:-10:0|t"},
				{ 22, "Goblin male",				FakeMorphData(37927, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\inv_mask_10:20:20:-10:0|t"},
				{ 23, "Goblin female",				FakeMorphData(37928, 0, 0, 0, 0, 0, 0, 0.f),	"|TInterface\\icons\\inv_mask_07:20:20:-10:0|t"},

				{ 24, "Gib the Banana-Hoarder",		FakeMorphData(39549, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 25, "Jimmy",						FakeMorphData(251, 0, 0, 0, 0, 0, 1, 0.f),		"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 26, "Anastaisa",					FakeMorphData(11873, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 27, "Gnomey",						FakeMorphData(31020, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 28, "King Mukla",					FakeMorphData(792, 0, 0, 0, 0, 0, 1, 0.f),		"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 29, "Basalt Rockling",			FakeMorphData(30537, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 30, "Guardian of Ancient Kings",	FakeMorphData(35573, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 31, "Valk'yre",					FakeMorphData(26077, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 32, "The Lich King",				FakeMorphData(24191, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 33, "Varian Wrynn",				FakeMorphData(28127, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 34, "Garrosh Hellscream",			FakeMorphData(32904, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 35, "Baine Bloodhoof",			FakeMorphData(30272, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 36, "Sylvanas Windrunner",		FakeMorphData(28213, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 37, "Vol'jin",					FakeMorphData(31736, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 38, "Lor'themar Theron",			FakeMorphData(17122, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 39, "Neltharion (humanoid)",		FakeMorphData(35435, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 40, "Thrall",						FakeMorphData(39145, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 41, "Alexstrasza",				FakeMorphData(39392, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 42, "Crab",						FakeMorphData(699, 0, 0, 0, 0, 0, 1, 0.f),		"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 43, "Spider",						FakeMorphData(539, 0, 0, 0, 0, 0, 1, 0.f),		"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 44, "Ethereal",					FakeMorphData(38804, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 45, "Snow Moonkin",				FakeMorphData(12237, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 46, "Pug",						FakeMorphData(31174, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 47, "Stormwind Guard",			FakeMorphData(3167, 0, 0, 0, 0, 0, 1, 0.f),		"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 48, "Orgrimmar Grunt",			FakeMorphData(4602, 0, 0, 0, 0, 0, 1, 0.f),		"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 49, "Mature Swine",				FakeMorphData(704, 0, 0, 0, 0, 0, 1, 0.f),		"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 50, "Shadowhorn Stag",			FakeMorphData(1992, 0, 0, 0, 0, 0, 1, 0.f),		"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 51, "Hogger",						FakeMorphData(384, 0, 0, 0, 0, 0, 1, 0.f),		"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 52, "Old Murk-Eye",				FakeMorphData(5243, 0, 0, 0, 0, 0, 1, 0.f),		"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 53, "Awakened Ancient",			FakeMorphData(29238, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				
				{ 54, "Faceless One",				FakeMorphData(28003, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 55, "Keeper Marandis",			FakeMorphData(150, 0, 0, 0, 0, 0, 1, 0.f),		"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 56, "Treant",						FakeMorphData(2077, 0, 0, 0, 0, 0, 1, 0.f),		"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 57, "Forest Moth",				FakeMorphData(36956, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 58, "Felguard",					FakeMorphData(5084, 0, 0, 0, 0, 0, 1, 0.f),		"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				{ 59, "Turtle",						FakeMorphData(10947, 0, 0, 0, 0, 0, 1, 0.f),	"|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" },
				
			};
		}
};

class SelfFakeMorpher : public CreatureScript
{
	public:
		SelfFakeMorpher() : CreatureScript("Creature_FakeMorph") { }
		
		bool OnGossipHello(Player* player, Creature* creature) override
		{

			if (player->fakeSelfMorph.scale == 0.f)
				player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\Spell_ChargeNegative:20:20:-10:0|t|TInterface\\icons\\Spell_ChargePositive:20:20:-10:0|t Change size/height scale\n", GOSSIP_SENDER_MAIN, 8);
			else
				player->ADD_GOSSIP_ITEM_EXTENDED(-1, "|TInterface\\icons\\TRADE_ENGINEERING:20:20:-10:0|tReset scale changes (free)\n", 6, 0, "|TInterface\\icons\\TRADE_ENGINEERING:20:20:-10:0|tAre you sure you want to reset your scale?", 0, false);

			if (player->fakeSelfMorph.displayid == 0)
			{
				if (player->GetOTeam() == ALLIANCE)//
					player->ADD_GOSSIP_ITEM_EXTENDED(-1, "|TInterface\\icons\\INV_BannerPVP_01:20:20:-10:0|t|TInterface\\icons\\Spell_Shadow_SacrificialShield:20:20:-10:0|tDisable crossfaction morph (free)\n", 5, 0, "Are you sure you want to disable your crossfaction morphing?", 0, false);
				else
					player->ADD_GOSSIP_ITEM_EXTENDED(-1, "|TInterface\\icons\\INV_BannerPVP_02:20:20:-10:0|t|TInterface\\icons\\Spell_Shadow_SacrificialShield:20:20:-10:0|tDisable crossfaction morph (free)\n", 5, 0, "Are you sure you want to disable your crossfaction morphing?", 0, false);

				player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\pvpcurrency-honor-alliance:20:20:-10:0|tAlliance races", GOSSIP_SENDER_MAIN, 1);
				player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\pvpcurrency-honor-horde:20:20:-10:0|tHorde races", GOSSIP_SENDER_MAIN, 2);
				player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\Ability_Hunter_Pet_Assist:20:20:-10:0|tNPC morphs", GOSSIP_SENDER_MAIN, 3);
				player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\INV_MISC_QUESTIONMARK:20:20:-10:0|tInput model number", GOSSIP_SENDER_MAIN, 6);
				player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\INV_MISC_QUESTIONMARK:20:20:-10:0|tInput NPC entry ID", GOSSIP_SENDER_MAIN, 7);
			}
			else
				player->ADD_GOSSIP_ITEM_EXTENDED(-1, "|TInterface\\icons\\TRADE_ENGINEERING:20:20:-10:0|tReset morphing changes (free)\n", 4, 0, "|TInterface\\icons\\TRADE_ENGINEERING:20:20:-10:0|tAre you sure you want to remove your morph?", 0, false);



			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
			return true;
		}
		
		bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
		{
			player->PlayerTalkClass->ClearMenus();

			if (sender == 1)
			{
				switch (action)
				{
				case 1:

					if (!player->isGameMaster())
					if (player->GetMoney() < uint64(goldCost))
					{
						player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, creature, 0, 0);
						player->CLOSE_GOSSIP_MENU();
						return true;
					}

					for (auto i = 0; i <= 11; ++i)
						if (auto data = GetMorphData(i))
						{
							std::ostringstream sure_stream;
							sure_stream << are_you_sure << data->name << "?";
							auto sure_check{ sure_stream.str() };


							std::ostringstream listing;
							listing << data->icon << " " << data->name;
							auto listing_str{ listing.str() };

							player->ADD_GOSSIP_ITEM_EXTENDED(-1, listing_str, 2, data->index, sure_check, player->isGameMaster() ? 0 :goldCost, false);
						}

					player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\misc_arrowleft:20:20:-10:0|tBack", GOSSIP_SENDER_MAIN, 4);
					player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
					break;
				case 2:
					if (!player->isGameMaster())
					if (player->GetMoney() < uint64(goldCost))
					{
						player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, creature, 0, 0);
						player->CLOSE_GOSSIP_MENU();
						return true;
					}

					for (auto i = 12; i <= 23; ++i)
						if (auto data = GetMorphData(i))
						{
							std::ostringstream sure_stream;
							sure_stream << are_you_sure << data->name << "?";
							auto sure_check{ sure_stream.str() };


							std::ostringstream listing;
							listing << data->icon << " " << data->name;
							auto listing_str{ listing.str() };

							player->ADD_GOSSIP_ITEM_EXTENDED(-1, listing_str, 2, data->index, sure_check, player->isGameMaster() ? 0 : goldCost, false);
						}

					player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\misc_arrowleft:20:20:-10:0|tBack", GOSSIP_SENDER_MAIN, 4);
					player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
					break;
				case 3:

					if (!player->isGameMaster())
					if (player->GetMoney() < uint64(goldCost))
					{
						player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, creature, 0, 0);
						player->CLOSE_GOSSIP_MENU();
						return true;
					}

					if (auto select = RAND(24,40))
					for (auto i = select; i <= (select+19); ++i)
						if (auto data = GetMorphData(i))
						{
							std::ostringstream sure_stream;
							sure_stream << are_you_sure << data->name << "?";
							auto sure_check{ sure_stream.str() };


							std::ostringstream listing;
							listing << data->icon << " " << data->name;
							auto listing_str{ listing.str() };

							player->ADD_GOSSIP_ITEM_EXTENDED(-1, listing_str, 2, data->index, sure_check, player->isGameMaster() ? 0 : goldCost, false);
						}

					player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\misc_arrowleft:20:20:-10:0|tBack", GOSSIP_SENDER_MAIN, 4);
					player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
					break;
				case 4:
					OnGossipHello(player, creature);
					break;
				case 6:
					if (!player->isGameMaster())
					if (player->GetMoney() < uint64(goldCost))
					{
						player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, creature, 0, 0);
						player->CLOSE_GOSSIP_MENU();
						return true;
					}

					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, "Enter Model ID\n\nTo ensure client safety, you will receive a 5 second preview of this model before confirmation.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1, "", 0, true);
					player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\misc_arrowleft:20:20:-10:0|tBack", GOSSIP_SENDER_MAIN, 4);
					player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
					break;
				case 7:
					if (!player->isGameMaster())
					if (player->GetMoney() < uint64(goldCost))
					{
						player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, creature, 0, 0);
						player->CLOSE_GOSSIP_MENU();
						return true;
					}
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, "Enter the ID of an NPC\n\nTo ensure client safety, you will receive a 5 second preview of this model before confirmation.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2, "", 0, true);
					player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\misc_arrowleft:20:20:-10:0|tBack", GOSSIP_SENDER_MAIN, 4);
					player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
					break;
				case 8:
					if (!player->isGameMaster())
						if (player->GetMoney() < uint64(goldCost/10))
						{
							player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, creature, 0, 0);
							player->CLOSE_GOSSIP_MENU();
							return true;
						}
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, "Enter the new scale you wish to use.\n\nNormally this value is 1.0\n\nexample:\n5.0 will increase your size to 500%.\n\nexample:\n0.2 will reduce your size to 20%.\n\nMaximum scale is 25.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3, "", 0, true);
					player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\misc_arrowleft:20:20:-10:0|tBack", GOSSIP_SENDER_MAIN, 4);
					player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
					break;
				}
			}
			else if (sender == 2 && action >= 0 && action < FakeMorphs.size())
			{
				std::string raceName;

				if (auto data = GetMorphData(action))
				{
					std::stringstream ss;
					ss << "You are now morphed into a " << data->name;
					std::string cMsg = ss.str();
					auto morph = FakeMorphs[action].data;
					morph.scale = player->fakeSelfMorph.scale;

					player->SetfakeSelfMorph(morph);
					creature->MonsterWhisper(cMsg.c_str(), player->GetGUID(), true);
					creature->MonsterWhisper("Remember, other players will continue to see you as normal.", player->GetGUID(), false);
					player->ModifyMoney(-goldCost);
				}

				player->CLOSE_GOSSIP_MENU();
			}
			else if (sender == 4)
			{
				if (player->fakeSelfMorph.displayid)
				{
					player->SetfakeSelfMorph(FakeMorphData(0, 0, 0, 0, 0, 0, 0, player->fakeSelfMorph.scale));
					creature->MonsterWhisper("Morph successfully removed", player->GetGUID(), true);
				}
				else
					creature->MonsterWhisper("You don't have a morph.", player->GetGUID(), false);
				player->CLOSE_GOSSIP_MENU();
			}
			else if (sender == 6)
			{
				if (player->fakeSelfMorph.scale != 0.f)
				{
					auto new_morph = player->fakeSelfMorph;
					new_morph.scale = 0.f;

					player->SetfakeSelfMorph(new_morph);
					creature->MonsterWhisper("Scale adjustment removed", player->GetGUID(), true);
				}
				else
					creature->MonsterWhisper("You don't have a morph.", player->GetGUID(), false);
				player->CLOSE_GOSSIP_MENU();
			}
			else if (sender == 5)
			{
				uint8 hair{ 0 };
				uint8 skin{ 0 };
				uint8 facial{ 0 };
				uint8 face{ 0 };
				uint8 haircolor{ 0 };

				player->GetCustomizationSettings(hair, skin, facial, face, haircolor);
					player->SetfakeSelfMorph(FakeMorphData(player->GetNativeDisplayId(), skin, face, hair, haircolor, facial, uint8(2), player->fakeSelfMorph.scale));

					creature->MonsterWhisper(player->GetOTeam() == HORDE ? "|TInterface\\icons\\INV_BannerPVP_01:20:20:-10:0|t|TInterface\\icons\\INV_BannerPVP_02:20:20:-10:0|t Your crossfaction morphing is now hidden.\nYou will no longer see your character as transformed while playing for the Alliance." : "|TInterface\\icons\\INV_BannerPVP_01:20:20:-10:0|t|TInterface\\icons\\INV_BannerPVP_02:20:20:-10:0|t  Your crossfaction morphing is now hidden.\nYou will no longer see your character as transformed while playing for the Horde.", player->GetGUID(), true);
					creature->MonsterWhisper("Remember, other players will continue to see you as normal.", player->GetGUID(), false);
				player->CLOSE_GOSSIP_MENU();
			}
			else if (sender == 8 && action == 1002)
			{
				player->SetfakeSelfMorph(player->preview_morph);
				creature->MonsterWhisper("You are now morphed into your specified model.", player->GetGUID(), true);
				creature->MonsterWhisper("Remember, other players will continue to see you as normal.", player->GetGUID(), false);
				if (!player->isGameMaster())
				player->ModifyMoney(-goldCost);
				player->CLOSE_GOSSIP_MENU();
			}
			else if (sender == 8 && action == 1003)
			{
				auto data = player->fakeSelfMorph;
				data.displayid = player->preview_morph.displayid;
				player->SetfakeSelfMorph(data);
				creature->MonsterWhisper("You are now morphed into your specified NPC.", player->GetGUID(), true);
				creature->MonsterWhisper("Remember, other players will continue to see you as normal.", player->GetGUID(), false);
				if (!player->isGameMaster())
				player->ModifyMoney(-goldCost);
				player->CLOSE_GOSSIP_MENU();
			}
			else if (sender == 8 && action == 1004)
			{

				auto data = player->fakeSelfMorph;
				data.scale = player->preview_morph.scale;

				player->SetfakeSelfMorph(data, false);

				std::stringstream ss;
				ss << "Your scale has been set to " << data.scale;
				std::string cMsg = ss.str();

				creature->MonsterWhisper(cMsg.c_str(), player->GetGUID(), true);
				creature->MonsterWhisper("Remember, other players will continue to see you as normal.", player->GetGUID(), false);
				player->ModifyMoney(-(goldCost / 10));
				player->CLOSE_GOSSIP_MENU();
			}
			return true;
		}


		bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, char const* code)
		{

			class DelayedMorphRemoval : public BasicEvent
			{
			public:
				DelayedMorphRemoval(Player* p, Creature* c) :
					plr(p), creature(c)
				{
				}

				bool Execute(uint64 /*execTime*/, uint32 /*diff*/)
				{
					plr->player_is_previewing_morph_model = false;
					plr->ForceValuesUpdateAtIndex(UNIT_FIELD_DISPLAYID);
					plr->ForceValuesUpdateAtIndex(UNIT_FIELD_NATIVEDISPLAYID);
					plr->ForceValuesUpdateAtIndex(OBJECT_FIELD_SCALE_X);
					creature->MonsterWhisper("<Morph preview expired>", plr->GetGUID(), true);
					return true;
				}

			private:
				Player* plr;
				Creature* creature;
			};


			player->PlayerTalkClass->ClearMenus();
			//ChatHandler(player->GetSession()).PSendSysMessage("sender %u, action %u, code %u", sender, action, atoi(code));
			if (sender == GOSSIP_SENDER_MAIN)
			{
				switch (action)
				{
				case GOSSIP_ACTION_INFO_DEF + 1://input npc model id
				{
					if (strlen(code) > 5)
					{
						ChatHandler(player->GetSession()).PSendSysMessage("Invalid model id");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					uint32 model = atoi(code);

					player->SetfakeSelfMorph(FakeMorphData(model, 0, 0, 0, 0, 0, 1, player->fakeSelfMorph.scale), true);
					std::ostringstream sure_stream;
					sure_stream << are_you_sure << "model id no. " << player->preview_morph.displayid << "?";
					auto sure_check{ sure_stream.str() };


					std::ostringstream listing;
					listing << "|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" << "Model no. " << player->preview_morph.displayid;
					auto listing_str{ listing.str() };

					player->ADD_GOSSIP_ITEM_EXTENDED(-1, listing_str, 8, 1002, sure_check, player->isGameMaster() ? 0 : goldCost, false);
					player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\misc_arrowleft:20:20:-10:0|tBack", GOSSIP_SENDER_MAIN, 4);
					player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
					creature->MonsterWhisper("<5 second morph preview>", player->GetGUID(), true);

					/*
						5 seconds to preview the new morph (or crash a client if it's bad, before the appearance is permenantly applied)
					*/
					player->m_Events.AddEvent(new DelayedMorphRemoval(player, creature), player->m_Events.CalculateTime(5000));
					return true;
					break;
				}
				case GOSSIP_ACTION_INFO_DEF + 2://input npc id
				{
					if (strlen(code) > 6)
					{
						ChatHandler(player->GetSession()).PSendSysMessage("Invalid NPC id");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					uint32 entry = atoi(code);
					if (auto cTemplate = sObjectMgr->GetCreatureTemplate(entry))
					{
						auto model = cTemplate->Modelid1;

						player->SetfakeSelfMorph(FakeMorphData(model, 0, 0, 0, 0, 0, 1, player->fakeSelfMorph.scale), true);
						std::ostringstream sure_stream;
						sure_stream << are_you_sure << cTemplate->Name << "?";
						auto sure_check{ sure_stream.str() };

						std::ostringstream listing;
						listing << "|TInterface\\icons\\INV_Misc_GroupLooking:20:20:-10:0|t" << cTemplate->Name << " - Entry " << code;
						auto listing_str{ listing.str() };

						player->ADD_GOSSIP_ITEM_EXTENDED(-1, listing_str, 8, 1003, sure_check, player->isGameMaster() ? 0 : goldCost, false);
						player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\misc_arrowleft:20:20:-10:0|tBack", GOSSIP_SENDER_MAIN, 4);
						player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
						creature->MonsterWhisper("<5 second morph preview>", player->GetGUID(), true);

						/*
							5 seconds to preview the new morph (or crash a client if it's bad, before the appearance is permenantly applied)
						*/
						player->m_Events.AddEvent(new DelayedMorphRemoval(player, creature), player->m_Events.CalculateTime(5000));
						return true;
					}
					else
					{

						std::ostringstream sure_stream;
						sure_stream << "Could not find data for NPC ID #" << code;
						auto sure_check{ sure_stream.str() };
						auto sure_cstr { sure_check.c_str()};

						creature->MonsterWhisper(sure_cstr, player->GetGUID(), true);
						player->CLOSE_GOSSIP_MENU();
						return true;
					}
					break;
				}
				case GOSSIP_ACTION_INFO_DEF + 3://input scale
				{
					if (strlen(code) > 6)
					{
						ChatHandler(player->GetSession()).PSendSysMessage("Invalid input");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					float scale{ 0.f };

					scale = atof(code);
					
					if (scale != 0.f)
						if (scale > 0.f && scale <= 25.f && scale)
						{

							auto new_morph = player->preview_morph;
							new_morph.scale = scale;

							player->SetfakeSelfMorph(new_morph, true);

							std::ostringstream sure_stream;
							sure_stream << "Keep model scale =" << scale << " ? ";
							auto sure_check{ sure_stream.str() };


							std::ostringstream listing;
							listing << "|TInterface\\icons\\Spell_ChargeNegative:20:20:-10:0|t|TInterface\\icons\\Spell_ChargePositive:20:20:-10:0|t  Keep scale: " << scale;
							// << "\n\nNote: To prevent abuse potential, this scale will not be active inside PvP instances.";
							// << "\n\nAbusing this feature will be treated as normal client-side hacking."
							auto listing_str{ listing.str() };

							player->ADD_GOSSIP_ITEM_EXTENDED(-1, listing_str, 8, 1004, sure_check, player->isGameMaster() ? 0 : goldCost/10, false);
							player->ADD_GOSSIP_ITEM(-1, "|TInterface\\icons\\misc_arrowleft:20:20:-10:0|t Back", GOSSIP_SENDER_MAIN, 4);
							player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
							creature->MonsterWhisper("<5 second morph preview>", player->GetGUID(), true);

							/*
								5 seconds to preview the new morph (or crash a client if it's bad, before the appearance is permenantly applied)
							*/
							player->m_Events.AddEvent(new DelayedMorphRemoval(player, creature), player->m_Events.CalculateTime(5000));
							return true;
						}
						else
						{
							ChatHandler(player->GetSession()).PSendSysMessage("Invalid input");
							player->CLOSE_GOSSIP_MENU();
						}
					else
					{
						ChatHandler(player->GetSession()).PSendSysMessage("Invalid input");
						player->CLOSE_GOSSIP_MENU();
					}
						return true;
						
						break;
				}
			}
			}
			return true;
		}
};

class SelfFakeMorph_player : public PlayerScript
{
	public:
		SelfFakeMorph_player() : PlayerScript("Player_FakeMorph") { }
		
		void OnSave(Player* player) override
		{
			if (player->fakeSelfMorph.displayid || player->fakeSelfMorph.scale != 0.f)
				CharacterDatabase.PExecute("REPLACE INTO `custom_fakemorphs` (`playterguid`, `displayid`, `skin`, `face`, `hairstyle`, `haircolor`, `facialhair`, `category`, `scale`) VALUES (%u, %u, %u, %u, %u, %u, %u, %u, %f)",
				player->GetGUIDLow(), player->fakeSelfMorph.displayid, player->fakeSelfMorph.skin, player->fakeSelfMorph.face, player->fakeSelfMorph.hairstyle, player->fakeSelfMorph.haircolor, player->fakeSelfMorph.facialhair, player->fakeSelfMorph.category, player->fakeSelfMorph.scale);
			else
				CharacterDatabase.PExecute("DELETE FROM `custom_fakemorphs` WHERE `playterguid` = %u", player->GetGUIDLow());
		}
		
		void OnLogin(Player* player) override
		{
			QueryResult result = CharacterDatabase.PQuery("SELECT `displayid`, `skin`, `face`, `hairstyle`, `haircolor`, `facialhair`, `category`, `scale` FROM `custom_fakemorphs` WHERE `playterguid` = %u", player->GetGUIDLow());
			if (!result)
				return;

			Field* field = result->Fetch();
			uint8 i = 0;
			uint32 display = field[i++].GetUInt32();
			uint8 skin = field[i++].GetUInt8();
			uint8 face = field[i++].GetUInt8();
			uint8 hairstyle = field[i++].GetUInt8();
			uint8 haircolor = field[i++].GetUInt8();
			uint8 facialhair = field[i++].GetUInt8();
			uint8 category = field[i++].GetUInt8();
			float scale = field[i++].GetFloat();
			player->SetfakeSelfMorph(FakeMorphData(display, skin, face, hairstyle, haircolor, facialhair, category, scale));
		}
};

void AddSC_SelfFakeMorpher()
{
	new SelfFakeMorph_world();
	new SelfFakeMorph_player();
	new SelfFakeMorpher();
}