/****************************************************************************************
*                                ADC_HW.H                                              *
****************************************************************************************
* File Name   : Adc_Hw.h
* Module      : Analog to Digital Converter (ADC) - Hardware Layer
* Description : AUTOSAR ADC driver hardware abstraction layer header file 
* Version     : 2.0.0 - Redesigned for clarity and maintainability
* Date        : 24/06/2025
* Author      : hoangphuc540202@gmail.com
* Github      : https://github.com/HoangPhuc02
****************************************************************************************/

#ifndef ADC_HW_H
#define ADC_HW_H

/****************************************************************************************
*                                 INCLUDE FILES                                        *
****************************************************************************************/
/* Standard AUTOSAR includes */
#include "Std_Types.h"
#include "Adc_Types.h"
#include "Adc_Cfg.h"

/* STM32 hardware includes */
// #include "stm32f10x_adc.h"
// #include "stm32f10x_dma.h"
// #include "misc.h"

/****************************************************************************************
*                              VERSION INFORMATION                                     *
****************************************************************************************/
#define ADC_HW_SW_MAJOR_VERSION     2
#define ADC_HW_SW_MINOR_VERSION     0
#define ADC_HW_SW_PATCH_VERSION     0

/****************************************************************************************
*                              HARDWARE VALIDATION MACROS                             *
****************************************************************************************/
#define ADC_HW_IS_VALID_UNIT(id)    (((id) >= 0) && ((id) < ADC_MAX_HW_UNITS))
#define ADC_HW_IS_VALID_GROUP(id)   (((id) >= 0) && ((id) < ADC_MAX_GROUPS))
#define ADC_HW_IS_VALID_CHANNEL(id) (((id) >= 0) && ((id) < ADC_MAX_CHANNELS))

/****************************************************************************************
*                              HARDWARE ABSTRACTION MACROS                            *
****************************************************************************************/

/**
 * @brief Get DMA interrupt flag for ADC unit
 */
#define ADC_HW_GET_DMA_IT_FLAG(id)  ((id == 1) ? DMA1_IT_TC1 : 0)

/**
 * @brief Get ADC interrupt flag
 */
#define ADC_HW_GET_ADC_IT_FLAG(id)  ((id == 1) ? ADC_IT_EOC : 0)

/****************************************************************************************
*                              HARDWARE STATUS MACROS                                 *
****************************************************************************************/
/**
 * @brief Check if ADC is enabled
 */
#define ADC_HW_IS_ENABLED(ADCx)     ((ADCx->CR2 & ADC_CR2_ADON) != 0)

/**
 * @brief Check if ADC conversion is ongoing
 */
#define ADC_HW_IS_CONVERTING(ADCx)  ((ADCx->SR & ADC_SR_STRT) != 0)

/**
 * @brief Check if DMA is enabled for ADC
 */
#define ADC_HW_IS_DMA_ENABLED(ADCx) ((ADCx->CR2 & ADC_CR2_DMA) != 0)

/**
 * @brief Check if end-of-conversion flag is set
 */
#define ADC_HW_IS_EOC_SET(ADCx)     ((ADCx->SR & ADC_SR_EOC) != 0)

/****************************************************************************************
*                              EXTERNAL DECLARATIONS                                   *
****************************************************************************************/
/* Configuration arrays */
extern Adc_GroupDefType     Adc_GroupConfig[ADC_MAX_GROUPS];
extern Adc_HwUnitDefType    Adc_HwUnitConfig[ADC_MAX_HW_UNITS];


