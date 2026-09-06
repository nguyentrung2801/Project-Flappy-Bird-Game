#ifndef GAME_PIPE_H
#define GAME_PIPE_H
#include <stdbool.h>
#include <stdint.h>
#include "game/config.h"
typedef struct { int16_t x, center; bool scored; } Pipe;
void Pipe_Reset(Pipe *pipe);
void Pipe_Update(Pipe *pipe, const LevelConfig *config);
#endif