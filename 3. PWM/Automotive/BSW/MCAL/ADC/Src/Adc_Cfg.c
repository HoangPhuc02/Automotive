/****************************************************************************************
*                                ADC_CFG.C                                              *
****************************************************************************************
* File Name   : Adc_Cfg.c
* Module      : Analog to Digital Converter (ADC)
* Description : AUTOSAR ADC driver configuration source file
* Version     : 2.0.0 - Redesigned for clarity and maintainability
* Date        : 24/06/2025
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
*                                 LOCAL MACROS                                         *
****************************************************************************************/
#define ADC_CHANNEL_GROUP_1_SIZE    (sizeof(Adc_ChannelGroup1)/sizeof(Adc_ChannelDefType))
#define ADC_CHANNEL_GROUP_2_SIZE    (sizeof(Adc_ChannelGroup2)/sizeof(Adc_ChannelDefType))
#define ADC_CHANNEL_GROUP_3_SIZE    (sizeof(Adc_ChannelGroup3)/sizeof(Adc_ChannelDefType))

/****************************************************************************************
*                                 CHANNEL CONFIGURATIONS                               *
****************************************************************************************/
/* Channel configuration for Group 1 - Temperature Sensor */
static const Adc_ChannelDefType Adc_ChannelGroup1[] = 
{
    {
        .Adc_ChannelId = 0,                          /* PA0 - ADC1_IN0 */
        .Adc_ChannelSampTime = ADC_SAMPLING_TIME_DEFAULT
    },
    {
        .Adc_ChannelId = 1,                          /* PA1 - ADC1_IN1 */
        .Adc_ChannelSampTime = ADC_SAMPLING_TIME_DEFAULT
    }
};

/* Channel configuration for Group 2 - Voltage Monitoring */
static const Adc_ChannelDefType Adc_ChannelGroup2[] = 
{
    {
        .Adc_ChannelId = 2,                          /* PA2 - ADC1_IN2 */
        .Adc_ChannelSampTime = ADC_SAMPLING_TIME_DEFAULT
    },
    {
        .Adc_ChannelId = 3,                          /* PA3 - ADC1_IN3 */
        .Adc_ChannelSampTime = ADC_SAMPLING_TIME_DEFAULT
    }
};

/* Channel configuration for Group 3 - Multi-channel Streaming */
static const Adc_ChannelDefType Adc_ChannelGroup3[] = 
{
    {
        .Adc_ChannelId = 4,                          /* PA4 - ADC1_IN4 */
        .Adc_ChannelSampTime = ADC_SAMPLING_TIME_DEFAULT
    },
    {
        .Adc_ChannelId = 5,                          /* PA5 - ADC1_IN5 */
        .Adc_ChannelSampTime = ADC_SAMPLING_TIME_DEFAULT
    },
    {
        .Adc_ChannelId = 6,                          /* PA6 - ADC1_IN6 */
        .Adc_ChannelSampTime = ADC_SAMPLING_TIME_DEFAULT
    },
    {
        .Adc_ChannelId = 7,                          /* PA7 - ADC1_IN7 */
        .Adc_ChannelSampTime = ADC_SAMPLING_TIME_DEFAULT
    }
};

/****************************************************************************************
*                                 RESULT BUFFER CONFIGURATIONS                         *
****************************************************************************************/
/* Result buffers for each group */
static Adc_ValueGroupType Adc_Group1_ResultBuffer[2 * 4];      /* 2 channels, 4 samples */
static Adc_ValueGroupType Adc_Group2_ResultBuffer[2 * 3];      /* 2 channels, 3 samples */
static Adc_ValueGroupType Adc_Group3_ResultBuffer[4 * 5];      /* 4 channels, 5 samples */

/****************************************************************************************
*                                 NOTIFICATION CALLBACKS                               *
****************************************************************************************/
/**
 * @brief Notification callback for Group 1
 * @details Called when Group 1 conversion is completed
 */
