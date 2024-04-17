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
//Basic headers
#include "WaypointMovementGenerator.h"
//Extended headers
#include "ObjectMgr.h"
#include "World.h"
//Flightmaster grid preloading
#include "MapManager.h"
//Creature-specific headers
#include "Creature.h"
#include "CreatureAI.h"
#include "CreatureGroups.h"
//Player-specific
#include "Player.h"
#include "MoveSplineInit.h"
#include "MoveSpline.h"
#include "Transport.h"

void WaypointMovementGenerator<Creature>::LoadPath(Creature* creature)
{
    if (!path_id)
        path_id = creature->GetWaypointPath();

    i_path = sWaypointMgr->GetPath(path_id);

    if (!i_path)
    {
        // No movement found for entry
        //        TC_LOG_ERROR("sql.sql", "WaypointMovementGenerator::LoadPath: creature %s (Entry: %u GUID: %u) doesn't have waypoint path id: %u", creature->GetName().c_str(), creature->GetEntry(), creature->GetGUIDLow(), path_id);
        return;
    }

    StartMoveNow(creature);
}

void WaypointMovementGenerator<Creature>::DoInitialize(Creature* creature)
{
    LoadPath(creature);
    creature->AddUnitState(UNIT_STATE_ROAMING|UNIT_STATE_ROAMING_MOVE);
}

void WaypointMovementGenerator<Creature>::DoFinalize(Creature* creature)
{
    creature->ClearUnitState(UNIT_STATE_ROAMING|UNIT_STATE_ROAMING_MOVE);
    creature->SetWalk(false);
}

void WaypointMovementGenerator<Creature>::DoReset(Creature* creature)
{
    creature->AddUnitState(UNIT_STATE_ROAMING|UNIT_STATE_ROAMING_MOVE);
    StartMoveNow(creature);
}

void WaypointMovementGenerator<Creature>::OnArrived(Creature* creature)
{
    if (!i_path || i_path->empty())
        return;
    if (m_isArrivalDone)
        return;

    creature->ClearUnitState(UNIT_STATE_ROAMING_MOVE);
    m_isArrivalDone = true;

    if (i_path->at(i_currentNode)->event_id && urand(0, 99) < i_path->at(i_currentNode)->event_chance)
    {
        TC_LOG_DEBUG("maps.script", "Creature movement start script %u at point %u for " UI64FMTD ".", i_path->at(i_currentNode)->event_id, i_currentNode, creature->GetGUID());
        creature->GetMap()->ScriptsStart(sWaypointScripts, i_path->at(i_currentNode)->event_id, creature, NULL);
    }

    // Inform script
    MovementInform(creature);
    creature->UpdateWaypointID(i_currentNode);
    Stop(i_path->at(i_currentNode)->delay);
}

