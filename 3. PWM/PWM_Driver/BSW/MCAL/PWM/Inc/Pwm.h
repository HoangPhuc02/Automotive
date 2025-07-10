/****************************************************************************************
*                                 PWM.H                                                *
****************************************************************************************
* File Name   : Pwm.h
* Module      : Pulse Width Modulation (PWM)
* Description : AUTOSAR PWM driver main header file
* Version     : 1.0.0 - AUTOSAR compliant implementation
* Date        : 2025
* Author      : PWM Driver Team
* Repository  : PWM_Driver
****************************************************************************************/

#ifndef PWM_H
#define PWM_H

/****************************************************************************************
*                                 INCLUDE FILES                                        *
****************************************************************************************/
#include "Std_Types.h"
#include "Pwm_Types.h"
#include "Pwm_Cfg.h"

/****************************************************************************************
*                              VERSION INFORMATION                                     *
****************************************************************************************/
#define PWM_VENDOR_ID               43      /*!< PWM Driver Vendor ID */
#define PWM_MODULE_ID               121     /*!< PWM Driver Module ID */

#define PWM_SW_MAJOR_VERSION        1       /*!< PWM Driver Major Version */
#define PWM_SW_MINOR_VERSION        0       /*!< PWM Driver Minor Version */
#define PWM_SW_PATCH_VERSION        0       /*!< PWM Driver Patch Version */

/* AUTOSAR Release Version */
#define PWM_AR_RELEASE_MAJOR_VERSION    4
#define PWM_AR_RELEASE_MINOR_VERSION    4
#define PWM_AR_RELEASE_REVISION_VERSION 0

/****************************************************************************************
*                              SERVICE IDS                                             *
****************************************************************************************/
#define PWM_INIT_SID                    0x00    /*!< Service ID for Pwm_Init */
#define PWM_DEINIT_SID                  0x01    /*!< Service ID for Pwm_DeInit */
#define PWM_SET_DUTY_CYCLE_SID          0x02    /*!< Service ID for Pwm_SetDutyCycle */
#define PWM_SET_PERIOD_AND_DUTY_SID     0x03    /*!< Service ID for Pwm_SetPeriodAndDuty */
#define PWM_SET_OUTPUT_TO_IDLE_SID      0x04    /*!< Service ID for Pwm_SetOutputToIdle */
#define PWM_GET_OUTPUT_STATE_SID        0x05    /*!< Service ID for Pwm_GetOutputState */
#define PWM_DISABLE_NOTIFICATION_SID    0x06    /*!< Service ID for Pwm_DisableNotification */
#define PWM_ENABLE_NOTIFICATION_SID     0x07    /*!< Service ID for Pwm_EnableNotification */
#define PWM_GET_VERSION_INFO_SID        0x08    /*!< Service ID for Pwm_GetVersionInfo */

/****************************************************************************************
*                              DEVELOPMENT ERROR CODES                                 *
****************************************************************************************/
#define PWM_E_PARAM_CONFIG              0x10    /*!< API called with wrong parameter */
#define PWM_E_UNINIT                    0x11    /*!< API called without module initialization */
#define PWM_E_PARAM_CHANNEL             0x12    /*!< API called with invalid channel identifier */
#define PWM_E_PERIOD_UNCHANGEABLE       0x13    /*!< Usage of unauthorized PWM service on PWM channel configured as fixed period */
#define PWM_E_ALREADY_INITIALIZED       0x14    /*!< PWM already initialized */
#define PWM_E_PARAM_POINTER             0x15    /*!< API called with invalid pointer */
#define PWM_E_PARAM_VALUE               0x16    /*!< API called with invalid parameter value */
#define PWM_E_TIMEOUT                   0x17    /*!< Operation timeout */
#define PWM_E_HW_FAILURE                0x18    /*!< Hardware failure */

