/****************************************************************************************
*                                ADC.C                                                  *
****************************************************************************************
* File Name   : Adc.c
* Module      : Analog to Digital Converter (ADC)
* Description : AUTOSAR ADC driver source file 
* Version     : 1.0.0
* Date        : 24/06/2025
* Author      : hoangphuc540202@gmail.com
* Github      : https://github.com/HoangPhuc02
****************************************************************************************/

/****************************************************************************************
*                                 INCLUDE FILES                                        *
****************************************************************************************/
#include "Adc.h"
#include "Adc_Cfg.h"
#include "Adc_Types.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "misc.h"   // For NVIC configuration
/* Include error detection module if enabled */
/* Include other required headers */

/****************************************************************************************
*                                 GLOBAL VARIABLES                                     *
****************************************************************************************/
/* ADC Driver State Variable */
/* ADC Group Status Variables */
/* ADC Notification Status Variables */
/* ADC Configuration Pointer */
/* ADC Power State Variables */
/* Other required global variables */
/****************************************************************************************
*                              STATIC FUNCTION DECLARATIONS                            *
****************************************************************************************/
static inline uint8 AdcHw_HandleChannelSequencing(Adc_GroupDefType* AdcGroup, ADC_TypeDef* ADCx);
static void AdcHw_ProcessConversionComplete(Adc_HwUnitType Unit);
static void AdcHw_CleanCurrentGroupConversion(Adc_HwUnitType Unit, Adc_GroupType Group);
static inline void AdcHw_CallNotification(Adc_GroupType Group);
static inline void AdcHw_NVICDisableInterrupt(ADC_TypeDef* ADCx);
static inline void AdcHw_NVICEnableInterrupt(ADC_TypeDef* ADCx);
static inline void AdcHw_DMADisable(ADC_TypeDef* ADCx, DMA_Channel_TypeDef* DMAx);
static inline void AdcHw_DMAEnable(ADC_TypeDef* ADCx, DMA_Channel_TypeDef* DMAx);
static inline void AdcHw_DMAInit(ADC_TypeDef* ADCx, DMA_Channel_TypeDef* DMAx, Adc_ValueGroupType* buffer, Adc_StreamNumSampleType size, Adc_StreamBufferModeType mode);
static inline void AdcHw_DMADeInit(DMA_Channel_TypeDef* DMAx);
static inline void AdcHw_NVICEnableDMA(ADC_TypeDef* ADCx, DMA_Channel_TypeDef* DMAx);
static inline void AdcHw_NVICDisableDMA(ADC_TypeDef* ADCx, DMA_Channel_TypeDef* DMAx);
static inline void AdcHW_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);
static inline void AdcHw_Enable(ADC_TypeDef* ADCx);
static inline void AdcHw_ConfigSWConversion(Adc_HwUnitType Unit, Adc_GroupType Group);
static inline Std_ReturnType AdcHw_RegisterGroupQueue(Adc_HwUnitType Unit, Adc_GroupType Group);
/****************************************************************************************
*                            INTERRUPT SCHEDULER FUNCTION                               *
****************************************************************************************/

/* Global flag for deferred processing */
static volatile uint8 AdcHw_DeferredProcessingFlag[ADC_MAX_HW_UNITS] = {0};
static volatile Adc_HwUnitType AdcHw_PendingUnits[ADC_MAX_HW_UNITS];
static volatile uint8 AdcHw_PendingCount = 0;

/**
 * @brief Optimized ISR - Immediate channel sequencing + deferred group completion
 * @param Unit ADC hardware unit
 */
