#include "gfx/render.h"
#include "gfx/lcd_init.h"
#include "game/config.h"

static const uint16_t bird_mask[BIRD_H] = {
  0b000111100000,
  0b001111110000,
  0b011111111000,
  0b111101111100,
  0b111111111110,
  0b011111111100,
  0b001111111000,
  0b000111110000
};

void Render_DrawFrameLines(void)
{
  UCG_SetRGB(0, 255, 255, 255);
  ucg_DrawHLine(&ucg, 0, 0, SCREEN_W);
  ucg_DrawHLine(&ucg, 0, SCREEN_H - 1, SCREEN_W);
}

void Bird_DrawAt(int16_t x, int16_t y)
{
  UCG_SetRGB(0, 255, 255, 0);
  for (int r = 0; r < BIRD_H; r++)
  {
    uint16_t row = bird_mask[r];
    for (int c = 0; c < BIRD_W; c++)
    {
      if (row & (1u << (BIRD_W - 1 - c)))
        ucg_DrawPixel(&ucg, x + c, y + r);
    }
  }

  UCG_SetRGB(0, 255, 255, 255);
  ucg_DrawPixel(&ucg, x + 2, y + 1);
  ucg_DrawPixel(&ucg, x + 3, y + 0);
  ucg_DrawPixel(&ucg, x + 9, y + 2);

  UCG_SetRGB(0, 0, 0, 0);
  ucg_DrawPixel(&ucg, x + 7, y + 3);
  ucg_DrawPixel(&ucg, x + 8, y + 3);

  UCG_SetRGB(0, 255, 255, 255);
  ucg_DrawPixel(&ucg, x + 11, y + 3);
  ucg_DrawPixel(&ucg, x + 11, y + 4);

  UCG_SetRGB(0, 0, 0, 0);
  ucg_DrawPixel(&ucg, x + 4, y + 4);
  ucg_DrawPixel(&ucg, x + 5, y + 5);
}

/* Clip signed coordinates before sending rectangles to the LCD. */
static void box(int x, int y, int width, int height)
{
  if (x < 0) { width += x; x = 0; }
  if (y < PLAY_TOP) { height -= PLAY_TOP - y; y = PLAY_TOP; }
  if (x + width > SCREEN_W) width = SCREEN_W - x;
  if (y + height > PLAY_BOTTOM) height = PLAY_BOTTOM - y;
  if (width > 0 && height > 0) ucg_DrawBox(&ucg, x, y, width, height);
}

static void pipe_draw(const Pipe *pipe, int width)
{
  int top = pipe->center - PIPE_GAP_HALF;
  int bottom = pipe->center + PIPE_GAP_HALF;
  box(pipe->x, PLAY_TOP, width, top - PLAY_TOP);
  box(pipe->x, bottom, width, PLAY_BOTTOM - bottom);
}

void Render_Game(const Game *game, bool first_frame)
{
  static Pipe previous_pipe;
  static int previous_width, previous_y;
  if (first_frame) {
    ucg_ClearScreen(&ucg);
    Render_DrawFrameLines();
  } else {
    int shift = previous_pipe.x - game->pipe.x;
    UCG_SetRGB(0, 0, 0, 0);
    if (previous_pipe.center == game->pipe.center && shift >= 0 &&
        shift < previous_width && previous_width == game->config->pipe_width) {
      /* Only update exposed strips when the pipe moves left. */
      Pipe trailing = previous_pipe;
      trailing.x += previous_width - shift;
      pipe_draw(&trailing, shift);
      UCG_SetRGB(0, 255, 255, 255);
      pipe_draw(&game->pipe, shift);
    } else {
      pipe_draw(&previous_pipe, previous_width);
      UCG_SetRGB(0, 255, 255, 255);
      pipe_draw(&game->pipe, game->config->pipe_width);
    }
    UCG_SetRGB(0, 0, 0, 0);
    box(BIRD_X, previous_y, BIRD_W, BIRD_H);
    /* Restore any scenery that moved into the bird's old rectangle. */
    int left = game->pipe.x > BIRD_X ? game->pipe.x : BIRD_X;
    int right = game->pipe.x + game->config->pipe_width;
    if (right > BIRD_X + BIRD_W) right = BIRD_X + BIRD_W;
    int top_end = game->pipe.center - PIPE_GAP_HALF;
    if (top_end > previous_y + BIRD_H) top_end = previous_y + BIRD_H;
    int bottom_start = game->pipe.center + PIPE_GAP_HALF;
    if (bottom_start < previous_y) bottom_start = previous_y;
    UCG_SetRGB(0, 255, 255, 255);
    box(left, previous_y, right - left, top_end - previous_y);
    box(left, bottom_start, right - left, previous_y + BIRD_H - bottom_start);
  }
  if (first_frame) {
    UCG_SetRGB(0, 255, 255, 255);
    pipe_draw(&game->pipe, game->config->pipe_width);
  }
  Bird_DrawAt(BIRD_X, (int16_t)game->bird.y);
  previous_pipe = game->pipe;
  previous_width = game->config->pipe_width;
  previous_y = (int)game->bird.y;
}
