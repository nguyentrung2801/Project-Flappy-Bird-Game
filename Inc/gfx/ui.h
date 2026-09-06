#ifndef GFX_UI_H
#define GFX_UI_H
#include <stdbool.h>
#include <stdint.h>
void UI_DrawMenu(unsigned level, bool start_selected, uint16_t last_score);
void UI_DrawGameOver(uint16_t score);
#endif