void AdcHw_Interrupt_Handler(Adc_HwUnitType Unit)
{
    Adc_HwUnitDefType* AdcHw = &Adc_HwUnitConfig[Unit];
    Adc_GroupType CurrGroup = AdcHw->AdcHw_GroupQueue[AdcHw->AdcHw_CurrGroupRegisterId];
    Adc_GroupDefType* AdcGroup = &Adc_GroupConfig[CurrGroup];
    ADC_TypeDef* ADCx = ADC_GET_HW_MODULE_ID(AdcHw->AdcHW_UnitId);
    
    /* CRITICAL: Time-sensitive operations in ISR context */
    
    /* 1. Read and store ADC result immediately */
    uint16 adcResult = ADC_GetConversionValue(ADCx);
    AdcGroup->Adc_ValueResultPtr[AdcGroup->Adc_SamplesResultCounter * AdcGroup->Adc_NbrOfChannel + 
                                 AdcGroup->Adc_ChannelConversionId] = adcResult;
    
    /* 2. Clear interrupt flag */
    ADC_ClearITPendingBit(ADCx, ADC_IT_EOC);
    
    /* 3. IMMEDIATE channel sequencing - cannot be delayed */
    uint8 groupCompleted = AdcHw_HandleChannelSequencing(AdcGroup, ADCx);
    
    /* 4. If group is completed, defer complex processing */
    if (groupCompleted) {
        AdcHw_DeferredProcessingFlag[Unit] = 1;
        if (AdcHw_PendingCount < ADC_MAX_HW_UNITS) {
            AdcHw_PendingUnits[AdcHw_PendingCount] = Unit;
            AdcHw_PendingCount++;
        }
    }
    
    /* Exit ISR - channel sequencing done, group completion deferred */
}

/**
 * @brief Handle immediate channel sequencing in ISR context
 * @param AdcGroup Current ADC group configuration
 * @param ADCx ADC hardware module
 * @return uint8 1 if group conversion completed, 0 if more conversions needed
 */
static inline uint8 AdcHw_HandleChannelSequencing(Adc_GroupDefType* AdcGroup, ADC_TypeDef* ADCx)
{
    uint8 groupCompleted = 0;
    
    /* Check if there are more channels in current sample */
    if (AdcGroup->Adc_ChannelConversionId < (AdcGroup->Adc_NbrOfChannel - 1)) {
        /* More channels to convert in this sample - IMMEDIATE START */
        AdcGroup->Adc_ChannelConversionId++;
        
        /* Configure and start next channel immediately */
        AdcHW_RegularChannelConfig(ADCx, 
                                AdcGroup->Adc_ChannelGroup[AdcGroup->Adc_ChannelConversionId].Adc_ChannelId,
                                1,
                                AdcGroup->Adc_ChannelGroup[AdcGroup->Adc_ChannelConversionId].Adc_SampleTime);
        
        /* Start next conversion immediately */
        ADC_SoftwareStartConvCmd(ADCx, ENABLE);
    }
    else {
        /* All channels done for this sample */
        AdcGroup->Adc_ChannelConversionId = 0;
        AdcGroup->Adc_SamplesResultCounter++;
        
        /* Check streaming mode behavior */
        if (AdcGroup->Adc_GroupAccessMode == ADC_ACCESS_MODE_STREAMING &&
            AdcGroup->Adc_StreamBufferMode == ADC_STREAM_BUFFER_CIRCULAR) {
            
            /* STREAMING CIRCULAR MODE - Never stops, never goes to next group */
            if (AdcGroup->Adc_SamplesResultCounter >= AdcGroup->Adc_StreamNumSamples) {
                /* Buffer full - wrap around to beginning (circular) */
                AdcGroup->Adc_SamplesResultCounter = 0;
                
                /* Signal group completion for notification but keep running */
                groupCompleted = 1;
            }
            
            /* Always continue with first channel of next sample */
            AdcHW_RegularChannelConfig(ADCx, 
                                    AdcGroup->Adc_ChannelGroup[0].Adc_ChannelId,
                                    1,
                                    AdcGroup->Adc_ChannelGroup[0].Adc_SampleTime);
        }
        else {
            /* NON-CIRCULAR MODES - Check if buffer is full */
            if (AdcGroup->Adc_SamplesResultCounter >= AdcGroup->Adc_StreamNumSamples) {
                /* Buffer full - stop and signal completion */
                AdcGroup->Adc_SamplesResultCounter = 0;
                groupCompleted = 1;
                /* Don't start next conversion - group is complete */
            }
            else {
                /* Continue with next sample */
                AdcHW_RegularChannelConfig(ADCx, 
                                        AdcGroup->Adc_ChannelGroup[0].Adc_ChannelId,
                                        1,
                                        AdcGroup->Adc_ChannelGroup[0].Adc_SampleTime);
            }
        }
    }
    
    return groupCompleted;
}

