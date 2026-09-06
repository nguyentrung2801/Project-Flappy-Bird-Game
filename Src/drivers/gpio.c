#include "drivers/gpio.h"
#include <stm32f401re_rcc.h>

void GPIO_AllClocks_Enable(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA |
                         RCC_AHB1Periph_GPIOB |
                         RCC_AHB1Periph_GPIOC, ENABLE);
}
