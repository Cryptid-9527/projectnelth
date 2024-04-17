/*
* ToDo:
* Find the correct Sound-Yells
*/

#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"
#include "shadowfang_keep.h"

enum Spells
{
	SPELL_CONJURE_FROST_MIXTURE = 93505,
	SPELL_CONJURE_POISONOUS_MIXTURE = 93697,
	SPELL_CONJURE_MYSTERY_TOXIN = 93563,
	SPELL_TOXIC_COAGULANT_GREEN = 93572,
	SPELL_TOXIC_COAGULANT_RED = 93573,
	SPELL_ICE_SHARDS = 93527,
	SPELL_TOXIC_COAGULANT_TRIGGERED = 93617
};

enum Events
{
	EVENT_CONJURE_POISONOUS_MIXTURE = 1,
	EVENT_CONJURE_FROST_MIXTURE,
	EVENT_CONJURE_MYSTERY_TOXIN,
	EVENT_ICE_SHARDS,
};

enum Texts
{
	SAY_AGGRO = 0,
	SAY_DEATH = 1,
	SAY_KILLER_1 = 2,
	SAY_KILLER_2 = 3,
};

class boss_lord_walden : public CreatureScript
{
public:
	boss_lord_walden() : CreatureScript("boss_lord_walden") { }

	struct boss_lord_waldenAI : public BossAI
	{
		boss_lord_waldenAI(Creature* creature) : BossAI(creature, DATA_LORD_WALDEN_EVENT) { }

		void Reset()
		{
			_Reset();
			enrage = false;
		}

		void EnterCombat(Unit* /*who*/)
		{
			enrage = false;
			_EnterCombat();
			Talk(SAY_AGGRO);
			events.ScheduleEvent(EVENT_CONJURE_POISONOUS_MIXTURE, 5000);
			events.ScheduleEvent(EVENT_CONJURE_FROST_MIXTURE, urand(10000, 20000));
			events.ScheduleEvent(EVENT_ICE_SHARDS, urand(16000, 22000));
			if (IsHeroic())
			events.ScheduleEvent(EVENT_CONJURE_MYSTERY_TOXIN, 25000);
		}

		void JustDied(Unit* /*killer*/)
		{
			Talk(SAY_DEATH);
			_JustDied();
		}

		void KilledUnit(Unit* victim)
		{
			Talk(RAND(SAY_KILLER_1, SAY_KILLER_2));
		}

		void JustSummoned(Creature* summon)
		{
			BossAI::JustSummoned(summon);

			if (summon->GetEntry() == NPC_TOXIN_TRIGGER)
			{
				float x, y, z;
				me->GetPosition(x, y, z);
				summon->SetCanFly(true);
				summon->SetDisableGravity(true);
				summon->NearTeleportTo(x, y, z + 5.0f, 0.0f);

				if (HealthAbovePct(35))
				{
					summon->CastSpell(summon, SPELL_TOXIC_COAGULANT_GREEN, false);
				}
				else
				{
					summon->CastSpell(summon, SPELL_TOXIC_COAGULANT_RED, false);
				}
			}
		}

		void DamageTaken(Unit* /*dealer*/, uint32& damage)
		{
			if (!enrage)
			if (IsHeroic())
				if (me->HealthBelowPctDamaged(35, damage))
				{
					events.CancelEvent(EVENT_CONJURE_MYSTERY_TOXIN);
					events.ScheduleEvent(EVENT_CONJURE_MYSTERY_TOXIN, 1);
					enrage = true;
				}
		}

