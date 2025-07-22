/*
 * =====================================================
 *  AUTOSAR PWM Driver - Interface Header
 * =====================================================
 */

#ifndef PWM_H
#define PWM_H

#include "Std_Types.h"

/* PWM Channel Type */
typedef uint8 Pwm_ChannelType;

/* PWM Period Type */
typedef uint16 Pwm_PeriodType;

/* PWM Configuration Type */
typedef struct
{
    Pwm_ChannelType ChannelId;
    Pwm_PeriodType Period;
    uint16 DutyCycle;
    /* Add more configuration as needed */
} Pwm_ConfigType;

/* Function Prototypes */
void Pwm_Init(const Pwm_ConfigType* ConfigPtr);
void Pwm_SetDutyCycle(Pwm_ChannelType ChannelNumber, uint16 DutyCycle);
void Pwm_SetPeriodAndDuty(Pwm_ChannelType ChannelNumber, Pwm_PeriodType Period, uint16 DutyCycle);

/* External Configuration Reference */
extern const Pwm_ConfigType Pwm_Config;

#endif /* PWM_H */
