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

#include "Chat.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptMgr.h"

class complaints_commandscript : public CommandScript
{
public:
    complaints_commandscript() : CommandScript("complaints_commandscript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        /*
            BEGIN Ingame complaint system
        */

        static std::vector<ChatCommand> complaintViewTopCommandTable =
        {
            { "source",            SEC_CONSOLE,         false, &HandleComplaintViewTopSourceCommand,             "" },
            { "target",             SEC_CONSOLE,          false, &HandleComplaintViewTopTargetCommand,             "" },
        };

        static std::vector<ChatCommand> complaintViewCommandTable =
        {
            { "top",             SEC_CONSOLE,       false, NULL,                                           "", complaintViewTopCommandTable },
            { "all",            SEC_CONSOLE,        false, &HandleComplaintViewAllCommand,              "" },//done
            { "source",          SEC_CONSOLE,       false, &HandleComplaintViewSourceCommand,              "" },//done
            { "target",          SEC_CONSOLE,       false, &HandleComplaintViewTargetCommand,              "" },//done
            { "type",            SEC_CONSOLE,       false, &HandleComplaintViewTypeCommand,              "" },//done
        };

        static std::vector<ChatCommand> complaintDeleteCommandTable =
        {
            { "source",            SEC_CONSOLE,         false, &HandleComplaintDeleteFromSourceCommand,             "" },//done
            { "oldest",             SEC_CONSOLE,          false, &HandleComplaintDeleteOldCommand,             "" },//done
        };


        static std::vector<ChatCommand> complaintCommandTable =
        {
            { "view",             SEC_CONSOLE,           false, NULL,                                "",complaintViewCommandTable },
            { "delete",           SEC_CONSOLE,           false, NULL,                                "",complaintDeleteCommandTable },
        };
        /*
            END Ingame complaint system
        */


        static std::vector<ChatCommand> commandTable =
        {
            { "complaints",         SEC_CONSOLE,  true,  NULL,                                "", complaintCommandTable },
        };
        return commandTable;
    }