/****************************************************************************************
*                              RUNTIME ERROR CODES                                     *
****************************************************************************************/
#define PWM_E_NOTIFICATION_OVERFLOW     0x19    /*!< Notification overflow */
#define PWM_E_DUTY_CYCLE_OUT_OF_RANGE   0x1A    /*!< Duty cycle out of range */
#define PWM_E_PERIOD_OUT_OF_RANGE       0x1B    /*!< Period out of range */

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
void Pwm_Init(const Pwm_ConfigType* ConfigPtr);

/**
 * @brief Service for PWM De-Initialization
 * @details [SWS_Pwm_00096] Definition of API function Pwm_DeInit
 * @return void
 * @ServiceID 0x01
 * @Sync Synchronous
 * @Reentrancy Non Reentrant
 */
void Pwm_DeInit(void);

/****************************************************************************************
*                              CHANNEL CONTROL FUNCTIONS                              *
****************************************************************************************/

/**
 * @brief Service sets the duty cycle of the PWM channel
 * @details [SWS_Pwm_91000] Definition of API function Pwm_SetDutyCycle
 * @param[in] ChannelNumber Numeric identifier of the PWM channel
 * @param[in] DutyCycle Min=0x0000 Max=0x8000
 * @return void
 * @ServiceID 0x02
 * @Sync Asynchronous
 * @Reentrancy Reentrant for different channel numbers
 */
void Pwm_SetDutyCycle(Pwm_ChannelType ChannelNumber, uint16 DutyCycle);

#if (PWM_SET_PERIOD_AND_DUTY_API == STD_ON)
/**
 * @brief Service sets the period and the duty cycle of a PWM channel
 * @details [SWS_Pwm_91001] Definition of API function Pwm_SetPeriodAndDuty
 * @param[in] ChannelNumber Numeric identifier of the PWM channel
 * @param[in] Period Period of the PWM signal
 * @param[in] DutyCycle Min=0x0000 Max=0x8000
 * @return void
 * @ServiceID 0x03
 * @Sync Asynchronous
 * @Reentrancy Reentrant for different channel numbers
 */
void Pwm_SetPeriodAndDuty(Pwm_ChannelType ChannelNumber, 
                          Pwm_PeriodType Period, 
                          uint16 DutyCycle);
#endif

#if (PWM_SET_OUTPUT_TO_IDLE_API == STD_ON)
/**
 * @brief Service sets the PWM output to the configured Idle state
 * @details [SWS_Pwm_91002] Definition of API function Pwm_SetOutputToIdle
 * @param[in] ChannelNumber Numeric identifier of the PWM channel
 * @return void
 * @ServiceID 0x04
 * @Sync Asynchronous
 * @Reentrancy Reentrant for different channel numbers
 */
void Pwm_SetOutputToIdle(Pwm_ChannelType ChannelNumber);
#endif

/****************************************************************************************
*                              STATUS FUNCTIONS                                        *
****************************************************************************************/

#if (PWM_GET_OUTPUT_STATE_API == STD_ON)
/**
 * @brief Service to read the internal state of the PWM output signal
 * @details [SWS_Pwm_00100] Definition of API function Pwm_GetOutputState
 * @param[in] ChannelNumber Numeric identifier of the PWM channel
 * @return Pwm_OutputStateType PWM_HIGH: High state, PWM_LOW: Low state
 * @ServiceID 0x05
 * @Sync Synchronous
 * @Reentrancy Reentrant for different channel numbers
 */
Pwm_OutputStateType Pwm_GetOutputState(Pwm_ChannelType ChannelNumber);
#endif

/****************************************************************************************
*                              NOTIFICATION FUNCTIONS                                  *
****************************************************************************************/

#if (PWM_NOTIFICATION_SUPPORTED == STD_ON)
/**
 * @brief Service to disable the PWM signal edge notification
 * @details [SWS_Pwm_91003] Definition of API function Pwm_DisableNotification
 * @param[in] ChannelNumber Numeric identifier of the PWM channel
 * @return void
 * @ServiceID 0x06
 * @Sync Asynchronous
 * @Reentrancy Reentrant for different channel numbers
 */
