 /****************************************************************************************
*                                ADC_CFG.C                                              *
****************************************************************************************
* File Name   : Adc_Cfg.c
* Module      : Analog to Digital Converter (ADC)
* Description : AUTOSAR ADC driver configuration source file
* Version     : 3.1.0 - Redesigned for clarity and maintainability
* Date        : 24/06/2025
* Update      : 23/07/2025
* Author      : hoangphuc540202@gmail.com
* Github      : https://github.com/HoangPhuc02
****************************************************************************************/

/****************************************************************************************
*                                 INCLUDE FILES                                        *
****************************************************************************************/
#include "Adc_Cfg.h"
#include "Adc_Hw.h"
#include "Adc_Types.h"

/****************************************************************************************
*                                 CHANNEL CONFIGURATIONS                               *
****************************************************************************************/
/* Channel configuration for Group 1 */
static const Adc_ChannelDefType Adc_ChannelGroup1[] = 
{
    {
        .Adc_ChannelId          = 0,                          /* PA0 - ADC1_IN0 */
        .Adc_ChannelSampTime    = ADC_SAMPLING_TIME_DEFAULT
    },
};
#define ADC_CHANNEL_GROUP_1_SIZE            sizeof(Adc_ChannelGroup1)/sizeof(Adc_ChannelDefType)
#define ADC_CHANNEL_GROUP_1_NUM_OF_SAMPLE   1
// #define ADC_CHANNEL_GROUP_1_RESULT_SIZE     (ADC_CHANNEL_GROUP_1_NUM_OF_SAMPLE * ADC_CHANNEL_GROUP_1_SIZE)
Adc_ValueGroupType Adc_Group1_ResultBuffer[ADC_CHANNEL_GROUP_1_RESULT_SIZE];  


/****************************************************************************************
*                                 NOTIFICATION CALLBACKS                               *
****************************************************************************************/
/**
 * @brief Notification callback for Group 1
 * @return void
 */
__attribute__((weak)) void Adc_Group1_Notification(void)  
{
    /* User-defined notification handling for Group 1 */
    /* This can be used to signal completion to application */
}

/****************************************************************************************
*                                 GROUP CONFIGURATIONS                                 *
****************************************************************************************/

Adc_GroupDefType Adc_GroupConfig[ADC_MAX_GROUPS] = 
{
    /* Group 1: Single access, one-shot, software trigger */
    {
        .Adc_HwUnitId           = ADC_INSTANCE_1,                               /* ADC Hardware Unit 0 (ADC1) */
        .Adc_GroupId            = 0,                                /* Group 0 (internally Group 1) */
        .Adc_GroupPriority      = 1,                          /* Highest priority */
        .Adc_GroupAccessMode    = ADC_ACCESS_MODE_SINGLE,
        .Adc_ValueResultSize    = ADC_CHANNEL_GROUP_1_RESULT_SIZE, /*1 channels × 1 samples = 1 */
        .Adc_StreamNumSamples   = ADC_CHANNEL_GROUP_1_NUM_OF_SAMPLE, /* 1 sample per channel */

        .Adc_GroupConvMode      = ADC_CONV_MODE_ONESHOT,
        .Adc_GroupReplacement   = ADC_GROUP_REPL_ABORT_RESTART,
        .Adc_Status             = ADC_IDLE,
        .Adc_ResultAlignment    = ADC_ALIGN_RIGHT,
        .Adc_ChannelGroup       = (Adc_ChannelDefType*)Adc_ChannelGroup1,
        .Adc_NbrOfChannel       = ADC_CHANNEL_GROUP_1_SIZE,                           /* 1 channels: PA0  */
        .Adc_TriggerSource      = ADC_TRIGG_SRC_SW,
        .Adc_HwTriggerSignal    = ADC_HW_TRIG_RISING_EDGE,
        .Adc_HwTriggerTimer     = 0,
        .Adc_StreamBufferMode   = ADC_STREAM_BUFFER_CIRCULAR,

        .Adc_ValueResultPtr     = Adc_Group1_ResultBuffer,
        .Adc_SetupBufferFlag    = 0,
        .Adc_NotificationCb     = Adc_Group1_Notification,
        .Adc_NotificationEnable = ADC_NOTIFICATION_DISABLE,
        .Adc_InterruptType      = ADC_HW_DMA
    },
};

/****************************************************************************************
*                                 HARDWARE UNIT CONFIGURATIONS                         *
****************************************************************************************/

Adc_HwUnitDefType Adc_HwUnitConfig[ADC_MAX_HW_UNITS] = 
{
    /* ADC Hardware Unit 0 (ADC1) */
    {
        .AdcHW_UnitId           = 0,                               /* Hardware Unit 0 corresponds to ADC1 */
        .AdcHw_QueueEnable      = ADC_ENABLE_QUEUING,         /* Queue enabled */
        .AdcHw_PriorityEnable   = ADC_PRIORITY_IMPLEMENTATION,
        .AdcHw_DMAAvailable     = ADC_DMA_NOT_AVAILABLE,     /* DMA not available */
    },
};

/****************************************************************************************
*                                 CHANNEL CONFIGURATIONS                               *
****************************************************************************************/


Adc_ChannelDefType Adc_ChannelConfig[ADC_MAX_CHANNELS] = 
{
    /* Channel 0 - PA0 */
    {
        .Adc_ChannelId          = 0,
        .Adc_ChannelSampTime    = ADC_SAMPLING_TIME_DEFAULT
    },
};

/****************************************************************************************
*                                 MAIN CONFIGURATION                                   *
****************************************************************************************/
const Adc_ConfigType Adc_Config = 
{
    .HwUnits = Adc_HwUnitConfig,
    .NumHwUnits = ADC_HW_CONFIG_SIZE,
    .Groups = Adc_GroupConfig,
    .NumGroups = ADC_GROUP_CONFIG_SIZE,
};




/****************************************************************************************
*                                 END OF FILE                                          *
****************************************************************************************/
