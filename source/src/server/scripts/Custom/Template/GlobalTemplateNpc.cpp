#include "ScriptPCH.h"
#include "Template.h"
#include "Player.h"

enum GossipOptions
{
	GOSSIP_SENDER_HELLO,
	GOSSIP_SPEC_SELECT_REMEDY,
	GOSSIP_SPEC_SELECT_PUBLIC,
	GOSSIP_SPEC_ACTION,
	GOSSIP_SPEC_APPLY_TALENTS,
	GOSSIP_SPEC_APPLY_GEAR,
	GOSSIP_SPEC_APPLY_BOTH
};

enum GossipActions
{
	ACTION_VIEW_HELLO,
	ACTION_VIEW_REMEDY,
	ACTION_VIEW_PUBLIC
};

class global_template_npc : public CreatureScript
{
public:
	global_template_npc() : CreatureScript("global_template_npc") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		//player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Choose:", GOSSIP_SENDER_HELLO, ACTION_VIEW_HELLO);

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\ICONS\\misc_arrowright:30:30|t Quick Start Templates", GOSSIP_SENDER_HELLO, ACTION_VIEW_REMEDY);

		if (sTemplateMgr->m_publishedTemplates[player->getClass()].size() > 0)
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\ICONS\\misc_arrowright:30:30|t Player Published Templates", GOSSIP_SENDER_HELLO, ACTION_VIEW_PUBLIC);

		player->SEND_GOSSIP_MENU(60025, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 uiAction)
	{
		player->PlayerTalkClass->ClearMenus();

		if (!player || !creature)
			return true;

		switch (uiSender)
		{
		case GOSSIP_SENDER_HELLO:
			switch (uiAction)
			{
			case ACTION_VIEW_HELLO:
				OnGossipHello(player, creature);
				return true;
			case ACTION_VIEW_REMEDY:
				if (!player->HasItemCount(100005, 1, false))
				{
					creature->MonsterWhisper("You don't have a Character Template Token!", player->GetGUID(), false);
					return false;
				}
				if (player->getLevel() != 85)
				{
					creature->MonsterWhisper("You must be level 85 before using the template npc!", player->GetGUID(), false);
					return false;
				}
				else
					ShowSpecMenu(player, creature, ACTION_VIEW_REMEDY);
				break;
			case ACTION_VIEW_PUBLIC:
				ShowSpecMenu(player, creature, ACTION_VIEW_PUBLIC);
				break;
			default:
				ShowErrorScreen(player, creature, GOSSIP_SENDER_HELLO, 0, "Something went wrong. Please report the issue to a GM. Error code: " + std::to_string(uiSender) + "." + std::to_string(uiAction) + ".");
				break;
			}
			break;
		case GOSSIP_SPEC_SELECT_REMEDY:
			ShowSpecTemplatesMenu(player, creature, uiAction, ACTION_VIEW_REMEDY);
			break;
		case GOSSIP_SPEC_SELECT_PUBLIC:
			ShowSpecTemplatesMenu(player, creature, uiAction, ACTION_VIEW_PUBLIC);
			break;
		case GOSSIP_SPEC_ACTION:
			ShowSpecActionsMenu(player, creature, uiAction);
			break;
		case GOSSIP_SPEC_APPLY_TALENTS:
			ApplyTemplate(player, uiAction, true, false);
			break;
		case GOSSIP_SPEC_APPLY_GEAR:
			ApplyTemplate(player, uiAction, false, true);
			break;
		case GOSSIP_SPEC_APPLY_BOTH:
			ApplyTemplate(player, uiAction, true, true);
			break;
		default:
			ShowErrorScreen(player, creature, GOSSIP_SENDER_HELLO, 0, "Something went wrong. Please report the issue to a GM. Error code: " + std::to_string(uiSender) + "." + std::to_string(uiAction) + ".");
			break;
		}
		return true;
	}

	void ShowSpecMenu(Player* player, Creature* creature, uint8 action)
	{
		uint8 gossipOption = action == ACTION_VIEW_REMEDY ? GOSSIP_SPEC_SELECT_REMEDY : GOSSIP_SPEC_SELECT_PUBLIC;

		ClassSpecs specs = sTemplateMgr->GetClassSpecs(player->getClass());
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|" + specs[0][1] + " View " + specs[0][0] + " Templates", gossipOption, 0);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|" + specs[1][1] + " View " + specs[1][0] + " Templates", gossipOption, 1);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|" + specs[2][1] + " View " + specs[2][0] + " Templates", gossipOption, 2);

