/*
 * =====================================================
 *  AUTOSAR ADC Driver Configuration
 * =====================================================
 */

#include "Adc.h"

/* ADC Configuration for Temperature Sensor */
const Adc_ConfigType Adc_Config = 
{
    .GroupId = AdcConf_AdcGroup_TemperatureSensor,
    .ChannelId = 0    /* ADC Channel 0 (PA0) */
};
