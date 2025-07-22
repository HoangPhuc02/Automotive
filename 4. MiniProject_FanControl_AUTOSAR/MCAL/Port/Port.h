/*
 * =====================================================
 *  AUTOSAR Port Driver - Interface Header
 * =====================================================
 *  Module: Port Driver (MCAL)
 *  Project: Temperature-Controlled Fan System
 *  Target: STM32F103C8T6
 *  Author: [Student Name]
 *  Date: [Date]
 * =====================================================
 */

#ifndef PORT_H
#define PORT_H

/*
 * =====================================================
 *  INCLUDES
 * =====================================================
 */
#include "Std_Types.h"

/*
 * =====================================================
 *  MODULE VERSION INFORMATION
 * =====================================================
 */
#define PORT_VENDOR_ID                    1u
#define PORT_MODULE_ID                    124u
#define PORT_AR_RELEASE_MAJOR_VERSION     4u
#define PORT_AR_RELEASE_MINOR_VERSION     2u
#define PORT_AR_RELEASE_REVISION_VERSION  2u
#define PORT_SW_MAJOR_VERSION             1u
#define PORT_SW_MINOR_VERSION             0u
#define PORT_SW_PATCH_VERSION             0u

/*
 * =====================================================
 *  TYPE DEFINITIONS
 * =====================================================
 */

/* Port pin ID type */
typedef uint8 Port_PinType;

/* Port pin direction type */
typedef enum
{
    PORT_PIN_IN  = 0,    /* Pin configured as input */
    PORT_PIN_OUT = 1     /* Pin configured as output */
} Port_PinDirectionType;

/* Port pin mode type */
typedef uint8 Port_PinModeType;

/* Port pin level value */
typedef enum
{
    PORT_PIN_LEVEL_LOW  = 0,    /* Pin level low */
    PORT_PIN_LEVEL_HIGH = 1     /* Pin level high */
} Port_PinLevelValue;

/* Port pin direction changeable */
typedef enum
{
    PORT_PIN_DIRECTION_UNCHANGEABLE = 0,    /* Direction not changeable */
    PORT_PIN_DIRECTION_CHANGEABLE   = 1     /* Direction changeable */
} Port_PinDirectionChangeable;

/* Port pin mode changeable */
typedef enum
{
    PORT_PIN_MODE_UNCHANGEABLE = 0,    /* Mode not changeable */
    PORT_PIN_MODE_CHANGEABLE   = 1     /* Mode changeable */
} Port_PinModeChangeable;

/*
 * =====================================================
 *  PORT PIN MODES (STM32F103 specific)
 * =====================================================
 */
#define PORT_PIN_MODE_DIO           0u      /* Digital I/O */
#define PORT_PIN_MODE_ADC           1u      /* Analog input (ADC) */
#define PORT_PIN_MODE_PWM           2u      /* PWM output (Timer) */
#define PORT_PIN_MODE_UART_TX       3u      /* UART transmit */
#define PORT_PIN_MODE_UART_RX       4u      /* UART receive */
#define PORT_PIN_MODE_SPI_SCK       5u      /* SPI clock */
#define PORT_PIN_MODE_SPI_MISO      6u      /* SPI master in slave out */
#define PORT_PIN_MODE_SPI_MOSI      7u      /* SPI master out slave in */
#define PORT_PIN_MODE_I2C_SCL       8u      /* I2C clock */
#define PORT_PIN_MODE_I2C_SDA       9u      /* I2C data */

/*
 * =====================================================
 *  PORT PIN IDS (for this project)
 * =====================================================
 */
#define PortConf_PortPin_TEMP_SENSOR    0u      /* PA0 - Temperature sensor ADC input */
#define PortConf_PortPin_FAN_PWM        8u      /* PA8 - Fan PWM output */
#define PortConf_PortPin_LED_STATUS     45u     /* PC13 - Status LED (PC13 = 32+13) */

/*
 * =====================================================
 *  PORT CONFIGURATION STRUCTURE
 * =====================================================
 */

/* Configuration structure for a single port pin */
typedef struct
{
    Port_PinType                  PinId;              /* Pin identifier */
    Port_PinDirectionType         PinDirection;       /* Pin direction */
    Port_PinModeType              PinMode;            /* Pin mode */
    Port_PinLevelValue            PinLevelValue;      /* Initial pin level */
    Port_PinDirectionChangeable   DirectionChangeable; /* Direction changeable */
    Port_PinModeChangeable        ModeChangeable;     /* Mode changeable */
} Port_ConfigPin;

