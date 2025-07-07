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
static Adc_ConfigType* Adc_ConfigPtr = NULL_PTR; // Pointer to the current ADC configuration
/*******************************************************************************
* Core ADC Functions
*******************************************************************************/

/* Adc_Init function implementation */
/* Brief description of the function */
/* Parameters description */
/* Return value description */
/* Detailed explanation of implementation */
void Adc_Init(const Adc_ConfigType* ConfigPtr)
{
/*
 * Implementation Note for Adc_Init:
 *
 * 1. Error Handling:
 *    - Check if ADC driver is already initialized
 *    - If yes, report ADC_E_ALREADY_INITIALIZED and return
 *    - Validate ConfigPtr (not NULL)
 *
 * 2. Initialization Steps:
 *    - Store configuration pointer for later use
 *    - Initialize ADC state variables
 *    - Set all ADC groups to ADC_IDLE state
 *    - Disable all notifications
 *    - Disable hardware triggers if configured as active
 *
 * 3. Hardware Configuration:
 *    - Only initialize registers specific to ADC functionality
 *    - Don't touch I/O registers (PORT driver's responsibility)
 *    - Don't modify registers that affect multiple modules (MCU driver's responsibility)
 *    - Follow hardware-specific initialization sequence
 *    - Only configure resources specified in ConfigPtr
 *
 * 4. Completion:
 *    - Set driver state to initialized
 *    - Apply power management settings if configured
 */
// init AdcHw_CurrGroupId status

    if(ConfigPtr == NULL_PTR)
    {
        // Report error: ADC_E_PARAM_CONFIG
        return;
    }
    if(ConfigPtr->Initialized == 1)
    {
        // Report error: ADC_E_ALREADY_INITIALIZED
        return;
    }
    Adc_ConfigPtr = (Adc_ConfigType*)ConfigPtr; // Store the configuration pointer
    Adc_HwUnitDefType* AdcHwUnitConfig = Adc_ConfigPtr->HwUnits;
    Adc_GroupDefType* AdcGroupConfig = Adc_ConfigPtr->Groups;
    for(uint8 i = 0; i < Adc_ConfigPtr->NumHwUnits; i++)
    {
        AdcHwUnitConfig[i].AdcHw_CurrGroupRegisterId = 0; // this will give id of the group in queue will be work next
        AdcHwUnitConfig[i].AdcHw_NbrOfGroupRegister = 0; // number of group are in the list
        AdcHwUnitConfig[i].AdcHW_Nvic = ADC_HW_EOC;
    }
    for(uint8 i = 0; i < Adc_ConfigPtr->NumGroups; i++)
    {
        AdcGroupConfig[i].Adc_Status = ADC_IDLE; //Idle by default
        AdcGroupConfig[i].Adc_GroupReplacement = 0; // Disable Group Replacement by default
        AdcGroupConfig[i].Adc_SamplesResultCounter = 0;
        AdcGroupConfig[i].Adc_ChannelConversionId = 0;
        AdcGroupConfig[i].Adc_NotificationEnable = ADC_NOTIFICATION_DISABLE; // No buffer set by default
    }
    Adc_ConfigPtr->Initialized = 1; // Mark the ADC as initialized
}

