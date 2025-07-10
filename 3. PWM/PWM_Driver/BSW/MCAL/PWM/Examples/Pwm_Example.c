/****************************************************************************************
*                                PWM_EXAMPLE.C                                         *
****************************************************************************************
* File Name   : Pwm_Example.c
* Module      : Pulse Width Modulation (PWM)
* Description : Example usage of AUTOSAR PWM driver
* Version     : 1.0.0
* Date        : 2025
* Author      : PWM Driver Team
* Repository  : PWM_Driver
****************************************************************************************/

/****************************************************************************************
*                                 INCLUDE FILES                                        *
****************************************************************************************/
#include "Pwm.h"
#include "Pwm_Cfg.h"

/****************************************************************************************
*                              NOTIFICATION FUNCTIONS                                 *
****************************************************************************************/

/**
 * @brief Example PWM notification function for Channel 0
 * @details Called when PWM edge occurs on Channel 0
 */
void Pwm_NotificationChannel0(void)
{
    /* Add your notification handling code here */
    /* Example: Toggle LED, increment counter, etc. */
}

/**
 * @brief Example PWM notification function for Channel 1
 * @details Called when PWM edge occurs on Channel 1
 */
void Pwm_NotificationChannel1(void)
{
    /* Add your notification handling code here */
    /* Example: Toggle LED, increment counter, etc. */
}

/****************************************************************************************
*                              EXAMPLE FUNCTIONS                                      *
****************************************************************************************/

/**
 * @brief Example function demonstrating PWM driver initialization
 * @details Shows how to initialize the PWM driver with configuration
 */
void Pwm_Example_Init(void)
{
    /* Initialize PWM driver with default configuration */
    Pwm_Init(&Pwm_Config);
}

/**
 * @brief Example function demonstrating basic PWM operations
 * @details Shows how to control PWM channels
 */
void Pwm_Example_BasicOperations(void)
{
    /* Set Channel 0 to 25% duty cycle */
    Pwm_SetDutyCycle(PWM_CHANNEL_0, 0x2000);  /* 25% = 0x8000 / 4 */
    
    /* Set Channel 1 to 50% duty cycle */
    Pwm_SetDutyCycle(PWM_CHANNEL_1, 0x4000);  /* 50% = 0x8000 / 2 */
    
    /* Set Channel 2 to 75% duty cycle */
    Pwm_SetDutyCycle(PWM_CHANNEL_2, 0x6000);  /* 75% = 0x8000 * 3 / 4 */
    
    /* Set Channel 3 to 100% duty cycle */
    Pwm_SetDutyCycle(PWM_CHANNEL_3, 0x8000);  /* 100% = 0x8000 */
}

/**
 * @brief Example function demonstrating period and duty cycle setting
 * @details Shows how to set both period and duty cycle for variable period channels
 */
void Pwm_Example_SetPeriodAndDuty(void)
{
    /* Set Channel 0 period to 2000 ticks and duty cycle to 50% */
    /* Note: Only works with PWM_VARIABLE_PERIOD channels */
    Pwm_SetPeriodAndDuty(PWM_CHANNEL_0, 2000, 0x4000);
    
    /* Set Channel 1 period to 1500 ticks and duty cycle to 25% */
    Pwm_SetPeriodAndDuty(PWM_CHANNEL_1, 1500, 0x2000);
}

/**
 * @brief Example function demonstrating output state control
 * @details Shows how to control PWM output states
 */
void Pwm_Example_OutputControl(void)
{
    Pwm_OutputStateType state;
    
    /* Get current output state of Channel 0 */
    state = Pwm_GetOutputState(PWM_CHANNEL_0);
    
    if (state == PWM_HIGH)
    {
        /* Output is currently high */
        /* Add your logic here */
    }
    else
    {
        /* Output is currently low */
        /* Add your logic here */
    }
    
    /* Set Channel 2 to idle state */
    Pwm_SetOutputToIdle(PWM_CHANNEL_2);
}

/**
 * @brief Example function demonstrating notification control
 * @details Shows how to enable/disable PWM notifications
 */
void Pwm_Example_NotificationControl(void)
{
    /* Enable rising edge notification for Channel 0 */
    Pwm_EnableNotification(PWM_CHANNEL_0, PWM_RISING_EDGE);
    
    /* Enable falling edge notification for Channel 1 */
    Pwm_EnableNotification(PWM_CHANNEL_1, PWM_FALLING_EDGE);
    
    /* Enable both edge notification for Channel 2 */
    Pwm_EnableNotification(PWM_CHANNEL_2, PWM_BOTH_EDGES);
    
    /* Disable notification for Channel 3 */
    Pwm_DisableNotification(PWM_CHANNEL_3);
}

