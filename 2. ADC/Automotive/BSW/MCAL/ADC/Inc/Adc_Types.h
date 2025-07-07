/****************************************************************************************
*                                ADC_TYPES.H                                          *
****************************************************************************************
* File Name   : Adc_Types.h
* Module      : Analog to Digital Converter (ADC)
* Description : AUTOSAR ADC driver type definitions header file 
* Version     : 1.0.0
* Date        : 24/06/2025
* Author      : hoangphuc540202@gmail.com
* Github      : https://github.com/HoangPhuc02
****************************************************************************************/

#ifndef ADC_TYPES_H
#define ADC_TYPES_H

/****************************************************************************************
*                                 INCLUDE FILES                                        *
****************************************************************************************/
/* Standard AUTOSAR includes */
#include "Std_Types.h"
#include "stm32f10x_adc.h"
#include "stm32f10x.h"
/****************************************************************************************
*                              TYPE DEFINITIONS                                        *
****************************************************************************************/
/**
 * @brief   ADC Type definitions
 * 
 * The following section contains all type definitions used by the ADC driver:
 * 
 * ================ BASIC TYPES ================
 * Adc_ChannelType            - uint8: Numeric ID of an ADC Channel
 * Adc_GroupType              - uint8: Numeric ID of an ADC Channel Group
 * Adc_ValueGroupType         - uint16: Type for converted values (raw, without scaling)
 * Adc_ConfigType             - struct: Main configuration structure for ADC initialization
 *                              Members: {
 *                                uint8 adcChannelId;
 *                                uint8 adcGroupId;
 *                                uint16 adcResolution;
 *                                uint32 adcSampleTime;
 *                                uint32 adcClockSource;
 *                                uint8 adcPowerState;
 *                              }
 * 
 * ================ GROUP TYPES ================
 * Adc_GroupConvModeType      - enum: Conversion mode
 *                              Members: {
 *                                ADC_CONV_MODE_ONESHOT,
 *                                ADC_CONV_MODE_CONTINUOUS
 *                              }
 * Adc_GroupPriorityType      - uint8: Priority level of an ADC group
 * Adc_GroupAccessModeType    - enum: Access mode
 *                              Members: {
 *                                ADC_ACCESS_MODE_SINGLE,
 *                                ADC_ACCESS_MODE_STREAMING
 *                              }
 * Adc_GroupReplacementType   - enum: Behavior when group is interrupted
 *                              Members: {
 *                                ADC_GROUP_REPL_ABORT_RESTART,
 *                                ADC_GROUP_REPL_SUSPEND_RESUME
 *                              }
 * 
 * ================ STREAMING TYPES ================
 * Adc_StreamNumSampleType    - uint8: Number of group conversions in streaming mode
 * Adc_StreamBufferModeType   - enum: Buffer type
 *                              Members: {
 *                                ADC_STREAM_BUFFER_LINEAR,
 *                                ADC_STREAM_BUFFER_CIRCULAR
 *                              }
 * 
 * ================ TIMING TYPES ================
 * Adc_PrescaleType           - uint8: ADC clock prescaler value
 * Adc_ConversionTimeType     - uint32: Time for ADC conversion
 * Adc_SamplingTimeType       - uint32: ADC sampling time in clock cycles
 * Adc_HwTriggerTimerType     - uint32: Reload value for ADC embedded timer
 * 
 * ================ TRIGGER TYPES ================
 * Adc_TriggerSourceType      - enum: Trigger source
 *                              Members: {
 *                                ADC_TRIGG_SRC_SW,
 *                                ADC_TRIGG_SRC_HW
 *                              }
 * Adc_HwTriggerSignalType    - enum: HW trigger edge detection
 *                              Members: {
 *                                ADC_HW_TRIG_RISING_EDGE,
 *                                ADC_HW_TRIG_FALLING_EDGE,
 *                                ADC_HW_TRIG_BOTH_EDGES
 *                              }
 * 
 * ================ PRIORITY TYPES ================
 * Adc_PriorityImplementationType - enum: Priority mechanism type
 *                              Members: {
 *                                ADC_PRIORITY_NONE,
 *                                ADC_PRIORITY_HW,
 *                                ADC_PRIORITY_HW_SW
 *                              }
 * 
 * ================ LIMIT CHECKING TYPES ================
 * Adc_ChannelRangeSelectType - enum: Valid conversion value range configuration
 *                              Members: {
 *                                ADC_RANGE_UNDER_LOW,
 *                                ADC_RANGE_BETWEEN,
 *                                ADC_RANGE_OVER_HIGH,
 *                                ADC_RANGE_ALWAYS,
 *                                ADC_RANGE_NOT_UNDER_LOW,
 *                                ADC_RANGE_NOT_BETWEEN,
 *                                ADC_RANGE_NOT_OVER_HIGH
 *                              }
 * 
 * ================ POWER MANAGEMENT TYPES ================
 * Adc_PowerStateType         - enum: Power state of the ADC module
 *                              Members: {
 *                                ADC_FULL_POWER,
 *                                DECREASING_POWER
 *                              }
 * Adc_PowerStateRequestResultType - enum: Result of power state transition requests
 *                              Members: {
 *                                ADC_SERVICE_ACCEPTED,
 *                                ADC_NOT_INIT,
 *                                ADC_SEQUENCE_ERROR,
 *                                ADC_HW_FAILURE,
 *                                ADC_POWER_STATE_NOT_SUPP,
 *                                ADC_TRANS_NOT_POSSIBLE
 *                              }
 */
