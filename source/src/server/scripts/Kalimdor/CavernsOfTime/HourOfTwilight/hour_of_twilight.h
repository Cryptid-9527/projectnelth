#ifndef HOUR_OF_TWILIGHT_H_
#define HOUR_OF_TWILIGHT_H_

#define HourOfTwilightScriptName "instance_hour_of_twilight"
#define DataHeader "HT"

uint32 const EncounterCount = 14;

enum Datas
{
    // Encounters
    DATA_ARCURION        = 1,
    DATA_ASIRA,
    DATA_ARCHBISHOP,

    // Additional Objects
    DATA_THRALL_EVENT_0,
    DATA_THRALL_EVENT_1,
    DATA_THRALL_EVENT_2,
    DATA_THRALL_EVENT_3,
    DATA_THRALL,

    DATA_EVENT_0,
    DATA_EVENT_1,
    DATA_EVENT_2,
    DATA_EVENT_3,

    DATA_HORDE_PORTAL,
    DATA_ALLIANCE_PORTAL
};

enum CreatureIds
{
    //Dungeon Bosses
    NPC_ARCURION            = 54590,
    NPC_ASIRA               = 54968,
    NPC_ARCHBISHOP          = 54938,
    NPC_FROZEN_SERVITOR     = 54555,
    NPC_THRALL              = 54971,
    NPC_THRALL_EVENT_0      = 54548,
    NPC_THRALL_EVENT_1      = 55779,
    NPC_THRALL_EVENT_2      = 54972,
    NPC_THRALL_EVENT_3      = 54634,
    NPC_TWILIGHT_SPARK      = 55466,

    NPC_EYE_STALKER         = 54522,
    NPC_EYE_STALKER_2       = 54599,
};

enum GameObjectIds
{
    GO_ICEWALL              = 210048,
    GO_HORDE_PORTAL         = 209081,
    GO_ALLIANCE_PORTAL      = 209080,
};

enum Actions
{
    ACTION_START_COMBAT = 1,
    ACTION_BOSS_PHASE_3,
    ACTION_STOP_COMBAT,
    ACTION_KILL_CREATURE,
};

enum Misc
{
    SPELL_DELETE_ICY_TOMB   = 102722,
};

template<class AI>
inline AI* GetHourOfTwilightAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, HourOfTwilightScriptName);
}

#endif // HOUR_OF_TWILIGHT_H_
