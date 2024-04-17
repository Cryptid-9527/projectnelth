/*
*	Author: Aps
*
* Explanations for future reference:
*
* - Custom Character Templates
* Each account can have up to 10 templates for each class. These templates are cross-faction since they are only talent and glyph templates.
* A player is required to name his templates, and can replace them by saving a new template with an existing name.
* A player can delete, publish or make a previously published template private.
* A player can only have 1 template for each spec public at a time. Attempting to publish a second template of the same spec will cause the old one to become private.
* Players can only see templates for their current class in all NPC gossips.
*
*/
#include "ScriptPCH.h"
#include "Template.h"
#include "Player.h"

enum TalentTemplateConstants
{
	SAVE_INTERVAL = 30000 // 5 mins
};

class account_template_loader : public WorldScript
{
public:
	account_template_loader() : WorldScript("account_template_loader"), m_updateTimer(SAVE_INTERVAL) {}

	uint32 m_updateTimer;

	void OnUpdate(uint32 diff) override
	{
		if (m_updateTimer <= diff)
		{
			SaveData();
			m_updateTimer = SAVE_INTERVAL;
		}
		else
			m_updateTimer -= diff;
	}

	void OnShutdown() override
	{
		SaveData();
	}

	void OnStartup() override
	{
		LoadData();
	}

private:

	void SaveData()
	{
		/*
		* DELETE REPLACED AND DELETED TEMPLATES
		*/
		if (sTemplateMgr->deletedPlayerTemplatesContainer.size() > 0)
		{
			// Initialise first value booleans
			bool firstTemplate = true;
			bool firstTalent = true;
			bool firstGlyph = true;

			// Initialise character_templates query
			std::stringstream templateQuery;
			templateQuery << "DELETE FROM `character_templates` WHERE `id` IN ";
			// Initialise character_template_talents query
			std::stringstream talentQuery;
			talentQuery << "DELETE FROM `character_template_talents` WHERE `template_id` IN ";
			// Initialise character_template_glyphs query
			std::stringstream glyphQuery;
			glyphQuery << "DELETE FROM `character_template_glyphs` WHERE `template_id` IN ";

			// Loop through each player's new templates
			for (auto& deletedPlayerTemplates : sTemplateMgr->deletedPlayerTemplatesContainer)
			{
				// Loop through all new templates of this player
				for (auto& m_playerTemplate : deletedPlayerTemplates.second)
				{
					PlayerTemplate playerTemplate = m_playerTemplate.second;
					// Delete template
					if (firstTemplate)
					{
						templateQuery << "(";
						firstTemplate = false;
					}
					else
					{
						templateQuery << ", ";
					}
					templateQuery << playerTemplate.id;

					// Delete talents
					if (firstTalent)
					{
						talentQuery << "(";
						firstTemplate = false;
					}
					else
					{
						talentQuery << ", ";
					}
					talentQuery << playerTemplate.id;

					// Delete glyphs
					if (firstGlyph)
					{
						glyphQuery << "(";
						firstTemplate = false;
					}
					else
					{
						glyphQuery << ", ";
					}
					glyphQuery << playerTemplate.id;
				}
			}
			// Close the query brackets
			templateQuery << ")";
			talentQuery << ")";
			glyphQuery << ")";
			// Run the queries
			CharacterDatabase.PExecute(templateQuery.str().c_str());
			CharacterDatabase.PExecute(talentQuery.str().c_str());
			CharacterDatabase.PExecute(glyphQuery.str().c_str());
			// Clear the queue
			sTemplateMgr->deletedPlayerTemplatesContainer.clear();
		}
		/*
		* STORE NEW TEMPLATES
		*/
		if (sTemplateMgr->newPlayerTemplatesContainer.size() > 0)
		{
			// Initialise first value booleans
			bool firstTemplate = true;
			bool firstTalent = true;
			bool firstGlyph = true;

			// Initialise character_templates query
			std::stringstream templateQuery;
			templateQuery << "INSERT INTO `character_templates` (`id`, `account_id`, `class_id`, `name`, `spec`, `category`, `public`, `publisher`) VALUES ";
			// Initialise character_template_talents query
			std::stringstream talentQuery;
			talentQuery << "INSERT INTO `character_template_talents` (`template_id`, `talent_id`, `talent_rank`) VALUES ";
			// Initialise character_template_glyphs query
			std::stringstream glyphQuery;
			glyphQuery << "INSERT INTO `character_template_glyphs` (`template_id`, `slot`, `glyph`) VALUES ";

			// Loop through each player's new templates
			for (auto& newPlayerTemplates : sTemplateMgr->newPlayerTemplatesContainer)
			{
				// Loop through all new templates of this player
				for (auto& m_playerTemplate : newPlayerTemplates.second)
				{
					PlayerTemplate playerTemplate = m_playerTemplate.second;

					if (!firstTemplate)
						templateQuery << ", ";
					firstTemplate = false;
					// New template
					templateQuery << "(" << playerTemplate.id << ", " << playerTemplate.account_id << ", " << playerTemplate.class_id << ", '" << playerTemplate.name << "', '" << playerTemplate.spec << "', " << playerTemplate.category << ", " << (playerTemplate.isPublic ? 1 : 0) << ", " << playerTemplate.publisher << ")";

					// Loop through all the talent points for this template
					for (auto& talent : playerTemplate.talents)
					{
						if (!firstTalent)
							talentQuery << ", ";
						firstTalent = false;
						// New talent point
						talentQuery << "(" << playerTemplate.id << ", " << talent.talentId << ", " << talent.talentRank << ")";
					}

					// Loop through all the glyphs for this template
					for (auto& glyph : playerTemplate.glyphs)
					{
						if (!firstGlyph)
							glyphQuery << ", ";
						firstGlyph = false;
						// New glyph point
						glyphQuery << "(" << playerTemplate.id << ", " << glyph.slot << ", " << glyph.glyph << ")";
					}

				}
			}
			// Run the queries
			CharacterDatabase.PExecute(templateQuery.str().c_str());
			CharacterDatabase.PExecute(talentQuery.str().c_str());
			CharacterDatabase.PExecute(glyphQuery.str().c_str());
			// Clear the queue
			sTemplateMgr->newPlayerTemplatesContainer.clear();
		}
		/*
		* UPDATE EXISTING TEMPLATES
		*/
		if (sTemplateMgr->updatedPlayerTemplatesContainer.size() > 0)
		{
			// Loop through each player's new templates
			for (auto& updatedPlayerTemplates : sTemplateMgr->updatedPlayerTemplatesContainer)
			{
				// Loop through all new templates of this player
				for (auto& playerTemplate : updatedPlayerTemplates.second)
				{
					// Aps: TODO: I don't know if there is a better way to include all updates in a single sql statement
					// Aps: This should be fine for now
					CharacterDatabase.PExecute("UPDATE `character_templates` SET `category` = %u, `public` = %u, `publisher` = %u WHERE `id` = %u;",
						playerTemplate.second.category,
						(playerTemplate.second.isPublic ? 1 : 0),
						playerTemplate.second.publisher,
						playerTemplate.second.id);
				}
			}
			// Clear the queue
			sTemplateMgr->updatedPlayerTemplatesContainer.clear();
		}
	}

