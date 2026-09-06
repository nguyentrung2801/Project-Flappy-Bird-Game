#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include <stdint.h>
#include <stm32f401re_gpio.h>

/* ============================ LCD COLOR ORDER ============================ */
/* Nếu màu bị đảo (vàng -> xanh) thì để LCD_IS_BGR = 1, đúng màu thì = 0 */
#define LCD_IS_BGR  1

/* =============================== GPIO: LED/BUZZ ============================= */
#define LED_GPIOx   GPIOA
#define LED_PIN     GPIO_Pin_11

#define BUZZ_GPIOx  GPIOC
#define BUZZ_PIN    GPIO_Pin_9

/* ============================== GPIO: SWITCHES ============================== */
/* Switch active-LOW, có kéo lên (pull-up): nhấn -> đọc 0 */
#define SW1_GPIOx   GPIOB
#define SW1_PIN     GPIO_Pin_5

#define SW2_GPIOx   GPIOB
#define SW2_PIN     GPIO_Pin_3

#define SW3_GPIOx   GPIOA
#define SW3_PIN     GPIO_Pin_4

#define SW4_GPIOx   GPIOB
#define SW4_PIN     GPIO_Pin_0

#define SW5_GPIOx   GPIOB
#define SW5_PIN     GPIO_Pin_4

#endif