bool WaypointMovementGenerator<Creature>::StartMove(Creature* creature)
{
    if (!i_path || i_path->empty())
        return false;

    if (auto first_waypoint = i_path->at(0))
    if (first_waypoint->smooth)
    {

        uint32 pathSize{ uint32(i_path->size()) };

        Movement::PointsArray allPoints;
        Movement::MoveSplineInit init(creature);

        for (size_t i = 0; i < i_path->size(); ++i)
            if (auto thisPoint = i_path->at(i))
            {
                G3D::Vector3 point;
                point.x = thisPoint->x;
                point.y = thisPoint->y;
                point.z = thisPoint->z;
                allPoints.push_back(point);
            }

        init.MovebyPath(allPoints);

        if (creature->CanFly())
        {
            init.SetFly();
            init.SetUncompressed();
        }

        if (first_waypoint->fly)
            init.SetVelocity(creature->GetSpeed(MOVE_FLIGHT));
        else if (first_waypoint->swim)
            init.SetVelocity(creature->GetSpeed(MOVE_SWIM));
        else if (first_waypoint->run)
            init.SetVelocity(creature->GetSpeed(MOVE_RUN));
        else init.SetVelocity(creature->GetSpeed(MOVE_WALK));


        init.SetSmooth();
        init.SetCyclic();

        /*
            The following disabled block of code is an attempt at formation smooth waypoints.
            Upon testing, this block of code would freeze the server in a way that would not produce a call stack in debug mode. 
            With debug mode enabled, The call stack can be triggered by going debug -> break all

            Through several attempts, the server would be continuously freezing at different points along the process of the function:
            member->UpdateGroundPositionZ(dx, dy, dz);

            It appears that not all of the NPCs are yet fully loaded, and therefore the core stops in its' tracks atteempting to make something that doesn't exist yet move with smooth pathing.
        */


        /*
        if (auto f = creature->GetFormation())
            if (f->getLeader() == creature)
            {

                bool inverseAngle{ first_waypoint->inverse_formation_angle };
                bool waypoint_flying{ first_waypoint->fly };
                auto m_members{ f->GetMembers() };

                if (m_members->size())
                    for (auto itr = m_members->begin(); itr != m_members->end(); ++itr)
                    if (auto member = itr->first)
                        if (member->IsInWorld())
                    if (auto form_info = itr->second)
                    {
                        if (member == f->getLeader() || !member->isAlive() || member->getVictim())
                            continue;

                        Movement::PointsArray my_path{ allPoints };
                        float angle = itr->second->follow_angle;
                        float pathangle = f->getLeader()->GetOrientation();

                        if (inverseAngle)
                        {
                            //angle = fmod(angle + M_PI, 2.0f * static_cast<float>(M_PI));
                            angle = fmod(form_info->follow_angle - pathangle, 2.0f * static_cast<float>(M_PI));
                        }

                        float dist = form_info->follow_dist;

                        if (my_path.size())
                        for (auto point = my_path.begin(); point != my_path.end(); ++itr)
                        {
                            float dx = point->x + std::cos(angle + pathangle) * dist;
                            float dy = point->y + std::sin(angle + pathangle) * dist;


                            Trinity::NormalizeMapCoord(dx);
                            Trinity::NormalizeMapCoord(dy);


                            // We add extra Z so if the leader is moving downward a slope the followers won't fall below
                            float dz = point->z + 10.f;
                            member->UpdateGroundPositionZ(dx, dy, dz);//Ground Z has Been established


                            if (waypoint_flying)                                            //waypoint calls for flight
                                if (auto templ = member->GetCreatureTemplate())
                                if (templ->InhabitType & INHABIT_AIR)   //If our npc can fly
                                    if ((point->z + form_info->follow_z_offset) > dz)                    //If the offset z is above the ground we've detected
                                    {
                                        dz = point->z + form_info->follow_z_offset;                      // We will use the z-offset z
                                    }

                            point->x = dx;
                            point->y = dy;
                            point->z = dz;
                        }

                        Movement::MoveSplineInit init2(member);

                        init2.MovebyPath(my_path);

                        if (member->CanFly())
                        {
                            init2.SetFly();
                            init2.SetUncompressed();
                        }

                        if (first_waypoint->fly)
                            init2.SetVelocity(member->GetSpeed(MOVE_FLIGHT));
                        else if (first_waypoint->swim)
                            init2.SetVelocity(member->GetSpeed(MOVE_SWIM));
                        else if (first_waypoint->run)
                            init2.SetVelocity(member->GetSpeed(MOVE_RUN));
                        else init2.SetVelocity(member->GetSpeed(MOVE_WALK));


                        init2.SetSmooth();
                        init2.SetCyclic();
                        init2.Launch();
                    }
            }
            */
        init.Launch();
        return true;
    }
    else
    {

        if (Stopped())
            return true;

        bool transportPath = creature->GetTransport() != NULL;

        if (m_isArrivalDone)
        {
            if ((i_currentNode == i_path->size() - 1) && !repeating) // If that's our last waypoint
            {
                float x = i_path->at(i_currentNode)->x;
                float y = i_path->at(i_currentNode)->y;
                float z = i_path->at(i_currentNode)->z;
                float o = creature->GetOrientation();

                if (!transportPath)
                    creature->SetHomePosition(x, y, z, o);
                else
                {
                    if (Transport* trans = creature->GetTransport())
                    {
                        o -= trans->GetOrientation();
                        creature->SetTransportHomePosition(x, y, z, o);
                        trans->CalculatePassengerPosition(x, y, z, o);
                        creature->SetHomePosition(x, y, z, o);
                    }
                    else
                        transportPath = false;
                    // else if (vehicle) - this should never happen, vehicle offsets are const
                }

                creature->GetMotionMaster()->Initialize();
                return false;
            }

            i_currentNode = (i_currentNode + 1) % i_path->size();
        }

        WaypointData const* node = i_path->at(i_currentNode);

        m_isArrivalDone = false;

        creature->AddUnitState(UNIT_STATE_ROAMING_MOVE);

        Movement::Location formationDest(node->x, node->y, node->z, 0.0f);
        Movement::MoveSplineInit init(creature);

        //! If creature is on transport, we assume waypoints set in DB are already transport offsets
        if (transportPath)
        {
            init.DisableTransportPathTransformations();
            if (TransportBase* trans = creature->GetDirectTransport())
                trans->CalculatePassengerPosition(formationDest.x, formationDest.y, formationDest.z, formationDest.orientation);
        }

        //! Do not use formationDest here, MoveTo requires transport offsets due to DisableTransportPathTransformations() call
        //! but formationDest contains global coordinates
        init.MoveTo(node->x, node->y, node->z);


        //! Accepts angles such as 0.00001 and -0.00001, 0 must be ignored, default value in waypoint table
        if (node->orientation && node->delay)
            init.SetFacing(node->orientation);


        init.SetWalk(!node->run && !node->fly);

        init.Launch();

        //Call for creature group update
        if (creature->GetFormation() && creature->GetFormation()->getLeader() == creature)
            creature->GetFormation()->LeaderMoveTo(node->x, node->y, node->z, node->inverse_formation_angle, node->fly);

        return true;

    }
    else return false;
}

