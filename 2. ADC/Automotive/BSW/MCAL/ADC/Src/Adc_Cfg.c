/****************************************************************************************
*                                ADC_CFG.C                                              *
****************************************************************************************
* File Name   : Adc_Cfg.c
* Module      : Analog to Digital Converter (ADC)
* Description : AUTOSAR ADC driver configuration source file
* Version     : 1.0.0
* Date        : 24/06/2025
* Author      : hoangphuc540202@gmail.com
* Github      : https://github.com/HoangPhuc02
****************************************************************************************/

#include "Adc_Cfg.h"

void Adc_Isr_Handler(ADC_TypeDef* ADCx)
{
    if (ADC_GetITStatus(ADCx, ADC_IT_EOC) != RESET)
    {
        if(ADCx == ADC1)AdcHw_Interrupt_Handler(1);
        else if(ADCx == ADC2)AdcHw_Interrupt_Handler(2);
        ADC_ClearITPendingBit(ADCx, ADC_IT_EOC);
    }
    // 1. Get interrupt status and clear

    // 2. Check FIFO

    // 2.1 If fifo error -> Recovery -> 8

    // 2.2 If fifo not error -> 3
    
    // 3. Check fifo threshold
    
    // 3.1 If fifo threshold reached -> Set End of interrupt -> 8

    // 3.2 If fifo threshold not reached -> 4

    // 4. Read Fifo samples 

    // 5. Check if all samples are read completes 

    // 5.1 If not return 4

    // 5.2 If yes go to 5 

    // 6. If all samples are read completes -> Call notification callback

    // 7. Check Conv mode one shot

    // 7.1 If one shot -> Stop conversion -> 8

    // 7.2 If not one shot -> Check if next group is available -> 8

    // 8 End of ISR -> exit
}

void Dma1_Channel1_Isr_Handler(void);

void Adc_Notification_Group1(void);
void Adc_Notification_Group2(void);
Adc_ChannelDefType Adc_ChannelGroup1_2[ADC_MAX_CHANNEL] = 
{
    {
        .Adc_ChannelId = 0, 
        .Adc_ChannelSampTime = ADC_SampleTime_1Cycles5 /*!< default value when using stm32f103*/
    },
    {
        .Adc_ChannelId = 1,
        .Adc_ChannelSampTime = ADC_SampleTime_1Cycles5 /*!< default value when using stm32f103*/
    }
    // Add more channels as needed
};
#define ADC_HW_UNIT_1 (uint8)1
#define ADC_HW_UNIT_2 (uint8)2

#define ADC_GROUP_1 (uint8)1
#define ADC_GROUP_2 (uint8)2

#define ADC_MAX_GROUP_HW_1 2
#define ADC_MAX_GROUP_HW_2 2

Adc_GroupType AdcHw_GroupQueueHw1[ADC_MAX_GROUP_HW_1] = {0};
Adc_GroupType AdcHw_GroupQueueHw2[ADC_MAX_GROUP_HW_2] = {0};

