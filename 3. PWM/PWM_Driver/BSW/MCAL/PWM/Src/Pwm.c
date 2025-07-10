/****************************************************************************************
*                                 PWM.C                                                *
****************************************************************************************
* File Name   : Pwm.c
* Module      : Pulse Width Modulation (PWM)
* Description : AUTOSAR PWM driver main implementation
* Version     : 1.0.0 - AUTOSAR compliant implementation
* Date        : 2025
* Author      : PWM Driver Team
* Repository  : PWM_Driver
****************************************************************************************/

/****************************************************************************************
*                                 INCLUDE FILES                                        *
****************************************************************************************/
#include "Pwm.h"
#include "Pwm_Hw.h"
#include "Det.h"
#include "SchM_Pwm.h"

/****************************************************************************************
*                              VERSION INFORMATION                                     *
****************************************************************************************/
#define PWM_SW_MAJOR_VERSION_C    1
#define PWM_SW_MINOR_VERSION_C    0
#define PWM_SW_PATCH_VERSION_C    0

/* Version compatibility checks */
#if (PWM_SW_MAJOR_VERSION != PWM_SW_MAJOR_VERSION_C)
    #error "Pwm.c: Version mismatch in Major version"
#endif

#if (PWM_SW_MINOR_VERSION != PWM_SW_MINOR_VERSION_C)
    #error "Pwm.c: Version mismatch in Minor version"
#endif

#if (PWM_SW_PATCH_VERSION != PWM_SW_PATCH_VERSION_C)
    #error "Pwm.c: Version mismatch in Patch version"
#endif

/****************************************************************************************
*                              GLOBAL VARIABLES                                        *
****************************************************************************************/

/**
 * @brief PWM driver state
 * @details Indicates whether the PWM driver is initialized or not
 */
Pwm_StateType Pwm_DriverState = PWM_UNINIT;

/**
 * @brief PWM configuration pointer
 * @details Pointer to the current PWM configuration
 */
const Pwm_ConfigType* Pwm_ConfigPtr = NULL_PTR;

/**
 * @brief PWM notification functions
 * @details Array of notification function pointers for each channel
 */
Pwm_NotificationFunctionType Pwm_NotificationFunctions[PWM_MAX_CHANNELS];

/****************************************************************************************
*                              LOCAL FUNCTION PROTOTYPES                              *
****************************************************************************************/

#if (PWM_DEV_ERROR_DETECT == STD_ON)
static Std_ReturnType Pwm_ValidateInit(const Pwm_ConfigType* ConfigPtr, uint8 ServiceId);
static Std_ReturnType Pwm_ValidateChannel(Pwm_ChannelType ChannelId, uint8 ServiceId);
static Std_ReturnType Pwm_ValidateChannelClass(Pwm_ChannelType ChannelId, uint8 ServiceId);
static Std_ReturnType Pwm_ValidatePointer(const void* Pointer, uint8 ServiceId);
static Std_ReturnType Pwm_ValidateDutyCycle(uint16 DutyCycle, uint8 ServiceId);
static Std_ReturnType Pwm_ValidatePeriod(Pwm_PeriodType Period, uint8 ServiceId);
#endif

/****************************************************************************************
*                              INITIALIZATION FUNCTIONS                               *
****************************************************************************************/

/**
 * @brief Service for PWM initialization
 * @details [SWS_Pwm_00095] Definition of API function Pwm_Init
 * @param[in] ConfigPtr Pointer to configuration set
 * @return void
 * @ServiceID 0x00
 * @Sync Synchronous
 * @Reentrancy Non Reentrant
 */
