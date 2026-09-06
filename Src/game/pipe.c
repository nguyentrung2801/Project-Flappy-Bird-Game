#include "game/pipe.h"
#include <stdlib.h>
void Pipe_Reset(Pipe *pipe)
{
    pipe->x = PIPE_START_X;
    pipe->center = 20 + rand() % 41;
    pipe->scored = false;
}
void Pipe_Update(Pipe *pipe, const LevelConfig *config)
{
    --pipe->x;
    if (pipe->x + config->pipe_width <= 0) Pipe_Reset(pipe);
}