    static bool HandleComplaintViewSourceCommand(ChatHandler* handler, char const* args)
    {

        std::string report_topics[]
        {
            "CHAT_SPAM",        // right click on chat, report for spam
            "CHEATING",         //right click on pic, report for cheating
            "UNK_2",            //unk
            "BAD_CHAR_NAME",    //right click on pic, bad player name
            "UNK_4",            //unk
            "UNK_5",            //unk
            "BAD_GUILD_NAME",   //right click on pic, bad guild name
            "UNK_7",            //unk
            "UNK_8",            //unk
            "BAD_ARENA_NAME",   //right click on pic, bad arena team name
            "CHAT_BAD_LANG",    //right click on chat, report for language
            "UNK_ERR_OUT_OF_RANGE"
        };

        if (!*args)
            return false;

        char* arg = strtok((char*)args, "");
        if (!arg)
            return false;

        std::string ARGs = arg;
        if (!atoi(ARGs.c_str()))
            return false;

        char* limitStr = strtok(NULL, " ");
        int32 limit = limitStr ? atoi(limitStr) : 25;


        LoginDatabase.EscapeString(ARGs);

        if (auto result = CharacterDatabase.PQuery("SELECT * FROM character_complaints WHERE source_guid IN (%s) ORDER BY DateTime DESC LIMIT %u", ARGs, limit))
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START Ingame reports submitted by GUID # |cffff0000|Harea:|h" << ARGs << "|h|r limited to |cffff0000|Harea:|h" << limit << "|h|r results --\n\n";

            do
            {
                if (auto fields2 = result->Fetch())
                {
                    auto date{ fields2[0].GetCString() };
                    uint64 source{ fields2[1].GetUInt64() };
                    auto target{ fields2[2].GetUInt64() };
                    auto type{ fields2[3].GetInt8() };
                    if (type > 10 || type < 0)
                        type = 11;

                    auto count{ fields2[4].GetUInt32() };

                    auto description{ fields2[11].GetCString() };

                    
                    ostream2 << date << ": ";
                    ostream2 << "|cffff0000|Harea:|h" << source << "|h|r reported GUID |cffff0000|Harea:|h" << target << "|h|r for |cffff0000|Harea:|h" << report_topics[type] << "|h|r (|cffff0000|Harea:|hx" << count << "|h|r) with recent comment: '|cffff0000|Harea:|h" << description << "|h|r'\n";
                }

            } while (result->NextRow());

            ostream2 << "\n-- END Ingame reports submitted by GUID # |cffff0000|Harea:|h" << ARGs << "|h|r --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->PSendSysMessage("%s", log);
        }
        else
        {
            handler->PSendSysMessage("Could not find MACRO DETAILS (CHARACTER) for GUID # %s", ARGs);
            return true;
        }
        return true;
    }

    static bool HandleComplaintViewTargetCommand(ChatHandler* handler, char const* args)
    {

        std::string report_topics[]
        {
            "CHAT_SPAM",        // right click on chat, report for spam
            "CHEATING",         //right click on pic, report for cheating
            "UNK_2",            //unk
            "BAD_CHAR_NAME",    //right click on pic, bad player name
            "UNK_4",            //unk
            "UNK_5",            //unk
            "BAD_GUILD_NAME",   //right click on pic, bad guild name
            "UNK_7",            //unk
            "UNK_8",            //unk
            "BAD_ARENA_NAME",   //right click on pic, bad arena team name
            "CHAT_BAD_LANG",    //right click on chat, report for language
            "UNK_ERR_OUT_OF_RANGE"
        };

        if (!*args)
            return false;

        char* arg = strtok((char*)args, "");
        if (!arg)
            return false;

        std::string ARGs = arg;
        if (!atoi(ARGs.c_str()))
            return false;

        char* limitStr = strtok(NULL, " ");
        int32 limit = limitStr ? atoi(limitStr) : 25;


        LoginDatabase.EscapeString(ARGs);

        if (auto result = CharacterDatabase.PQuery("SELECT * FROM character_complaints WHERE target_guid IN (%s) ORDER BY DateTime DESC LIMIT %u", ARGs, limit))
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START Ingame reports targeting GUID # |cffff0000|Harea:|h" << ARGs << "|h|r limited to |cffff0000|Harea:|h" << limit << "|h|r results --\n\n";

            do
            {
                if (auto fields2 = result->Fetch())
                {
                    auto date{ fields2[0].GetCString() };
                    uint64 source{ fields2[1].GetUInt64() };
                    auto target{ fields2[2].GetUInt64() };
                    auto type{ fields2[3].GetInt8() };
                    if (type > 10 || type < 0)
                        type = 11;

                    auto count{ fields2[4].GetUInt32() };

                    auto description{ fields2[11].GetCString() };


                    ostream2 << date << ": ";
                    ostream2 << "|cffff0000|Harea:|h" << target << "|h|r was reported by GUID |cffff0000|Harea:|h" << source << "|h|r for |cffff0000|Harea:|h" << report_topics[type] << "|h|r (|cffff0000|Harea:|hx" << count << "|h|r) with recent comment: '|cffff0000|Harea:|h" << description << "|h|r'\n";
                }

            } while (result->NextRow());

            ostream2 << "\n-- END Ingame reports targeting GUID # |cffff0000|Harea:|h" << ARGs << "|h|r --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->PSendSysMessage("%s", log);
        }
        else
        {
            handler->PSendSysMessage("Could not find MACRO DETAILS (CHARACTER) for GUID # %s", ARGs);
            return true;
        }
        return true;
    }

    static bool HandleComplaintViewAllCommand(ChatHandler* handler, char const* args)
    {

        std::string report_topics[]
        {
            "CHAT_SPAM",        // right click on chat, report for spam
            "CHEATING",         //right click on pic, report for cheating
            "UNK_2",            //unk
            "BAD_CHAR_NAME",    //right click on pic, bad player name
            "UNK_4",            //unk
            "UNK_5",            //unk
            "BAD_GUILD_NAME",   //right click on pic, bad guild name
            "UNK_7",            //unk
            "UNK_8",            //unk
            "BAD_ARENA_NAME",   //right click on pic, bad arena team name
            "CHAT_BAD_LANG",    //right click on chat, report for language
            "UNK_ERR_OUT_OF_RANGE"
        };

        std::string ARGs{ "25" };

        if (*args)
            if (auto arg = strtok((char*)args, ""))
                if (auto submitted_arg = atoi(arg))
                    ARGs = std::string(arg);

        LoginDatabase.EscapeString(ARGs);

        if (auto result = CharacterDatabase.PQuery("SELECT * FROM character_complaints ORDER BY DateTime DESC LIMIT %u", ARGs))
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START all ingame reports submitted, limited to |cffff0000|Harea:|h" << ARGs << "|h|r most recent results --\n";

            do
            {
                if (auto fields2 = result->Fetch())
                {
                    auto date{ fields2[0].GetCString() };
                    uint64 source{ fields2[1].GetUInt64() };
                    auto target{ fields2[2].GetUInt64() };
                    auto type{ fields2[3].GetInt8() };
                    if (type > 10 || type < 0)
                        type = 11;

                    auto count{ fields2[4].GetUInt32() };

                    auto description{ fields2[11].GetCString() };


                    ostream2 << date << ": ";
                    ostream2 << "|cffff0000|Harea:|h" << source << "|h|r reported GUID |cffff0000|Harea:|h" << target << "|h|r for |cffff0000|Harea:|h" << report_topics[type] << "|h|r (|cffff0000|Harea:|hx" << count << "|h|r) with recent comment: '|cffff0000|Harea:|h" << description << "|h|r'\n";
                }

            } while (result->NextRow());

            ostream2 << "\n-- END all ingame reports submitted, limited to |cffff0000|Harea:|h" << ARGs << "|h|r most recent results --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->PSendSysMessage("%s", log);
        }
        else
        {
            handler->PSendSysMessage("Could not find any complaint results.");
            return true;
        }
        return true;
    }

    static bool HandleComplaintViewTypeCommand(ChatHandler* handler, char const* args)
    {

        std::string report_topics[]
        {
            "CHAT_SPAM",        // right click on chat, report for spam
            "CHEATING",         //right click on pic, report for cheating
            "UNK_2",            //unk
            "BAD_CHAR_NAME",    //right click on pic, bad player name
            "UNK_4",            //unk
            "UNK_5",            //unk
            "BAD_GUILD_NAME",   //right click on pic, bad guild name
            "UNK_7",            //unk
            "UNK_8",            //unk
            "BAD_ARENA_NAME",   //right click on pic, bad arena team name
            "CHAT_BAD_LANG",    //right click on chat, report for language
            "UNK_ERR_OUT_OF_RANGE"
        };

        if (!*args)
            return false;

        char* arg = strtok((char*)args, "");
        if (!arg)
            return false;

        std::string ARGs = arg;
        if (!atoi(ARGs.c_str()))
            return false;

        char* limitStr = strtok(NULL, " ");
        int32 limit = limitStr ? atoi(limitStr) : 25;

        auto report_type_requested{ atoi(ARGs.c_str()) };
        if (!report_type_requested || report_type_requested < 0 || report_type_requested > 10)
            report_type_requested = 11;

        LoginDatabase.EscapeString(ARGs);

        if (auto result = CharacterDatabase.PQuery("SELECT * FROM character_complaints WHERE TYPE = %s ORDER BY DateTime DESC LIMIT %u;", ARGs, limit))
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START |cffff0000|Harea:|h"<< report_topics[report_type_requested] << "|h|r ("<< report_type_requested << ") reports, limited to |cffff0000|Harea:|h" << limit << "|h|r most recent results --\n";

            do
            {
                if (auto fields2 = result->Fetch())
                {
                    auto date{ fields2[0].GetCString() };
                    uint64 source{ fields2[1].GetUInt64() };
                    auto target{ fields2[2].GetUInt64() };
                    auto type{ fields2[3].GetInt8() };
                    if (type > 10 || type < 0)
                        type = 11;

                    auto count{ fields2[4].GetUInt32() };

                    auto description{ fields2[11].GetCString() };


                    ostream2 << date << ": ";
                    ostream2 << "|cffff0000|Harea:|h" << source << "|h|r reported GUID |cffff0000|Harea:|h" << target << "|h|r for |cffff0000|Harea:|h" << report_topics[type] << "|h|r (|cffff0000|Harea:|hx" << count << "|h|r) with recent comment: '|cffff0000|Harea:|h" << description << "|h|r'\n";
                }

            } while (result->NextRow());

            ostream2 << "-- END |cffff0000|Harea:|h" << report_topics[report_type_requested] << "|h|r (" << report_type_requested << ") reports, limited to |cffff0000|Harea:|h" << limit << "|h|r most recent results --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->PSendSysMessage("%s", log);
        }
        else
        {
            handler->PSendSysMessage("Could not find any complaint results for type %s", ARGs);
            return true;
        }
        return true;
    }


    static bool HandleComplaintViewTopSourceCommand(ChatHandler* handler, char const* args)
    {

        std::string report_topics[]
        {
            "CHAT_SPAM",        // right click on chat, report for spam
            "CHEATING",         //right click on pic, report for cheating
            "UNK_2",            //unk
            "BAD_CHAR_NAME",    //right click on pic, bad player name
            "UNK_4",            //unk
            "UNK_5",            //unk
            "BAD_GUILD_NAME",   //right click on pic, bad guild name
            "UNK_7",            //unk
            "UNK_8",            //unk
            "BAD_ARENA_NAME",   //right click on pic, bad arena team name
            "CHAT_BAD_LANG",    //right click on chat, report for language
            "UNK_ERR_OUT_OF_RANGE"
        };


        std::string ARGs{ "25" };

        if (*args)
            if (auto arg = strtok((char*)args, ""))
                if (auto submitted_arg = atoi(arg))
                    ARGs = std::string(arg);

        LoginDatabase.EscapeString(ARGs);

        if (auto result = CharacterDatabase.PQuery("SELECT source_guid, SUM(count) as sum FROM character_complaints GROUP BY source_guid ORDER BY SUM DESC LIMIT %s;", ARGs))
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START most active |cffff0000|Harea:|hReport Sources|h|r, limited to |cffff0000|Harea:|h" << ARGs << "|h|r results --\n";

            uint32 rank{ 1 };
            do
            {
                
                if (auto fields2 = result->Fetch())
                {
                    auto source { fields2[0].GetUInt64() };
                    auto count  { fields2[1].GetUInt32() };

                    ostream2 << rank <<". |cffff0000|Harea:|h" << source << "|h|r has |cffff0000|Harea:|h" << count << "|h|r reports submitted.\n";
                }
                rank += 1;
            } while (result->NextRow());

            ostream2 << "-- END most active |cffff0000|Harea:|hReport Sources|h|r, limited to |cffff0000|Harea:|h" << ARGs << "|h|r results --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->PSendSysMessage("%s", log);
        }
        else
        {
            handler->PSendSysMessage("Could not find any complaint results to form most active list.");
            return true;
        }
        return true;
    }
    static bool HandleComplaintViewTopTargetCommand(ChatHandler* handler, char const* args)
    {

        std::string report_topics[]
        {
            "CHAT_SPAM",        // right click on chat, report for spam
            "CHEATING",         //right click on pic, report for cheating
            "UNK_2",            //unk
            "BAD_CHAR_NAME",    //right click on pic, bad player name
            "UNK_4",            //unk
            "UNK_5",            //unk
            "BAD_GUILD_NAME",   //right click on pic, bad guild name
            "UNK_7",            //unk
            "UNK_8",            //unk
            "BAD_ARENA_NAME",   //right click on pic, bad arena team name
            "CHAT_BAD_LANG",    //right click on chat, report for language
            "UNK_ERR_OUT_OF_RANGE"
        };


        std::string ARGs{ "25" };

        if (*args)
            if (auto arg = strtok((char*)args, ""))
                if (auto submitted_arg = atoi(arg))
                    ARGs = std::string(arg);

        LoginDatabase.EscapeString(ARGs);

        if (auto result = CharacterDatabase.PQuery("SELECT target_guid, SUM(count) as sum FROM character_complaints GROUP BY target_guid ORDER BY SUM DESC LIMIT %s;", ARGs))
        {
            std::ostringstream ostream2{ "" };
            ostream2 << "-- START most active |cffff0000|Harea:|hReport Targets|h|r, limited to |cffff0000|Harea:|h" << ARGs << "|h|r results --\n";

            uint32 rank{ 1 };
            do
            {

                if (auto fields2 = result->Fetch())
                {
                    auto source{ fields2[0].GetUInt64() };
                    auto count{ fields2[1].GetUInt32() };

                    ostream2 << rank << ". |cffff0000|Harea:|h" << source << "|h|r has received |cffff0000|Harea:|h" << count << "|h|r reports.\n";
                }
                rank += 1;
            } while (result->NextRow());

            ostream2 << "-- END most active |cffff0000|Harea:|hReport Targets|h|r, limited to |cffff0000|Harea:|h" << ARGs << "|h|r results --\n";
            auto resulting_string{ ostream2.str() };
            auto log{ resulting_string.c_str() };
            handler->PSendSysMessage("%s", log);
        }
        else
        {
            handler->PSendSysMessage("Could not find any complaint results to form most active list.");
            return true;
        }
        return true;
    }

    static bool HandleComplaintDeleteOldCommand(ChatHandler* handler, char const* /*args*/)
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_COMPLAINT_OLDEST);
        CharacterDatabase.DirectExecute(stmt);
        handler->PSendSysMessage("Database has been purged of complaints older than 14 days.");
        return true;
    }

    static bool HandleComplaintDeleteFromSourceCommand(ChatHandler* handler, char const* args)
    {

        if (!*args)
            return false;

        char* arg = strtok((char*)args, "");
        if (!arg)
            return false;

        std::string ARGs = arg;
        if (!atoi(ARGs.c_str()))
            return false;

        if (auto count = CharacterDatabase.PQuery("SELECT COUNT(source_guid) FROM character_complaints WHERE source_guid = %s;", ARGs))
        if (auto stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_COMPLAINT_SOURCE))
        {
            auto reports_found = count->Fetch();
            auto finalCount{ reports_found[0].GetUInt32() };
            stmt->setString(0, ARGs);
            handler->PSendSysMessage("Database has been purged of %u complaints submitted by GUID %s.", finalCount, ARGs);
            CharacterDatabase.DirectExecute(stmt);
        }

        return true;
    }
};

void AddSC_complaints_commandscript()
{
    new complaints_commandscript();
}