Adc_GroupDefType Adc_GroupConfig[ADC_MAX_GROUPS] = 
{
    {
        .Adc_HwUnitId = ADC_HW_UNIT_1, /*!< ADC hardware unit ID */
        
        .Adc_GroupId = ADC_GROUP_1,
        .Adc_GroupPriority = 2,
        .Adc_GroupAccessMode = ADC_ACCESS_MODE_SINGLE, /*!<Type for configuring the access mode to group conversion results*/
        .Adc_GroupConvMode = ADC_CONV_MODE_ONESHOT, /*!<Type for configuring the conversion mode of an ADC Channel group*/

        // Adc_GroupReplacement
        // Adc_Status

        .Adc_ResultAlignment = ADC_ALIGN_RIGHT, /*!<Type for configuring the alignment of ADC raw results in ADC result buffer*/

        .Adc_ChannelGroup = Adc_ChannelGroup1_2,
        .Adc_NbrOfChannel = 2,

        // Adc_ChannelConversionId
  
        .Adc_TriggerSource   = ADC_TRIGG_SRC_SW, /*!<Trigger source for the ADC group*/
        .Adc_HwTriggerSignal = ADC_HW_TRIG_RISING_EDGE, /*!<HW trigger edge detection for the ADC group*/
        .Adc_HwTriggerTimer  = 0, /*!<Reload value for ADC embedded timer*/

        .Adc_StreamBufferMode = ADC_STREAM_BUFFER_LINEAR, /*!<Type for configuring the buffer type of the ADC group*/
        .Adc_StreamNumSamples = 1, /*!<Number of group conversions in streaming access mode*/ 

        .Adc_ValueResultPtr = NULL_PTR,
        .Adc_ValueResultSize = 2, /*!<Size of the result buffer for the ADC group*/

        //  Adc_SamplesResultCounter;
        .Adc_NotificationCb = NULL_PTR,

        
    },
    {
        .Adc_HwUnitId = ADC_HW_UNIT_1, /*!< ADC hardware unit ID */
        
        .Adc_GroupId = ADC_GROUP_2,
        .Adc_GroupPriority = 2,
        .Adc_GroupAccessMode = ADC_ACCESS_MODE_STREAMING, /*!<Type for configuring the access mode to group conversion results*/
        .Adc_GroupConvMode = ADC_CONV_MODE_ONESHOT, /*!<Type for configuring the conversion mode of an ADC Channel group*/

        .Adc_ResultAlignment = ADC_ALIGN_RIGHT, /*!<Type for configuring the alignment of ADC raw results in ADC result buffer*/

        .Adc_ChannelGroup = Adc_ChannelGroup1_2,
        .Adc_NbrOfChannel = 2,
  
        //.Adc_TransferType; /*!<Type for configuring the transfer type of the ADC group*/

        .Adc_TriggerSource   = ADC_TRIGG_SRC_SW, /*!<Trigger source for the ADC group*/
        .Adc_HwTriggerSignal = ADC_HW_TRIG_RISING_EDGE, /*!<HW trigger edge detection for the ADC group*/
        .Adc_HwTriggerTimer  = 0, /*!<Reload value for ADC embedded timer*/

        .Adc_StreamBufferMode = ADC_STREAM_BUFFER_LINEAR, /*!<Type for configuring the buffer type of the ADC group*/
        .Adc_StreamNumSamples = 8, /*!<Number of group conversions in streaming access mode*/ 

        .Adc_ValueResultPtr = NULL_PTR,
        .Adc_ValueResultSize = 16, /*!<Size of the result buffer for the ADC group*/

        .Adc_NotificationCb = NULL_PTR,

        .Adc_Status = ADC_IDLE, //Idle by default
        .Adc_GroupReplacement = 0, // Disable Group Replacement by default
        .Adc_SamplesResultCounter = 0,
        .Adc_ChannelConversionId = 0,
        .Adc_NotificationEnable = ADC_NOTIFICATION_DISABLE, // No buffer set by default
    },
};

const Adc_HwUnitDefType Adc_HwUnitConfig[ADC_MAX_HW_UNITS] =
{
    {
        .AdcHW_UnitId = ADC_HW_UNIT_1,
        .AdcHW_Prescale = 2,
        .AdcHw_QueueEnable = 1,
        .AdcHw_PriorityEnable = 0,
        .AdcHw_GroupQueue = AdcHw_GroupQueueHw1,
        .AdcHw_GroupQueueSize = ADC_MAX_GROUP_HW_1,
        .AdcHw_DMAAvailable = 1, // if the hw adc module has dma, this will be set to 1
        .AdcHw_HWTrigger = 0,

    },
    {
        .AdcHW_UnitId = ADC_HW_UNIT_2,
        .AdcHW_Prescale = 2,
        .AdcHw_QueueEnable = 1,
        .AdcHw_PriorityEnable = 0,
        .AdcHw_GroupQueue = AdcHw_GroupQueueHw2,
        .AdcHw_GroupQueueSize = ADC_MAX_GROUP_HW_2,
        .AdcHw_DMAAvailable = 0, // if the hw adc module has dma, this will be set to 1
        .AdcHw_HWTrigger = 0,

    },
};


Adc_ConfigType Adc_Config = {
    .HwUnits = Adc_HwUnitConfig,
    .NumHwUnits = ADC_MAX_HW_UNITS,

    // Pointer to an array of all configured groups
    .Groups = Adc_GroupConfig,
    .NumGroups = ADC_MAX_GROUPS,

    .PriorityImplementation = ADC_PRIORITY_NONE,
    .Initialized = 0, // Number of priorities supported by the driver
};
