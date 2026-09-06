#ifndef GFX_LCD_INIT_H
#define GFX_LCD_INIT_H

#include <Ucglib.h>
#include "board_config.h"

/* dùng chung cho toàn project */
extern ucg_t ucg;

#if LCD_IS_BGR
  #define UCG_SetRGB(idx, r, g, b)   ucg_SetColor(&ucg, (idx), (b), (g), (r))
#else
  #define UCG_SetRGB(idx, r, g, b)   ucg_SetColor(&ucg, (idx), (r), (g), (b))
#endif

void LCD_Init_All(void);

#endif
