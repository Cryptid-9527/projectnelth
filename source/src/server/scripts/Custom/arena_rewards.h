#include "ScriptPCH.h"
#include "ArenaTeam.h"
#include "ArenaTeamMgr.h"
#include "Player.h"

using namespace std;

//typedef std::list<ArenaTeamMember> MemberList;

enum RankSlots
{
	R1_GLAD		= 0,
	GLADIATOR	= 1,
	DUELIST		= 2,
	RIVAL		= 3,
	CHALLENGER	= 4
};

enum TitleIDs
{
	GLADIATOR_TITLE		= 42,
	DUELIST_TITLE		= 43,
	RIVAL_TITLE			= 44,
	CHALLENGR_TITLE		= 45,
	MERCILESS_GLAD		= 62,
	VENGEFUL_GLAD		= 71,
	BRUTAL_GLAD			= 80,
	DEADLY_GLAD			= 157,
	FURIOUS_GLAD		= 167,
	RELENTLESS_GLAD		= 169,
	WRATHFUL_GLAD		= 177,
	VICIOUS_GLAD		= 279,
	RUTHLESS_GLAD		= 280,
	CATACLYSMIC_GLAD	= 281
};

enum QuestIDs
{
	GLADIATOR_QUEST		= 300001,
	DUELIST_QUEST		= 300002,
	RIVAL_QUEST			= 300003,
	CHALLENGR_QUEST		= 300004,
	MERCILESS_QUEST		= 300005,
	VENGEFUL_QUEST		= 300006,
	BRUTAL_QUEST		= 300007,
	DEADLY_QUEST		= 300008,
	FURIOUS_QUEST		= 300009,
	RELENTLESS_QUEST	= 300010,
	WRATHFUL_QUEST		= 300011,
	VICIOUS_QUEST		= 300012,
	RUTHLESS_QUEST		= 300013,
	CATACLYSMIC_QUEST	= 300014
};

struct ArenaTeamSortingData
{
	uint32 rating;
	uint32 id;
	uint32 slot;
};

class ArenaRatingSorter
{
public:
	vector<ArenaTeamSortingData*> temp;
	vector<ArenaTeamSortingData*> unsorted;
	vector<ArenaTeamSortingData*> sorted;

	void GenerateArenaTeamRankings();
	void FindHighest(uint32 pos);
	void Sort();
};

class ArenaRewardsData
{
public:
	ArenaRewardsData() {}

	void DistributeArenaTitles();
	bool TitleDistributionForSlotEnabled(uint8 slot) { return m_distributeTitlesForArenaSlot[slot]; }
	void ToggleTitleDistributionForSlot(uint8 slot, bool enabled) { m_distributeTitlesForArenaSlot[slot] = enabled; }
	void InitPlayerData(Player* plr);
private:
	bool m_distributeTitlesForArenaSlot[3];
};