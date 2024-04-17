#include "BattlegroundMgr.h"
#include "DisableMgr.h"
#include "ScriptMgr.h"
#include "Battleground.h"

class Skirmish_npc : public CreatureScript
{
public:
    Skirmish_npc() : CreatureScript("Skirmish_npc") {}

    bool OnGossipHello(Player *pPlayer, Creature *pCreature)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Queue for 2v2 Skirmish", GOSSIP_SENDER_MAIN, ARENA_TYPE_2v2);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Queue for 3v3 Skirmish", GOSSIP_SENDER_MAIN, ARENA_TYPE_3v3);
        pPlayer->PlayerTalkClass->SendGossipMenu(1, pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player *pPlayer, Creature * /*pCreature*/, uint32 /*sender*/, uint32 action)
    {
        if (action != ARENA_TYPE_2v2 && action != ARENA_TYPE_3v3)
            return true;

        if (pPlayer->InBattlegroundQueue())
            return true;

        uint32 arenaRating = 0;
        uint32 matchmakerRating = 0;

        uint8 arenatype = action;

        //check existance
        Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(BATTLEGROUND_AA);
        if (!bg)
        {
            TC_LOG_ERROR("network.opcode", "Battleground: template bg (all arenas) not found");
            return false;
        }

        if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_AA, NULL))
        {
            ChatHandler(pPlayer->GetSession()).PSendSysMessage(LANG_ARENA_DISABLED);
            return false;
        }

        BattlegroundTypeId bgTypeId = bg->GetTypeID();
        BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, arenatype);
        PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), pPlayer->getLevel());
        if (!bracketEntry)
            return false;

        BattlegroundQueue &bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);

        uint32 avgTime = 0;
        GroupQueueInfo* ginfo = bgQueue.AddGroup(pPlayer, NULL, bgTypeId, bracketEntry, arenatype, false, false, arenaRating, matchmakerRating, 0);
        avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());

        // add to queue
        uint32 queueSlot = pPlayer->AddBattlegroundQueueId(bgQueueTypeId);
        // add joined time data
        pPlayer->AddBattlegroundQueueJoinTime(bgTypeId, ginfo->JoinTime);

        WorldPacket data; // send status packet (in queue)
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, pPlayer, queueSlot, STATUS_WAIT_QUEUE, avgTime, ginfo->JoinTime, arenatype);
        pPlayer->GetSession()->SendPacket(&data);

        sBattlegroundMgr->ScheduleQueueUpdate(matchmakerRating, arenatype, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());

        pPlayer->PlayerTalkClass->ClearMenus();
        pPlayer->CLOSE_GOSSIP_MENU();
        return true;
    }
};

uint32 const tabardEntry[13] = { 38314, 38309, 38312, 38310, 23705, 23709, 38313, 38311, 36941, 28788, 45983, 49086, 51534 };
// Tabard vendor
class npc_arena_vendor : public CreatureScript
{
public:
    npc_arena_vendor() : CreatureScript("npc_arena_vendor"){ }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        player->PlayerTalkClass->ClearMenus();
        uint32 rating = player->GetArenaPersonalRating(1);

        if (rating >= 2350)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Wrathful Gladiator Tabard", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
        if (rating >= 2200)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Relentless Gladiator Tabard", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
        if (rating >= 2100)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Furious Gladiator Tabard", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
        if (rating >= 2000)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Tabard of the protector", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
        if (rating >= 1800)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Competitors Tabard", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
        if (rating >= 1700)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Tabard of Void", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
        if (rating >= 1600)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Tabard of Fury", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
        if (rating >= 1500)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Tabard of Frost", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
        if (rating >= 1400)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Tabard of Flame", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
        if (rating >= 1300)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Tabard of Arcane", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
        if (rating >= 1200)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Tabard of Brilliance", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
        if (rating >= 1100)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Tabard of Nature", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
        if (rating >= 1000)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Tabard of the Defender", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        CurrencyTypes currency = CURRENCY_TYPE_HONOR_POINTS;
        if (action > GOSSIP_ACTION_INFO_DEF + 9)
            currency = CURRENCY_TYPE_CONQUEST_POINTS;
        uint32 points = player->GetCurrency(currency, false);
        if (points >= 50000)
        {
            if (player->AddItem(tabardEntry[action - (GOSSIP_ACTION_INFO_DEF + 1)], 1))
                player->ModifyCurrency(currency, -50000);
            else
                creature->MonsterWhisper("You don´t have enough space in your bags!", player->GetGUID());
        }
        else
            creature->MonsterWhisper("Not enough points! You need at least 500 honor/conquest points!", player->GetGUID());

        player->CLOSE_GOSSIP_MENU();
        return true;
    }
};

void AddSC_Skirmish_npc()
{
    new Skirmish_npc();
    new npc_arena_vendor();
}