/****************************************************************************************
*                      Macro definitions                                          *
****************************************************************************************/
static inline ADC_TypeDef* ADC_GET_HW_MODULE_ID(uint8_t id) {
    return (id == 1) ? ADC1 : (id == 2) ? ADC2 : NULL;
}

static inline DMA_Channel_TypeDef* ADC_GET_HW_DMA_ID(uint8_t id) {
    return (id == 1) ? DMA1_Channel1 : NULL;
}
/****************************************************************************************
*                              VERSION INFORMATION                                     *
****************************************************************************************/

#define ADC_SW_PATCH_VERSION    0 

#define ADC_NOTIFICATION_ENABLE     1
#define ADC_NOTIFICATION_DISABLE    0

#define ADC_DMA_ENABLE 1
#define ADC_DMA_DISABLE 0

#define ADC_ENABLE_QUEUING 1
#define ADC_PRIORITY_IMPLEMENTATION 0

/****************************************************************************************
*                      BASIC NUMERIC TYPES                                             *
****************************************************************************************/
/**
 * @brief   Adc_ChannelType
 * @typedef uint8
 * @details Numeric ID of an ADC Channel
 * @reqs    SWS_Adc_00506
 */
typedef uint8 Adc_ChannelType;

/**
 * @brief   Adc_GroupType
 * @typedef uint8
 * @details Numeric ID of an ADC Channel Group
 * @reqs    SWS_Adc_00507
 */
typedef uint8 Adc_GroupType;    

/**
 * @brief   Adc_ValueGroupType
 * @typedef uint16
 * @details Type for reading the converted values of a channel group (raw, without 
 *          further scaling, alignment according to ADC_RESULT_ALIGNMENT).
 *          The size is determined by the highest resolution used by any channel 
 *          in any group.
 * @note    The size of this type must be large enough to hold the maximum
 *          resolution . STM32F10x ADC supports up to 12-bit resolution so we need
 *          at least 16 bits to hold the value.
 * @reqs    SWS_Adc_00508
 */
typedef uint16 Adc_ValueGroupType;

/**
 * @brief  Adc_GroupPriorityType
 * @typedef uint8
 * @details Priority level of the channel. Lowest priority is 0.
 * @reqs   SWS_Adc_00516
 */
typedef uint8 Adc_GroupPriorityType;

/**
 * @brief   Adc_StreamNumSampleType
 * @typedef uint8
 * @details Type for the number of group conversions in streaming access mode
 * @note    In single access mode, parameter is 1
 * @reqs    SWS_Adc_00518
 */
typedef uint8 Adc_StreamNumSampleType;  

/****************************************************************************************
*                      Non standard types                                              *
****************************************************************************************/
/**
 * @brief For ennumarate the Ids of the different groups.
 */
