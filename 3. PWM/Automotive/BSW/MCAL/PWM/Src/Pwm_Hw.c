/****************************************************************************************
*                                 PWM_HW.C                                             *
****************************************************************************************
* File Name   : Pwm_Hw.c
* Module      : Pulse Width Modulation (PWM)
* Description : AUTOSAR PWM driver hardware abstraction layer implementation
* Version     : 1.0.0 - AUTOSAR compliant implementation with STM32F10x SPL
* Date        : 27/06/2025
* Author      : hoangphuc540202@gmail.com
* Github      : https://github.com/HoangPhuc02
***************************************************************************************/

/****************************************************************************************
*                                 INCLUDE FILES                                        *
****************************************************************************************/
#include "Pwm_Hw.h"
#include "Pwm_Cfg.h"
#include "Det.h"


/****************************************************************************************
*                              LOCAL VARIABLES                                         *
****************************************************************************************/


/****************************************************************************************
*                              INITIALIZATION FUNCTIONS                               *
****************************************************************************************/

/**
 * @brief Initializes PWM hardware unit
 * @details Configures timer hardware for PWM operation
 * @param[in] HwUnit Hardware unit identifier
 * @param[in] ConfigPtr Pointer to hardware unit configuration
 * @return E_OK if initialization successful, E_NOT_OK otherwise
 */
Std_ReturnType PwmHw_InitHwUnit(Pwm_HwUnitType HwUnit, const Pwm_HwUnitConfigType* ConfigPtr)
{
    Std_ReturnType RetVal = E_OK;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    /* Validate parameters */
    if ((HwUnit >= PWM_MAX_HW_UNITS) || (ConfigPtr == NULL_PTR))
    {
        RetVal = E_NOT_OK;
    }
    else
    {
        /* Enable timer clock */
        PWM_HW_ENABLE_TIMER_CLOCK(HwUnit);
        TIM_TypeDef* TIM_Instance = PWM_HW_GET_TIMER(HwUnit);
        /* Initialize timer base configuration */
        TIM_TimeBaseStructure.TIM_Period = ConfigPtr->MaxPeriod - 1;
        TIM_TimeBaseStructure.TIM_Prescaler = ConfigPtr->Prescaler - 1;
        TIM_TimeBaseStructure.TIM_ClockDivision = ConfigPtr->ClockDivision;
        TIM_TimeBaseStructure.TIM_CounterMode = ConfigPtr->CounterMode;
        TIM_TimeBaseStructure.TIM_RepetitionCounter = ConfigPtr->RepetitionCounter;
        
        /* Configure timer */
        TIM_TimeBaseInit(TIM_Instance, &TIM_TimeBaseStructure);

        // /* Initialize runtime data */
        // Pwm_HwUnitRuntime[HwUnit].HwUnit = HwUnit;
        // Pwm_HwUnitRuntime[HwUnit].TimerInstance = ConfigPtr->TimerInstance;
        // Pwm_HwUnitRuntime[HwUnit].CurrentPeriod = ConfigPtr->MaxPeriod;
        // Pwm_HwUnitRuntime[HwUnit].IsInitialized = STD_ON;
        // Pwm_HwUnitRuntime[HwUnit].IsRunning = STD_OFF;
        // Pwm_HwUnitRuntime[HwUnit].ActiveChannels = 0;
        
        /* Enable ARR preload */
        TIM_ARRPreloadConfig(TIM_Instance, ENABLE);
        
        /* Enable timer */
        TIM_Cmd(TIM_Instance, ENABLE);
        
        /* Update runtime state */
        // Pwm_HwUnitRuntime[HwUnit].IsRunning = STD_ON;
    }
    
    return RetVal;
}

/**
 * @brief Deinitializes PWM hardware unit
 * @details Resets timer hardware and disables clocks
 * @param[in] HwUnit Hardware unit identifier
 * @return E_OK if deinitialization successful, E_NOT_OK otherwise
 */
