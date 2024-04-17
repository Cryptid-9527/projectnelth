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

#include "BattlegroundEYR.h"
#include "World.h"
#include "WorldPacket.h"
#include "ObjectMgr.h"
#include "BattlegroundMgr.h"
#include "Creature.h"
#include "Language.h"
#include "Object.h"
#include "Player.h"
#include "Util.h"
#include "WorldSession.h"

BattlegroundEYR::BattlegroundEYR()
{
    m_IsInformedNearVictory = false;
    m_BuffChange = true;
    BgObjects.resize(BG_EYR_OBJECT_MAX);
    BgCreatures.resize(BG_EYR_ALL_NODES_COUNT + 5);//+5 for aura triggers
    m_Points_Trigger[EYR_FEL_REAVER] = EYR_TR_FEL_REAVER_BUFF;
    m_Points_Trigger[EYR_BLOOD_ELF] = EYR_TR_BLOOD_ELF_BUFF;
    m_Points_Trigger[EYR_DRAENEI_RUINS] = EYR_TR_DRAENEI_RUINS_BUFF;
    m_Points_Trigger[EYR_MAGE_TOWER] = EYR_TR_MAGE_TOWER_BUFF;

    StartMessageIds[BG_STARTING_EVENT_FIRST]  = LANG_BG_EY_START_TWO_MINUTES;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_EY_START_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_BG_EY_START_HALF_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_EY_HAS_BEGUN;
}

BattlegroundEYR::~BattlegroundEYR()
{
}

void BattlegroundEYR::PostUpdateImpl(uint32 diff)
{
    if (GetStatus() == STATUS_IN_PROGRESS)
    {
        int team_points[BG_TEAMS_COUNT] = { 0, 0 };

        if (mui_flagTimer <= diff)
        {
            if (m_FlagState == BG_EYR_FLAG_STATE_ON_PLAYER)
                if (Player *player = ObjectAccessor::FindPlayer(GetFlagPickerGUID()))
                {
                    for (uint8 node = BG_EYR_NODE_FEL_REAVER; node < BG_EYR_DYNAMIC_NODES_COUNT; node++)
                    {
                        if (GameObject *obj = GetBgMap()->GetGameObject(BgObjects[node*8+BG_EYR_OBJECT_AURA_CONTESTED]))
                        {
                            if (player->IsWithinDistInMap(obj, 3.0f))
                            {
                                if ((m_Nodes[node] == BG_EYR_NODE_STATUS_ALLY_OCCUPIED && player->GetTeam() == ALLIANCE) ||
                                    (m_Nodes[node] == BG_EYR_NODE_STATUS_HORDE_OCCUPIED && player->GetTeam() == HORDE))
                                    EventPlayerCapturedFlag(player, node*8+BG_EYR_OBJECT_AURA_CONTESTED);
                            }
                        }
                    }
                }
            mui_flagTimer = 1000;
        }
        else
            mui_flagTimer -= diff;

        if (m_FlagState == BG_EYR_FLAG_STATE_WAIT_RESPAWN || m_FlagState == BG_EYR_FLAG_STATE_ON_GROUND)
        {
            m_FlagsTimer -= diff;

            if (m_FlagsTimer < 0)
            {
                m_FlagsTimer = 0;
                if (m_FlagState == BG_EYR_FLAG_STATE_WAIT_RESPAWN)
                    RespawnFlag(true);
                else
                    RespawnFlagAfterDrop();
            }
        }

        for (int node = 0; node < BG_EYR_DYNAMIC_NODES_COUNT; ++node)
        {
            // 3 sec delay to spawn new banner instead previous despawned one
            if (m_BannerTimers[node].timer)
            {
                if (m_BannerTimers[node].timer > diff)
                    m_BannerTimers[node].timer -= diff;
                else
                {
                    m_BannerTimers[node].timer = 0;
                    _CreateBanner(node, m_BannerTimers[node].type, m_BannerTimers[node].teamIndex, false);
                }
            }

            // 1-minute to occupy a node from contested state
            if (m_NodeTimers[node])
            {
                if (m_NodeTimers[node] > diff)
                    m_NodeTimers[node] -= diff;
                else
                {
                    m_NodeTimers[node] = 0;
                    // Change from contested to occupied !
                    uint8 teamIndex = m_Nodes[node]-1;
                    
                    m_Nodes[node] += 2;
                    // burn current contested banner
                    _DelBanner(node, BG_EYR_NODE_TYPE_CONTESTED, teamIndex);
                    // create new occupied banner
                    _CreateBanner(node, BG_EYR_NODE_TYPE_OCCUPIED, teamIndex, true);
                    _NodeOccupied(node, (teamIndex == 0) ? ALLIANCE:HORDE);
                    _SendNodeUpdate(node);
                    // Message to chatlog
                   
                }
            }

            for (int team = 0; team < BG_TEAMS_COUNT; ++team)
                if (m_Nodes[node] == team + BG_EYR_NODE_TYPE_OCCUPIED)
                    ++team_points[team];
        }

        // Accumulate points
        for (int team = 0; team < BG_TEAMS_COUNT; ++team)
        {
            int points = team_points[team];
            if (!points)
                continue;
            m_lastTick[team] += diff;
            if (m_lastTick[team] > BG_EYR_TickIntervals[points])
            {
                m_lastTick[team] -= BG_EYR_TickIntervals[points];
                m_TeamScores[team] += BG_EYR_TickPoints[points];
                m_HonorScoreTics[team] += BG_EYR_TickPoints[points];
                m_ReputationScoreTics[team] += BG_EYR_TickPoints[points];
                if (m_ReputationScoreTics[team] >= m_ReputationTics)
                {
                    RewardReputationToTeam(509, 510, 10, team == ALLIANCE ? ALLIANCE : HORDE);
                    m_ReputationScoreTics[team] -= m_ReputationTics;
                }
                if (m_HonorScoreTics[team] >= m_HonorTics)
                {
                    RewardHonorToTeam(GetBonusHonorFromKill(1), (team == TEAM_ALLIANCE) ? ALLIANCE : HORDE);
                    m_HonorScoreTics[team] -= m_HonorTics;
                }
                if (!m_IsInformedNearVictory && m_TeamScores[team] > BG_EYR_WARNING_NEAR_VICTORY_SCORE)
                {
                    if (team == TEAM_ALLIANCE)
                        SendMessageToAll(LANG_BG_EYR_A_NEAR_VICTORY, CHAT_MSG_BG_SYSTEM_NEUTRAL);
                    else
                        SendMessageToAll(LANG_BG_EYR_H_NEAR_VICTORY, CHAT_MSG_BG_SYSTEM_NEUTRAL);
                    PlaySoundToAll(BG_EYR_SOUND_NEAR_VICTORY);
                    m_IsInformedNearVictory = true;
                }

                if (m_TeamScores[team] > BG_EYR_MAX_TEAM_SCORE)
                    m_TeamScores[team] = BG_EYR_MAX_TEAM_SCORE;
                if (team == TEAM_ALLIANCE)
                    UpdateWorldState(BG_EYR_OP_RESOURCES_ALLY, m_TeamScores[team]);
                if (team == TEAM_HORDE)
                    UpdateWorldState(BG_EYR_OP_RESOURCES_HORDE, m_TeamScores[team]);
                // update achievement flags
                // we increased m_TeamScores[team] so we just need to check if it is 500 more than other teams resources
                uint8 otherTeam = (team + 1) % BG_TEAMS_COUNT;
                if (m_TeamScores[team] > m_TeamScores[otherTeam] + 500)
                    m_TeamScores500Disadvantage[otherTeam] = true;
            }
        }

        // Test win condition
        if (m_TeamScores[TEAM_ALLIANCE] >= BG_EYR_MAX_TEAM_SCORE)
            EndBattleground(ALLIANCE);
        if (m_TeamScores[TEAM_HORDE] >= BG_EYR_MAX_TEAM_SCORE)
            EndBattleground(HORDE);
    }
}