typedef enum
{
    GROUP_0 = 0, /*!< ID 0 ADC Channel group*/
    GROUP_1,     /*!< ID 1 ADC Channel group*/
    GROUP_2,     /*!< ID 2 ADC Channel group*/
    GROUP_3,     /*!< ID 3 ADC Channel group*/
    GROUP_4,     /*!< ID 4 ADC Channel group*/
    GROUP_5,     /*!< ID 5 ADC Channel group*/
    GROUP_6,     /*!< ID 6 ADC Channel group*/
    GROUP_7,     /*!< ID 7 ADC Channel group*/
    GROUP_8,     /*!< ID 8 ADC Channel group*/
    GROUP_9,     /*!< ID 9 ADC Channel group*/
    GROUP_10     /*!< ID 10 ADC Channel group*/
} Group_IDType;
/**
 * @brief For ennumarate the states of power for the ADC HW unit.
 */
typedef enum
{
    PWR_STATE_0 = 0, /*!< Power state 0*/
    PWR_STATE_1,     /*!< Power state 1*/
    PWR_STATE_2,     /*!< Power state 2*/
    PWR_STATE_3,     /*!< Power state 3*/
    PWR_STATE_4,     /*!< Power state 4*/
    PWR_STATE_5,     /*!< Power state 5*/
    PWR_STATE_6,     /*!< Power state 6*/
    PWR_STATE_7,     /*!< Power state 7*/
    PWR_STATE_8,     /*!< Power state 8*/
    PWR_STATE_9,     /*!< Power state 9*/
    PWR_STATE_10     /*!< Power state 10*/
} PowerStateType; 

/****************************************************************************************
*                      TIMING AND CLOCK TYPES                                          *
****************************************************************************************/
/**
 * @brief   Adc_PrescaleType
 * @typedef uint8
 * @details Type for ADC prescaler value.
 *          This type is used to define the prescaler value for the ADC clock.
 *          The prescaler is used to divide the ADC clock frequency to achieve 
 *          the desired sampling rate.
 * @reqs    SWS_Adc_00509
 */
typedef uint8 Adc_PrescaleType;

/**
 * @brief   Adc_ResolutionType
 * @typedef uint8
 * @details Type of channel resolution in number of bits.
 * @reqs    SWS_Adc_00512
 */
typedef uint8 Adc_ResolutionType;

/**
 * @brief   Adc_ConversionTimeType
 * @typedef uint32
 * @details Type for ADC conversion time.
 *          This type is used to define the time taken for an ADC conversion in 
 *          microseconds or clock cycles, depending on the configuration.
 * @reqs    SWS_Adc_00510
 */
typedef uint32 Adc_ConversionTimeType;

/** 
 * @brief   Adc_SamplingTimeType
 * @typedef uint32
 * @details Type for ADC sampling time.
 *          the time during which the value is sampled, (in clock-cycles)
 * @reqs    SWS_Adc_00511
 */
typedef uint32 Adc_SamplingTimeType;

/** 
 * @brief   Adc_HwTriggerTimerType
 * @typedef uint32
 * @details Type for the reload value of the ADC module embedded timer.
 */
typedef uint32 Adc_HwTriggerTimerType;
/***************************************************************************************
*                      CONVERSION AND TRIGGER ENUMS                                    *
****************************************************************************************/
/**
 * @brief   Adc_TriggerSourceType
 * @typedef enum
 * @details Type for configuring the trigger for an ADC Channel Group.
 * @reqs    SWS_Adc_00514
 */
typedef enum
{
    ADC_TRIGG_SRC_SW    =   0x00U,        /*!< Software trigger by API Call*/
    ADC_TRIGG_SRC_HW    =   0x01U,        /*!< Hardware trigger by HW event*/
} Adc_TriggerSourceType;

/**
 * @brief   Adc_GroupStatusType
 * @typedef enum
 * @details Type for ADC group status.
 *          Represents the current status of an ADC group.
 * @note:
 * ADC_CONV_MODE_ONESHOT : Performs single conversion of each channel after
 *                         trigger event. Can be stopped by software API call
 *                         for software trigger, or by disabling hardware trigger.
 * ADC_CONV_MODE_CONTINUOUS : Performs repeated conversions of each channel.
 *                            Only available for software trigger source.
 *                            Can be stopped by software API call.
 * @reqs    SWS_Adc_00515
 */