void Pwm_Init(const Pwm_ConfigType* ConfigPtr)
{
    uint8 ChannelIndex;
    uint8 HwUnitIndex;
    
#if (PWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if driver is already initialized */
    if (Pwm_DriverState == PWM_INIT)
    {
        (void)Det_ReportError(PWM_MODULE_ID, PWM_INSTANCE_ID, PWM_INIT_SID, PWM_E_ALREADY_INITIALIZED);
        return;
    }
    
    /* Validate configuration pointer */
    if (Pwm_ValidateInit(ConfigPtr, PWM_INIT_SID) != E_OK)
    {
        return;
    }
#endif
    
    /* Store configuration pointer */
    Pwm_ConfigPtr = ConfigPtr;
    
    /* Initialize notification functions array */
    for (ChannelIndex = 0; ChannelIndex < PWM_MAX_CHANNELS; ChannelIndex++)
    {
        Pwm_NotificationFunctions[ChannelIndex] = NULL_PTR;
    }
    
    /* Initialize hardware units */
    for (HwUnitIndex = 0; HwUnitIndex < ConfigPtr->PwmMaxHwUnits; HwUnitIndex++)
    {
        const Pwm_HwUnitConfigType* HwUnitConfig = &ConfigPtr->PwmHwUnitConfig[HwUnitIndex];
        
        /* Check if hardware unit is enabled */
        if (PWM_HW_IS_TIMER_ENABLED(HwUnitConfig->HwUnit))
        {
            (void)Pwm_Hw_InitHwUnit(HwUnitConfig->HwUnit, HwUnitConfig);
        }
    }
    
    /* Initialize PWM channels */
    for (ChannelIndex = 0; ChannelIndex < ConfigPtr->PwmMaxChannels; ChannelIndex++)
    {
        const Pwm_ChannelConfigType* ChannelConfig = &ConfigPtr->PwmChannelConfig[ChannelIndex];
        
        /* Check if channel's hardware unit is enabled */
        if (PWM_HW_IS_TIMER_ENABLED(ChannelConfig->HwUnit))
        {
            (void)Pwm_Hw_InitChannel(ChannelConfig->ChannelId, ChannelConfig);
            
            /* Set notification function if configured */
            if (ChannelConfig->NotificationPtr != NULL_PTR)
            {
                Pwm_NotificationFunctions[ChannelConfig->ChannelId] = ChannelConfig->NotificationPtr;
            }
        }
    }
    
    /* Set driver state to initialized */
    Pwm_DriverState = PWM_INIT;
}

/**
 * @brief Service for PWM De-Initialization
 * @details [SWS_Pwm_00096] Definition of API function Pwm_DeInit
 * @return void
 * @ServiceID 0x01
 * @Sync Synchronous
 * @Reentrancy Non Reentrant
 */
void Pwm_DeInit(void)
{
    uint8 HwUnitIndex;
    uint8 ChannelIndex;
    
#if (PWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if driver is initialized */
    if (Pwm_DriverState == PWM_UNINIT)
    {
        (void)Det_ReportError(PWM_MODULE_ID, PWM_INSTANCE_ID, PWM_DEINIT_SID, PWM_E_UNINIT);
        return;
    }
#endif
    
    /* Set all channels to idle state */
    for (ChannelIndex = 0; ChannelIndex < PWM_MAX_CHANNELS; ChannelIndex++)
    {
        (void)Pwm_Hw_SetOutputToIdle(ChannelIndex);
        (void)Pwm_Hw_DisableNotification(ChannelIndex);
    }
    
    /* De-initialize hardware units */
    for (HwUnitIndex = 0; HwUnitIndex < PWM_MAX_HW_UNITS; HwUnitIndex++)
    {
        (void)Pwm_Hw_DeInitHwUnit(HwUnitIndex);
    }
    
    /* Clear notification functions array */
    for (ChannelIndex = 0; ChannelIndex < PWM_MAX_CHANNELS; ChannelIndex++)
    {
        Pwm_NotificationFunctions[ChannelIndex] = NULL_PTR;
    }
    
    /* Clear configuration pointer */
    Pwm_ConfigPtr = NULL_PTR;
    
    /* Set driver state to uninitialized */
    Pwm_DriverState = PWM_UNINIT;
}

/****************************************************************************************
*                              CHANNEL CONTROL FUNCTIONS                              *
****************************************************************************************/

/**
 * @brief Service to set the duty cycle of the PWM channel
 * @details [SWS_Pwm_00097] Definition of API function Pwm_SetDutyCycle
 * @param[in] ChannelId Numeric identifier of the PWM channel
 * @param[in] DutyCycle Min=0x0000 Max=0x8000
 * @return void
 * @ServiceID 0x02
 * @Sync Synchronous
 * @Reentrancy Reentrant for different channel numbers
 */