/**
 * @brief Deferred processing function - Call from main loop or low-priority task
 * This function should be called regularly from the main application loop
 */
void AdcHw_MainFunction(void)
{
    uint8 i;
    
    /* Process all pending units */
    for (i = 0; i < AdcHw_PendingCount; i++) {
        Adc_HwUnitType Unit = AdcHw_PendingUnits[i];
        
        if (AdcHw_DeferredProcessingFlag[Unit]) {
            /* Disable interrupts for atomic access */
            __disable_irq();
            AdcHw_DeferredProcessingFlag[Unit] = 0;
            __enable_irq();
            
            /* Now perform all the complex processing outside ISR context */
            AdcHw_ProcessConversionComplete(Unit);
        }
    }
    
    /* Reset pending count */
    AdcHw_PendingCount = 0;
}
/**
 * @brief Call notification callback
 * @param Group ADC group ID
 */
static inline void AdcHw_CallNotification(Adc_GroupType Group)
{
    if (ADC_NOTIFICATION_ENABLE == Adc_GroupConfig[Group].Adc_NotificationEnable) {
        Adc_GroupConfig[Group].Adc_NotificationCb();
    }
}

/**
 * @brief Process conversion completion - Called from main context
 * @param Unit ADC hardware unit
 */
static void AdcHw_ProcessConversionComplete(Adc_HwUnitType Unit)
{
    Adc_HwUnitDefType* AdcHw = &Adc_HwUnitConfig[Unit];
    Adc_GroupType CurrGroup = AdcHw->AdcHw_GroupQueue[AdcHw->AdcHw_CurrGroupRegisterId];
    Adc_GroupDefType* AdcGroup = &Adc_GroupConfig[CurrGroup];
    uint8 NextConversion = 0;
    /* Call notification */
    AdcHw_CallNotification(CurrGroup);
    /* Handle different completion modes */
    if (AdcGroup->Adc_GroupAccessMode == ADC_ACCESS_MODE_STREAMING &&
        AdcGroup->Adc_StreamBufferMode == ADC_STREAM_BUFFER_CIRCULAR) {
        
        /* STREAMING CIRCULAR MODE - Never stops, never goes to next group */
        NextConversion = 0;  /* Never advance to next group */
        AdcGroup->Adc_Status = ADC_COMPLETED;  /* Buffer completed but still running */
        /* Do NOT call AdcHw_HandleNextConversion - stay on current group forever */
    }

    else {
        /* ONE-SHOT or other modes - can go to next group */
        NextConversion = 1;
        AdcGroup->Adc_Status = ADC_STREAM_COMPLETED;
        /* Clean current group conversion and check for next conversion */
        AdcHw_CleanCurrentGroupConversion(Unit, CurrGroup);
    }

    if(NextConversion && AdcHw->AdcHw_QueueEnable) {
        /* Handle queue management for next conversion */
        if (AdcHw->AdcHw_NbrOfGroupRegister > 0) {
            AdcHw_ConfigConversion(Unit, AdcHw->AdcHw_CurrGroupRegisterId);
        }
    }
}




/****************************************************************************************
*                                   SCHEDULER FUNCTION                                  *
****************************************************************************************/
/**
 * @brief Clean current group conversion data and reset state
 * @param Unit ADC hardware unit
 * @param Group ADC group to clean
 */
