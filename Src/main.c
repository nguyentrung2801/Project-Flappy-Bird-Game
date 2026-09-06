#include <system_stm32f4xx.h>
#include <timer.h>

#include "drivers/timebase.h"
#include "gfx/lcd_init.h"

#include "periph/led.h"
#include "periph/buzzer.h"
#include "periph/input.h"

#include "app.h"

int main(void)
{
  SystemCoreClockUpdate();

  TimerInit();

  LED_Init();
  LED_Off();
  Buzzer_Init();
  Input_Init();

  LCD_Init_All();

  App_RunForever();
}