/****************************************************************************************
*                              INITIALIZATION FUNCTIONS                               *
****************************************************************************************/
/**
 * @brief Initialize ADC hardware module
 * @param[in] HwUnitId ADC hardware unit ID
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_Init(Adc_HwUnitType HwUnitId);

/**
 * @brief Deinitialize ADC hardware module
 * @param[in] HwUnitId ADC hardware unit ID
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_DeInit(Adc_HwUnitType HwUnitId);

/**
 * @brief Configure ADC hardware module for a specific group
 * @param[in] HwUnitId ADC hardware unit ID
 * @param[in] GroupId ADC group ID
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_ConfigureGroup(Adc_HwUnitType HwUnitId, Adc_GroupType GroupId);

/****************************************************************************************
*                              CONVERSION CONTROL FUNCTIONS                           *
****************************************************************************************/
/**
 * @brief Start software-triggered conversion
 * @param[in] HwUnitId ADC hardware unit ID
 * @param[in] GroupId ADC group ID
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_StartSwConversion(Adc_HwUnitType HwUnitId, Adc_GroupType GroupId);

/**
 * @brief Stop software-triggered conversion
 * @param[in] HwUnitId ADC hardware unit ID
 * @param[in] GroupId ADC group ID
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_StopSwConversion(Adc_HwUnitType HwUnitId, Adc_GroupType GroupId);

/**
 * @brief Start hardware-triggered conversion
 * @param[in] HwUnitId ADC hardware unit ID
 * @param[in] GroupId ADC group ID
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_StartHwConversion(Adc_HwUnitType HwUnitId, Adc_GroupType GroupId);

/**
 * @brief Stop hardware-triggered conversion
 * @param[in] HwUnitId ADC hardware unit ID
 * @param[in] GroupId ADC group ID
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_StopHwConversion(Adc_HwUnitType HwUnitId, Adc_GroupType GroupId);

/**
 * @note : call when finish hw conversion or hw conversion is stop by ADC_HWSTOPConversion
 */
Std_ReturnType AdcHw_RecallSwConversion(Adc_HwUnitType HwUnitId);
/****************************************************************************************
*                              CHANNEL CONFIGURATION FUNCTIONS                        *
****************************************************************************************/
/**
 * @brief Configure ADC channels for a group
 * @param[in] HwUnitId ADC hardware unit ID
 * @param[in] GroupId ADC group ID
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_ConfigureChannels(ADC_TypeDef* ADCx, Adc_HwUnitDefType* HwUnitConfig, Adc_GroupDefType* GroupConfig);


/****************************************************************************************
*                              RESULT HANDLING FUNCTIONS                              *
****************************************************************************************/
/**
 * @brief Read conversion result
 * @param[in] HwUnitId ADC hardware unit ID
 * @param[in] GroupId ADC group ID
 * @param[out] ResultPtr Pointer to result buffer
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_ReadResult(Adc_HwUnitType HwUnitId, 
                               Adc_GroupType GroupId, 
                               Adc_ValueGroupType* ResultPtr);

/**
 * @brief Read streaming results
 * @param[in] HwUnitId ADC hardware unit ID
 * @param[in] GroupId ADC group ID
 * @param[out] ResultPtr Pointer to result buffer
 * @param[in] NumSamples Number of samples to read
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_ReadStreamingResults(Adc_HwUnitType HwUnitId, 
                                         Adc_GroupType GroupId, 
                                         Adc_ValueGroupType* ResultPtr, 
                                         Adc_StreamNumSampleType NumSamples);

/****************************************************************************************
*                              STATUS FUNCTIONS                                       *
****************************************************************************************/
/**
 * @brief Set private ADC group runtime status
 * @param[in] GroupId ADC group ID
 * @return Sample Counter
 */
Adc_StreamNumSampleType AdcHw_GetGroupRuntimeSampCounter(Adc_GroupType GroupId);

/**
 * @brief Get ADC group status
 * @param[in] HwUnitId ADC hardware unit ID
 * @param[in] GroupId ADC group ID
 * @return Group status
 */
Adc_StatusType AdcHw_GetGroupRuntimeStatus(Adc_GroupType GroupId);

/**
 * @brief Reset private ADC group runtime Parameter
 * @param[in] GroupId ADC group ID
 * @return E_NOT_OK if invalid Group, E_OK if reset successfull
 */
Std_ReturnType AdcHw_ResetGroupRuntime(Adc_GroupType GroupId);

/**
 * 
 */
void AdcHw_SetGroupStatus(Adc_GroupType GroupId, Adc_StatusType Status);
/**
 * @brief Reset private ADC hw runtime  Parameter
 * @param[in] HwUnitId ADC hardware unit ID
 * @return E_NOT_OK if invalid HW, E_OK if reset successfull
 */
Std_ReturnType AdcHw_ResetHwRuntime(Adc_HwUnitType HwUnitId);
/**
 * @brief Check if hardware unit is busy
 * @param[in] HwUnitId ADC hardware unit ID
 * @return TRUE if busy, FALSE if idle
 */
Adc_HwUnitStateType AdcHw_GetHwUnitState(Adc_HwUnitType HwUnitId);

/**
 * @brief Get current converting channel
 * @param[in] HwUnitId ADC hardware unit ID
 * @param[in] GroupId ADC group ID
 * @return Current channel ID
 */
Adc_ChannelType AdcHw_GetCurrentChannel(Adc_HwUnitType HwUnitId, Adc_GroupType GroupId);


