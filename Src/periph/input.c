#include "periph/input.h"
#include "board_config.h"
#include "drivers/gpio.h"
#include "drivers/timebase.h"
typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
    bool raw, stable, pressed;
    uint32_t changed_at;
} Button;
static Button buttons[INPUT_COUNT] = {
    {.port = SW1_GPIOx, .pin = SW1_PIN},
    {.port = SW3_GPIOx, .pin = SW3_PIN},
    {.port = SW5_GPIOx, .pin = SW5_PIN}
};
void Input_Init(void)
{
    GPIO_AllClocks_Enable();
    for (unsigned i = 0; i < INPUT_COUNT; ++i) {
        GPIO_InitTypeDef gpio;
        GPIO_StructInit(&gpio);
        gpio.GPIO_Pin = buttons[i].pin;
        gpio.GPIO_Mode = GPIO_Mode_IN;
        gpio.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(buttons[i].port, &gpio);
        buttons[i].raw = buttons[i].stable =
            GPIO_ReadInputDataBit(buttons[i].port, buttons[i].pin) == 0;
        buttons[i].changed_at = millis();
        buttons[i].pressed = false;
    }
}
void Input_Update(uint32_t now)
{
    for (unsigned i = 0; i < INPUT_COUNT; ++i) {
        Button *b = &buttons[i];
        bool raw = GPIO_ReadInputDataBit(b->port, b->pin) == 0;
        if (raw != b->raw) { b->raw = raw; b->changed_at = now; }
        if (b->raw != b->stable && (uint32_t)(now - b->changed_at) >= 15u) {
            b->stable = b->raw;
            if (b->stable) b->pressed = true;
        }
    }
}
bool Input_TakePress(InputButton button)
{
    if ((unsigned)button >= INPUT_COUNT) return false;
    bool pressed = buttons[button].pressed;
    buttons[button].pressed = false;
    return pressed;
}
void Input_ClearEvents(void)
{
    for (unsigned i = 0; i < INPUT_COUNT; ++i) buttons[i].pressed = false;
}