/* Main configuration structure for all port pins */
typedef struct
{
    const Port_ConfigPin*   Pins;           /* Pointer to pin configurations */
    uint8                   NumPins;        /* Number of configured pins */
} Port_ConfigType;

/*
 * =====================================================
 *  FUNCTION PROTOTYPES
 * =====================================================
 */

/*
 * Service Name: Port_Init
 * Service ID: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): ConfigPtr - Pointer to configuration set
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Initializes the Port Driver module
 */
void Port_Init(const Port_ConfigType* ConfigPtr);

/*
 * Service Name: Port_SetPinDirection
 * Service ID: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): Pin - Port Pin ID number
 *                  Direction - Port Pin Direction
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Sets the port pin direction
 */
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction);

/*
 * Service Name: Port_RefreshPortDirection
 * Service ID: 0x02
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Refreshes port direction
 */
void Port_RefreshPortDirection(void);

/*
 * Service Name: Port_GetVersionInfo
 * Service ID: 0x03
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): versioninfo - Pointer to where to store version info
 * Return value: None
 * Description: Returns the version information of this module
 */
void Port_GetVersionInfo(Std_VersionInfoType* versioninfo);

/*
 * Service Name: Port_SetPinMode
 * Service ID: 0x04
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): Pin - Port Pin ID number
 *                  Mode - New Port Pin mode to be set on port pin
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Sets the port pin mode
 */
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode);

/*
 * =====================================================
 *  EXTERNAL CONFIGURATION REFERENCE
 * =====================================================
 */

/* External reference to port configuration */
extern const Port_ConfigType Port_Config;

/*
 * =====================================================
 *  ERROR CODES (for DET reporting)
 * =====================================================
 */
#define PORT_E_PARAM_PIN                0x0A    /* Invalid Pin ID */
#define PORT_E_DIRECTION_UNCHANGEABLE   0x0B    /* Direction unchangeable */
#define PORT_E_PARAM_CONFIG             0x0C    /* Invalid configuration */
#define PORT_E_PARAM_INVALID_MODE       0x0D    /* Invalid pin mode */
#define PORT_E_MODE_UNCHANGEABLE        0x0E    /* Mode unchangeable */
#define PORT_E_UNINIT                   0x0F    /* Port not initialized */
#define PORT_E_PARAM_POINTER            0x10    /* Null pointer */

#endif /* PORT_H */

/*
 * =====================================================
 *  IMPLEMENTATION GUIDELINES FOR STUDENTS
 * =====================================================
 * 
 * 1. Port Driver Responsibilities:
 *    - Configure GPIO pins according to their intended function
 *    - Set up alternate functions for peripherals (ADC, PWM, etc.)
 *    - Configure pull-up/pull-down resistors as needed
 *    - Set initial pin levels for outputs
 * 
 * 2. STM32F103 GPIO Configuration:
 *    - Each port has CRL (pins 0-7) and CRH (pins 8-15) registers
 *    - Each pin uses 4 bits: CNF[1:0] and MODE[1:0]
 *    - MODE: 00=Input, 01=Output 10MHz, 10=Output 2MHz, 11=Output 50MHz
 *    - CNF for input: 00=Analog, 01=Floating, 10=Pull-up/down, 11=Reserved
 *    - CNF for output: 00=Push-pull, 01=Open-drain, 10=Alt Push-pull, 11=Alt Open-drain
 * 
 * 3. Pin Configuration for This Project:
 *    - PA0 (ADC): Analog input mode (MODE=00, CNF=00)
 *    - PA8 (PWM): Alternate function push-pull (MODE=11, CNF=10)
 *    - PC13 (LED): General purpose output push-pull (MODE=10, CNF=00)
 * 
 * 4. Clock Configuration:
 *    - Enable appropriate peripheral clocks (GPIOA, GPIOC)
 *    - Enable alternate function clock if using AFIO
 * 
 * 5. Error Handling:
 *    - Validate pin numbers and modes
 *    - Check for null pointers
 *    - Report errors using DET if available
 * 
 * =====================================================
 */
