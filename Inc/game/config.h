#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H
/* Half-open rectangles: [x, x + width). */
#define SCREEN_W 128
#define SCREEN_H 80
#define PLAY_TOP 1
#define PLAY_BOTTOM (SCREEN_H - 1)
#define GAME_STEP_MS 20u
#define BIRD_X 20
#define BIRD_W 12
#define BIRD_H 8
#define PIPE_START_X 115
#define PIPE_GAP_HALF 18
typedef struct { int pipe_width; float gravity, jump_velocity; } LevelConfig;
const LevelConfig *Level_GetConfig(unsigned level);
#endif