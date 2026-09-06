#include "game/collision.h"
bool Collision_Check(const Bird *bird, const Pipe *pipe, const LevelConfig *config)
{
    if (bird->y < PLAY_TOP || bird->y + BIRD_H > PLAY_BOTTOM) return true;
    if (pipe->x < BIRD_X + BIRD_W && pipe->x + config->pipe_width > BIRD_X)
        return bird->y < pipe->center - PIPE_GAP_HALF ||
               bird->y + BIRD_H > pipe->center + PIPE_GAP_HALF;
    return false;
}