Std_ReturnType Adc_SetupResultBuffer (Adc_GroupType Group,
                                      Adc_ValueGroupType* DataBufferPtr)
{
/**
 * @brief Sets up the result buffer for an ADC channel group
 *
 * Implementation notes:
 * 1. This function should initialize the result buffer pointer of the selected group
 *    with the address passed as parameter.
 * 2. Implementation must include the following error checks (if DET is enabled):
 *    - ADC_E_UNINIT: Check if ADC driver is initialized
 *    - ADC_E_PARAM_GROUP: Validate that the group ID exists
 *    - ADC_E_PARAM_POINTER: Verify DataBufferPtr is not NULL
 *    - ADC_E_BUSY: Check if group is in ADC_IDLE state, report runtime error otherwise
 * 
 * 3. External considerations (for environment code):
 *    - Ensure no group conversions start without buffer initialization
 *    - The buffer size must be adequate to hold all channel conversion results
 *    - For streaming access, buffer must accommodate multiple sets of results
 *      as specified by the streaming sample parameter
 * 
 * @param[in] Group ID of ADC channel group
 * @param[in] DataBufferPtr Pointer to result buffer
 * @return Std_ReturnType E_OK: Operation completed successfully
 *                        E_NOT_OK: Operation failed
 */
    Std_ReturnType retVal = E_NOT_OK;
    // check king errors 
    if(Group >= ADC_MAX_GROUPS)
    {
        // Report error: ADC_E_PARAM_GROUP
        retVal = E_NOT_OK;
    }
    else if(Adc_GroupConfig[Group].Adc_Status != ADC_IDLE)
    {
        // Report error: ADC_E_BUSY
        retVal = E_NOT_OK;
    }
    else if(DataBufferPtr == NULL_PTR)
    {
        // Report error: ADC_E_PARAM_POINTER
        retVal = E_NOT_OK;
    }
    else
    {
        // Set the result buffer pointer for the group
        Adc_GroupConfig[Group].Adc_ValueResultPtr = DataBufferPtr;
        retVal = E_OK; // Operation successful
    }
    return retVal;

}
/* Adc_DeInit function implementation */
/* Brief description of the function */
/* Parameters description */
/* Return value description */
/* Detailed explanation of implementation */
void Adc_DeInit(void)
{
    // Check if the ADC driver is initialized
    if (Adc_ConfigPtr == NULL_PTR)
    {
        // Report error: ADC_E_UNINIT
        return;
    }

    // Check if all groups are in IDLE or STREAM_COMPLETED state
    for (uint8 i = 0; i < ADC_MAX_GROUPS; i++)
    {
        if (Adc_GroupConfig[i].Adc_Status != ADC_IDLE && 
            Adc_GroupConfig[i].Adc_Status != ADC_STREAM_COMPLETED)
        {
            // Report runtime error: ADC_E_BUSY
            return;
        }
    }

    // Reset all hardware units to power-on state
    for (Adc_GroupType i = 0; i < Adc_ConfigPtr->NumHwUnits; i++)
    {
        ADC_TypeDef* ADCx = ADC_GET_HW_MODULE_ID(i);
    
        
        // Reset ADC registers
        ADC_DeInit(ADCx);
        
        // Disable ADC interrupts
        if(Adc_HwUnitConfig[i].AdcHw_DMAAvailable == 1)
        {
            DMA_Channel_TypeDef* DMAx = ADC_GET_HW_DMA_ID(i);
            AdcHw_DMADisable(ADCx, DMAx);
            AdcHw_DMADeInit(ADCx);
            AdcHw_NVICEnableDMA(ADCx);
        }
        AdcHw_NVICDisableInterrupt(ADCx);
        
        // Reset internal status variables
        Adc_HwUnitConfig[i].AdcHw_CurrGroupRegisterId = 0;
        Adc_HwUnitConfig[i].AdcHw_NbrOfGroupRegister = 0;
        Adc_HwUnitConfig[i].AdcHW_Nvic = ADC_HW_EOC;
    }

    // Reset all group configurations to default
    for (uint8 i = 0; i < Adc_ConfigPtr->NumGroups; i++)
    {
        Adc_GroupConfig[i].Adc_Status = ADC_IDLE;
        Adc_GroupConfig[i].Adc_GroupReplacement = 0;
        Adc_GroupConfig[i].Adc_SamplesResultCounter = 0;
        Adc_GroupConfig[i].Adc_ChannelConversionId = 0;
        Adc_GroupConfig[i].Adc_NotificationEnable = ADC_NOTIFICATION_DISABLE;
        Adc_GroupConfig[i].Adc_ValueResultPtr = NULL_PTR;
    }

    // Set driver to uninitialized state
    Adc_ConfigPtr = NULL_PTR;
}

