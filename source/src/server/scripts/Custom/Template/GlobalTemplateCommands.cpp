/*
* Revamped by Aps
*
* Explanations for future reference:
*
* Each unique spec can have multiple templates. Each template can have 1 talent template, 1 glyph template and multiple gear templates.
* Each gear template is identified and stored using the race id of the character that was used to extract it.
* If a template is extracted using the same name as an existing one for that spec, the system will only attempt to extract a new gear template, since talents and glyphs are common for all races.
* If a gear template for the active race does not exist it will be created and stored in the db. If it already exists, it will return an error.
* There is no way to delete individual gear templates. The entire template needs to be deleted and re-extracted.
*
*/
#include "ScriptPCH.h"
#include "Template.h"
#include "Player.h"

class template_commands : public CommandScript
{
public:
	template_commands() : CommandScript("template_commands") { }

	std::vector<ChatCommand> GetCommands() const override
	{
		static std::vector<ChatCommand> templateExtractTable =
		{
			{ "pvp", SEC_CONSOLE, false, &HandleCreatePvpTemplateCommand, "Extracts a pvp template from your character's current specialisation and gear with the defined name." },
			{ "pve", SEC_CONSOLE, false, &HandleCreatePveTemplateCommand, "Extracts a pve template from your character's current specialisation and gear with the defined name." },
		};

		static std::vector<ChatCommand> templateDeleteTable =
		{
			{ "pvp", SEC_CONSOLE, false, &HandleDeletePvpTemplateCommand, "Deletes a pvp template of the given name from the current character's specialisation." },
			{ "pve", SEC_CONSOLE, false, &HandleDeletePveTemplateCommand, "Deletes a pve template of the given name from the current character's specialisation." },
		};

		static std::vector<ChatCommand> templateCommandTable =
		{
			{ "extract", SEC_CONSOLE, false, NULL, "", templateExtractTable },
			{ "delete", SEC_CONSOLE, false, NULL, "", templateDeleteTable },
		};

		static std::vector<ChatCommand> commandTable =
		{
			{ "template", SEC_CONSOLE, true, NULL, "Command group to manage Specialisation Templates.", templateCommandTable },
		};
		return commandTable;
	}

	static bool HandleCreatePvpTemplateCommand(ChatHandler* handler, const char* args)
	{
		if (!args || !*args || args == "")
			return false;

		Player* player = handler->GetSession()->GetPlayer();
		if (!player)
			return false;

		std::string name = args;
		CreateTemplate(player, name, CATEGORY_PVP);
		return true;
	}

	static bool HandleCreatePveTemplateCommand(ChatHandler* handler, const char* args)
	{
		if (!args || !*args || args == "")
			return false;

		Player* player = handler->GetSession()->GetPlayer();
		if (!player)
			return false;

		std::string name = args;
		CreateTemplate(player, name, CATEGORY_PVE);
		return true;
	}

	static bool HandleDeletePvpTemplateCommand(ChatHandler* handler, const char* args)
	{
		if (!args || !*args || args == "")
			return false;

		Player* player = handler->GetSession()->GetPlayer();
		std::string name = args;
		DeleteTemplate(player, name, CATEGORY_PVP);
		return true;
	}

	static bool HandleDeletePveTemplateCommand(ChatHandler* handler, const char* args)
	{
		if (!args || !*args || args == "")
			return false;

		Player* player = handler->GetSession()->GetPlayer();
		std::string name = args;
		DeleteTemplate(player, name, CATEGORY_PVE);
		return true;
	}

	static bool DeleteTemplate(Player* player, std::string name, uint8 category)
	{
		std::map<std::string, PlayerTemplate> specTemplates = sTemplateMgr->m_ServerTemplates[player->getClass()][sTemplateMgr->GetSpecNameFromTree(player->GetPrimaryTalentTree(player->GetActiveSpec()))][category];

		if (specTemplates.find(name) != specTemplates.end())
		{
			// Delete from runtime
			sTemplateMgr->m_ServerTemplates[player->getClass()][sTemplateMgr->GetSpecNameFromTree(player->GetPrimaryTalentTree(player->GetActiveSpec()))][category].erase(name);
			// Delete from DB
			sTemplateMgr->DeleteTemplateFromDB(specTemplates[name].id);

			ChatHandler(player->GetSession()).PSendSysMessage("Template Deleted");
			return true;
		}

		ChatHandler(player->GetSession()).PSendSysMessage("Template not found.");
		return false;
	}

	static bool CreateTemplate(Player* player, std::string name, uint8 category)
	{
		sTemplateMgr->ExtractNamedTemplateToDB(player, category, name);
		ChatHandler(player->GetSession()).PSendSysMessage("Template Extracted");
		return true;
	}
};

void AddSC_TemplateCommands()
{
	new template_commands();
}