		void OnSpellCastFinished(SpellInfo const* spell, SpellFinishReason reason)
		{
			switch (spell->Id)
			{
			case SPELL_ICE_SHARDS:
			case 93703:
			case SPELL_CONJURE_MYSTERY_TOXIN:
				if (reason != SPELL_FINISHED_SUCCESSFUL_CAST)
				{
					me->SetReactState(REACT_AGGRESSIVE);
					UpdateVictim();
				}
				break;
			case SPELL_CONJURE_FROST_MIXTURE:
			case 93702://frost hc
			case SPELL_CONJURE_POISONOUS_MIXTURE:
			case 93704://poison hc
				me->SetReactState(REACT_AGGRESSIVE);
				UpdateVictim();
				break;
			}
		}
		void UpdateAI(uint32 const diff)
		{
			events.Update(diff);
			while (uint32 eventId = events.ExecuteEvent())
				switch (eventId)
				{
				case EVENT_ICE_SHARDS:
					if (me->HasUnitState(UNIT_STATE_CASTING)) events.ScheduleEvent(eventId, 250);
					else
					{
						me->SetReactState(REACT_PASSIVE);
						me->AttackStop();
						me->StopMoving();
						DoCast(SPELL_ICE_SHARDS);
						events.ScheduleEvent(EVENT_ICE_SHARDS, urand(16000, 22000));
					}
					break;
				case EVENT_CONJURE_MYSTERY_TOXIN:
					if (me->HasUnitState(UNIT_STATE_CASTING)) events.ScheduleEvent(eventId, 250);
					else
					{
						me->SetReactState(REACT_PASSIVE);
						me->AttackStop();
						me->StopMoving();
						DoCast(SPELL_CONJURE_MYSTERY_TOXIN);
						events.ScheduleEvent(EVENT_CONJURE_MYSTERY_TOXIN, 45000);
						//events.ScheduleEvent(EVENT_ICE_SHARDS, 10000);
					}
					break;
				case EVENT_CONJURE_FROST_MIXTURE:
					if (me->HasUnitState(UNIT_STATE_CASTING)) events.ScheduleEvent(eventId, 250);
					else
					{
						me->SetReactState(REACT_PASSIVE);
						me->AttackStop();
						me->StopMoving();
						DoCastRandom(SPELL_CONJURE_FROST_MIXTURE, 50.f);
						events.ScheduleEvent(EVENT_CONJURE_FROST_MIXTURE, urand(10000, 20000));
					}
					break;
				case EVENT_CONJURE_POISONOUS_MIXTURE:
					if (me->HasUnitState(UNIT_STATE_CASTING)) events.ScheduleEvent(eventId, 250);
					else
					{
						me->SetReactState(REACT_PASSIVE);
						me->AttackStop();
						me->StopMoving();
						DoCastRandom(SPELL_CONJURE_POISONOUS_MIXTURE, 50.0f);
						events.ScheduleEvent(EVENT_CONJURE_POISONOUS_MIXTURE, urand(15000, 30000));
					}
					break;
				}

			if (!me->HasUnitState(UNIT_STATE_CASTING))
				if (UpdateVictim())
					DoMeleeAttackIfReady();
		}
		private:
			bool enrage { false };

	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new boss_lord_waldenAI(creature);
	}
};

class spell_toxic_coagulant_green : public SpellScriptLoader
{
public:
	spell_toxic_coagulant_green() : SpellScriptLoader("spell_toxic_coagulant_green") { }

	class spell_toxic_coagulant_green_AuraScript : public AuraScript
	{
		PrepareAuraScript(spell_toxic_coagulant_green_AuraScript)

			void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
		{
			Unit* target = GetTarget();
			if (!target)
				return;

			if (target->isMoving())
				target->RemoveAuraFromStack(SPELL_TOXIC_COAGULANT_TRIGGERED);

			if (Aura* aura = GetAura())
				if (aura->GetStackAmount() == 3)
				{
					target->CastSpell(target, 93660);
					aura->Remove(AURA_REMOVE_BY_DEFAULT);
				}
		}

		void Register()
		{
			OnEffectPeriodic += AuraEffectPeriodicFn(spell_toxic_coagulant_green_AuraScript::HandleEffectPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
		}
	};

	AuraScript* GetAuraScript() const
	{
		return new spell_toxic_coagulant_green_AuraScript();
	}
};

class NotMovingFilter
{
public:
	bool operator()(WorldObject* target) const
	{
		return target->ToUnit() && target->ToUnit()->isMoving() == false;
	}
};

void AddSC_boss_lord_walden()
{
	new boss_lord_walden();
	new spell_toxic_coagulant_green();
}