bool WaypointMovementGenerator<Creature>::DoUpdate(Creature* creature, uint32 diff)
{
    // Waypoint movement can be switched on/off
    // This is quite handy for escort quests and other stuff
    if (creature->HasUnitState(UNIT_STATE_NOT_MOVE))
    {
        creature->ClearUnitState(UNIT_STATE_ROAMING_MOVE);
        return true;
    }
    // prevent a crash at empty waypoint path.
    if (!i_path || i_path->empty())
        return false;

    if (Stopped())
    {
        if (CanMove(diff))
            return StartMove(creature);
    }
    else
    {
        if (!creature->GetTransGUID())
            creature->SetHomePosition(creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ(), creature->GetOrientation());
        if (creature->IsStopped())
            Stop(STOP_TIME_FOR_PLAYER);
        else if (creature->movespline->Finalized())
        {
            OnArrived(creature);
            return StartMove(creature);
        }
    }
     return true;
 }

void WaypointMovementGenerator<Creature>::MovementInform(Creature* creature)
{
    if (creature->AI())
        creature->AI()->MovementInform(WAYPOINT_MOTION_TYPE, i_currentNode);
}

bool WaypointMovementGenerator<Creature>::GetResetPos(Creature*, float& x, float& y, float& z)
{
    // prevent a crash at empty waypoint path.
    if (!i_path || i_path->empty())
        return false;

    const WaypointData* node = i_path->at(i_currentNode);
    x = node->x; y = node->y; z = node->z;
    return true;
}


//----------------------------------------------------//

uint32 FlightPathMovementGenerator::GetPathAtMapEnd() const
{
    if (i_currentNode >= i_path->size())
        return i_path->size();

    uint32 curMapId = (*i_path)[i_currentNode].mapid;
    for (uint32 i = i_currentNode; i < i_path->size(); ++i)
    {
        if ((*i_path)[i].mapid != curMapId)
            return i;
    }

    return i_path->size();
}

void FlightPathMovementGenerator::DoInitialize(Player* player)
{
    Reset(player);
    InitEndGridInfo();
}

void FlightPathMovementGenerator::DoFinalize(Player* player)
{
    // remove flag to prevent send object build movement packets for flight state and crash (movement generator already not at top of stack)
    player->ClearUnitState(UNIT_STATE_IN_FLIGHT);

    player->Dismount();
    player->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_TAXI_FLIGHT);

    if (player->m_taxi.empty())
    {
        player->getHostileRefManager().setOnlineOfflineState(true);
        // update z position to ground and orientation for landing point
        // this prevent cheating with landing  point at lags
        // when client side flight end early in comparison server side
        player->StopMoving();
    }

    player->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAGS_TAXI_BENCHMARK);
}