void BattlegroundEYR::StartingEventCloseDoors()
{
    SpawnBGObject(BG_EYR_OBJECT_GATE_A, RESPAWN_IMMEDIATELY);
    SpawnBGObject(BG_EYR_OBJECT_GATE_H, RESPAWN_IMMEDIATELY);

    // despawn banners, auras and buffs
    for (int obj = BG_EYR_OBJECT_BANNER_NEUTRAL; obj < BG_EYR_DYNAMIC_NODES_COUNT * 8; ++obj)
        SpawnBGObject(obj, RESPAWN_ONE_DAY);
    for (int i = 0; i < BG_EYR_DYNAMIC_NODES_COUNT * 3; ++i)
        SpawnBGObject(BG_EYR_OBJECT_SPEEDBUFF_FEL_REAVER + i, RESPAWN_ONE_DAY);

    // Starting base spirit guides
    _NodeOccupied(EYR_SPIRIT_MAIN_ALLIANCE, ALLIANCE);
    _NodeOccupied(EYR_SPIRIT_MAIN_HORDE, HORDE);
}

void BattlegroundEYR::StartingEventOpenDoors()
{
    SpawnBGObject(BG_EYR_OBJECT_GATE_A, RESPAWN_ONE_DAY);
    SpawnBGObject(BG_EYR_OBJECT_GATE_H, RESPAWN_ONE_DAY);

    // spawn neutral banners
    for (int banner = BG_EYR_OBJECT_BANNER_NEUTRAL, i = 0; i < 5; banner += 8, ++i)
        SpawnBGObject(banner, RESPAWN_IMMEDIATELY);
    for (int i = 0; i < BG_EYR_DYNAMIC_NODES_COUNT; ++i)
    {
        //randomly select buff to spawn
        uint8 buff = urand(0, 2);
        SpawnBGObject(BG_EYR_OBJECT_SPEEDBUFF_FEL_REAVER + buff + i * 3, RESPAWN_IMMEDIATELY);
    }

    // Achievement: Flurry
    StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, EYR_EVENT_START_BATTLE);
}

void BattlegroundEYR::AddPlayer(Player* player)
{
    Battleground::AddPlayer(player);
    //create score and add it to map, default values are set in the constructor
    BattlegroundEYRScore* sc = new BattlegroundEYRScore;
    PlayerScores[player->GetGUID()] = sc;
    sc->BgTeam = player->GetTeam();
    sc->TalentTree = player->GetPrimaryTalentTree(player->GetActiveSpec());
}

void BattlegroundEYR::RemovePlayer(Player* player, uint64 guid, uint32 /*team*/)
{
    if (IsFlagPickedup())
    {
        if (m_FlagKeeper == guid)
        {
            if (player)
                EventPlayerDroppedFlag(player);
            else
            {
                SetFlagPicker(0);
                RespawnFlag(true);
            }
        }
    }
}

void BattlegroundEYR::HandleKillPlayer(Player* player, Player* killer)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    if (killer)
        if (player)
            Battleground::HandleKillPlayer(player, killer);
        else TC_LOG_ERROR("bg.battleground", "BattlegroundEYR::HandleKillPlayer: Killer player not found");
    else TC_LOG_ERROR("bg.battleground", "BattlegroundEYR::HandleKillPlayer: Victim player not found");

    EventPlayerDroppedFlag(player);
}

void BattlegroundEYR::HandleAreaTrigger(Player* player, uint32 trigger)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    Battleground::HandleAreaTrigger(player, trigger);
}

/*  type: 0-neutral, 1-contested, 3-occupied
    teamIndex: 0-ally, 1-horde                        */
void BattlegroundEYR::_CreateBanner(uint8 node, uint8 type, uint8 teamIndex, bool delay)
{
    // Just put it into the queue
    if (delay)
    {
        m_BannerTimers[node].timer = 2000;
        m_BannerTimers[node].type = type;
        m_BannerTimers[node].teamIndex = teamIndex;
        return;
    }

    uint8 obj = node*8 + type + teamIndex;

    SpawnBGObject(obj, RESPAWN_IMMEDIATELY);

    // handle aura with banner
    if (!type)
        return;
    obj = node * 8 + ((type == BG_EYR_NODE_TYPE_OCCUPIED) ? (5 + teamIndex) : 7);
    SpawnBGObject(obj, RESPAWN_IMMEDIATELY);
}

void BattlegroundEYR::_DelBanner(uint8 node, uint8 type, uint8 teamIndex)
{
    uint8 obj = node*8 + type + teamIndex;
    SpawnBGObject(obj, RESPAWN_ONE_DAY);

    // handle aura with banner
    if (!type)
        return;
    obj = node * 8 + ((type == BG_EYR_NODE_TYPE_OCCUPIED) ? (5 + teamIndex) : 7);
    SpawnBGObject(obj, RESPAWN_ONE_DAY);
}

void BattlegroundEYR::UpdatePointWorldStates()
{
    UpdateWorldState(BG_EYR_NODE_FELREAVER_RUINS_WS, GameStateToWorldState(m_Nodes[EYR_FEL_REAVER]));
    UpdateWorldState(BG_EYR_NODE_MAGE_TOWER_WS, GameStateToWorldState(m_Nodes[EYR_MAGE_TOWER]));
    UpdateWorldState(BG_EYR_NODE_DRAENEI_RUINS_WS, GameStateToWorldState(m_Nodes[EYR_DRAENEI_RUINS]));
    UpdateWorldState(BG_EYR_NODE_BLOODELF_TOWER_WS, GameStateToWorldState(m_Nodes[EYR_BLOOD_ELF]));
}