/****************************************************************************************
*                              DMA FUNCTIONS                                          *
****************************************************************************************/
/**
 * @brief Initialize DMA for ADC conversion
 * @param[in] HwUnitId ADC hardware unit ID
 * @param[in] GroupId ADC group ID
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_InitDma(Adc_HwUnitType HwUnitId, Adc_GroupType GroupId);

/**
 * @brief Deinitialize DMA for ADC conversion
 * @param[in] HwUnitId ADC hardware unit ID
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_DeInitDma(Adc_HwUnitType HwUnitId);

/**
 * @brief Enable DMA for ADC conversion
 * @param[in] HwUnitId ADC hardware unit ID
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_EnableDma(Adc_HwUnitType HwUnitId);

/**
 * @brief Disable DMA for ADC conversion
 * @param[in] HwUnitId ADC hardware unit ID
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_DisableDma(Adc_HwUnitType HwUnitId);

/****************************************************************************************
*                              INTERRUPT FUNCTIONS                                    *
****************************************************************************************/
/**
 * @brief Enable ADC interrupts
 * @param[in] HwUnitId ADC hardware unit ID
 * @param[in] InterruptType Type of interrupt to enable @reg ADC_Hw_Interrupt_types ADC_HW.h
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_EnableInterrupt(Adc_HwUnitType HwUnitId, uint8 InterruptType);

/**
 * @brief Disable ADC interrupts
 * @param[in] HwUnitId ADC hardware unit ID
 * @param[in] InterruptType Type of interrupt to disable @reg ADC_Hw_Interrupt_types ADC_HW.h
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_DisableInterrupt(Adc_HwUnitType HwUnitId, uint8 InterruptType);

/**
 * @brief ADC interrupt service routine
 * @param[in] HwUnitId ADC hardware unit ID
 * @return void
 */
void AdcHw_InterruptHandler(ADC_TypeDef* ADCx, Adc_HwUnitType HwUnitId);

/**
 * @brief DMA interrupt service routine
 * @param[in] HwUnitId ADC hardware unit ID
 * @return void
 */
void AdcHw_DmaInterruptHandler(DMA_Channel_TypeDef* DMAx, Adc_HwUnitType HwUnitId);

/****************************************************************************************
*                              QUEUE MANAGEMENT FUNCTIONS                             *
****************************************************************************************/
#if (ADC_ENABLE_QUEUING == STD_ON)
/**
 * @brief Add group to conversion queue
 * @param[in] HwUnitId ADC hardware unit ID
 * @param[in] GroupId ADC group ID
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_AddGroupToQueue(Adc_HwUnitType HwUnitId, Adc_GroupType GroupId);

/**
 * @brief Remove group from conversion queue
 * @param[in] HwUnitId ADC hardware unit ID
 * @param[in] GroupId ADC group ID
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_RemoveGroupFromQueue(Adc_HwUnitType HwUnitId, Adc_GroupType GroupId);

/**
 * @brief Get next group from queue
 * @param[in] HwUnitId ADC hardware unit ID
 * @return Group ID or ADC_INVALID_GROUP_ID if queue is empty
 */
Adc_GroupType AdcHw_GetNextGroupFromQueue(Adc_HwUnitType HwUnitId);

/**
 * @brief Check if group is in queue
 * @param[in] HwUnitId ADC hardware unit ID
 * @param[in] GroupId ADC group ID
 * @return index of group in queue , if not in queue return ADC_INVALID_GROUP_ID
 */
Adc_GroupType AdcHw_IsGroupInQueue(Adc_HwUnitType HwUnitId, Adc_GroupType GroupId);

/**
 * @brief Clear conversion queue
 * @param[in] HwUnitId ADC hardware unit ID
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_ClearQueue(Adc_HwUnitType HwUnitId);
#endif /* ADC_ENABLE_QUEUING */

