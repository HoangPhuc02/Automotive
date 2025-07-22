/*
 * =====================================================
 *  AUTOSAR Port Driver - Implementation
 * =====================================================
 *  Module: Port Driver (MCAL)
 *  Project: Temperature-Controlled Fan System
 *  Target: STM32F103C8T6
 *  Author: [Student Name]
 *  Date: [Date]
 * =====================================================
 */

/*
 * =====================================================
 *  INCLUDES
 * =====================================================
 */
#include "Port.h"
/* TODO: Include STM32F103 hardware register definitions */
/* #include "stm32f10x.h" */

/*
 * =====================================================
 *  LOCAL MACROS AND CONSTANTS
 * =====================================================
 */

/* GPIO Register Base Addresses for STM32F103 */
#define GPIOA_BASE          0x40010800UL
#define GPIOB_BASE          0x40010C00UL
#define GPIOC_BASE          0x40011000UL
#define RCC_BASE            0x40021000UL

/* GPIO Register Offsets */
#define GPIO_CRL_OFFSET     0x00    /* Port configuration register low */
#define GPIO_CRH_OFFSET     0x04    /* Port configuration register high */
#define GPIO_IDR_OFFSET     0x08    /* Port input data register */
#define GPIO_ODR_OFFSET     0x0C    /* Port output data register */
#define GPIO_BSRR_OFFSET    0x10    /* Port bit set/reset register */
#define GPIO_BRR_OFFSET     0x14    /* Port bit reset register */
#define GPIO_LCKR_OFFSET    0x18    /* Port configuration lock register */

/* RCC Register Offsets */
#define RCC_APB2ENR_OFFSET  0x18    /* APB2 peripheral clock enable register */

/* Clock Enable Bits */
#define RCC_APB2ENR_IOPAEN  (1UL << 2)   /* GPIOA clock enable */
#define RCC_APB2ENR_IOPBEN  (1UL << 3)   /* GPIOB clock enable */
#define RCC_APB2ENR_IOPCEN  (1UL << 4)   /* GPIOC clock enable */
#define RCC_APB2ENR_AFIOEN  (1UL << 0)   /* AFIO clock enable */

/* GPIO Configuration Values */
#define GPIO_MODE_INPUT     0x0     /* Input mode */
#define GPIO_MODE_OUTPUT_10 0x1     /* Output 10MHz */
#define GPIO_MODE_OUTPUT_2  0x2     /* Output 2MHz */
#define GPIO_MODE_OUTPUT_50 0x3     /* Output 50MHz */

#define GPIO_CNF_ANALOG     0x0     /* Analog input */
#define GPIO_CNF_FLOATING   0x1     /* Floating input */
#define GPIO_CNF_PULLUP     0x2     /* Input with pull-up/down */
#define GPIO_CNF_OUT_PP     0x0     /* Output push-pull */
#define GPIO_CNF_OUT_OD     0x1     /* Output open-drain */
#define GPIO_CNF_AF_PP      0x2     /* Alternate function push-pull */
#define GPIO_CNF_AF_OD      0x3     /* Alternate function open-drain */

/*
 * =====================================================
 *  LOCAL VARIABLES
 * =====================================================
 */

/* Port driver initialization state */
static boolean Port_Initialized = FALSE;

/* Pointer to current configuration */
static const Port_ConfigType* Port_ConfigPtr = NULL_PTR;

/*
 * =====================================================
 *  LOCAL FUNCTION PROTOTYPES
 * =====================================================
 */

/*
 * Function: Port_GetGpioBase
 * Description: Get GPIO base address from pin number
 * Parameters: Pin - Port pin ID
 * Return: Base address of GPIO port
 */
static uint32 Port_GetGpioBase(Port_PinType Pin);

/*
 * Function: Port_GetPinNumber
 * Description: Get pin number within port (0-15)
 * Parameters: Pin - Port pin ID
 * Return: Pin number (0-15)
 */