uint32 BattlegroundEYR::GameStateToWorldState(uint32 GameState)
{
    switch (GameState)
    {
    case BG_EYR_NODE_TYPE_NEUTRAL:                  return STATE_NEUTRAL;               break;
    case BG_EYR_NODE_STATUS_ALLY_CONTESTED:         return STATE_ALLIANCE_ASSAULT;      break;
    case BG_EYR_NODE_STATUS_HORDE_CONTESTED:        return STATE_HORDE_ASSAULT;         break;
    case BG_EYR_NODE_STATUS_ALLY_OCCUPIED:          return STATE_ALLIANCE_CONTROL;      break;
    case BG_EYR_NODE_STATUS_HORDE_OCCUPIED:         return STATE_HORDE_CONTROL;         break;
    default:
        TC_LOG_ERROR("bg.battleground", "BattleGroundEYR: GameStateToWorldState returning NULL.");
        return NULL;
        break;
    }
    TC_LOG_ERROR("bg.battleground", "BattleGroundEYR: GameStateToWorldState returning NULL.");
    return NULL;
}

void BattlegroundEYR::FillInitialWorldStates(WorldPacket& data)
{
    // How many bases each team owns
    uint32 ally = 0, horde = 0;
    for (uint8 node = 0; node < BG_EYR_DYNAMIC_NODES_COUNT; ++node)
        if (m_Nodes[node] == BG_EYR_NODE_STATUS_ALLY_OCCUPIED)
            ++ally;
        else if (m_Nodes[node] == BG_EYR_NODE_STATUS_HORDE_OCCUPIED)
            ++horde;

    data << uint32(BG_EYR_OP_OCCUPIED_BASES_HORDE) << uint32(horde);
    data << uint32(BG_EYR_OP_OCCUPIED_BASES_ALLY)  << uint32(ally);
    data << uint32(0xab6) << uint32(0x0);
    data << uint32(0xab5) << uint32(0x0);
    data << uint32(0xab4) << uint32(0x0);
    data << uint32(0xab3) << uint32(0x0);
    data << uint32(0xab2) << uint32(0x0);
    data << uint32(0xab1) << uint32(0x0);
    data << uint32(0xab0) << uint32(0x0);
    data << uint32(0xaaf) << uint32(0x0);
    
    data << uint32(BG_EYR_NODE_FELREAVER_RUINS_WS) << uint32(GameStateToWorldState(m_Nodes[EYR_FEL_REAVER]));
    data << uint32(BG_EYR_NODE_MAGE_TOWER_WS) << uint32(GameStateToWorldState(m_Nodes[EYR_MAGE_TOWER]));
    data << uint32(BG_EYR_NODE_DRAENEI_RUINS_WS) << uint32(GameStateToWorldState(m_Nodes[EYR_DRAENEI_RUINS]));
    data << uint32(BG_EYR_NODE_BLOODELF_TOWER_WS) << uint32(GameStateToWorldState(m_Nodes[EYR_BLOOD_ELF]));

    data << uint32(EYR_NETHERSTORM_FLAG)                << uint32(m_FlagState == BG_EYR_FLAG_STATE_ON_BASE);

    // Team scores
    data << uint32(BG_EYR_OP_RESOURCES_MAX)      << uint32(BG_EYR_MAX_TEAM_SCORE);
    data << uint32(BG_EYR_OP_RESOURCES_WARNING)  << uint32(BG_EYR_WARNING_NEAR_VICTORY_SCORE);

    data << uint32(BG_EYR_OP_RESOURCES_ALLY)     << uint32(m_TeamScores[TEAM_ALLIANCE]);
    data << uint32(BG_EYR_OP_RESOURCES_HORDE)    << uint32(m_TeamScores[TEAM_HORDE]);
    data << uint32(0xa05) << uint32(0x8e);
    data << uint32(0xaa0) << uint32(0x0);
    data << uint32(0xa9f) << uint32(0x0);
    data << uint32(0xa9e) << uint32(0x0);
    data << uint32(0xc0d) << uint32(0x17b);
}

void BattlegroundEYR::_SendNodeUpdate(uint8 /*node*/)
{  
    UpdatePointWorldStates();

    uint32 ally = 0, horde = 0;
    for (uint8 i = 0; i < BG_EYR_DYNAMIC_NODES_COUNT; ++i)
        if (m_Nodes[i] == BG_EYR_NODE_STATUS_ALLY_OCCUPIED)
            ++ally;
        else if (m_Nodes[i] == BG_EYR_NODE_STATUS_HORDE_OCCUPIED)
            ++horde;

    UpdateWorldState(BG_EYR_OP_OCCUPIED_BASES_ALLY, ally);
    UpdateWorldState(BG_EYR_OP_OCCUPIED_BASES_HORDE, horde);
}

void BattlegroundEYR::_NodeOccupied(uint8 node, Team team)
{
    WorldSafeLocsEntry const* sg = sWorldSafeLocsStore.LookupEntry(BG_EYR_GraveyardIds[node]);
    if (!sg || !AddSpiritGuide(node, sg->x, sg->y, sg->z, 3.124139f, team))
        TC_LOG_ERROR("bg.battleground", "BatteGroundEY: Failed to spawn spirit guide! point: %u, team: %u, graveyard_id: %u",
                       node, team, BG_EYR_GraveyardIds[node]);

    SendMessageToAll((team == ALLIANCE ? m_CapturingPointTypesR[node].MessageIdAlliance : m_CapturingPointTypesR[node].MessageIdHorde), (team == ALLIANCE ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE), NULL);
    PlaySoundToAll(team == ALLIANCE ? BG_EYR_SOUND_ALLIANCE_CAPTURE : BG_EYR_SOUND_HORDE_CAPTURE);

    uint8 capturedNodes = 0;
    for (uint8 i = 0; i < BG_EYR_DYNAMIC_NODES_COUNT; ++i)
    {
        if (m_Nodes[node] == GetTeamIndexByTeamId(team) + BG_EYR_NODE_TYPE_OCCUPIED && !m_NodeTimers[i])
            ++capturedNodes;
    }


    if (node >= BG_EYR_DYNAMIC_NODES_COUNT)//only dynamic nodes, no start points
        return;

    Creature* trigger = BgCreatures[node+7] ? GetBGCreature(node+7) : NULL;//0-6 spirit guides
    if (!trigger)
        trigger = AddCreature(WORLD_TRIGGER, node+7, team, BG_EYR_NodePositions[node][0], BG_EYR_NodePositions[node][1], BG_EYR_NodePositions[node][2], BG_EYR_NodePositions[node][3]);

    //add bonus honor aura trigger creature when node is accupied
    //cast bonus aura (+50% honor in 25yards)
    //aura should only apply to players who have accupied the node, set correct faction for trigger
    if (trigger)
    {
        trigger->setFaction(team == ALLIANCE ? 84 : 83);
        trigger->CastSpell(trigger, SPELL_HONORABLE_DEFENDER_25Y, false);
    }
}