void Adc_Group1_Notification(void)  
{
    /* User-defined notification handling for Group 1 */
    /* This can be used to signal completion to application */
}

/**
 * @brief Notification callback for Group 2
 * @details Called when Group 2 conversion is completed
 */
void Adc_Group2_Notification(void) 
{
    /* User-defined notification handling for Group 2 */
    /* This can be used to signal completion to application */
}

/**
 * @brief Notification callback for Group 3
 * @details Called when Group 3 conversion is completed
 */
void Adc_Group3_Notification(void) 
{
    /* User-defined notification handling for Group 3 */
    /* This can be used to signal completion to application */
}
/****************************************************************************************
*                                 GROUP CONFIGURATIONS                                 *
****************************************************************************************/
Adc_GroupDefType Adc_GroupConfig[ADC_MAX_GROUPS] = 
{
    /* Group 1: Temperature Sensor - Single access, continuous, software trigger */
    {
        .Adc_HwUnitId = ADC_INSTANCE_1,
        .Adc_GroupId = 0,
        .Adc_GroupPriority = 1,                      /* Highest priority */
        .Adc_GroupAccessMode = ADC_ACCESS_MODE_SINGLE,
        .Adc_GroupConvMode = ADC_CONV_MODE_CONTINUOUS,
        .Adc_GroupReplacement = ADC_GROUP_REPL_ABORT_RESTART,
        .Adc_Status = ADC_IDLE,
        .Adc_ResultAlignment = ADC_ALIGN_RIGHT,
        .Adc_ChannelGroup = (Adc_ChannelDefType*)Adc_ChannelGroup1,
        .Adc_NbrOfChannel = ADC_CHANNEL_GROUP_1_SIZE,
        .Adc_TriggerSource = ADC_TRIGG_SRC_SW,
        .Adc_HwTriggerSignal = ADC_HW_TRIG_RISING_EDGE,
        .Adc_HwTriggerTimer = 0,
        .Adc_StreamBufferMode = ADC_STREAM_BUFFER_LINEAR,
        .Adc_StreamNumSamples = 4,
        .Adc_ValueResultPtr = Adc_Group1_ResultBuffer,
        .Adc_ValueResultSize = 8,                    /* 2 channels × 4 samples */
        .Adc_SetupBufferFlag = 0,
        .Adc_NotificationCb = Adc_Group1_Notification,
        .Adc_NotificationEnable = ADC_NOTIFICATION_ENABLE,
        .Adc_InterruptType = ADC_HW_DMA
    },
    
    /* Group 2: Voltage Monitoring - Single access, continuous, software trigger */
    {
        .Adc_HwUnitId = ADC_INSTANCE_1,
        .Adc_GroupId = 1,
        .Adc_GroupPriority = 2,                      /* Medium priority */
        .Adc_GroupAccessMode = ADC_ACCESS_MODE_SINGLE,
        .Adc_GroupConvMode = ADC_CONV_MODE_CONTINUOUS,
        .Adc_GroupReplacement = ADC_GROUP_REPL_ABORT_RESTART,
        .Adc_Status = ADC_IDLE,
        .Adc_ResultAlignment = ADC_ALIGN_RIGHT,
        .Adc_ChannelGroup = (Adc_ChannelDefType*)Adc_ChannelGroup2,
        .Adc_NbrOfChannel = ADC_CHANNEL_GROUP_2_SIZE,
        .Adc_TriggerSource = ADC_TRIGG_SRC_SW,
        .Adc_HwTriggerSignal = ADC_HW_TRIG_RISING_EDGE,
        .Adc_HwTriggerTimer = 0,
        .Adc_StreamBufferMode = ADC_STREAM_BUFFER_LINEAR,
        .Adc_StreamNumSamples = 3,
        .Adc_ValueResultPtr = Adc_Group2_ResultBuffer,
        .Adc_ValueResultSize = 6,                    /* 2 channels × 3 samples */
        .Adc_SetupBufferFlag = 0,
        .Adc_NotificationCb = Adc_Group2_Notification,
        .Adc_NotificationEnable = ADC_NOTIFICATION_ENABLE,
        .Adc_InterruptType = ADC_HW_DMA
    },
    
    /* Group 3: Multi-channel Streaming - Streaming access, one-shot, software trigger */
    {
        .Adc_HwUnitId = ADC_INSTANCE_1,
        .Adc_GroupId = 2,
        .Adc_GroupPriority = 3,                      /* Low priority */
        .Adc_GroupAccessMode = ADC_ACCESS_MODE_STREAMING,
        .Adc_GroupConvMode = ADC_CONV_MODE_ONESHOT,
        .Adc_GroupReplacement = ADC_GROUP_REPL_SUSPEND_RESUME,
        .Adc_Status = ADC_IDLE,
        .Adc_ResultAlignment = ADC_ALIGN_RIGHT,
        .Adc_ChannelGroup = (Adc_ChannelDefType*)Adc_ChannelGroup3,
        .Adc_NbrOfChannel = ADC_CHANNEL_GROUP_3_SIZE,
        .Adc_TriggerSource = ADC_TRIGG_SRC_SW,
        .Adc_HwTriggerSignal = ADC_HW_TRIG_RISING_EDGE,
        .Adc_HwTriggerTimer = 0,
        .Adc_StreamBufferMode = ADC_STREAM_BUFFER_CIRCULAR,
        .Adc_StreamNumSamples = 5,
        .Adc_ValueResultPtr = Adc_Group3_ResultBuffer,
        .Adc_ValueResultSize = 20,                   /* 4 channels × 5 samples */
        .Adc_SetupBufferFlag = 0,
        .Adc_NotificationCb = Adc_Group3_Notification,
        .Adc_NotificationEnable = ADC_NOTIFICATION_ENABLE,
        .Adc_InterruptType = ADC_HW_DMA
    },
    
    /* Remaining groups - Reserved for future use */
    {0}, {0}, {0}, {0}, {0}, {0}, {0}
};

