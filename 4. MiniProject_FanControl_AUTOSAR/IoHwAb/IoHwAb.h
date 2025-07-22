/*
 * =====================================================
 *  AUTOSAR IoHwAb Module - Interface Header
 * =====================================================
 *  Module: I/O Hardware Abstraction Layer
 *  Project: Temperature-Controlled Fan System
 *  Target: STM32F103C8T6
 *  Author: [Student Name]
 *  Date: [Date]
 * =====================================================
 */
/****************************************************************************************
*                       AUTOSAR IoHwAb Module - Interface Header                       *
****************************************************************************************
* File Name   : IoHwAb.h
* Module      : I/O Hardware Abstraction Layer
* Description : AUTOSAR IoHwAb driver implementation file
* Version     : 1.0.0
* Date        : 24/06/2025
* Author      : hoangphuc540202@gmail.com
* Github      : https://github.com/HoangPhuc02
****************************************************************************************/
#ifndef IOHWAB_H
#define IOHWAB_H

/*
 * =====================================================
 *  INCLUDES
 * =====================================================
 */
#include "Std_Types.h"      /* AUTOSAR standard types */

/*
 * =====================================================
 *  TYPE DEFINITIONS
 * =====================================================
 */

/* IoHwAb module state */
typedef enum
{
    IOHWAB_UNINITIALIZED = 0,
    IOHWAB_INITIALIZED   = 1
} IoHwAb_StateType;

/* Temperature sensor type */
typedef enum
{
    TEMP_SENSOR_LM35 = 0,      /* LM35 linear temperature sensor */
    TEMP_SENSOR_NTC  = 1       /* NTC thermistor */
} IoHwAb_TempSensorType;

/*
 * =====================================================
 *  CONSTANTS AND MACROS
 * =====================================================
 */

/* Module version information */
#define IOHWAB_VENDOR_ID                    1u
#define IOHWAB_MODULE_ID                    255u    /* IoHwAb doesn't have standard ID */
#define IOHWAB_AR_RELEASE_MAJOR_VERSION     4u
#define IOHWAB_AR_RELEASE_MINOR_VERSION     2u
#define IOHWAB_AR_RELEASE_REVISION_VERSION  2u
#define IOHWAB_SW_MAJOR_VERSION             1u
#define IOHWAB_SW_MINOR_VERSION             0u
#define IOHWAB_SW_PATCH_VERSION             0u

/* Hardware pin assignments */
#define IOHWAB_TEMP_SENSOR_PIN              0u      /* PA0 - ADC input */
#define IOHWAB_FAN_PWM_PIN                  8u      /* PA8 - PWM output */
#define IOHWAB_LED_STATUS_PIN               13u     /* PC13 - GPIO output */

/* Temperature sensor specifications */
#define IOHWAB_TEMP_SENSOR_TYPE             TEMP_SENSOR_LM35
#define IOHWAB_TEMP_MIN_CELSIUS             -10     /* Minimum measurable temperature */
#define IOHWAB_TEMP_MAX_CELSIUS             100     /* Maximum measurable temperature */
#define IOHWAB_TEMP_INVALID_VALUE           0xFFFF  /* Invalid temperature reading */

/* Fan control specifications */
#define IOHWAB_FAN_DUTY_MIN                 0u      /* Minimum duty cycle (%) */
#define IOHWAB_FAN_DUTY_MAX                 100u    /* Maximum duty cycle (%) */
#define IOHWAB_FAN_PWM_FREQUENCY_HZ         10000u  /* PWM frequency: 10kHz */

/* LED control */
#define IOHWAB_LED_OFF                      FALSE
#define IOHWAB_LED_ON                       TRUE

/*
 * =====================================================
 *  FUNCTION PROTOTYPES
 * =====================================================
 */

/*
 * Function: IoHwAb_Init
 * Service ID: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Non-reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Initialize the IoHwAb module and all underlying MCAL drivers.
 *              This function must be called before any other IoHwAb function.
 */
void IoHwAb_Init(void);

/*
 * Function: IoHwAb_ReadTemperature
 * Service ID: 0x02
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: uint16 - Current temperature in Celsius (0-100°C)
 *                       Returns IOHWAB_TEMP_INVALID_VALUE on error
 * Description: Read the current temperature from the analog temperature sensor.
 *              Converts ADC reading to temperature value in Celsius.
 */
uint16 IoHwAb_ReadTemperature(void);

/*
 * Function: IoHwAb_SetFanDuty
 * Service ID: 0x03
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): percent - Fan duty cycle percentage (0-100%)
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Set the fan speed by adjusting PWM duty cycle.
 *              0% = Fan OFF, 100% = Fan at maximum speed.
 */
