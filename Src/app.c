#include "app.h"
#include "game/game.h"
#include "gfx/render.h"
#include "gfx/ui.h"
#include "drivers/timebase.h"
#include "periph/input.h"
#include "periph/led.h"
#include "periph/buzzer.h"
#include <stdlib.h>

typedef enum { MENU, PLAYING, GAME_OVER } AppState;
void App_RunForever(void)
{
    Game game;
    AppState state = MENU;
    unsigned level = 1;
    uint16_t last_score = 0;
    bool start_selected = false, jump_pending = false, seeded = false;
    uint32_t last_step = 0, over_started = 0;
    UI_DrawMenu(level, start_selected, last_score);
    for (;;) {
        Timebase_Poll();
        uint32_t now = millis();
        Input_Update(now);
        Buzzer_Update(now);
        bool up = Input_TakePress(INPUT_UP);
        bool select = Input_TakePress(INPUT_SELECT);
        bool down = Input_TakePress(INPUT_DOWN);
        if (state == MENU) {
            bool redraw = false;
            if (up && start_selected) { start_selected = false; redraw = true; }
            if (down && !start_selected) { start_selected = true; redraw = true; }
            if (select) {
                if (!start_selected) { level = level == 1 ? 2 : 1; redraw = true; }
                else {
                    if (!seeded) { srand(now); seeded = true; }
                    Game_Reset(&game, level);
                    Input_ClearEvents();
                    jump_pending = false;
                    Buzzer_Off();
                    LED_Off();
                    Render_Game(&game, true);
                    last_step = millis();
                    state = PLAYING;
                }
            }
            if (state == MENU && redraw) UI_DrawMenu(level, start_selected, last_score);
        } else if (state == PLAYING) {
            jump_pending = jump_pending || select;
            bool changed = false;
            /* Catch up the complete model before drawing a single frame. */
            while ((uint32_t)(now - last_step) >= GAME_STEP_MS && !game.over) {
                last_step += GAME_STEP_MS;
                if (jump_pending) Buzzer_PlayJump(now);
                Game_Update(&game, jump_pending);
                jump_pending = false;
                changed = true;
            }
            if (game.over) {
                Buzzer_Off();
                Input_ClearEvents();
                last_score = game.score;
                state = GAME_OVER;
                UI_DrawGameOver(last_score);
                over_started = millis();
                LED_On();
            } else if (changed) Render_Game(&game, false);
        } else {
            uint32_t elapsed = now - over_started;
            if (elapsed >= 1200u) {
                LED_Off();
                Input_ClearEvents();
                start_selected = false;
                state = MENU;
                UI_DrawMenu(level, start_selected, last_score);
            } else if ((elapsed / 120u) % 2u == 0) LED_On();
            else LED_Off();
        }
    }
}