Std_ReturnType PwmHw_DeInitHwUnit(Pwm_HwUnitType HwUnit)
{
    Std_ReturnType RetVal = E_OK;
    
    /* Validate parameters */
    if (HwUnit >= PWM_MAX_HW_UNITS)
    {
        RetVal = E_NOT_OK;
    }
    else
    {
        TIM_TypeDef* TIM_Instance = PWM_HW_GET_TIMER(HwUnit);
        /* Disable timer */
        if (TIM_Instance != NULL_PTR)
        {
            TIM_Cmd(TIM_Instance, DISABLE);
            TIM_DeInit(TIM_Instance);
        }
        
        /* Disable timer clock */
        PWM_HW_DISABLE_TIMER_CLOCK(HwUnit);
        
        /* Reset runtime data */
        // Pwm_HwUnitRuntime[HwUnit].HwUnit = PWM_HW_UNIT_INVALID;
        // Pwm_HwUnitRuntime[HwUnit].TimerInstance = NULL_PTR;
        // Pwm_HwUnitRuntime[HwUnit].CurrentPeriod = 0;
        // Pwm_HwUnitRuntime[HwUnit].IsInitialized = STD_OFF;
        // Pwm_HwUnitRuntime[HwUnit].IsRunning = STD_OFF;
        // Pwm_HwUnitRuntime[HwUnit].ActiveChannels = 0;
    }
    
    return RetVal;
}

/**
 * @brief Initializes PWM channel hardware
 * @details Configures GPIO pins and timer channels for PWM output
 * @param[in] ChannelId Channel identifier
 * @param[in] ChannelConfig Pointer to channel configuration
 * @return E_OK if initialization successful, E_NOT_OK otherwise
 */
Std_ReturnType PwmHw_InitChannel(Pwm_ChannelType ChannelId, const Pwm_ChannelConfigType* ChannelConfig)
{
    Std_ReturnType RetVal = E_OK;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    Pwm_HwUnitType HwUnitId;

    /* Validate parameters */
    if ((ChannelId >= PWM_MAX_CHANNELS) || (ChannelConfig == NULL_PTR))
    {
        RetVal = E_NOT_OK;
    }
    else
    {

        /* Get timer channel */
        HwUnitId = ChannelConfig->HwUnit;

        uint16 TIM_Channel = PWM_HW_GET_TIM_CHANNEL(ChannelId);
        TIM_TypeDef* TIM_Instance = PWM_HW_GET_TIMER(HwUnitId);

        /* Configure timer output compare */
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   // default
        TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
        TIM_OCInitStructure.TIM_Pulse = ChannelConfig->DutyCycle;
        TIM_OCInitStructure.TIM_OCPolarity = (ChannelConfig->Polarity == PWM_HIGH) ? TIM_OCPolarity_High : TIM_OCPolarity_Low;
        TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
        TIM_OCInitStructure.TIM_OCIdleState = (ChannelConfig->IdleState == PWM_HIGH) ? TIM_OCIdleState_Set : TIM_OCIdleState_Reset;
        TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
        
        /* Initialize output compare based on channel */
        switch (TIM_Channel)
        {
            case TIM_Channel_1:
                TIM_OC1Init(TIM_Instance, &TIM_OCInitStructure);
                TIM_OC1PreloadConfig(TIM_Instance, TIM_OCPreload_Enable);
                break;
            case TIM_Channel_2:
                TIM_OC2Init(TIM_Instance, &TIM_OCInitStructure);
                TIM_OC2PreloadConfig(TIM_Instance, TIM_OCPreload_Enable);
                break;
            case TIM_Channel_3:
                TIM_OC3Init(TIM_Instance, &TIM_OCInitStructure);
                TIM_OC3PreloadConfig(TIM_Instance, TIM_OCPreload_Enable);
                break;
            case TIM_Channel_4:
                TIM_OC4Init(TIM_Instance, &TIM_OCInitStructure);
                TIM_OC4PreloadConfig(TIM_Instance, TIM_OCPreload_Enable);
                break;
            default:
                RetVal = E_NOT_OK;
                break;
        }
        
        if (RetVal == E_OK)
        {
            /* Initialize channel runtime data */
            Pwm_ChannelConfig[ChannelId].NotificationEnabled = FALSE;
            

            /* Enable main output for advanced timers */
            if (ChannelConfig->HwUnit == PWM_HW_UNIT_TIM1)
            {
                TIM_CtrlPWMOutputs(TIM1, ENABLE);
            }
        }
    }
    
    return RetVal;
}

