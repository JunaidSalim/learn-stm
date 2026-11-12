#ifndef BOARD_H
#define BOARD_H
#include <stm32f1xx.h>

void Error_Handler(void);
void SystemClockInit();
void GPIOInitStruct(GPIO_TypeDef* port, uint32_t pin, uint32_t mode = 0, uint32_t pull = 0,
                    uint32_t speed = 2);

#endif