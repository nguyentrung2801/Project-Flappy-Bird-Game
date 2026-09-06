#include "periph/led.h"
#include "board_config.h"
#include "drivers/gpio.h"

#include <stm32f401re_gpio.h>

void LED_Init(void)
{
  GPIO_AllClocks_Enable();

  GPIO_InitTypeDef gi;
  gi.GPIO_Pin   = LED_PIN;
  gi.GPIO_Mode  = GPIO_Mode_OUT;
  gi.GPIO_OType = GPIO_OType_PP;
  gi.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  gi.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LED_GPIOx, &gi);
}

void LED_On(void)     { GPIO_SetBits(LED_GPIOx, LED_PIN); }
void LED_Off(void)    { GPIO_ResetBits(LED_GPIOx, LED_PIN); }
void LED_Toggle(void) { GPIO_ToggleBits(LED_GPIOx, LED_PIN); }