void BattlegroundEYR::_NodeDeOccupied(uint8 node)
{
    if (node >= BG_EYR_DYNAMIC_NODES_COUNT)
        return;

    //remove bonus honor aura trigger creature when node is lost
    if (node < BG_EYR_DYNAMIC_NODES_COUNT)//only dynamic nodes, no start points
        DelCreature(node+7);//NULL checks are in DelCreature! 0-6 spirit guides

    // Those who are waiting to resurrect at this node are taken to the closest own node's graveyard
    std::vector<uint64> ghost_list = m_ReviveQueue[BgCreatures[node]];
    if (!ghost_list.empty())
    {
        WorldSafeLocsEntry const* ClosestGrave = NULL;
        for (std::vector<uint64>::const_iterator itr = ghost_list.begin(); itr != ghost_list.end(); ++itr)
        {
            Player* player = ObjectAccessor::FindPlayer(*itr);
            if (!player)
                continue;

            if (!ClosestGrave)                              // cache
                ClosestGrave = GetClosestGraveYard(player);

            if (ClosestGrave)
                player->TeleportTo(player->GetMapId(), ClosestGrave->x, ClosestGrave->y, ClosestGrave->z, player->GetOrientation());
        }
    }

    if (BgCreatures[node])
        DelCreature(node);


    // buff object isn't despawned
}

/* Invoked if a player used a banner as a gameobject */
void BattlegroundEYR::EventPlayerClickedOnFlag(Player* source, GameObject* /*target_obj*/)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    uint8 node = BG_EYR_NODE_FEL_REAVER;
    GameObject* obj = GetBgMap()->GetGameObject(BgObjects[node*8+7]);
    while ((node < BG_EYR_DYNAMIC_NODES_COUNT) && ((!obj) || (!source->IsWithinDistInMap(obj, 10))))
    {
        ++node;
        obj = GetBgMap()->GetGameObject(BgObjects[node*8+BG_EYR_OBJECT_AURA_CONTESTED]);
    }

    if (node == BG_EYR_DYNAMIC_NODES_COUNT)
    {
        // this means our player isn't close to any of banners - maybe cheater ??
        return;
    }

    TeamId teamIndex = GetTeamIndexByTeamId(source->GetTeam());

    // Check if player really could use this banner, not cheated
    if (!(m_Nodes[node] == 0 || teamIndex == m_Nodes[node]%2))
        return;

    source->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);
    m_prevNodes[node] = m_Nodes[node];

    switch (m_Nodes[node])//status of node in question
    {
    case BG_EYR_NODE_TYPE_NEUTRAL:
        UpdatePlayerScore(source, SCORE_BASES_ASSAULTED, 1);
        m_Nodes[node] = teamIndex + 1;
        // burn current neutral banner
        _DelBanner(node, BG_EYR_NODE_TYPE_NEUTRAL, 0);
        // create new contested banner
        _CreateBanner(node, BG_EYR_NODE_TYPE_CONTESTED, teamIndex, true);
        _SendNodeUpdate(node);
        m_NodeTimers[node] = BG_EYR_FLAG_CAPTURING_TIME;
        SendMessage2ToAll(LANG_BG_EYR_NODE_CLAIMED, (source->GetTeam() == ALLIANCE ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE), source, _GetNodeNameId(node), (source->GetTeam() == ALLIANCE ? LANG_BG_EYR_ALLY : LANG_BG_EYR_HORDE));
        PlaySoundToAll(BG_EYR_SOUND_NODE_CLAIMED);
        break;
    case BG_EYR_NODE_STATUS_ALLY_CONTESTED:
    case BG_EYR_NODE_STATUS_HORDE_CONTESTED:
        if (m_Nodes[node] == (source->GetTeam() == ALLIANCE ? BG_EYR_NODE_STATUS_ALLY_CONTESTED : BG_EYR_NODE_STATUS_HORDE_CONTESTED))
        {
            TC_LOG_ERROR("bg.battleground", (source->GetTeam() == ALLIANCE ? "BattlegroundEYR:: Alliance player click Alliance contested banner: %u clicked %u" : "BattlegroundEYR:: Horde player click Horde contested banner: %u clicked %u"), source->GetGUID(), node);
            return;
        }
        switch (m_prevNodes[node])// If last state is NOT ocupied, change node to enemy-contestedc
        {
        case BG_EYR_NODE_TYPE_NEUTRAL://CONTESTED BANNER THAT WAS PREVIOUSLY NEUTRAL
            UpdatePlayerScore(source, SCORE_BASES_ASSAULTED, 1);
            m_Nodes[node] = teamIndex + BG_EYR_NODE_TYPE_CONTESTED;
            _DelBanner(node, BG_EYR_NODE_TYPE_NEUTRAL, !teamIndex);
            _CreateBanner(node, BG_EYR_NODE_TYPE_CONTESTED, teamIndex, true);
            _SendNodeUpdate(node);
            m_NodeTimers[node] = BG_EYR_FLAG_CAPTURING_TIME;
            SendMessage2ToAll(LANG_BG_EYR_NODE_ASSAULTED, (source->GetTeam() == ALLIANCE ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE), source, _GetNodeNameId(node));
            PlaySoundToAll(source->GetTeam() == ALLIANCE ? BG_EYR_SOUND_ASSAULT_ALLIANCE : BG_EYR_SOUND_ASSAULT_HORDE);
            break;
        case BG_EYR_NODE_STATUS_ALLY_CONTESTED://CONTESTED BANNER FLIPPED AND BACK
        case BG_EYR_NODE_STATUS_HORDE_CONTESTED://CONTESTED BANNER FLIPPED AND BACK
            UpdatePlayerScore(source, SCORE_BASES_ASSAULTED, 1);
            m_Nodes[node] = teamIndex + BG_EYR_NODE_TYPE_CONTESTED;
            _DelBanner(node, BG_EYR_NODE_TYPE_CONTESTED, !teamIndex);
            _CreateBanner(node, BG_EYR_NODE_TYPE_CONTESTED, teamIndex, true);
            _SendNodeUpdate(node);
            m_NodeTimers[node] = BG_EYR_FLAG_CAPTURING_TIME;
            SendMessage2ToAll(LANG_BG_EYR_NODE_ASSAULTED, (source->GetTeam() == ALLIANCE ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE), source, _GetNodeNameId(node));
            PlaySoundToAll(source->GetTeam() == ALLIANCE ? BG_EYR_SOUND_ASSAULT_ALLIANCE : BG_EYR_SOUND_ASSAULT_HORDE);
            break;
        case BG_EYR_NODE_STATUS_ALLY_OCCUPIED://DEFENDING A PREVIOUSLY HELD BASE
        case BG_EYR_NODE_STATUS_HORDE_OCCUPIED://DEFENDING A PREVIOUSLY HELD BASE
            UpdatePlayerScore(source, SCORE_BASES_DEFENDED, 1);
            m_Nodes[node] = teamIndex + BG_EYR_NODE_TYPE_OCCUPIED;
            _DelBanner(node, BG_EYR_NODE_TYPE_CONTESTED, !teamIndex);
            _CreateBanner(node, BG_EYR_NODE_TYPE_OCCUPIED, teamIndex, true);
            _SendNodeUpdate(node);
            m_NodeTimers[node] = 0;
            _NodeOccupied(node, (teamIndex == TEAM_ALLIANCE) ? ALLIANCE : HORDE);
            SendMessage2ToAll(LANG_BG_EYR_NODE_DEFENDED, (source->GetTeam() == ALLIANCE ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE), source, _GetNodeNameId(node));
            PlaySoundToAll(source->GetTeam() == ALLIANCE ? BG_EYR_SOUND_ALLIANCE_CAPTURE : BG_EYR_SOUND_HORDE_CAPTURE);
            break;
        default:
            TC_LOG_ERROR("bg.battleground", "BattlegroundEYR.cpp: m_prevNodes[node] returning default value");
            break;
        }
        break;
    case BG_EYR_NODE_STATUS_ALLY_OCCUPIED:
    case BG_EYR_NODE_STATUS_HORDE_OCCUPIED:
        if (m_Nodes[node] == (source->GetTeam() == ALLIANCE ? BG_EYR_NODE_STATUS_ALLY_OCCUPIED : BG_EYR_NODE_STATUS_HORDE_OCCUPIED))
        {
            TC_LOG_ERROR("bg.battleground", (source->GetTeam() == ALLIANCE ? "BattlegroundEYR:: Alliance player click Alliance occupied banner: %u clicked %u" : "BattlegroundEYR:: Horde player click Horde occupied banner: %u clicked %u"), source->GetGUID(), node);
            return;
        }
        UpdatePlayerScore(source, SCORE_BASES_ASSAULTED, 1);
        m_Nodes[node] = teamIndex + BG_EYR_NODE_TYPE_CONTESTED;
        _DelBanner(node, BG_EYR_NODE_TYPE_OCCUPIED, !teamIndex);
        _CreateBanner(node, BG_EYR_NODE_TYPE_CONTESTED, teamIndex, true);
        _SendNodeUpdate(node);
        _NodeDeOccupied(node);
        SendMessage2ToAll(LANG_BG_EYR_NODE_ASSAULTED, (source->GetTeam() == ALLIANCE ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE), source, _GetNodeNameId(node));
        m_NodeTimers[node] = BG_EYR_FLAG_CAPTURING_TIME;
        PlaySoundToAll((teamIndex == TEAM_ALLIANCE) ? BG_EYR_SOUND_ASSAULT_ALLIANCE : BG_EYR_SOUND_ASSAULT_HORDE);
        break;
    default:
        TC_LOG_ERROR("bg.battleground", "BattlegroundEYR.cpp: m_Nodes[node] returning default value");
        break;
    }
}

