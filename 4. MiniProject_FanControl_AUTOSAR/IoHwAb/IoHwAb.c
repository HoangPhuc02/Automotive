/*
 * =====================================================
 *  AUTOSAR IoHwAb Module - Implementation
 * =====================================================
 *  Module: I/O Hardware Abstraction Layer
 *  Project: Temperature-Controlled Fan System
 *  Target: STM32F103C8T6
 *  Author: [Student Name]
 *  Date: [Date]
 * =====================================================
 */

/*
 * =====================================================
 *  INCLUDES
 * =====================================================
 */
#include "IoHwAb.h"



/*
 * =====================================================
 *  LOCAL VARIABLES
 * =====================================================
 */

/* Module state */
static IoHwAb_StateType IoHwAb_ModuleState = IOHWAB_UNINITIALIZED;

/* Current values (for extended features) */
#ifdef IOHWAB_EXTENDED_FEATURES
static uint8 IoHwAb_CurrentFanDuty = 0u;
static boolean IoHwAb_CurrentLedState = FALSE;
#endif
Adc_GroupType AdcConf_AdcGroup_TemperatureSensor = 0; /* ADC group for temperature sensor */
/*
 * =====================================================
 *  LOCAL FUNCTION PROTOTYPES
 * =====================================================
 */

/*
 * Function: IoHwAb_ConvertAdcToTemperature
 * Description: Convert raw ADC value to temperature in Celsius
 * Parameters: adcValue - Raw ADC reading (0-4095)
 * Return: uint16 - Temperature in Celsius, or IOHWAB_TEMP_INVALID_VALUE on error
 */
static uint16 IoHwAb_ConvertAdcToTemperature(uint16 adcValue);

/*
 * Function: IoHwAb_ConvertPercentToPwm
 * Description: Convert percentage (0-100%) to PWM duty cycle value
 * Parameters: percent - Duty cycle percentage (0-100)
 * Return: uint16 - PWM duty cycle value for PWM driver
 */
static uint16 IoHwAb_ConvertPercentToPwm(uint8 percent);

/*
 * Function: IoHwAb_ValidateParameters
 * Description: Validate input parameters for IoHwAb functions
 * Parameters: functionId - Function ID for error reporting
 *             param - Parameter value to validate
 *             min - Minimum allowed value
 *             max - Maximum allowed value
 * Return: boolean - TRUE if valid, FALSE if invalid
 */
static boolean IoHwAb_ValidateParameters(uint8 functionId, uint32 param, uint32 min, uint32 max);

/*
 * =====================================================
 *  PUBLIC FUNCTION IMPLEMENTATIONS
 * =====================================================
 */

/*
 * Function: IoHwAb_Init
 * Description: Initialize the IoHwAb module and all underlying MCAL drivers
 */
void IoHwAb_Init(void)
{
    /* TODO: Initialize all MCAL drivers in correct order */
    
    /* 1. Initialize Port Driver first (GPIO configuration) */
    Port_Init(&PortCfg_Port);
    
    /* 2. Initialize DIO Driver */
    /* DIO driver usually doesn't need explicit initialization */
    
    /* 3. Initialize ADC Driver */
    Adc_Init(&Adc_Config);
    Adc_SetupResultBuffer(AdcConf_AdcGroup_TemperatureSensor, Adc_Group1_ResultBuffer);
    /* 4. Initialize PWM Driver */
    Pwm_Init(&Pwm_Config);
    
    /* Set initial states */
    IoHwAb_SetFanDuty(IOHWAB_FAN_DUTY_MIN);    /* Fan OFF initially */
    IoHwAb_SetLed(IOHWAB_LED_OFF);             /* LED OFF initially */

    // Adc_StartGroupConversion(AdcConf_AdcGroup_TemperatureSensor);
    
    /* Mark module as initialized */
    IoHwAb_ModuleState = IOHWAB_INITIALIZED;
}

