#ifndef __STM32F1xx_IT_H
#define __STM32F1xx_IT_H

#ifdef __cplusplus
extern "C" {
#endif

void NMI_Handler();
void HardFault_Handler();
void MemManage_Handler();
void BusFault_Handler();
void UsageFault_Handler();
// void SVC_Handler();
void DebugMon_Handler();
// void PendSV_Handler();
void SysTick_Handler();
void USB_HP_CAN1_TX_IRQHandler();
void USB_LP_CAN1_RX0_IRQHandler();
void EXTI15_10_IRQHandler();

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_IT_H */