void BattlegroundEYR::EventPlayerDroppedFlag(Player* Source)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
    {
        // if not running, do not cast things at the dropper player, neither send unnecessary messages
        // just take off the aura
        if (IsFlagPickedup() && GetFlagPickerGUID() == Source->GetGUID())
        {
            SetFlagPicker(0);
            Source->RemoveAurasDueToSpell(BG_EYR_NETHERSTORM_FLAG_SPELL);
        }
        return;
    }

    if (!IsFlagPickedup())
        return;

    if (GetFlagPickerGUID() != Source->GetGUID())
        return;

    SetFlagPicker(0);
    Source->RemoveAurasDueToSpell(BG_EYR_NETHERSTORM_FLAG_SPELL);
    m_FlagState = BG_EYR_FLAG_STATE_ON_GROUND;
    m_FlagsTimer = BG_EYR_FLAG_RESPAWN_TIME;
    Source->CastSpell(Source, SPELL_RECENTLY_DROPPED_FLAG, true);
    Source->CastSpell(Source, BG_EYR_PLAYER_DROPPED_FLAG_SPELL, true);
    //this does not work correctly :((it should remove flag carrier name)
    UpdateWorldState(EYR_NETHERSTORM_FLAG_STATE_HORDE, BG_EYR_FLAG_STATE_WAIT_RESPAWN);
    UpdateWorldState(EYR_NETHERSTORM_FLAG_STATE_ALLIANCE, BG_EYR_FLAG_STATE_WAIT_RESPAWN);

    if (Source->GetTeam() == ALLIANCE)
        SendMessageToAll(LANG_BG_EY_DROPPED_FLAG, CHAT_MSG_BG_SYSTEM_ALLIANCE, NULL);
    else
        SendMessageToAll(LANG_BG_EY_DROPPED_FLAG, CHAT_MSG_BG_SYSTEM_HORDE, NULL);
}

int32 BattlegroundEYR::_GetNodeNameId(uint8 node)
{
    switch (node)
    {
    case EYR_FEL_REAVER:        return LANG_BG_EYR_NODE_FRR;
    case EYR_BLOOD_ELF:         return LANG_BG_EYR_NODE_BET;
    case EYR_DRAENEI_RUINS:     return LANG_BG_EYR_NODE_DR;
    case EYR_MAGE_TOWER:        return LANG_BG_EYR_NODE_MT;
    default:
        ASSERT(false);
    }
    return 0;
}

