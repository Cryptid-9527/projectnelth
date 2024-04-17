/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
Name: lookup_commandscript
%Complete: 100
Comment: All lookup related commands
Category: commandscripts
EndScriptData */

#include "AccountMgr.h"
#include "Chat.h"
#include "GameEventMgr.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ReputationMgr.h"
#include "ScriptMgr.h"
#include "SpellInfo.h"

class lookup_commandscript : public CommandScript
{
public:
    lookup_commandscript() : CommandScript("lookup_commandscript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> lookupPlayerCommandTable =
        {
            { "guid",           SEC_CONSOLE,     true,  &HandleLookupPlayersGUIDCommand,     "" },
            { "ip",             SEC_CONSOLE,     true,  &HandleLookupPlayerIpCommand,        "" },
            { "account",        SEC_CONSOLE,     true,  &HandleLookupPlayerAccountCommand,   "" },
            { "email",          SEC_CONSOLE,     true,  &HandleLookupPlayerEmailCommand,     "" },
            { "item",           SEC_CONSOLE,     true,  &HandleLookupPlayerItemCommand,     "" },
        };

        static std::vector<ChatCommand> lookupAccountTable =
        {
            { "id",           SEC_CONSOLE,     true,  &HandleLookupAccountIdCommand,     "" },
        };

        static std::vector<ChatCommand> lookupHistoryTradeTable =
        {
            { "to",             SEC_CONSOLE,  true,  &HandleLookupHistoryTradeToCommand,     "" },
            { "from",           SEC_CONSOLE,  true,  &HandleLookupHistoryTradeFromCommand,   "" },
            { "details",        SEC_CONSOLE,  true,  &HandleLookupHistoryTradeDetailsCommand,"" },

        };
        static std::vector<ChatCommand> lookupHistoryMailTable =
        {
            { "to",             SEC_CONSOLE,  true,  &HandleLookupHistoryMailToCommand,      "" },
            { "from",           SEC_CONSOLE,  true,  &HandleLookupHistoryMailFromCommand,    "" },
            { "details",        SEC_CONSOLE,  true,  &HandleLookupHistoryMailDetailsCommand, "" },
        };

        static std::vector<ChatCommand> lookupHistoryPvPTable =
        {
            { "kicks",          SEC_CONSOLE,  true,  &HandleLookupHistoryPvPKicksCommand,      "" },
            { "accuracy",       SEC_CONSOLE,  true,  &HandleLookupHistoryPvPAccuracyCommand,    "" },
        };

        static std::vector<ChatCommand> lookupMacroCommandTable =
        {
            { "character",      SEC_CONSOLE,  true,  &HandleLookupMacroCharacterCommand,        "" },
            { "account",        SEC_CONSOLE,  true,  &HandleLookupMacroAccountCommand,       "" },
        };


        static std::vector<ChatCommand> lookupHistoryCommandTable =
        {
            { "ip",             SEC_CONSOLE,     true,  &HandleLookupHistoryIPCommand,        "" },
            { "account",        SEC_CONSOLE,     true,  &HandleLookupHistoryAccountCommand,   "" },
            { "commands",       SEC_CONSOLE,     true,  &HandleLookupHistoryCommandsCommand,  "" },
            { "trade",          SEC_CONSOLE,     true,  NULL,                                 "", lookupHistoryTradeTable },
            { "pvp",            SEC_CONSOLE,     true,  NULL,                                 "", lookupHistoryPvPTable },
            { "mail",           SEC_CONSOLE,     true,  NULL,                                 "", lookupHistoryMailTable },
        };

        static std::vector<ChatCommand> lookupSpellCommandTable =
        {
            { "id",             SEC_CONSOLE,  true,  &HandleLookupSpellIdCommand,         "" },
            { "",               SEC_CONSOLE,  true,  &HandleLookupSpellCommand,           "" },
        };

        static std::vector<ChatCommand> lookupLootFromCommandTable =
        {
            { "creature",              SEC_CONSOLE,  true,  &HandleLookupLootFromCreatureCommand,           "" },
            { "chest",                 SEC_CONSOLE,  true,  &HandleLookupLootFromChestCommand,           "" },
            { "spell",                 SEC_CONSOLE,  true,  &HandleLookupLootFromSpellCommand,           "" },
            { "mail",                  SEC_CONSOLE,  true,  &HandleLookupLootFromMailCommand,           "" },
            { "fishing",               SEC_CONSOLE,  true,  &HandleLookupLootFromFishingCommand,           "" },
            { "skinning",              SEC_CONSOLE,  true,  &HandleLookupLootFromSkinningCommand,           "" },
            { "pickpocket",            SEC_CONSOLE,  true,  &HandleLookupLootFromPickpocketingCommand,           "" },
            { "milling",               SEC_CONSOLE,  true,  &HandleLookupLootFromMillingCommand,           "" },
            { "table",                 SEC_CONSOLE,  true,  &HandleLookupLootFromTableCommand,           "" },
            { "prospecting",           SEC_CONSOLE,  true,  &HandleLookupLootFromProspectingCommand,           "" },
            { "item",                  SEC_CONSOLE,  true,  &HandleLookupLootFromItemCommand,           "" },
        };


        static std::vector<ChatCommand> lookupLootToCommandTable =
        {
            { "item",                   SEC_CONSOLE,  true,  &HandleLookupLootForItemCommand,           "" },
            { "table",                  SEC_CONSOLE,  true,  &HandleLookupLootForTableCommand,           "" },
        };

        static std::vector<ChatCommand> lookupLootCommandTable =
        {
            { "to",                    SEC_CONSOLE,  true,  NULL,            "", lookupLootToCommandTable },
            { "from",                  SEC_CONSOLE,  true,  NULL,           "", lookupLootFromCommandTable},
        };

        static std::vector<ChatCommand> lookupCommandTable =
        {
            { "area",           SEC_CONSOLE,      true,  &HandleLookupAreaCommand,            "" },
            { "loot",           SEC_CONSOLE,      true,  NULL,            "", lookupLootCommandTable },
            { "creature",       SEC_CONSOLE,  true,  &HandleLookupCreatureCommand,        "" },
            { "event",          SEC_CONSOLE,     true,  &HandleLookupEventCommand,           "" },
            { "faction",        SEC_CONSOLE,  true,  &HandleLookupFactionCommand,         "" },
            { "item",           SEC_CONSOLE,  true,  &HandleLookupItemCommand,            "" },
            { "itemset",        SEC_CONSOLE,  true,  &HandleLookupItemSetCommand,         "" },
            { "object",         SEC_CONSOLE,  true,  &HandleLookupObjectCommand,          "" },
            { "quest",          SEC_CONSOLE,  true,  &HandleLookupQuestCommand,           "" },
            { "player",         SEC_CONSOLE,     true,  NULL,                                "", lookupPlayerCommandTable },
            { "macro",          SEC_CONSOLE,     true,  NULL,                                "", lookupMacroCommandTable },
            { "history",        SEC_CONSOLE,     true,  NULL,                                "", lookupHistoryCommandTable },
            { "skill",          SEC_CONSOLE,  true,  &HandleLookupSkillCommand,           "" },
            { "spell",          SEC_CONSOLE,  true,  NULL,                                "", lookupSpellCommandTable },
            { "taxinode",       SEC_CONSOLE,  true,  &HandleLookupTaxiNodeCommand,        "" },
            { "tele",           SEC_CONSOLE,      true,  &HandleLookupTeleCommand,            "" },
            { "title",          SEC_CONSOLE,     true,  &HandleLookupTitleCommand,           "" },
            { "map",            SEC_CONSOLE,  true,  &HandleLookupMapCommand,             "" },
            { "account",            SEC_CONSOLE,  true,  NULL,             "", lookupAccountTable },
        };

        static std::vector<ChatCommand> commandTable =
        {
            { "lookup",         SEC_CONSOLE,  true,  NULL,                                "", lookupCommandTable },
        };
        return commandTable;
    }

    static bool HandleLookupAreaCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        std::string namePart = args;
        std::wstring wNamePart;

        if (!Utf8toWStr(namePart, wNamePart))
            return false;

        bool found = false;
        uint32 count = 0;
        uint32 maxResults = sWorld->getIntConfig(CONFIG_MAX_RESULTS_LOOKUP_COMMANDS);

        // converting string that we try to find to lower case
        wstrToLower(wNamePart);

        // Search in AreaTable.dbc
        for (uint32 areaflag = 0; areaflag < sAreaStore.GetNumRows(); ++areaflag)
        {
            AreaTableEntry const* areaEntry = sAreaStore.LookupEntry(areaflag);
            if (areaEntry)
            {
                std::string name = areaEntry->area_name;
                if (name.empty())
                    continue;

                if (!Utf8FitTo(name, wNamePart))
                    continue;

                if (maxResults && count++ == maxResults)
                {
                    handler->PSendSysMessage(LANG_COMMAND_LOOKUP_MAX_RESULTS, maxResults);
                    return true;
                }

                // send area in "id - [name]" format
                std::ostringstream ss;
                if (handler->GetSession())
                    ss << areaEntry->ID << " - |cffffffff|Harea:" << areaEntry->ID << "|h[" << name<< "]|h|r";
                else
                    ss << areaEntry->ID << " - " << name;

                handler->SendSysMessage(ss.str().c_str());

                if (!found)
                    found = true;
            }
        }

        if (!found)
            handler->SendSysMessage(LANG_COMMAND_NOAREAFOUND);

