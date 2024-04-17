#ifndef DEF_TIME_END_H
#define DEF_TIME_END_H

#define DataHeader "TE"

enum Datas
{
    DATA_FRAGMENTS,
    DATA_TYRANDE_GAUNTLET,
    DATA_JAINA_PICKED_STATE,
    DATA_RANDOM_BOSS_SELECTION = 5003,
};

enum Datas64
{
    DATA_ECHO_OF_SYLVANAS_GUID,
    DATA_ECHO_OF_BAINE_GUID,
    DATA_MUROZOND_GUID,
    DATA_ECHO_OF_TYRANDE_GUID,
    DATA_ECHO_OF_JAINA_GUID,
    DATA_HOURGLASS_OF_TIME_GUID,
};

enum Bosses
{
    BOSS_ECHO_OF_SYLVANAS,
    BOSS_ECHO_OF_BAINE,
    BOSS_MUROZOND,
    BOSS_ECHO_OF_TYRANDE,
    BOSS_ECHO_OF_JAINA,
};

enum GameObjectIds
{
    GO_MUROZOND_CACHE = 209547,
    GO_ET_TELEPORT    = 209438,
    GO_BAINE_WALL     = 209990,
    GO_HOURGLASS_OF_TIME = 209249,
};

enum CreatureIds
{
    NPC_ECHO_OF_JAINA    = 54445,
    NPC_ECHO_OF_BAINE    = 54431,
    NPC_ECHO_OF_SYLVANAS = 54123,
    NPC_ECHO_OF_TYRANDE  = 54544,

    NPC_MUROZOND         = 54432,
    NPC_BRITTLE_GHOUL    = 54952,
    NPC_JUMP_TRIGGER     = 54385,
    NPC_BLIGHTED_ARROW   = 54403,
    NPC_GHOUL            = 54197,
    NPC_RISEN_GHOUL      = 54191,

    NPC_VOLCANO          = 54556,

    /*Tyrande*/
    NPC_GEN_BUNNY                   = 45979, // not impl => SPELL_DARK_LUNAR_LIGHT (gauntlet part)
    NPC_GODDESS_EYES_A              = 54594,
    NPC_GODDESS_EYES_B              = 54597,
    NPC_ELUNE_MOONLANCE             = 54574,
    NPC_ELUNE_MOONLANCE_A           = 54580,
    NPC_ELUNE_MOONLANCE_B           = 54582,
    NPC_ELUNE_MOONLANCE_C           = 54581,
    NPC_POOL_OF_MOONLIGHT           = 54508,
    // talk Ein Strahl aus Mondlicht erscheint! Type: RaidBossEmote (41) 01:21:52.736
    // talk Das Mondlicht verschwindet in der Dunkelheit! Type: RaidBossEmote (41) 01:22:23.531
    NPC_TIMED_TWISTED_NIGHT_SABER   = 54688,
    NPC_TIME_TWISTED_SABER_2        = 54700,
    NPC_TIME_TWISTED_SABER_3        = 54699,
    NPC_TIME_TWISTED_SENTINEL       = 54512,
    NPC_TIME_TWISTED_HUNTRESS       = 54701,

    NPC_HOURGLASS_TRIGGER           = 54928,
    NPC_MORUZAND_SANDSTORM          = 54945,
    NPC_INFINITE_WARDEN             = 54923,
    NPC_INFINITE_SUPPRESSOR         = 54920,
};

enum SharedSpells
{
    SPELL_HOURGLASS_SUPPRESSION = 102534,
};

enum WorldStatesET
{
    WORLDSTATE_SHOW_FRAGMENTS      = 6046,
    WORLDSTATE_FRAGMENTS_COLLECTED = 6025,
};

enum IMiscs
{
    MAX_FRAGMENTS = 16,
};

enum InstanceTeleporter
{
    START_TELEPORT = 102564,
    DRAKE_SANCTUM_RUBIS = 102579,
    DRAKE_SANCTUM_EMERAUD = 104761,
    DRAKE_SANCTUM_BRONZE = 104764,
    DRAKE_SANCTUM_OBSIDIEN = 103868,
    DRAKE_SANCTUM_AZUR = 102126,
};

struct randBoss
{
    InstanceTeleporter boss1;
    std::string text1;
    Bosses eb1;

    InstanceTeleporter boss2;
    std::string text2;
    Bosses eb2;
};