void BattlegroundEYR::EventPlayerClickedOnMiddleFlag(Player* Source, GameObject* target_obj)
{
    if (GetStatus() != STATUS_IN_PROGRESS || IsFlagPickedup() || !Source->IsWithinDistInMap(target_obj, 10) || (Source->isGameMaster()) || (!Source->isGMVisible()))
        return;

    if (Source->GetTeam() == ALLIANCE)
    {
        UpdateWorldState(EYR_NETHERSTORM_FLAG_STATE_ALLIANCE, BG_EYR_FLAG_STATE_ON_PLAYER);      
    }
    else
    {
        UpdateWorldState(EYR_NETHERSTORM_FLAG_STATE_HORDE, BG_EYR_FLAG_STATE_ON_PLAYER);        
    }

    if (m_FlagState == BG_EYR_FLAG_STATE_ON_BASE)
        UpdateWorldState(EYR_NETHERSTORM_FLAG, 0);
    m_FlagState = BG_EYR_FLAG_STATE_ON_PLAYER;

    SpawnBGObject(BG_EYR_OBJECT_FLAG_NETHERSTORM, RESPAWN_ONE_DAY);
    SetFlagPicker(Source->GetGUID());
    PlaySoundToAll((Source->GetTeam() == ALLIANCE) ? BG_EYR_SOUND_ASSAULT_ALLIANCE : BG_EYR_SOUND_ASSAULT_HORDE);
    // Get flag aura on player
    Source->CastSpell(Source, BG_EYR_NETHERSTORM_FLAG_SPELL, true);
    Source->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);

    if (Source->GetTeam() == ALLIANCE)
        PSendMessageToAll(LANG_BG_EY_HAS_TAKEN_FLAG, CHAT_MSG_BG_SYSTEM_ALLIANCE, NULL, Source->GetName().c_str());
    else
        PSendMessageToAll(LANG_BG_EY_HAS_TAKEN_FLAG, CHAT_MSG_BG_SYSTEM_HORDE, NULL, Source->GetName().c_str());
}

void BattlegroundEYR::EventPlayerCapturedFlag(Player* Source, uint32 BgObjectType)
{
    if (GetStatus() != STATUS_IN_PROGRESS || GetFlagPickerGUID() != Source->GetGUID())
        return;

    SetFlagPicker(0);
    m_FlagState = BG_EYR_FLAG_STATE_WAIT_RESPAWN;
    Source->RemoveAurasDueToSpell(BG_EYR_NETHERSTORM_FLAG_SPELL);

    Source->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);

    //    if (Source->GetTeam() == ALLIANCE)
    //    PlaySoundToAll(BG_EYR_SOUND_ALLIANCE_CAPTURE);
    // else
    //    PlaySoundToAll(BG_EYR_SOUND_HORDE_CAPTURE);

    SpawnBGObject(BgObjectType, RESPAWN_IMMEDIATELY);

    m_FlagsTimer = BG_EYR_FLAG_RESPAWN_TIME;
    m_FlagCapturedBgObjectType = BgObjectType;
   

    uint8 ally = 0, horde = 0;
    for (uint8 i = 0; i < BG_EYR_DYNAMIC_NODES_COUNT; ++i)
        if (m_Nodes[i] == BG_EYR_NODE_STATUS_ALLY_OCCUPIED)
            ++ally;
        else if (m_Nodes[i] == BG_EYR_NODE_STATUS_HORDE_OCCUPIED)
            ++horde;

    if ((Source->GetTeam() == HORDE ? horde : ally) > 0)
        UpdatePlayerScore(Source, SCORE_FLAG_CAPTURES, BG_EYR_FlagPoints[Source->GetTeam() == HORDE ? (horde - 1) : (ally - 1)]);

    //send audio and visual notifications of the flag
    SendMessageToAll((Source->GetTeam() == ALLIANCE ? LANG_BG_EY_CAPTURED_FLAG_A : LANG_BG_EY_CAPTURED_FLAG_H), (Source->GetTeam() == ALLIANCE ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE), Source);
    PlaySoundToAll(Source->GetTeam() == ALLIANCE ? BG_EYR_SOUND_ALLIANCE_CAPTURE : BG_EYR_SOUND_HORDE_CAPTURE);

    //Update score that was just increased by flag cap
}

void BattlegroundEYR::AddPoints(uint32 Team, uint32 Points)
{
    TeamId team_index = GetTeamIndexByTeamId(Team);
    m_TeamScores[team_index] += Points;
    UpdateWorldState(Team == ALLIANCE ? EYR_ALLIANCE_RESOURCES : EYR_HORDE_RESOURCES, m_TeamScores[team_index]);
}

uint32 BattlegroundEYR::GetPrematureWinner()
{
    // How many bases each team owns
    uint8 ally = 0, horde = 0;
    for (uint8 i = 0; i < BG_EYR_DYNAMIC_NODES_COUNT; ++i)
        if (m_Nodes[i] == BG_EYR_NODE_STATUS_ALLY_OCCUPIED)
            ++ally;
        else if (m_Nodes[i] == BG_EYR_NODE_STATUS_HORDE_OCCUPIED)
            ++horde;

    if (ally > horde)
        return ALLIANCE;
    else if (horde > ally)
        return HORDE;

    // If the values are equal, fall back to the original result (based on number of players on each team)
    return Battleground::GetPrematureWinner();
}