/****************************************************************************************
*                                 HARDWARE UNIT CONFIGURATIONS                         *
****************************************************************************************/
Adc_HwUnitDefType Adc_HwUnitConfig[ADC_MAX_HW_UNITS] = 
{
    /* ADC Hardware Unit 0 (ADC1) */
    {
        .AdcHW_UnitId = 0,
        .AdcHw_QueueEnable = ADC_ENABLE_QUEUING,
        .AdcHw_PriorityEnable = ADC_PRIORITY_IMPLEMENTATION,
        .AdcHw_DMAAvailable = ADC_DMA_AVAILABLE
    },
    
    /* ADC Hardware Unit 1 (ADC2) */
    {
        .AdcHW_UnitId = 1,
        .AdcHw_QueueEnable = ADC_ENABLE_QUEUING,
        .AdcHw_PriorityEnable = ADC_PRIORITY_IMPLEMENTATION,
        .AdcHw_DMAAvailable = ADC_DMA_NOT_AVAILABLE
    }
};

/****************************************************************************************
*                                 CHANNEL CONFIGURATIONS                               *
****************************************************************************************/
Adc_ChannelDefType Adc_ChannelConfig[ADC_MAX_CHANNELS] = 
{
    /* Channel 0 - PA0 */
    { .Adc_ChannelId = 0, .Adc_ChannelSampTime = ADC_SAMPLING_TIME_DEFAULT },
    /* Channel 1 - PA1 */
    { .Adc_ChannelId = 1, .Adc_ChannelSampTime = ADC_SAMPLING_TIME_DEFAULT },
    /* Channel 2 - PA2 */
    { .Adc_ChannelId = 2, .Adc_ChannelSampTime = ADC_SAMPLING_TIME_DEFAULT },
    /* Channel 3 - PA3 */
    { .Adc_ChannelId = 3, .Adc_ChannelSampTime = ADC_SAMPLING_TIME_DEFAULT },
    /* Channel 4 - PA4 */
    { .Adc_ChannelId = 4, .Adc_ChannelSampTime = ADC_SAMPLING_TIME_DEFAULT },
    /* Channel 5 - PA5 */
    { .Adc_ChannelId = 5, .Adc_ChannelSampTime = ADC_SAMPLING_TIME_DEFAULT },
    /* Channel 6 - PA6 */
    { .Adc_ChannelId = 6, .Adc_ChannelSampTime = ADC_SAMPLING_TIME_DEFAULT },
    /* Channel 7 - PA7 */
    { .Adc_ChannelId = 7, .Adc_ChannelSampTime = ADC_SAMPLING_TIME_DEFAULT },
    /* Channel 8 - PB0 */
    { .Adc_ChannelId = 8, .Adc_ChannelSampTime = ADC_SAMPLING_TIME_DEFAULT },
    /* Channel 9 - PB1 */
    { .Adc_ChannelId = 9, .Adc_ChannelSampTime = ADC_SAMPLING_TIME_DEFAULT },
    /* Remaining channels */
    { 10, ADC_SAMPLING_TIME_DEFAULT },
    { 11, ADC_SAMPLING_TIME_DEFAULT },
    { 12, ADC_SAMPLING_TIME_DEFAULT },
    { 13, ADC_SAMPLING_TIME_DEFAULT },
    { 14, ADC_SAMPLING_TIME_DEFAULT },
    { 15, ADC_SAMPLING_TIME_DEFAULT }
};