static void AdcHw_CleanCurrentGroupConversion(Adc_HwUnitType Unit, Adc_GroupType Group)
{
    Adc_HwUnitDefType* AdcHw = &Adc_HwUnitConfig[Unit];
    Adc_GroupType id = AdcHw->AdcHw_CurrGroupRegisterId;
    Adc_GroupType size = AdcHw->AdcHw_GroupQueueSize;
    Adc_GroupType group_find = size;
    Adc_GroupType reg = AdcHw->AdcHw_NbrOfGroupRegister;
    Adc_GroupType check = id + reg;

    
    /* If Queue mechanism is disable */
    if(AdcHw->AdcHw_QueueEnable == 0)
    {
        if(AdcHw->AdcHw_GroupQueue[0] == Group) /* If group is already in list*/
        {
            AdcHw->AdcHw_GroupQueue[0] = 0; /* Clean first group*/
            AdcHw->AdcHw_CurrGroupRegisterId = 0;
        }
    }
    /* If Queue mechanism is enable */
    /* Delete finished conversion */
    else if(AdcHw->AdcHw_GroupQueue[id] == Group)
    {
        AdcHw->AdcHw_GroupQueue[id] = 0;
        AdcHw->AdcHw_CurrGroupRegisterId = (++id == size) ? id : 0;
    }
    /* Delete a request pending*/
    /* If number of request pending + current id < size*/
    else if(check < size)
    {
        for(Adc_GroupType i = id; i < check; i++) {
            if (AdcHw->AdcHw_GroupQueue[i] == Group) {
                group_find = i;
            }
            if(group_find != size)
                AdcHw->AdcHw_GroupQueue[i]= AdcHw->AdcHw_GroupQueue[i+1];
        }
    } 
    else 
    {
        for(Adc_GroupType i = id; i < size; i++) {
            if (AdcHw->AdcHw_GroupQueue[i] == Group) {
                group_find = i;
            }
            if(group_find != size)
            {
                if (i == size - 1)  // last element
                    AdcHw->AdcHw_GroupQueue[i] = 0; // clear last element
                else AdcHw->AdcHw_GroupQueue[i]= AdcHw->AdcHw_GroupQueue[i+1];
            }
        }
        for(Adc_GroupType i = 0; i < size - check; i++) {
            if (AdcHw->AdcHw_GroupQueue[i] == Group) {
                group_find = id;
            }
            if(group_find != size)
            {
                if(i+1 == id )  // last element, this queue is full 
                    AdcHw->AdcHw_GroupQueue[i] = 0; // clear last element
                else AdcHw->AdcHw_GroupQueue[i]= AdcHw->AdcHw_GroupQueue[i+1];
            }
        }
    }
    AdcHw->AdcHw_NbrOfGroupRegister--;
}

// to go to this code make sure that there is no conversion on this adc hw
static void AdcHw_ConfigSWConversion(Adc_HwUnitType Unit, 
                                     Adc_GroupType Group)
{
    ADC_TypeDef* ADCx = ADC_GET_HW_MODULE_ID(Adc_HwUnitConfig[Unit].AdcHW_UnitId);
    DMA_Channel_TypeDef* DMAx = ADC_GET_HW_DMA_ID(Adc_HwUnitConfig[Unit].AdcHW_UnitId);
    /* 1. Reset buffer*/
    memset(Adc_GroupConfig[Group].Adc_ValueResultPtr, 0,\
           Adc_GroupConfig[Group].Adc_ValueResultSize * sizeof(Adc_ValueGroupType));
    
    /* 3. Configure hardware*/
    AdcHw_ModuleConfig(Group);
    AdcHw_ChannelConfig(Group);
    // should be regist in to queue
    // check priority like priority mechanisim
    if(Adc_GroupConfig[Group].Adc_NbrOfChannel == 1 &&
       Adc_GroupConfig[Group].Adc_GroupAccessMode == ADC_ACCESS_MODE_SINGLE &&
       Adc_GroupConfig[Group].Adc_GroupConvMode == ADC_CONV_MODE_ONESHOT)
    {
        // if adc hw dma is disable
        if(Adc_HwUnitConfig[Unit].AdcHW_Nvic == ADC_HW_DMA)
        {
            AdcHw_NVICDisableDMA(ADCx,DMAx);
            Adc_HwUnitConfig[Unit].AdcHW_Nvic = ADC_HW_EOC;  // Fixed: assignment instead of comparison
        }
    }
    else if (Adc_HwUnitConfig[Unit].AdcHw_DMAAvailable == 1) // some module dont have dma
    {
        AdcHw_DMAInit(ADCx, DMAx\
                    Adc_GroupConfig[Group].Adc_ValueResultPtr, \
                    Adc_GroupConfig[Group].Adc_ValueResultSize, \
                    Adc_GroupConfig[Group].Adc_StreamBufferMode);
        // if adc hw dma is disable
        if(Adc_HwUnitConfig[Unit].AdcHW_Nvic != ADC_HW_DMA)
        {
            AdcHw_NVICEnableDMA(ADCx);
            Adc_HwUnitConfig[Unit].AdcHW_Nvic = ADC_HW_DMA; 
        }
    }
    // In case this module dont have dma
    else
    {
        // if adc hw dma is disable
        if(Adc_HwUnitConfig[Unit].AdcHW_Nvic == ADC_HW_DMA)
        {
            AdcHw_NVICDisableDMA(ADCx);
            Adc_HwUnitConfig[Unit].AdcHW_Nvic = ADC_HW_EOC;  
        }
    }
    if(Adc_HwUnitConfig[Unit].AdcHW_Nvic == ADC_HW_DMA)
    {
        DMA_Cmd(DMAx, Enable);
        ADC_DMACmd(ADCx, ENABLE);
    }
    AdcHw_Enable(ADCx); // Enable ADC hardware module
    /* 2. Set state*/
    Adc_GroupConfig[Group].Adc_Status = ADC_BUSY; // Set group status to busy
    AdcHw_SwStartConv(ADCx);
}

