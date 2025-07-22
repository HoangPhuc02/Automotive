/*
 * =====================================================
 *  AUTOSAR ADC Driver - Interface Header
 * =====================================================
 */

#ifndef ADC_H
#define ADC_H

#include "Std_Types.h"

/* ADC Group Type */
typedef uint8 Adc_GroupType;

/* ADC Channel Type */
typedef uint8 Adc_ChannelType;

/* ADC Value Type */
typedef uint16 Adc_ValueType;

/* ADC Status Type */
typedef enum
{
    ADC_IDLE = 0,
    ADC_BUSY,
    ADC_COMPLETED,
    ADC_STREAM_COMPLETED
} Adc_StatusType;

/* ADC Configuration Type */
typedef struct
{
    Adc_GroupType GroupId;
    Adc_ChannelType ChannelId;
    /* Add more configuration parameters as needed */
} Adc_ConfigType;

/* ADC Group Configuration IDs */
#define AdcConf_AdcGroup_TemperatureSensor    0u

/* Function Prototypes */
void Adc_Init(const Adc_ConfigType* ConfigPtr);
Std_ReturnType Adc_StartGroupConversion(Adc_GroupType Group);
void Adc_StopGroupConversion(Adc_GroupType Group);
Std_ReturnType Adc_ReadGroup(Adc_GroupType Group, Adc_ValueType* DataBufferPtr);
Adc_StatusType Adc_GetGroupStatus(Adc_GroupType Group);
Adc_ValueType Adc_ReadChannel(Adc_ChannelType ChannelId);

/* External Configuration Reference */
extern const Adc_ConfigType Adc_Config;

#endif /* ADC_H */