/****************************************************************************************
*                              DUTY CYCLE CONTROL FUNCTIONS                          *
****************************************************************************************/
#if (PWM_SET_DUTY_CYCLE_API == STD_ON)
/**
 * @brief Sets PWM duty cycle for a channel
 * @details Updates the compare value for the specified channel
 * @param[in] ChannelId Channel identifier
 * @param[in] DutyCycle New duty cycle value (0x0000 to 0x8000)
 * @return E_OK if successful, E_NOT_OK otherwise
 * @note validate in api  
 */
Std_ReturnType PwmHw_SetDutyCycle(Pwm_ChannelType ChannelId, uint16 DutyCycle)
{
    Std_ReturnType RetVal = E_OK;
    uint16 CompareValue;
    uint16 TIM_Channel = PWM_HW_GET_TIM_CHANNEL(ChannelId);
    TIM_TypeDef* TIM_Instance = PWM_HW_GET_TIMER(Pwm_ChannelConfig[ChannelId].HwUnit);
    /* Validate parameters */
    if ((ChannelId >= PWM_MAX_CHANNELS) || (DutyCycle > 0x8000))
    {
        RetVal = E_NOT_OK;
    }
    else
    {
        /* Calculate compare value */
        //  AbsoluteDutyCycle = ((uint32)AbsolutePeriodTime * RelativeDutyCycle) >> 15;
        // shift bits to left means divide it by 2^15
        // when u use duty cycle as max 0x8000 when u shift it will like 2^15 / 2^15 = 1 or 100%
        // when u want 50% which 0x4000 / 2^15 = 1/2 
        CompareValue = (uint16)(((uint32)(DutyCycle) * (uint32)(Pwm_ChannelConfig[ChannelId].Period)) >> 15);
        
        // TODO fix this 
        /* Update compare value based on channel */
        switch (TIM_Channel)
        {
            case TIM_Channel_1:
                TIM_SetCompare1(TIM_Instance, CompareValue);
                break;
            case TIM_Channel_2:
                TIM_SetCompare2(TIM_Instance, CompareValue);
                break;
            case TIM_Channel_3:
                TIM_SetCompare3(TIM_Instance, CompareValue);
                break;
            case TIM_Channel_4:
                TIM_SetCompare4(TIM_Instance, CompareValue);
                break;
            default:
                RetVal = E_NOT_OK;
                break;
        }
        
        if (RetVal == E_OK)
        {
            /* Update runtime data */
            Pwm_ChannelConfig[ChannelId].DutyCycle = DutyCycle;
        }
    }
    
    return RetVal;
}
#endif