static uint8 Port_GetPinNumber(Port_PinType Pin);

/*
 * Function: Port_ConfigurePin
 * Description: Configure a single GPIO pin
 * Parameters: PinConfig - Pin configuration structure
 * Return: None
 */
static void Port_ConfigurePin(const Port_ConfigPin* PinConfig);

/*
 * Function: Port_EnableClock
 * Description: Enable clock for GPIO port
 * Parameters: Pin - Port pin ID
 * Return: None
 */
static void Port_EnableClock(Port_PinType Pin);

/*
 * Function: Port_WriteRegister
 * Description: Write value to register
 * Parameters: Address - Register address
 *             Value - Value to write
 * Return: None
 */
static void Port_WriteRegister(uint32 Address, uint32 Value);

/*
 * Function: Port_ReadRegister
 * Description: Read value from register
 * Parameters: Address - Register address
 * Return: Register value
 */
static uint32 Port_ReadRegister(uint32 Address);

/*
 * =====================================================
 *  PUBLIC FUNCTION IMPLEMENTATIONS
 * =====================================================
 */

/*
 * Function: Port_Init
 * Description: Initialize the Port Driver module
 */
void Port_Init(const Port_ConfigType* ConfigPtr)
{
    uint8 pinIndex = 0;
    
    /* TODO: Add parameter validation */
    if (ConfigPtr == NULL_PTR)
    {
        /* Report error to DET */
        /* Det_ReportError(PORT_MODULE_ID, 0, 0x00, PORT_E_PARAM_CONFIG); */
        return;
    }
    
    /* Store configuration pointer */
    Port_ConfigPtr = ConfigPtr;
    
    /* Configure each pin according to configuration */
    for (pinIndex = 0; pinIndex < ConfigPtr->NumPins; pinIndex++)
    {
        /* Enable clock for the GPIO port */
        Port_EnableClock(ConfigPtr->Pins[pinIndex].PinId);
        
        /* Configure the pin */
        Port_ConfigurePin(&ConfigPtr->Pins[pinIndex]);
    }
    
    /* Mark driver as initialized */
    Port_Initialized = TRUE;
}

/*
 * Function: Port_SetPinDirection
 * Description: Set the port pin direction
 */
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction)
{
    uint32 gpioBase = 0;
    uint8 pinNumber = 0;
    uint32 configReg = 0;
    uint32 regOffset = 0;
    uint8 bitPosition = 0;
    uint32 mode = 0;
    
    /* Check if Port driver is initialized */
    if (Port_Initialized == FALSE)
    {
        /* Report error to DET */
        /* Det_ReportError(PORT_MODULE_ID, 0, 0x01, PORT_E_UNINIT); */
        return;
    }
    
    /* TODO: Check if pin direction is changeable */
    /* This requires checking the pin configuration */
    
    /* Get GPIO base address and pin number */
    gpioBase = Port_GetGpioBase(Pin);
    pinNumber = Port_GetPinNumber(Pin);
    
    /* Determine register offset (CRL for pins 0-7, CRH for pins 8-15) */
    if (pinNumber < 8)
    {
        regOffset = GPIO_CRL_OFFSET;
        bitPosition = pinNumber * 4;
    }
    else
    {
        regOffset = GPIO_CRH_OFFSET;
        bitPosition = (pinNumber - 8) * 4;
    }
    
    /* Read current configuration register */
    configReg = Port_ReadRegister(gpioBase + regOffset);
    
    /* Clear the mode bits for this pin */
    configReg &= ~(0xF << bitPosition);
    
    /* Set new mode based on direction */
    if (Direction == PORT_PIN_IN)
    {
        mode = (GPIO_CNF_FLOATING << 2) | GPIO_MODE_INPUT;
    }
    else
    {
        mode = (GPIO_CNF_OUT_PP << 2) | GPIO_MODE_OUTPUT_50;
    }
    
    /* Set the new configuration */
    configReg |= (mode << bitPosition);
    
    /* Write back to register */
    Port_WriteRegister(gpioBase + regOffset, configReg);
}