/* Adc_StartGroupConversion function implementation */
/* Brief description of the function */
/* Parameters description */
/* Return value description */
/* Detailed explanation of implementation */
void Adc_StartGroupConversion (Adc_GroupType Group)
{
    
    if (Group >= ADC_MAX_GROUPS)
    {
        // Report error: ADC_E_PARAM_GROUP
        return;
    }
    Adc_GroupDefType* ADCGroup = &Adc_GroupConfig[Group];
    Adc_HwUnitDefType* ADCHwUnit = &Adc_HwUnitConfig[ADCGroup->Adc_HwUnitId];
    if(ADCHwUnit->AdcHw_HWTrigger == 1 || ADCGroup->Adc_Status != ADC_IDLE)
    {
        // Report error: ADC_E_BUSY
        return;
    }
    else if(ADCGroup->Adc_TriggerSource == ADC_TRIGG_SRC_HW )
    {
        // Report error: ADC_E_WRONG_TRIGG_SRC
        return;
    }
    ADC_TypeDef* ADCx = ADC_GET_HW_MODULE_ID(ADCGroup->Adc_HwUnitId);

    if(ADCHwUnit->AdcHw_PriorityEnable != ADC_PRIORITY_NONE) // priority enable not care queue
    {
        /* GroupConversion shall report a runtime error ADC_E_BUSY. ()
        Note: The condition that the group is not in state ADC_IDLE means in this context:
        The conversion of the same group is currently ongoing
        or
        A conversion request for the same group is already stored one time in the queue*/
        /* If the priority mechanism is enabled: when called while a group,
        which can be implicitly stopped, is not in state ADC_IDLE and not in state ADC_
        STREAM_COMPLETED, the function Adc_StartGroupConversion shall report a run
        time error ADC_E_BUSY. */
    }
    else if(ADCHwUnit->AdcHw_QueueEnable == 1 ) //queue enabled, priority disabled
    {
        // There is a conversion working
        if(AdcHw_RegisterGroupQueue(ADCHwUnit, Group) == E_NOT_OK)
        {
            // Report error: ADC_E_BUSY
            return;
        }
    }

    else if(ADCHwUnit->AdcHw_QueueEnable == 0 ) //queue disabled, priority disabled
        // if group in queue 
        // check queue 0 to get current thing 
    {
        if(AdcHw_RegisterGroupQueue(ADCHwUnit, Group) == E_NOT_OK)
        {
            // Report error: ADC_E_BUSY
            // conversion is ongoing
            // hw trigger is enable
            return;
        }
    }
    // Reset result buffer
    /* To go to this line it alreaty make sure that there are no conversion is ongoing or the queue is empty if enable*/
    AdcHw_ConfigSWConversion(ADCGroup->Adc_HwUnitId, Group);
}
/* SWS_Adc_00346 Note:
 * -----------------------------
 * When both priority mechanism and queuing are disabled: or enabled,
 * 
 * Adc_StartGroupConversion must return ADC_E_BUSY error if:
 * 1. Any group that can't be implicitly stopped is currently busy (converting)
 *    OR
 * 2. Any group that can't be implicitly stopped has HW trigger enabled
 * 
 * This prevents conflicts when multiple groups try to use the same ADC hardware
 * simultaneously without any arbitration mechanism (priority/queuing) in place.
 * 
 * Implementation must check all groups on the same hardware unit before
 * starting a new conversion.
 */
/* Implementation Guide for SWS_Adc_00426:
 * --------------------------------------
 * 1. In Adc_StartGroupConversion():
 *    - Get the hardware unit of the group to be started
 *    - Loop through all configured groups
 *    - For each group using the same HW unit:
 *      a. Check if it CAN be implicitly stopped using these criteria:
 *         - SW triggered + one-shot mode + single access mode OR
 *         - SW triggered + continuous mode + linear streaming mode OR
 *         - HW triggered + one-shot mode + linear streaming mode
 *      b. If it CAN be implicitly stopped, check if it's:
 *         - NOT in IDLE state AND
 *         - NOT in STREAM_COMPLETED state
 *      c. If conditions a+b are true, report ADC_E_BUSY and return E_NOT_OK
 * 
 * 2. Helper function to determine if a group can be implicitly stopped:
 *    bool Adc_GroupCanBeImplicitlyStopped(Adc_GroupType Group) {
 *      const Adc_GroupConfigType *config = &Adc_ConfigPtr->groupConfigPtr[Group];
 *      
 *      // Case 1: SW triggered + one-shot + single access
 *      if (config->AdcGroupTriggerSource == ADC_TRIGGER_SRC_SW &&
 *          config->AdcGroupConversionMode == ADC_GROUP_CONV_MODE_ONESHOT &&
 *          config->AdcGroupAccessMode == ADC_ACCESS_MODE_SINGLE) {
 *          return TRUE;
 *      }
 *      
 *      // Case 2: SW triggered + continuous + linear streaming
 *      if (config->AdcGroupTriggerSource == ADC_TRIGGER_SRC_SW &&
 *          config->AdcGroupConversionMode == ADC_GROUP_CONV_MODE_CONTINUOUS &&
 *          config->AdcGroupAccessMode == ADC_ACCESS_MODE_STREAMING &&
 *          config->AdcStreamBufferMode == ADC_STREAM_BUFFER_LINEAR) {
 *          return TRUE;
 *      }
 *      
 *      // Case 3: HW triggered + one-shot + linear streaming
 *      if (config->AdcGroupTriggerSource != ADC_TRIGGER_SRC_SW &&
 *          config->AdcGroupConversionMode == ADC_GROUP_CONV_MODE_ONESHOT &&
 *          config->AdcGroupAccessMode == ADC_ACCESS_MODE_STREAMING &&
 *          config->AdcStreamBufferMode == ADC_STREAM_BUFFER_LINEAR) {
 *          return TRUE;
 *      }
 *      
 *      return FALSE;
 *    }
 * 
 * 3. This check should be performed separately from SWS_Adc_00346, as they
 *    apply to different types of groups (can vs. cannot be implicitly stopped).
 *    
 * 4. The check should only be performed when both ADC_PRIORITY_IMPLEMENTATION 
 *    is set to ADC_PRIORITY_NONE and ADC_ENABLE_QUEUING is STD_OFF.
 */

