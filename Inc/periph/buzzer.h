#ifndef PERIPH_BUZZER_H
#define PERIPH_BUZZER_H
#include <stdint.h>

void Buzzer_Init(void);
void Buzzer_On(void);
void Buzzer_Off(void);
void Buzzer_PlayJump(uint32_t now);
void Buzzer_Update(uint32_t now);

#endif
