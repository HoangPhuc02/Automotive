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
#include "stm32f10x_flash.h"
#include "misc.h" // For NVIC configuration
#include "Dio.h"
#include "Port.h"
#include "Pwm.h"
// #include <string.h>

/* Test Configuration */
#define PWM_TEST_CHANNEL_4      4    // Test channel ID
#define PWM_TEST_CHANNEL_5      5    // Second test channel (if available)

/* Test constants */
#define TEST_DELAY_SHORT        100  // Short delay in ms
#define TEST_DELAY_MEDIUM       500  // Medium delay in ms
#define TEST_DELAY_LONG         1000 // Long delay in ms

extern const Port_ConfigType PortCfg_Port; 
extern const Pwm_ConfigType Pwm_Config;

volatile uint8 counter_notification_failing = 0;
volatile uint8 counter_notification_rising = 0;
volatile uint8 counter_notification_bothedge = 0;
void Pwm_Tim2_Channel1_Notification(void)
{
    counter_notification_rising++;
}
void Pwm_Tim2_Channel2_Notification(void)
{
    counter_notification_failing++;
}
void Pwm_Tim2_Channel3_Notification(void)
{
    counter_notification_bothedge++;
}



void SystemClock_Config(void);
void Timer_Init(void);
void NVIC_Config(void);

// Test Functions cho PWM AUTOSAR APIs
void PWM_Test_Basic_Operations(void);
void PWM_Test_Duty_Cycle_Variations(void);
void PWM_Test_Output_State_Reading(void);
void PWM_Test_Idle_State_Control(void);
void PWM_Test_Notifications(void);
void PWM_Test_Edge_Cases(void);
uint16 Pwm_PercentageToDutyCycle(uint8 percentage)
{
    // Convert percentage (0-100) to duty cycle value
    return (uint16)((0x8000 * percentage) / 100);
}

// Helper functions
void Test_Delay_ms(uint16_t ms);
int main()
{
    // Initialize the hardware
    SystemClock_Config();
    // Timer_Init();
    NVIC_Config();
    Port_Init(&PortCfg_Port);
    Pwm_Init(&Pwm_Config);

    // PWM_Test_Basic_Operations();
    // PWM_Test_Duty_Cycle_Variations();
    // PWM_Test_Idle_State_Control();
    PWM_Test_Notifications();
    /* PWM AUTOSAR API Test Suite */
    while (1)
    {

    }
}
void NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // Enable TIM3 interrupt in NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // Highest priority
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // No sub-priority
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE; // Enable the interrupt
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
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

/****************************************************************************************
*                              PWM TEST FUNCTIONS                                      *
****************************************************************************************
* PWM AUTOSAR Driver Test Suite for STM32F103C8T6
* 
* Current Test Configuration:
* - Active Channels: 8 channels (0-7)
* - TIM1 Channels: 0-3 (PA8-PA11) - Variable Period Class
* - TIM2 Channels: 4-7 (PA0-PA3) - Fixed Period Class  
* - PWM Frequency: 1kHz default
* - Timer Clock: 72MHz -> 1MHz after prescaler
*
* Test 1: Basic Operations (IMPLEMENTED)
*   - Channel 4 (PA0): 25% duty cycle test
*   - Channel 5 (PA1): 75% duty cycle test
*   - Verify basic PWM signal generation on TIM2
*
* Test 2: Duty Cycle Variations (IMPLEMENTED)
*   - Percentage to duty cycle conversion utility
*   - Test 10% duty cycle on channels 4 and 5
*   - Verify conversion accuracy and PWM output
*
* Test 3: Idle State Control (IMPLEMENTED)
*   - Channel 6 (PA2): Test SetOutputToIdle() - forces HIGH idle state
*   - Channel 7 (PA3): Test SetOutputToIdle() - forces HIGH idle state
*   - Test PWM restoration after idle using SetDutyCycle(75%)
*   - Verify proper forced output and restoration sequence
*
* Test 4: Notifications (IMPLEMENTED)
*   - Channel 4 (PA0): Rising edge notifications (TIM2_CH1)
*   - Channel 5 (PA1): Falling edge notifications (TIM2_CH2)
*   - Channel 6 (PA2): Both edges notifications (TIM2_CH3)
*   - Counter variables track notification events
*
* Known Issues:
*   - PWM restoration after idle state not working correctly
*   - PA0 (TIM2_CH1) may have specific hardware configuration issues
*   - TIM_ForcedOCxConfig() implementation needs verification
*
* Expected Hardware Results:
*   - PA0 (Channel 4): 25% duty cycle PWM + rising edge notifications
*   - PA1 (Channel 5): 10% or 75% duty cycle PWM + falling edge notifications
*   - PA2 (Channel 6): HIGH idle state, then 75% PWM + both edge notifications
*   - PA3 (Channel 7): HIGH idle state, then 75% PWM
*   - All signals at 1kHz frequency with 3.3V logic levels
****************************************************************************************/

