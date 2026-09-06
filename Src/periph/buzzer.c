#include "periph/buzzer.h"
#include "board_config.h"
#include "drivers/gpio.h"

#include <stm32f401re_gpio.h>
#include <stdbool.h>

static bool playing;
static uint32_t started_at;

void Buzzer_Init(void)
{
  GPIO_AllClocks_Enable();

  GPIO_InitTypeDef gi;
  gi.GPIO_Pin   = BUZZ_PIN;
  gi.GPIO_Mode  = GPIO_Mode_OUT;
  gi.GPIO_OType = GPIO_OType_PP;
  gi.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  gi.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(BUZZ_GPIOx, &gi);

  Buzzer_Off();
}

void Buzzer_On(void)  { GPIO_SetBits(BUZZ_GPIOx, BUZZ_PIN); }
void Buzzer_Off(void)
{
  GPIO_ResetBits(BUZZ_GPIOx, BUZZ_PIN);
  playing = false;
}
void Buzzer_PlayJump(uint32_t now)
{
  Buzzer_On();
  started_at = now;
  playing = true;
}
void Buzzer_Update(uint32_t now)
{
  if (playing && (uint32_t)(now - started_at) >= 60u) Buzzer_Off();
}