/*
 * Function: IoHwAb_ReadTemperature
 * Description: Read current temperature from analog sensor
 */
uint16 IoHwAb_ReadTemperature(void)
{
    uint16 adcValue = 0;
    uint16 temperature = IOHWAB_TEMP_INVALID_VALUE;
    
    /* Check if module is initialized */
    if (IoHwAb_ModuleState != IOHWAB_INITIALIZED)
    {
        return IOHWAB_TEMP_INVALID_VALUE;
    }
    

     Adc_StartGroupConversion(AdcConf_AdcGroup_TemperatureSensor);
    
    // Wait for conversion to complete
    while (Adc_GetGroupStatus(AdcConf_AdcGroup_TemperatureSensor) != ADC_STREAM_COMPLETED)
    {
        // Wait or add timeout
    }
    
    // Read ADC result
    if (Adc_ReadGroup(AdcConf_AdcGroup_TemperatureSensor, &temperature) == E_OK)
    {
        // temperature = IoHwAb_ConvertAdcToTemperature(adcValue);
    }
   //TODO: BUG READ GROUP NOT GET THE VALUE RESULT
    
    return temperature;
}

/*
 * Function: IoHwAb_SetFanDuty
 * Description: Set fan speed by adjusting PWM duty cycle
 */
void IoHwAb_SetFanDuty(uint8 percent)
{
    uint16 pwmDutyValue = 0;
    
    /* Check if module is initialized */
    if (IoHwAb_ModuleState != IOHWAB_INITIALIZED)
    {
        return;
    }
    
    /* Validate input parameter */
    if (IoHwAb_ValidateParameters(0x03, percent, IOHWAB_FAN_DUTY_MIN, IOHWAB_FAN_DUTY_MAX) == FALSE)
    {
        return;
    }
    
    /* Convert percentage to PWM duty cycle value */
    pwmDutyValue = IoHwAb_ConvertPercentToPwm(percent);
    
    /* Set PWM duty cycle */
    Pwm_SetDutyCycle(IOHWAB_PWM_CHANNEL_FAN, pwmDutyValue);
    
}

/*
 * Function: IoHwAb_SetLed
 * Description: Control status LED
 */
void IoHwAb_SetLed(boolean state)
{
    Dio_LevelType dioLevel = STD_LOW;
    
    /* Check if module is initialized */
    if (IoHwAb_ModuleState != IOHWAB_INITIALIZED)
    {
        return;
    }
    
    /* Convert boolean to DIO level */
    /* Note: LED might be active low or active high depending on hardware */
    /* Assuming active low (LED ON = LOW) */
    dioLevel = (state == TRUE) ? STD_LOW : STD_HIGH; // Active low logic
    
    /* Set LED state */
    Dio_WriteChannel(IOHWAB_DIO_CHANNEL_LED, dioLevel);
    
}



/*
 * =====================================================
 *  LOCAL FUNCTION IMPLEMENTATIONS
 * =====================================================
 */

/*
 * Function: IoHwAb_ConvertAdcToTemperature
 * Description: Convert raw ADC value to temperature in Celsius
 */