/*
 * Function: Port_RefreshPortDirection
 * Description: Refresh port direction for all configured pins
 */
void Port_RefreshPortDirection(void)
{
    uint8 pinIndex = 0;
    
    /* Check if Port driver is initialized */
    if (Port_Initialized == FALSE)
    {
        return;
    }
    
    /* Check if configuration pointer is valid */
    if (Port_ConfigPtr == NULL_PTR)
    {
        return;
    }
    
    /* Refresh direction for all pins with unchangeable direction */
    for (pinIndex = 0; pinIndex < Port_ConfigPtr->NumPins; pinIndex++)
    {
        if (Port_ConfigPtr->Pins[pinIndex].DirectionChangeable == PORT_PIN_DIRECTION_UNCHANGEABLE)
        {
            /* Reconfigure pin direction */
            Port_SetPinDirection(Port_ConfigPtr->Pins[pinIndex].PinId, 
                               Port_ConfigPtr->Pins[pinIndex].PinDirection);
        }
    }
}

/*
 * Function: Port_GetVersionInfo
 * Description: Return the version information of this module
 */
void Port_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
    /* Check for null pointer */
    if (versioninfo == NULL_PTR)
    {
        /* Report error to DET */
        /* Det_ReportError(PORT_MODULE_ID, 0, 0x03, PORT_E_PARAM_POINTER); */
        return;
    }
    
    /* Fill version information */
    versioninfo->vendorID = PORT_VENDOR_ID;
    versioninfo->moduleID = PORT_MODULE_ID;
    versioninfo->sw_major_version = PORT_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = PORT_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = PORT_SW_PATCH_VERSION;
}

/*
 * Function: Port_SetPinMode
 * Description: Set the port pin mode
 */
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode)
{
    uint32 gpioBase = 0;
    uint8 pinNumber = 0;
    uint32 configReg = 0;
    uint32 regOffset = 0;
    uint8 bitPosition = 0;
    uint32 pinConfig = 0;
    
    /* Check if Port driver is initialized */
    if (Port_Initialized == FALSE)
    {
        /* Report error to DET */
        /* Det_ReportError(PORT_MODULE_ID, 0, 0x04, PORT_E_UNINIT); */
        return;
    }
    
    /* Get GPIO base address and pin number */
    gpioBase = Port_GetGpioBase(Pin);
    pinNumber = Port_GetPinNumber(Pin);
    
    /* Determine register offset */
    if (pinNumber < 8)
    {
        regOffset = GPIO_CRL_OFFSET;
        bitPosition = pinNumber * 4;
    }
    else
    {
        regOffset = GPIO_CRH_OFFSET;
        bitPosition = (pinNumber - 8) * 4;
    }
    
    /* Determine pin configuration based on mode */
    switch (Mode)
    {
        case PORT_PIN_MODE_DIO:
            /* Digital I/O - check current direction */
            pinConfig = (GPIO_CNF_OUT_PP << 2) | GPIO_MODE_OUTPUT_50;
            break;
            
        case PORT_PIN_MODE_ADC:
            /* Analog input */
            pinConfig = (GPIO_CNF_ANALOG << 2) | GPIO_MODE_INPUT;
            break;
            
        case PORT_PIN_MODE_PWM:
            /* PWM output (alternate function) */
            pinConfig = (GPIO_CNF_AF_PP << 2) | GPIO_MODE_OUTPUT_50;
            break;
            
        /* TODO: Add other modes as needed */
        case PORT_PIN_MODE_UART_TX:
            pinConfig = (GPIO_CNF_AF_PP << 2) | GPIO_MODE_OUTPUT_50;
            break;
            
        case PORT_PIN_MODE_UART_RX:
            pinConfig = (GPIO_CNF_FLOATING << 2) | GPIO_MODE_INPUT;
            break;
            
        default:
            /* Invalid mode - report error */
            /* Det_ReportError(PORT_MODULE_ID, 0, 0x04, PORT_E_PARAM_INVALID_MODE); */
            return;
    }
    
    /* Read current configuration register */
    configReg = Port_ReadRegister(gpioBase + regOffset);
    
    /* Clear the configuration bits for this pin */
    configReg &= ~(0xF << bitPosition);
    
    /* Set the new configuration */
    configReg |= (pinConfig << bitPosition);
    
    /* Write back to register */
    Port_WriteRegister(gpioBase + regOffset, configReg);
}

