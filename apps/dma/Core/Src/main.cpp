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

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_tx;
char msg[] = "Hello STM32 Lovers! This message is transferred in DMA Mode.\r\n";

static void GpioInit() {
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIOInitStruct(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL);  // LED (PA5 = LD2)
}

static void UartInit() {
  __HAL_RCC_USART2_CLK_ENABLE();
  GPIOInitStruct(GPIOA, GPIO_PIN_2, GPIO_MODE_AF_PP, GPIO_NOPULL, 3);  // TX (alt-function push-pull)
  GPIOInitStruct(GPIOA, GPIO_PIN_3, GPIO_MODE_INPUT, GPIO_NOPULL);     // RX

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart2);
}

int main(void) {
  HAL_Init();
  SystemClockInit();

  GpioInit();
  UartInit();
  __HAL_RCC_DMA1_CLK_ENABLE();  // DMA controller clock must be on before configuring a channel

  hdma_usart2_tx.Instance = DMA1_Channel7;  // USART2_TX is hard-wired to DMA1 channel 7 on STM32F1
  hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart2_tx.Init.Mode = DMA_NORMAL;
  hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
  HAL_DMA_Init(&hdma_usart2_tx);

  HAL_DMA_Start(&hdma_usart2_tx, (uint32_t)msg, (uint32_t)&huart2.Instance->DR, strlen(msg));
  huart2.Instance->CR3 |= USART_CR3_DMAT;  // tell USART2 to raise a DMA request on each empty TX
  HAL_DMA_PollForTransfer(&hdma_usart2_tx, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
  huart2.Instance->CR3 &= ~USART_CR3_DMAT;  // clear ONLY the DMAT bit once the transfer is done

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);  // LED on = transfer complete
  while (1);
}