typedef enum
{
    ADC_CONV_MODE_ONESHOT = 0x00U,        /*!<One-shot conversion mode  */
    ADC_CONV_MODE_CONTINUOUS = 0x01U,     /*!<Continuous conversion mode*/
} Adc_GroupConvModeType;

/**
 * @brief   Adc_GroupAccessModeType
 * @typedef enum
 * @details Type for configuring the access mode to group conversion results.
 * @reqs    SWS_Adc_00528
 */
typedef enum{
    ADC_ACCESS_MODE_SINGLE = 0x00U,     /*!< Single access mode*/
    ADC_ACCESS_MODE_STREAMING = 0x01U   /*!< Streaming access mode */
} Adc_GroupAccessModeType;

/**
 * @brief   Adc_HwTriggerSignalType
 * @typedef enum
 * @details Type for configuring the access mode to group conversion results.
 * @reqs    SWS_Adc_00520
 */
typedef enum {
    ADC_HW_TRIG_RISING_EDGE = 0x00U,    /*!< Hardware trigger rising edge */
    ADC_HW_TRIG_FALLING_EDGE  = 0x01U,  /*!< Hardware trigger falling edge */
    ADC_HW_TRIG_BOTH_EDGES = 0x02U      /*!< Hardware trigger both edges */
} Adc_HwTriggerSignalType;

/****************************************************************************************
*                      BUFFER AND REPLACEMENT ENUMS                                    *
****************************************************************************************/
/**
 * @brief   Adc_StreamBufferModeType
 * @typedef enum
 * @details Type for configuring streaming access mode buffer type
 * @note    
 * ADC_STREAM_BUFFER_LINEAR : Stops conversion when buffer is full.
 * ADC_STREAM_BUFFER_CIRCULAR : Continues conversion when buffer is full by wrapping around.
 * @reqs    SWS_Adc_00519
 */
typedef enum 
{
    ADC_STREAM_BUFFER_LINEAR    = 0x00U,
    ADC_STREAM_BUFFER_CIRCULAR  = 0x01U
} Adc_StreamBufferModeType;

/**
 * @brief   Adc_GroupReplacementType
 * @typedef enum
 * @details Type for configuring the replacement mechanism used when a group 
 *          is interrupted by a higher priority group.
 *          
 * @note    ADC_GROUP_REPL_ABORT_RESTART: When interrupted, the complete conversion
 *          round restarts after high-priority conversion finishes. In streaming mode,
 *          only interrupted results are discarded. Previous results remain intact.
 *          
 * @note    ADC_GROUP_REPL_SUSPEND_RESUME: When interrupted, the conversion
 *          continues from where it left off after high-priority conversion finishes.
 *          Previous results remain intact.
 * @reqs    SWS_Adc_00523
 */
typedef enum 
{
    ADC_GROUP_REPL_ABORT_RESTART,
    ADC_GROUP_REPL_SUSPEND_RESUME,
}Adc_GroupReplacementType;
/**
 * @brief   Adc_ResultAlignmentType
 * @typedef enum
 * @details Type for alignment of ADC raw results in ADC result buffer (left/right alignment).
 * @reqs    SWS_Adc_00525
 */
typedef enum 
{
    ADC_ALIGN_LEFT  = 0x00U,    /*!< Left alignment */
    ADC_ALIGN_RIGHT = 0x01U     /*!< Right alignment */
}Adc_ResultAlignmentType;
/****************************************************************************************
*                                       STATUS ENUMS                                    *
****************************************************************************************/
/**
 * @brief   Adc_StatusType
 * @typedef enum
 * @details Current status of the conversion of the requested ADC channel group.
 * @note ADC_IDLE : - The conversion of the specified
 *                  group has not been started.
 *                  - No result is available.
 *       ADC_BUSY : - The conversion of the specified group
 *                  has been started and is still going on.
 *                  - So far no result is available.
 *       ADC_COMPLETED : - A conversion round (which is not the
 *                      final one) of the specified group has
 *                      been finished.
 *                      - A result is available for all channels
 *                      of the group.
 *       ADC_STREAM_COMPLETED : - The result buffer is completely filled
 *                              - For each channel of the selected group
 *                              the number of samples to be acquired is available
 * @reqs    SWS_Adc_00513   
 */