// priority enabled: group can be implicity stopped, group state is not ADC_IDLE report ADC_E_BUSY
// note group not in ADC_IDLE mean : this conversion is currently ongoing or a conversion request is still pending

// priority enabled or priority disabled, queue enable not in ADC_IDLE nor ADC_STREAM_COMPLETED state report ADC_E_BUSY



//


void Adc_StopGroupConversion (Adc_GroupType Group)
{
    if (Group >= ADC_MAX_GROUPS)
    {
        // Report error: ADC_E_PARAM_GROUP
        return;
    }
    Adc_GroupDefType* ADCGroup = &Adc_GroupConfig[Group];

    if (ADCGroup == NULL_PTR)
    {
        // Report error: ADC_E_PARAM_GROUP
        return;
    }
    else if (ADCGroup->Adc_Status == ADC_IDLE || \
        ADCGroup->Adc_Status == ADC_STREAM_COMPLETED)
    {
        // Report error: ADC_E_IDLE
        return;
    }
    else if(ADCGroup->Adc_TriggerSource == ADC_TRIGG_SRC_HW )
    {
        // Report error: ADC_E_WRONG_TRIGG_SRC
        return;
    }
    // Stop the conversion process for the group
    ADCGroup->Adc_Status = ADC_IDLE; // Set group status to idle
    ADC_TypeDef* ADCx = ADC_GET_HW_MODULE_ID(ADCGroup->Adc_HwUnitId);
    Adc_HwUnitDefType* ADCHwUnit = &Adc_HwUnitConfig[ADCGroup->Adc_HwUnitId];

    // delete from register entry
    if(ADCGroup->Adc_GroupConvMode == ADC_CONV_MODE_CONTINUOUS 
        || ADCGroup->Adc_GroupConvMode == ADC_CONV_MODE_ONESHOT) // there are a group is activity add to queue list
    {
        AdcHw_SwStopConv(ADCx); // Stop the software conversion process
        AdcHw_RemoveGroupQueue(ADCHwUnit, Group);
    }
     
}
Std_ReturnType Adc_ReadGroup (Adc_GroupType Group,\
                               Adc_ValueGroupType* DataBufferPtr)
{
    if (Group >= ADC_MAX_GROUPS)
    {
        // Report error: ADC_E_PARAM_GROUP
        return E_NOT_OK;
    }
    Adc_GroupDefType* ADCGroup = &Adc_GroupConfig[Group];

    if (ADCGroup == NULL_PTR)
    {
        // Report error: ADC_E_PARAM_GROUP
        return E_NOT_OK;
    }
    else if (ADCGroup->Adc_Status ==  ADC_COMPLETED)
    {
        // Report error: ADC_E_BUSY
        return E_NOT_OK;
    }
    else if (ADCGroup->Adc_Status == ADC_IDLE)
    {
        // Report error: ADC_E_IDLE
        return E_NOT_OK;
    }
    if(ADCGroup->Adc_TriggerSource == ADC_TRIGG_SRC_SW)
    {
        Adc_StopGroupConversion(Group);
    }
    else 
    {
        /** Stop HW conversion is not supported for now */
    }
    /**
     * @brief Read group implementation 
     * @reqs : [SWS_Adc_00329] [SWS_Adc_00330] 
     * State transition handling:
     * - When group status is ADC_STREAM_COMPLETED:
     *   1. For continuous conversion modes (single access or circular streaming buffer)
     *      and hardware triggered groups in single/circular streaming access mode:
     *      State transitions to ADC_BUSY [SWS_Adc_00329]
     *
     *   2. For software triggered conversions that auto-stop (streaming with linear access
     *      or one-shot with single access) and hardware triggered conversions with linear
     *      streaming access: State transitions to ADC_IDLE [SWS_Adc_00330]
     */

    Adc_ChannelType Channels = ADCGroup->Adc_NbrOfChannel;
    Adc_StreamNumSampleType Samples = ADCGroup->Adc_StreamNumSamples;
    Adc_StreamNumSampleType LastConvIndex = ADCGroup->Adc_SamplesResultCounter - 1;
    for(uint8 i = 0; i < Channels ; i++)
    {
        // Read the conversion result for each channel in the group
        // This may involve reading from hardware registers or buffers
        // Ensure to handle the data correctly based on the group configuration
        DataBufferPtr[i] = ADCGroup->Adc_ValueResultPtr[LastConvIndex * Channels + i];
    }
    return E_OK;
}