void Pwm_SetDutyCycle(Pwm_ChannelType ChannelId, uint16 DutyCycle)
{
#if (PWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if driver is initialized */
    if (Pwm_DriverState == PWM_UNINIT)
    {
        (void)Det_ReportError(PWM_MODULE_ID, PWM_INSTANCE_ID, PWM_SET_DUTY_CYCLE_SID, PWM_E_UNINIT);
        return;
    }
    
    /* Validate channel ID */
    if (Pwm_ValidateChannel(ChannelId, PWM_SET_DUTY_CYCLE_SID) != E_OK)
    {
        return;
    }
    
    /* Validate duty cycle */
    if (Pwm_ValidateDutyCycle(DutyCycle, PWM_SET_DUTY_CYCLE_SID) != E_OK)
    {
        return;
    }
#endif
    
    /* Enter critical section */
    SchM_Enter_Pwm_PWM_EXCLUSIVE_AREA_00();
    
    /* Set duty cycle */
    (void)Pwm_Hw_SetDutyCycle(ChannelId, DutyCycle);
    
    /* Exit critical section */
    SchM_Exit_Pwm_PWM_EXCLUSIVE_AREA_00();
}

#if (PWM_SET_PERIOD_AND_DUTY_API == STD_ON)
/**
 * @brief Service to set the period and the duty cycle of a PWM channel
 * @details [SWS_Pwm_00098] Definition of API function Pwm_SetPeriodAndDuty
 * @param[in] ChannelId Numeric identifier of the PWM channel
 * @param[in] Period Period of the PWM signal
 * @param[in] DutyCycle Min=0x0000 Max=0x8000
 * @return void
 * @ServiceID 0x03
 * @Sync Synchronous
 * @Reentrancy Reentrant for different channel numbers
 */
void Pwm_SetPeriodAndDuty(Pwm_ChannelType ChannelId, Pwm_PeriodType Period, uint16 DutyCycle)
{
    const Pwm_ChannelConfigType* ChannelConfig;
    
#if (PWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if driver is initialized */
    if (Pwm_DriverState == PWM_UNINIT)
    {
        (void)Det_ReportError(PWM_MODULE_ID, PWM_INSTANCE_ID, PWM_SET_PERIOD_AND_DUTY_SID, PWM_E_UNINIT);
        return;
    }
    
    /* Validate channel ID */
    if (Pwm_ValidateChannel(ChannelId, PWM_SET_PERIOD_AND_DUTY_SID) != E_OK)
    {
        return;
    }
    
    /* Validate period */
    if (Pwm_ValidatePeriod(Period, PWM_SET_PERIOD_AND_DUTY_SID) != E_OK)
    {
        return;
    }
    
    /* Validate duty cycle */
    if (Pwm_ValidateDutyCycle(DutyCycle, PWM_SET_PERIOD_AND_DUTY_SID) != E_OK)
    {
        return;
    }
    
    /* Check if channel supports variable period */
    if (Pwm_ValidateChannelClass(ChannelId, PWM_SET_PERIOD_AND_DUTY_SID) != E_OK)
    {
        return;
    }
#endif
    
    /* Get channel configuration */
    ChannelConfig = Pwm_GetChannelConfig(ChannelId);
    
    /* Check if channel class allows period change */
    if (ChannelConfig->ChannelClass != PWM_VARIABLE_PERIOD)
    {
#if (PWM_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(PWM_MODULE_ID, PWM_INSTANCE_ID, PWM_SET_PERIOD_AND_DUTY_SID, PWM_E_PERIOD_UNCHANGEABLE);
#endif
        return;
    }
    
    /* Enter critical section */
    SchM_Enter_Pwm_PWM_EXCLUSIVE_AREA_01();
    
    /* Set period and duty cycle */
    (void)Pwm_Hw_SetPeriodAndDuty(ChannelId, Period, DutyCycle);
    
    /* Exit critical section */
    SchM_Exit_Pwm_PWM_EXCLUSIVE_AREA_01();
}
#endif /* PWM_SET_PERIOD_AND_DUTY_API */

#if (PWM_SET_OUTPUT_TO_IDLE_API == STD_ON)
/**
 * @brief Service to set the PWM output to the configured Idle state
 * @details [SWS_Pwm_00099] Definition of API function Pwm_SetOutputToIdle
 * @param[in] ChannelId Numeric identifier of the PWM channel
 * @return void
 * @ServiceID 0x04
 * @Sync Synchronous
 * @Reentrancy Reentrant for different channel numbers
 */
