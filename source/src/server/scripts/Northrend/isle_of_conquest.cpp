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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "PassiveAI.h"
#include "BattlegroundIC.h"
#include "Player.h"
#include "GameObjectAI.h"

// TO-DO: This should be done with SmartAI, but yet it does not correctly support vehicles's AIs.
//        Even adding ReactState Passive we still have issues using SmartAI.

enum events
{
    EVENT_ENERGY = 1,
};

class npc_four_car_garage : public CreatureScript
{
    public:
        npc_four_car_garage() : CreatureScript("npc_four_car_garage") {}

        struct npc_four_car_garageAI : public NullCreatureAI
        {
            npc_four_car_garageAI(Creature* creature) : NullCreatureAI(creature) { }

            void PassengerBoarded(Unit* who, int8 /*seatId*/, bool apply)
            {
                if (apply)
                {
                    uint32 spellId = 0;

                    switch (me->GetEntry())
                    {
                        case NPC_DEMOLISHER:
                            spellId = SPELL_DRIVING_CREDIT_DEMOLISHER;
                            break;
                        case NPC_GLAIVE_THROWER_A:
                        case NPC_GLAIVE_THROWER_H:
                            spellId = SPELL_DRIVING_CREDIT_GLAIVE;
                            break;
                        case NPC_SIEGE_ENGINE_H:
                        case NPC_SIEGE_ENGINE_A:
                            spellId = SPELL_DRIVING_CREDIT_SIEGE;
                            break;
                        case NPC_CATAPULT:
                            spellId = SPELL_DRIVING_CREDIT_CATAPULT;
                            break;
                        default:
                            return;
                    }

                    me->CastSpell(who, spellId, true);
                }
            }

            void InitializeAI() override
            {
                me->SetReactState(REACT_PASSIVE);

                //TC_LOG_ERROR("entities.unit", "Initialize AI");     
                vehicle_events.ScheduleEvent(EVENT_ENERGY, 5000);
                if (me->ToUnit()->getPowerType() == POWER_ENERGY)
                {
                    me->ToUnit()->SetMaxPower(POWER_ENERGY, 100);
                    //TC_LOG_ERROR("entities.unit", "events: 0");     
                }
                else
                {
                    //TC_LOG_ERROR("entities.unit", "events: 0 failed");
                }
            }
            void UpdateAI(uint32 const diff)
            {
                vehicle_events.Update(diff);
                while (uint32 eventId = vehicle_events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                    case EVENT_ENERGY:
                        //TC_LOG_ERROR("entities.unit", "events: 1");                     
                        if (me->ToUnit()->getPowerType() == POWER_ENERGY)
                        {
                            if (me->ToUnit()->GetMaxPower(POWER_ENERGY) < 100)
                            {
                                me->ToUnit()->SetMaxPower(POWER_ENERGY, 100);
                                //TC_LOG_ERROR("entities.unit", "power set to 100");      
                            }
                            me->ToUnit()->SetPower(POWER_ENERGY, std::min(me->GetPower(POWER_ENERGY) + int32(20), me->ToUnit()->GetMaxPower(POWER_ENERGY)));
                            //TC_LOG_ERROR("entities.unit", "events: 2: %u of %u", me->GetPower(POWER_ENERGY), me->ToUnit()->GetMaxPower(POWER_ENERGY));   
                        }
                        vehicle_events.ScheduleEvent(EVENT_ENERGY, 5000);
                        break;
                    default:
                        break;
                    }
                }
            }
            EventMap vehicle_events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_four_car_garageAI(creature);
        }
};

class npc_siege_cannon : public CreatureScript
{
public:
    npc_siege_cannon() : CreatureScript("npc_siege_cannon") {}

    struct npc_siege_cannonAI : public NullCreatureAI
    {
        npc_siege_cannonAI(Creature* creature) : NullCreatureAI(creature) { }

        void PassengerBoarded(Unit* who, int8 /*seatId*/, bool apply)
        {
            if (apply)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG2_PREVENT_SPELL_CLICK);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_HIDE_NAMEPLATE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED);
            }
            else
            {
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG2_PREVENT_SPELL_CLICK);
                me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_HIDE_NAMEPLATE);
            }
        }
        void InitializeAI() override
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_ATTACKABLE_1/* | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC*/);
            me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_HIDE_NAMEPLATE);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG2_PREVENT_SPELL_CLICK);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED);
            turret_events.ScheduleEvent(EVENT_ENERGY, (me->GetZoneId() == 4710 ? 185000 : 5000));
        }
        void UpdateAI(uint32 const diff)
        {

            //me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            //me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
            //events
            turret_events.Update(diff);
            while (uint32 eventId = turret_events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_ENERGY:
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG2_PREVENT_SPELL_CLICK);
                    me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_HIDE_NAMEPLATE);
                    break;
                default:
                    break;
                }
            }
        }
        EventMap turret_events;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_siege_cannonAI(creature);
    }
};

class spell_turret_magma : public SpellScriptLoader
{
public:
    spell_turret_magma() : SpellScriptLoader("spell_turret_magma") { }

    class spell_turret_magma_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_turret_magma_SpellScript);

        /*
            Spell 68832 is the proper opcode for siege engine magma cannon fire.
            This spell fails because it produces a client opcode currently unhandled in the devcore.
            This will need to be addressed before this actual spell can be implemented.

            CMSG_PRODUCED_BY_THIS_SPELL                        = 0x41D21B86,

            In the meantime, I will swap out the spell 68322 for an identical spell (53420) to allow the cannons to operate similarly.
        */
        SpellCastResult RefuseCast()
        {
            TC_LOG_ERROR("bg.battleground", "Cast failed");
            return SPELL_FAILED_TRY_AGAIN;
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(spell_turret_magma_SpellScript::RefuseCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_turret_magma_SpellScript();
    }
};

class go_linear_transport : public GameObjectScript
{
public:
    go_linear_transport() : GameObjectScript("go_linear_transport") { }

    struct go_linear_transportAI : public GameObjectAI
    {

        go_linear_transportAI(GameObject* go) : GameObjectAI(go)
        {
        }

        void RelocateToProgress(uint32 progress)
        {
        }

        void UpdateAI(uint32 diff)
        {
            RelocateToProgress(diff);
        }

    };

    GameObjectAI* GetAI(GameObject* go) const
    {
        return new go_linear_transportAI(go);
    }
};


void AddSC_isle_of_conquest()
{
    new npc_siege_cannon();
    new npc_four_car_garage();
    new spell_turret_magma();
    new go_linear_transport();
}