		// GossipHello is skipped if no published templates exist, so this becomes the front page and doesnt need a back button
		if (sTemplateMgr->m_publishedTemplates[player->getClass()].size() > 0)
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\ICONS\\misc_arrowleft:30:30|t Back", GOSSIP_SENDER_HELLO, ACTION_VIEW_HELLO);
		player->SEND_GOSSIP_MENU(60025, creature->GetGUID());
	}

	void ShowSpecTemplatesMenu(Player* player, Creature* creature, uint32 specIndex, uint8 action)
	{
		ServerTemplates templates;
		uint8 currentGossip;

		// Aps: Get all templates for this spec
		ClassSpecs specs = sTemplateMgr->GetClassSpecs(player->getClass());
		std::string spec = specs[specIndex][0];

		if (action == ACTION_VIEW_REMEDY)
		{
			templates = sTemplateMgr->m_ServerTemplates[player->getClass()][spec];
			currentGossip = GOSSIP_SPEC_SELECT_REMEDY;
		}
		else if (action == ACTION_VIEW_PUBLIC)
		{
			templates = sTemplateMgr->m_publishedTemplates[player->getClass()][spec];
			currentGossip = GOSSIP_SPEC_SELECT_PUBLIC;
		}

		for (int i = 0; i < MAX_CATEGORIES; i++)
		{
			std::string title = i == CATEGORY_PVP ? "PvP Templates:" : "----------------------------------------------\nPvE Templates:";
			// Print title
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, title, action, specIndex);
			// Check if templates exist
			if (templates[i].size() < 1)
			{
				std::string category = i == CATEGORY_PVP ? "PvP" : "PvE";
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Unfortunately there are no " + category + " templates available for this Specialisation.", currentGossip, specIndex);
			}
			else
			{
				// Print the templates
				for (auto& m_template : templates[i])
				{
					std::string specIcon = "";
					// find the spec icon
					ClassSpecs specs = sTemplateMgr->GetClassSpecs(player->getClass());
					for (std::vector<std::string> spec : specs)
					{
						if (spec[0] == m_template.second.spec)
						{
							specIcon = spec[1];
						}
					}
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|" + specIcon + " " + m_template.second.name, GOSSIP_SPEC_ACTION, m_template.second.id);
				}
			}
		}
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\ICONS\\misc_arrowleft:30:30|t Back", GOSSIP_SENDER_HELLO, action);
		player->SEND_GOSSIP_MENU(60025, creature->GetGUID());
	}

	void ShowSpecActionsMenu(Player* player, Creature* creature, uint32 template_id)
	{
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\ICONS\\misc_arrowright:30:30|t Talents Only", GOSSIP_SPEC_APPLY_TALENTS, template_id);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\ICONS\\misc_arrowright:30:30|t Gear Only", GOSSIP_SPEC_APPLY_GEAR, template_id);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\ICONS\\misc_arrowright:30:30|t Both", GOSSIP_SPEC_APPLY_BOTH, template_id);

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\ICONS\\misc_arrowleft:30:30|t Back", GOSSIP_SENDER_HELLO, 0);
		player->SEND_GOSSIP_MENU(60025, creature->GetGUID());
	}

	void ApplyTemplate(Player* player, uint32 template_id, bool learnTalents, bool equipGear)
	{
		PlayerTemplate mTemplate = sTemplateMgr->GetTemplate(template_id);
		sTemplateMgr->ApplyTemplate(player, template_id, learnTalents, equipGear);
		player->CLOSE_GOSSIP_MENU();

		player->UpdateSkillsForLevel();
		player->SaveToDB();
	}

	void ShowErrorScreen(Player* player, Creature* creature, uint32 uiSender, uint32 uiAction, std::string error)
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, error, uiSender, uiAction);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\ICONS\\misc_arrowleft:30:30|t Back", uiSender, uiAction);
		player->SEND_GOSSIP_MENU(60025, creature->GetGUID());
	}

};

void AddSC_GlobalTemplates()
{
	new global_template_npc();
}