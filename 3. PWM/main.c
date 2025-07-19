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
#include <string.h>

/* Test Configuration */
#define PWM_TEST_CHANNEL_0      0    // Test channel ID
#define PWM_TEST_CHANNEL_1      1    // Second test channel (if available)

/* Test constants */
#define TEST_DELAY_SHORT        100  // Short delay in ms
#define TEST_DELAY_MEDIUM       500  // Medium delay in ms
#define TEST_DELAY_LONG         1000 // Long delay in ms

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
    Timer_Init();
    NVIC_Config();
    Port_Init(&PortCfg_Port);
    Pwm_Init(&Pwm_Config);
    
    /* PWM AUTOSAR API Test Suite */
    while (1)
    {
        /* Test 1: Basic PWM Operations */
        PWM_Test_Basic_Operations();
        Test_Delay_ms(1000);
        
        /* Test 2: Duty Cycle Variations */
        PWM_Test_Duty_Cycle_Variations();
        Test_Delay_ms(1000);
        
        /* Test 3: Output State Reading */
        PWM_Test_Output_State_Reading();
        Test_Delay_ms(1000);
        
        /* Test 4: Idle State Control */
        PWM_Test_Idle_State_Control();
        Test_Delay_ms(1000);
        
        /* Test 5: Notifications */
        PWM_Test_Notifications();
        Test_Delay_ms(1000);
        
        /* Test 6: Edge Cases */
        PWM_Test_Edge_Cases();
        Test_Delay_ms(2000);
        
        /* Test cycle complete - repeat */
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
* Test Suite Description:
* 
* Test 1: Basic Operations
*   - Set 50%, 25%, 75% duty cycles
*   - Verify PWM signal generation
*
* Test 2: Duty Cycle Variations  
*   - Test percentage to duty cycle conversion
*   - Test duty cycle to percentage conversion
*   - Verify utility functions
*
* Test 3: Output State Reading
*   - Test Pwm_GetOutputState() with 0%, 50%, 100% duty cycles
*   - Verify state reading accuracy
*
* Test 4: Idle State Control
*   - Test Pwm_SetOutputToIdle() functionality
*   - Verify output goes to idle state
*   - Test resume from idle state
*
* Test 5: Notifications
*   - Test rising edge notifications
*   - Test falling edge notifications  
*   - Test both edges notifications
*   - Test notification disable
*
* Test 6: Edge Cases
*   - Test boundary values (0%, 100%)
*   - Test Pwm_SetPeriodAndDuty() 
*   - Test multiple channels
*   - Error condition handling
*
* Expected Results:
*   - All tests should pass without errors
*   - PWM signals should be visible on oscilloscope
*   - LED indicators show test results
****************************************************************************************/

/**
 * @brief Test basic PWM operations
 * @details Tests initialization, basic duty cycle setting
 */
void PWM_Test_Basic_Operations(void)
{
    /* Test 1.1: Set 50% duty cycle */
    Pwm_SetDutyCycle(PWM_TEST_CHANNEL_0, 0x4000); // 50% duty cycle
    Test_Delay_ms(TEST_DELAY_MEDIUM);
    
    /* Test 1.2: Set 25% duty cycle */
    Pwm_SetDutyCycle(PWM_TEST_CHANNEL_0, 0x2000); // 25% duty cycle  
    Test_Delay_ms(TEST_DELAY_MEDIUM);
    
    /* Test 1.3: Set 75% duty cycle */
    Pwm_SetDutyCycle(PWM_TEST_CHANNEL_0, 0x6000); // 75% duty cycle
    Test_Delay_ms(TEST_DELAY_MEDIUM);
}

/**
 * @brief Test duty cycle variations
 * @details Tests different duty cycle values and utility functions
 */
void PWM_Test_Duty_Cycle_Variations(void)
{
    uint16 duty_cycle;
    // uint8 percentage;
    
    /* Test 2.1: Using percentage conversion */
    duty_cycle = Pwm_PercentageToDutyCycle(10); // 10%
    Pwm_SetDutyCycle(PWM_TEST_CHANNEL_0, duty_cycle);
    Test_Delay_ms(300);
    
    duty_cycle = Pwm_PercentageToDutyCycle(30); // 30%
    Pwm_SetDutyCycle(PWM_TEST_CHANNEL_0, duty_cycle);
    Test_Delay_ms(300);
    
    duty_cycle = Pwm_PercentageToDutyCycle(60); // 60%
    Pwm_SetDutyCycle(PWM_TEST_CHANNEL_0, duty_cycle);
    Test_Delay_ms(300);
    
    duty_cycle = Pwm_PercentageToDutyCycle(90); // 90%
    Pwm_SetDutyCycle(PWM_TEST_CHANNEL_0, duty_cycle);
    Test_Delay_ms(300);
    
    /* Test 2.2: Verify conversion back to percentage */
    // percentage = Pwm_DutyCycleToPercentage(0x4000); // Should be 50%
}

/**
 * @brief Test output state reading
 * @details Tests Pwm_GetOutputState API
 */
void PWM_Test_Output_State_Reading(void)
{
    volatile Pwm_OutputStateType output_state;
    
    /* Test 3.1: Set 0% duty cycle and check state */
    Pwm_SetDutyCycle(0, 0x0000); // 0% duty cycle
    Test_Delay_ms(100);
    output_state = Pwm_GetOutputState(0);
    // Should be PWM_LOW for 0% duty cycle
    
    /* Test 3.2: Set 100% duty cycle and check state */
    Pwm_SetDutyCycle(0, 0x8000); // 100% duty cycle
    Test_Delay_ms(100);
    output_state = Pwm_GetOutputState(0);
    // Should be PWM_HIGH for 100% duty cycle
    
    /* Test 3.3: Set 50% duty cycle and check state */
    Pwm_SetDutyCycle(0, 0x4000); // 50% duty cycle
    Test_Delay_ms(100);
    output_state = Pwm_GetOutputState(0);
    // State can be either HIGH or LOW depending on timing
}

/**
 * @brief Test idle state control
 * @details Tests Pwm_SetOutputToIdle API
 */
void PWM_Test_Idle_State_Control(void)
{
    /* Test 4.1: Set normal PWM operation */
    Pwm_SetDutyCycle(0, 0x4000); // 50% duty cycle
    Test_Delay_ms(500);
    
    /* Test 4.2: Set output to idle state */
    Pwm_SetOutputToIdle(0);
    Test_Delay_ms(500);
    
    /* Test 4.3: Resume normal operation */
    Pwm_SetDutyCycle(0, 0x6000); // 75% duty cycle
    Test_Delay_ms(500);
}

/**
 * @brief Test PWM notifications
 * @details Tests notification enable/disable
 */
void PWM_Test_Notifications(void)
{
    /* Test 5.1: Enable rising edge notification */
    Pwm_EnableNotification(0, PWM_RISING_EDGE);
    Pwm_SetDutyCycle(0, 0x4000); // 50% duty cycle
    Test_Delay_ms(500);
    
    /* Test 5.2: Enable falling edge notification */
    Pwm_EnableNotification(0, PWM_FALLING_EDGE);
    Test_Delay_ms(500);
    
    /* Test 5.3: Enable both edges notification */
    Pwm_EnableNotification(0, PWM_BOTH_EDGES);
    Test_Delay_ms(500);
    
    /* Test 5.4: Disable notifications */
    Pwm_DisableNotification(0);
    Test_Delay_ms(500);
}

/**
 * @brief Test edge cases and error conditions
 * @details Tests boundary values and error handling
 */
void PWM_Test_Edge_Cases(void)
{
    /* Test 6.1: Minimum duty cycle (0%) */
    Pwm_SetDutyCycle(0, 0x0000);
    Test_Delay_ms(300);
    
    /* Test 6.2: Maximum duty cycle (100%) */
    Pwm_SetDutyCycle(0, 0x8000);
    Test_Delay_ms(300);
    
    /* Test 6.3: Test period and duty setting */
    Pwm_SetPeriodAndDuty(0, 1000, 0x4000); // Period 1000, 50% duty
    Test_Delay_ms(300);
    
    /* Test 6.4: Test multiple channels if available */
    if (PWM_MAX_CHANNELS > 1)
    {
        Pwm_SetDutyCycle(1, 0x2000); // Channel 1: 25%
        Test_Delay_ms(300);
    }
}

/****************************************************************************************
*                              HELPER FUNCTIONS                                        *
****************************************************************************************/

/**
 * @brief Test delay function
 */
void Test_Delay_ms(uint16_t ms)
{
    delay(ms);
}