/****************************************************************************************
*                              STREAMING FUNCTIONS                                    *
****************************************************************************************/
//TODO: wil be use in ver 3.0
#if (ADC_ENABLE_STREAMING == STD_ON)
/**
 * @brief Configure streaming buffer
 * @param[in] HwUnitId ADC hardware unit ID
 * @param[in] GroupId ADC group ID
 * @param[in] BufferPtr Pointer to streaming buffer
 * @param[in] BufferSize Size of streaming buffer
 * @param[in] BufferMode Buffer mode (linear/circular)
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_ConfigureStreamingBuffer(Adc_HwUnitType HwUnitId, 
                                             Adc_GroupType GroupId,
                                             Adc_ValueGroupType* BufferPtr,
                                             uint16 BufferSize,
                                             Adc_StreamBufferModeType BufferMode);

/**
 * @brief Start streaming conversion
 * @param[in] HwUnitId ADC hardware unit ID
 * @param[in] GroupId ADC group ID
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_StartStreaming(Adc_HwUnitType HwUnitId, Adc_GroupType GroupId);

/**
 * @brief Stop streaming conversion
 * @param[in] HwUnitId ADC hardware unit ID
 * @param[in] GroupId ADC group ID
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_StopStreaming(Adc_HwUnitType HwUnitId, Adc_GroupType GroupId);

/**
 * @brief Get streaming buffer status
 * @param[in] HwUnitId ADC hardware unit ID
 * @param[in] GroupId ADC group ID
 * @param[out] FilledSamples Number of filled samples
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType AdcHw_GetStreamingStatus(Adc_HwUnitType HwUnitId, 
                                       Adc_GroupType GroupId, 
                                       uint16* FilledSamples);
#endif /* ADC_ENABLE_STREAMING */

/****************************************************************************************
*                              VALIDATION FUNCTIONS                                   *
****************************************************************************************/
/**
 * @brief Validate hardware unit ID
 * @param[in] HwUnitId ADC hardware unit ID
 * @return TRUE if valid, FALSE if invalid
 */
boolean AdcHw_ValidateHwUnit(Adc_HwUnitType HwUnitId);

/**
 * @brief Validate group ID
 * @param[in] GroupId ADC group ID
 * @return TRUE if valid, FALSE if invalid
 */
boolean AdcHw_ValidateGroup(Adc_GroupType GroupId);

/**
 * @brief Validate channel ID
 * @param[in] ChannelId ADC channel ID
 * @return TRUE if valid, FALSE if invalid
 */
boolean AdcHw_ValidateChannel(Adc_ChannelType ChannelId);

/****************************************************************************************
*                              UTILITY FUNCTIONS                                      *
****************************************************************************************/
/**
 * @brief Convert ADC channel ID to hardware channel number
 * @param[in] ChannelId ADC channel ID
 * @return Hardware channel number
 */
uint8 AdcHw_GetHwChannelNumber(Adc_ChannelType ChannelId);

/**
 * @brief Convert sampling time to hardware register value
 * @param[in] SampleTime Sampling time
 * @return Hardware register value
 */
uint32 AdcHw_GetHwSampleTime(Adc_SamplingTimeType SampleTime);

/**
 * @brief Get hardware trigger source
 * @param[in] TriggerSource Trigger source
 * @return Hardware trigger source value
 */
uint32 AdcHw_GetHwTriggerSource(Adc_TriggerSourceType TriggerSource);

/****************************************************************************************
*                              DEFERRED PROCESSING FUNCTIONS                          *
****************************************************************************************/
/**
 * @brief Main function for deferred processing
 * @return void
 * @note This function should be called periodically from the main loop
 */
void AdcHw_MainFunction(void);

/**
 * @brief Process completed conversions
 * @param[in] HwUnitId ADC hardware unit ID
 * @return void
 */
void AdcHw_ProcessCompletedConversions(Adc_HwUnitType HwUnitId);

/**
 * @brief Handle group completion
 * @param[in] HwUnitId ADC hardware unit ID
 * @param[in] GroupId ADC group ID
 * @return void
 */
void AdcHw_HandleGroupCompletion(Adc_HwUnitType HwUnitId, Adc_GroupType GroupId);

/****************************************************************************************
*                              CONSTANTS                                              *
****************************************************************************************/
#define ADC_INVALID_GROUP_ID        0xFFU
#define ADC_INVALID_CHANNEL_ID      0xFFU
#define ADC_INVALID_HW_UNIT_ID      0xFFU

/* @reg ADC_Hw_Interrupt_types */
#define ADC_INTERRUPT_EOC           (uint8)0x01U
#define ADC_INTERRUPT_DMA_TC        (uint8)0x02U
#define ADC_INTERRUPT_ERROR         (uint8)0x04U

/* Hardware limits */
#define ADC_HW_MAX_CHANNELS_PER_GROUP   16U
#define ADC_HW_MAX_CONVERSION_TIME_US   20U
#define ADC_HW_MAX_SAMPLING_CYCLES      239U

#endif /* ADC_HW_H */

/****************************************************************************************
*                                 END OF FILE                                          *
****************************************************************************************/
