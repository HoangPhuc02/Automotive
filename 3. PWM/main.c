/**********************************************************
* File Name   : main 
* Description : Main Source File
* Details     : Main file implementation.
* Version     : 1.0.0
* Date        : 20/06/2025
* Author      : hoangphuc540202@gmail.com
* Github      : https://github.com/HoangPhuc02
 **********************************************************/

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "misc.h" // For NVIC configuration
#include "Dio.h"
#include "Port.h"
#include "Pwm.h"

extern const Port_ConfigType PortCfg_Port; 
extern const Pwm_ConfigType Pwm_Config;
// extern const Port_ConfigType PortCfg_Port; 
void delay(uint16_t time)
{
    TIM_SetCounter(TIM2, 0);
    while (TIM_GetCounter(TIM2) < time)
        ;
}


void SystemClock_Config(void);
void Timer_Init(void);
void NVIC_Config(void);
int main()
{
    // Initialize the hardware
    SystemClock_Config();
    Timer_Init();
    NVIC_Config();
    Port_Init(&PortCfg_Port);
    Pwm_Init(&Pwm_Config);
    
    while (1)
    {
        // Start ADC conversion
    
        // Wait for conversion to complete
        // Do something with adcValue
        delay(500); // Delay for 500ms
    }
}
void NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // Enable TIM3 interrupt in NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // Highest priority
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // No sub-priority
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE; // Enable the interrupt
    NVIC_Init(&NVIC_InitStructure);
}
void SystemClock_Config(void)
{
    ErrorStatus HSEStartUpStatus;
    
    /* Reset RCC configuration */
    RCC_DeInit();
    
    /* Enable HSE (High Speed External) oscillator */
    RCC_HSEConfig(RCC_HSE_ON);
    
    /* Wait for HSE to be ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    
    if(HSEStartUpStatus == SUCCESS)
    {   
        FLASH_SetLatency(FLASH_Latency_2); // why need this code 
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        /* Configure AHB clock (HCLK) */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);    // AHB = SYSCLK = 72MHz
        
        /* Configure APB1 clock (PCLK1) */
        RCC_PCLK1Config(RCC_HCLK_Div2);     // APB1 = 36MHz (Timer clock = 72MHz)
        
        /* Configure APB2 clock (PCLK2) */
        RCC_PCLK2Config(RCC_HCLK_Div1);     // APB2 = 72MHz
        
        /* Configure PLL: HSE × 9 = 8MHz × 9 = 72MHz */
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
        
        /* Enable PLL */
        RCC_PLLCmd(ENABLE);
        
        /* Wait for PLL to be ready */
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        
        /* Select PLL as system clock source */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        
        /* Wait until PLL is used as system clock source */
        while(RCC_GetSYSCLKSource() != 0x08);
    }
    else
    {
        /* HSE failed to start - handle error */
        /* You can add error handling here */
        while(1); // Infinite loop - system halt
    }
}
void Timer_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef tim;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_Period = 0xFFFF;
    tim.TIM_Prescaler = 8000 - 1; // Prescaler for 1ms tick
    TIM_TimeBaseInit(TIM2, &tim);
    TIM_Cmd(TIM2, ENABLE);
}
