/*
*	Author: Aps
*
* Explanations for future reference:
*
* - Custom Character Templates
* Each account can have up to 10 templates for each class. These templates are cross-faction since they are only talent and glyph templates.
* A player is required to name his templates, and can replace them by saving a new template with the same name.
* A player can delete, publish or make a previously published template private.
* A player can only have 1 template for each spec public at a time. Attempting to publish a second template of the same spec will cause the old one to become private.
* Players can only see templates for their current class in all NPC gossips.
*
*/
#include "ScriptPCH.h"
#include "Template.h"
#include "Player.h"

enum TalentMenuGossips
{
	GOSSIP_SENDER_HELLO,
	GOSSIP_SENDER_DEFAULT,
	GOSSIP_SENDER_TEMPLATE_SELECT,
	GOSSIP_SENDER_SAVE,
	GOSSIP_SENDER_DELETE,
	GOSSIP_SENDER_NEW,
	GOSSIP_SENDER_CHOOSE_CATEGORY,
	GOSSIP_SENDER_MAKE_PUBLIC_PVP,
	GOSSIP_SENDER_MAKE_PUBLIC_PVE,
	GOSSIP_SENDER_MAKE_PRIVATE
};

enum TalentNpcOptions
{
	VIEW_SAVED_TEMPLATES,
	SAVE_CURRENT_TEMPLATE_CATEGORY,
	SAVE_CURRENT_TEMPLATE,
	DELETE_SAVED_TEMPLATE,
	MAKE_TEMPLATE_PUBLIC,
	MAKE_TEMPLATE_PRIVATE
};

