#ifndef PERIPH_INPUT_H
#define PERIPH_INPUT_H
#include <stdbool.h>
#include <stdint.h>
typedef enum { INPUT_UP, INPUT_SELECT, INPUT_DOWN, INPUT_COUNT } InputButton;
void Input_Init(void);
void Input_Update(uint32_t now);
bool Input_TakePress(InputButton button);
void Input_ClearEvents(void);
#endif