/**
 * @brief  Enables the hardware trigger for the requested ADC Channel group
 * @param[in] Group: Numeric ID of the ADC channel group
 * @return  void
 * @reqs   SWS_Adc_91001
 */
void Adc_EnableHardwareTrigger (Adc_GroupType Group)
{
    /* This API is not work for now*/
}


void Adc_DisableHardwareTrigger (Adc_GroupType Group)
{
    /* This API is not work for now*/
}



void Adc_EnableGroupNotification (Adc_GroupType Group)
{
    /* This API is not work for now*/
    /* Todo Enable NVIC */
    /* Handle Streaming Conversion*/
}


void Adc_DisableGroupNotification (Adc_GroupType Group)
{
    /* This API is not work for now*/
    /* Todo Disable NVIC */
}



Adc_StreamNumSampleType Adc_GetStreamLastPointer (Adc_GroupType Group,
                                                  Adc_ValueGroupType** PtrToSamplePtr)
{
    return 1; // This function is not implemented yet
}

/* Adc_StopGroupConversion function implementation */
/* Brief description of the function */
/* Parameters description */
/* Return value description */
/* Detailed explanation of implementation */

/* Adc_ReadGroup function implementation */
/* Brief description of the function */
/* Parameters description */
/* Return value description */
/* Detailed explanation of implementation */

/* Adc_EnableGroupNotification function implementation */
/* Brief description of the function */
/* Parameters description */
/* Return value description */
/* Detailed explanation of implementation */

/* Adc_DisableGroupNotification function implementation */
/* Brief description of the function */
/* Parameters description */
/* Return value description */
/* Detailed explanation of implementation */

/* Adc_GetGroupStatus function implementation */
/* Brief description of the function */
/* Parameters description */
/* Return value description */
/* Detailed explanation of implementation */
Adc_StatusType Adc_GetGroupStatus (Adc_GroupType Group)
{
    if (Group >= ADC_MAX_GROUPS)
    {
        // Report error: ADC_E_PARAM_GROUP
        return 255;
    }
    Adc_GroupDefType* ADCGroup = &Adc_GroupConfig[Group];

    if (ADCGroup == NULL_PTR)
    {
        // Report error: ADC_E_PARAM_GROUP
        return 255;
    }
    return ADCGroup->Adc_Status; // Return the current status of the group
}
/*******************************************************************************
* Power Management Functions
*******************************************************************************/

// /* Adc_SetPowerState function implementation */
// Std_ReturnType Adc_SetPowerState (Adc_PowerStateRequestResultType* Result)
// {
//     /* This API is not work for now*/
// }
// /* Adc_GetCurrentPowerState function implementation */
// Std_ReturnType Adc_GetCurrentPowerState (Adc_PowerStateType* CurrentPowerState,
//                                         Adc_PowerStateRequestResultType* Result)
// {
//     /* This API is not work for now*/
// }

// /* Adc_GetTargetPowerState function implementation */
// Std_ReturnType Adc_GetTargetPowerState( Adc_PowerStateType* TargetPowerState,
//                                         Adc_PowerStateRequestResultType* Result)
// {
//     /* This API is not work for now*/
// }

// Std_ReturnType Adc_PreparePowerState( Adc_PowerStateType PowerState,
//                                      Adc_PowerStateRequestResultType* Result)
// {
//     /* This API is not work for now*/
// }
/*******************************************************************************
* Utility Functions
*******************************************************************************/

/* Adc_GetVersionInfo function implementation */
void Adc_GetVersionInfo (Std_VersionInfoType* versioninfo)
{
    versioninfo->vendorID = ADC_VENDOR_ID;
    versioninfo->moduleID = ADC_MODULE_ID;
    versioninfo->sw_major_version = ADC_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = ADC_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = ADC_SW_PATCH_VERSION;
}
/* Adc_MainFunction function implementation */
/* Brief description of the function */
/* Parameters description */
/* Return value description */
/* Detailed explanation of implementation */

/*******************************************************************************
* Static Helper Functions
*******************************************************************************/

/* Static helper function implementations */
/* Brief description of each function */
/* Parameters description */
/* Return value description */
/* Detailed explanation of implementation */

/****************************************************************************************
*                                 END OF FILE                                          *
****************************************************************************************/