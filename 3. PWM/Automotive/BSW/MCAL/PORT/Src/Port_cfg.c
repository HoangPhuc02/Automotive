/**********************************************************
* File Name   : Port_cfg.c
* Module      : Port Driver 
* Description : Port Driver Configuration Source File
* Details     : File configures the GPIO pins for the Port Driver
*               according to AUTOSAR standards, designed for STM32F103
*               using the Standard Peripheral Library (SPL).
* Version     : 1.0.0
* Date        : 27/06/2025
* Author      : hoangphuc540202@gmail.com
* Github      : https://github.com/HoangPhuc02
 **********************************************************/

#include "Port_cfg.h"

/**
 * @brief Configuration for the Port Driver
 *        - PinCount: Total number of pins configured
 *        - PinConfigs: Pointer to an array of pin configurations
 */
const Port_ConfigType PortCfg_Port = {
    .PinCount = PortCfg_PinsCount,
    .PinConfigs = PortCfg_Pins
};
/**
 * @brief Configuration for each pin
 *        - PortNum:   PORT_ID_A, PORT_ID_B, PORT_ID_C, ...
 *        - PinNum:    Pin number within the port (0-15)
 *        - Mode:      PORT_PIN_MODE_DIO, ...
 *        - Direction: PORT_PIN_OUT / PORT_PIN_IN
 *        - DirectionChangeable: 1 = Allow direction change at runtime
 *        - Level:     PORT_PIN_LEVEL_HIGH / PORT_PIN_LEVEL_LOW
 *        - Pull:      PORT_PIN_PULL_NONE / UP / DOWN
 *        - ModeChangeable: 1 = Allow mode change at runtime
 * @note When add more pins, ensure to update PortCfg_PinsCount
 */
const Port_PinConfigType PortCfg_Pins[PortCfg_PinsCount] = {
    /* PA0: DIO, Output, Enable Change Direction & Mode runtime, HIGH, no pull */
    {
        .PortNum = PORT_ID_A,
        .PinNum = 0,
        .Mode = PORT_PIN_MODE_ADC,
        .Direction = PORT_PIN_IN,
        .DirectionChangeable = 1,
        .Level = PORT_PIN_LEVEL_HIGH,
        .Pull = PORT_PIN_PULL_NONE,
        .ModeChangeable = 1,
        .Speed = PORT_PIN_SPEED_10MHZ,
    },
    /* PA1: DIO, Input, Disable Change Direction & Mode runtime, pull-up */
    {
        .PortNum = PORT_ID_A,
        .PinNum = 1,
        .Mode = PORT_PIN_MODE_ADC,
        .Direction = PORT_PIN_IN,
        .DirectionChangeable = 1,
        .Level = PORT_PIN_LEVEL_HIGH,
        .Pull = PORT_PIN_PULL_NONE,
        .ModeChangeable = 1,
        .Speed = PORT_PIN_SPEED_10MHZ,
    },
    {
        .PortNum = PORT_ID_A,
        .PinNum = 2,
        .Mode = PORT_PIN_MODE_ADC,
        .Direction = PORT_PIN_IN,
        .DirectionChangeable = 1,
        .Level = PORT_PIN_LEVEL_HIGH,
        .Pull = PORT_PIN_PULL_NONE,
        .ModeChangeable = 1,
        .Speed = PORT_PIN_SPEED_10MHZ,
    },
    {
        .PortNum = PORT_ID_A,
        .PinNum = 3,
        .Mode = PORT_PIN_MODE_ADC,
        .Direction = PORT_PIN_IN,
        .DirectionChangeable = 1,
        .Level = PORT_PIN_LEVEL_HIGH,
        .Pull = PORT_PIN_PULL_NONE,
        .ModeChangeable = 1,
        .Speed = PORT_PIN_SPEED_10MHZ,
    },
    /* PA1: DIO, Input, Disable Change Direction & Mode runtime, pull-up */
    {
        .PortNum = PORT_ID_A,
        .PinNum = 4,
        .Mode = PORT_PIN_MODE_ADC,
        .Direction = PORT_PIN_IN,
        .DirectionChangeable = 1,
        .Level = PORT_PIN_LEVEL_HIGH,
        .Pull = PORT_PIN_PULL_NONE,
        .ModeChangeable = 1,
        .Speed = PORT_PIN_SPEED_10MHZ,
    },
        {
        .PortNum = PORT_ID_A,
        .PinNum = 5,
        .Mode = PORT_PIN_MODE_ADC,
        .Direction = PORT_PIN_IN,
        .DirectionChangeable = 1,
        .Level = PORT_PIN_LEVEL_HIGH,
        .Pull = PORT_PIN_PULL_NONE,
        .ModeChangeable = 1,
        .Speed = PORT_PIN_SPEED_10MHZ,
    },
        /* PA1: DIO, Input, Disable Change Direction & Mode runtime, pull-up */
    {
        .PortNum = PORT_ID_A,
        .PinNum = 6,
        .Mode = PORT_PIN_MODE_ADC,
        .Direction = PORT_PIN_IN,
        .DirectionChangeable = 1,
        .Level = PORT_PIN_LEVEL_HIGH,
        .Pull = PORT_PIN_PULL_NONE,
        .ModeChangeable = 1,
        .Speed = PORT_PIN_SPEED_10MHZ,
    },
        {
        .PortNum = PORT_ID_A,
        .PinNum = 7,
        .Mode = PORT_PIN_MODE_ADC,
        .Direction = PORT_PIN_IN,
        .DirectionChangeable = 1,
        .Level = PORT_PIN_LEVEL_HIGH,
        .Pull = PORT_PIN_PULL_NONE,
        .ModeChangeable = 1,
        .Speed = PORT_PIN_SPEED_10MHZ,
    },
    /* PB0: DIO, Output, Disable Change Direction & Mode runtime, LOW, no pull */
    {
        .PortNum = PORT_ID_B,
        .PinNum = 0,
        .Mode = PORT_PIN_MODE_ADC,
        .Direction = PORT_PIN_OUT,
        .DirectionChangeable = 0,
        .Level = PORT_PIN_LEVEL_LOW,
        .Pull = PORT_PIN_PULL_NONE,
        .ModeChangeable = 0,
        .Speed = PORT_PIN_SPEED_10MHZ
    },
    /* PC13: DIO, Output, Enable Change Mode runtime, no pull */
    {
        .PortNum = PORT_ID_B,
        .PinNum = 1,
        .Mode = PORT_PIN_MODE_ADC,
        .Direction = PORT_PIN_OUT,
        .DirectionChangeable = 1,
        .Level = PORT_PIN_LEVEL_LOW,
        .Pull = PORT_PIN_PULL_DOWN,
        .ModeChangeable = 1,
        .Speed = PORT_PIN_SPEED_10MHZ
    }
   
};