#define PLAYER_FLIGHT_SPEED 32.0f

void FlightPathMovementGenerator::DoReset(Player* player)
{
    player->getHostileRefManager().setOnlineOfflineState(false);
    player->AddUnitState(UNIT_STATE_IN_FLIGHT);
    player->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_TAXI_FLIGHT);

    Movement::MoveSplineInit init(player);
    uint32 end = GetPathAtMapEnd();
    for (uint32 i = GetCurrentNode(); i != end; ++i)
    {
        G3D::Vector3 vertice((*i_path)[i].x, (*i_path)[i].y, (*i_path)[i].z);
        init.Path().push_back(vertice);
    }
    init.SetFirstPointId(GetCurrentNode());
    init.SetFly();
    init.SetSmooth();
    init.SetWalk(true);
    init.SetVelocity(PLAYER_FLIGHT_SPEED);
    init.Launch();
}

bool FlightPathMovementGenerator::DoUpdate(Player* player, uint32 /*diff*/)
{
    if (!player)
        return false;

    uint32 pointId = (uint32)player->movespline->currentPathIdx();
    if (pointId > i_currentNode)
    {
        bool departureEvent = true;
        do
        {
            if (i_currentNode > (i_path->size()-1))
                break;
            DoEventIfAny(player, (*i_path)[i_currentNode], departureEvent);
            if (pointId == i_currentNode)
                break;
            if (i_currentNode == _preloadTargetNode)
                PreloadEndGrid();
            i_currentNode += (uint32)departureEvent;
            departureEvent = !departureEvent;
        }
        while (true);
    }

    return i_currentNode < (i_path->size()-1);
}

void FlightPathMovementGenerator::SetCurrentNodeAfterTeleport()
{
    if (i_path->empty())
        return;

    uint32 map0 = (*i_path)[0].mapid;
    for (size_t i = 1; i < i_path->size(); ++i)
    {
        if ((*i_path)[i].mapid != map0)
        {
            i_currentNode = i;
            return;
        }
    }
}

void FlightPathMovementGenerator::DoEventIfAny(Player* player, TaxiPathNodeEntry const& node, bool departure)
{
    if (uint32 eventid = departure ? node.departureEventID : node.arrivalEventID)
    {
        TC_LOG_DEBUG("maps.script", "Taxi %s event %u of node %u of path %u for player %s", departure ? "departure" : "arrival", eventid, node.index, node.path, player->GetName().c_str());
        player->GetMap()->ScriptsStart(sEventScripts, eventid, player, player);
    }
}

bool FlightPathMovementGenerator::GetResetPos(Player*, float& x, float& y, float& z)
{
    const TaxiPathNodeEntry& node = (*i_path)[i_currentNode];
    x = node.x; y = node.y; z = node.z;
    return true;
}

void FlightPathMovementGenerator::InitEndGridInfo()
{
    /*! Storage to preload flightmaster grid at end of flight. For multi-stop flights, this will
       be reinitialized for each flightmaster at the end of each spline (or stop) in the flight. */
    uint32 nodeCount = (*i_path).size();        //! Number of nodes in path.
    _endMapId = (*i_path)[nodeCount - 1].mapid; //! MapId of last node
    _preloadTargetNode = nodeCount - 3;
    _endGridX = (*i_path)[nodeCount - 1].x;
    _endGridY = (*i_path)[nodeCount - 1].y;
}

void FlightPathMovementGenerator::PreloadEndGrid()
{
    // used to preload the final grid where the flightmaster is
    Map* endMap = sMapMgr->FindBaseNonInstanceMap(_endMapId);

    // Load the grid
    if (endMap)
    {
        TC_LOG_INFO("misc", "Preloading rid (%f, %f) for map %u at node index %u/%u", _endGridX, _endGridY, _endMapId, _preloadTargetNode, (uint32)(i_path->size()-1));
        endMap->LoadGrid(_endGridX, _endGridY);
    }
    else
        TC_LOG_INFO("misc", "Unable to determine map to preload flightmaster grid");
}