Adc_GroupType AdcHw_CheckGroupRegisterExist(Adc_HwUnitType Unit, Adc_GroupType Group)
{
    Adc_HwUnitDefType* AdcHw = &Adc_HwUnitConfig[Unit];
    Adc_GroupType id = AdcHw->AdcHw_GroupQueueSize ;
    /* If this group is not in the queue it will return the size of queue */
    if(AdcHw->AdcHw_GroupQueueSize != AdcHw->AdcHw_NbrOfGroupRegister){
        for(Adc_GroupType i = 0; i < AdcHw->AdcHw_GroupQueueSize; i++) {
            if (AdcHw->AdcHw_GroupQueue[i] == Group) {
                id = i;
            }
        }
    }
    return id;
}
static inline Std_ReturnType AdcHw_RegisterGroupQueue(Adc_HwUnitType Unit, Adc_GroupType Group) 
{
    Std_ReturnType res = E_NOT_OK;
    if(Adc_HwUnitConfig[Unit].AdcHw_QueueEnable == 0)
    {
        if(Adc_HwUnitConfig[Unit].AdcHw_GroupQueue[0] == 0) // if group is in queue
        {
            Adc_HwUnitConfig[Unit].AdcHw_GroupQueue[0] = Group; // clear first element
            res = E_OK;
        }
    }
    else if(AdcHw_CheckGroupRegisterExist(Unit, Group) == Adc_HwUnitConfig[Unit].AdcHw_GroupQueueSize) {
        Adc_GroupType id = Adc_HwUnitConfig[Unit].AdcHw_NbrOfGroupRegister + Adc_HwUnitConfig[Unit].AdcHw_CurrGroupRegisterId;
        if (id >= Adc_HwUnitConfig[Unit].AdcHw_GroupQueueSize) {
            id = id - Adc_HwUnitConfig[Unit].AdcHw_GroupQueueSize ;
        }
        Adc_HwUnitConfig[Unit].AdcHw_GroupQueue[id] = Group;
        res = E_OK;
    }
    if(res == E_OK) Adc_HwUnitConfig[Unit].AdcHw_NbrOfGroupRegister++;
    return res;
}


//
static inline void AdcHw_CallNotification(Adc_GroupType Group)
{
    if(ADC_NOTIFICATION_ENABLE == Adc_GroupConfig[Group].Adc_NotificationEnable)
    {
        Adc_GroupConfig[Group].Adc_NotificationCb();
    }
}

/****************************************************************************************
*                              INTERRUPT IMPLEMENTATIONS                                 *
****************************************************************************************/
void AdcHw_NvicConfig(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    // Configure NVIC for ADC1 and ADC2
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_Init(&NVIC_InitStructure);
}

inline void AdcHw_DMAInit(ADC_TypeDef* ADCx, DMA_Channel_TypeDef* DMAx, 
                                 Adc_ValueGroupType* buffer, 
                                 Adc_StreamNumSampleType size, 
                                 Adc_StreamBufferModeType mode)
{
    AdcHw_DMADisable(ADCx, DMAx); // Ensure DMA is de-initialized before re-initialization
    DMA_InitTypeDef DMA_InitStructure;
    // Configure DMA for ADC1 and ADC2
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32)&ADCx->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32)buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = size;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = (mode == ADC_STREAM_BUFFER_CIRCULAR) ? DMA_Mode_Circular : DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMAx, &DMA_InitStructure);
}

