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

#include "stm31f10x.h"
#include "stm31f10x_rcc.h"
#include "stm31f10x_gpio.h"
#include "stm31f10x_tim.h"
#include "stm31f10x_adc.h"
#include "stm31f10x_dma.h"
#include "misc.h" // For NVIC configuration
#include "Pwm.h"


void TIM1_IRQHandler(void)
{
    // Check if the timer update interrupt is pending
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        Pwm_NotificationHandler()
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
    if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
    {

        // Clear the interrupt flag
        Pwm_Tim1_Handler(TIM_IT_CC1);
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
    }
    if (TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)
    {

        // Clear the interrupt flag
        Pwm_Tim1_Handler(TIM_IT_CC2);
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
    }
    if (TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET)
    {

        // Clear the interrupt flag
        Pwm_Tim1_Handler(TIM_IT_CC3)
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);
    }
    if (TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET)
    {

        // Clear the interrupt flag
        Pwm_Tim1_Handler(TIM_IT_CC4);
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC);
    }
}   
void TIM2_IRQHandler(void)
{
    // Check if the timer update interrupt is pending
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        Pwm_Tim2_Channel1_Notification(TIM_IT_Update);
        Pwm_Tim2_Channel2_Notification(TIM_IT_Update);
        Pwm_Tim2_Channel3_Notification(TIM_IT_Update);
        Pwm_Tim2_Channel4_Notification(TIM_IT_Update);
        // Clear the interrupt flag
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        
    }
    if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
    {
        Pwm_Tim2_Channel1_Notification(TIM_IT_CC1);
        // Clear the interrupt flag
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
    }
    if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
    {
        Pwm_Tim2_Channel2_Notification(TIM_IT_CC2);
        // Clear the interrupt flag
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
    }
    if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)
    {
        Pwm_Tim2_Channel3_Notification(TIM_IT_CC3);
        // Clear the interrupt flag
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
    }
    if (TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET)
    {
        Pwm_Tim2_Channel4_Notification(TIM_IT_CC4);
        // Clear the interrupt flag
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
    }
}   
void TIM3_IRQHandler(void)
{
    // Check if the timer update interrupt is pending
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        Pwm_Tim3_Channel1_Notification(TIM_IT_Update);
        Pwm_Tim3_Channel2_Notification(TIM_IT_Update);
        Pwm_Tim3_Channel3_Notification(TIM_IT_Update);
        Pwm_Tim3_Channel4_Notification(TIM_IT_Update);
        // Clear the interrupt flag
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        
    }
    if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
    {
        Pwm_Tim3_Channel1_Notification(TIM_IT_CC1);
        // Clear the interrupt flag
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
    }
        if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
    {
        Pwm_Tim3_Channel2_Notification(TIM_IT_CC2);
        // Clear the interrupt flag
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
    }
    if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
    {
        Pwm_Tim3_Channel3_Notification(TIM_IT_CC3);
        // Clear the interrupt flag
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
    }
    if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)
    {
        Pwm_Tim3_Channel4_Notification(TIM_IT_CC4);
        // Clear the interrupt flag
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
    }
}   

void TIM4_IRQHandler(void)
{
    // Check if the timer update interrupt is pending
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        Pwm_Tim4_Channel1_Notification(TIM_IT_Update);
        Pwm_Tim4_Channel2_Notification(TIM_IT_Update);
        Pwm_Tim4_Channel3_Notification(TIM_IT_Update);
        Pwm_Tim4_Channel4_Notification(TIM_IT_Update);
        // Clear the interrupt flag
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
        
    }
    if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
    {
        Pwm_Tim4_Channel1_Notification(TIM_IT_CC1);
        // Clear the interrupt flag
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
    }
        if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)
    {
        Pwm_Tim4_Channel2_Notification(TIM_IT_CC2);
        // Clear the interrupt flag
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
    }
    if (TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)
    {
        Pwm_Tim4_Channel3_Notification(TIM_IT_CC3);
        // Clear the interrupt flag
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
    }
    if (TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET)
    {

        // Clear the interrupt flag
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
    }
}   
