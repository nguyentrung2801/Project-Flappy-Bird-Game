#include "gfx/lcd_init.h"

ucg_t ucg;

void LCD_Init_All(void)
{
  Ucglib4WireSWSPI_begin(&ucg, 0);

  ucg_SetFont(&ucg, ucg_font_helvR12_tf);
  ucg_ClearScreen(&ucg);

  ucg_SetFont(&ucg, ucg_font_helvR08_tf);
  UCG_SetRGB(0, 255, 255, 255);
  UCG_SetRGB(1, 0, 0, 0);

  ucg_SetRotate180(&ucg);
}
