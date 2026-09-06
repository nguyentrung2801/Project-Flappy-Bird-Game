#ifndef GAME_BIRD_H
#define GAME_BIRD_H
#include <stdbool.h>
#include "game/config.h"
typedef struct { float y, velocity; } Bird;
void Bird_Reset(Bird *bird);
void Bird_Update(Bird *bird, const LevelConfig *config, bool jump);
#endif