inline void AdcHw_DMADeInit(DMA_Channel_TypeDef* DMAx)
{
    DMA_DeInit(DMAx);
}


inline void AdcHw_DMADisable(ADC_TypeDef* ADCx, DMA_Channel_TypeDef* DMAx)
{
    DMA_Cmd(DMAx, DISABLE);
    DMA_DeInit(DMAx);
    AdcHw_NVICDisableDMA(ADCx, DMAx);
}

inline void AdcHw_DMAEnable(ADC_TypeDef* ADCx, DMA_Channel_TypeDef* DMAx)
{
    // Enable DMA for ADC1 and ADC2
    ADC_DMACmd(ADCx, ENABLE);
    ADC_Cmd(ADCx, ENABLE);
    DMA_Cmd(DMAx, ENABLE);
}


inline void AdcHw_NVICDisableInterrupt(ADC_TypeDef* ADCx)
{
    ADC_ITConfig(ADCx, ADC_IT_EOC, DISABLE);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
}

inline void AdcHw_NVICEnableInterrupt(ADC_TypeDef* ADCx)
{
    ADC_ITConfig(ADCx, ADC_IT_EOC, ENABLE);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

inline void AdcHw_NVICEnableDMA(ADC_TypeDef* ADCx, DMA_Channel_TypeDef* DMAx)
{
    NVIC_EnableIRQ(DMAx);
    AdcHw_NVICDisableInterrupt(ADCx);
}

inline void AdcHw_NVICDisableDMA(ADC_TypeDef* ADCx, DMA_Channel_TypeDef* DMAx)
{
    AdcHw_NVICEnableInterrupt(ADCx);
    NVIC_DisableIRQ(DMAx);
}

/****************************************************************************************
*                              MODULE CONFIGURATION                                 *
****************************************************************************************/
void AdcHw_ModuleConfig(const Adc_GroupType group)
{
    // Hardware-specific configuration for the ADC group
    // This may include setting up registers, enabling channels, etc.
    // Ensure to follow the hardware manual for correct register settings
    // and initialization sequence.
    const Adc_GroupDefType* groupPtr = &Adc_GroupConfig[group];
    const Adc_HwUnitDefType* hwPtr = &Adc_HwUnitConfig[groupPtr->Adc_HwUnitId];
    ADC_InitTypeDef adc ;
    ADC_TypeDef* ADCx = ADC_GET_HW_MODULE_ID(groupPtr->Adc_HwUnitId);
    adc.ADC_Mode = ADC_Mode_Independent;    // Independent mode

    if(ADC_DMA_DISABLE == hwPtr->AdcHw_DMAAvailable )  // if hw dont have dma use it to simulate 
    {
        adc.ADC_ScanConvMode = DISABLE;
        adc.ADC_NbrOfChannel = 1 ;              // Number of channels to be converted
        if(groupPtr->Adc_NbrOfChannel == 1 && \
            groupPtr->Adc_GroupConvMode == ADC_CONV_MODE_ONESHOT && \
            groupPtr->Adc_GroupAccessMode == ADC_ACCESS_MODE_SINGLE)
        {
            adc.ADC_ContinuousConvMode = DISABLE; // Oneshot mode
        }
        else 
        {
            adc.ADC_ContinuousConvMode = ENABLE; // Multi channel conversion
        }

    }
        
    else
    {
        adc.ADC_NbrOfChannel = groupPtr->Adc_NbrOfChannel;              // Number of channels to be converted
        adc.ADC_ScanConvMode = (groupPtr->Adc_NbrOfChannel == 1) ? DISABLE : ENABLE;       // Multi channel conversion
        if(groupPtr->Adc_GroupConvMode == ADC_CONV_MODE_CONTINUOUS || \
            groupPtr->Adc_GroupAccessMode == ADC_ACCESS_MODE_STREAMING)
        {
            adc.ADC_ContinuousConvMode = ENABLE;
        }
        else 
        {
            adc.ADC_ContinuousConvMode = DISABLE;    //Continuous mode
        }
    }
        

    if(groupPtr->Adc_TriggerSource == ADC_TRIGG_SRC_SW)
    {
        adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // No external trigger
    }
    else{
        //do nothing now
    }
    adc.ADC_DataAlign = (groupPtr->Adc_ResultAlignment == ADC_ALIGN_RIGHT) ? ADC_DataAlign_Right : ADC_DataAlign_Left; // Right alignment
    ADC_Init(ADCx, &adc);
}
inline void AdcHW_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)
{
    ADC_RegularChannelConfig(ADCx, ADC_Channel, Rank, ADC_SampleTime);
}
Std_ReturnType AdcHW_ChannelConfig(const Adc_GroupType group,ADC_TypeDef* ADC)
{
    const Adc_GroupDefType* groupPtr = &Adc_GroupConfig[group];
    const Adc_HwUnitDefType* hwPtr = &Adc_HwUnitConfig[groupPtr->Adc_HwUnitId];
    Adc_StreamNumSampleType samples = groupPtr->Adc_StreamNumSamples;
    Adc_ChannelType channels = groupPtr->Adc_NbrOfChannel;
    Std_ReturnType res = E_NOT_OK;
    // Configure ADC channels based on the group configuration
    // This may involve setting up channel numbers, sample times, etc.
    // Ensure to follow the hardware manual for correct channel settings
    if(samples * channels > ADC_MAX_CHANNEL)
    {
        // Report error: ADC_E_PARAM_GROUP
        return res;
    }
    if(ADC_DMA_DISABLE == hwPtr->AdcHw_DMAAvailable )  // if hw dont have dma use it to simulate 
    {
        AdcHW_RegularChannelConfig(ADC, groupPtr->Adc_ChannelGroup[0].Adc_ChannelId, 1, groupPtr->Adc_ChannelGroup[0].Adc_SampleTime);
        res = E_OK;
    }
    else
    {
        for(uint8 i = 0; i < channels; i++)
        {
            AdcHW_RegularChannelConfig(ADC, groupPtr->Adc_ChannelGroup[i].Adc_ChannelId, i+1, groupPtr->Adc_ChannelGroup[i].Adc_SampleTime);
        }
        res = E_OK;
    }
    return res;
}