#if (PWM_SET_PERIOD_AND_DUTY_API == STD_ON)
/**
 * @brief Sets PWM period and duty cycle for a channel
 * @details Updates both period and duty cycle for variable period channels
 * @param[in] ChannelId Channel identifier
 * @param[in] Period New period value
 * @param[in] DutyCycle New duty cycle value (0x0000 to 0x8000)
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType PwmHw_SetPeriodAndDuty(Pwm_ChannelType ChannelId, Pwm_PeriodType Period, uint16 DutyCycle)
{
    Std_ReturnType RetVal = E_OK;
    uint16 CompareValue;
    
    /* Validate parameters */
    if ((ChannelId >= PWM_MAX_CHANNELS) || (Period == 0) || (DutyCycle > 0x8000))
    {
        RetVal = E_NOT_OK;
    }
    else
    {
        uint16 TIM_Channel = PWM_HW_GET_TIM_CHANNEL(ChannelId);
        TIM_TypeDef* TIM_Instance = PWM_HW_GET_TIMER(Pwm_ChannelConfig[ChannelId].HwUnit);
        /* Update timer period */
        TIM_SetAutoreload(TIM_Instance, Period - 1);
        
        /* Calculate compare value */
        CompareValue = (uint16)(((uint32)(DutyCycle) * (uint32)(Pwm_ChannelConfig[ChannelId].Period)) >> 15);
        
        /* Update compare value based on channel */
        switch (TIM_Channel)
        {
            case TIM_Channel_1:
                TIM_SetCompare1(TIM_Instance, CompareValue);
                break;
            case TIM_Channel_2:
                TIM_SetCompare2(TIM_Instance, CompareValue);
                break;
            case TIM_Channel_3:
                TIM_SetCompare3(TIM_Instance, CompareValue);
                break;
            case TIM_Channel_4:
                TIM_SetCompare4(TIM_Instance, CompareValue);
                break;
            default:
                RetVal = E_NOT_OK;
                break;
        }
        
        if (RetVal == E_OK)
        {
            /* Update runtime data */
            Pwm_ChannelConfig[ChannelId].Period = Period;
            Pwm_ChannelConfig[ChannelId].DutyCycle = DutyCycle;
            Pwm_HwUnitConfig[Pwm_ChannelConfig[ChannelId].HwUnit].MaxPeriod = Period;
        }
    }
    
    return RetVal;
}
#endif
/****************************************************************************************
*                              OUTPUT CONTROL FUNCTIONS                               *
****************************************************************************************/

/**
 * @brief Sets PWM output to idle state
 * @details Forces the PWM output to the configured idle state
 * @param[in] ChannelId Channel identifier
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType PwmHw_SetOutputToIdle(Pwm_ChannelType ChannelId)
{
    Std_ReturnType RetVal = E_OK;
    
    /* Validate parameters */
    if (ChannelId >= PWM_MAX_CHANNELS)
    {
        RetVal = E_NOT_OK;
    }
    else
    {
        uint16 TIM_Channel = PWM_HW_GET_TIM_CHANNEL(ChannelId);
        TIM_TypeDef* TIM_Instance = PWM_HW_GET_TIMER(Pwm_ChannelConfig[ChannelId].HwUnit);
        /* Disable output compare based on channel */
        TIM_CCxCmd(TIM_Instance, TIM_Channel, TIM_CCx_Disable);
        
        
        // if (RetVal == E_OK)
        // {
        //     /* Update runtime state */
        //     Pwm_ChannelConfig[ChannelId].IsRunning = STD_OFF;
        // }
    }
    
    return RetVal;
}

/**
 * @brief Gets PWM output state
 * @details Returns the current logical state of the PWM output
 * @param[in] ChannelId Channel identifier
 * @return PWM_HIGH or PWM_LOW
 */
Pwm_OutputStateType PwmHw_GetOutputState(Pwm_ChannelType ChannelId)
{
    uint16 TIM_Channel = PWM_HW_GET_TIM_CHANNEL(ChannelId);
    TIM_TypeDef* TIM_Instance = PWM_HW_GET_TIMER(Pwm_ChannelConfig[ChannelId].HwUnit);
    uint16_t compare_value;
    Pwm_OutputStateType RetVal;
    switch (TIM_Channel)
    {
        case TIM_Channel_1: compare_value = TIM_Instance->CCR1; break;
        case TIM_Channel_2: compare_value = TIM_Instance->CCR2; break;
        case TIM_Channel_3: compare_value = TIM_Instance->CCR3; break;
        case TIM_Channel_4: compare_value = TIM_Instance->CCR4; break;
        default: return PWM_LOW;
    }
    if(Pwm_HwUnitConfig[Pwm_ChannelConfig[ChannelId].HwUnit].MaxPeriod == 1)
        RetVal = (compare_value > 0) ? PWM_HIGH : PWM_LOW;
    else
        RetVal = (compare_value == 0) ? PWM_LOW : PWM_HIGH;
    return RetVal;
}