void IoHwAb_SetFanDuty(uint8 percent);

/*
 * Function: IoHwAb_SetLed
 * Service ID: 0x04
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): state - LED state (TRUE = ON, FALSE = OFF)
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Control the status LED to indicate fan operation state.
 */
void IoHwAb_SetLed(boolean state);

/*
 * Function: IoHwAb_GetModuleState
 * Service ID: 0x05
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: IoHwAb_StateType - Current module state
 * Description: Get the current initialization state of the IoHwAb module.
 */
IoHwAb_StateType IoHwAb_GetModuleState(void);

/*
 * =====================================================
 *  OPTIONAL EXTENDED FUNCTIONS (for advanced features)
 * =====================================================
 */

#ifdef IOHWAB_EXTENDED_FEATURES

/*
 * Function: IoHwAb_ReadRawAdc
 * Description: Read raw ADC value without temperature conversion (for debugging)
 * Return: uint16 - Raw ADC value (0-4095 for 12-bit ADC)
 */
uint16 IoHwAb_ReadRawAdc(void);

/*
 * Function: IoHwAb_GetFanDuty
 * Description: Get current fan duty cycle setting
 * Return: uint8 - Current duty cycle percentage (0-100%)
 */
uint8 IoHwAb_GetFanDuty(void);

/*
 * Function: IoHwAb_GetLedState
 * Description: Get current LED state
 * Return: boolean - Current LED state (TRUE = ON, FALSE = OFF)
 */
boolean IoHwAb_GetLedState(void);

/*
 * Function: IoHwAb_SelfTest
 * Description: Perform self-test of all hardware components
 * Return: boolean - TRUE if all tests pass, FALSE if any test fails
 */
boolean IoHwAb_SelfTest(void);

#endif /* IOHWAB_EXTENDED_FEATURES */

/*
 * =====================================================
 *  CONFIGURATION VALIDATION
 * =====================================================
 */

/* Validate temperature sensor configuration */
#if (IOHWAB_TEMP_SENSOR_TYPE != TEMP_SENSOR_LM35) && (IOHWAB_TEMP_SENSOR_TYPE != TEMP_SENSOR_NTC)
    #error "Invalid temperature sensor type configuration"
#endif

/* Validate pin assignments */
#if (IOHWAB_TEMP_SENSOR_PIN > 15) || (IOHWAB_FAN_PWM_PIN > 15) || (IOHWAB_LED_STATUS_PIN > 15)
    #error "Invalid pin configuration - pins must be 0-15"
#endif

/* Validate temperature range */
#if (IOHWAB_TEMP_MIN_CELSIUS >= IOHWAB_TEMP_MAX_CELSIUS)
    #error "Invalid temperature range configuration"
#endif

/* Validate fan duty cycle range */
#if (IOHWAB_FAN_DUTY_MIN >= IOHWAB_FAN_DUTY_MAX)
    #error "Invalid fan duty cycle range configuration"
#endif

#endif /* IOHWAB_H */

/*
 * =====================================================
 *  IMPLEMENTATION NOTES FOR STUDENTS
 * =====================================================
 * 
 * 1. IoHwAb Layer Responsibilities:
 *    - Provide hardware-independent interface to application
 *    - Handle unit conversions (ADC counts to °C, % to PWM duty)
 *    - Call only MCAL layer functions, never access registers directly
 *    - No application logic (like temperature thresholds)
 * 
 * 2. Temperature Conversion:
 *    - LM35: 10mV/°C, linear response
 *    - Formula: Temp(°C) = (ADC_Value * Vref * 100) / ADC_Resolution
 *    - For 3.3V Vref, 12-bit ADC: Temp = (ADC * 3.3 * 100) / 4095
 * 
 * 3. PWM Duty Cycle Conversion:
 *    - Input: 0-100% (percentage)
 *    - Output: PWM driver expects 0x0000-0x8000 (16-bit)
 *    - Formula: PWM_Value = (Percentage * 0x8000) / 100
 * 
 * 4. Error Handling:
 *    - Check if module is initialized before processing requests
 *    - Validate input parameters (range checking)
 *    - Return error codes or invalid values for failures
 *    - Handle MCAL driver errors gracefully
 * 
 * 5. Thread Safety:
 *    - Mark functions as reentrant where appropriate
 *    - Protect shared resources if using RTOS
 *    - Consider atomic operations for critical sections
 * 
 * =====================================================
 */
