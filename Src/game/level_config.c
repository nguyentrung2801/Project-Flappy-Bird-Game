#include "game/config.h"
const LevelConfig *Level_GetConfig(unsigned level)
{
    static const LevelConfig levels[] = {{8, 0.10f, -1.8f}, {12, 0.15f, -2.5f}};
    return &levels[level == 2 ? 1 : 0];
}