class account_template_npc : public CreatureScript
{
public:
	account_template_npc() : CreatureScript("account_template_npc") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\ICONS\\misc_arrowright:30:30|t View your saved Templates", GOSSIP_SENDER_DEFAULT, VIEW_SAVED_TEMPLATES);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\ICONS\\misc_arrowright:30:30|t Save your current Template", GOSSIP_SENDER_DEFAULT, SAVE_CURRENT_TEMPLATE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\ICONS\\misc_arrowright:30:30|t Delete a saved Template", GOSSIP_SENDER_DEFAULT, DELETE_SAVED_TEMPLATE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\ICONS\\misc_arrowright:30:30|t Make a Template public", GOSSIP_SENDER_DEFAULT, MAKE_TEMPLATE_PUBLIC);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\ICONS\\misc_arrowright:30:30|t Make a Template private", GOSSIP_SENDER_DEFAULT, MAKE_TEMPLATE_PRIVATE);
		player->SEND_GOSSIP_MENU(60025, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 uiAction)
	{
		player->PlayerTalkClass->ClearMenus();
		switch (uiSender)
		{
		case GOSSIP_SENDER_HELLO:
			OnGossipHello(player, creature);
			return true;
		case GOSSIP_SENDER_DEFAULT:
			switch (uiAction)
			{
			case VIEW_SAVED_TEMPLATES: // View Saved Templates
				MenuSavedSpecs(player, creature);
				break;
			case SAVE_CURRENT_TEMPLATE_CATEGORY: // Choose category to save Template
				ChoseCategory(player, creature, uiAction);
				break;
			case SAVE_CURRENT_TEMPLATE: // Save Current Template
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Please provide a name for your template. Note that if you use the same name for a second time, your current template will be replaced.", GOSSIP_SENDER_DEFAULT, 2);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, "|TInterface\\ICONS\\inv_inscription_majorglyph16:30:30|t Click here to enter a name...", GOSSIP_SENDER_NEW, 0, "", 0, true);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\ICONS\\misc_arrowleft:30:30|t Back", GOSSIP_SENDER_HELLO, 0);
				player->SEND_GOSSIP_MENU(60025, creature->GetGUID());
				break;
			case DELETE_SAVED_TEMPLATE: // Delete Saved Template
				MenuDeleteTemplate(player, creature);
				break;
			case MAKE_TEMPLATE_PUBLIC: // Make Public
				MenuMakePublic(player, creature);
				break;
			case MAKE_TEMPLATE_PRIVATE: // Make Private
				MenuMakePrivate(player, creature);
				break;
			default:
				player->GetSession()->SendAreaTriggerMessage("Something went wrong. sender: %u, action: %u", uiSender, uiAction);
				break;
			}
			break;
		case GOSSIP_SENDER_TEMPLATE_SELECT:
			ApplyTemplate(player, creature, uiAction);
			break;
		case GOSSIP_SENDER_DELETE:
			DeleteTemplate(player, creature, uiAction);
			break;
		case GOSSIP_SENDER_SAVE:
			sTemplateMgr->ApplyTemplateForPlayer(player, uiAction);
			// Let player know his template was applied
			player->GetSession()->SendAreaTriggerMessage("Template Applied!");
			// Close the NPC gossip menu
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_SENDER_CHOOSE_CATEGORY:
			ChoseCategory(player, creature, uiAction);
			break;
		case GOSSIP_SENDER_MAKE_PUBLIC_PVP:
			MakePublic(player, creature, CATEGORY_PVP, uiAction);
			break;
		case GOSSIP_SENDER_MAKE_PUBLIC_PVE:
			MakePublic(player, creature, CATEGORY_PVE, uiAction);
			break;
		case GOSSIP_SENDER_MAKE_PRIVATE:
			MakePrivate(player, creature, uiAction, true);
			break;
		default:
			ShowErrorScreen(player, creature, GOSSIP_SENDER_HELLO, 0, "Something went wrong. Please report the issue to a GM. Error code: " + std::to_string(uiSender) + "." + std::to_string(uiAction) + ".");
			break;
		}
		return true;
	}

	bool OnGossipSelectCode(Player* player, Creature* creature, uint32 uiSender, uint32 uiAction, char const* code)
	{
		player->PlayerTalkClass->ClearMenus();
		switch (uiSender)
		{
		case GOSSIP_SENDER_NEW:
			SaveNewTemplate(player, creature, code);
			break;
		default:
			player->GetSession()->SendAreaTriggerMessage("Something went wrong. Error code: %u.%u.", uiSender, uiAction);
			break;
		}
		return true;
	}

	void MenuSavedSpecs(Player* player, Creature* creature)
	{
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Click on the Template you wish to apply:", GOSSIP_SENDER_DEFAULT, VIEW_SAVED_TEMPLATES);

		if (player->m_templates.size() == 0)
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "You do not have any saved templates for this class.", GOSSIP_SENDER_DEFAULT, VIEW_SAVED_TEMPLATES);
		}
		else
		{
			for (auto& playerTemplate : player->m_templates)
			{
				std::string specIcon = "";
				// find the spec icon
				ClassSpecs specs = sTemplateMgr->GetClassSpecs(player->getClass());
				for (std::vector<std::string> spec : specs)
				{
					if (spec[0] == playerTemplate.second.spec)
					{
						specIcon = spec[1];
						break;
					}
				}
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|" + specIcon + " " + playerTemplate.second.name, GOSSIP_SENDER_TEMPLATE_SELECT, playerTemplate.second.id);
			}
		}

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\ICONS\\misc_arrowleft:30:30|t Back", GOSSIP_SENDER_HELLO, 0);
		player->SEND_GOSSIP_MENU(60025, creature->GetGUID());
	}

	void MenuDeleteTemplate(Player* player, Creature* creature)
	{
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Click on the Template you wish to delete:", GOSSIP_SENDER_DEFAULT, 3);
		if (player->m_templates.size() == 0)
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "You have no templates for this class", GOSSIP_SENDER_DEFAULT, 3);
		}
		else
		{
			for (auto& playerTemplate : player->m_templates)
			{
				std::string specIcon = "";
				// find the spec icon
				ClassSpecs specs = sTemplateMgr->GetClassSpecs(player->getClass());
				for (std::vector<std::string> spec : specs)
				{
					if (spec[0] == playerTemplate.second.spec)
					{
						specIcon = spec[1];
					}
				}
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|" + specIcon + " " + playerTemplate.second.name, GOSSIP_SENDER_DELETE, playerTemplate.second.id);
			}
		}
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\ICONS\\misc_arrowleft:30:30|t Back", GOSSIP_SENDER_HELLO, 0);
		player->SEND_GOSSIP_MENU(60025, creature->GetGUID());
	}

	void MenuMakePrivate(Player* player, Creature* creature)
	{
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Click on the Template you wish to make private:", GOSSIP_SENDER_DEFAULT, 4);

		if (player->m_templates.size() == 0)
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "You have no public templates for this class", GOSSIP_SENDER_DEFAULT, MAKE_TEMPLATE_PRIVATE);
		}
		else
		{
			for (auto& playerTemplate : player->m_templates)
			{
				// Don't show templates that are already private
				if (playerTemplate.second.isPublic)
				{
					std::string specIcon = "";
					// find the spec icon
					ClassSpecs specs = sTemplateMgr->GetClassSpecs(player->getClass());
					for (std::vector<std::string> spec : specs)
					{
						if (spec[0] == playerTemplate.second.spec)
						{
							specIcon = spec[1];
						}
					}
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|" + specIcon + " " + playerTemplate.second.name, GOSSIP_SENDER_MAKE_PRIVATE, playerTemplate.second.id);

				}
			}
		}
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\ICONS\\misc_arrowleft:30:30|t Back", GOSSIP_SENDER_HELLO, 0);
		player->SEND_GOSSIP_MENU(60025, creature->GetGUID());
	}

	void MenuMakePublic(Player* player, Creature* creature)
	{
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Please Note: You may only have one template of each specialisation public. If you chose to publish a specialisation that you already published, your old one will become private.", GOSSIP_SENDER_DEFAULT, 4);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Click on the Template you wish to make public:", GOSSIP_SENDER_DEFAULT, 4);

		bool no_templates = true;
		for (auto& playerTemplate : player->m_templates)
		{
			// Don't show templates that are already public
			if (!playerTemplate.second.isPublic)
			{
				std::string specIcon = "";
				// find the spec icon
				ClassSpecs specs = sTemplateMgr->GetClassSpecs(player->getClass());
				for (std::vector<std::string> spec : specs)
				{
					if (spec[0] == playerTemplate.second.spec)
					{
						specIcon = spec[1];
						no_templates = false;
					}
				}
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|" + specIcon + " " + playerTemplate.second.name, GOSSIP_SENDER_CHOOSE_CATEGORY, playerTemplate.second.id);
			}
		}
		if (no_templates)
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "You have no private templates for this class", GOSSIP_SENDER_DEFAULT, MAKE_TEMPLATE_PUBLIC);
		}
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\ICONS\\misc_arrowleft:30:30|t Back", GOSSIP_SENDER_HELLO, 0);
		player->SEND_GOSSIP_MENU(60025, creature->GetGUID());
	}

	void ChoseCategory(Player* player, Creature* creature, uint32 template_id)
	{
		PlayerTemplate chosenTemplate = player->GetTemplate(template_id);
		// No template found
		if (!chosenTemplate)
		{
			ShowErrorScreen(player, creature, GOSSIP_SENDER_DEFAULT, MAKE_TEMPLATE_PUBLIC, "We were unable to find the template you requested. If the problem persists please let us know.");
			return;
		}
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Please chose the category you would like to place this template under:", GOSSIP_SENDER_CHOOSE_CATEGORY, template_id);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\ICONS\\misc_arrowright:30:30|t PVP", GOSSIP_SENDER_MAKE_PUBLIC_PVP, template_id);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|TInterface\\ICONS\\misc_arrowright:30:30|t PVE", GOSSIP_SENDER_MAKE_PUBLIC_PVE, template_id);
		player->SEND_GOSSIP_MENU(60025, creature->GetGUID());
	}

	void DeleteTemplate(Player* player, Creature* creature, uint32 templateId)
	{
		for (auto& playerTemplate : player->m_templates)
		{
			if (playerTemplate.second.id == templateId)
			{
				sTemplateMgr->OverwriteTemplateForPlayer(player, sTemplateMgr->GetClassString(player->getClass()), playerTemplate.second.name);
				ShowErrorScreen(player, creature, GOSSIP_SENDER_DEFAULT, 3, "Template Deleted.");
				return;
			}
		}
	}

	void SaveNewTemplate(Player* player, Creature* creature, std::string name)
	{
		// Check if a name is entered
		if (name == "")
		{
			ShowErrorScreen(player, creature, GOSSIP_SENDER_DEFAULT, 2, "You must enter a name for your template.");
			return;
		}
		// Check if all talent points used
		if (player->GetFreeTalentPoints() > 0)
		{
			ShowErrorScreen(player, creature, GOSSIP_SENDER_NEW, 0, "You must use all your talent points before you can create a template.");
			return;
		}
		// Check if all glyphs are used
		for (int i = 0; i < MAX_GLYPH_SLOT_INDEX; i++)
		{
			if (uint32 oldGlyph = player->GetGlyph(player->GetActiveSpec(), i))
			{
				continue;
			}
			else
			{
				// Minor glyphs are not required
				if (i != 1 && i != 2 && i != 4)
				{
					ShowErrorScreen(player, creature, GOSSIP_SENDER_DEFAULT, 2, "You must use your Prime and Major glyphs before you can create a template.");
					return;
				}
			}
		}
		// Count the player's templates for this class
		int classCount = 0;
		for (auto m_template : player->m_templates)
		{
			if (m_template.second.class_id == player->getClass())
			{
				classCount++;
			}
		}
		// Player cannot have more than 10 templates for each class
		if (classCount >= 10)
		{
			ShowErrorScreen(player, creature, GOSSIP_SENDER_DEFAULT, 2, "You cannot have more than 10 templates for each class.");
			return;
		}
		// If all is good, save the new template (replace the old one if needed)
		sTemplateMgr->OverwriteTemplateForPlayer(player, sTemplateMgr->GetClassString(player->getClass()), name);
		PlayerTemplate newTemplate = sTemplateMgr->ExtractTalentTemplateForPlayer(player, sTemplateMgr->GetSpecNameFromTree(player->GetPrimaryTalentTree(player->GetActiveSpec())), name);
		// Add to queue to be stored in the db
		sTemplateMgr->newPlayerTemplatesContainer[player->GetSession()->GetAccountId()][newTemplate.id] = newTemplate;
		// Add to the global templates container
		sTemplateMgr->playerTemplatesContainer[player->GetSession()->GetAccountId()][newTemplate.id] = newTemplate;
		// Let player know his template was saved
		ShowErrorScreen(player, creature, GOSSIP_SENDER_HELLO, 0, "Template saved!");
	}

	void MakePublic(Player* player, Creature* creature, uint8 category, uint32 template_id)
	{
		uint32 account_id = player->GetSession()->GetAccountId();

		if (player->m_templates.find(template_id) != player->m_templates.end())
		{
			PlayerTemplate playerTemplate = player->m_templates[template_id];
			// If it's already public we don't need to do anything
			if (playerTemplate.isPublic)
				return;

			// Check if a player already has a template with this spec public
			if (uint32 currentPublicId = HasPublicSpec(player, playerTemplate.spec))
			{
				// We need to make that template private
				MakePrivate(player, creature, currentPublicId, false);
			}

			// if new templates for this player exist and the template we want to update only exists in memory
			if (sTemplateMgr->newPlayerTemplatesContainer.find(account_id) != sTemplateMgr->newPlayerTemplatesContainer.end() &&
				sTemplateMgr->newPlayerTemplatesContainer[account_id].find(template_id) != sTemplateMgr->newPlayerTemplatesContainer[account_id].end())
			{
				// in this case it's not saved yet so we can simply change it from here
				PlayerTemplate queuedTemplate = sTemplateMgr->newPlayerTemplatesContainer[account_id][template_id];

				queuedTemplate.category = category;
				queuedTemplate.isPublic = true;
				queuedTemplate.publisher = player->GetGUID();
				queuedTemplate.publisherName = player->GetName();
				sTemplateMgr->newPlayerTemplatesContainer[account_id][template_id] = queuedTemplate;

				playerTemplate = queuedTemplate;
			}
			// If a change isn't queued to be updated in the database then we need to add it
			else if (sTemplateMgr->updatedPlayerTemplatesContainer.find(account_id) == sTemplateMgr->updatedPlayerTemplatesContainer.end() ||
				sTemplateMgr->updatedPlayerTemplatesContainer[account_id].find(playerTemplate.id) == sTemplateMgr->updatedPlayerTemplatesContainer[account_id].end())
			{
				playerTemplate.category = category;
				playerTemplate.isPublic = true;
				playerTemplate.publisher = player->GetGUID();
				playerTemplate.publisherName = player->GetName();
				sTemplateMgr->updatedPlayerTemplatesContainer[account_id][playerTemplate.id] = playerTemplate;
			}
			// Else if it was already queued to be updated then it means this time we are reverting it back to its db value
			else
			{
				sTemplateMgr->updatedPlayerTemplatesContainer[account_id].erase(playerTemplate.id);
				playerTemplate.category = category;
				playerTemplate.isPublic = true;
				playerTemplate.publisher = player->GetGUID();
				playerTemplate.publisherName = player->GetName();
			}

			// Upate the containers
			player->m_templates[playerTemplate.id] = playerTemplate;
			sTemplateMgr->playerTemplatesContainer[account_id][playerTemplate.id] = playerTemplate;
			sTemplateMgr->m_publishedTemplates[playerTemplate.class_id][playerTemplate.spec][playerTemplate.category][playerTemplate.name] = playerTemplate;

			ShowErrorScreen(player, creature, GOSSIP_SENDER_DEFAULT, MAKE_TEMPLATE_PUBLIC, "Template Published.");
			return;
		}
		else
		{
			ShowErrorScreen(player, creature, GOSSIP_SENDER_DEFAULT, MAKE_TEMPLATE_PUBLIC, "We were unable to find the template you requested. If the problem persists please let us know.");
			return;
		}
	}

	void MakePrivate(Player* player, Creature* creature, uint32 templateId, bool withMessage)
	{
		uint32 account_id = player->GetSession()->GetAccountId();

		if (player->m_templates.find(templateId) != player->m_templates.end())
		{
			PlayerTemplate playerTemplate = player->m_templates[templateId];
			// If it's already private we don't need to do anything
			if (!playerTemplate.isPublic)
				return;

			// if new templates for this player exist and the template we want to update only exists in memory
			if (sTemplateMgr->newPlayerTemplatesContainer.find(account_id) != sTemplateMgr->newPlayerTemplatesContainer.end() &&
				sTemplateMgr->newPlayerTemplatesContainer[account_id].find(templateId) != sTemplateMgr->newPlayerTemplatesContainer[account_id].end())
			{
				// in this case it's not saved yet so we can simply change it from here
				PlayerTemplate queuedTemplate = sTemplateMgr->newPlayerTemplatesContainer[account_id][templateId];

				queuedTemplate.isPublic = false;
				queuedTemplate.publisher = 0;
				queuedTemplate.publisherName = "";
				sTemplateMgr->newPlayerTemplatesContainer[account_id][templateId] = queuedTemplate;

				playerTemplate = queuedTemplate;
			}
			// If a change isn't queued to be updated in the database then we need to add it
			else if (sTemplateMgr->updatedPlayerTemplatesContainer.find(account_id) == sTemplateMgr->updatedPlayerTemplatesContainer.end() ||
				sTemplateMgr->updatedPlayerTemplatesContainer[account_id].find(playerTemplate.id) == sTemplateMgr->updatedPlayerTemplatesContainer[account_id].end())
			{
				playerTemplate.isPublic = false;
				playerTemplate.publisher = 0;
				playerTemplate.publisherName = "";
				sTemplateMgr->updatedPlayerTemplatesContainer[account_id][playerTemplate.id] = playerTemplate;
			}
			// Else if it was already queued to be updated then it means this time we are reverting it back to its db value
			else
			{
				sTemplateMgr->updatedPlayerTemplatesContainer[account_id].erase(playerTemplate.id);
				playerTemplate.isPublic = false;
				playerTemplate.publisher = 0;
				playerTemplate.publisherName = "";
			}

			// Upate the containers
			player->m_templates[playerTemplate.id] = playerTemplate;
			sTemplateMgr->playerTemplatesContainer[account_id][playerTemplate.id] = playerTemplate;
			sTemplateMgr->m_publishedTemplates[playerTemplate.class_id][playerTemplate.spec][playerTemplate.category].erase(playerTemplate.name);

			// delete the spec if its empty
			bool specEmpty = true;
			for (auto& templateCategories : sTemplateMgr->m_publishedTemplates[playerTemplate.class_id][playerTemplate.spec])
			{
				for (auto& m_template : templateCategories.second)
				{
					specEmpty = false;
					break;
				}
			}
			if (specEmpty)
				sTemplateMgr->m_publishedTemplates[playerTemplate.class_id].erase(playerTemplate.spec);

			if (withMessage)
			{
				ShowErrorScreen(player, creature, GOSSIP_SENDER_DEFAULT, MAKE_TEMPLATE_PRIVATE, "Template is now Private.");
			}
		}
	}

	uint32 HasPublicSpec(Player* player, std::string spec)
	{
		uint32 templateId = 0;
		for (auto& m_myTemplate : player->m_templates)
		{
			PlayerTemplate myTemplate = m_myTemplate.second;
			if (myTemplate.class_id == player->getClass() && myTemplate.spec == spec && myTemplate.isPublic)
			{
				templateId = myTemplate.id;
				break;
			}
		}
		return templateId;
	}

	void ApplyTemplate(Player* player, Creature* creature, uint32 template_id)
	{
		sTemplateMgr->ApplyTemplateForPlayer(player, template_id);
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

void AddSC_AccountTemplates()
{
	new account_template_npc();
}