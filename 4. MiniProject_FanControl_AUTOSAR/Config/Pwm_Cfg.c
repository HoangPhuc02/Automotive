/*
 * =====================================================
 *  AUTOSAR PWM Driver Configuration
 * =====================================================
 */

#include "Pwm.h"

/* PWM Configuration for Fan Control */
const Pwm_ConfigType Pwm_Config = 
{
    .ChannelId = 0,         /* Timer1 Channel 1 (PA8) */
    .Period = 100,          /* 10kHz PWM frequency (1MHz/100 = 10kHz) */
    .DutyCycle = 0          /* Initial duty cycle 0% */
};