        return true;
    }

    static bool HandleLookupCreatureCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        std::string namePart = args;
        std::wstring wNamePart;

        // converting string that we try to find to lower case
        if (!Utf8toWStr(namePart, wNamePart))
            return false;

        wstrToLower(wNamePart);

        bool found = false;
        uint32 count = 0;
        uint32 maxResults = sWorld->getIntConfig(CONFIG_MAX_RESULTS_LOOKUP_COMMANDS);

        CreatureTemplateContainer const* ctc = sObjectMgr->GetCreatureTemplates();
        for (CreatureTemplateContainer::const_iterator itr = ctc->begin(); itr != ctc->end(); ++itr)
        {
            uint32 id = itr->second.Entry;
            uint8 localeIndex = handler->GetSessionDbLocaleIndex();
            if (CreatureLocale const* creatureLocale = sObjectMgr->GetCreatureLocale(id))
            {
                if (creatureLocale->Name.size() > localeIndex && !creatureLocale->Name[localeIndex].empty())
                {
                    std::string name = creatureLocale->Name[localeIndex];

                    if (Utf8FitTo(name, wNamePart))
                    {
                        if (maxResults && count++ == maxResults)
                        {
                            handler->PSendSysMessage(LANG_COMMAND_LOOKUP_MAX_RESULTS, maxResults);
                            return true;
                        }

                        if (handler->GetSession())
                            handler->PSendSysMessage(LANG_CREATURE_ENTRY_LIST_CHAT, id, id, name.c_str());
                        else
                            handler->PSendSysMessage(LANG_CREATURE_ENTRY_LIST_CONSOLE, id, name.c_str());

                        if (!found)
                            found = true;

                        continue;
                    }
                }
            }

            std::string name = itr->second.Name;
            if (name.empty())
                continue;

            if (Utf8FitTo(name, wNamePart))
            {
                if (maxResults && count++ == maxResults)
                {
                    handler->PSendSysMessage(LANG_COMMAND_LOOKUP_MAX_RESULTS, maxResults);
                    return true;
                }

                if (handler->GetSession())
                    handler->PSendSysMessage(LANG_CREATURE_ENTRY_LIST_CHAT, id, id, name.c_str());
                else
                    handler->PSendSysMessage(LANG_CREATURE_ENTRY_LIST_CONSOLE, id, name.c_str());

                if (!found)
                    found = true;
            }
        }

        if (!found)
            handler->SendSysMessage(LANG_COMMAND_NOCREATUREFOUND);

        return true;
    }

    static bool HandleLookupEventCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        std::string namePart = args;
        std::wstring wNamePart;

        // converting string that we try to find to lower case
        if (!Utf8toWStr(namePart, wNamePart))
            return false;

        wstrToLower(wNamePart);

        bool found = false;
        uint32 count = 0;
        uint32 maxResults = sWorld->getIntConfig(CONFIG_MAX_RESULTS_LOOKUP_COMMANDS);

        GameEventMgr::GameEventDataMap const& events = sGameEventMgr->GetEventMap();
        GameEventMgr::ActiveEvents const& activeEvents = sGameEventMgr->GetActiveEventList();

        for (uint32 id = 0; id < events.size(); ++id)
        {
            GameEventData const& eventData = events[id];

            std::string descr = eventData.description;
            if (descr.empty())
                continue;

            if (Utf8FitTo(descr, wNamePart))
            {
                if (maxResults && count++ == maxResults)
                {
                    handler->PSendSysMessage(LANG_COMMAND_LOOKUP_MAX_RESULTS, maxResults);
                    return true;
                }

                char const* active = activeEvents.find(id) != activeEvents.end() ? handler->GetTrinityString(LANG_ACTIVE) : "";

                if (handler->GetSession())
                    handler->PSendSysMessage(LANG_EVENT_ENTRY_LIST_CHAT, id, id, eventData.description.c_str(), active);
                else
                    handler->PSendSysMessage(LANG_EVENT_ENTRY_LIST_CONSOLE, id, eventData.description.c_str(), active);

                if (!found)
                    found = true;
            }
        }

        if (!found)
            handler->SendSysMessage(LANG_NOEVENTFOUND);

        return true;
    }

    static bool HandleLookupFactionCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        // Can be NULL at console call
        Player* target = handler->getSelectedPlayer();

        std::string namePart = args;
        std::wstring wNamePart;

        if (!Utf8toWStr(namePart, wNamePart))
            return false;

        // converting string that we try to find to lower case
        wstrToLower (wNamePart);

        bool found = false;
        uint32 count = 0;
        uint32 maxResults = sWorld->getIntConfig(CONFIG_MAX_RESULTS_LOOKUP_COMMANDS);

        for (uint32 id = 0; id < sFactionStore.GetNumRows(); ++id)
        {
            if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(id))
            {
                FactionState const* factionState = target ? target->GetReputationMgr().GetState(factionEntry) : NULL;

                std::string name = factionEntry->name;
                if (name.empty())
                    continue;

                if (!Utf8FitTo(name, wNamePart))
                    continue;

                if (maxResults && count++ == maxResults)
                {
                    handler->PSendSysMessage(LANG_COMMAND_LOOKUP_MAX_RESULTS, maxResults);
                    return true;
                }

                // send faction in "id - [faction] rank reputation [visible] [at war] [own team] [unknown] [invisible] [inactive]" format
                // or              "id - [faction] [no reputation]" format
                std::ostringstream ss;
                if (handler->GetSession())
                    ss << id << " - |cffffffff|Hfaction:" << id << "|h[" << name << "]|h|r";
                else
                    ss << id << " - " << name;

                if (factionState) // and then target != NULL also
                {
                    uint32 index = target->GetReputationMgr().GetReputationRankStrIndex(factionEntry);
                    std::string rankName = handler->GetTrinityString(index);

                    ss << ' ' << rankName << "|h|r (" << target->GetReputationMgr().GetReputation(factionEntry) << ')';

                    if (factionState->Flags & FACTION_FLAG_VISIBLE)
                        ss << handler->GetTrinityString(LANG_FACTION_VISIBLE);
                    if (factionState->Flags & FACTION_FLAG_AT_WAR)
                        ss << handler->GetTrinityString(LANG_FACTION_ATWAR);
                    if (factionState->Flags & FACTION_FLAG_PEACE_FORCED)
                        ss << handler->GetTrinityString(LANG_FACTION_PEACE_FORCED);
                    if (factionState->Flags & FACTION_FLAG_HIDDEN)
                        ss << handler->GetTrinityString(LANG_FACTION_HIDDEN);
                    if (factionState->Flags & FACTION_FLAG_INVISIBLE_FORCED)
                        ss << handler->GetTrinityString(LANG_FACTION_INVISIBLE_FORCED);
                    if (factionState->Flags & FACTION_FLAG_INACTIVE)
                        ss << handler->GetTrinityString(LANG_FACTION_INACTIVE);
                }
                else
                    ss << handler->GetTrinityString(LANG_FACTION_NOREPUTATION);

                handler->SendSysMessage(ss.str().c_str());

                if (!found)
                    found = true;
            }
        }

        if (!found)
            handler->SendSysMessage(LANG_COMMAND_FACTION_NOTFOUND);
        return true;
    }

    static bool HandleLookupItemCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        std::string namePart = args;
        std::wstring wNamePart;

        // converting string that we try to find to lower case
        if (!Utf8toWStr(namePart, wNamePart))
            return false;

        wstrToLower(wNamePart);

        bool found = false;
        uint32 count = 0;
        uint32 maxResults = sWorld->getIntConfig(CONFIG_MAX_RESULTS_LOOKUP_COMMANDS);

        // Search in `item_template`
        ItemTemplateContainer const* its = sObjectMgr->GetItemTemplateStore();
        for (auto itr = its->begin(); itr != its->end(); ++itr)
        {
            std::ostringstream result{ "" };
            result << "\n" << itr->second.ItemId << " - " << itr->second.GetItemLink(false, 0, 0, 0, 0, 0, 0, 0);
            auto print_result{ result.str() };

            int localeIndex = handler->GetSessionDbLocaleIndex();
            if (localeIndex >= 0)
            {
                uint8 ulocaleIndex = uint8(localeIndex);
                if (ItemLocale const* il = sObjectMgr->GetItemLocale(itr->second.ItemId))
                {
                    if (il->Name.size() > ulocaleIndex && !il->Name[ulocaleIndex].empty())
                    {
                        std::string name = il->Name[ulocaleIndex];

                        if (Utf8FitTo(name, wNamePart))
                        {
                            if (maxResults && count++ == maxResults)
                            {
                                handler->PSendSysMessage(LANG_COMMAND_LOOKUP_MAX_RESULTS, maxResults);
                                return true;
                            }

                            if (handler->GetSession())
                                handler->SendSysMessage(print_result.c_str());
                            else
                                handler->PSendSysMessage(LANG_ITEM_LIST_CONSOLE, itr->second.ItemId, name.c_str());

                            if (!found)
                                found = true;

                            continue;
                        }
                    }
                }
            }

            std::string name = itr->second.Name1;
            if (name.empty())
                continue;

            if (Utf8FitTo(name, wNamePart))
            {
                if (maxResults && count++ == maxResults)
                {
                    handler->PSendSysMessage(LANG_COMMAND_LOOKUP_MAX_RESULTS, maxResults);
                    return true;
                }

                if (handler->GetSession())
                    handler->SendSysMessage(print_result.c_str());
                else
                    handler->PSendSysMessage(LANG_ITEM_LIST_CONSOLE, itr->second.ItemId, name.c_str());

                if (!found)
                    found = true;
            }
        }

        if (!found)
            handler->SendSysMessage(LANG_COMMAND_NOITEMFOUND);

        return true;
    }

    static bool HandleLookupItemSetCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        std::string namePart = args;
        std::wstring wNamePart;

        if (!Utf8toWStr(namePart, wNamePart))
            return false;

        // converting string that we try to find to lower case
        wstrToLower(wNamePart);

        bool found = false;
        uint32 count = 0;
        uint32 maxResults = sWorld->getIntConfig(CONFIG_MAX_RESULTS_LOOKUP_COMMANDS);

        // Search in ItemSet.dbc
        for (uint32 id = 0; id < sItemSetStore.GetNumRows(); id++)
        {
            ItemSetEntry const* set = sItemSetStore.LookupEntry(id);
            if (set)
            {
                std::string name = set->name;
                if (name.empty())
                    continue;

                if (!Utf8FitTo(name, wNamePart))
                    continue;

                if (maxResults && count++ == maxResults)
                {
                    handler->PSendSysMessage(LANG_COMMAND_LOOKUP_MAX_RESULTS, maxResults);
                    return true;
                }

                // send item set in "id - [namedlink locale]" format
                if (handler->GetSession())
                    handler->PSendSysMessage(LANG_ITEMSET_LIST_CHAT, id, id, name.c_str(), "");
                else
                    handler->PSendSysMessage(LANG_ITEMSET_LIST_CONSOLE, id, name.c_str(), "");

                if (!found)
                    found = true;
            }
        }
        if (!found)
            handler->SendSysMessage(LANG_COMMAND_NOITEMSETFOUND);

        return true;
    }

    static bool HandleLookupObjectCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        std::string namePart = args;
        std::wstring wNamePart;

        // converting string that we try to find to lower case
        if (!Utf8toWStr(namePart, wNamePart))
            return false;

        wstrToLower(wNamePart);

        bool found = false;
        uint32 count = 0;
        uint32 maxResults = sWorld->getIntConfig(CONFIG_MAX_RESULTS_LOOKUP_COMMANDS);

        GameObjectTemplateContainer const* gotc = sObjectMgr->GetGameObjectTemplates();
        for (GameObjectTemplateContainer::const_iterator itr = gotc->begin(); itr != gotc->end(); ++itr)
        {
            uint8 localeIndex = handler->GetSessionDbLocaleIndex();
            if (GameObjectLocale const* objectLocalte = sObjectMgr->GetGameObjectLocale(itr->second.entry))
            {
                if (objectLocalte->Name.size() > localeIndex && !objectLocalte->Name[localeIndex].empty())
                {
                    std::string name = objectLocalte->Name[localeIndex];

                    if (Utf8FitTo(name, wNamePart))
                    {
                        if (maxResults && count++ == maxResults)
                        {
                            handler->PSendSysMessage(LANG_COMMAND_LOOKUP_MAX_RESULTS, maxResults);
                            return true;
                        }

                        if (handler->GetSession())
                            handler->PSendSysMessage(LANG_GO_ENTRY_LIST_CHAT, itr->second.entry, itr->second.entry, name.c_str());
                        else
                            handler->PSendSysMessage(LANG_GO_ENTRY_LIST_CONSOLE, itr->second.entry, name.c_str());

                        if (!found)
                            found = true;

                        continue;
                    }
                }
            }

            std::string name = itr->second.name;
            if (name.empty())
                continue;

            if (Utf8FitTo(name, wNamePart))
            {
                if (maxResults && count++ == maxResults)
                {
                    handler->PSendSysMessage(LANG_COMMAND_LOOKUP_MAX_RESULTS, maxResults);
                    return true;
                }

                if (handler->GetSession())
                    handler->PSendSysMessage(LANG_GO_ENTRY_LIST_CHAT, itr->second.entry, itr->second.entry, name.c_str());
                else
                    handler->PSendSysMessage(LANG_GO_ENTRY_LIST_CONSOLE, itr->second.entry, name.c_str());

                if (!found)
                    found = true;
            }
        }

        if (!found)
            handler->SendSysMessage(LANG_COMMAND_NOGAMEOBJECTFOUND);

        return true;
    }

    static bool HandleLookupQuestCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        // can be NULL at console call
        Player* target = handler->getSelectedPlayer();

        std::string namePart = args;
        std::wstring wNamePart;

        // converting string that we try to find to lower case
        if (!Utf8toWStr(namePart, wNamePart))
            return false;

        wstrToLower(wNamePart);

        bool found = false;
        uint32 count = 0;
        uint32 maxResults = sWorld->getIntConfig(CONFIG_MAX_RESULTS_LOOKUP_COMMANDS);

        ObjectMgr::QuestMap const& qTemplates = sObjectMgr->GetQuestTemplates();
        for (ObjectMgr::QuestMap::const_iterator iter = qTemplates.begin(); iter != qTemplates.end(); ++iter)
        {
            Quest* qInfo = iter->second;

            int localeIndex = handler->GetSessionDbLocaleIndex();
            if (localeIndex >= 0)
            {
                uint8 ulocaleIndex = uint8(localeIndex);
                if (QuestLocale const* questLocale = sObjectMgr->GetQuestLocale(qInfo->GetQuestId()))
                {
                    if (questLocale->Title.size() > ulocaleIndex && !questLocale->Title[ulocaleIndex].empty())
                    {
                        std::string title = questLocale->Title[ulocaleIndex];

                        if (Utf8FitTo(title, wNamePart))
                        {
                            if (maxResults && count++ == maxResults)
                            {
                                handler->PSendSysMessage(LANG_COMMAND_LOOKUP_MAX_RESULTS, maxResults);
                                return true;
                            }

                            char const* statusStr = "";

                            if (target)
                            {
                                QuestStatus status = target->GetQuestStatus(qInfo->GetQuestId());

                                switch (status)
                                {
                                    case QUEST_STATUS_COMPLETE:
                                        statusStr = handler->GetTrinityString(LANG_COMMAND_QUEST_COMPLETE);
                                        break;
                                    case QUEST_STATUS_INCOMPLETE:
                                        statusStr = handler->GetTrinityString(LANG_COMMAND_QUEST_ACTIVE);
                                        break;
                                    case QUEST_STATUS_REWARDED:
                                        statusStr = handler->GetTrinityString(LANG_COMMAND_QUEST_REWARDED);
                                        break;
                                    default:
                                        break;
                                }
                            }

                            if (handler->GetSession())
                                handler->PSendSysMessage(LANG_QUEST_LIST_CHAT, qInfo->GetQuestId(), qInfo->GetQuestId(), qInfo->GetQuestLevel(), title.c_str(), statusStr);
                            else
                                handler->PSendSysMessage(LANG_QUEST_LIST_CONSOLE, qInfo->GetQuestId(), title.c_str(), statusStr);

                            if (!found)
                                found = true;

                            continue;
                        }
                    }
                }
            }

            std::string title = qInfo->GetTitle();
            if (title.empty())
                continue;

            if (Utf8FitTo(title, wNamePart))
            {
                if (maxResults && count++ == maxResults)
                {
                    handler->PSendSysMessage(LANG_COMMAND_LOOKUP_MAX_RESULTS, maxResults);
                    return true;
                }

                char const* statusStr = "";

                if (target)
                {
                    QuestStatus status = target->GetQuestStatus(qInfo->GetQuestId());

                    switch (status)
                    {
                        case QUEST_STATUS_COMPLETE:
                            statusStr = handler->GetTrinityString(LANG_COMMAND_QUEST_COMPLETE);
                            break;
                        case QUEST_STATUS_INCOMPLETE:
                            statusStr = handler->GetTrinityString(LANG_COMMAND_QUEST_ACTIVE);
                            break;
                        case QUEST_STATUS_REWARDED:
                            statusStr = handler->GetTrinityString(LANG_COMMAND_QUEST_REWARDED);
                            break;
                        default:
                            break;
                    }
                }

                if (handler->GetSession())
                    handler->PSendSysMessage(LANG_QUEST_LIST_CHAT, qInfo->GetQuestId(), qInfo->GetQuestId(), qInfo->GetQuestLevel(), title.c_str(), statusStr);
                else
                    handler->PSendSysMessage(LANG_QUEST_LIST_CONSOLE, qInfo->GetQuestId(), title.c_str(), statusStr);

                if (!found)
                    found = true;
            }
        }

        if (!found)
            handler->SendSysMessage(LANG_COMMAND_NOQUESTFOUND);

        return true;
    }

    static bool HandleLookupSkillCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        // can be NULL in console call
        Player* target = handler->getSelectedPlayer();

        std::string namePart = args;
        std::wstring wNamePart;

        if (!Utf8toWStr(namePart, wNamePart))
            return false;

        // converting string that we try to find to lower case
        wstrToLower(wNamePart);

        bool found = false;
        uint32 count = 0;
        uint32 maxResults = sWorld->getIntConfig(CONFIG_MAX_RESULTS_LOOKUP_COMMANDS);

        // Search in SkillLine.dbc
        for (uint32 id = 0; id < sSkillLineStore.GetNumRows(); id++)
        {
            SkillLineEntry const* skillInfo = sSkillLineStore.LookupEntry(id);
            if (skillInfo)
            {
                std::string name = skillInfo->name;
                if (name.empty())
                    continue;

                if (!Utf8FitTo(name, wNamePart))
                    continue;

                if (maxResults && count++ == maxResults)
                {
                    handler->PSendSysMessage(LANG_COMMAND_LOOKUP_MAX_RESULTS, maxResults);
                    return true;
                }

                char valStr[50] = "";
                char const* knownStr = "";
                if (target && target->HasSkill(id))
                {
                    knownStr = handler->GetTrinityString(LANG_KNOWN);
                    uint32 curValue = target->GetPureSkillValue(id);
                    uint32 maxValue  = target->GetPureMaxSkillValue(id);
                    uint32 permValue = target->GetSkillPermBonusValue(id);
                    uint32 tempValue = target->GetSkillTempBonusValue(id);

                    char const* valFormat = handler->GetTrinityString(LANG_SKILL_VALUES);
                    snprintf(valStr, 50, valFormat, curValue, maxValue, permValue, tempValue);
                }

                // send skill in "id - [namedlink locale]" format
                if (handler->GetSession())
                    handler->PSendSysMessage(LANG_SKILL_LIST_CHAT, id, id, name.c_str(), "", knownStr, valStr);
                else
                    handler->PSendSysMessage(LANG_SKILL_LIST_CONSOLE, id, name.c_str(), "", knownStr, valStr);

                if (!found)
                    found = true;
            }
        }
        if (!found)
            handler->SendSysMessage(LANG_COMMAND_NOSKILLFOUND);

        return true;
    }

    static bool HandleLookupSpellCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        // can be NULL at console call
        Player* target = handler->getSelectedPlayer();

        std::string namePart = args;
        std::wstring wNamePart;

        if (!Utf8toWStr(namePart, wNamePart))
            return false;

        // converting string that we try to find to lower case
        wstrToLower(wNamePart);

        bool found = false;
        uint32 count = 0;
        uint32 maxResults = sWorld->getIntConfig(CONFIG_MAX_RESULTS_LOOKUP_COMMANDS);

        // Search in Spell.dbc
        for (uint32 id = 0; id < sSpellMgr->GetSpellInfoStoreSize(); id++)
        {
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(id);
            if (spellInfo)
            {
                std::string name = spellInfo->SpellName;
                if (name.empty())
                    continue;

                if (!Utf8FitTo(name, wNamePart))
                    continue;

                if (maxResults && count++ == maxResults)
                {
                    handler->PSendSysMessage(LANG_COMMAND_LOOKUP_MAX_RESULTS, maxResults);
                    return true;
                }

                bool known = target && target->HasSpell(id);
                bool learn = (spellInfo->Effects[0].Effect == SPELL_EFFECT_LEARN_SPELL);

                SpellInfo const* learnSpellInfo = sSpellMgr->GetSpellInfo(spellInfo->Effects[0].TriggerSpell);

                uint32 talentCost = GetTalentSpellCost(id);

                bool talent = (talentCost > 0);
                bool passive = spellInfo->IsPassive();
                bool active = target && target->HasAura(id);

                // unit32 used to prevent interpreting uint8 as char at output
                // find rank of learned spell for learning spell, or talent rank
                uint32 rank = talentCost ? talentCost : learn && learnSpellInfo ? learnSpellInfo->GetRank() : spellInfo->GetRank();

                // send spell in "id - [name, rank N] [talent] [passive] [learn] [known]" format
                std::ostringstream ss;
                if (handler->GetSession())
                    ss << id << " - |cffffffff|Hspell:" << id << "|h[" << name;
                else
                    ss << id << " - " << name;

                // include rank in link name
                if (rank)
                    ss << handler->GetTrinityString(LANG_SPELL_RANK) << rank;

                if (handler->GetSession())
                    ss << "]|h|r";

                if (talent)
                    ss << handler->GetTrinityString(LANG_TALENT);
                if (passive)
                    ss << handler->GetTrinityString(LANG_PASSIVE);
                if (learn)
                    ss << handler->GetTrinityString(LANG_LEARN);
                if (known)
                    ss << handler->GetTrinityString(LANG_KNOWN);
                if (active)
                    ss << handler->GetTrinityString(LANG_ACTIVE);

                handler->SendSysMessage(ss.str().c_str());

                if (!found)
                    found = true;
            }
        }
        if (!found)
            handler->SendSysMessage(LANG_COMMAND_NOSPELLFOUND);

        return true;
    }

    static bool HandleLookupSpellIdCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        // can be NULL at console call
        Player* target = handler->getSelectedPlayer();

        uint32 id = atoi((char*)args);

        bool found = false;
        uint32 count = 0;
        uint32 maxResults = 1;

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(id);
        if (spellInfo)
        {
            int locale = handler->GetSessionDbcLocale();
            std::string name = spellInfo->SpellName;
            if (name.empty())
            {
                handler->SendSysMessage(LANG_COMMAND_NOSPELLFOUND);
                return true;
            }

            if (locale < TOTAL_LOCALES)
            {
                if (maxResults && count++ == maxResults)
                {
                    handler->PSendSysMessage(LANG_COMMAND_LOOKUP_MAX_RESULTS, maxResults);
                    return true;
                }

                bool known = target && target->HasSpell(id);
                bool learn = (spellInfo->Effects[0].Effect == SPELL_EFFECT_LEARN_SPELL);

                SpellInfo const* learnSpellInfo = sSpellMgr->GetSpellInfo(spellInfo->Effects[0].TriggerSpell);

                uint32 talentCost = GetTalentSpellCost(id);

                bool talent = (talentCost > 0);
                bool passive = spellInfo->IsPassive();
                bool active = target && target->HasAura(id);

                // unit32 used to prevent interpreting uint8 as char at output
                // find rank of learned spell for learning spell, or talent rank
                uint32 rank = talentCost ? talentCost : learn && learnSpellInfo ? learnSpellInfo->GetRank() : spellInfo->GetRank();

                // send spell in "id - [name, rank N] [talent] [passive] [learn] [known]" format
                std::ostringstream ss;
                if (handler->GetSession())
                    ss << id << " - |cffffffff|Hspell:" << id << "|h[" << name;
                else
                    ss << id << " - " << name;

                // include rank in link name
                if (rank)
                    ss << handler->GetTrinityString(LANG_SPELL_RANK) << rank;

                if (handler->GetSession())
                    ss << ' ' << localeNames[locale] << "]|h|r";
                else
                    ss << ' ' << localeNames[locale];

                if (talent)
                    ss << handler->GetTrinityString(LANG_TALENT);
                if (passive)
                    ss << handler->GetTrinityString(LANG_PASSIVE);
                if (learn)
                    ss << handler->GetTrinityString(LANG_LEARN);
                if (known)
                    ss << handler->GetTrinityString(LANG_KNOWN);
                if (active)
                    ss << handler->GetTrinityString(LANG_ACTIVE);

                handler->SendSysMessage(ss.str().c_str());

                if (!found)
                    found = true;
            }
        }

        if (!found)
            handler->SendSysMessage(LANG_COMMAND_NOSPELLFOUND);

        return true;
    }

    static bool HandleLookupTaxiNodeCommand(ChatHandler* handler, const char * args)
    {
        if (!*args)
            return false;

        std::string namePart = args;
        std::wstring wNamePart;

        if (!Utf8toWStr(namePart, wNamePart))
            return false;

        // converting string that we try to find to lower case
        wstrToLower(wNamePart);

        bool found = false;
        uint32 count = 0;
        uint32 maxResults = sWorld->getIntConfig(CONFIG_MAX_RESULTS_LOOKUP_COMMANDS);

        // Search in TaxiNodes.dbc
        for (uint32 id = 0; id < sTaxiNodesStore.GetNumRows(); id++)
        {
            TaxiNodesEntry const* nodeEntry = sTaxiNodesStore.LookupEntry(id);
            if (nodeEntry)
            {
                std::string name = nodeEntry->name;
                if (name.empty())
                    continue;

                if (!Utf8FitTo(name, wNamePart))
                    continue;

                if (maxResults && count++ == maxResults)
                {
                    handler->PSendSysMessage(LANG_COMMAND_LOOKUP_MAX_RESULTS, maxResults);
                    return true;
                }

                // send taxinode in "id - [name] (Map:m X:x Y:y Z:z)" format
                if (handler->GetSession())
                    handler->PSendSysMessage(LANG_TAXINODE_ENTRY_LIST_CHAT, id, id, name.c_str(), "",
                        nodeEntry->map_id, nodeEntry->x, nodeEntry->y, nodeEntry->z);
                else
                    handler->PSendSysMessage(LANG_TAXINODE_ENTRY_LIST_CONSOLE, id, name.c_str(), "",
                        nodeEntry->map_id, nodeEntry->x, nodeEntry->y, nodeEntry->z);

                if (!found)
                    found = true;
            }
        }
        if (!found)
            handler->SendSysMessage(LANG_COMMAND_NOTAXINODEFOUND);

        return true;
    }

    // Find tele in game_tele order by name
    static bool HandleLookupTeleCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
        {
            handler->SendSysMessage(LANG_COMMAND_TELE_PARAMETER);
            handler->SetSentErrorMessage(true);
            return false;
        }

        char const* str = strtok((char*)args, " ");
        if (!str)
            return false;

        std::string namePart = str;
        std::wstring wNamePart;

        if (!Utf8toWStr(namePart, wNamePart))
            return false;

        // converting string that we try to find to lower case
        wstrToLower(wNamePart);

        std::ostringstream reply;
        uint32 count = 0;
        uint32 maxResults = sWorld->getIntConfig(CONFIG_MAX_RESULTS_LOOKUP_COMMANDS);
        bool limitReached = false;

        GameTeleContainer const & teleMap = sObjectMgr->GetGameTeleMap();
        for (GameTeleContainer::const_iterator itr = teleMap.begin(); itr != teleMap.end(); ++itr)
        {
            GameTele const* tele = &itr->second;

            if (tele->wnameLow.find(wNamePart) == std::wstring::npos)
                continue;

            if (maxResults && count++ == maxResults)
            {
                limitReached = true;
                break;
            }

            if (handler->GetSession())
                reply << "  |cffffffff|Htele:" << itr->first << "|h[" << tele->name << "]|h|r\n";
            else
                reply << "  " << itr->first << ' ' << tele->name << "\n";
        }

        if (reply.str().empty())
            handler->SendSysMessage(LANG_COMMAND_TELE_NOLOCATION);
        else
            handler->PSendSysMessage(LANG_COMMAND_TELE_LOCATION, reply.str().c_str());

        if (limitReached)
            handler->PSendSysMessage(LANG_COMMAND_LOOKUP_MAX_RESULTS, maxResults);

        return true;
    }

    static bool HandleLookupTitleCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        // can be NULL in console call
        Player* target = handler->getSelectedPlayer();

        // title name have single string arg for player name
        char const* targetName = target ? target->GetName().c_str() : "NAME";

        std::string namePart = args;
        std::wstring wNamePart;

        if (!Utf8toWStr(namePart, wNamePart))
            return false;

        // converting string that we try to find to lower case
        wstrToLower(wNamePart);

        uint32 counter = 0;                                     // Counter for figure out that we found smth.
        uint32 maxResults = sWorld->getIntConfig(CONFIG_MAX_RESULTS_LOOKUP_COMMANDS);

        // Search in CharTitles.dbc
        for (uint32 id = 0; id < sCharTitlesStore.GetNumRows(); id++)
        {
            CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(id);
            if (titleInfo)
            {
                std::string name = titleInfo->name;
                if (name.empty())
                    continue;

                if (!Utf8FitTo(name, wNamePart))
                    continue;

                if (maxResults && counter == maxResults)
                {
                    handler->PSendSysMessage(LANG_COMMAND_LOOKUP_MAX_RESULTS, maxResults);
                    return true;
                }

                char const* knownStr = target && target->HasTitle(titleInfo) ? handler->GetTrinityString(LANG_KNOWN) : "";

                char const* activeStr = target && target->GetUInt32Value(PLAYER_CHOSEN_TITLE) == titleInfo->bit_index
                    ? handler->GetTrinityString(LANG_ACTIVE)
                    : "";

                char titleNameStr[80];
                snprintf(titleNameStr, 80, name.c_str(), targetName);

                // send title in "id (idx:idx) - [namedlink locale]" format
                if (handler->GetSession())
                    handler->PSendSysMessage(LANG_TITLE_LIST_CHAT, id, titleInfo->bit_index, id, titleNameStr, "", knownStr, activeStr);
                else
                    handler->PSendSysMessage(LANG_TITLE_LIST_CONSOLE, id, titleInfo->bit_index, titleNameStr, "", knownStr, activeStr);

                ++counter;
            }
        }
        if (counter == 0)  // if counter == 0 then we found nth
            handler->SendSysMessage(LANG_COMMAND_NOTITLEFOUND);

        return true;
    }


    static bool HandleLookupPlayersGUIDCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        char* arg = strtok((char*)args, "");
        if (!arg)
            return false;

        std::string ARGs = arg;
        if (!atoi(ARGs.c_str()))
            return false;

        LoginDatabase.EscapeString(ARGs);

        auto result = CharacterDatabase.PQuery("SELECT name FROM characters WHERE GUID IN (%s)", arg);

        if (result)
        {

            std::ostringstream ostream2{ "" };
            ostream2 << "-- START Characters matching GUID # " << "|cffff0000|Harea:" << "|h" << ARGs << "|h|r" << " --\n";

            do
            {
                if (auto fields2 = result->Fetch())
                {
                    auto data{ fields2[0].GetString() };
                    ostream2 << " Found: '" << "|cffff0000|Harea:" << "|h" << data << "|h|r" << "' \n";
                }
            } while (result->NextRow());

            ostream2 << "-- END Characters matching GUID # " << "|cffff0000|Harea:" << "|h" << ARGs << "|h|r" << " --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->SendSysMessage(log);
        }
        else
        {
            handler->PSendSysMessage("Could not find Characters matching GUID # %s", ARGs);
            return true;
        }
        return true;
    }

    //
    static bool HandleLookupAccountIdCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        char* arg = strtok((char*)args, "");
        if (!arg)
            return false;

        std::string ARGs = arg;
        if (!atoi(ARGs.c_str()))
            return false;

        LoginDatabase.EscapeString(ARGs);

        auto result = LoginDatabase.PQuery("SELECT username FROM account WHERE id IN (%s)", arg);

        if (result)
        {

            std::ostringstream ostream2{ "" };
            ostream2 << "-- START Accounts matching Acc ID # " << "|cffff0000|Harea:" << "|h" << ARGs << "|h|r" << " --\n";

            do
            {
                if (auto fields2 = result->Fetch())
                {
                    auto data{ fields2[0].GetString() };
                    ostream2 << " Found: '" << "|cffff0000|Harea:" << "|h" << data << "|h|r" << "' \n";
                }
            } while (result->NextRow());

            ostream2 << "-- END Accounts matching Acc ID # " << "|cffff0000|Harea:" << "|h" << ARGs << "|h|r" << " --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->SendSysMessage(log);
        }
        else
        {
            handler->PSendSysMessage("Could not find Account matching ID # %s", ARGs);
            return true;
        }
        return true;
    }

    static bool HandleLookupMapCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        std::string namePart = args;
        std::wstring wNamePart;

        if (!Utf8toWStr(namePart, wNamePart))
            return false;

        wstrToLower(wNamePart);

        uint32 counter = 0;
        uint32 maxResults = sWorld->getIntConfig(CONFIG_MAX_RESULTS_LOOKUP_COMMANDS);
        uint8 locale = handler->GetSession() ? handler->GetSession()->GetSessionDbcLocale() : sWorld->GetDefaultDbcLocale();

        // search in Map.dbc
        for (uint32 id = 0; id < sMapStore.GetNumRows(); id++)
        {
            if (MapEntry const* mapInfo = sMapStore.LookupEntry(id))
            {
                std::string name = mapInfo->name;
                if (name.empty())
                    continue;

                if (Utf8FitTo(name, wNamePart) && locale < TOTAL_LOCALES)
                {
                    if (maxResults && counter == maxResults)
                    {
                        handler->PSendSysMessage(LANG_COMMAND_LOOKUP_MAX_RESULTS, maxResults);
                        return true;
                    }

                    std::ostringstream ss;
                    ss << id << " - [" << name << ']';

                    if (mapInfo->IsContinent())
                        ss << handler->GetTrinityString(LANG_CONTINENT);

                    switch (mapInfo->map_type)
                    {
                        case MAP_INSTANCE:
                            ss << handler->GetTrinityString(LANG_INSTANCE);
                            break;
                        case MAP_RAID:
                            ss << handler->GetTrinityString(LANG_RAID);
                            break;
                        case MAP_BATTLEGROUND:
                            ss << handler->GetTrinityString(LANG_BATTLEGROUND);
                            break;
                        case MAP_ARENA:
                            ss << handler->GetTrinityString(LANG_ARENA);
                            break;
                    }

                    handler->SendSysMessage(ss.str().c_str());

                    ++counter;
                }
            }
        }

        if (!counter)
            handler->SendSysMessage(LANG_COMMAND_NOMAPFOUND);

        return true;
    }

    static bool HandleLookupPlayerIpCommand(ChatHandler* handler, char const* args)
    {
        std::string ip;
        int32 limit;
        char* limitStr;

        Player* target = handler->getSelectedPlayer();
        if (!*args)
        {
            // NULL only if used from console
            if (!target || target == handler->GetSession()->GetPlayer())
                return false;

            ip = target->GetSession()->GetMaskedIP();
            limit = -1;
        }
        else
        {
            ip = strtok((char*)args, " ");
            limitStr = strtok(NULL, " ");
            limit = limitStr ? atoi(limitStr) : -1;
        }
        PreparedStatement* pre_stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_IP_MASKING_IP_FROM_MASK);
        pre_stmt->setString(0, ip);
        PreparedQueryResult pre_result = LoginDatabase.Query(pre_stmt);

        std::string actual_ip{""};

        if (pre_result)
            actual_ip = (*pre_result)[0].GetString();

            PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_BY_IP);
            stmt->setString(0, actual_ip);
            PreparedQueryResult result = LoginDatabase.Query(stmt);

        return LookupPlayerSearchCommand(result, limit, handler);
    }

    static bool HandleLookupPlayerAccountCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        std::string account = strtok((char*)args, " ");
        char* limitStr = strtok(NULL, " ");
        int32 limit = limitStr ? atoi(limitStr) : -1;

        if (!AccountMgr::normalizeString
            (account))
            return false;

        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_LIST_BY_NAME);
        stmt->setString(0, account);
        PreparedQueryResult result = LoginDatabase.Query(stmt);

        return LookupPlayerSearchCommand(result, limit, handler);
    }

    static bool HandleLookupHistoryIPCommand(ChatHandler* handler, char const* args)
    {
        if (!args)
            return false;

        char* arg = strtok((char*)args, " ");

        if (!arg)
            return false;

        // ORDER BY DateTime DESC LIMIT %u;", ARGs, limit))
        std::string IP = arg;
        char* limitStr = strtok(NULL, " ");
        int32 limit = limitStr ? atoi(limitStr) : 25;

        LoginDatabase.EscapeString(IP);
        if (auto result = LoginDatabase.PQuery("SELECT aih.Account, aih.date_recent, aih.date_first, aa.gmlevel FROM(account_ip_history aih LEFT JOIN ip_masking im ON im.IP = aih.IP LEFT JOIN account_access aa ON aa.id = aih.Account) WHERE im.Mask = %s ORDER BY aih.date_recent DESC LIMIT %u", IP.c_str(), limit))
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START HISTORY of IP " << "|cffff0000|Harea:" << "|h" << IP << "|h|r" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";

            do
            {
                if (auto fields2 = result->Fetch())
                {
                    uint64 account{ fields2[0].GetUInt64() };

                    uint32 gmlevel{ fields2[3].GetUInt32() };
                    if (auto s = handler->GetSession())
                    if (AccountMgr::IsPlayerAccount(gmlevel) || AccountMgr::IsHeadGMAccount(s->GetSecurity()))
                    {
                        ostream2 << "Account: " << "|cffADFF2F|Harea:" << "|h" << account << "|h|r" << ", ";
                        ostream2 << "Recent: " << "|cffADFF2F|Harea:" << "|h[" << fields2[1].GetCString() << "]|h|r" << ", ";
                        ostream2 << "First: " "|cffADFF2F|Harea:" << "|h[" << fields2[2].GetCString() << "]|h|r" << ".\n";
                    }
                }

            } while (result->NextRow());

            ostream2 << "-- END HISTORY of IP " << "|cffff0000|Harea:" << "|h" << IP << "|h|r" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->SendSysMessage(log);
        }
        else
        {
            handler->PSendSysMessage("Could not find Account history for IP %s", IP);
            return true;
        }

        return true;
    }

    static bool HandleLookupHistoryPvPKicksCommand(ChatHandler* handler, char const* args)
    {
        if (!args)
            return false;

        char* arg = strtok((char*)args, " ");

        if (!arg)
            return false;

        // ORDER BY DateTime DESC LIMIT %u;", ARGs, limit))
        std::string GUID = arg;
        char* limitStr = strtok(NULL, " ");

        uint64 guid_int{ uint64(atoi(GUID.c_str())) };

        int32 limit = limitStr ? atoi(limitStr) : 25;

        uint32 rowcount{ 0 };
        std::ostringstream ostream2{ "" };
        LoginDatabase.EscapeString(GUID);
        if (auto result = CharacterDatabase.PQuery("SELECT * FROM character_pvp_detection_data WHERE eventType = 1 AND playerGUID = %u ORDER BY DateAndTime DESC LIMIT %u;", guid_int, limit))
        {
            ostream2 << "-- START HISTORY of PVP Cast Interrupts by character " << "|cffff0000|Harea:" << "|h" << guid_int << "|h|r" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " -- \n";
            do
            {
                if (auto fields2 = result->Fetch())
                {
                    rowcount += 1;

                    /*
                        0   //playerGUID
                        1   //eventType
                        2   //DateAndTime filled already
                        3   //data1 = match GUID, NEEDS ACTUAL DB GUID
                        4   //data2 = bracket size
                        5   //data3 = team of player who just interrupted a spell
                        6   //data4 = team of player who just got interrupted
                        7   //data5 = unit who just got interrupted, only set to a value if a player was interrupted.
                        8   //data6 = spell from victim which got interrupted
                        9   //data7 = spell used to interrupt victim's spell
                        10  //data8 = true or false as to whether or not this interrupt succeeded. this function is for a successful interrupt so it is always 1.
                        11  //data9 = reaction time between server-side spell start and current time. NOTE: both of these time values reference the time of server startup as 0 and counts up in milliseconds.
                        12  //data10 = cast completion percentage, something set by kickbots under certain conditions.
                        13  //data11 = Interrupting Player's latency at this given moment
                        14  //data12 = GUID of potential kill target player friendly to the interrupt victim
                        15  //data13 = PCT health of potential kill target player friendly to the interrupt victim
                        16  //data14 = Actual health of potential kill target player friendly to the interrupt victim
                    */

                    std::ostringstream ostream_attempt { "" };

                    auto DateAndTime                            { fields2[2].GetCString()   };
                    auto MatchGUID                              { fields2[3].GetUInt64()    };
                    auto team_size                              { fields2[4].GetUInt32()     };
                    auto team_of_interrupter                    { fields2[5].GetUInt32()    };
                    auto team_of_interrupted                    { fields2[6].GetUInt32()    };
                    auto interrupt_target                       { fields2[7].GetUInt64()    };
                    auto interrupted_spell                      { fields2[8].GetUInt32()    };
                    auto interrupting_spell                     { fields2[9].GetUInt64()    };
                    auto interrupt_success                      { fields2[10].GetBool()     };
                    auto interrupt_time_since_cast_start        { fields2[11].GetUInt32()   };
                    auto interrupted_spell_cast_percentage      { fields2[12].GetUInt32()   };
                    auto interrupter_latency                    { fields2[13].GetUInt32()   };

                    auto pot_kill_guid                          { fields2[14].GetUInt64()   };
                    auto pot_kill_hp_pct                        { fields2[15].GetUInt32()   };
                    auto pot_kill_actual_hp                     { fields2[16].GetUInt32()   };


                    ostream_attempt
                        << "|cffADFF2F|Harea:" << "|h[" << DateAndTime << "]|h|r "
                        << "|cffADFF2F|Harea:" << "|h(" << team_size << "vs" << team_size << ")|h|r "//2vs2
                        << "team |cffADFF2F|Harea:" << "|h" << team_of_interrupter << "|h|r "
                        << "vs opponent |cffADFF2F|Harea:" << "|h" << team_of_interrupted << "|h|r: "
                        << "|cff71d5ff|Hspell:" << interrupting_spell << "|h[";

                    if (auto info = sSpellMgr->GetSpellInfo(interrupting_spell))
                        ostream_attempt << info->SpellName; 

                    ostream_attempt << "]|h|r "
                        << "interrupt on "
                        << "|cffADFF2F|Harea:" << "|h" << interrupt_target << "|h|r" << "\'s "
                        << "|cff71d5ff|Hspell:" << interrupted_spell << "|h[";

                    if (auto info = sSpellMgr->GetSpellInfo(interrupted_spell))
                        ostream_attempt << info->SpellName;

                    ostream_attempt << "]|h|r "
                        << ": " 
                        << "|cffff0000|Harea:" << "|h" << interrupt_time_since_cast_start << "|h|r"
                        << " ms reaction ("
                        << "|cffff0000|Harea:" << "|h" << interrupted_spell_cast_percentage << "|h|r"
                        << "% cast) on "
                        << "|cffff0000|Harea:" << "|h" << interrupter_latency << "|h|r"
                        << "ms ping. potential kill: " 
                        << "|cffADFF2F|Harea:" << "|h" << pot_kill_guid << "|h|r"
                        << " at " 
                        << "|cffADFF2F|Harea:" << "|h" << pot_kill_actual_hp << "|h|r"
                        << " hp (" 
                        << "|cffADFF2F|Harea:" << "|h" << pot_kill_hp_pct << "%|h|r) \n";



                    auto string_attempt { ostream_attempt.str() };
                    auto log_attempt { string_attempt.c_str() };
                    ostream2 << string_attempt;


                }

            } while (result->NextRow());


            ostream2 << "-- END HISTORY of PVP Cast Interrupts by character " << "|cffff0000|Harea:" << "|h" << guid_int << "|h|r" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " -- \n";

            auto resulting_string{ ostream2.str() };

            auto log{ resulting_string.c_str() };
            handler->SendSysMessage(log);
        }
        else
        {
            handler->PSendSysMessage("Could not find Account history for IP %s", GUID);
            return true;
        }

        return true;
    }


    static bool HandleLookupHistoryPvPAccuracyCommand(ChatHandler* handler, char const* args)
    {
        if (!args)
            return false;

        char* arg = strtok((char*)args, " ");

        if (!arg)
            return false;

        // ORDER BY DateTime DESC LIMIT %u;", ARGs, limit))
        std::string GUID = arg;
        char* limitStr = strtok(NULL, " ");

        uint64 guid_int{ uint64(atoi(GUID.c_str())) };

        int32 limit = limitStr ? atoi(limitStr) : 25;

        std::ostringstream ostream2{ "" };
        LoginDatabase.EscapeString(GUID);
        if (auto result = CharacterDatabase.PQuery("SELECT * FROM character_pvp_detection_data WHERE eventType = 2 AND playerGUID = %u ORDER BY DateAndTime DESC LIMIT %u;", guid_int, limit))
        {
            ostream2 << "-- START HISTORY of PVP spell placement accuracy by character " << "|cffff0000|Harea:" << "|h" << guid_int << "|h|r" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " -- \n";
            do
            {
                if (auto fields2 = result->Fetch())
                {

                    std::ostringstream ostream_attempt{ "" };

                    auto DateAndTime                { fields2[2].GetCString() };
                    auto closest_player_guid        { fields2[3].GetUInt64() };
                    auto team_size                  { fields2[4].GetUInt32() };
                    auto team_of_placer             { fields2[5].GetUInt32() };
                    auto spell_placed               { fields2[6].GetUInt32() };
                    auto placement_latency          { fields2[7].GetUInt32() };
                    auto dist_10kx_nearest_player   { fields2[8].GetUInt32() };
                    auto dist_caster_to_placement   { fields2[9].GetUInt32() };
                    float readable_dist{ ((float(dist_10kx_nearest_player) / 10000.f)) };

                    ostream_attempt
                        << "|cffADFF2F|Harea:" << "|h[" << DateAndTime << "]|h|r "
                        << "|cffADFF2F|Harea:" << "|h(" << team_size << "v" << team_size << "|h|r, "//2vs2
                        << "team |cffADFF2F|Harea:" << "|h" << team_of_placer << "|h|r): "
                        << "|cff71d5ff|Hspell:" << spell_placed << "|h[";

                    if (auto info = sSpellMgr->GetSpellInfo(spell_placed))
                        ostream_attempt << info->SpellName;

                    ostream_attempt << "]|h|r "
                        << "placed within "
                        << "|cffff0000|Harea:" << "|h" << readable_dist << "|h|r "
                        << "yards of player " 
                        << "|cffADFF2F|Harea:" << "|h" << closest_player_guid << "|h|r"
                        << " from " 
                        << "|cffff0000|Harea:" << "|h" << float(dist_caster_to_placement) << "|h|r"
                        << " yards away. "
                        << "Latency: " 
                        << "|cffADFF2F|Harea:" << "|h" << placement_latency << "|h|r"
                        << " ms ping.\n";


                    auto string_attempt{ ostream_attempt.str() };
                    auto log_attempt{ string_attempt.c_str() };
                    ostream2 << string_attempt;


                }

            } while (result->NextRow());


            ostream2 << "-- END HISTORY of PVP spell placement accuracy by character " << "|cffff0000|Harea:" << "|h" << guid_int << "|h|r" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " -- \n";

            auto resulting_string{ ostream2.str() };

            auto log{ resulting_string.c_str() };
            handler->SendSysMessage(log);
        }
        else
        {
            handler->PSendSysMessage("Could not find Account history for IP %s", GUID);
            return true;
        }

        return true;
    }


    static bool HandleLookupHistoryAccountCommand(ChatHandler* handler, char const* args)
    {
        if (!args)
            return false;

        char* arg = strtok((char*)args, " ");

        if (!arg)
            return false;

        // ORDER BY DateTime DESC LIMIT %u;", ARGs, limit))
        std::string AccStr = arg;
        char* limitStr = strtok(NULL, " ");
        int32 limit = limitStr ? atoi(limitStr) : 25;

        LoginDatabase.EscapeString(AccStr);
        //
        if (auto result = LoginDatabase.PQuery("SELECT im.Mask, aih.date_recent, aih.date_first, aa.gmlevel FROM(account_ip_history aih LEFT JOIN ip_masking im ON im.IP = aih.IP LEFT JOIN account_access aa ON aa.id = aih.Account) WHERE aih.Account = %s ORDER BY aih.date_recent DESC LIMIT %u;", AccStr.c_str(), limit))
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START HISTORY of Account " << "|cffff0000|Harea:" << "|h" << AccStr << "|h|r" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";

            do
            {
                if (auto fields2 = result->Fetch())
                {
                    uint64 account{ fields2[0].GetUInt64() };
                    uint32 gmlevel{ fields2[3].GetUInt32() };

                    if (auto s = handler->GetSession())
                    if (AccountMgr::IsPlayerAccount(gmlevel) || AccountMgr::IsHeadGMAccount(s->GetSecurity()))
                    {
                        ostream2 << "IP: " << "|cffADFF2F|Harea:" << "|h" << account << "|h|r" << ", ";
                        ostream2 << "Recent: " << "|cffADFF2F|Harea:" << "|h[" << fields2[1].GetCString() << "]|h|r" << ", ";
                        ostream2 << "First: " "|cffADFF2F|Harea:" << "|h[" << fields2[2].GetCString() << "]|h|r" << ".\n";
                    }
                    
                }

            } while (result->NextRow());

            ostream2 << "-- END HISTORY of Account " << "|cffff0000|Harea:" << "|h" << AccStr << "|h|r" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->SendSysMessage(log);
        }
        else
        {
            handler->PSendSysMessage("Could not find IP history for Account %s", AccStr);
            return true;
        }

        return true;
    }

    static bool HandleLookupHistoryCommandsCommand(ChatHandler* handler, char const* args)
    {
        if (!args)
            return false;

        char* arg = strtok((char*)args, " ");

        if (!arg)
            return false;

        // ORDER BY DateTime DESC LIMIT %u;", ARGs, limit))
        std::string AccStr = arg;
        char* limitStr = strtok(NULL, " ");
        int32 limit = limitStr ? atoi(limitStr) : 25;

        LoginDatabase.EscapeString(AccStr);
        //
        if (auto result = CharacterDatabase.PQuery("SELECT * FROM command_log cl WHERE cl.Account = %s ORDER BY cl.DateTime DESC LIMIT %u;", AccStr.c_str(), limit))
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START COMMAND HISTORY of Account " << "|cffff0000|Harea:" << "|h" << AccStr << "|h|r" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";

            do
            {
                if (auto fields2 = result->Fetch())
                {
                    auto DateTime{ fields2[0].GetCString() };
                    auto AccountId { fields2[1].GetUInt32() };
                    auto command{ fields2[2].GetString() };
                    auto target_name{ fields2[3].GetString() };
                    uint64 target_guid { fields2[4].GetUInt32() };

                    if (auto s = handler->GetSession())
                        {
                            ostream2 << "|cffADFF2F|Harea:" << "|h[" << DateTime << "]|h|r";
                            ostream2 << ": \'" << "|cffff0000|Harea:" << "|h" << command << "|h|r\'" << " executed targeting ";
                            ostream2 << "|cffADFF2F|Harea:" << "|h[" << target_name << "] (GUID: "<< target_guid << ")|h|r" 
                                << ".\n";
                        }

                }

            } while (result->NextRow());

            ostream2 << "-- END COMMAND HISTORY of Account " << "|cffff0000|Harea:" << "|h" << AccStr << "|h|r" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->SendSysMessage(log);
        }
        else
        {
            handler->PSendSysMessage("Could not find command history for Account %s", AccStr);
            return true;
        }

        return true;
    }


    static bool HandleLookupHistoryTradeToCommand(ChatHandler* handler, char const* args)
    {
        if (!args)
            return false;

        char* arg = strtok((char*)args, " ");

        if (!arg)
            return false;

        // ORDER BY DateTime DESC LIMIT %u;", ARGs, limit))
        std::string ARGs = arg;
        if (!atoi(ARGs.c_str()))
            return false;
        char* limitStr = strtok(NULL, " ");
        int32 limit = limitStr ? atoi(limitStr) : 25;
        LoginDatabase.EscapeString(ARGs);

        if (auto result = CharacterDatabase.PQuery("SELECT Log_GUID, DateTime,p1_GUID AS sender,p1_money_offer AS money,(IF(p1_item1_entry > 0, 1, 0)+ IF(p1_item2_entry > 0, 1, 0)+ IF(p1_item3_entry > 0, 1, 0)+ IF(p1_item4_entry > 0, 1, 0)+ IF(p1_item5_entry > 0, 1, 0)+ IF(p1_item6_entry > 0, 1, 0)) AS item_count FROM char_history_trade WHERE(p2_GUID IN(%s)) UNION SELECT Log_GUID,DateTime,p2_GUID AS sender,p2_money_offer AS money,(IF(p1_item1_entry > 0, 1, 0)+ IF(p2_item7_entry > 0, 1, 0)+ IF(p2_item8_entry > 0, 1, 0)+ IF(p2_item9_entry > 0, 1, 0)+ IF(p2_item10_entry > 0, 1, 0)+ IF(p2_item11_entry > 0, 1, 0)) AS item_count FROM char_history_trade WHERE p1_GUID IN (%s) ORDER BY DateTime DESC LIMIT %u;", ARGs, ARGs, limit))
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START TRADE HISTORY (RECEIVE) of Player " << "|cffff0000|Harea:" << "|h" << ARGs << "|h|r" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";

            do
            {
                if (auto fields2 = result->Fetch())
                {
                    uint64 trade_guid   { fields2[0].GetUInt64()    };
                    auto date           { fields2[1].GetCString()   };
                    auto from_guid      { fields2[2].GetUInt64()    };
                    auto money_from     { fields2[3].GetUInt64()    };
                    auto item_count     { fields2[4].GetUInt32()    };

                    ostream2 << "Trade no. " << "|cffADFF2F|Harea:" << "|h" << trade_guid << "|h|r" << ", ";
                    ostream2 << "From player " << "|cffADFF2F|Harea:" << "|h" << from_guid << "|h|r" << ", ";
                    ostream2 << "Occured: " << "|cffADFF2F|Harea:" << "|h[" << date << "]|h|r" << ", ";
                    ostream2 << "Received: " << "|cffADFF2F|Harea:" << "|h" << moneyToString(money_from) << "|h|r" << ", ";
                    ostream2 << "and " "|cffADFF2F|Harea:" << "|h" << item_count << "|h|r items" << ".\n";
                }

            } while (result->NextRow());

            ostream2 << "-- END TRADE HISTORY (RECEIVE) of Player " << "|cffff0000|Harea:" << "|h" << ARGs << "|h|r" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->SendSysMessage(log);
        }
        else
        {
            handler->PSendSysMessage("Could not find TRADE HISTORY (RECEIVE) for account %s", ARGs);
            return true;
        }
        return true;
    }
    static bool HandleLookupHistoryTradeFromCommand(ChatHandler* handler, char const* args)
    {
        if (!args)
            return false;

        char* arg = strtok((char*)args, " ");

        if (!arg)
            return false;

        // ORDER BY DateTime DESC LIMIT %u;", ARGs, limit))
                std::string ARGs = arg;
                if (!atoi(ARGs.c_str()))
                    return false;
        char* limitStr = strtok(NULL, " ");
        int32 limit = limitStr ? atoi(limitStr) : 25;
        LoginDatabase.EscapeString(ARGs);

        if (auto result = CharacterDatabase.PQuery("SELECT  Log_GUID,  DateTime,  p2_GUID AS recipient,  p1_money_offer AS money,  ( IF(p1_item1_entry > 0, 1, 0)  + IF(p1_item2_entry > 0, 1, 0) + IF(p1_item3_entry > 0, 1, 0) + IF(p1_item4_entry > 0, 1, 0) + IF(p1_item5_entry > 0, 1, 0) + IF(p1_item6_entry > 0, 1, 0) ) AS item_count FROM char_history_trade WHERE (p1_GUID IN (%s)) UNION  SELECT  Log_GUID,  DateTime, p1_GUID AS recipient,  p2_money_offer AS money,  ( IF(p1_item1_entry > 0, 1, 0)  + IF(p2_item7_entry > 0, 1, 0) + IF(p2_item8_entry > 0, 1, 0) + IF(p2_item9_entry > 0, 1, 0) + IF(p2_item10_entry > 0, 1, 0) + IF(p2_item11_entry > 0, 1, 0) ) AS item_count FROM char_history_trade WHERE p2_GUID IN (%s) ORDER BY DateTime DESC LIMIT %u;", ARGs, ARGs, limit))
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START TRADE HISTORY (SEND) of Player " << "|cffff0000|Harea:" << "|h" << ARGs << "|h|r" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";

            do
            {
                if (auto fields2 = result->Fetch())
                {
                    uint64 trade_guid{ fields2[0].GetUInt64() };
                    auto date{ fields2[1].GetCString() };
                    auto from_guid{ fields2[2].GetUInt64() };
                    auto money_from{ fields2[3].GetUInt64() };
                    auto item_count{ fields2[4].GetUInt32() };

                    ostream2 << "Trade no. " << "|cffADFF2F|Harea:" << "|h" << trade_guid << "|h|r" << ", ";
                    ostream2 << "To player " << "|cffADFF2F|Harea:" << "|h" << from_guid << "|h|r" << ", ";
                    ostream2 << "Occured: " << "|cffADFF2F|Harea:" << "|h[" << date << "]|h|r" << ", ";
                    ostream2 << "Sent: " << "|cffADFF2F|Harea:" << "|h" << moneyToString(money_from) << "|h|r" << ", ";
                    ostream2 << "and " "|cffADFF2F|Harea:" << "|h" << item_count << "|h|r items" << ".\n";
                }

            } while (result->NextRow());

            ostream2 << "-- END TRADE HISTORY (SEND) of Player " << "|cffff0000|Harea:" << "|h" << ARGs << "|h|r" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->SendSysMessage(log);
        }
        else
        {
            handler->PSendSysMessage("Could not find TRADE HISTORY (RECEIVE) for account %s", ARGs);
            return true;
        }
        return true;
    }
    static bool HandleLookupHistoryTradeDetailsCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        char* arg = strtok((char*)args, "");
        if (!arg)
            return false;

        std::string ARGs = arg;
        if (!atoi(ARGs.c_str()))
            return false;
        LoginDatabase.EscapeString(ARGs);

        if (auto result = CharacterDatabase.PQuery("SELECT * FROM char_history_trade WHERE Log_GUID IN(%s)", ARGs))
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START TRADE HISTORY (SPECIFIC) of Trade # " << "|cffff0000|Harea:" << "|h" << ARGs << "|h|r" << " --\n";

            do
            {
                if (auto fields2 = result->Fetch())
                {
                    auto trade_guid{ fields2[0].GetUInt64() };
                    auto date{ fields2[1].GetCString() };
                    auto p1_guid{ fields2[2].GetUInt64() };
                    auto p2_guid{ fields2[3].GetUInt64() };
                    auto p1_money{ fields2[4].GetUInt64() };
                    auto p2_money{ fields2[5].GetUInt32() };


                    ostream2 << "Trade no. " << "|cffADFF2F|Harea:" << "|h" << trade_guid << "|h|r" << ", ";
                    ostream2 << "Occured at: " << "|cffADFF2F|Harea:" << "|h" << date << "|h|r" << "\n";

                    ostream2 << "------------------------------------------------------------\n";
                    ostream2 << "Player " << "|cffADFF2F|Harea:" << "|h" << p1_guid << "|h|r ";
                    ostream2 << "offered: " << "|cffADFF2F|Harea:" << "|h" << moneyToString(p1_money) << "|h|r and Item(s):\n";
                    for (auto i = 0; i <= 5; ++i)
                    {
                        auto index{6 + (i*3)};
                        auto item_entry { fields2[index].GetUInt32() };
                        auto item_guid  { fields2[index+1].GetUInt64() };
                        auto item_count { fields2[index+2].GetUInt32() };

                            if (item_entry > 0)
                            {
                                ostream2 << "Item guid: " << "|cffADFF2F|Harea:" << "|h" << item_guid << "|h|r" << ", ";
                                ostream2 << " |cffADFF2F|Harea:" << "|h" << item_count << "|h|r x ";
                                ostream2 << sObjectMgr->GetItemTemplate(item_entry)->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << "\n";
                            }
                    }
                    ostream2 << "------------------------------------------------------------\n";
                    ostream2 << "Player " << "|cffADFF2F|Harea:" << "|h" << p2_guid << "|h|r ";
                    ostream2 << "offered: " << "|cffADFF2F|Harea:" << "|h" << moneyToString(p2_money) << "|h|r and Item(s):\n";
                    for (auto i = 0; i <= 5; ++i)
                    {
                        auto index{ 24 + (i * 3) };
                        auto item_entry{ fields2[index].GetUInt32() };
                        auto item_guid{ fields2[index + 1].GetUInt64() };
                        auto item_count{ fields2[index + 2].GetUInt32() };

                        if (item_entry > 0)
                        {
                            ostream2 << "Item guid: " << "|cffADFF2F|Harea:" << "|h" << item_guid << "|h|r" << ", ";
                            ostream2 << " |cffADFF2F|Harea:" << "|h" << item_count << "|h|r x ";
                            ostream2 << sObjectMgr->GetItemTemplate(item_entry)->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << "\n";
                        }
                    }
                }

            } while (result->NextRow());

            ostream2 << "------------------------------------------------------------\n";
            ostream2 << "-- END TRADE HISTORY (SPECIFIC) of Trade # " << "|cffff0000|Harea:" << "|h" << ARGs << "|h|r" << " --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->SendSysMessage(log);
        }
        else
        {
            handler->PSendSysMessage("Could not find TRADE HISTORY (SPECIFIC) for trade # %s", ARGs);
            return true;
        }
        return true;
    }

    static bool HandleLookupMacroCharacterCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        char* arg = strtok((char*)args, "");
        if (!arg)
            return false;

        std::string ARGs = arg;
        if (!atoi(ARGs.c_str()))
            return false;
        LoginDatabase.EscapeString(ARGs);

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_MACROS_FOR_CHARACTER);
        stmt->setString(0, ARGs);
        PreparedQueryResult result = CharacterDatabase.Query(stmt);

        if (result)
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START MACRO DETAILS (CHARACTER) of GUID #" << "|cffff0000|Harea:" << "|h" << ARGs << "|h|r" << " --\n";

            do
            {

                if (auto fields2 = result->Fetch())
                {
                    auto data{ fields2[3].GetCString() };
                    ostream2 << data << "\n";

                }

            } while (result->NextRow());

            ostream2 << "------------------------------------------------------------\n";
            ostream2 << "-- END MACRO DETAILS (CHARACTER) of GUID #" << "|cffff0000|Harea:" << "|h" << ARGs << "|h|r" << " --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->SendSysMessage(log);
        }
        else
        {
            handler->PSendSysMessage("Could not find MACRO DETAILS (CHARACTER) for GUID #%s", ARGs);
            return true;
        }
        return true;
    }


    static bool HandleLookupMacroAccountCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        char* arg = strtok((char*)args, "");
        if (!arg)
            return false;

        std::string ARGs = arg;
        if (!atoi(ARGs.c_str()))
            return false;
        LoginDatabase.EscapeString(ARGs);

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_MACROS_FOR_ACCOUNT);
        stmt->setString(0, ARGs);
        PreparedQueryResult result = CharacterDatabase.Query(stmt);

        if (result)
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START MACRO DETAILS (ACCOUNT) of Account Id # " << "|cffff0000|Harea:" << "|h" << ARGs << "|h|r" << " --\n";

            do
            {

                if (auto fields2 = result->Fetch())
                {
                    auto data{ fields2[3].GetCString() };
                    ostream2 << data << "\n";

                }

            } while (result->NextRow());

            ostream2 << "------------------------------------------------------------\n";
            ostream2 << "-- END MACRO DETAILS (ACCOUNT) of Account Id # " << "|cffff0000|Harea:" << "|h" << ARGs << "|h|r" << " --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->SendSysMessage(log);
        }
        else
        {
            handler->PSendSysMessage("Could not find MACRO DETAILS (ACCOUNT) for Account Id # %s", ARGs);
            return true;
        }
        return true;
    }
    static bool HandleLookupHistoryMailToCommand(ChatHandler* handler, char const* args)
    {
        if (!args)
            return false;

        char* arg = strtok((char*)args, " ");

        if (!arg)
            return false;

        // ORDER BY DateTime DESC LIMIT %u;", ARGs, limit))
        std::string ARGs = arg;
        if (!atoi(ARGs.c_str()))
            return false;
        char* limitStr = strtok(NULL, " ");
        int32 limit = limitStr ? atoi(limitStr) : 25;
        LoginDatabase.EscapeString(ARGs);

        if (auto result = CharacterDatabase.PQuery("SELECT Log_GUID, DateTime, send_GUID, recv_GUID, money, COD, (IF(item1_entry > 0, 1, 0)+ IF(item2_entry > 0, 1, 0)+ IF(item3_entry > 0, 1, 0)+ IF(item4_entry > 0, 1, 0)+ IF(item5_entry > 0, 1, 0)+ IF(item6_entry > 0, 1, 0)+ IF(item7_entry > 0, 1, 0)+ IF(item8_entry > 0, 1, 0)+ IF(item9_entry > 0, 1, 0)+ IF(item10_entry > 0, 1, 0)+ IF(item11_entry > 0, 1, 0)+ IF(item12_entry > 0, 1, 0)) AS item_count FROM char_history_mail WHERE recv_GUID = %u ORDER BY DateTime DESC LIMIT %u;", ARGs, limit))
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START MAIL HISTORY (RECEIVE) of Player " << "|cffff0000|Harea:" << "|h" << ARGs << "|h|r" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";

            do
            {
                if (auto fields2 = result->Fetch())
                {
                    uint64 trade_guid{ fields2[0].GetUInt64() };
                    auto date{ fields2[1].GetCString() };
                    auto from_guid{ fields2[2].GetUInt64() };
                    auto to_guid{ fields2[3].GetUInt64() };
                    auto money_from{ fields2[4].GetUInt64() };
                    auto cod_from{ fields2[5].GetUInt64() };
                    auto item_count{ fields2[6].GetUInt32() };

                    ostream2 << "Mail no. " << "|cffADFF2F|Harea:" << "|h" << trade_guid << "|h|r" << ", ";
                    ostream2 << "From player " << "|cffADFF2F|Harea:" << "|h" << from_guid << "|h|r" << ", ";
                    ostream2 << "Occured: " << "|cffADFF2F|Harea:" << "|h[" << date << "]|h|r" << ", ";
                    ostream2 << "Received: " << "|cffADFF2F|Harea:" << "|h" << moneyToString(money_from) << "|h|r money" << ", ";
                    ostream2 << "|cffADFF2F|Harea:" << "|h" << moneyToString(cod_from) << "|h|r COD" << ", ";
                    ostream2 << "and " "|cffADFF2F|Harea:" << "|h" << item_count << "|h|r items" << ".\n";
                }

            } while (result->NextRow());

            ostream2 << "-- END MAIL HISTORY (RECEIVE) of Player " << "|cffff0000|Harea:" << "|h" << ARGs << "|h|r" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->SendSysMessage(log);
        }
        else
        {
            handler->PSendSysMessage("Could not find MAIL HISTORY (RECEIVE) for character %s", ARGs);
            return true;
        }
        return true;
    }
    static bool HandleLookupHistoryMailFromCommand(ChatHandler* handler, char const* args)
    {
        if (!args)
            return false;

        char* arg = strtok((char*)args, " ");

        if (!arg)
            return false;

        // ORDER BY DateTime DESC LIMIT %u;", ARGs, limit))
        std::string ARGs = arg;
        if (!atoi(ARGs.c_str()))
            return false;

        char* limitStr = strtok(NULL, " ");
        int32 limit = limitStr ? atoi(limitStr) : 25;
        LoginDatabase.EscapeString(ARGs);

        if (auto result = CharacterDatabase.PQuery("SELECT Log_GUID, DateTime, send_GUID, recv_GUID, money, COD, (IF(item1_entry > 0, 1, 0)+ IF(item2_entry > 0, 1, 0)+ IF(item3_entry > 0, 1, 0)+ IF(item4_entry > 0, 1, 0)+ IF(item5_entry > 0, 1, 0)+ IF(item6_entry > 0, 1, 0)+ IF(item7_entry > 0, 1, 0)+ IF(item8_entry > 0, 1, 0)+ IF(item9_entry > 0, 1, 0)+ IF(item10_entry > 0, 1, 0)+ IF(item11_entry > 0, 1, 0)+ IF(item12_entry > 0, 1, 0)) AS item_count FROM char_history_mail WHERE send_GUID = %s ORDER BY DateTime DESC LIMIT %u;", ARGs, limit))
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START MAIL HISTORY (SEND) of Player " << "|cffff0000|Harea:" << "|h" << ARGs << "|h|r" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";

            do
            {
                if (auto fields2 = result->Fetch())
                {
                    uint64 trade_guid{ fields2[0].GetUInt64() };
                    auto date{ fields2[1].GetCString() };
                    auto from_guid{ fields2[2].GetUInt64() };
                    auto to_guid{ fields2[3].GetUInt64() };
                    auto money_from{ fields2[4].GetUInt64() };
                    auto cod_from { fields2[5].GetUInt64() };
                    auto item_count{ fields2[6].GetUInt32() };

                    ostream2 << "Mail no. " << "|cffADFF2F|Harea:" << "|h" << trade_guid << "|h|r" << ", ";
                    ostream2 << "To player " << "|cffADFF2F|Harea:" << "|h" << to_guid << "|h|r" << ", ";
                    ostream2 << "Occured: " << "|cffADFF2F|Harea:" << "|h[" << date << "]|h|r" << ", ";
                    ostream2 << "Sent: " << "|cffADFF2F|Harea:" << "|h" << moneyToString(money_from) << "|h|r money" << ", ";
                    ostream2 << "|cffADFF2F|Harea:" << "|h" << moneyToString(cod_from) << "|h|r COD" << ", ";
                    ostream2 << "and " "|cffADFF2F|Harea:" << "|h" << item_count << "|h|r items" << ".\n";
                }

            } while (result->NextRow());

            ostream2 << "-- END MAIL HISTORY (SEND) of Player " << "|cffff0000|Harea:" << "|h" << ARGs << "|h|r" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->SendSysMessage(log);
        }
        else
        {
            handler->PSendSysMessage("Could not find MAIL HISTORY (SEND) for character %s", ARGs);
            return true;
        }
        return true;
    }
    static bool HandleLookupHistoryMailDetailsCommand(ChatHandler* handler, char const* args)
    {
        if (!args)
            return false;

        char* arg = strtok((char*)args, " ");

        if (!arg)
            return false;

        // ORDER BY DateTime DESC LIMIT %u;", ARGs, limit))
        std::string ARGs = arg;
        if (!atoi(ARGs.c_str()))
            return false;

        LoginDatabase.EscapeString(ARGs);

        if (auto result = CharacterDatabase.PQuery("SELECT * FROM char_history_mail WHERE Log_GUID IN (%s);", ARGs))
        {
            std::ostringstream ostream2{ "" };

            ostream2 << "-- START MAIL HISTORY (SPECIFIC) of Mail # " << "|cffff0000|Harea:" << "|h" << ARGs << "|h|r" << " --\n";

            do
            {
                if (auto fields2 = result->Fetch())
                {
                    auto trade_guid{ fields2[0].GetUInt64() };
                    auto date{ fields2[1].GetCString() };

                    auto p1_guid{ fields2[2].GetUInt64() };
                    auto p2_guid{ fields2[3].GetUInt64() };
                    auto p1_money{ fields2[4].GetUInt64() };
                    auto p2_money{ fields2[5].GetUInt32() };


                    ostream2 << "Mail no. " << "|cffADFF2F|Harea:" << "|h" << trade_guid << "|h|r" << ", ";
                    ostream2 << "Occured at: " << "|cffADFF2F|Harea:" << "|h" << date << "|h|r" << "\n";

                    ostream2 << "------------------------------------------------------------\n";
                    ostream2 << "From " << "|cffADFF2F|Harea:" << "|h" << p1_guid << "|h|r to " << "|cffADFF2F|Harea:" << "|h" << p2_guid << "|h|r" ;
                    ostream2 << "\nsent:\n"
                        << "Money:   |cffADFF2F|Harea:" << "|h" << moneyToString(p1_money) << "|h|r\n"
                        << "COD:     |cffADFF2F|Harea:" << "|h" << moneyToString(p2_money) << "|h|r\n\nItem(s):\n";
                    for (auto i = 0; i < 12; ++i)
                    {
                        auto index{ 6 + (i * 3) };
                        auto item_entry{ fields2[index].GetUInt32() };
                        auto item_guid{ fields2[index + 1].GetUInt64() };
                        auto item_count{ fields2[index + 2].GetUInt32() };

                        if (item_entry > 0)
                        if (auto templ = sObjectMgr->GetItemTemplate(item_entry))
                        {
                            ostream2 << "GUID: " << "|cffADFF2F|Harea:" << "|h" << item_guid << "|h|r" << ","
                            << " |cffADFF2F|Harea:" << "|h" << item_count << "|h|r x "
                            << templ->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << " \n";
                        }
                    }
                    ostream2 << "------------------------------------------------------------\n";
                }

            } while (result->NextRow());
            ostream2 << "-- END MAIL HISTORY (SPECIFIC) of Mail # " << "|cffff0000|Harea:" << "|h" << ARGs << "|h|r" << " --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->SendSysMessage(log);
        }
        else
        {
            handler->PSendSysMessage("Could not find MAIL HISTORY (SPECIFIC) for mail log no. %s", ARGs);
            return true;
        }
        return true;
    }

    static bool HandleLookupPlayerEmailCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        std::string email = strtok((char*)args, " ");
        char* limitStr = strtok(NULL, " ");
        int32 limit = limitStr ? atoi(limitStr) : -1;

        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_LIST_BY_EMAIL);
        stmt->setString(0, email);
        PreparedQueryResult result = LoginDatabase.Query(stmt);

        return LookupPlayerSearchCommand(result, limit, handler);
    }

    static bool HandleLookupPlayerItemCommand(ChatHandler* handler, char const* args)
    {
        if (!args)
            return false;

        char* arg = strtok((char*)args, " ");

        if (!arg)
            return false;

        std::string ARGs = arg;
        if (!atoi(ARGs.c_str()))
            return false;

        char* limitStr = strtok(NULL, " ");

        char const* id = handler->extractKeyFromLink(limitStr, "Hitem");

        uint32 entry = id ? uint32(atoi(id)) : uint32(0);
        if (!entry)
            return false;

        LoginDatabase.EscapeString(ARGs);
        //                                                  0           1               2               3               4               5           6           7           8           9                           10                  11              12          13                     
        if (auto result = CharacterDatabase.PQuery("SELECT  `guid`,  `itemEntry`,  `owner_guid`,  `creatorGuid`,  `giftCreatorGuid`,  `count`,  `duration`,  `charges`,  `flags`, LEFT(`enchantments`, 256),  `randomPropertyId`,  `durability`,  `playedTime`, LEFT(`text`, 256) FROM item_instance_buyback iib WHERE iib.itemEntry IN (%u) AND iib.owner_guid IN (%s) UNION SELECT  `guid`,  `itemEntry`,  `owner_guid`,  `creatorGuid`,  `giftCreatorGuid`,  `count`,  `duration`,  `charges`,  `flags`, LEFT(`enchantments`, 256), `randomPropertyId`,  `durability`,  `playedTime`, LEFT(`text`, 256) FROM item_instance ii WHERE ii.itemEntry IN(%u) AND ii.owner_guid IN(%s);", entry, ARGs, entry, ARGs))
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START ITEM SEARCH of Player " << "|cffff0000|Harea:" << "|h" << ARGs << "|h|r" << ", for item "<< sObjectMgr->GetItemTemplate(entry)->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << "--\n";


            do
            {
                if (auto fields2 = result->Fetch())
                {
                    uint64 item_guid { fields2[0].GetUInt64() };
                    uint64 creator_guid{ fields2[3].GetUInt64() };
                    uint64 gift_creator_guid{ fields2[4].GetUInt64() };
                    auto item_entry{ fields2[1].GetUInt32() };
                    auto item_count{ fields2[5].GetUInt32() };
                    auto played_time{ fields2[12].GetUInt32() };

                    //TC_LOG_ERROR("sql.sql", "%u %u %u", item_guid, item_entry, item_count);

                    
                    ostream2 << "Item GUID No. " << "|cffADFF2F|Harea:" << "|h" << item_guid << "|h|r" << ", ";
                    ostream2 << "Stack Amount: " << "|cffADFF2F|Harea:" << "|h" << item_count << "|h|r" << ", ";
                    ostream2 << "Created By: " << "|cffADFF2F|Harea:" << "|h[" << creator_guid << "]|h|r" << ", ";
                    ostream2 << "Gifted By: " << "|cffADFF2F|Harea:" << "|h[" << creator_guid << "]|h|r" << "\n";
                }

            } while (result->NextRow());

            ostream2 << "-- END ITEM SEARCH of Player " << "|cffff0000|Harea:" << "|h" << ARGs << "|h|r" << ", for item " << sObjectMgr->GetItemTemplate(entry)->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << "--\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->SendSysMessage(log);
        }
        else
        {
            handler->PSendSysMessage("Could not find any items of entry %u for character %s", entry, ARGs);
            return true;
        }
        return true;
    }

    static bool LookupPlayerSearchCommand(PreparedQueryResult result, int32 limit, ChatHandler* handler)
    {
        if (!result)
        {
            handler->PSendSysMessage(LANG_NO_PLAYERS_FOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        int32 counter = 0;
        uint32 count = 0;
        uint32 maxResults = sWorld->getIntConfig(CONFIG_MAX_RESULTS_LOOKUP_COMMANDS);

        do
        {
            if (maxResults && count++ == maxResults)
            {
                handler->PSendSysMessage(LANG_COMMAND_LOOKUP_MAX_RESULTS, maxResults);
                return true;
            }

            Field* fields           = result->Fetch();
            uint32 accountId        = fields[0].GetUInt32();
            std::string accountName = fields[1].GetString();

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_GUID_NAME_BY_ACC);
            stmt->setUInt32(0, accountId);
            PreparedQueryResult result2 = CharacterDatabase.Query(stmt);

            if (result2)
            {
                handler->PSendSysMessage(LANG_LOOKUP_PLAYER_ACCOUNT, accountName.c_str(), accountId);

                do
                {
                    Field* characterFields  = result2->Fetch();
                    uint32 guid             = characterFields[0].GetUInt32();
                    std::string name        = characterFields[1].GetString();

                    handler->PSendSysMessage(LANG_LOOKUP_PLAYER_CHARACTER, name.c_str(), guid, sObjectMgr->GetPlayerByLowGUID(guid) ? "[Online]" : "");
                    ++counter;
                }
                while (result2->NextRow() && (limit == -1 || counter < limit));
            }
        }
        while (result->NextRow());

        if (counter == 0) // empty accounts only
        {
            handler->PSendSysMessage(LANG_NO_PLAYERS_FOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        return true;
    }


    static bool HandleLookupLootForItemCommand(ChatHandler* handler, char const* args)
    {
        if (!args)
            return false;

        uint32 itemId = 0;

        if (args[0] == '[')                                        // [name] manual form
        {
            char const* itemNameStr = strtok((char*)args, "]");

            if (itemNameStr && itemNameStr[0])
            {
                std::string itemName = itemNameStr + 1;
                WorldDatabase.EscapeString(itemName);

                PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_ITEM_TEMPLATE_BY_NAME);
                stmt->setString(0, itemName);
                PreparedQueryResult result = WorldDatabase.Query(stmt);

                if (!result)
                {
                    handler->PSendSysMessage(LANG_COMMAND_COULDNOTFIND, itemNameStr + 1);
                    handler->SetSentErrorMessage(true);
                    return false;
                }
                itemId = result->Fetch()->GetUInt32();
            }
            else
                return false;
        }
        else                                                    // item_id or [name] Shift-click form |color|Hitem:item_id:0:0:0|h[name]|h|r
        {
            char const* id = handler->extractKeyFromLink((char*)args, "Hitem");
            if (!id)
                return false;
            itemId = uint32(atol(id));
        }

        char const* ccount = strtok(NULL, " ");

        int32 limit = 1;

        if (ccount)
            limit = strtol(ccount, NULL, 10);

        if (limit == 0)
            limit = 25;

        uint32 entry = itemId;

        //                                                  0           1               2               3               4               5           6           7           8           9                           10                  11              12          13                     
        if (auto item_template = sObjectMgr->GetItemTemplate(entry))
            if (auto result = WorldDatabase.PQuery("SELECT * FROM (SELECT %u AS 'item', entry, lootmode, groupid, 'item' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM item_loot_template WHERE item = %u UNION SELECT %u AS 'item', entry, lootmode, groupid, 'creature' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM creature_loot_template WHERE item = %u UNION SELECT %u AS 'item', entry, lootmode, groupid, 'Table' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM reference_loot_template WHERE item = %u UNION SELECT %u AS 'item', entry, lootmode, groupid, 'spell' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM spell_loot_template WHERE item = %u UNION SELECT %u AS 'item', entry, lootmode, groupid, 'pickpocketing' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM pickpocketing_loot_template WHERE item = %u UNION SELECT %u AS 'item', entry, lootmode, groupid, 'fishing' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM fishing_loot_template WHERE item = %u UNION SELECT %u AS 'item', entry, lootmode, groupid, 'skinning' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM skinning_loot_template WHERE item = %u UNION SELECT %u AS 'item', entry, lootmode, groupid, 'prospecting' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM prospecting_loot_template WHERE item = %u UNION SELECT %u AS 'item', entry, lootmode, groupid, 'gameobject' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM gameobject_loot_template WHERE item = %u UNION SELECT %u AS 'item', entry, lootmode, groupid, 'disenchant' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM disenchant_loot_template WHERE item = %u UNION SELECT %u AS 'item', entry, lootmode, groupid, 'milling' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM milling_loot_template WHERE item = %u UNION SELECT %u AS 'item', entry, lootmode, groupid, 'mail' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM mail_loot_template WHERE item = %u) results ORDER BY ABS(chance) DESC LIMIT %u;", entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, limit))
            {
                std::ostringstream ostream2{ "" };
                ostream2 << "-- START LOOT SOURCES to ITEM " << item_template->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";


                do
                {
                    if (auto fields2 = result->Fetch())
                    {
                        uint32 source_entry{ fields2[1].GetUInt32() };
                        uint32 loot_mode{ fields2[2].GetUInt32() };
                        uint32 group_id{ fields2[3].GetUInt32() };
                        std::string source_name{ fields2[4].GetString() };
                        auto chance{ fields2[5].GetFloat() };
                        auto min_count_or_ref{ fields2[6].GetInt32() };
                        bool ref{ min_count_or_ref < 0 };
                        uint32 min_count{ uint32(ref ? 1 : min_count_or_ref) };
                        bool quest_required{ chance < 0.f };
                        float true_chance{ fabs(chance) };
                        auto max_count{ fields2[7].GetUInt32() };


                        ostream2 << "|cffADFF2F|Harea:" << "|h" << source_name << "|h|r " << "|cffff0000|Harea:" << "|h" << source_entry << "|h|r" << " has chance: ";
                        ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to drop ";
                        ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                        ostream2 << " to|cffADFF2F|Harea:"<<"|h " << max_count << "|h|r) from group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                    }

                } while (result->NextRow());

                ostream2 << "-- END LOOT SOURCES to ITEM " << item_template->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";
                auto resulting_string{ ostream2.str() };
                auto log{ resulting_string.c_str() };
                TC_LOG_ERROR("sql.sql", "success return");
                handler->SendSysMessage(log);
                return true;
            }
            else
            {
                handler->PSendSysMessage("Database did not return any results for %u", entry);
                return true;
            }
        else
        {
            handler->PSendSysMessage("Could not identify an item template for entry %u", entry);
            return true;
        }
        TC_LOG_ERROR("sql.sql", "end return");
        return true;
    }

    static bool HandleLookupLootForTableCommand(ChatHandler* handler, char const* args)
    {
        if (!args)
            return false;

        char* arg = strtok((char*)args, " ");

        if (!arg)
            return false;

        // ORDER BY DateTime DESC LIMIT %u;", ARGs, limit))
        std::string AccStr = arg;
        uint32 entry = uint32(atoi(arg) ? atoi(arg) : 0);

        char* limitStr = strtok(NULL, " ");
        int32 limit = limitStr ? atoi(limitStr) : 25;
        //                                                  0           1               2               3               4               5           6           7           8           9                           10                  11              12          13                     
            if (auto result = WorldDatabase.PQuery("SELECT * FROM  (SELECT %u AS 'item', entry, lootmode, groupid, 'item' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM item_loot_template WHERE MinCountOrRef = -%u UNION SELECT %u AS 'item', entry, lootmode, groupid, 'creature' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM creature_loot_template WHERE MinCountOrRef = -%u UNION SELECT %u AS 'item', entry, lootmode, groupid, 'Table' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM reference_loot_template WHERE MinCountOrRef = -%u UNION SELECT %u AS 'item', entry, lootmode, groupid, 'spell' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM spell_loot_template WHERE MinCountOrRef = -%u UNION SELECT %u AS 'item', entry, lootmode, groupid, 'pickpocketing' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM pickpocketing_loot_template WHERE MinCountOrRef = -%u UNION SELECT %u AS 'item', entry, lootmode, groupid, 'fishing' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM fishing_loot_template WHERE MinCountOrRef = -%u UNION SELECT %u AS 'item', entry, lootmode, groupid, 'skinning' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM skinning_loot_template WHERE MinCountOrRef = -%u UNION SELECT %u AS 'item', entry, lootmode, groupid, 'prospecting' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM prospecting_loot_template WHERE MinCountOrRef = -%u UNION SELECT %u AS 'item', entry, lootmode, groupid, 'gameobject' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM gameobject_loot_template WHERE MinCountOrRef = -%u UNION SELECT %u AS 'item', entry, lootmode, groupid, 'disenchant' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM disenchant_loot_template WHERE MinCountOrRef = -%u UNION SELECT %u AS 'item', entry, lootmode, groupid, 'milling' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM milling_loot_template WHERE MinCountOrRef = -%u UNION SELECT %u AS 'item', entry, lootmode, groupid, 'mail' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount  FROM mail_loot_template WHERE MinCountOrRef = -%u) results ORDER BY ABS(chance) DESC LIMIT %u;", entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, entry, limit))
            {
                std::ostringstream ostream2{ "" };
                ostream2 << "-- START LOOT SOURCES to TABLE |cffff0000|Harea:"<< "|h" << entry << "|h|r, limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";


                do
                {
                    if (auto fields2 = result->Fetch())
                    {
                        uint32 source_entry{ fields2[1].GetUInt32() };
                        uint32 loot_mode{ fields2[2].GetUInt32() };
                        uint32 group_id{ fields2[3].GetUInt32() };
                        std::string source_name{ fields2[4].GetString() };
                        auto chance{ fields2[5].GetFloat() };
                        auto min_count_or_ref{ fields2[6].GetInt32() };
                        bool ref{ min_count_or_ref < 0 };
                        uint32 min_count{ uint32(ref ? 1 : min_count_or_ref) };
                        bool quest_required{ chance < 0.f };
                        float true_chance{ fabs(chance) };
                        auto max_count{ fields2[7].GetUInt32() };


                        ostream2 << "|cffADFF2F|Harea:" << "|h" << source_name << "|h|r " << "|cffff0000|Harea:" << "|h" << source_entry << "|h|r" << " has chance: ";
                        ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to roll ";
                        ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                        ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from this table, if rolled among group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                    }

                } while (result->NextRow());

                ostream2 << "-- END LOOT SOURCES to TABLE |cffff0000|Harea:" << "|h" << entry << "|h|r, limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";
                auto resulting_string{ ostream2.str() };
                auto log{ resulting_string.c_str() };
                TC_LOG_ERROR("sql.sql", "success return");
                handler->SendSysMessage(log);
                return true;
            }
            else
            {
                handler->PSendSysMessage("Database did not return any results for %u", entry);
                return true;
            }
        TC_LOG_ERROR("sql.sql", "end return");
        return true;
    }

    static bool HandleLookupLootFromItemCommand(ChatHandler* handler, char const* args)
    {
        if (!args)
            return false;

        uint32 itemId = 0;

        if (args[0] == '[')                                        // [name] manual form
        {
            char const* itemNameStr = strtok((char*)args, "]");

            if (itemNameStr && itemNameStr[0])
            {
                std::string itemName = itemNameStr + 1;
                WorldDatabase.EscapeString(itemName);

                PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_ITEM_TEMPLATE_BY_NAME);
                stmt->setString(0, itemName);
                PreparedQueryResult result = WorldDatabase.Query(stmt);

                if (!result)
                {
                    handler->PSendSysMessage(LANG_COMMAND_COULDNOTFIND, itemNameStr + 1);
                    handler->SetSentErrorMessage(true);
                    return false;
                }
                itemId = result->Fetch()->GetUInt32();
            }
            else
                return false;
        }
        else                                                    // item_id or [name] Shift-click form |color|Hitem:item_id:0:0:0|h[name]|h|r
        {
            char const* id = handler->extractKeyFromLink((char*)args, "Hitem");
            if (!id)
                return false;
            itemId = uint32(atol(id));
        }

        char const* ccount = strtok(NULL, " ");

        int32 limit = 1;

        if (ccount)
            limit = strtol(ccount, NULL, 10);

        if (limit == 0)
            limit = 25;

        uint32 entry = itemId;

        //                                                  0           1               2               3               4               5           6           7           8           9                           10                  11              12          13                     
        if (auto item_template = sObjectMgr->GetItemTemplate(entry))
            if (auto result = WorldDatabase.PQuery("SELECT * FROM (SELECT entry, item, lootmode, groupid, 'item' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount FROM item_loot_template WHERE entry = %u) results ORDER BY ABS(chance) DESC LIMIT %u;", entry, limit))
            {
                std::ostringstream ostream2{ "" };
                ostream2 << "-- START LOOT DROPS from ITEM " << item_template->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";


                do
                {
                    if (auto fields2 = result->Fetch())
                    {
                        uint32 reward_entry{ fields2[1].GetUInt32() };
                        uint32 loot_mode{ fields2[2].GetUInt32() };
                        uint32 group_id{ fields2[3].GetUInt32() };
                        std::string source_name{ fields2[4].GetString() };
                        auto chance{ fields2[5].GetFloat() };
                        auto min_count_or_ref{ fields2[6].GetInt32() };
                        bool ref{ min_count_or_ref < 0 };
                        uint32 min_count{ uint32(ref ? 1 : min_count_or_ref) };
                        bool quest_required{ chance < 0.f };
                        float true_chance{ fabs(chance) };
                        auto max_count{ fields2[7].GetUInt32() };

                        if (ref)
                        {
                            ostream2 << "|cffADFF2F|Harea:" << "|h" << "Table " << "|h|r " << "|cffff0000|Harea:" << "|h" << -min_count_or_ref << "|h|r has chance: ";
                            ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to be rolled ";
                            ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                            ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                        }
                        else
                        if (auto t = sObjectMgr->GetItemTemplate(reward_entry))
                        {
                            ostream2 << "|cffADFF2F|Harea:" << "|h" << source_name << "|h|r " << t->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << " has chance: ";
                            ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to be dropped ";
                            ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                            ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                        }

                    }

                } while (result->NextRow());

                ostream2 << "-- END LOOT DROPS from ITEM " << item_template->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";
                auto resulting_string{ ostream2.str() };
                auto log{ resulting_string.c_str() };
                //TC_LOG_ERROR("sql.sql", "success return");
                handler->SendSysMessage(log);
                return true;
            }
            else
            {
                handler->PSendSysMessage("Database did not return any results for %u", entry);
                return true;
            }
        else
        {
            handler->PSendSysMessage("Could not identify an item template for entry %u", entry);
            return true;
        }
        //TC_LOG_ERROR("sql.sql", "end return");
        return true;
    }

    static bool HandleLookupLootFromProspectingCommand(ChatHandler* handler, char const* args)
    {
        if (!args)
            return false;

        uint32 itemId = 0;

        if (args[0] == '[')                                        // [name] manual form
        {
            char const* itemNameStr = strtok((char*)args, "]");

            if (itemNameStr && itemNameStr[0])
            {
                std::string itemName = itemNameStr + 1;
                WorldDatabase.EscapeString(itemName);

                PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_ITEM_TEMPLATE_BY_NAME);
                stmt->setString(0, itemName);
                PreparedQueryResult result = WorldDatabase.Query(stmt);

                if (!result)
                {
                    handler->PSendSysMessage(LANG_COMMAND_COULDNOTFIND, itemNameStr + 1);
                    handler->SetSentErrorMessage(true);
                    return false;
                }
                itemId = result->Fetch()->GetUInt32();
            }
            else
                return false;
        }
        else                                                    // item_id or [name] Shift-click form |color|Hitem:item_id:0:0:0|h[name]|h|r
        {
            char const* id = handler->extractKeyFromLink((char*)args, "Hitem");
            if (!id)
                return false;
            itemId = uint32(atol(id));
        }

        char const* ccount = strtok(NULL, " ");

        int32 limit = 1;

        if (ccount)
            limit = strtol(ccount, NULL, 10);

        if (limit == 0)
            limit = 25;

        uint32 entry = itemId;

        //                                                  0           1               2               3               4               5           6           7           8           9                           10                  11              12          13                     
        if (auto item_template = sObjectMgr->GetItemTemplate(entry))
            if (auto result = WorldDatabase.PQuery("SELECT * FROM (SELECT entry, item, lootmode, groupid, 'item' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount FROM prospecting_loot_template WHERE entry = %u) results ORDER BY ABS(chance) DESC LIMIT %u;", entry, limit))
            {
                std::ostringstream ostream2{ "" };
                ostream2 << "-- START LOOT DROPS from PROSPECTING " << item_template->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";


                do
                {
                    if (auto fields2 = result->Fetch())
                    {
                        uint32 reward_entry{ fields2[1].GetUInt32() };
                        uint32 loot_mode{ fields2[2].GetUInt32() };
                        uint32 group_id{ fields2[3].GetUInt32() };
                        std::string source_name{ fields2[4].GetString() };
                        auto chance{ fields2[5].GetFloat() };
                        auto min_count_or_ref{ fields2[6].GetInt32() };
                        bool ref{ min_count_or_ref < 0 };
                        uint32 min_count{ uint32(ref ? 1 : min_count_or_ref) };
                        bool quest_required{ chance < 0.f };
                        float true_chance{ fabs(chance) };
                        auto max_count{ fields2[7].GetUInt32() };

                        if (ref)
                        {
                            ostream2 << "|cffADFF2F|Harea:" << "|h" << "Table " << "|h|r " << "|cffff0000|Harea:" << "|h" << -min_count_or_ref << "|h|r has chance: ";
                            ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to be rolled ";
                            ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                            ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                        }
                        else
                            if (auto t = sObjectMgr->GetItemTemplate(reward_entry))
                            {
                                ostream2 << "|cffADFF2F|Harea:" << "|h" << source_name << "|h|r " << t->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << " has chance: ";
                                ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to be dropped ";
                                ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                                ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                            }

                    }

                } while (result->NextRow());

                ostream2 << "-- END LOOT DROPS from PROSPECTING " << item_template->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";
                auto resulting_string{ ostream2.str() };
                auto log{ resulting_string.c_str() };
                TC_LOG_ERROR("sql.sql", "success return");
                handler->SendSysMessage(log);
                return true;
            }
            else
            {
                handler->PSendSysMessage("Database did not return any results for %u", entry);
                return true;
            }
        else
        {
            handler->PSendSysMessage("Could not identify an item template for entry %u", entry);
            return true;
        }
        TC_LOG_ERROR("sql.sql", "end return");
        return true;
    }

    static bool HandleLookupLootFromFishingCommand(ChatHandler* handler, char const* args)
    {

        if (!args)
            return false;

        char* arg = strtok((char*)args, " ");

        if (!arg)
            return false;

        // ORDER BY DateTime DESC LIMIT %u;", ARGs, limit))
        std::string AccStr = arg;
        uint32 entry = uint32(atoi(arg) ? atoi(arg) : 0);

        char* limitStr = strtok(NULL, " ");
        int32 limit = limitStr ? atoi(limitStr) : 25;

            if (auto result = WorldDatabase.PQuery("SELECT * FROM  (SELECT gob.entry AS 'gobj', glt.item, glt.lootmode, glt.groupid, 'Fishing Spot' AS 'source', glt.ChanceOrQuestChance AS 'chance', glt.MinCountOrRef, glt.MaxCount, glt.entry FROM gameobject_template gob LEFT JOIN gameobject_loot_template glt ON glt.entry = gob.data1 WHERE gob.type = 25 AND gob.entry = %u) results ORDER BY ABS(chance) DESC LIMIT %u;", entry, limit))
            {
                std::ostringstream ostream2{ "" };
                ostream2 << "-- START LOOT SOURCES from Fishing Spot ID. |cffff0000|Harea:" << "|h" << entry << "|h|r, limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";

                do
                {
                    if (auto fields2 = result->Fetch())
                    {
                        uint32 reward_entry{ fields2[1].GetUInt32() };
                        uint32 loot_mode{ fields2[2].GetUInt32() };
                        uint32 group_id{ fields2[3].GetUInt32() };
                        std::string source_name{ fields2[4].GetString() };
                        auto chance{ fields2[5].GetFloat() };
                        auto min_count_or_ref{ fields2[6].GetInt32() };
                        bool ref{ min_count_or_ref < 0 };
                        uint32 min_count{ uint32(ref ? 1 : min_count_or_ref) };
                        bool quest_required{ chance < 0.f };
                        float true_chance{ fabs(chance) };
                        auto max_count{ fields2[7].GetUInt32() };
                        auto fishing_loot_id{ fields2[8].GetUInt32() };

                        if (ref)
                        {
                            ostream2 << "|cffADFF2F|Harea:" << "|h" << "Table " << "|h|r " << "|cffff0000|Harea:" << "|h" << -min_count_or_ref << "|h|r has chance: ";
                            ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to be rolled ";
                            ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                            ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from gobj_loot no. " << "|cffff0000|Harea:" << "|h" << fishing_loot_id << "|h|r" << ", group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                        }
                        else
                            if (auto t = sObjectMgr->GetItemTemplate(reward_entry))
                            {
                                ostream2 << t->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << " has chance: ";
                                ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to be dropped ";
                                ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                                ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from gobj_loot no. " << "|cffff0000|Harea:" << "|h" << fishing_loot_id << "|h|r" <<", group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                            }

                    }

                } while (result->NextRow());

                ostream2 << "-- END LOOT SOURCES from Fishing Spot ID. |cffff0000|Harea:" << "|h" << entry << "|h|r, limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";
                auto resulting_string{ ostream2.str() };
                auto log{ resulting_string.c_str() };
                handler->SendSysMessage(log);
                return true;
            }
            else
            {
                handler->PSendSysMessage("Database did not return any results for %u", entry);
                return true;
            }
        return true;
    }

    static bool HandleLookupLootFromSkinningCommand(ChatHandler* handler, char const* args)
    {

        if (!args)
            return false;

        char* arg = strtok((char*)args, " ");

        if (!arg)
            return false;

        // ORDER BY DateTime DESC LIMIT %u;", ARGs, limit))
        std::string AccStr = arg;
        uint32 entry = uint32(atoi(arg) ? atoi(arg) : 0);

        char* limitStr = strtok(NULL, " ");
        int32 limit = limitStr ? atoi(limitStr) : 25;

        if (auto result = WorldDatabase.PQuery("SELECT * FROM  (SELECT ct.entry AS 'ctj', slt.item, slt.lootmode, slt.groupid, 'Fishing Spot' AS 'source', slt.ChanceOrQuestChance AS 'chance', slt.MinCountOrRef, slt.MaxCount, slt.entry FROM creature_template ct LEFT JOIN skinning_loot_template slt ON slt.entry = ct.skinloot WHERE ct.skinloot > 0 AND ct.entry = %u) results ORDER BY ABS(chance) DESC LIMIT %u;", entry, limit))
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START LOOT from Skinning NPC ID. |cffff0000|Harea:" << "|h" << entry << "|h|r, limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";

            do
            {
                if (auto fields2 = result->Fetch())
                {
                    uint32 reward_entry{ fields2[1].GetUInt32() };
                    uint32 loot_mode{ fields2[2].GetUInt32() };
                    uint32 group_id{ fields2[3].GetUInt32() };
                    std::string source_name{ fields2[4].GetString() };
                    auto chance{ fields2[5].GetFloat() };
                    auto min_count_or_ref{ fields2[6].GetInt32() };
                    bool ref{ min_count_or_ref < 0 };
                    uint32 min_count{ uint32(ref ? 1 : min_count_or_ref) };
                    bool quest_required{ chance < 0.f };
                    float true_chance{ fabs(chance) };
                    auto max_count{ fields2[7].GetUInt32() };
                    auto fishing_loot_id{ fields2[8].GetUInt32() };

                    if (ref)
                    {
                        ostream2 << "|cffADFF2F|Harea:" << "|h" << "Table " << "|h|r " << "|cffff0000|Harea:" << "|h" << -min_count_or_ref << "|h|r has chance: ";
                        ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to be rolled ";
                        ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                        ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from skin_loot no. " << "|cffff0000|Harea:" << "|h" << fishing_loot_id << "|h|r" << ", group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                    }
                    else
                        if (auto t = sObjectMgr->GetItemTemplate(reward_entry))
                        {
                            ostream2 << t->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << " has chance: ";
                            ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to be dropped ";
                            ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                            ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from skin_loot no. " << "|cffff0000|Harea:" << "|h" << fishing_loot_id << "|h|r" << ", group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                        }

                }

            } while (result->NextRow());

            ostream2 << "-- END LOOT from Skinning NPC ID. |cffff0000|Harea:" << "|h" << entry << "|h|r, limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->SendSysMessage(log);
            return true;
        }
        else
        {
            handler->PSendSysMessage("Database did not return any results for %u", entry);
            return true;
        }
        return true;
    }


    static bool HandleLookupLootFromMillingCommand(ChatHandler* handler, char const* args)
    {
        if (!args)
            return false;

        uint32 itemId = 0;

        if (args[0] == '[')                                        // [name] manual form
        {
            char const* itemNameStr = strtok((char*)args, "]");

            if (itemNameStr && itemNameStr[0])
            {
                std::string itemName = itemNameStr + 1;
                WorldDatabase.EscapeString(itemName);

                PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_ITEM_TEMPLATE_BY_NAME);
                stmt->setString(0, itemName);
                PreparedQueryResult result = WorldDatabase.Query(stmt);

                if (!result)
                {
                    handler->PSendSysMessage(LANG_COMMAND_COULDNOTFIND, itemNameStr + 1);
                    handler->SetSentErrorMessage(true);
                    return false;
                }
                itemId = result->Fetch()->GetUInt32();
            }
            else
                return false;
        }
        else                                                    // item_id or [name] Shift-click form |color|Hitem:item_id:0:0:0|h[name]|h|r
        {
            char const* id = handler->extractKeyFromLink((char*)args, "Hitem");
            if (!id)
                return false;
            itemId = uint32(atol(id));
        }

        char const* ccount = strtok(NULL, " ");

        int32 limit = 1;

        if (ccount)
            limit = strtol(ccount, NULL, 10);

        if (limit == 0)
            limit = 25;

        uint32 entry = itemId;

        //                                                  0           1               2               3               4               5           6           7           8           9                           10                  11              12          13                     
        if (auto item_template = sObjectMgr->GetItemTemplate(entry))
            if (auto result = WorldDatabase.PQuery("SELECT * FROM (SELECT entry, item, lootmode, groupid, 'item' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount FROM milling_loot_template WHERE entry = %u) results ORDER BY ABS(chance) DESC LIMIT %u;", entry, limit))
            {
                std::ostringstream ostream2{ "" };
                ostream2 << "-- START LOOT DROPS from MILLING " << item_template->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";


                do
                {
                    if (auto fields2 = result->Fetch())
                    {
                        uint32 reward_entry{ fields2[1].GetUInt32() };
                        uint32 loot_mode{ fields2[2].GetUInt32() };
                        uint32 group_id{ fields2[3].GetUInt32() };
                        std::string source_name{ fields2[4].GetString() };
                        auto chance{ fields2[5].GetFloat() };
                        auto min_count_or_ref{ fields2[6].GetInt32() };
                        bool ref{ min_count_or_ref < 0 };
                        uint32 min_count{ uint32(ref ? 1 : min_count_or_ref) };
                        bool quest_required{ chance < 0.f };
                        float true_chance{ fabs(chance) };
                        auto max_count{ fields2[7].GetUInt32() };

                        if (ref)
                        {
                            ostream2 << "|cffADFF2F|Harea:" << "|h" << "Table " << "|h|r " << "|cffff0000|Harea:" << "|h" << -min_count_or_ref << "|h|r has chance: ";
                            ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to be rolled ";
                            ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                            ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                        }
                        else
                            if (auto t = sObjectMgr->GetItemTemplate(reward_entry))
                            {
                                ostream2 << "|cffADFF2F|Harea:" << "|h" << source_name << "|h|r " << t->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << " has chance: ";
                                ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to be dropped ";
                                ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                                ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                            }

                    }

                } while (result->NextRow());

                ostream2 << "-- END LOOT DROPS from MILLING " << item_template->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";
                auto resulting_string{ ostream2.str() };
                auto log{ resulting_string.c_str() };
                handler->SendSysMessage(log);
                return true;
            }
            else
            {
                handler->PSendSysMessage("Database did not return any results for %u", entry);
                return true;
            }
        else
        {
            handler->PSendSysMessage("Could not identify an item template for entry %u", entry);
            return true;
        }
        return true;
    }

    static bool HandleLookupLootFromPickpocketingCommand(ChatHandler* handler, char const* args)
    {

        if (!args)
            return false;

        char* arg = strtok((char*)args, " ");

        if (!arg)
            return false;

        // ORDER BY DateTime DESC LIMIT %u;", ARGs, limit))
        std::string AccStr = arg;
        uint32 entry = uint32(atoi(arg) ? atoi(arg) : 0);

        char* limitStr = strtok(NULL, " ");
        int32 limit = limitStr ? atoi(limitStr) : 25;

        if (auto result = WorldDatabase.PQuery("SELECT * FROM  (SELECT ct.entry AS 'ctj', plt.item, plt.lootmode, plt.groupid, 'Fishing Spot' AS 'source', plt.ChanceOrQuestChance AS 'chance', plt.MinCountOrRef, plt.MaxCount, plt.entry FROM creature_template ct LEFT JOIN pickpocketing_loot_template plt ON plt.entry = ct.pickpocketloot WHERE ct.pickpocketloot > 0 AND ct.entry = %u) results ORDER BY ABS(chance) DESC LIMIT %u;", entry, limit))
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START LOOT from Pickpocketing NPC ID. |cffff0000|Harea:" << "|h" << entry << "|h|r, limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";

            do
            {
                if (auto fields2 = result->Fetch())
                {
                    uint32 reward_entry{ fields2[1].GetUInt32() };
                    uint32 loot_mode{ fields2[2].GetUInt32() };
                    uint32 group_id{ fields2[3].GetUInt32() };
                    std::string source_name{ fields2[4].GetString() };
                    auto chance{ fields2[5].GetFloat() };
                    auto min_count_or_ref{ fields2[6].GetInt32() };
                    bool ref{ min_count_or_ref < 0 };
                    uint32 min_count{ uint32(ref ? 1 : min_count_or_ref) };
                    bool quest_required{ chance < 0.f };
                    float true_chance{ fabs(chance) };
                    auto max_count{ fields2[7].GetUInt32() };
                    auto fishing_loot_id{ fields2[8].GetUInt32() };

                    if (ref)
                    {
                        ostream2 << "|cffADFF2F|Harea:" << "|h" << "Table " << "|h|r " << "|cffff0000|Harea:" << "|h" << -min_count_or_ref << "|h|r has chance: ";
                        ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to be rolled ";
                        ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                        ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from pick_loot no. " << "|cffff0000|Harea:" << "|h" << fishing_loot_id << "|h|r" << ", group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                    }
                    else
                        if (auto t = sObjectMgr->GetItemTemplate(reward_entry))
                        {
                            ostream2 << t->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << " has chance: ";
                            ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to be dropped ";
                            ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                            ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from pick_loot no. " << "|cffff0000|Harea:" << "|h" << fishing_loot_id << "|h|r" << ", group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                        }

                }

            } while (result->NextRow());

            ostream2 << "-- END LOOT from Pickpocketing NPC ID. |cffff0000|Harea:" << "|h" << entry << "|h|r, limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->SendSysMessage(log);
            return true;
        }
        else
        {
            handler->PSendSysMessage("Database did not return any results for %u", entry);
            return true;
        }
        return true;
    }

    static bool HandleLookupLootFromTableCommand(ChatHandler* handler, char const* args)
    {

        if (!args)
            return false;

        char* arg = strtok((char*)args, " ");

        if (!arg)
            return false;

        // ORDER BY DateTime DESC LIMIT %u;", ARGs, limit))
        std::string AccStr = arg;
        uint32 entry = uint32(atoi(arg) ? atoi(arg) : 0);

        char* limitStr = strtok(NULL, " ");
        int32 limit = limitStr ? atoi(limitStr) : 25;

        //                                                  0           1               2               3               4               5           6           7           8           9                           10                  11              12          13                     
            if (auto result = WorldDatabase.PQuery("SELECT * FROM (SELECT entry, item, lootmode, groupid, 'item' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount FROM reference_loot_template WHERE entry = %u) results ORDER BY ABS(chance) DESC LIMIT %u;", entry, limit))
            {
                std::ostringstream ostream2{ "" };
                ostream2 << "-- START LOOT DROPS from TABLE " << "|cffff0000|Harea:" << "|h" << entry << " |h|r results" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";


                do
                {
                    if (auto fields2 = result->Fetch())
                    {
                        uint32 reward_entry{ fields2[1].GetUInt32() };
                        uint32 loot_mode{ fields2[2].GetUInt32() };
                        uint32 group_id{ fields2[3].GetUInt32() };
                        std::string source_name{ fields2[4].GetString() };
                        auto chance{ fields2[5].GetFloat() };
                        auto min_count_or_ref{ fields2[6].GetInt32() };
                        bool ref{ min_count_or_ref < 0 };
                        uint32 min_count{ uint32(ref ? 1 : min_count_or_ref) };
                        bool quest_required{ chance < 0.f };
                        float true_chance{ fabs(chance) };
                        auto max_count{ fields2[7].GetUInt32() };

                        if (ref)
                        {
                            ostream2 << "|cffADFF2F|Harea:" << "|h" << "Table " << "|h|r " << "|cffff0000|Harea:" << "|h" << -min_count_or_ref << "|h|r has chance: ";
                            ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to be rolled ";
                            ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                            ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                        }
                        else
                            if (auto t = sObjectMgr->GetItemTemplate(reward_entry))
                            {
                                ostream2 << "|cffADFF2F|Harea:" << "|h" << source_name << "|h|r " << t->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << " has chance: ";
                                ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to be dropped ";
                                ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                                ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                            }

                    }

                } while (result->NextRow());

                ostream2 << "-- START LOOT DROPS from TABLE " << "|cffff0000|Harea:" << "|h" << entry << " |h|r results" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";
                auto resulting_string{ ostream2.str() };
                auto log{ resulting_string.c_str() };
                handler->SendSysMessage(log);
                return true;
            }
            else
            {
                handler->PSendSysMessage("Database did not return any results for %u", entry);
                return true;
            }
        return true;
    }

    static bool HandleLookupLootFromChestCommand(ChatHandler* handler, char const* args)
    {

        if (!args)
            return false;

        char* arg = strtok((char*)args, " ");

        if (!arg)
            return false;

        // ORDER BY DateTime DESC LIMIT %u;", ARGs, limit))
        std::string AccStr = arg;
        uint32 entry = uint32(atoi(arg) ? atoi(arg) : 0);

        char* limitStr = strtok(NULL, " ");
        int32 limit = limitStr ? atoi(limitStr) : 25;

        if (auto result = WorldDatabase.PQuery("SELECT * FROM  (SELECT gob.entry AS 'gobj', glt.item, glt.lootmode, glt.groupid, 'Chest' AS 'source', glt.ChanceOrQuestChance AS 'chance', glt.MinCountOrRef, glt.MaxCount, glt.entry FROM gameobject_template gob LEFT JOIN gameobject_loot_template glt ON glt.entry = gob.data1 WHERE gob.type = 3 AND gob.entry = %u) results ORDER BY ABS(chance) DESC LIMIT %u;", entry, limit))
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START LOOT SOURCES from Chest ID. |cffff0000|Harea:" << "|h" << entry << "|h|r, limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";

            do
            {
                if (auto fields2 = result->Fetch())
                {
                    uint32 reward_entry{ fields2[1].GetUInt32() };
                    uint32 loot_mode{ fields2[2].GetUInt32() };
                    uint32 group_id{ fields2[3].GetUInt32() };
                    std::string source_name{ fields2[4].GetString() };
                    auto chance{ fields2[5].GetFloat() };
                    auto min_count_or_ref{ fields2[6].GetInt32() };
                    bool ref{ min_count_or_ref < 0 };
                    uint32 min_count{ uint32(ref ? 1 : min_count_or_ref) };
                    bool quest_required{ chance < 0.f };
                    float true_chance{ fabs(chance) };
                    auto max_count{ fields2[7].GetUInt32() };
                    auto fishing_loot_id{ fields2[8].GetUInt32() };

                    if (ref)
                    {
                        ostream2 << "|cffADFF2F|Harea:" << "|h" << "Table " << "|h|r " << "|cffff0000|Harea:" << "|h" << -min_count_or_ref << "|h|r has chance: ";
                        ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to be rolled ";
                        ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                        ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from gobj_loot no. " << "|cffff0000|Harea:" << "|h" << fishing_loot_id << "|h|r" << ", group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                    }
                    else
                        if (auto t = sObjectMgr->GetItemTemplate(reward_entry))
                        {
                            ostream2 << t->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << " has chance: ";
                            ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to be dropped ";
                            ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                            ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from gobj_loot no. " << "|cffff0000|Harea:" << "|h" << fishing_loot_id << "|h|r" << ", group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                        }

                }

            } while (result->NextRow());

            ostream2 << "-- END LOOT SOURCES from Chest ID. |cffff0000|Harea:" << "|h" << entry << "|h|r, limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->SendSysMessage(log);
            return true;
        }
        else
        {
            handler->PSendSysMessage("Database did not return any results for %u", entry);
            return true;
        }
        return true;
    }


    static bool HandleLookupLootFromSpellCommand(ChatHandler* handler, char const* args)
    {

        if (!args)
            return false;

        char* arg = strtok((char*)args, " ");

        if (!arg)
            return false;

        // ORDER BY DateTime DESC LIMIT %u;", ARGs, limit))
        std::string AccStr = arg;
        uint32 entry = uint32(atoi(arg) ? atoi(arg) : 0);

        char* limitStr = strtok(NULL, " ");
        int32 limit = limitStr ? atoi(limitStr) : 25;

        //                                                  0           1               2               3               4               5           6           7           8           9                           10                  11              12          13                     
        if (auto result = WorldDatabase.PQuery("SELECT * FROM (SELECT entry, item, lootmode, groupid, 'item' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount FROM spell_loot_template WHERE entry = %u) results ORDER BY ABS(chance) DESC LIMIT %u;", entry, limit))
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START LOOT DROPS from SPELL " << "|cffff0000|Harea:" << "|h" << entry << " |h|r results" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";


            do
            {
                if (auto fields2 = result->Fetch())
                {
                    uint32 reward_entry{ fields2[1].GetUInt32() };
                    uint32 loot_mode{ fields2[2].GetUInt32() };
                    uint32 group_id{ fields2[3].GetUInt32() };
                    std::string source_name{ fields2[4].GetString() };
                    auto chance{ fields2[5].GetFloat() };
                    auto min_count_or_ref{ fields2[6].GetInt32() };
                    bool ref{ min_count_or_ref < 0 };
                    uint32 min_count{ uint32(ref ? 1 : min_count_or_ref) };
                    bool quest_required{ chance < 0.f };
                    float true_chance{ fabs(chance) };
                    auto max_count{ fields2[7].GetUInt32() };

                    if (ref)
                    {
                        ostream2 << "|cffADFF2F|Harea:" << "|h" << "Spell " << "|h|r " << "|cffff0000|Harea:" << "|h" << -min_count_or_ref << "|h|r has chance: ";
                        ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to be rolled ";
                        ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                        ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                    }
                    else
                        if (auto t = sObjectMgr->GetItemTemplate(reward_entry))
                        {
                            ostream2 << "|cffADFF2F|Harea:" << "|h" << source_name << "|h|r " << t->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << " has chance: ";
                            ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to be dropped ";
                            ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                            ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                        }

                }

            } while (result->NextRow());

            ostream2 << "-- START LOOT DROPS from SPELL " << "|cffff0000|Harea:" << "|h" << entry << " |h|r results" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->SendSysMessage(log);
            return true;
        }
        else
        {
            handler->PSendSysMessage("Database did not return any results for %u", entry);
            return true;
        }
        return true;
    }


    static bool HandleLookupLootFromMailCommand(ChatHandler* handler, char const* args)
    {

        if (!args)
            return false;

        char* arg = strtok((char*)args, " ");

        if (!arg)
            return false;

        // ORDER BY DateTime DESC LIMIT %u;", ARGs, limit))
        std::string AccStr = arg;
        uint32 entry = uint32(atoi(arg) ? atoi(arg) : 0);

        char* limitStr = strtok(NULL, " ");
        int32 limit = limitStr ? atoi(limitStr) : 25;

        //                                                  0           1               2               3               4               5           6           7           8           9                           10                  11              12          13                     
        if (auto result = WorldDatabase.PQuery("SELECT * FROM (SELECT entry, item, lootmode, groupid, 'item' AS 'source', ChanceOrQuestChance AS 'chance', MinCountOrRef, MaxCount FROM mail_loot_template WHERE entry = %u) results ORDER BY ABS(chance) DESC LIMIT %u;", entry, limit))
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START LOOT DROPS from Mail " << "|cffff0000|Harea:" << "|h" << entry << " |h|r results" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";


            do
            {
                if (auto fields2 = result->Fetch())
                {
                    uint32 reward_entry{ fields2[1].GetUInt32() };
                    uint32 loot_mode{ fields2[2].GetUInt32() };
                    uint32 group_id{ fields2[3].GetUInt32() };
                    std::string source_name{ fields2[4].GetString() };
                    auto chance{ fields2[5].GetFloat() };
                    auto min_count_or_ref{ fields2[6].GetInt32() };
                    bool ref{ min_count_or_ref < 0 };
                    uint32 min_count{ uint32(ref ? 1 : min_count_or_ref) };
                    bool quest_required{ chance < 0.f };
                    float true_chance{ fabs(chance) };
                    auto max_count{ fields2[7].GetUInt32() };

                    if (ref)
                    {
                        ostream2 << "|cffADFF2F|Harea:" << "|h" << "Spell " << "|h|r " << "|cffff0000|Harea:" << "|h" << -min_count_or_ref << "|h|r has chance: ";
                        ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to be rolled ";
                        ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                        ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                    }
                    else
                        if (auto t = sObjectMgr->GetItemTemplate(reward_entry))
                        {
                            ostream2 << "|cffADFF2F|Harea:" << "|h" << source_name << "|h|r " << t->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << " has chance: ";
                            ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to be dropped ";
                            ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                            ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                        }

                }

            } while (result->NextRow());

            ostream2 << "-- START LOOT DROPS from Mail " << "|cffff0000|Harea:" << "|h" << entry << " |h|r results" << ", limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->SendSysMessage(log);
            return true;
        }
        else
        {
            handler->PSendSysMessage("Database did not return any results for %u", entry);
            return true;
        }
        return true;
    }


    static bool HandleLookupLootFromCreatureCommand(ChatHandler* handler, char const* args)
    {

        if (!args)
            return false;

        char* arg = strtok((char*)args, " ");

        if (!arg)
            return false;

        // ORDER BY DateTime DESC LIMIT %u;", ARGs, limit))
        std::string AccStr = arg;
        uint32 entry = uint32(atoi(arg) ? atoi(arg) : 0);

        char* limitStr = strtok(NULL, " ");
        int32 limit = limitStr ? atoi(limitStr) : 25;

        if (auto result = WorldDatabase.PQuery("SELECT * FROM  (SELECT ct.entry AS 'ctj', slt.item, slt.lootmode, slt.groupid, 'Fishing Spot' AS 'source', slt.ChanceOrQuestChance AS 'chance', slt.MinCountOrRef, slt.MaxCount, slt.entry FROM creature_template ct LEFT JOIN creature_loot_template slt ON slt.entry = ct.lootid WHERE ct.lootid > 0 AND ct.entry = %u) results ORDER BY ABS(chance) DESC LIMIT %u;", entry, limit))
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START LOOT from Looting NPC ID. |cffff0000|Harea:" << "|h" << entry << "|h|r, limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";

            do
            {
                if (auto fields2 = result->Fetch())
                {
                    uint32 reward_entry{ fields2[1].GetUInt32() };
                    uint32 loot_mode{ fields2[2].GetUInt32() };
                    uint32 group_id{ fields2[3].GetUInt32() };
                    std::string source_name{ fields2[4].GetString() };
                    auto chance{ fields2[5].GetFloat() };
                    auto min_count_or_ref{ fields2[6].GetInt32() };
                    bool ref{ min_count_or_ref < 0 };
                    uint32 min_count{ uint32(ref ? 1 : min_count_or_ref) };
                    bool quest_required{ chance < 0.f };
                    float true_chance{ fabs(chance) };
                    auto max_count{ fields2[7].GetUInt32() };
                    auto fishing_loot_id{ fields2[8].GetUInt32() };

                    if (ref)
                    {
                        ostream2 << "|cffADFF2F|Harea:" << "|h" << "Table " << "|h|r " << "|cffff0000|Harea:" << "|h" << -min_count_or_ref << "|h|r has chance: ";
                        ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to be rolled ";
                        ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                        ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from npc_loot no. " << "|cffff0000|Harea:" << "|h" << fishing_loot_id << "|h|r" << ", group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                    }
                    else
                        if (auto t = sObjectMgr->GetItemTemplate(reward_entry))
                        {
                            ostream2 << t->GetItemLink(false, 0, 0, 0, 0, 0, 0, 0) << " has chance: ";
                            ostream2 << "|cffff0000|Harea:" << "|h" << chance << "%|h|r" << " to be dropped ";
                            ostream2 << "(|cffADFF2F|Harea:" << "|h" << min_count << "|h|r";
                            ostream2 << " to|cffADFF2F|Harea:" << "|h " << max_count << "|h|r) times from npc_loot no. " << "|cffff0000|Harea:" << "|h" << fishing_loot_id << "|h|r" << ", group |cffADFF2F|Harea:" << "|h" << group_id << "|h|r.\n";
                        }

                }

            } while (result->NextRow());

            ostream2 << "-- END LOOT from Looting NPC ID. |cffff0000|Harea:" << "|h" << entry << "|h|r, limited to |cffff0000|Harea:" << "|h" << limit << "|h|r results" << " --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->SendSysMessage(log);
            return true;
        }
        else
        {
            handler->PSendSysMessage("Database did not return any results for %u", entry);
            return true;
        }
        return true;
    }

};

void AddSC_lookup_commandscript()
{
    new lookup_commandscript();
}