/****************************************************************************************
*                                 MAIN CONFIGURATION                                   *
****************************************************************************************/
const Adc_ConfigType Adc_Config = 
{
    .HwUnits = Adc_HwUnitConfig,
    .NumHwUnits = ADC_MAX_HW_UNITS,
    .Groups = Adc_GroupConfig,
    .NumGroups = ADC_MAX_GROUPS
};

/****************************************************************************************
*                                 CALLBACK FUNCTIONS                                   *
****************************************************************************************/
/**
 * @brief Transfer complete callback
 * @param[in] ADCx ADC hardware module
 * @details Called when ADC conversion is completed
 */
void Adc_TransferComplete_Callback(ADC_TypeDef* ADCx)
{
    /* Determine hardware unit */
    Adc_HwUnitType HwUnit = 0;
    
    if (ADCx == ADC1) 
    {
        HwUnit = 0;    /* Unit 0 for ADC1 */
    }
    else if (ADCx == ADC2) 
    {
        HwUnit = 1;    /* Unit 1 for ADC2 */
    }
    else
    {
        /* Invalid ADC instance */
        return;
    }
    
    /* Call hardware interrupt handler */
    AdcHw_InterruptHandler(ADCx, HwUnit);
}

/**
 * @brief DMA transfer complete callback
 * @param[in] DMAx_Channely DMA channel
 * @details Called when DMA transfer is completed
 */
void Adc_DmaTransferComplete_Callback(DMA_Channel_TypeDef* DMAx_Channely)
{
    /* ADC1 uses Hardware Unit 0 */
    Adc_HwUnitType HwUnit = 0;
    
    /* Call DMA interrupt handler */
    AdcHw_DmaInterruptHandler(DMAx_Channely, HwUnit);
}

/**
 * @brief Error callback
 * @param[in] ADCx ADC hardware module
 * @param[in] ErrorCode Error code
 * @details Called when ADC error occurs
 */
void Adc_Error_Callback(ADC_TypeDef* ADCx, uint32 ErrorCode)
{
    /* Handle ADC errors */
    /* This can be used for error logging and recovery */
    (void)ADCx;        /* Suppress unused parameter warning */
    (void)ErrorCode;   /* Suppress unused parameter warning */
}

/****************************************************************************************
*                                 END OF FILE                                          *
****************************************************************************************/