/*
 * =====================================================
 *  LOCAL FUNCTION IMPLEMENTATIONS
 * =====================================================
 */

/*
 * Function: Port_GetGpioBase
 * Description: Get GPIO base address from pin number
 */
static uint32 Port_GetGpioBase(Port_PinType Pin)
{
    uint32 baseAddress = 0;
    
    /* Calculate port from pin number */
    /* Assuming: PA0-PA15 = 0-15, PB0-PB15 = 16-31, PC0-PC15 = 32-47, etc. */
    if (Pin < 16)
    {
        baseAddress = GPIOA_BASE;
    }
    else if (Pin < 32)
    {
        baseAddress = GPIOB_BASE;
    }
    else if (Pin < 48)
    {
        baseAddress = GPIOC_BASE;
    }
    else
    {
        /* TODO: Add support for more ports if needed */
        baseAddress = GPIOA_BASE;  /* Default fallback */
    }
    
    return baseAddress;
}

/*
 * Function: Port_GetPinNumber
 * Description: Get pin number within port (0-15)
 */
static uint8 Port_GetPinNumber(Port_PinType Pin)
{
    return (uint8)(Pin % 16);
}

/*
 * Function: Port_ConfigurePin
 * Description: Configure a single GPIO pin
 */
static void Port_ConfigurePin(const Port_ConfigPin* PinConfig)
{
    uint32 gpioBase = 0;
    uint8 pinNumber = 0;
    uint32 configReg = 0;
    uint32 regOffset = 0;
    uint8 bitPosition = 0;
    uint32 pinConfigValue = 0;
    
    /* Get GPIO base and pin number */
    gpioBase = Port_GetGpioBase(PinConfig->PinId);
    pinNumber = Port_GetPinNumber(PinConfig->PinId);
    
    /* Determine register offset */
    if (pinNumber < 8)
    {
        regOffset = GPIO_CRL_OFFSET;
        bitPosition = pinNumber * 4;
    }
    else
    {
        regOffset = GPIO_CRH_OFFSET;
        bitPosition = (pinNumber - 8) * 4;
    }
    
    /* Determine configuration based on pin mode and direction */
    switch (PinConfig->PinMode)
    {
        case PORT_PIN_MODE_DIO:
            if (PinConfig->PinDirection == PORT_PIN_IN)
            {
                pinConfigValue = (GPIO_CNF_FLOATING << 2) | GPIO_MODE_INPUT;
            }
            else
            {
                pinConfigValue = (GPIO_CNF_OUT_PP << 2) | GPIO_MODE_OUTPUT_50;
            }
            break;
            
        case PORT_PIN_MODE_ADC:
            pinConfigValue = (GPIO_CNF_ANALOG << 2) | GPIO_MODE_INPUT;
            break;
            
        case PORT_PIN_MODE_PWM:
            pinConfigValue = (GPIO_CNF_AF_PP << 2) | GPIO_MODE_OUTPUT_50;
            break;
            
        default:
            pinConfigValue = (GPIO_CNF_FLOATING << 2) | GPIO_MODE_INPUT;
            break;
    }
    
    /* Read current configuration register */
    configReg = Port_ReadRegister(gpioBase + regOffset);
    
    /* Clear the configuration bits for this pin */
    configReg &= ~(0xF << bitPosition);
    
    /* Set the new configuration */
    configReg |= (pinConfigValue << bitPosition);
    
    /* Write back to register */
    Port_WriteRegister(gpioBase + regOffset, configReg);
    
    /* Set initial level for output pins */
    if ((PinConfig->PinDirection == PORT_PIN_OUT) && 
        (PinConfig->PinMode == PORT_PIN_MODE_DIO))
    {
        if (PinConfig->PinLevelValue == PORT_PIN_LEVEL_HIGH)
        {
            Port_WriteRegister(gpioBase + GPIO_BSRR_OFFSET, (1UL << pinNumber));
        }
        else
        {
            Port_WriteRegister(gpioBase + GPIO_BRR_OFFSET, (1UL << pinNumber));
        }
    }
}

