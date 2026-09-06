#include "game/game.h"
#include "game/collision.h"
void Game_Reset(Game *game, unsigned level)
{
    game->config = Level_GetConfig(level);
    Bird_Reset(&game->bird);
    Pipe_Reset(&game->pipe);
    game->score = 0;
    game->over = false;
}
void Game_Update(Game *game, bool jump)
{
    if (game->over) return;
    Bird_Update(&game->bird, game->config, jump);
    Pipe_Update(&game->pipe, game->config);
    game->over = Collision_Check(&game->bird, &game->pipe, game->config);
    if (!game->over && !game->pipe.scored &&
        game->pipe.x + game->config->pipe_width <= BIRD_X) {
        game->pipe.scored = true;
        if (game->score < UINT16_MAX) ++game->score;
    }
}