void Pwm_DisableNotification(Pwm_ChannelType ChannelNumber);

/**
 * @brief Service to enable the PWM signal edge notification
 * @details [SWS_Pwm_91004] Definition of API function Pwm_EnableNotification
 * @param[in] ChannelNumber Numeric identifier of the PWM channel
 * @param[in] Notification Type of notification (PWM_RISING_EDGE, PWM_FALLING_EDGE, PWM_BOTH_EDGES)
 * @return void
 * @ServiceID 0x07
 * @Sync Asynchronous
 * @Reentrancy Reentrant for different channel numbers
 */
void Pwm_EnableNotification(Pwm_ChannelType ChannelNumber, 
                           Pwm_EdgeNotificationType Notification);
#endif

/****************************************************************************************
*                              VERSION INFORMATION                                     *
****************************************************************************************/

#if (PWM_VERSION_INFO_API == STD_ON)
/**
 * @brief Service returns the version information of this module
 * @details [SWS_Pwm_00103] Definition of API function Pwm_GetVersionInfo
 * @param[out] versioninfo Pointer to where to store the version information
 * @return void
 * @ServiceID 0x08
 * @Sync Synchronous
 * @Reentrancy Reentrant
 */
void Pwm_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

/****************************************************************************************
*                              EXTENDED API FUNCTIONS                                  *
****************************************************************************************/

/**
 * @brief Start PWM channel output
 * @param[in] ChannelNumber Numeric identifier of the PWM channel
 * @return Std_ReturnType E_OK: Success, E_NOT_OK: Failed
 */
Std_ReturnType Pwm_StartChannel(Pwm_ChannelType ChannelNumber);

/**
 * @brief Stop PWM channel output
 * @param[in] ChannelNumber Numeric identifier of the PWM channel
 * @return Std_ReturnType E_OK: Success, E_NOT_OK: Failed
 */
Std_ReturnType Pwm_StopChannel(Pwm_ChannelType ChannelNumber);

/**
 * @brief Get current duty cycle of PWM channel
 * @param[in] ChannelNumber Numeric identifier of the PWM channel
 * @return Current duty cycle value (0x0000-0x8000)
 */
uint16 Pwm_GetCurrentDutyCycle(Pwm_ChannelType ChannelNumber);

/**
 * @brief Get current period of PWM channel
 * @param[in] ChannelNumber Numeric identifier of the PWM channel
 * @return Current period value
 */
Pwm_PeriodType Pwm_GetCurrentPeriod(Pwm_ChannelType ChannelNumber);

/**
 * @brief Check if PWM channel is running
 * @param[in] ChannelNumber Numeric identifier of the PWM channel
 * @return 1: Running, 0: Stopped
 */
uint8 Pwm_IsChannelRunning(Pwm_ChannelType ChannelNumber);

/**
 * @brief Set PWM frequency
 * @param[in] ChannelNumber Numeric identifier of the PWM channel
 * @param[in] Frequency Frequency in Hz
 * @return Std_ReturnType E_OK: Success, E_NOT_OK: Failed
 */
Std_ReturnType Pwm_SetFrequency(Pwm_ChannelType ChannelNumber, 
                                uint32 Frequency);

/**
 * @brief Get PWM frequency
 * @param[in] ChannelNumber Numeric identifier of the PWM channel
 * @return Current frequency in Hz
 */
uint32 Pwm_GetFrequency(Pwm_ChannelType ChannelNumber);

#if (PWM_ENABLE_PHASE_SHIFT == STD_ON)
/**
 * @brief Set phase shift between PWM channels
 * @param[in] ChannelNumber Numeric identifier of the PWM channel
 * @param[in] PhaseShift Phase shift value
 * @return Std_ReturnType E_OK: Success, E_NOT_OK: Failed
 */
