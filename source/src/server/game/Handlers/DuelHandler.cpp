/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Log.h"
#include "Opcodes.h"
#include "UpdateData.h"
#include "Player.h"
#include "Chat.h"
#include "Pet.h"

void WorldSession::HandleDuelAcceptedOpcode(WorldPacket& recvPacket)
{
    uint64 guid;
    Player* player;
    Player* plTarget;

    recvPacket >> guid;

    if (!GetPlayer()->duel)                                  // ignore accept from duel-sender
        return;

    player       = GetPlayer();
    plTarget = player->duel->opponent;

    if (player == player->duel->initiator || !plTarget || player == plTarget || player->duel->startTime != 0 || plTarget->duel->startTime != 0)
        return;

    //TC_LOG_DEBUG(LOG_FILTER_PACKETIO, "WORLD: Received CMSG_DUEL_ACCEPTED");
    TC_LOG_DEBUG("network.opcode", "Player 1 is: %u (%s)", player->GetGUIDLow(), player->GetName().c_str());
    TC_LOG_DEBUG("network.opcode", "Player 2 is: %u (%s)", plTarget->GetGUIDLow(), plTarget->GetName().c_str());

    time_t now = time(NULL);
    player->duel->startTimer = now;
    plTarget->duel->startTimer = now;

    player->DuelCooldownReset(plTarget);
    player->SendDuelCountdown(3000);
    plTarget->SendDuelCountdown(3000);

    player->duel->accepted = true;
    plTarget->duel->accepted = true;
}
void Player::DuelCooldownReset(Player* challenger)
{
    //TC_LOG_ERROR("entities.unit", "1");
    /*
    To prevent players from using duel completion as a means to forcefully reset their cooldowns with another player,
    we will move the reset to the event where the second player accepts the duel.

    We have a few conditions for this to happen:
    1. We want the players to be in pre-determined areas (mainly outside their city) so that any possible abuse can be reported by bystanders.
    2. We want to check for temporary summons to prevent abuse intended to hurt performance.
    */

    //Script is occuring in the accepting player's "Player" entity.
    if (sWorld->getBoolConfig(CONFIG_RESET_ARENA_COOLDOWNS_AFTER_DUEL))
    {
        if (!IsDuelSupported())      
            return;   
        /*
            Clear Diminishing Returns
        */
        ToUnit()->ClearDiminishings();
        if (Pet* myPet = GetPet())
            myPet->ToUnit()->ClearDiminishings();

        challenger->ToUnit()->ClearDiminishings();
        if (Pet* chalPet = challenger->GetPet())
            chalPet->ToUnit()->ClearDiminishings();
        /*
            Refresh Accepter
        */
        RemoveArenaSpellCooldowns(true);
        SetHealth(GetMaxHealth());
        if (getPowerType() == POWER_MANA)
            SetPower(POWER_MANA, GetMaxPower(POWER_MANA));
        /*
            Refresh Challenger
        */
        challenger->RemoveArenaSpellCooldowns(true);
        challenger->SetHealth(challenger->GetMaxHealth());
        if (challenger->getPowerType() == POWER_MANA)
            challenger->SetPower(POWER_MANA, challenger->GetMaxPower(POWER_MANA));
        /*
            Warlock Refresh soul shards
        */
        if (getClass() == CLASS_WARLOCK)
        {
            if (ToUnit()->GetPower(POWER_SOUL_SHARDS) != ToUnit()->GetMaxPower(POWER_SOUL_SHARDS))
                ToUnit()->CastSpell(ToUnit(), 101977, true);
            if (ToUnit()->GetPower(POWER_SOUL_SHARDS) != ToUnit()->GetMaxPower(POWER_SOUL_SHARDS))
                ToUnit()->CastSpell(ToUnit(), 101977, true);
            if (ToUnit()->GetPower(POWER_SOUL_SHARDS) != ToUnit()->GetMaxPower(POWER_SOUL_SHARDS))
                ToUnit()->CastSpell(ToUnit(), 101977, true);
        }
        if (challenger->getClass() == CLASS_WARLOCK)
        {
            if (challenger->ToUnit()->GetPower(POWER_SOUL_SHARDS) != challenger->ToUnit()->GetMaxPower(POWER_SOUL_SHARDS))
                challenger->ToUnit()->CastSpell(challenger->ToUnit(), 101977, true);
            if (challenger->ToUnit()->GetPower(POWER_SOUL_SHARDS) != challenger->ToUnit()->GetMaxPower(POWER_SOUL_SHARDS))
                challenger->ToUnit()->CastSpell(challenger->ToUnit(), 101977, true);
            if (challenger->ToUnit()->GetPower(POWER_SOUL_SHARDS) != challenger->ToUnit()->GetMaxPower(POWER_SOUL_SHARDS))
                challenger->ToUnit()->CastSpell(challenger->ToUnit(), 101977, true);
        }
        /*
            Reset Rogue Combo Points
        */
        if (getClass() == CLASS_ROGUE || getClass() == CLASS_DRUID)
            ClearComboPoints();
        if (challenger->getClass() == CLASS_ROGUE || challenger->getClass() == CLASS_DRUID)
            challenger->ClearComboPoints();

        /*
            Reset Paladin Holy Power
        */
        if (getClass() == CLASS_PALADIN)
        {
            if (ToUnit()->GetPower(POWER_HOLY_POWER) != 0)
                Regenerate(POWER_HOLY_POWER);
            if (ToUnit()->GetPower(POWER_HOLY_POWER) != 0)
                Regenerate(POWER_HOLY_POWER);
            if (ToUnit()->GetPower(POWER_HOLY_POWER) != 0)
                Regenerate(POWER_HOLY_POWER);
        }
        if (challenger->getClass() == CLASS_PALADIN)
        {
            if (challenger->ToUnit()->GetPower(POWER_HOLY_POWER) != 0)
                challenger->Regenerate(POWER_HOLY_POWER);
            if (challenger->ToUnit()->GetPower(POWER_HOLY_POWER) != 0)
                challenger->Regenerate(POWER_HOLY_POWER);
            if (challenger->ToUnit()->GetPower(POWER_HOLY_POWER) != 0)
                challenger->Regenerate(POWER_HOLY_POWER);
        }
        /*
            Reset Druid Eclipse
        */
        if (getClass() == CLASS_DRUID)
            if (GetPower(POWER_ECLIPSE) != 0)
            {
                ToUnit()->SetPower(POWER_ECLIPSE, 0);
                RemoveAurasDueToSpell(48517/*Solar Eclipse*/);
                RemoveAurasDueToSpell(67483/*Solar Eclipse Marker*/);
                RemoveAurasDueToSpell(48518/*Lunar Eclipse*/);
                RemoveAurasDueToSpell(67484/*Lunar Eclipse Marker*/);
            }
        if (challenger->getClass() == CLASS_DRUID)
            if (challenger->GetPower(POWER_ECLIPSE) != 0)
            {
                challenger->ToUnit()->SetPower(POWER_ECLIPSE, 0);
                challenger->RemoveAurasDueToSpell(48517/*Solar Eclipse*/);
                challenger->RemoveAurasDueToSpell(67483/*Solar Eclipse Marker*/);
                challenger->RemoveAurasDueToSpell(48518/*Lunar Eclipse*/);
                challenger->RemoveAurasDueToSpell(67484/*Lunar Eclipse Marker*/);
            }
        /*
            Reset Death Knight Runes and runic Power
        */
        if (getClass() == CLASS_DEATH_KNIGHT)
            ToUnit()->SetPower(POWER_RUNIC_POWER, 0);
        if (challenger->getClass() == CLASS_DEATH_KNIGHT)
            challenger->ToUnit()->SetPower(POWER_RUNIC_POWER, 0);
        /*
            Reset warrior and druid rage
        */
        if (getClass() == CLASS_WARRIOR || getClass() == CLASS_DRUID)
            if (GetPower(POWER_RAGE) != 0)
                ToUnit()->SetPower(POWER_RAGE, 0);
        if (challenger->getClass() == CLASS_WARRIOR || challenger->getClass() == CLASS_DRUID)
            if (challenger->GetPower(POWER_RAGE) != 0)
                challenger->ToUnit()->SetPower(POWER_RAGE, 0);

        /* no duration instantcasts, spells longer than 35s -- head dev frog -- 21/10/16 */

        /*
             Bone shield - 1 minute
        */
        if (challenger->getClass() == CLASS_DEATH_KNIGHT)
            challenger->RemoveAurasDueToSpell(49222);
        if (getClass() == CLASS_DEATH_KNIGHT)
            RemoveAurasDueToSpell(49222);
        /*
             Presence of Mind -on use/cancelled
        */
        if (challenger->getClass() == CLASS_MAGE)
            challenger->RemoveAurasDueToSpell(12043);
        if (getClass() == CLASS_MAGE)
            RemoveAurasDueToSpell(12043);
        /*
             Shadow Orbs 1 minute
        */
        if (challenger->getClass() == CLASS_PRIEST)
            challenger->RemoveAurasDueToSpell(77487);
        if (getClass() == CLASS_PRIEST)
            RemoveAurasDueToSpell(77487);
        /*
            Cold Blood on use/cancelled
        */
        if (challenger->getClass() == CLASS_ROGUE)
            challenger->RemoveAurasDueToSpell(14177);
        if (getClass() == CLASS_ROGUE)
            RemoveAurasDueToSpell(14177);
        /*
            Natures Swiftness on use/cancelled
        */
        if (challenger->getClass() == CLASS_DRUID)
            challenger->RemoveAurasDueToSpell(17116);
        if (getClass() == CLASS_DRUID)
            RemoveAurasDueToSpell(17116);
        /*
            Natures Swiftness on use/cancelled
        */
        if (challenger->getClass() == CLASS_SHAMAN)
            challenger->RemoveAurasDueToSpell(16188);
        if (getClass() == CLASS_SHAMAN)
            RemoveAurasDueToSpell(16188);
        /*
            Beacon of Light 5 minutes
        */
        if (challenger->getClass() == CLASS_PALADIN)
            challenger->RemoveAurasDueToSpell(53563);
        if (getClass() == CLASS_PALADIN)
            RemoveAurasDueToSpell(53563);

        ChatHandler(challenger->GetSession()).PSendSysMessage("Your cooldowns have been reset.");
        ChatHandler(GetSession()).PSendSysMessage("Your cooldowns have been reset.");
    }
}

void WorldSession::HandleDuelCancelledOpcode(WorldPacket& recvPacket)
{
    TC_LOG_DEBUG("network.opcode", "WORLD: Received CMSG_DUEL_CANCELLED");
    uint64 guid;
    recvPacket >> guid;

    // no duel requested
    if (!GetPlayer()->duel)
        return;

    // player surrendered in a duel using /forfeit
    if (GetPlayer()->duel->accepted == true)
    {
        GetPlayer()->CombatStopWithPets(true);
        if (GetPlayer()->duel->opponent)
            GetPlayer()->duel->opponent->CombatStopWithPets(true);

        GetPlayer()->CastSpell(GetPlayer(), 7267, true);    // beg
        GetPlayer()->DuelComplete(DUEL_WON);
        return;
    }

    GetPlayer()->DuelComplete(DUEL_INTERRUPTED);
}
