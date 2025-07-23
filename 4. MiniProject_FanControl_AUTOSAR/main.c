/*
 * =====================================================
 *  AUTOSAR Fan Control System - Main Application
 * =====================================================
 *  Project: Temperature-Controlled Fan System
 *  Target: STM32F103C8T6 Blue Pill
 *  Author: [Student Name]
 *  Date: [Date]
 * =====================================================
 */

#include "IoHwAb.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_flash.h"

/* Temperature thresholds in Celsius */
// #define TEMP_LOW_THRESHOLD      20u    /* Below this: Fan OFF */
// #define TEMP_MEDIUM_THRESHOLD   40u    /* Above this: Fan 100% */

#define TEMP_LOW_THRESHOLD     1500u    /* Below this: Fan OFF */
#define TEMP_MEDIUM_THRESHOLD  2500u    /* Above this: Fan 100% */

/* Fan duty cycle percentages */
#define FAN_DUTY_OFF           0u      /* Fan stopped */
#define FAN_DUTY_MEDIUM        50u     /* Fan at 50% */
#define FAN_DUTY_HIGH          100u    /* Fan at 100% */

/* System timing */
#define TEMP_READ_INTERVAL_MS  1000u   /* Read temperature every 1 second */

/* Global variables */
static uint16 current_temperature = 0;
static uint8 current_fan_duty = 0;
static boolean led_status = FALSE;

/*
 * Function: Application_Init
 * Description: Initialize the application and all hardware abstraction layers
 * Parameters: None
 * Return: None
 */
void Application_Init(void)
{
    /* Initialize IoHwAb - this will initialize all MCAL drivers */
    IoHwAb_Init();
    
    /* Initial state: Fan OFF, LED OFF */
    IoHwAb_SetFanDuty(FAN_DUTY_OFF);
    IoHwAb_SetLed(FALSE);
}
/*
 * Function: Application_UpdateFanControl
 * Description: Update fan speed and LED status based on current temperature
 * Parameters: temperature - Current temperature in Celsius
 * Return: None
 */
void Application_UpdateFanControl(uint16 temperature)
{
    uint8 new_fan_duty = 0;
    boolean new_led_status = FALSE;
    
    /* Determine fan duty cycle based on temperature thresholds */
    if (temperature < TEMP_LOW_THRESHOLD)
    {
        /* Temperature below 30°C: Fan OFF */
        new_fan_duty = FAN_DUTY_OFF;
        new_led_status = FALSE;
    }
    else if (temperature < TEMP_MEDIUM_THRESHOLD)
    {
        /* Temperature 30-40°C: Fan at 50% */
        new_fan_duty = FAN_DUTY_MEDIUM;
        new_led_status = TRUE;
    }
    else
    {
        /* Temperature 40°C and above: Fan at 100% */
        new_fan_duty = FAN_DUTY_HIGH;
        new_led_status = TRUE;
    }
    
    /* Update fan duty cycle if changed */
    if (new_fan_duty != current_fan_duty)
    {
        current_fan_duty = new_fan_duty;
        IoHwAb_SetFanDuty(current_fan_duty);
    }
    
    /* Update LED status if changed */
    if (new_led_status != led_status)
    {
        led_status = new_led_status;
        IoHwAb_SetLed(led_status);
    }
}

/*
 * Function: main
 * Description: Main application entry point
 * Parameters: None
 * Return: int (never returns in embedded systems)
 */
int main(void)
{
    /* Initialize application and hardware */
    Application_Init();
    
    /* Main application loop */
    while (1)
    {
        /* Read current temperature from sensor */
        current_temperature = IoHwAb_ReadTemperature();
        
        /* Update fan control based on temperature */
        Application_UpdateFanControl(current_temperature);

        /* TODO: Add delay or use system timer for proper timing */
        /* Simple delay - replace with proper timer in production code */
        for (volatile uint32 i = 0; i < 1000000; i++)
        {
            /* Delay approximately 1 second (depends on CPU frequency) */
            __asm("nop");
        }
    }
    
    /* Should never reach here */
    return 0;
}

/*
 * Function: SystemClock_Config
 * Description: Configure the system clock (if needed)
 * Parameters: None
 * Return: None
 * Note: This might be handled by startup code or IoHwAb_Init()
 */
void SystemClock_Config(void)
{
    /* TODO: Implement system clock configuration if not handled elsewhere */
    /* For STM32F103C8T6:
     * - External crystal: 8MHz
     * - PLL to achieve 72MHz system clock
     * - Configure AHB, APB1, APB2 prescalers
     */
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




