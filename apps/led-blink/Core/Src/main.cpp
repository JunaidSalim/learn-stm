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

void SystemClockInit();
void GPIOInitStruct(GPIO_TypeDef* port, uint32_t pin, uint32_t mode = 0, uint32_t pull = 0,
                    uint32_t speed = 2);

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

void Error_Handler(void) {
  __disable_irq();
  while (1) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    HAL_Delay(1000);
  }
}

void GPIOInitStruct(GPIO_TypeDef* port, uint32_t pin, uint32_t mode, uint32_t pull,
                    uint32_t speed) {
  GPIO_InitTypeDef gpio = {};
  gpio.Pin = pin;
  gpio.Mode = mode;
  gpio.Pull = pull;    // default: no pull
  gpio.Speed = speed;  // default: low (2 MHz)
  HAL_GPIO_Init(port, &gpio);
}

void SystemClockInit() {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;  // 8 MHz (HSE) * 9 = 72 MHz
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType =
      RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;  // HCLK 72 MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;   // PCLK1 36, TIM2 CLK 72
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;   // PCLK 72

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;  // 72 / 6 = 12 MHz
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
    Error_Handler();
  }
}