typedef enum
{
    ADC_IDLE = 0x00U,   /*!< ADC module is uninitialized */
    ADC_BUSY = 0x01U,   /*!< ADC module is busy with a conversion */
    ADC_COMPLETED = 0x02U, /*!< ADC module has completed a conversion */
    ADC_STREAM_COMPLETED = 0x03U /*!< ADC module has completed streaming */
} Adc_StatusType;
/****************************************************************************************
*                      PRIORITY AND RANGE ENUMS                                        *
****************************************************************************************/
/**
 * @brief   Adc_PriorityImplementationType
 * @typedef enum
 * @details  Type for configuring the priority of ADC channels.
 * @note
 *  ADC_PRIORITY_NONE : priority mechanism is not available
 *  ADC_PRIORITY_HW   : Hardware priority mechanism is available only
 *  ADC_PRIORITY_HW_SW: Hardware and software priority mechanism is available
 * @reqs    SWS_Adc_00522
 */
typedef enum {
    ADC_PRIORITY_NONE = 0x00U, 
    ADC_PRIORITY_HW = 0x01U, 
    ADC_PRIORITY_HW_SW = 0x02U 
} Adc_PriorityImplementationType;

/**
 * @brief   Adc_ChannelRangeSelectType
 * @typedef enum
 * @details Defines which conversion values are considered valid based on configured limits.
 *          Used for limit checking against values defined by AdcChannelLowLimit and 
 *          AdcChannelHighLimit.
 * @reqs    SWS_Adc_00524
 */
typedef enum 
{
    ADC_RANGE_UNDER_LOW    = 0x00U,   /*!< Range below low limit - low limit value included */
    ADC_RANGE_BETWEEN      = 0x01U,   /*!< Range between low limit and high limit - high limit value included */
    ADC_RANGE_OVER_HIGH    = 0x02U,   /*!< Range above high limit */
    ADC_RANGE_ALWAYS       = 0x03U,   /*!< Complete range - independent from channel limit settings */
    ADC_RANGE_NOT_UNDER_LOW = 0x04U,  /*!< Range above low limit */
    ADC_RANGE_NOT_BETWEEN  = 0x05U,   /*!< Range above high limit or below low limit - low limit value included */
    ADC_RANGE_NOT_OVER_HIGH = 0x06U   /*!< Range below high limit - high limit value included */
} Adc_ChannelRangeSelectType;


/****************************************************************************************
*                      POWER MANAGEMENT ENUMS                                          *
****************************************************************************************/
/**
 * @brief   Adc_PowerStateType
 * @typedef enum
 * @details Represents the power state of the ADC module.
 *          Values 1-255 represent power modes with decreasing power consumption.
 *          Used to define currently active power state or target power state to be set.
 * @reqs   SWS_Adc_00526
 */
typedef enum
{
    ADC_FULL_POWER = 0x00U,         /*!< Full power mode (highest power consumption) */
    DECREASING_POWER = 0x01U,       /*!< Power modes with decreasing power consumptions. Other 254 values tdb*/
} Adc_PowerStateType;

/**
 * @brief   Adc_PowerStateRequestResultType
 * @typedef enum
 * @details Result of the requests related to power state transitions.
 * @reqs    SWS_Adc_00527
 */
typedef enum
{
    ADC_SERVICE_ACCEPTED      = 0x00U,   /*!< Power state change executed */
    ADC_NOT_INIT              = 0x01U,   /*!< ADC Module not initialized */
    ADC_SEQUENCE_ERROR        = 0x02U,   /*!< Wrong API call sequence */
    ADC_HW_FAILURE            = 0x03U,   /*!< The HW module has a failure which prevents it to enter the required power state */
    ADC_POWER_STATE_NOT_SUPP  = 0x04U,   /*!< ADC Module does not support the requested power state */
    ADC_TRANS_NOT_POSSIBLE    = 0x05U    /*!< ADC Module cannot transition directly from the current power state to the requested power state or the HW peripheral is still busy */
} Adc_PowerStateRequestResultType;

