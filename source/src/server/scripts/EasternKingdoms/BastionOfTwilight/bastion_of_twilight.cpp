#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"
#include "bastion_of_twilight.h"

enum Spells
{
    SPELL_TWILIGHT_RUPTURE = 93378
};

class spell_blade_barrier : public SpellScriptLoader
{
public:
    spell_blade_barrier() : SpellScriptLoader("spell_blade_barrier") { }

    class spell_blade_barrier_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_blade_barrier_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
            // Set absorbtion amount to unlimited
            amount = -1;
        }

        void Absorb(AuraEffect* /* aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
        {
            uint32 damage = dmgInfo.GetDamage();
            if (damage <= 10000)
                absorbAmount = damage - 1;
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_blade_barrier_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            OnEffectAbsorb += AuraEffectAbsorbFn(spell_blade_barrier_AuraScript::Absorb, EFFECT_0);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_blade_barrier_AuraScript();
    }
};

class npc_BOT_flamestrike_trigger : public CreatureScript
{
public:
    npc_BOT_flamestrike_trigger() : CreatureScript("npc_BOT_flamestrike_trigger") { }

    struct npc_BOT_flamestrike_triggerAI : public ScriptedAI
    {
        npc_BOT_flamestrike_triggerAI(Creature* creature) : ScriptedAI(creature)
        {
            me->SetReactState(REACT_PASSIVE);
            me->AddUnitState(UNIT_STATE_ROOT);
        }


        void DamageTaken(Unit* /*done_by*/, uint32& damage) { damage = 0; }
    };


    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_BOT_flamestrike_triggerAI(creature);
    }
};

class spell_twilight_rupture : public SpellScriptLoader
{
public:
    spell_twilight_rupture() : SpellScriptLoader("spell_twilight_rupture") { }

    class spell_twilight_rupture_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_twilight_rupture_SpellScript);

        void HandleOnEffectHitTarget(SpellEffIndex /*effIndex*/)
        {
            if (auto target = GetHitUnit())
                GetCaster()->CastSpell(target, SPELL_TWILIGHT_RUPTURE, true);
        }

        void Register()
        {
            //AfterCast += SpellCastFn(spell_twilight_rupture_SpellScript::HandleAfterCast);
            OnEffectHitTarget += SpellEffectFn(spell_twilight_rupture_SpellScript::HandleOnEffectHitTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_twilight_rupture_SpellScript();
    }
};

void AddSC_bastion_of_twilight()
{
    new spell_blade_barrier();
    new npc_BOT_flamestrike_trigger();
    new spell_twilight_rupture();
}
