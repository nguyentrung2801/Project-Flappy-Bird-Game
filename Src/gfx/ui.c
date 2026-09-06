#include "gfx/ui.h"
#include "gfx/lcd_init.h"
#include "gfx/render.h"
#include <stdio.h>
void UI_DrawMenu(unsigned level, bool start_selected, uint16_t last_score)
{
    char text[32];
    ucg_ClearScreen(&ucg);
    Render_DrawFrameLines();
    ucg_SetFont(&ucg, ucg_font_helvR08_tf);
    UCG_SetRGB(0, 255, 255, 255);
    ucg_DrawString(&ucg, 6, 16, 0, "FLAPPY BIRD");
    snprintf(text, sizeof(text), "%c LEVEL: %u", start_selected ? ' ' : '>', level);
    ucg_DrawString(&ucg, 6, 34, 0, text);
    ucg_DrawString(&ucg, 6, 48, 0, start_selected ? "> START" : "  START");
    snprintf(text, sizeof(text), "LAST SCORE: %u", (unsigned)last_score);
    ucg_DrawString(&ucg, 6, 64, 0, text);
}
void UI_DrawGameOver(uint16_t score)
{
    char text[24];
    ucg_ClearScreen(&ucg);
    Render_DrawFrameLines();
    UCG_SetRGB(0, 255, 255, 255);
    ucg_DrawString(&ucg, 18, 30, 0, "GAME OVER");
    snprintf(text, sizeof(text), "SCORE: %u", (unsigned)score);
    ucg_DrawString(&ucg, 18, 48, 0, text);
}