/**
 * @brief Example function demonstrating version information
 * @details Shows how to get PWM driver version information
 */
void Pwm_Example_VersionInfo(void)
{
    Std_VersionInfoType versionInfo;
    
    /* Get version information */
    Pwm_GetVersionInfo(&versionInfo);
    
    /* Use version information */
    /* Example: Display version, check compatibility, etc. */
}

/**
 * @brief Example function demonstrating driver deinitialization
 * @details Shows how to properly deinitialize the PWM driver
 */
void Pwm_Example_DeInit(void)
{
    /* Deinitialize PWM driver */
    Pwm_DeInit();
}

/**
 * @brief Main example function
 * @details Demonstrates complete PWM driver usage sequence
 */
void Pwm_Example_Main(void)
{
    /* Initialize PWM driver */
    Pwm_Example_Init();
    
    /* Perform basic operations */
    Pwm_Example_BasicOperations();
    
    /* Set period and duty cycle */
    Pwm_Example_SetPeriodAndDuty();
    
    /* Control output states */
    Pwm_Example_OutputControl();
    
    /* Control notifications */
    Pwm_Example_NotificationControl();
    
    /* Get version information */
    Pwm_Example_VersionInfo();
    
    /* Main application loop would go here */
    
    /* Deinitialize when done */
    Pwm_Example_DeInit();
}

/****************************************************************************************
*                              ADVANCED EXAMPLES                                      *
****************************************************************************************/

/**
 * @brief Example function demonstrating servo motor control
 * @details Shows how to control servo motors with PWM
 */
void Pwm_Example_ServoControl(void)
{
    /* Servo control typically uses 50Hz (20ms period) */
    /* 1ms pulse = 0° position, 1.5ms pulse = 90° position, 2ms pulse = 180° position */
    
    /* Set servo to 0° position (1ms pulse width) */
    Pwm_SetPeriodAndDuty(PWM_CHANNEL_0, 20000, 0x0CCD);  /* 1ms out of 20ms */
    
    /* Set servo to 90° position (1.5ms pulse width) */
    Pwm_SetPeriodAndDuty(PWM_CHANNEL_1, 20000, 0x1333);  /* 1.5ms out of 20ms */
    
    /* Set servo to 180° position (2ms pulse width) */
    Pwm_SetPeriodAndDuty(PWM_CHANNEL_2, 20000, 0x1999);  /* 2ms out of 20ms */
}

/**
 * @brief Example function demonstrating LED brightness control
 * @details Shows how to control LED brightness with PWM
 */
void Pwm_Example_LedBrightness(void)
{
    uint16 brightness;
    
    /* Gradual brightness increase */
    for (brightness = 0; brightness <= 0x8000; brightness += 0x0800)
    {
        Pwm_SetDutyCycle(PWM_CHANNEL_0, brightness);
        /* Add delay here for visible effect */
    }
    
    /* Gradual brightness decrease */
    for (brightness = 0x8000; brightness > 0; brightness -= 0x0800)
    {
        Pwm_SetDutyCycle(PWM_CHANNEL_0, brightness);
        /* Add delay here for visible effect */
    }
}

/**
 * @brief Example function demonstrating motor speed control
 * @details Shows how to control DC motor speed with PWM
 */
void Pwm_Example_MotorSpeedControl(void)
{
    /* Set motor to different speeds */
    
    /* Stop motor */
    Pwm_SetDutyCycle(PWM_CHANNEL_0, 0x0000);    /* 0% duty cycle */
    
    /* Low speed */
    Pwm_SetDutyCycle(PWM_CHANNEL_0, 0x2000);    /* 25% duty cycle */
    
    /* Medium speed */
    Pwm_SetDutyCycle(PWM_CHANNEL_0, 0x4000);    /* 50% duty cycle */
    
    /* High speed */
    Pwm_SetDutyCycle(PWM_CHANNEL_0, 0x6000);    /* 75% duty cycle */
    
    /* Maximum speed */
    Pwm_SetDutyCycle(PWM_CHANNEL_0, 0x8000);    /* 100% duty cycle */
}

/****************************************************************************************
*                                 END OF FILE                                          *
****************************************************************************************/