/****************************************************************************************
*                              NOTIFICATION FUNCTIONS                                 *
****************************************************************************************/

/**
 * @brief Enables PWM notification for a channel
 * @details Enables interrupt generation for the specified channel
 * @param[in] ChannelId Channel identifier
 * @param[in] Notification Notification edge type
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType PwmHw_EnableNotification(Pwm_ChannelType ChannelId, Pwm_EdgeNotificationType Notification)
{
    Std_ReturnType RetVal = E_OK;
    uint16 TIM_Channel = PWM_HW_GET_TIM_CHANNEL(ChannelId);
    TIM_TypeDef* TIM_Instance = PWM_HW_GET_TIMER(Pwm_ChannelConfig[ChannelId].HwUnit);
    // check rising by using update or cc-capture compare ccr
    /* Validate parameters */
    if (ChannelId >= PWM_MAX_CHANNELS)
    {
        RetVal = E_NOT_OK;
    }
    else
    {
        if(Notification == PWM_RISING_EDGE || Notification == PWM_BOTH_EDGES)
        {

        }
        if(Notification == PWM_FALLING_EDGE || Notification == PWM_BOTH_EDGES)
        {

        }

        /* Enable interrupt based on channel */
        switch (TIM_Channel)
        {
            case TIM_Channel_1:
                TIM_ITConfig(TIM_Instance, TIM_IT_CC1, ENABLE);
                break;
            case TIM_Channel_2:
                TIM_ITConfig(TIM_Instance, TIM_IT_CC2, ENABLE);
                break;
            case TIM_Channel_3:
                TIM_ITConfig(TIM_Instance, TIM_IT_CC3, ENABLE);
                break;
            case TIM_Channel_4:
                TIM_ITConfig(TIM_Instance, TIM_IT_CC4, ENABLE);
                break;
            default:
                RetVal = E_NOT_OK;
                break;
        }
        
        if (RetVal == E_OK)
        {
            /* Update runtime data */
            Pwm_ChannelConfig[ChannelId].NotificationEnabled = STD_ON;
        }
    }
    
    return RetVal;
}

/**
 * @brief Disables PWM notification for a channel
 * @details Disables interrupt generation for the specified channel
 * @param[in] ChannelId Channel identifier
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType PwmHw_DisableNotification(Pwm_ChannelType ChannelId)
{
    Std_ReturnType RetVal = E_OK;
        uint16 TIM_Channel = PWM_HW_GET_TIM_CHANNEL(ChannelId);
    TIM_TypeDef* TIM_Instance = PWM_HW_GET_TIMER(Pwm_ChannelConfig[ChannelId].HwUnit);
    /* Validate parameters */
    if (ChannelId >= PWM_MAX_CHANNELS)
    {
        RetVal = E_NOT_OK;
    }
    else
    {
        /* Disable interrupt based on channel */
        switch (TIM_Channel)
        {
            case TIM_Channel_1:
                TIM_ITConfig(TIM_Instance, TIM_IT_CC1, DISABLE);
                break;
            case TIM_Channel_2:
                TIM_ITConfig(TIM_Instance, TIM_IT_CC2, DISABLE);
                break;
            case TIM_Channel_3:
                TIM_ITConfig(TIM_Instance, TIM_IT_CC3, DISABLE);
                break;
            case TIM_Channel_4:
                TIM_ITConfig(TIM_Instance, TIM_IT_CC4, DISABLE);
                break;
            default:
                RetVal = E_NOT_OK;
                break;
        }
        
        if (RetVal == E_OK)
        {
            /* Update runtime data */
            Pwm_ChannelConfig[ChannelId].NotificationEnabled = FALSE;
        }
    }
    
    return RetVal;
}


