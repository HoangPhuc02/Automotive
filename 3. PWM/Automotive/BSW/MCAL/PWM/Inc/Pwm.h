/****************************************************************************************
*                                 PWM.H                                                *
****************************************************************************************
* File Name   : Pwm.h
* Module      : Pulse Width Modulation (PWM)
* Description : AUTOSAR PWM driver main header file
* Version     : 1.0.0 - AUTOSAR compliant implementation
* Date        : 27/06/2025
* Author      : hoangphuc540202@gmail.com
* Github      : https://github.com/HoangPhuc02
***************************************************************************************/

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
#define PWM_INSTANCE_ID             0       /*!< PWM Driver Instance ID */

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
#define PWM_INIT_ID                    0x00    /*!< Service ID for Pwm_Init */
#define PWM_DEINIT_ID                  0x01    /*!< Service ID for Pwm_DeInit */
#define PWM_SET_DUTY_CYCLE_ID          0x02    /*!< Service ID for Pwm_SetDutyCycle */
#define PWM_SET_PERIOD_AND_DUTY_ID     0x03    /*!< Service ID for Pwm_SetPeriodAndDuty */
#define PWM_SET_OUTPUT_TO_IDLE_ID      0x04    /*!< Service ID for Pwm_SetOutputToIdle */
#define PWM_GET_OUTPUT_STATE_ID        0x05    /*!< Service ID for Pwm_GetOutputState */
#define PWM_DISABLE_NOTIFICATION_ID    0x06    /*!< Service ID for Pwm_DisableNotification */
#define PWM_ENABLE_NOTIFICATION_ID     0x07    /*!< Service ID for Pwm_EnableNotification */
#define PWM_GET_VERSION_INFO_ID        0x08    /*!< Service ID for Pwm_GetVersionInfo */

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
*                                 CORE API FUNCTIONS                                  *
****************************************************************************************/

/* === INITIALIZATION === */
/**
 * @brief Initialize PWM driver
 * @param[in] ConfigPtr Pointer to configuration set
 */
void Pwm_Init(const Pwm_ConfigType* ConfigPtr);

/**
 * @brief Deinitialize PWM driver  
 */
void Pwm_DeInit(void);

/* === BASIC CONTROL === */
/**
 * @brief Set PWM duty cycle
 * @param[in] ChannelNumber PWM channel (0 to PWM_MAX_CHANNELS-1)
 * @param[in] DutyCycle Duty cycle (0x0000=0% to 0x8000=100%)
 * @reqs 
 */
void Pwm_SetDutyCycle(Pwm_ChannelType ChannelNumber, uint16 DutyCycle);

/**
 * @brief Set both period and duty cycle
 * @param[in] ChannelNumber PWM channel
 * @param[in] Period Period in timer ticks
 * @param[in] DutyCycle Duty cycle (0x0000-0x8000)
 */
void Pwm_SetPeriodAndDuty(Pwm_ChannelType ChannelNumber, 
                          Pwm_PeriodType Period, 
                          uint16 DutyCycle);

/**
 * @brief Set channel output to idle state
 * @param[in] ChannelNumber PWM channel
 */
void Pwm_SetOutputToIdle(Pwm_ChannelType ChannelNumber);

/**
 * @brief Get current PWM output state
 * @param[in] ChannelNumber PWM channel
 * @return PWM_HIGH or PWM_LOW
 */
Pwm_OutputStateType Pwm_GetOutputState(Pwm_ChannelType ChannelNumber);

/****************************************************************************************
*                              UTILITY FUNCTIONS                                       *
****************************************************************************************/

/**
 * @brief Convert percentage to duty cycle
 * @param[in] Percentage 0-100%
 * @return Duty cycle value (0x0000-0x8000)
 */
uint16 Pwm_PercentageToDutyCycle(uint8 Percentage);

/**
 * @brief Convert duty cycle to percentage
 * @param[in] DutyCycle Duty cycle value (0x0000-0x8000)
 * @return Percentage 0-100%
 */
uint8 Pwm_DutyCycleToPercentage(uint16 DutyCycle);

/**
 * @brief Get version information
 * @param[out] versioninfo Version information structure
 */
void Pwm_GetVersionInfo(Std_VersionInfoType* versioninfo);

/****************************************************************************************
*                              GLOBAL VARIABLES                                        *
****************************************************************************************/

/* Driver state */
extern Pwm_DriverStateType Pwm_DriverState;

/* Configuration pointer */
extern const Pwm_ConfigType* Pwm_ConfigPtr;

#endif /* PWM_H */

/****************************************************************************************
*                                 END OF FILE                                          *
****************************************************************************************/