static inline void AdcHw_Enable(ADC_TypeDef* ADCx )
{
    // Configure the ADC hardware module and channels based on the group configuration
    // This may involve setting up registers, enabling channels, etc.
    // Ensure to follow the hardware manual for correct register settings
    // and initialization sequence.
    ADC_Cmd(ADCx, ENABLE);
    // Calibrate ADC
    ADC_ResetCalibration(ADCx);
    while(ADC_GetResetCalibrationStatus(ADCx));
    ADC_StartCalibration(ADCx);
    while(ADC_GetCalibrationStatus(ADCx));
}

// Start Conversion function
void AdcHw_SwStartConv(Adc_HwUnitType Unit)
{
    // Start the software conversion process
    // This may involve setting up registers, enabling interrupts, etc.
    // Ensure to follow the hardware manual for correct register settings
    // and initialization sequence.
    ADC_TypeDef* ADCx = ADC_GET_HW_MODULE_ID(Unit);
    ADC_Cmd(ADCx, ENABLE);
    ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}

// Stop Conversion function
void AdcHw_SwStopConv(Adc_HwUnitType Unit)
{
    ADC_TypeDef* ADCx = ADC_GET_HW_MODULE_ID(Unit);
    ADC_Cmd(ADCx, DISABLE);
    ADC_SoftwareStartConvCmd(ADCx, DISABLE);
    // Clear pending interrupts
    /*
    if(ADC_DMA_ENABLE == Adc_HwUnitConfig[Unit].AdcHw_DMAAvailable)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC1);
    }
    else 
    {
        ADC_ClearITPendingBit(ADCx, ADC_IT_EOC);
    }
    */
    // this should call the next conversion if queue enabeld

void AdcHw_HwStartConv(ADC_TypeDef* ADCx)
{
    ADC_Cmd(ADC1, ENABLE);
    ADC_ExternalTrigInjectedConvCmd(ADCx, ENABLE);
}