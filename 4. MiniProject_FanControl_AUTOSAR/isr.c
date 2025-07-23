/****************************************************************************************
*                                ISR.C                                                  *
****************************************************************************************
* File Name   : isr.c
* Description : Interrupt Service Routine Source File
* Details     : Interrupt implementation.
* Version     : 1.0.0
* Date        : 01/07/1015
* Author      : hoangphuc540101@gmail.com
* Github      : https://github.com/HoangPhuc01
****************************************************************************************/

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "misc.h" // For NVIC configuration
#include "Pwm.h"
#include "Adc_Cfg.h"

void HardFault_Handler(void)
{
    // Fault occurred - examine registers
    __asm volatile (
        "mov r0, lr\n"
        "mov r1, sp\n"
        "bl fault_handler_c\n"
    );
}

void fault_handler_c(uint32_t lr_value, uint32_t sp_value)
{
    // Set breakpoint here to examine fault
    volatile uint32_t *stack_pointer = (uint32_t*)sp_value;
    volatile uint32_t pc = stack_pointer[6];  // Program Counter at fault
    volatile uint32_t lr = stack_pointer[5];  // Link Register
    volatile uint32_t psr = stack_pointer[7]; // Program Status Register
    
    // Examine fault status registers
    volatile uint32_t cfsr = *(volatile uint32_t*)0xE000ED28;  // Configurable Fault Status
    volatile uint32_t hfsr = *(volatile uint32_t*)0xE000ED2C;  // Hard Fault Status
    volatile uint32_t dfsr = *(volatile uint32_t*)0xE000ED30;  // Debug Fault Status
    volatile uint32_t afsr = *(volatile uint32_t*)0xE000ED3C;  // Auxiliary Fault Status
    
    // Set breakpoint on next line to examine these values
    while(1);  // Trap here for debugging
}

void ADC1_2_IRQHandler(void)
{

    // Check if the ADC conversion is complete
    if (ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET)
    {
        Adc_TransferComplete_Callback(ADC1);

        // Clear the interrupt flag
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
    }

    else if (ADC_GetITStatus(ADC2, ADC_IT_EOC) != RESET)
    {
        // Handle overrun error
        Adc_TransferComplete_Callback(ADC1);

        // Clear the interrupt flag
        ADC_ClearITPendingBit(ADC2, ADC_IT_EOC);
    }
}
void DMA1_Channel1_IRQHandler(void)
{

    if (DMA_GetITStatus(DMA1_IT_TC1))
    {
        Adc_DmaTransferComplete_Callback(DMA1_Channel1);
        DMA_ClearITPendingBit(DMA1_IT_TC1);
    }
}


void TIM1_IRQHandler(void)
{
    // Check if the timer update interrupt is pending
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        Pwm_NotificationHandler(PWM_HW_UNIT_TIM1, TIM_IT_Update);
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
    if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
    {

        // Clear the interrupt flag
        Pwm_NotificationHandler(PWM_HW_UNIT_TIM1, TIM_IT_CC1);
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
    }
    if (TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)
    {

        // Clear the interrupt flag
        Pwm_NotificationHandler(PWM_HW_UNIT_TIM1, TIM_IT_CC2);
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
    }
    if (TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET)
    {

        // Clear the interrupt flag
        Pwm_NotificationHandler(PWM_HW_UNIT_TIM1, TIM_IT_CC3);
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);
    }
    if (TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET)
    {

        // Clear the interrupt flag
        Pwm_NotificationHandler(PWM_HW_UNIT_TIM1, TIM_IT_CC4);
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);
    }
}   
void TIM2_IRQHandler(void)
{
    // Check if the timer update interrupt is pending
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        Pwm_NotificationHandler(PWM_HW_UNIT_TIM2, TIM_IT_Update);
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
    if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
    {

        // Clear the interrupt flag
        Pwm_NotificationHandler(PWM_HW_UNIT_TIM2, TIM_IT_CC1);
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
    }
    if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
    {

        // Clear the interrupt flag
        Pwm_NotificationHandler(PWM_HW_UNIT_TIM2, TIM_IT_CC2);
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
    }
    if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)
    {

        // Clear the interrupt flag
        Pwm_NotificationHandler(PWM_HW_UNIT_TIM2, TIM_IT_CC3);
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
    }
    if (TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET)
    {

        // Clear the interrupt flag
        Pwm_NotificationHandler(PWM_HW_UNIT_TIM2, TIM_IT_CC4);
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
    }
} 
void TIM3_IRQHandler(void)
{
    // Check if the timer update interrupt is pending
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        Pwm_NotificationHandler(PWM_HW_UNIT_TIM3, TIM_IT_Update);
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
    if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
    {

        // Clear the interrupt flag
        Pwm_NotificationHandler(PWM_HW_UNIT_TIM3, TIM_IT_CC1);
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
    }
    if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
    {

        // Clear the interrupt flag
        Pwm_NotificationHandler(PWM_HW_UNIT_TIM3, TIM_IT_CC2);
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
    }
    if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
    {

        // Clear the interrupt flag
        Pwm_NotificationHandler(PWM_HW_UNIT_TIM3, TIM_IT_CC3);
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
    }
    if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)
    {

        // Clear the interrupt flag
        Pwm_NotificationHandler(PWM_HW_UNIT_TIM3, TIM_IT_CC4);
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
    }
} 

void TIM4_IRQHandler(void)
{
    // Check if the timer update interrupt is pending
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        Pwm_NotificationHandler(PWM_HW_UNIT_TIM4, TIM_IT_Update);
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
    if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
    {

        // Clear the interrupt flag
        Pwm_NotificationHandler(PWM_HW_UNIT_TIM4, TIM_IT_CC1);
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
    }
    if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)
    {

        // Clear the interrupt flag
        Pwm_NotificationHandler(PWM_HW_UNIT_TIM4, TIM_IT_CC2);
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
    }
    if (TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)
    {

        // Clear the interrupt flag
        Pwm_NotificationHandler(PWM_HW_UNIT_TIM4, TIM_IT_CC3);
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
    }
    if (TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET)
    {

        // Clear the interrupt flag
        Pwm_NotificationHandler(PWM_HW_UNIT_TIM4, TIM_IT_CC4);
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
    }
} 