/*
 * Function: Port_EnableClock
 * Description: Enable clock for GPIO port
 */
static void Port_EnableClock(Port_PinType Pin)
{
    uint32 clockBit = 0;
    uint32 rccReg = 0;
    
    /* Determine which clock to enable based on port */
    if (Pin < 16)  /* GPIOA */
    {
        clockBit = RCC_APB2ENR_IOPAEN;
    }
    else if (Pin < 32)  /* GPIOB */
    {
        clockBit = RCC_APB2ENR_IOPBEN;
    }
    else if (Pin < 48)  /* GPIOC */
    {
        clockBit = RCC_APB2ENR_IOPCEN;
    }
    
    /* Read current RCC register */
    rccReg = Port_ReadRegister(RCC_BASE + RCC_APB2ENR_OFFSET);
    
    /* Enable the clock */
    rccReg |= clockBit;
    
    /* Also enable AFIO clock for alternate functions */
    rccReg |= RCC_APB2ENR_AFIOEN;
    
    /* Write back to RCC register */
    Port_WriteRegister(RCC_BASE + RCC_APB2ENR_OFFSET, rccReg);
}

/*
 * Function: Port_WriteRegister
 * Description: Write value to register
 */
static void Port_WriteRegister(uint32 Address, uint32 Value)
{
    /* TODO: Replace with proper register access */
    /* For now, direct pointer access (should be done through CMSIS) */
    *((volatile uint32*)Address) = Value;
}

/*
 * Function: Port_ReadRegister
 * Description: Read value from register
 */
static uint32 Port_ReadRegister(uint32 Address)
{
    /* TODO: Replace with proper register access */
    /* For now, direct pointer access (should be done through CMSIS) */
    return *((volatile uint32*)Address);
}

/*
 * =====================================================
 *  IMPLEMENTATION NOTES FOR STUDENTS
 * =====================================================
 * 
 * 1. Register Access:
 *    - Replace direct pointer access with CMSIS functions
 *    - Use proper volatile qualifiers for memory-mapped registers
 *    - Add memory barriers if needed for optimization safety
 * 
 * 2. Error Handling:
 *    - Implement DET (Development Error Tracer) reporting
 *    - Add parameter validation for all public functions
 *    - Handle edge cases and invalid configurations
 * 
 * 3. Pin Mapping:
 *    - Current implementation assumes simple linear mapping
 *    - Adjust pin numbering scheme if needed for your configuration
 *    - Add support for additional ports (PD, PE, etc.) if required
 * 
 * 4. Clock Management:
 *    - Ensure clocks are enabled before accessing GPIO registers
 *    - Consider adding clock disable functionality for power saving
 *    - Handle PLL and system clock configuration if needed
 * 
 * 5. Alternate Function Configuration:
 *    - Some pins may require AFIO remapping
 *    - Check STM32F103 datasheet for specific alternate function requirements
 *    - Configure AFIO registers if needed for PWM/ADC functions
 * 
 * 6. Performance Optimization:
 *    - Consider grouping register accesses to reduce overhead
 *    - Use bit-banding for atomic bit operations if beneficial
 *    - Optimize for code size vs. execution speed based on requirements
 * 
 * =====================================================
 */