Std_ReturnType Pwm_SetPhaseShift(Pwm_ChannelType ChannelNumber, 
                                 Pwm_PhaseShiftType PhaseShift);
#endif

/****************************************************************************************
*                              POWER STATE FUNCTIONS                                   *
****************************************************************************************/

#if (PWM_POWER_STATE_SUPPORTED == STD_ON)
/**
 * @brief Prepare power state transition
 * @param[in] PowerState Target power state
 * @param[out] Result Result of power state request
 * @return Std_ReturnType E_OK: Success, E_NOT_OK: Failed
 */
Std_ReturnType Pwm_PreparePowerState(Pwm_PowerStateType PowerState, 
                                     Pwm_PowerStateRequestResultType* Result);

/**
 * @brief Set power state
 * @param[in] PowerState Target power state
 * @param[out] Result Result of power state request
 * @return Std_ReturnType E_OK: Success, E_NOT_OK: Failed
 */
Std_ReturnType Pwm_SetPowerState(Pwm_PowerStateType PowerState, 
                                 Pwm_PowerStateRequestResultType* Result);

/**
 * @brief Get current power state
 * @param[out] CurrentPowerState Current power state
 * @param[out] TargetPowerState Target power state
 * @return Std_ReturnType E_OK: Success, E_NOT_OK: Failed
 */
Std_ReturnType Pwm_GetCurrentPowerState(Pwm_PowerStateType* CurrentPowerState, 
                                        Pwm_PowerStateType* TargetPowerState);

/**
 * @brief Get target power state
 * @param[out] TargetPowerState Target power state
 * @param[out] Result Result of power state request
 * @return Std_ReturnType E_OK: Success, E_NOT_OK: Failed
 */
Std_ReturnType Pwm_GetTargetPowerState(Pwm_PowerStateType* TargetPowerState, 
                                       Pwm_PowerStateRequestResultType* Result);
#endif

/****************************************************************************************
*                              UTILITY FUNCTIONS                                       *
****************************************************************************************/

/**
 * @brief Convert percentage to duty cycle value
 * @param[in] Percentage Percentage value (0-100)
 * @return Duty cycle value (0x0000-0x8000)
 */
uint16 Pwm_PercentageToDutyCycle(uint8 Percentage);

/**
 * @brief Convert duty cycle value to percentage
 * @param[in] DutyCycle Duty cycle value (0x0000-0x8000)
 * @return Percentage value (0-100)
 */
uint8 Pwm_DutyCycleToPercentage(uint16 DutyCycle);

/**
 * @brief Calculate period from frequency
 * @param[in] Frequency Frequency in Hz
 * @param[in] TimerClock Timer clock frequency in Hz
 * @return Period value
 */
Pwm_PeriodType Pwm_FrequencyToPeriod(uint32 Frequency, uint32 TimerClock);

/**
 * @brief Calculate frequency from period
 * @param[in] Period Period value
 * @param[in] TimerClock Timer clock frequency in Hz
 * @return Frequency in Hz
 */
uint32 Pwm_PeriodToFrequency(Pwm_PeriodType Period, uint32 TimerClock);

/****************************************************************************************
*                              GLOBAL VARIABLES                                        *
****************************************************************************************/

/* Driver state */
extern Pwm_DriverStateType Pwm_DriverState;

/* Configuration pointer */
extern const Pwm_ConfigType* Pwm_ConfigPtr;

/* Runtime channel data */
extern Pwm_RuntimeChannelType Pwm_RuntimeChannelData[PWM_MAX_CHANNELS];

/* Runtime hardware unit data */
extern Pwm_RuntimeHwUnitType Pwm_RuntimeHwUnitData[PWM_MAX_HW_UNITS];

#endif /* PWM_H */

/****************************************************************************************
*                                 END OF FILE                                          *
****************************************************************************************/