static uint16 IoHwAb_ConvertAdcToTemperature(uint16 adcValue)
{
    uint32 millivolts = 0;
    uint16 temperature = 0;
    
    #if (IOHWAB_TEMP_SENSOR_TYPE == TEMP_SENSOR_LM35)
    
    /* LM35 Temperature Sensor Conversion */
    /* Formula: Temp(°C) = (ADC_Value * Vref_mV) / (ADC_Resolution * mV_per_°C) */
    /* LM35: 10mV/°C linear response */
    
    millivolts = ((uint32)adcValue * IOHWAB_ADC_VREF_MV) / IOHWAB_ADC_RESOLUTION;
    temperature = (uint16)(millivolts / IOHWAB_LM35_MV_PER_CELSIUS);
    
    #elif (IOHWAB_TEMP_SENSOR_TYPE == TEMP_SENSOR_NTC)
    
    /* NTC Thermistor Conversion */
    /* TODO: Implement NTC conversion using Steinhart-Hart equation */
    /* or lookup table for better accuracy */
    
    /* Simplified linear approximation (not recommended for production) */
    /* This is just a placeholder - implement proper NTC conversion */
    millivolts = ((uint32)adcValue * IOHWAB_ADC_VREF_MV) / IOHWAB_ADC_RESOLUTION;
    
    /* Simple linear mapping (INCORRECT for NTC - fix this!) */
    /* Assuming 25°C at mid-range voltage for demonstration */
    if (millivolts > 1650)  /* Above 1.65V */
    {
        temperature = 25 - ((millivolts - 1650) / 50);  /* Rough approximation */
    }
    else
    {
        temperature = 25 + ((1650 - millivolts) / 50);  /* Rough approximation */
    }
    
    #else
    #error "Unsupported temperature sensor type"
    #endif
    
    return temperature;
}

/*
 * Function: IoHwAb_ConvertPercentToPwm
 * Description: Convert percentage to PWM duty cycle value
 */
static uint16 IoHwAb_ConvertPercentToPwm(uint8 percent)
{
    uint32 pwmValue = 0;
    
    /* Convert percentage (0-100%) to PWM value (0x0000-0x8000) */
    /* Formula: PWM_Value = (Percentage * PWM_MAX_VALUE) / 100 */
    
    pwmValue = ((uint32)percent * IOHWAB_PWM_MAX_VALUE) / IOHWAB_PERCENT_MAX;
    
    /* Ensure result doesn't exceed maximum PWM value */
    if (pwmValue > IOHWAB_PWM_MAX_VALUE)
    {
        pwmValue = IOHWAB_PWM_MAX_VALUE;
    }
    
    return (uint16)pwmValue;
}

/*
 * Function: IoHwAb_ValidateParameters
 * Description: Validate input parameters
 */
static boolean IoHwAb_ValidateParameters(uint8 functionId, uint32 param, uint32 min, uint32 max)
{
    /* Suppress unused parameter warning */
    (void)functionId;
    
    /* Check parameter range */
    if ((param < min) || (param > max))
    {
        /* TODO: Report error with Development Error Tracer (DET) if available */
        /* Det_ReportError(IOHWAB_MODULE_ID, 0, functionId, IOHWAB_E_PARAM_VALUE); */
        return FALSE;
    }
    
    return TRUE;
}

/*
 * =====================================================
 *  IMPLEMENTATION NOTES FOR STUDENTS
 * =====================================================
 * 
 * 1. MCAL Driver Integration:
 *    - Replace placeholder code with actual MCAL function calls
 *    - Check MCAL driver documentation for correct API usage
 *    - Handle return values and error conditions from MCAL drivers
 * 
 * 2. Temperature Sensor Calibration:
 *    - LM35: Should be linear, but verify with actual hardware
 *    - NTC: Requires complex non-linear conversion
 *    - Consider temperature offset calibration for accuracy
 * 
 * 3. Error Handling:
 *    - Implement proper error reporting (DET integration)
 *    - Add timeout mechanisms for ADC conversions
 *    - Validate all inputs and handle edge cases
 * 
 * 4. Performance Optimization:
 *    - Use DMA for continuous ADC readings if needed
 *    - Implement averaging/filtering for stable temperature readings
 *    - Consider using interrupt-driven ADC for better responsiveness
 * 
 * 5. Hardware Considerations:
 *    - Verify LED polarity (active high/low)
 *    - Check PWM frequency requirements for fan control
 *    - Add proper voltage scaling for different reference voltages
 * 
 * 6. Configuration Management:
 *    - Move magic numbers to configuration files
 *    - Make sensor type selectable through configuration
 *    - Add compile-time checks for configuration validity
 * 
 * =====================================================
 */
