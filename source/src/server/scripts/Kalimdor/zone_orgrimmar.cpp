/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
 SDName: Orgrimmar
 SD%Complete: 0
 SDComment: Quest support:
 SDCategory: Orgrimmar
 EndScriptData */

 /* ContentData
 EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Player.h"
#include "Unit.h"

Position const FallPos[2] =
{
    {2068.401367f, -4607.590332f, 59.7814f, 6.201787f}, // fall pos ground
    {2068.401367f, -4607.590332f, 59.7814f, 6.201787f} // fall pos fly
};

class npc_stabled_hunter_pet : public CreatureScript
{
    /*

    */

public:
    npc_stabled_hunter_pet() : CreatureScript("npc_stabled_hunter_pet") { }

    struct npc_stabled_hunter_petAI : ScriptedAI
    {
        npc_stabled_hunter_petAI(Creature* creature) : ScriptedAI(creature) { hasTarget = false; }

        void InitializeAI()
        {
            me->AddUnitMovementFlag(MOVEMENTFLAG_DISABLE_GRAVITY);
            me->GetMotionMaster()->MoveAroundPoint(FallPos[1], 4.0f, 0, 135.322f);  //Position, radius, clockwise true/false, speed
        }
        void Reset()
        {
            me->AddUnitMovementFlag(MOVEMENTFLAG_DISABLE_GRAVITY);
            me->GetMotionMaster()->MoveAroundPoint(FallPos[1], 4.0f, 0, 135.322f);  //Position, radius, clockwise true/false, speed
        }

        void UpdateAI(const uint32 diff)
        {
            me->AddUnitMovementFlag(MOVEMENTFLAG_DISABLE_GRAVITY);
        }

    private:
        float followdist;
        bool hasTarget;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_stabled_hunter_petAI(creature);
    }

};
void AddSC_orgrimmar()
{
    new npc_stabled_hunter_pet();
}