void Pwm_SetOutputToIdle(Pwm_ChannelType ChannelId)
{
#if (PWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if driver is initialized */
    if (Pwm_DriverState == PWM_UNINIT)
    {
        (void)Det_ReportError(PWM_MODULE_ID, PWM_INSTANCE_ID, PWM_SET_OUTPUT_TO_IDLE_SID, PWM_E_UNINIT);
        return;
    }
    
    /* Validate channel ID */
    if (Pwm_ValidateChannel(ChannelId, PWM_SET_OUTPUT_TO_IDLE_SID) != E_OK)
    {
        return;
    }
#endif
    
    /* Enter critical section */
    SchM_Enter_Pwm_PWM_EXCLUSIVE_AREA_02();
    
    /* Set output to idle */
    (void)Pwm_Hw_SetOutputToIdle(ChannelId);
    
    /* Exit critical section */
    SchM_Exit_Pwm_PWM_EXCLUSIVE_AREA_02();
}
#endif /* PWM_SET_OUTPUT_TO_IDLE_API */

#if (PWM_GET_OUTPUT_STATE_API == STD_ON)
/**
 * @brief Service to read the internal state of the PWM output signal
 * @details [SWS_Pwm_00100] Definition of API function Pwm_GetOutputState
 * @param[in] ChannelId Numeric identifier of the PWM channel
 * @return PWM_HIGH The PWM output state is high
 * @return PWM_LOW The PWM output state is low
 * @ServiceID 0x05
 * @Sync Synchronous
 * @Reentrancy Reentrant
 */
Pwm_OutputStateType Pwm_GetOutputState(Pwm_ChannelType ChannelId)
{
    Pwm_OutputStateType OutputState = PWM_LOW;
    
#if (PWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if driver is initialized */
    if (Pwm_DriverState == PWM_UNINIT)
    {
        (void)Det_ReportError(PWM_MODULE_ID, PWM_INSTANCE_ID, PWM_GET_OUTPUT_STATE_SID, PWM_E_UNINIT);
        return PWM_LOW;
    }
    
    /* Validate channel ID */
    if (Pwm_ValidateChannel(ChannelId, PWM_GET_OUTPUT_STATE_SID) != E_OK)
    {
        return PWM_LOW;
    }
#endif
    
    /* Get output state */
    OutputState = Pwm_Hw_GetOutputState(ChannelId);
    
    return OutputState;
}
#endif /* PWM_GET_OUTPUT_STATE_API */

/****************************************************************************************
*                              NOTIFICATION FUNCTIONS                                 *
****************************************************************************************/

#if (PWM_NOTIFICATION_SUPPORTED == STD_ON)
/**
 * @brief Service to disable the PWM signal edge notification
 * @details [SWS_Pwm_00101] Definition of API function Pwm_DisableNotification
 * @param[in] ChannelId Numeric identifier of the PWM channel
 * @return void
 * @ServiceID 0x06
 * @Sync Synchronous
 * @Reentrancy Reentrant for different channel numbers
 */
void Pwm_DisableNotification(Pwm_ChannelType ChannelId)
{
#if (PWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if driver is initialized */
    if (Pwm_DriverState == PWM_UNINIT)
    {
        (void)Det_ReportError(PWM_MODULE_ID, PWM_INSTANCE_ID, PWM_DISABLE_NOTIFICATION_SID, PWM_E_UNINIT);
        return;
    }
    
    /* Validate channel ID */
    if (Pwm_ValidateChannel(ChannelId, PWM_DISABLE_NOTIFICATION_SID) != E_OK)
    {
        return;
    }
#endif
    
    /* Enter critical section */
    SchM_Enter_Pwm_PWM_EXCLUSIVE_AREA_03();
    
    /* Disable notification */
    (void)Pwm_Hw_DisableNotification(ChannelId);
    
    /* Exit critical section */
    SchM_Exit_Pwm_PWM_EXCLUSIVE_AREA_03();
}

/**
 * @brief Service to enable the PWM signal edge notification according to notification parameter
 * @details [SWS_Pwm_00102] Definition of API function Pwm_EnableNotification
 * @param[in] ChannelId Numeric identifier of the PWM channel
 * @param[in] Notification Type of notification
 * @return void
 * @ServiceID 0x07
 * @Sync Synchronous
 * @Reentrancy Reentrant for different channel numbers
 */
