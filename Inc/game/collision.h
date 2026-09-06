#ifndef GAME_COLLISION_H
#define GAME_COLLISION_H
#include "game/bird.h"
#include "game/pipe.h"
bool Collision_Check(const Bird *bird, const Pipe *pipe, const LevelConfig *config);
#endif