/**
 * @brief Test basic PWM operations on TIM2 channels
 * @details Tests initialization and basic duty cycle setting on PA0 and PA1
 * 
 * Test Results:
 * - Channel 4 (PA0/TIM2_CH1): 25% duty cycle (0x2000) 
 * - Channel 5 (PA1/TIM2_CH2): 75% duty cycle (0x6000)
 * - Expected: PA0 = 250μs HIGH, 750μs LOW per 1ms cycle
 * - Expected: PA1 = 750μs HIGH, 250μs LOW per 1ms cycle
 */
void PWM_Test_Basic_Operations(void)
{
    
    /* Test 1.1: Set different duty cycles for TIM2 channels */
    /* Channel 4 (PA0): 25% duty cycle (0x2000) */
    /* Channel 5 (PA1): 75% duty cycle (0x6000) */
    Pwm_SetDutyCycle(PWM_TEST_CHANNEL_4, 0x2000); // 25% duty cycle  
    Pwm_SetDutyCycle(PWM_TEST_CHANNEL_5, 0x6000); // 75% duty cycle

}

/**
 * @brief Test duty cycle variations using percentage conversion
 * @details Tests percentage to duty cycle conversion utility and applies to TIM2 channels
 * 
 * Test Results:
 * - Utility function: Pwm_PercentageToDutyCycle(10) = 0x0CCC (10% of 0x8000)
 * - Channel 4 (PA0): 10% duty cycle = 100μs HIGH, 900μs LOW per cycle
 * - Channel 5 (PA1): 10% duty cycle = 100μs HIGH, 900μs LOW per cycle
 * - Verify both channels show identical low duty cycle waveforms
 */
void PWM_Test_Duty_Cycle_Variations(void)
{
    uint16 duty_cycle;
    
    /* Test 2.1: Using percentage conversion utility */
    duty_cycle = Pwm_PercentageToDutyCycle(10); // 10% = 0x0CCC
    Pwm_SetDutyCycle(PWM_TEST_CHANNEL_4, duty_cycle);
    Pwm_SetDutyCycle(PWM_TEST_CHANNEL_5, duty_cycle);
}


/**
 * @brief Test idle state control and PWM restoration
 * @details Tests SetOutputToIdle() and subsequent PWM restoration using SetDutyCycle()
 * 
 * Test Sequence:
 * 1. Channel 6 (PA2): Force to HIGH idle state using TIM_ForcedOCxConfig()
 * 2. Channel 7 (PA3): Force to HIGH idle state using TIM_ForcedOCxConfig()  
 * 3. Attempt PWM restoration: SetDutyCycle(75%) should reinitialize PWM mode
 * 
 * Expected Results:
 * - PA2: HIGH during idle, then 75% PWM (750μs HIGH, 250μs LOW)
 * - PA3: HIGH during idle, then 75% PWM (750μs HIGH, 250μs LOW)
 * 
 * Known Issue: PWM restoration not working - channels remain in forced state
 */
void PWM_Test_Idle_State_Control(void)
{
    /* Test 3.1: Force channels to configured idle state (HIGH for Ch6&7) */
    Pwm_SetOutputToIdle(6);  // PA2 -> HIGH (idle state from config)
    Pwm_SetOutputToIdle(7);  // PA3 -> HIGH (idle state from config)
    
    /* Test 3.2: Attempt to restore PWM operation */
    Pwm_SetDutyCycle(6, 0x6000); // 75% duty cycle restoration attempt
    Pwm_SetDutyCycle(7, 0x6000); // 75% duty cycle restoration attempt
}

/**
 * @brief Test PWM notification system
 * @details Tests interrupt-based notifications on TIM2 channels with different edge types
 * 
 * Notification Configuration:
 * - Channel 4 (PA0): Rising edge notifications -> counter_notification_rising++
 * - Channel 5 (PA1): Falling edge notifications -> counter_notification_failing++  
 * - Channel 6 (PA2): Both edges notifications -> counter_notification_bothedge++
 * 
 * Expected Behavior:
 * - Each PWM edge generates interrupt calling respective notification function
 * - Counters increment based on PWM frequency and edge configuration
 * - At 1kHz PWM: Rising edge = 1000 interrupts/sec, Both edges = 2000 interrupts/sec
 * 
 * Hardware Requirements:
 * - TIM2 interrupts must be enabled in NVIC
 * - PWM signals must be active on respective channels
 */
void PWM_Test_Notifications(void)
{
    /* Test 4.1: Enable rising edge notification on PA0 */
    Pwm_EnableNotification(4, PWM_RISING_EDGE);
    
    /* Test 4.2: Enable falling edge notification on PA1 */
    Pwm_EnableNotification(5, PWM_FALLING_EDGE);
    
    /* Test 4.3: Enable both edges notification on PA2 */
    Pwm_EnableNotification(6, PWM_BOTH_EDGES);
}


/****************************************************************************************
*                              HELPER FUNCTIONS                                        *
****************************************************************************************/