bool BattlegroundEYR::SetupBattleground()
{
    for (int i = 0; i < BG_EYR_DYNAMIC_NODES_COUNT; ++i)
    {
        if (!AddObject(BG_EYR_OBJECT_BANNER_NEUTRAL + 8 * i, isRatedBattleground() ? BG_EYR_RBG_OBJECTID_NODE_BANNER_0 : BG_EYR_OBJECTID_NODE_BANNER_0 + i, BG_EYR_NodePositions[i][0], BG_EYR_NodePositions[i][1], BG_EYR_NodePositions[i][2], BG_EYR_NodePositions[i][3], 0, 0, std::sin(BG_EYR_NodePositions[i][3] / 2), std::cos(BG_EYR_NodePositions[i][3] / 2), RESPAWN_ONE_DAY)
            || !AddObject(BG_EYR_OBJECT_BANNER_CONT_A + 8 * i, isRatedBattleground() ? BG_EYR_RBG_OBJECTID_BANNER_CONT_A : BG_EYR_OBJECTID_BANNER_CONT_A, BG_EYR_NodePositions[i][0], BG_EYR_NodePositions[i][1], BG_EYR_NodePositions[i][2], BG_EYR_NodePositions[i][3], 0, 0, std::sin(BG_EYR_NodePositions[i][3] / 2), std::cos(BG_EYR_NodePositions[i][3] / 2), RESPAWN_ONE_DAY)
            || !AddObject(BG_EYR_OBJECT_BANNER_CONT_H + 8 * i, isRatedBattleground() ? BG_EYR_RBG_OBJECTID_BANNER_CONT_H : BG_EYR_OBJECTID_BANNER_CONT_H, BG_EYR_NodePositions[i][0], BG_EYR_NodePositions[i][1], BG_EYR_NodePositions[i][2], BG_EYR_NodePositions[i][3], 0, 0, std::sin(BG_EYR_NodePositions[i][3] / 2), std::cos(BG_EYR_NodePositions[i][3] / 2), RESPAWN_ONE_DAY)
            || !AddObject(BG_EYR_OBJECT_BANNER_ALLY + 8 * i, isRatedBattleground() ? BG_EYR_RBG_OBJECTID_BANNER_A : BG_EYR_OBJECTID_BANNER_A, BG_EYR_NodePositions[i][0], BG_EYR_NodePositions[i][1], BG_EYR_NodePositions[i][2], BG_EYR_NodePositions[i][3], 0, 0, std::sin(BG_EYR_NodePositions[i][3] / 2), std::cos(BG_EYR_NodePositions[i][3] / 2), RESPAWN_ONE_DAY)
            || !AddObject(BG_EYR_OBJECT_BANNER_HORDE + 8 * i, isRatedBattleground() ? BG_EYR_RBG_OBJECTID_BANNER_H : BG_EYR_OBJECTID_BANNER_H, BG_EYR_NodePositions[i][0], BG_EYR_NodePositions[i][1], BG_EYR_NodePositions[i][2], BG_EYR_NodePositions[i][3], 0, 0, std::sin(BG_EYR_NodePositions[i][3] / 2), std::cos(BG_EYR_NodePositions[i][3] / 2), RESPAWN_ONE_DAY)
            || !AddObject(BG_EYR_OBJECT_AURA_ALLY + 8*i, BG_EYR_OBJECTID_AURA_A, BG_EYR_NodePositions[i][0], BG_EYR_NodePositions[i][1], BG_EYR_NodePositions[i][2], BG_EYR_NodePositions[i][3], 0, 0, std::sin(BG_EYR_NodePositions[i][3]/2), std::cos(BG_EYR_NodePositions[i][3]/2), RESPAWN_ONE_DAY)
            || !AddObject(BG_EYR_OBJECT_AURA_HORDE + 8*i, BG_EYR_OBJECTID_AURA_H, BG_EYR_NodePositions[i][0], BG_EYR_NodePositions[i][1], BG_EYR_NodePositions[i][2], BG_EYR_NodePositions[i][3], 0, 0, std::sin(BG_EYR_NodePositions[i][3]/2), std::cos(BG_EYR_NodePositions[i][3]/2), RESPAWN_ONE_DAY)
            || !AddObject(BG_EYR_OBJECT_AURA_CONTESTED + 8*i, BG_EYR_OBJECTID_AURA_C, BG_EYR_NodePositions[i][0], BG_EYR_NodePositions[i][1], BG_EYR_NodePositions[i][2], BG_EYR_NodePositions[i][3], 0, 0, std::sin(BG_EYR_NodePositions[i][3]/2), std::cos(BG_EYR_NodePositions[i][3]/2), RESPAWN_ONE_DAY)
            )
        {
            TC_LOG_ERROR("sql.sql", "BatteGroundEYR: Failed to spawn some object Battleground not created!");
            return false;
        }
    }

    if (!AddObject(BG_EYR_OBJECT_GATE_A, BG_OBJECT_A_DOOR_EYR_ENTRY, 2527.6f, 1596.91f, 1262.13f, -3.12414f, -0.173642f, -0.001515f, 0.98477f, -0.008594f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_EYR_OBJECT_GATE_H, BG_OBJECT_H_DOOR_EYR_ENTRY, 1803.21f, 1539.49f, 1261.09f, 3.14159f, 0.173648f, 0, 0.984808f, 0, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_EYR_OBJECT_FLAG_NETHERSTORM, 184141, 2174.249756f, 1568.513185f, 1159.740235f, -1.503602f, 0, 0, 0.662620f, -0.748956f, RESPAWN_ONE_DAY))
    {
        TC_LOG_ERROR("sql.sql", "BatteGroundEYR: Failed to spawn door object Battleground not created!");
        return false;
    }
    //buffs
    for (int i = 0; i < BG_EYR_DYNAMIC_NODES_COUNT; ++i)
    {
        AreaTriggerEntry const* at = sAreaTriggerStore.LookupEntry(m_Points_Trigger[i]);
        if (!at)
        {
            TC_LOG_ERROR("bg.battleground", "BattlegroundEY: Unknown trigger: %u", m_Points_Trigger[i]);
            continue;
        }
        if (!AddObject(BG_EYR_OBJECT_SPEEDBUFF_FEL_REAVER + i * 3, Buff_Entries[0], at->x, at->y, at->z, 0.907571f, 0, 0, 0.438371f, 0.898794f, RESPAWN_ONE_DAY)
            || !AddObject(BG_EYR_OBJECT_SPEEDBUFF_FEL_REAVER + i * 3 + 1, Buff_Entries[1], at->x, at->y, at->z, 0.907571f, 0, 0, 0.438371f, 0.898794f, RESPAWN_ONE_DAY)
            || !AddObject(BG_EYR_OBJECT_SPEEDBUFF_FEL_REAVER + i * 3 + 2, Buff_Entries[2], at->x, at->y, at->z, 0.907571f, 0, 0, 0.438371f, 0.898794f, RESPAWN_ONE_DAY))
            TC_LOG_ERROR("bg.battleground", "BattlegroundEY: Cannot spawn buff");


    }

    WorldSafeLocsEntry const* sg = NULL;
    sg = sWorldSafeLocsStore.LookupEntry(EYR_GRAVEYARD_MAIN_ALLIANCE);
    if (!sg || !AddSpiritGuide(EYR_SPIRIT_MAIN_ALLIANCE, sg->x, sg->y, sg->z, 3.124139f, ALLIANCE))
    {
        TC_LOG_ERROR("sql.sql", "BatteGroundEY: Failed to spawn spirit guide! Battleground not created!");
        return false;
    }

    sg = sWorldSafeLocsStore.LookupEntry(EYR_GRAVEYARD_MAIN_HORDE);
    if (!sg || !AddSpiritGuide(EYR_SPIRIT_MAIN_HORDE, sg->x, sg->y, sg->z, 3.193953f, HORDE))
    {
        TC_LOG_ERROR("sql.sql", "BatteGroundEY: Failed to spawn spirit guide! Battleground not created!");
        return false;
    }
    for (uint8 i = 0; i < 5; ++i)
    {
        Creature* trigger = GetBGCreature(i + BG_EYR_ALL_NODES_COUNT);//0-5 spirit guides
        if (!trigger)
            trigger = AddCreature(WORLD_TRIGGER, i + BG_EYR_ALL_NODES_COUNT, TEAM_NEUTRAL, BG_EYR_TriggerPositions[i][0], BG_EYR_TriggerPositions[i][1], BG_EYR_TriggerPositions[i][2], BG_EYR_TriggerPositions[i][3]);
    }

    return true;
}