void Pwm_EnableNotification(Pwm_ChannelType ChannelId, Pwm_EdgeNotificationType Notification)
{
#if (PWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if driver is initialized */
    if (Pwm_DriverState == PWM_UNINIT)
    {
        (void)Det_ReportError(PWM_MODULE_ID, PWM_INSTANCE_ID, PWM_ENABLE_NOTIFICATION_SID, PWM_E_UNINIT);
        return;
    }
    
    /* Validate channel ID */
    if (Pwm_ValidateChannel(ChannelId, PWM_ENABLE_NOTIFICATION_SID) != E_OK)
    {
        return;
    }
    
    /* Validate notification parameter */
    if ((Notification != PWM_RISING_EDGE) && (Notification != PWM_FALLING_EDGE) && (Notification != PWM_BOTH_EDGES))
    {
        (void)Det_ReportError(PWM_MODULE_ID, PWM_INSTANCE_ID, PWM_ENABLE_NOTIFICATION_SID, PWM_E_PARAM_VALUE);
        return;
    }
#endif
    
    /* Enter critical section */
    SchM_Enter_Pwm_PWM_EXCLUSIVE_AREA_04();
    
    /* Enable notification */
    (void)Pwm_Hw_EnableNotification(ChannelId, Notification);
    
    /* Exit critical section */
    SchM_Exit_Pwm_PWM_EXCLUSIVE_AREA_04();
}
#endif /* PWM_NOTIFICATION_SUPPORTED */

/****************************************************************************************
*                              VERSION INFORMATION FUNCTIONS                          *
****************************************************************************************/

#if (PWM_VERSION_INFO_API == STD_ON)
/**
 * @brief Service returns the version information of this module
 * @details [SWS_Pwm_00103] Definition of API function Pwm_GetVersionInfo
 * @param[out] VersionInfo Pointer to where to store the version information
 * @return void
 * @ServiceID 0x08
 * @Sync Synchronous
 * @Reentrancy Reentrant
 */
void Pwm_GetVersionInfo(Std_VersionInfoType* VersionInfo)
{
#if (PWM_DEV_ERROR_DETECT == STD_ON)
    /* Validate pointer */
    if (Pwm_ValidatePointer(VersionInfo, PWM_GET_VERSION_INFO_SID) != E_OK)
    {
        return;
    }
#endif
    
    /* Fill version information */
    VersionInfo->vendorID = PWM_VENDOR_ID;
    VersionInfo->moduleID = PWM_MODULE_ID;
    VersionInfo->sw_major_version = PWM_SW_MAJOR_VERSION;
    VersionInfo->sw_minor_version = PWM_SW_MINOR_VERSION;
    VersionInfo->sw_patch_version = PWM_SW_PATCH_VERSION;
}
#endif /* PWM_VERSION_INFO_API */

/****************************************************************************************
*                              INTERRUPT SERVICE ROUTINES                             *
****************************************************************************************/

/**
 * @brief PWM notification handler
 * @details Called from interrupt service routine when PWM notification occurs
 * @param[in] ChannelId Channel identifier that generated the notification
 * @return void
 */
void Pwm_NotificationHandler(Pwm_ChannelType ChannelId)
{
    /* Check if channel is valid and notification function is configured */
    if ((ChannelId < PWM_MAX_CHANNELS) && (Pwm_NotificationFunctions[ChannelId] != NULL_PTR))
    {
        /* Call notification function */
        Pwm_NotificationFunctions[ChannelId]();
    }
}

/****************************************************************************************
*                              VALIDATION FUNCTIONS                                   *
****************************************************************************************/

#if (PWM_DEV_ERROR_DETECT == STD_ON)
/**
 * @brief Validates PWM initialization
 * @param[in] ConfigPtr Configuration pointer to validate
 * @param[in] ServiceId Service ID for error reporting
 * @return E_OK if valid, E_NOT_OK otherwise
 */
static Std_ReturnType Pwm_ValidateInit(const Pwm_ConfigType* ConfigPtr, uint8 ServiceId)
{
    Std_ReturnType RetVal = E_OK;
    
    /* Check for NULL pointer */
    if (ConfigPtr == NULL_PTR)
    {
        (void)Det_ReportError(PWM_MODULE_ID, PWM_INSTANCE_ID, ServiceId, PWM_E_PARAM_CONFIG);
        RetVal = E_NOT_OK;
    }
    else
    {
        /* Validate configuration */
        if (Pwm_ValidateConfig(ConfigPtr) != E_OK)
        {
            (void)Det_ReportError(PWM_MODULE_ID, PWM_INSTANCE_ID, ServiceId, PWM_E_PARAM_CONFIG);
            RetVal = E_NOT_OK;
        }
    }
    
    return RetVal;
}

