#ifndef GFX_RENDER_H
#define GFX_RENDER_H

#include "game/game.h"

void Render_DrawFrameLines(void);

/* Bird sprite render */
void Bird_DrawAt(int16_t x, int16_t y);
void Render_Game(const Game *game, bool first_frame);

#endif
