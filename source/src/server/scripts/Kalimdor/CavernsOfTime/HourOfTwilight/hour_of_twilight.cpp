#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "hour_of_twilight.h"

enum Spells
{
    SPELL_TELEPORT_ARCURION_DEAD = 108928,
    SPELL_TELEPORT_ASIRA_DEAD = 108929,
    SPELL_TELEPORT_BENEDICTUS = 108930,

};

enum Npcs
{
    NPC_THRALL_FIRE_TOTEM = 55474,
};

/// 210026
class go_hot_time_transit_device : public GameObjectScript
{
public:
    go_hot_time_transit_device() : GameObjectScript("go_hot_time_transit_device") {}

    bool OnGossipHello(Player* player, GameObject* go)
    {
        if (player->isInCombat())
            return true;
        if (InstanceScript *instance = go->GetInstanceScript())
        {
            if (instance->GetData(DATA_EVENT_2) == DONE)
                player->CastSpell(player, SPELL_TELEPORT_BENEDICTUS, true);
            else if (instance->IsDone(DATA_ASIRA))
                player->CastSpell(player, SPELL_TELEPORT_ASIRA_DEAD, true);
            else if (instance->IsDone(DATA_ARCURION))
                player->CastSpell(player, SPELL_TELEPORT_ARCURION_DEAD, true);
        }
        return true;
    }

};


void AddSC_hour_of_twilight()
{
    new go_hot_time_transit_device();
}
