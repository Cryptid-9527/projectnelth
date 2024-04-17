#ifndef TALENT_FUNCTIONS_H
#define TALENT_FUNCTIONS_H

#include "Define.h"
#include "Player.h"
#include <ace/Singleton.h>
#include <ace/Thread_Mutex.h>

enum TemplateCategories
{
	CATEGORY_PVP,
	CATEGORY_PVE,

	MAX_CATEGORIES
};

enum templateSpells
{
	PLATE_MAIL = 750,
	MAIL = 8737
};

enum WeaponProficiencies
{
	BLOCK = 107,
	BOWS = 264,
	CROSSBOWS = 5011,
	DAGGERS = 1180,
	FIST_WEAPONS = 15590,
	GUNS = 266,
	ONE_H_AXES = 196,
	ONE_H_MACES = 198,
	ONE_H_SWORDS = 201,
	POLEARMS = 200,
	SHOOT = 5019,
	STAVES = 227,
	TWO_H_AXES = 197,
	TWO_H_MACES = 199,
	TWO_H_SWORDS = 202,
	WANDS = 5009,
	THROW_WAR = 2567
};

struct TalentTreeTemplate
{
	std::string playerClass;
	std::string playerSpec;
	uint32 talentTree;
};

struct GearTemplate
{
	int race_id;
	int pos;
	int itemEntry;
	int enchant;
	int socket1;
	int socket2;
	int socket3;
	int bonusEnchant;
	int prismaticEnchant;
	int reforge;

};

struct TalentTemplate
{
	int talentId;
	int talentRank;
};

struct GlyphTemplate
{
	int slot;
	int glyph;
};

typedef std::list<GearTemplate> GearTemplates;
typedef std::map<uint32, GearTemplates> GearTemplatesMap;

class PlayerTemplate
{
public:
	PlayerTemplate()
	{
		id = 0;
		class_id = 0;
		category = 0;
		account_id = 0;
		isPublic = false;
		publisher = 0;
	}
	bool operator == (const PlayerTemplate& other) const
	{
		return (other.id == id);
	}
	bool operator ! () const
	{
		return !id;
	}
	int                       id;
	int                       class_id;
	int                       category;
	std::string               name;
	std::string               spec;
	std::list<TalentTemplate> talents;
	std::list<GlyphTemplate>  glyphs;
	GearTemplatesMap		  gear;
	// Aps: Used only by player templates
	int                       account_id;
	bool                      isPublic;
	int                       publisher;
	std::string               publisherName;
};

//              category                name
typedef std::map<uint32, std::map<std::string, PlayerTemplate>> ServerTemplates; // Used by the server
//            template_id
typedef std::map<uint32, PlayerTemplate> PlayerTemplates; // Used by players

typedef std::vector<TalentTreeTemplate> TalentTreeContainer;
typedef std::vector<TalentTemplate*> TalentContainer;
typedef std::vector<GlyphTemplate*> GlyphContainer;

typedef std::vector<std::vector<std::string>> ClassSpecs;

class sTemplate
{
public:
	void LoadTalentTrees();
	void LoadNpcTemplates();

	void ApplyGlyph(Player* player, uint8 slot, uint32 glyphID);
	void ClearGlyphs(Player* player);
	void ApplyBonus(Player* player, Item* item, EnchantmentSlot slot, uint32 bonusEntry);

	void ExtractNamedTemplateToDB(Player* player, uint8 category, std::string name);
	GearTemplates ExtractGearTemplate(Player* player);
	std::list<TalentTemplate> ExtractTalentTemplate(Player* player);
	std::list<GlyphTemplate> ExtractGlyphTemplate(Player* player);

	void StoreTemplateToDB(PlayerTemplate npcTemplate, bool gearOnly = false);
	void DeleteTemplateFromDB(uint32 templateId);

	bool OverwriteTemplateForPlayer(Player* player, std::string& playerSpecStr, std::string& name);
	PlayerTemplate ExtractTalentTemplateForPlayer(Player* player, std::string& playerSpecStr, std::string& name);
	bool ApplyTemplateForPlayer(Player* player, int id);

	bool TemplateExists(Player* player, uint32 template_id);

	bool IsAllowedToStoreOrEquip(uint32 itemID);
	bool UpdatePrimaryTalentTree(Player* player, const char* spec);

	std::string GetClassString(uint32 classId);
	PlayerTemplate GetTemplate(uint32 templateId);

	GearTemplates ChoseGearTemplate(Player* player, PlayerTemplate m_template);
	void LearnTemplateTalents(Player* player, PlayerTemplate m_template);
	void LearnTemplateGlyphs(Player* player, PlayerTemplate m_template);
	void EquipTemplateGear(Player* player, PlayerTemplate m_template);
	void LearnPlateMailSpells(Player* player);

	void ApplyTemplate(Player* player, uint32 template_id, bool learnTalents, bool equipGear);
	ClassSpecs GetClassSpecs(uint8 classId, bool toLower = false);
	std::string GetSpecNameFromIndex(uint8 classId, uint8 index);
	std::string GetSpecNameFromTree(uint32 tree);

	// Aps: Holds all Remedy templates
	//      class_id               spec
	std::map<uint8, std::map<std::string, ServerTemplates>> m_ServerTemplates;
	// Aps: Holds all templates made by players that were published
	std::map<uint8, std::map<std::string, ServerTemplates>> m_publishedTemplates;

	// Aps: Holds all templates saved by players
	//     account_id
	std::map<uint32, PlayerTemplates> playerTemplatesContainer;

	// Aps: Used to keep track of updates before they are saved
	std::map<uint32, PlayerTemplates> newPlayerTemplatesContainer;
	std::map<uint32, PlayerTemplates> deletedPlayerTemplatesContainer;
	std::map<uint32, PlayerTemplates> updatedPlayerTemplatesContainer;

	TalentTreeContainer m_TalentTreeContainer;
	std::map<uint32, std::string> talentTreeNames;

	uint32 nextId;
	uint32 nextTemplateId;
};
#define sTemplateMgr ACE_Singleton<sTemplate, ACE_Null_Mutex>::instance()
#endif