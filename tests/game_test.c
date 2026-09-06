#include "game/game.h"
#include "game/collision.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
    for (unsigned level = 1; level <= 2; ++level) {
        const LevelConfig *config = Level_GetConfig(level);
        assert(config->pipe_width == (level == 1 ? 8 : 12));
        Pipe pipe = {.x = BIRD_X, .center = 40, .scored = false};
        Bird bird = {.y = 22.0f, .velocity = 0.0f};
        assert(!Collision_Check(&bird, &pipe, config));
        bird.y = 21.99f;
        assert(Collision_Check(&bird, &pipe, config));
        bird.y = 50.0f;
        assert(!Collision_Check(&bird, &pipe, config));
        bird.y = 50.01f;
        assert(Collision_Check(&bird, &pipe, config));
        pipe.x = BIRD_X - config->pipe_width;
        bird.y = 10.0f;
        assert(!Collision_Check(&bird, &pipe, config));
        ++pipe.x;
        assert(Collision_Check(&bird, &pipe, config));
        pipe.x = PIPE_START_X;
        bird.y = PLAY_TOP;
        assert(!Collision_Check(&bird, &pipe, config));
        bird.y -= 0.01f;
        assert(Collision_Check(&bird, &pipe, config));
        bird.y = PLAY_BOTTOM - BIRD_H;
        assert(!Collision_Check(&bird, &pipe, config));
        bird.y += 0.01f;
        assert(Collision_Check(&bird, &pipe, config));

        Game game;
        Game_Reset(&game, level);
        game.pipe.x = BIRD_X - config->pipe_width + 1;
        game.pipe.center = 40;
        Game_Update(&game, false);
        assert(game.score == 1 && game.pipe.scored && !game.over);
        Game_Update(&game, false);
        assert(game.score == 1);

        game.pipe.x = 1 - config->pipe_width;
        Pipe_Update(&game.pipe, config);
        assert(game.pipe.x == PIPE_START_X && !game.pipe.scored);
        assert(game.pipe.center >= 20 && game.pipe.center <= 60);

        Game_Reset(&game, level);
        game.bird.y = PLAY_BOTTOM - BIRD_H;
        game.pipe.x = BIRD_X - config->pipe_width + 1;
        Game_Update(&game, false);
        assert(game.over && game.score == 0);
        float dead_y = game.bird.y;
        int dead_x = game.pipe.x;
        Game_Update(&game, true);
        assert(game.bird.y == dead_y && game.pipe.x == dead_x);
        Game_Reset(&game, level);
        assert(!game.over && game.score == 0 && game.bird.velocity == 0.0f);
        Game_Update(&game, true);
        assert(game.bird.y < 30.0f && game.pipe.x == PIPE_START_X - 1);
    }
    puts("game_test: all checks passed");
    return 0;
}
