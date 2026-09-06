#ifndef GAME_GAME_H
#define GAME_GAME_H
#include "game/bird.h"
#include "game/pipe.h"
typedef struct {
    Bird bird;
    Pipe pipe;
    const LevelConfig *config;
    uint16_t score;
    bool over;
} Game;
void Game_Reset(Game *game, unsigned level);
void Game_Update(Game *game, bool jump);
#endif