	void LoadData()
	{
		// Load the max template id
		QueryResult result = CharacterDatabase.PQuery("SELECT max(`id`) FROM `character_templates`;");
		if (result)
		{
			Field* fields = result->Fetch();
			uint32 id = fields[0].GetUInt32() + 1;
			sTemplateMgr->nextId = id;
		}
		else
		{
			sTemplateMgr->nextId = 1;
		}
		// Load all Templates with character names
		QueryResult tempResult = CharacterDatabase.PQuery("SELECT `id`, `account_id`, `class_id`, `name`, `spec`, `category`, `public`, `publisher` FROM `character_templates`;");
		if (tempResult)
		{
			// for each table row do
			do
			{
				Field* fields = tempResult->Fetch();
				uint32 id = fields[0].GetUInt32();
				uint32 account_id = fields[1].GetUInt32();
				uint32 class_id = fields[2].GetUInt32();
				std::string name = fields[3].GetString();
				std::string spec = fields[4].GetString();
				uint8 category = fields[5].GetUInt8();
				bool isPublic = fields[6].GetUInt32() == 1 ? true : false;
				uint32 publisher = fields[7].GetUInt32();

				PlayerTemplate specTemplate;
				specTemplate.id = id;
				specTemplate.account_id = account_id;
				specTemplate.class_id = class_id;
				specTemplate.name = name;
				specTemplate.spec = spec;
				specTemplate.category = category;
				specTemplate.isPublic = isPublic;
				specTemplate.publisher = publisher;
				specTemplate.publisherName = "";

				// Load all talents for this template
				QueryResult talentsResult = CharacterDatabase.PQuery("SELECT `talent_id`, `talent_rank` FROM `character_template_talents` WHERE `template_id` = %u;", id);
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
						// Place the talent inside the current spec template
						specTemplate.talents.emplace_back(curTalent);

					} while (talentsResult->NextRow());
				}

				// Load all glyphs for this template
				QueryResult glyphsResult = CharacterDatabase.PQuery("SELECT `slot`, `glyph` FROM `character_template_glyphs` WHERE `template_id` = %u;", id);
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
						// Place the glyph inside the current spec template
						specTemplate.glyphs.emplace_back(curGlyph);

					} while (glyphsResult->NextRow());
				}

				// Save the template inside the player's saved templates
				sTemplateMgr->playerTemplatesContainer[account_id][specTemplate.id] = specTemplate;

			} while (tempResult->NextRow());
		}
		// Load the names of the characters who published
		QueryResult namesResult = CharacterDatabase.PQuery("SELECT `temps`.`id`, `temps`.`account_id`, `chars`.`name` FROM `character_templates` as `temps` JOIN `characters` as `chars` ON `temps`.`publisher` = `chars`.`guid`;");
		if (namesResult)
		{
			do
			{
				Field* fields = namesResult->Fetch();
				uint32 id = fields[0].GetUInt32();
				uint32 account_id = fields[1].GetUInt32();
				std::string publisherName = fields[2].GetString();

				for (auto& playerTemplate : sTemplateMgr->playerTemplatesContainer[account_id])
				{
					if (playerTemplate.second.id == id)
					{
						PlayerTemplate& pTemplate = playerTemplate.second;
						pTemplate.publisherName = publisherName;
						// Aps: Store published template in our container
						sTemplateMgr->m_publishedTemplates[pTemplate.class_id][pTemplate.spec][pTemplate.category][pTemplate.name] = pTemplate;
						break;
					}
				}

			} while (namesResult->NextRow());
		}
	}
};

void AddSC_TemplateLoader()
{
	new account_template_loader();
}