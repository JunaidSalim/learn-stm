/*
  STM32F103RBT6
  Flash: 128 KB
  RAM:   20  KB

  A0:   ADC1 CH0
  A1:   ADC1 CH1 @ DMA1 CH1   (Up   strip reading)
  A2:   ADC1 CH2 @ DMA1 CH1   (Left strip reading)
  A3:   FM17580 RST
  A4:   SPI1 CS (Used by FM17580)
  A5:   LED

*/

#include "main.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  HAL_Init();
  SystemClockInit();

  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIOInitStruct(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL);

  while (1) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    HAL_Delay(1000);  // 1 sec
  }
}

// int main(void) {
//   HAL_Init();  // starts SysTick so HAL_Delay works (clock stays at 8 MHz HSI, fine here)
//   __HAL_RCC_GPIOA_CLK_ENABLE();

//   volatile uint32_t* GPIOA_CRL = (uint32_t*)0x40010800;
//   volatile uint32_t* GPIOA_ODR = (uint32_t*)(0x40010800 + 0x0C);

//   *GPIOA_CRL = (*GPIOA_CRL & ~(0xF << 20)) | (0x2 << 20);  // PA5 output, 2 MHz, push-pull

//   while (1) {
//     *GPIOA_ODR ^= (1 << 5);  // toggle PA5
//     HAL_Delay(500);
//   }
// }
