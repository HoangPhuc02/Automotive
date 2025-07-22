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

/* Temperature thresholds in Celsius */
#define TEMP_LOW_THRESHOLD      30u    /* Below this: Fan OFF */
#define TEMP_MEDIUM_THRESHOLD   40u    /* Above this: Fan 100% */

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
    
    /* TODO: Add any additional application initialization here */
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
 * Function: Application_PrintStatus
 * Description: Print current system status (for debugging)
 * Parameters: None
 * Return: None
 */
void Application_PrintStatus(void)
{
    /* TODO: Implement UART/SWO debugging output if needed */
    /* printf("Temp: %d°C, Fan: %d%%, LED: %s\n", 
             current_temperature, current_fan_duty, 
             led_status ? "ON" : "OFF"); */
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
        
        /* Optional: Print status for debugging */
        Application_PrintStatus();
        
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
}

/* Error handler */
void Error_Handler(void)
{
    /* TODO: Implement error handling */
    /* In case of error:
     * - Turn off fan for safety
     * - Flash LED to indicate error
     * - Enter infinite loop or reset system
     */
    
    /* Safety: Turn off fan */
    IoHwAb_SetFanDuty(0);
    
    /* Flash LED to indicate error */
    while (1)
    {
        IoHwAb_SetLed(TRUE);
        for (volatile uint32 i = 0; i < 500000; i++) __asm("nop");
        IoHwAb_SetLed(FALSE);
        for (volatile uint32 i = 0; i < 500000; i++) __asm("nop");
    }
}

/*
 * =====================================================
 *  TODO List for Students:
 * =====================================================
 * 
 * 1. Implement proper timing mechanism instead of busy-wait loops
 *    - Use SysTick timer or other timer for precise timing
 *    - Consider using RTOS if available
 * 
 * 2. Add temperature filtering
 *    - Implement moving average filter to reduce noise
 *    - Add hysteresis to prevent oscillation at thresholds
 * 
 * 3. Add safety features
 *    - Temperature sensor failure detection
 *    - Fan failure detection (current sensing)
 *    - Emergency shutdown procedures
 * 
 * 4. Add debugging/monitoring features
 *    - UART output for monitoring temperature and fan status
 *    - SWO debug output
 *    - Consider adding potentiometer for manual fan control
 * 
 * 5. Optimize power consumption
 *    - Use sleep modes when possible
 *    - Optimize ADC conversion timing
 * 
 * 6. Add user interface (optional)
 *    - Multiple LEDs for different status
 *    - Buzzer for temperature alarms
 *    - LCD display for temperature reading
 * 
 * =====================================================
 */
