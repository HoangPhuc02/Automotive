/****************************************************************************************
*                                 PWM_HW.C                                             *
****************************************************************************************
* File Name   : Pwm_Hw.c
* Module      : Pulse Width Modulation (PWM)
* Description : AUTOSAR PWM driver hardware abstraction layer implementation
* Version     : 1.0.0 - AUTOSAR compliant implementation with STM32F10x SPL
* Date        : 2025
* Author      : PWM Driver Team
* Repository  : PWM_Driver
****************************************************************************************/

/****************************************************************************************
*                                 INCLUDE FILES                                        *
****************************************************************************************/
#include "Pwm_Hw.h"
#include "Pwm_Cfg.h"
#include "Det.h"

/****************************************************************************************
*                              VERSION INFORMATION                                     *
****************************************************************************************/
#define PWM_HW_SW_MAJOR_VERSION_C    1
#define PWM_HW_SW_MINOR_VERSION_C    0
#define PWM_HW_SW_PATCH_VERSION_C    0

/* Version compatibility checks */
#if (PWM_HW_SW_MAJOR_VERSION != PWM_HW_SW_MAJOR_VERSION_C)
    #error "Pwm_Hw.c: Version mismatch in Major version"
#endif

#if (PWM_HW_SW_MINOR_VERSION != PWM_HW_SW_MINOR_VERSION_C)
    #error "Pwm_Hw.c: Version mismatch in Minor version"
#endif

#if (PWM_HW_SW_PATCH_VERSION != PWM_HW_SW_PATCH_VERSION_C)
    #error "Pwm_Hw.c: Version mismatch in Patch version"
#endif

/****************************************************************************************
*                              LOCAL VARIABLES                                         *
****************************************************************************************/

/**
 * @brief Hardware unit runtime data
 * @details Stores runtime information for each hardware unit
 */
static Pwm_HwUnitRuntimeType Pwm_HwUnitRuntime[PWM_MAX_HW_UNITS];

/**
 * @brief Channel runtime data
 * @details Stores runtime information for each PWM channel
 */
static Pwm_ChannelRuntimeType Pwm_ChannelRuntime[PWM_MAX_CHANNELS];

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
Std_ReturnType Pwm_Hw_InitHwUnit(Pwm_HwUnitType HwUnit, const Pwm_HwUnitConfigType* ConfigPtr)
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
        
        /* Initialize timer base configuration */
        TIM_TimeBaseStructure.TIM_Period = ConfigPtr->MaxPeriod - 1;
        TIM_TimeBaseStructure.TIM_Prescaler = ConfigPtr->Prescaler - 1;
        TIM_TimeBaseStructure.TIM_ClockDivision = ConfigPtr->ClockDivision;
        TIM_TimeBaseStructure.TIM_CounterMode = ConfigPtr->CounterMode;
        TIM_TimeBaseStructure.TIM_RepetitionCounter = ConfigPtr->RepetitionCounter;
        
        /* Configure timer */
        TIM_TimeBaseInit(ConfigPtr->TimerInstance, &TIM_TimeBaseStructure);
        
        /* Initialize runtime data */
        Pwm_HwUnitRuntime[HwUnit].HwUnit = HwUnit;
        Pwm_HwUnitRuntime[HwUnit].TimerInstance = ConfigPtr->TimerInstance;
        Pwm_HwUnitRuntime[HwUnit].CurrentPeriod = ConfigPtr->MaxPeriod;
        Pwm_HwUnitRuntime[HwUnit].IsInitialized = STD_ON;
        Pwm_HwUnitRuntime[HwUnit].IsRunning = STD_OFF;
        Pwm_HwUnitRuntime[HwUnit].ActiveChannels = 0;
        
        /* Enable ARR preload */
        TIM_ARRPreloadConfig(ConfigPtr->TimerInstance, ENABLE);
        
        /* Enable timer */
        TIM_Cmd(ConfigPtr->TimerInstance, ENABLE);
        
        /* Update runtime state */
        Pwm_HwUnitRuntime[HwUnit].IsRunning = STD_ON;
    }
    
    return RetVal;
}

