#include "game/bird.h"
void Bird_Reset(Bird *bird) { bird->y = 30.0f; bird->velocity = 0.0f; }
void Bird_Update(Bird *bird, const LevelConfig *config, bool jump)
{
    if (jump) bird->velocity = config->jump_velocity;
    bird->velocity += config->gravity;
    bird->y += bird->velocity;
}