/****************************************************************************************
*                      CONFIGURATION STRUCTURE TYPES                                   *
****************************************************************************************/

/** 
 * @brief   Adc_ConfigType
 * @typedef struct
 * @details Data structure containing the set of configuration parameters 
 *          required for initializing the ADC Driver and ADC HW Unit(s).
 */
/**
 * private enum
 */

typedef uint8_t Adc_HwUnitType; /*!<Type for the ID of the ADC HW Unit*/
typedef enum {
    ADC_HW_DMA,
    ADC_HW_EOC
}Adc_HwNvicType  ;
/* Not used for now */
//  [ECUC_Adc_00087]

typedef struct _Adc_HwUnitType
{
    Adc_HwUnitType      AdcHW_UnitId;   
    Adc_PrescaleType    AdcHW_Prescale;
    uint8               AdcHw_QueueEnable;
    Adc_PriorityImplementationType   AdcHw_PriorityEnable; // default is none cause i have no idea using queue now
    Adc_GroupType       *AdcHw_GroupQueue; // Queue of groups for this ADC hardware unit
    Adc_GroupType       AdcHw_GroupQueueSize;
    Adc_GroupType       AdcHw_CurrGroupRegisterId; // this will give id of the group in queue will be work next
    Adc_GroupType       AdcHw_NbrOfGroupRegister; // number of group are in the list
    Adc_HwNvicType      AdcHW_Nvic;
    uint8               AdcHw_DMAAvailable; // if the hw adc module has dma, this will be set to 1 
    uint8               AdcHw_HWTrigger;
} Adc_HwUnitDefType;

// typedef uint8  Adc_ChannelLimitCheckType; // Enable or disable limit checking for an ADC channel
// typedef uint32 Adc_ChannelHighLimitType;
// typedef uint32 Adc_ChannelLowLimitType; 
// typedef uint8  Adc_ChannelRefVoltsrcHighType; // should be an enum but not used for now
// typedef uint8  Adc_ChannelRefVoltsrcLowType; // should be an enum but not used for now
// typedef struct
// {
//     Adc_ChannelType             Adc_ChannelId;
//     Adc_SamplingTimeType        Adc_ChannelSampTime;
//     Adc_ConversionTimeType      Adc_ChannelConvTime; /*!<Time for ADC conversion in microseconds or clock cycles*/
//     Adc_ResolutionType          Adc_ChannelResolution;

//     uint8                       Adc_ChannelLimitCheck; // Enable or disable limit checking for an ADC channel
//     Adc_ChannelHighLimitType    Adc_ChannelHighLimit;
//     Adc_ChannelLowLimitType     Adc_ChannelLowLimit;  
//     Adc_ChannelRangeSelectType  Adc_ChannelRangeSelect;

//     Adc_ChannelRefVoltsrcHighType Adc_ChannelRefVoltsrcHigh; // should be an enum but not used for now
//     Adc_ChannelRefVoltsrcLowType  Adc_ChannelRefVoltsrcLow; // should be an enum but not used for now
//     // uint8                Adc_ChannelRank; /*!< less than rank allow */
// }Adc_ChannelDefType;

typedef struct
{
    Adc_ChannelType             Adc_ChannelId;
    Adc_SamplingTimeType        Adc_ChannelSampTime;
}Adc_ChannelDefType;

// Function Callback for Notify 
typedef void (*Adc_NotificationCallBack)(void);



/**
 * @brief   Adc_GroupDefType
 * @typedef struct
 * @details Type for assignment of channels to a channel group. (size tbd)
 * @todo    Redefine it
 * @reqs    SWS_Adc_00517
 */