randBoss const combi[12] =
{
    //Ruby first                                                                        //Second boss
    {DRAKE_SANCTUM_RUBIS,       "Ruby Dragonshrine",        BOSS_ECHO_OF_SYLVANAS,      DRAKE_SANCTUM_AZUR,        "Azure Dragonshrine",       BOSS_ECHO_OF_JAINA,          },
    {DRAKE_SANCTUM_RUBIS,       "Ruby Dragonshrine",        BOSS_ECHO_OF_SYLVANAS,      DRAKE_SANCTUM_EMERAUD,     "Emerald Dragonshrine",     BOSS_ECHO_OF_TYRANDE,        },
    {DRAKE_SANCTUM_RUBIS,       "Ruby Dragonshrine",        BOSS_ECHO_OF_SYLVANAS,      DRAKE_SANCTUM_OBSIDIEN,    "Obsidian Dragonshrine",    BOSS_ECHO_OF_BAINE,          },

    //Azure first
    {DRAKE_SANCTUM_AZUR,        "Azure Dragonshrine",       BOSS_ECHO_OF_JAINA,         DRAKE_SANCTUM_RUBIS,       "Ruby Dragonshrine",        BOSS_ECHO_OF_SYLVANAS,       },
    {DRAKE_SANCTUM_AZUR,        "Azure Dragonshrine",       BOSS_ECHO_OF_JAINA,         DRAKE_SANCTUM_EMERAUD,     "Emerald Dragonshrine",     BOSS_ECHO_OF_TYRANDE,        },
    {DRAKE_SANCTUM_AZUR,        "Azure Dragonshrine",       BOSS_ECHO_OF_JAINA,         DRAKE_SANCTUM_OBSIDIEN,    "Obsidian Dragonshrine",    BOSS_ECHO_OF_BAINE,          },

    //Emerald first
    {DRAKE_SANCTUM_EMERAUD,     "Emerald Dragonshrine",     BOSS_ECHO_OF_TYRANDE,       DRAKE_SANCTUM_RUBIS,       "Ruby Dragonshrine",        BOSS_ECHO_OF_SYLVANAS,       },
    {DRAKE_SANCTUM_EMERAUD,     "Emerald Dragonshrine",     BOSS_ECHO_OF_TYRANDE,       DRAKE_SANCTUM_AZUR,        "Azure Dragonshrine",       BOSS_ECHO_OF_JAINA,          },
    {DRAKE_SANCTUM_EMERAUD,     "Emerald Dragonshrine",     BOSS_ECHO_OF_TYRANDE,       DRAKE_SANCTUM_OBSIDIEN,    "Obsidian Dragonshrine",    BOSS_ECHO_OF_BAINE,          },

    //Obsidian first
    {DRAKE_SANCTUM_OBSIDIEN,    "Obsidian Dragonshrine",    BOSS_ECHO_OF_BAINE,         DRAKE_SANCTUM_RUBIS,       "Ruby Dragonshrine",        BOSS_ECHO_OF_SYLVANAS,       },
    {DRAKE_SANCTUM_OBSIDIEN,    "Obsidian Dragonshrine",    BOSS_ECHO_OF_BAINE,         DRAKE_SANCTUM_AZUR,        "Azure Dragonshrine",       BOSS_ECHO_OF_JAINA,          },
    {DRAKE_SANCTUM_OBSIDIEN,    "Obsidian Dragonshrine",    BOSS_ECHO_OF_BAINE,         DRAKE_SANCTUM_EMERAUD,     "Emerald Dragonshrine",     BOSS_ECHO_OF_TYRANDE,        },
};

Position const Murozond_Tornados[18]
{
    {4196.32f,  -384.417f,   120.985f,    2.57427f},
    {4202.81f,  -400.409f,   120.664f,    4.74985f},
    {4201.31f,  -434.286f,   118.738f,    4.41526f},
    {4184.26f,  -458.655f,   121.343f,    3.3322f},
    {4167.17f,  -475.106f,   121.712f,    0.587235f},
    {4142.15f,  -487.829f,   123.397f,    1.52186f},
    {4121.13f,  -494.625f,   123.206f,    1.03492f},
    {4095.35f,  -500.284f,   122.866f,    0.104215f},
    {4064.f,    -487.916f,   122.634f,    5.81403f},
    {4045.18f,  -458.991f,   121.175f,    1.77316f},
    {4041.63f,  -437.402f,   119.284f,    1.05846f},
    {4046.46f,  -410.846f,   119.347f,    0.940629f},
    {4063.58f,  -384.482f,   118.963f,    4.69487f},
    {4096.17f,  -369.108f,   119.157f,    5.69621f},
    {4105.5f,   -355.504f,   119.311f,    2.92772f},
    {4123.8f,   -356.305f,   119.585f,    2.6528f},
    {4155.43f,  -349.267f,   121.963f,    0.112025f},
    {4171.99f,   -367.086f,   121.058f,    0.202347f}
};
#endif