/**
 * @brief Deinitializes PWM hardware unit
 * @details Resets timer hardware and disables clocks
 * @param[in] HwUnit Hardware unit identifier
 * @return E_OK if deinitialization successful, E_NOT_OK otherwise
 */
Std_ReturnType Pwm_Hw_DeInitHwUnit(Pwm_HwUnitType HwUnit)
{
    Std_ReturnType RetVal = E_OK;
    
    /* Validate parameters */
    if (HwUnit >= PWM_MAX_HW_UNITS)
    {
        RetVal = E_NOT_OK;
    }
    else
    {
        /* Disable timer */
        if (Pwm_HwUnitRuntime[HwUnit].TimerInstance != NULL_PTR)
        {
            TIM_Cmd(Pwm_HwUnitRuntime[HwUnit].TimerInstance, DISABLE);
            TIM_DeInit(Pwm_HwUnitRuntime[HwUnit].TimerInstance);
        }
        
        /* Disable timer clock */
        PWM_HW_DISABLE_TIMER_CLOCK(HwUnit);
        
        /* Reset runtime data */
        Pwm_HwUnitRuntime[HwUnit].HwUnit = PWM_HW_UNIT_INVALID;
        Pwm_HwUnitRuntime[HwUnit].TimerInstance = NULL_PTR;
        Pwm_HwUnitRuntime[HwUnit].CurrentPeriod = 0;
        Pwm_HwUnitRuntime[HwUnit].IsInitialized = STD_OFF;
        Pwm_HwUnitRuntime[HwUnit].IsRunning = STD_OFF;
        Pwm_HwUnitRuntime[HwUnit].ActiveChannels = 0;
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
Std_ReturnType Pwm_Hw_InitChannel(Pwm_ChannelType ChannelId, const Pwm_ChannelConfigType* ChannelConfig)
{
    Std_ReturnType RetVal = E_OK;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin;
    uint16_t TIM_Channel;
    
    /* Validate parameters */
    if ((ChannelId >= PWM_MAX_CHANNELS) || (ChannelConfig == NULL_PTR))
    {
        RetVal = E_NOT_OK;
    }
    else
    {
        /* Get GPIO port and pin */
        GPIOx = PWM_HW_GET_GPIO_PORT(ChannelConfig->HwUnit);
        GPIO_Pin = PWM_HW_GET_GPIO_PIN(ChannelConfig->HwUnit, ChannelConfig->HwChannel);
        
        /* Enable GPIO clock */
        if (GPIOx == GPIOA)
        {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        }
        else if (GPIOx == GPIOB)
        {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        }
        else if (GPIOx == GPIOC)
        {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
        }
        
        /* Configure GPIO pin */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOx, &GPIO_InitStructure);
        
        /* Get timer channel */
        TIM_Channel = PWM_HW_GET_TIM_CHANNEL(ChannelConfig->HwChannel);
        
        /* Configure timer output compare */
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
        TIM_OCInitStructure.TIM_Pulse = ChannelConfig->DefaultDutyCycle;
        TIM_OCInitStructure.TIM_OCPolarity = (ChannelConfig->Polarity == PWM_HIGH) ? TIM_OCPolarity_High : TIM_OCPolarity_Low;
        TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
        TIM_OCInitStructure.TIM_OCIdleState = (ChannelConfig->IdleState == PWM_HIGH) ? TIM_OCIdleState_Set : TIM_OCIdleState_Reset;
        TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
        
        /* Initialize output compare based on channel */
        switch (ChannelConfig->HwChannel)
        {
            case 0:
                TIM_OC1Init(Pwm_HwUnitRuntime[ChannelConfig->HwUnit].TimerInstance, &TIM_OCInitStructure);
                TIM_OC1PreloadConfig(Pwm_HwUnitRuntime[ChannelConfig->HwUnit].TimerInstance, TIM_OCPreload_Enable);
                break;
            case 1:
                TIM_OC2Init(Pwm_HwUnitRuntime[ChannelConfig->HwUnit].TimerInstance, &TIM_OCInitStructure);
                TIM_OC2PreloadConfig(Pwm_HwUnitRuntime[ChannelConfig->HwUnit].TimerInstance, TIM_OCPreload_Enable);
                break;
            case 2:
                TIM_OC3Init(Pwm_HwUnitRuntime[ChannelConfig->HwUnit].TimerInstance, &TIM_OCInitStructure);
                TIM_OC3PreloadConfig(Pwm_HwUnitRuntime[ChannelConfig->HwUnit].TimerInstance, TIM_OCPreload_Enable);
                break;
            case 3:
                TIM_OC4Init(Pwm_HwUnitRuntime[ChannelConfig->HwUnit].TimerInstance, &TIM_OCInitStructure);
                TIM_OC4PreloadConfig(Pwm_HwUnitRuntime[ChannelConfig->HwUnit].TimerInstance, TIM_OCPreload_Enable);
                break;
            default:
                RetVal = E_NOT_OK;
                break;
        }
        
        if (RetVal == E_OK)
        {
            /* Initialize channel runtime data */
            Pwm_ChannelRuntime[ChannelId].ChannelId = ChannelId;
            Pwm_ChannelRuntime[ChannelId].HwUnit = ChannelConfig->HwUnit;
            Pwm_ChannelRuntime[ChannelId].HwChannel = ChannelConfig->HwChannel;
            Pwm_ChannelRuntime[ChannelId].CurrentPeriod = ChannelConfig->DefaultPeriod;
            Pwm_ChannelRuntime[ChannelId].CurrentDutyCycle = ChannelConfig->DefaultDutyCycle;
            Pwm_ChannelRuntime[ChannelId].CurrentState = PWM_LOW;
            Pwm_ChannelRuntime[ChannelId].IsInitialized = STD_ON;
            Pwm_ChannelRuntime[ChannelId].IsRunning = STD_OFF;
            Pwm_ChannelRuntime[ChannelId].NotificationEnabled = ChannelConfig->NotificationEnabled;
            
            /* Update hardware unit active channels */
            Pwm_HwUnitRuntime[ChannelConfig->HwUnit].ActiveChannels |= (1U << ChannelConfig->HwChannel);
            
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

/**
 * @brief Sets PWM duty cycle for a channel
 * @details Updates the compare value for the specified channel
 * @param[in] ChannelId Channel identifier
 * @param[in] DutyCycle New duty cycle value (0x0000 to 0x8000)
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType Pwm_Hw_SetDutyCycle(Pwm_ChannelType ChannelId, uint16 DutyCycle)
{
    Std_ReturnType RetVal = E_OK;
    uint16 CompareValue;
    
    /* Validate parameters */
    if ((ChannelId >= PWM_MAX_CHANNELS) || (DutyCycle > 0x8000))
    {
        RetVal = E_NOT_OK;
    }
    else if (Pwm_ChannelRuntime[ChannelId].IsInitialized == STD_OFF)
    {
        RetVal = E_NOT_OK;
    }
    else
    {
        /* Calculate compare value */
        CompareValue = (uint16)((((uint32)DutyCycle) * Pwm_ChannelRuntime[ChannelId].CurrentPeriod) >> 15);
        
        /* Update compare value based on channel */
        switch (Pwm_ChannelRuntime[ChannelId].HwChannel)
        {
            case 0:
                TIM_SetCompare1(Pwm_HwUnitRuntime[Pwm_ChannelRuntime[ChannelId].HwUnit].TimerInstance, CompareValue);
                break;
            case 1:
                TIM_SetCompare2(Pwm_HwUnitRuntime[Pwm_ChannelRuntime[ChannelId].HwUnit].TimerInstance, CompareValue);
                break;
            case 2:
                TIM_SetCompare3(Pwm_HwUnitRuntime[Pwm_ChannelRuntime[ChannelId].HwUnit].TimerInstance, CompareValue);
                break;
            case 3:
                TIM_SetCompare4(Pwm_HwUnitRuntime[Pwm_ChannelRuntime[ChannelId].HwUnit].TimerInstance, CompareValue);
                break;
            default:
                RetVal = E_NOT_OK;
                break;
        }
        
        if (RetVal == E_OK)
        {
            /* Update runtime data */
            Pwm_ChannelRuntime[ChannelId].CurrentDutyCycle = DutyCycle;
            
            /* Update current state based on duty cycle */
            if (DutyCycle == 0)
            {
                Pwm_ChannelRuntime[ChannelId].CurrentState = PWM_LOW;
            }
            else if (DutyCycle == 0x8000)
            {
                Pwm_ChannelRuntime[ChannelId].CurrentState = PWM_HIGH;
            }
            else
            {
                Pwm_ChannelRuntime[ChannelId].CurrentState = PWM_LOW; /* PWM mode */
            }
        }
    }
    
    return RetVal;
}

/**
 * @brief Sets PWM period and duty cycle for a channel
 * @details Updates both period and duty cycle for variable period channels
 * @param[in] ChannelId Channel identifier
 * @param[in] Period New period value
 * @param[in] DutyCycle New duty cycle value (0x0000 to 0x8000)
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType Pwm_Hw_SetPeriodAndDuty(Pwm_ChannelType ChannelId, Pwm_PeriodType Period, uint16 DutyCycle)
{
    Std_ReturnType RetVal = E_OK;
    uint16 CompareValue;
    
    /* Validate parameters */
    if ((ChannelId >= PWM_MAX_CHANNELS) || (Period == 0) || (DutyCycle > 0x8000))
    {
        RetVal = E_NOT_OK;
    }
    else if (Pwm_ChannelRuntime[ChannelId].IsInitialized == STD_OFF)
    {
        RetVal = E_NOT_OK;
    }
    else
    {
        /* Update timer period */
        TIM_SetAutoreload(Pwm_HwUnitRuntime[Pwm_ChannelRuntime[ChannelId].HwUnit].TimerInstance, Period - 1);
        
        /* Calculate compare value */
        CompareValue = (uint16)((((uint32)DutyCycle) * Period) >> 15);
        
        /* Update compare value based on channel */
        switch (Pwm_ChannelRuntime[ChannelId].HwChannel)
        {
            case 0:
                TIM_SetCompare1(Pwm_HwUnitRuntime[Pwm_ChannelRuntime[ChannelId].HwUnit].TimerInstance, CompareValue);
                break;
            case 1:
                TIM_SetCompare2(Pwm_HwUnitRuntime[Pwm_ChannelRuntime[ChannelId].HwUnit].TimerInstance, CompareValue);
                break;
            case 2:
                TIM_SetCompare3(Pwm_HwUnitRuntime[Pwm_ChannelRuntime[ChannelId].HwUnit].TimerInstance, CompareValue);
                break;
            case 3:
                TIM_SetCompare4(Pwm_HwUnitRuntime[Pwm_ChannelRuntime[ChannelId].HwUnit].TimerInstance, CompareValue);
                break;
            default:
                RetVal = E_NOT_OK;
                break;
        }
        
        if (RetVal == E_OK)
        {
            /* Update runtime data */
            Pwm_ChannelRuntime[ChannelId].CurrentPeriod = Period;
            Pwm_ChannelRuntime[ChannelId].CurrentDutyCycle = DutyCycle;
            Pwm_HwUnitRuntime[Pwm_ChannelRuntime[ChannelId].HwUnit].CurrentPeriod = Period;
            
            /* Update current state based on duty cycle */
            if (DutyCycle == 0)
            {
                Pwm_ChannelRuntime[ChannelId].CurrentState = PWM_LOW;
            }
            else if (DutyCycle == 0x8000)
            {
                Pwm_ChannelRuntime[ChannelId].CurrentState = PWM_HIGH;
            }
            else
            {
                Pwm_ChannelRuntime[ChannelId].CurrentState = PWM_LOW; /* PWM mode */
            }
        }
    }
    
    return RetVal;
}

/****************************************************************************************
*                              OUTPUT CONTROL FUNCTIONS                               *
****************************************************************************************/

/**
 * @brief Sets PWM output to idle state
 * @details Forces the PWM output to the configured idle state
 * @param[in] ChannelId Channel identifier
 * @return E_OK if successful, E_NOT_OK otherwise
 */
Std_ReturnType Pwm_Hw_SetOutputToIdle(Pwm_ChannelType ChannelId)
{
    Std_ReturnType RetVal = E_OK;
    
    /* Validate parameters */
    if (ChannelId >= PWM_MAX_CHANNELS)
    {
        RetVal = E_NOT_OK;
    }
    else if (Pwm_ChannelRuntime[ChannelId].IsInitialized == STD_OFF)
    {
        RetVal = E_NOT_OK;
    }
    else
    {
        /* Disable output compare based on channel */
        switch (Pwm_ChannelRuntime[ChannelId].HwChannel)
        {
            case 0:
                TIM_CCxCmd(Pwm_HwUnitRuntime[Pwm_ChannelRuntime[ChannelId].HwUnit].TimerInstance, TIM_Channel_1, TIM_CCx_Disable);
                break;
            case 1:
                TIM_CCxCmd(Pwm_HwUnitRuntime[Pwm_ChannelRuntime[ChannelId].HwUnit].TimerInstance, TIM_Channel_2, TIM_CCx_Disable);
                break;
            case 2:
                TIM_CCxCmd(Pwm_HwUnitRuntime[Pwm_ChannelRuntime[ChannelId].HwUnit].TimerInstance, TIM_Channel_3, TIM_CCx_Disable);
                break;
            case 3:
                TIM_CCxCmd(Pwm_HwUnitRuntime[Pwm_ChannelRuntime[ChannelId].HwUnit].TimerInstance, TIM_Channel_4, TIM_CCx_Disable);
                break;
            default:
                RetVal = E_NOT_OK;
                break;
        }
        
        if (RetVal == E_OK)
        {
            /* Update runtime state */
            Pwm_ChannelRuntime[ChannelId].IsRunning = STD_OFF;
        }
    }
    
    return RetVal;
}

/**
 * @brief Gets PWM output state
 * @details Returns the current logical state of the PWM output
 * @param[in] ChannelId Channel identifier
 * @return PWM_HIGH or PWM_LOW
 */
Pwm_OutputStateType Pwm_Hw_GetOutputState(Pwm_ChannelType ChannelId)
{
    Pwm_OutputStateType OutputState = PWM_LOW;
    
    /* Validate parameters */
    if ((ChannelId < PWM_MAX_CHANNELS) && (Pwm_ChannelRuntime[ChannelId].IsInitialized == STD_ON))
    {
        OutputState = Pwm_ChannelRuntime[ChannelId].CurrentState;
    }
    
    return OutputState;
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
Std_ReturnType Pwm_Hw_EnableNotification(Pwm_ChannelType ChannelId, Pwm_EdgeNotificationType Notification)
{
    Std_ReturnType RetVal = E_OK;
    
    /* Validate parameters */
    if (ChannelId >= PWM_MAX_CHANNELS)
    {
        RetVal = E_NOT_OK;
    }
    else if (Pwm_ChannelRuntime[ChannelId].IsInitialized == STD_OFF)
    {
        RetVal = E_NOT_OK;
    }
    else
    {
        /* Enable interrupt based on channel */
        switch (Pwm_ChannelRuntime[ChannelId].HwChannel)
        {
            case 0:
                TIM_ITConfig(Pwm_HwUnitRuntime[Pwm_ChannelRuntime[ChannelId].HwUnit].TimerInstance, TIM_IT_CC1, ENABLE);
                break;
            case 1:
                TIM_ITConfig(Pwm_HwUnitRuntime[Pwm_ChannelRuntime[ChannelId].HwUnit].TimerInstance, TIM_IT_CC2, ENABLE);
                break;
            case 2:
                TIM_ITConfig(Pwm_HwUnitRuntime[Pwm_ChannelRuntime[ChannelId].HwUnit].TimerInstance, TIM_IT_CC3, ENABLE);
                break;
            case 3:
                TIM_ITConfig(Pwm_HwUnitRuntime[Pwm_ChannelRuntime[ChannelId].HwUnit].TimerInstance, TIM_IT_CC4, ENABLE);
                break;
            default:
                RetVal = E_NOT_OK;
                break;
        }
        
        if (RetVal == E_OK)
        {
            /* Update runtime data */
            Pwm_ChannelRuntime[ChannelId].NotificationEnabled = STD_ON;
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
Std_ReturnType Pwm_Hw_DisableNotification(Pwm_ChannelType ChannelId)
{
    Std_ReturnType RetVal = E_OK;
    
    /* Validate parameters */
    if (ChannelId >= PWM_MAX_CHANNELS)
    {
        RetVal = E_NOT_OK;
    }
    else if (Pwm_ChannelRuntime[ChannelId].IsInitialized == STD_OFF)
    {
        RetVal = E_NOT_OK;
    }
    else
    {
        /* Disable interrupt based on channel */
        switch (Pwm_ChannelRuntime[ChannelId].HwChannel)
        {
            case 0:
                TIM_ITConfig(Pwm_HwUnitRuntime[Pwm_ChannelRuntime[ChannelId].HwUnit].TimerInstance, TIM_IT_CC1, DISABLE);
                break;
            case 1:
                TIM_ITConfig(Pwm_HwUnitRuntime[Pwm_ChannelRuntime[ChannelId].HwUnit].TimerInstance, TIM_IT_CC2, DISABLE);
                break;
            case 2:
                TIM_ITConfig(Pwm_HwUnitRuntime[Pwm_ChannelRuntime[ChannelId].HwUnit].TimerInstance, TIM_IT_CC3, DISABLE);
                break;
            case 3:
                TIM_ITConfig(Pwm_HwUnitRuntime[Pwm_ChannelRuntime[ChannelId].HwUnit].TimerInstance, TIM_IT_CC4, DISABLE);
                break;
            default:
                RetVal = E_NOT_OK;
                break;
        }
        
        if (RetVal == E_OK)
        {
            /* Update runtime data */
            Pwm_ChannelRuntime[ChannelId].NotificationEnabled = STD_OFF;
        }
    }
    
    return RetVal;
}

/****************************************************************************************
*                              RUNTIME DATA ACCESS FUNCTIONS                          *
****************************************************************************************/

/**
 * @brief Gets channel runtime data
 * @param[in] ChannelId Channel identifier
 * @return Pointer to channel runtime data or NULL_PTR if not found
 */
const Pwm_ChannelRuntimeType* Pwm_Hw_GetChannelRuntime(Pwm_ChannelType ChannelId)
{
    const Pwm_ChannelRuntimeType* RuntimePtr = NULL_PTR;
    
    if ((ChannelId < PWM_MAX_CHANNELS) && (Pwm_ChannelRuntime[ChannelId].IsInitialized == STD_ON))
    {
        RuntimePtr = &Pwm_ChannelRuntime[ChannelId];
    }
    
    return RuntimePtr;
}

/**
 * @brief Gets hardware unit runtime data
 * @param[in] HwUnit Hardware unit identifier
 * @return Pointer to hardware unit runtime data or NULL_PTR if not found
 */
const Pwm_HwUnitRuntimeType* Pwm_Hw_GetHwUnitRuntime(Pwm_HwUnitType HwUnit)
{
    const Pwm_HwUnitRuntimeType* RuntimePtr = NULL_PTR;
    
    if ((HwUnit < PWM_MAX_HW_UNITS) && (Pwm_HwUnitRuntime[HwUnit].IsInitialized == STD_ON))
    {
        RuntimePtr = &Pwm_HwUnitRuntime[HwUnit];
    }
    
    return RuntimePtr;
}