typedef struct{
    Adc_HwUnitType          Adc_HwUnitId;
    
    Adc_GroupType            Adc_GroupId; /*!<ID of the ADC Channel group*/
    Adc_GroupPriorityType    Adc_GroupPriority; /*!<Priority level of the channel group*/
    Adc_GroupAccessModeType  Adc_GroupAccessMode; /*!<Type for configuring the access mode to group conversion results*/
    Adc_GroupConvModeType    Adc_GroupConvMode; /*!<Type for configuring the conversion mode of an ADC Channel group*/
    
    Adc_GroupReplacementType Adc_GroupReplacement; /* This is not available now cause priority disable*/
    
    Adc_StatusType           Adc_Status;
    Adc_ResultAlignmentType  Adc_ResultAlignment; /*!<Type for configuring the alignment of ADC raw results in ADC result buffer*/

    Adc_ChannelDefType       *Adc_ChannelGroup;
    Adc_ChannelType          Adc_NbrOfChannel;
    Adc_ChannelType          Adc_ChannelConversionId; // get the current channel conversion;

    Adc_TriggerSourceType    Adc_TriggerSource; /*!<Trigger source for the ADC group*/
    Adc_HwTriggerSignalType  Adc_HwTriggerSignal; /*!<HW trigger edge detection for the ADC group*/
    Adc_HwTriggerTimerType   Adc_HwTriggerTimer; /*!<Reload value for ADC embedded timer*/

    Adc_StreamBufferModeType Adc_StreamBufferMode; /*!<Type for configuring the buffer type of the ADC group*/
    Adc_StreamNumSampleType  Adc_StreamNumSamples; /*!<Number of group conversions in streaming access mode*/ 
    
    Adc_ValueGroupType       *Adc_ValueResultPtr;
    uint32                   Adc_ValueResultSize; /*!<Size of the result buffer for the ADC group*/
                                                  /* It equal to Adc_StreamNumSamples * Adc_NbrOfChannel*/
    Adc_StreamNumSampleType  Adc_SamplesResultCounter; /*!<Index of the current result in the result buffer
                                                        if single mode, this is 0 by default                                                    
                                                        if streaming mode, this is the number of samples already acquired.  
                                                        start from 0 and not greater than Adc_StreamNumSamples */
    Adc_NotificationCallBack Adc_NotificationCb;
    uint8                    Adc_NotificationEnable;

    
} Adc_GroupDefType; 

 /**
 * @brief **Adc configuration structure**
 *
 * Data structure containing the set of configuration parameters required for initializing the ADC
 * Driver and ADC HW Unit(s). (Elements tbd)
 *
 * @reqs    SWS_Adc_00505
 */

typedef struct
{
    // Pointer to an array of all configured hardware units
    const Adc_HwUnitDefType*        HwUnits;
    uint8                           NumHwUnits;

    // Pointer to an array of all configured groups
    const Adc_GroupDefType*        Groups;
    uint8                           NumGroups;

    // Global priority implementation for the driver
    Adc_PriorityImplementationType  PriorityImplementation;
    uint8                           Initialized; // Number of priorities supported by the driver
    // Pointer to the array that holds the conversion results (managed by the driver)
    // This is often a global array that the driver writes into via DMA.
    // Adc_ValueGroupType* AdcResultsBuffer;
    // uint16 AdcResultsBufferSize;

    // ... other global driver parameters (e.g., debug settings, power modes)
} Adc_ConfigType;
//  General configuration (parameters) of the ADC Driver software module.
typedef struct 
{
    uint8 AdcDeInitApi;
    uint8 AdcDevErrorDetect;
    uint8 AdcEnableLimitCheck;
    uint8 AdcEnableQueuing;
    uint8 AdcEnableStartStopGroupApi;
    uint8 AdcGrpNotifCapability;
    uint8 AdcHwTriggerApi;
    uint8 AdcLowPowerStatesSupport;
    uint8 AdcPowerStateAsynchTransitionMode;
    uint8 AdcPriorityImplementation;
    uint8 AdcReadGroupApi;
    uint8 AdcResultAlignment;
    uint8 AdcVersionInfoApi;
    uint8 AdcEcucPartitionRef;
    uint8 AdcKernelEcucPartitionRef;

}Adc_General;
#define ADC_ENABLE_QUEUING 1
#define ADC_PRIORITY_IMPLEMENTATION 0
typedef struct{
    uint32 AdcPowerState;
    uint32 AdcPowerStateConfig;
} AdcPowerState;

#endif /* ADC_TYPES_H */

/****************************************************************************************
*                                 END OF FILE                                          *
****************************************************************************************/