/**
 * @brief Validates PWM channel ID
 * @param[in] ChannelId Channel ID to validate
 * @param[in] ServiceId Service ID for error reporting
 * @return E_OK if valid, E_NOT_OK otherwise
 */
static Std_ReturnType Pwm_ValidateChannel(Pwm_ChannelType ChannelId, uint8 ServiceId)
{
    Std_ReturnType RetVal = E_OK;
    
    /* Check channel ID range */
    if (ChannelId >= PWM_MAX_CHANNELS)
    {
        (void)Det_ReportError(PWM_MODULE_ID, PWM_INSTANCE_ID, ServiceId, PWM_E_PARAM_CHANNEL);
        RetVal = E_NOT_OK;
    }
    
    return RetVal;
}

/**
 * @brief Validates PWM channel class for period change
 * @param[in] ChannelId Channel ID to validate
 * @param[in] ServiceId Service ID for error reporting
 * @return E_OK if valid, E_NOT_OK otherwise
 */
static Std_ReturnType Pwm_ValidateChannelClass(Pwm_ChannelType ChannelId, uint8 ServiceId)
{
    Std_ReturnType RetVal = E_OK;
    const Pwm_ChannelConfigType* ChannelConfig;
    
    /* Get channel configuration */
    ChannelConfig = Pwm_GetChannelConfig(ChannelId);
    
    /* Check if channel supports variable period */
    if ((ChannelConfig != NULL_PTR) && (ChannelConfig->ChannelClass != PWM_VARIABLE_PERIOD))
    {
        (void)Det_ReportError(PWM_MODULE_ID, PWM_INSTANCE_ID, ServiceId, PWM_E_PERIOD_UNCHANGEABLE);
        RetVal = E_NOT_OK;
    }
    
    return RetVal;
}

/**
 * @brief Validates pointer parameter
 * @param[in] Pointer Pointer to validate
 * @param[in] ServiceId Service ID for error reporting
 * @return E_OK if valid, E_NOT_OK otherwise
 */
static Std_ReturnType Pwm_ValidatePointer(const void* Pointer, uint8 ServiceId)
{
    Std_ReturnType RetVal = E_OK;
    
    /* Check for NULL pointer */
    if (Pointer == NULL_PTR)
    {
        (void)Det_ReportError(PWM_MODULE_ID, PWM_INSTANCE_ID, ServiceId, PWM_E_PARAM_POINTER);
        RetVal = E_NOT_OK;
    }
    
    return RetVal;
}

/**
 * @brief Validates duty cycle value
 * @param[in] DutyCycle Duty cycle to validate
 * @param[in] ServiceId Service ID for error reporting
 * @return E_OK if valid, E_NOT_OK otherwise
 */
static Std_ReturnType Pwm_ValidateDutyCycle(uint16 DutyCycle, uint8 ServiceId)
{
    Std_ReturnType RetVal = E_OK;
    
    /* Check duty cycle range */
    if (DutyCycle > 0x8000)
    {
        (void)Det_ReportError(PWM_MODULE_ID, PWM_INSTANCE_ID, ServiceId, PWM_E_DUTY_CYCLE_OUT_OF_RANGE);
        RetVal = E_NOT_OK;
    }
    
    return RetVal;
}

/**
 * @brief Validates period value
 * @param[in] Period Period to validate
 * @param[in] ServiceId Service ID for error reporting
 * @return E_OK if valid, E_NOT_OK otherwise
 */
static Std_ReturnType Pwm_ValidatePeriod(Pwm_PeriodType Period, uint8 ServiceId)
{
    Std_ReturnType RetVal = E_OK;
    
    /* Check period range */
    if ((Period < PWM_MIN_PERIOD) || (Period > PWM_MAX_PERIOD))
    {
        (void)Det_ReportError(PWM_MODULE_ID, PWM_INSTANCE_ID, ServiceId, PWM_E_PERIOD_OUT_OF_RANGE);
        RetVal = E_NOT_OK;
    }
    
    return RetVal;
}
#endif /* PWM_DEV_ERROR_DETECT */
