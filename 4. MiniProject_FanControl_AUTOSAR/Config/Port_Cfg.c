/*
 * =====================================================
 *  AUTOSAR Port Driver Configuration
 * =====================================================
 */

#include "Port.h"

/* Pin Configuration Array */
static const Port_ConfigPin Port_Pins[] = 
{
    /* Temperature Sensor Pin - PA0 (ADC Input) */
    {
        .PinId = PortConf_PortPin_TEMP_SENSOR,    /* PA0 */
        .PinDirection = PORT_PIN_IN,
        .PinMode = PORT_PIN_MODE_ADC,
        .PinLevelValue = PORT_PIN_LEVEL_LOW,
        .DirectionChangeable = PORT_PIN_DIRECTION_UNCHANGEABLE,
        .ModeChangeable = PORT_PIN_MODE_UNCHANGEABLE
    },
    
    /* Fan PWM Pin - PA8 (PWM Output) */
    {
        .PinId = PortConf_PortPin_FAN_PWM,        /* PA8 */
        .PinDirection = PORT_PIN_OUT,
        .PinMode = PORT_PIN_MODE_PWM,
        .PinLevelValue = PORT_PIN_LEVEL_LOW,
        .DirectionChangeable = PORT_PIN_DIRECTION_UNCHANGEABLE,
        .ModeChangeable = PORT_PIN_MODE_UNCHANGEABLE
    },
    
    /* Status LED Pin - PC13 (GPIO Output) */
    {
        .PinId = PortConf_PortPin_LED_STATUS,     /* PC13 */
        .PinDirection = PORT_PIN_OUT,
        .PinMode = PORT_PIN_MODE_DIO,
        .PinLevelValue = PORT_PIN_LEVEL_LOW,      /* LED OFF initially */
        .DirectionChangeable = PORT_PIN_DIRECTION_UNCHANGEABLE,
        .ModeChangeable = PORT_PIN_MODE_UNCHANGEABLE
    }
};

/* Main Port Configuration */
const Port_ConfigType Port_Config = 
{
    .Pins = Port_Pins,
    .NumPins = sizeof(Port_Pins) / sizeof(Port_ConfigPin)
};