void BattlegroundEYR::Reset()
{
    //call parent's class reset
    Battleground::Reset();

    m_TeamScores[TEAM_ALLIANCE]          = 0;
    m_TeamScores[TEAM_HORDE]             = 0;
    m_lastTick[TEAM_ALLIANCE]            = 0;
    m_lastTick[TEAM_HORDE]               = 0;
    m_HonorScoreTics[TEAM_ALLIANCE]      = 0;
    m_HonorScoreTics[TEAM_HORDE]         = 0;
    m_ReputationScoreTics[TEAM_ALLIANCE] = 0;
    m_ReputationScoreTics[TEAM_HORDE]    = 0;
    m_FlagCapturedBgObjectType           = 0;
    m_FlagKeeper                         = 0;
    m_DroppedFlagGUID                    = 0;
    mui_flagTimer = 1000;
    m_FlagState                          = BG_EYR_FLAG_STATE_ON_BASE;
    m_IsInformedNearVictory              = false;
    bool isBGWeekend = sBattlegroundMgr->IsBGWeekend(GetTypeID());
    m_HonorTics = (isBGWeekend) ? BG_EYR_EYRBGWeekendHonorTicks : BG_EYR_NotEYRBGWeekendHonorTicks;
    m_ReputationTics = (isBGWeekend) ? BG_EYR_EYRBGWeekendReputationTicks : BG_EYR_NotEYRBGWeekendReputationTicks;
    m_TeamScores500Disadvantage[TEAM_ALLIANCE] = false;
    m_TeamScores500Disadvantage[TEAM_HORDE]    = false;

    for (uint8 i = 0; i < BG_EYR_DYNAMIC_NODES_COUNT; ++i)
    {
        m_Nodes[i] = 0;
        m_prevNodes[i] = 0;
        m_NodeTimers[i] = 0;
        m_BannerTimers[i].timer = 0;
    }

    for (uint8 i = 0; i < BG_EYR_ALL_NODES_COUNT + 5; ++i)//+5 for aura triggers
        if (BgCreatures[i])
            DelCreature(i);
}

void BattlegroundEYR::RespawnFlag(bool send_message)
{
    if (m_FlagCapturedBgObjectType > 0)
        SpawnBGObject(m_FlagCapturedBgObjectType, RESPAWN_ONE_DAY);

    m_FlagCapturedBgObjectType = 0;
    m_FlagState = BG_EYR_FLAG_STATE_ON_BASE;
    SpawnBGObject(BG_EYR_OBJECT_FLAG_NETHERSTORM, RESPAWN_IMMEDIATELY);

    if (send_message)
    {
        SendMessageToAll(LANG_BG_EY_RESETED_FLAG, CHAT_MSG_BG_SYSTEM_NEUTRAL);
        PlaySoundToAll(BG_EYR_SOUND_FLAG_RESET);             // flags respawned sound...
    }

    UpdateWorldState(EYR_NETHERSTORM_FLAG, 1);
}

void BattlegroundEYR::RespawnFlagAfterDrop()
{
    RespawnFlag(true);

    GameObject* obj = HashMapHolder<GameObject>::Find(GetDroppedFlagGUID());
    if (obj)
        obj->Delete();
    else
        TC_LOG_ERROR("bg.battleground", "BattlegroundEY: Unknown dropped flag guid: %u", GUID_LOPART(GetDroppedFlagGUID()));

    SetDroppedFlagGUID(0);
}

void BattlegroundEYR::EndBattleground(uint32 winner)
{
    // Win reward
    if (winner == ALLIANCE)
        RewardHonorToTeam(GetBonusHonorFromKill(1), ALLIANCE);
    if (winner == HORDE)
        RewardHonorToTeam(GetBonusHonorFromKill(1), HORDE);
    // Complete map_end rewards (even if no team wins)
    RewardHonorToTeam(GetBonusHonorFromKill(1), HORDE);
    RewardHonorToTeam(GetBonusHonorFromKill(1), ALLIANCE);

    Battleground::EndBattleground(winner);
}

WorldSafeLocsEntry const* BattlegroundEYR::GetClosestGraveYard(Player* player)
{
    TeamId teamIndex = GetTeamIndexByTeamId(player->GetTeam());
    uint32 g_id = player->GetTeam() == HORDE ? EYR_GRAVEYARD_MAIN_HORDE : EYR_GRAVEYARD_MAIN_ALLIANCE;
    // Is there any occupied node for this team?
    std::vector<uint8> nodes;
    for (uint8 i = 0; i < BG_EYR_DYNAMIC_NODES_COUNT; ++i)
        if (m_Nodes[i] == teamIndex + 3)
            nodes.push_back(i);

    WorldSafeLocsEntry const* good_entry = sWorldSafeLocsStore.LookupEntry(g_id);
    // If so, select the closest node to place ghost on
    if (!nodes.empty())
    {
        float plr_x = player->GetPositionX();
        float plr_y = player->GetPositionY();

        float mindist = 999999.0f;
        for (uint8 i = 0; i < nodes.size(); ++i)
        {
            WorldSafeLocsEntry const*entry = sWorldSafeLocsStore.LookupEntry(BG_EYR_GraveyardIds[nodes[i]]);
            if (!entry)
                continue;
            float dist = (entry->x - plr_x)*(entry->x - plr_x)+(entry->y - plr_y)*(entry->y - plr_y);
            if (mindist > dist)
            {
                mindist = dist;
                good_entry = entry;
            }
        }
        nodes.clear();
    }

    return good_entry;
}

void BattlegroundEYR::UpdatePlayerScore(Player* Source, uint32 type, uint32 value, bool doAddHonor)
{
    BattlegroundScoreMap::iterator itr = PlayerScores.find(Source->GetGUID());
    if (itr == PlayerScores.end())                         // player not found...
        return;

    switch (type)
    {
        case SCORE_BASES_ASSAULTED:
            ((BattlegroundEYRScore*)itr->second)->BasesAssaulted += value;
            Source->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, EYR_OBJECTIVE_ASSAULT_BASE);
            break;
        case SCORE_BASES_DEFENDED:
            ((BattlegroundEYRScore*)itr->second)->BasesDefended += value;
            Source->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, EYR_OBJECTIVE_DEFEND_BASE);
            break;
        case SCORE_FLAG_CAPTURES:                           // flags captured
            ((BattlegroundEYRScore*)itr->second)->FlagCaptures += value;
            Source->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, EYR_OBJECTIVE_CAPTURE_FLAG);
            break;
        default:
            Battleground::UpdatePlayerScore(Source, type, value, doAddHonor);
            break;
    }
}

bool BattlegroundEYR::IsAllNodesConrolledByTeam(uint32 team) const
{
    uint32 count = 0;
    for (int i = 0; i < BG_EYR_DYNAMIC_NODES_COUNT; ++i)
        if ((team == ALLIANCE && m_Nodes[i] == BG_EYR_NODE_STATUS_ALLY_OCCUPIED) ||
            (team == HORDE    && m_Nodes[i] == BG_EYR_NODE_STATUS_HORDE_OCCUPIED))
            ++count;

    return count == BG_EYR_DYNAMIC_NODES_COUNT;
}
