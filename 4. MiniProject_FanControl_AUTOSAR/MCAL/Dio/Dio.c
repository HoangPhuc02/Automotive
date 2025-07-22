/*
 * =====================================================
 *  AUTOSAR DIO Driver - Implementation
 * =====================================================
 */

#include "Dio.h"

/* GPIO Base Addresses */
#define GPIOA_BASE    0x40010800UL
#define GPIOC_BASE    0x40011000UL

/* GPIO Register Offsets */
#define GPIO_IDR_OFFSET    0x08    /* Input Data Register */
#define GPIO_ODR_OFFSET    0x0C    /* Output Data Register */
#define GPIO_BSRR_OFFSET   0x10    /* Bit Set/Reset Register */

/* Helper Functions */
static uint32 Dio_GetGpioBase(Dio_ChannelType ChannelId)
{
    if (ChannelId < 16) return GPIOA_BASE;
    if (ChannelId < 48) return GPIOC_BASE;
    return GPIOA_BASE;  /* Default */
}

static uint8 Dio_GetPinNumber(Dio_ChannelType ChannelId)
{
    return (uint8)(ChannelId % 16);
}

/* Read Channel */
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId)
{
    uint32 gpioBase = Dio_GetGpioBase(ChannelId);
    uint8 pinNumber = Dio_GetPinNumber(ChannelId);
    uint32 inputReg = *((volatile uint32*)(gpioBase + GPIO_IDR_OFFSET));
    
    return ((inputReg >> pinNumber) & 0x1) ? STD_HIGH : STD_LOW;
}

/* Write Channel */
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level)
{
    uint32 gpioBase = Dio_GetGpioBase(ChannelId);
    uint8 pinNumber = Dio_GetPinNumber(ChannelId);
    
    if (Level == STD_HIGH)
    {
        /* Set bit using BSRR register */
        *((volatile uint32*)(gpioBase + GPIO_BSRR_OFFSET)) = (1UL << pinNumber);
    }
    else
    {
        /* Reset bit using BSRR register (upper 16 bits) */
        *((volatile uint32*)(gpioBase + GPIO_BSRR_OFFSET)) = (1UL << (pinNumber + 16));
    }
}

/* Read Port */
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId)
{
    /* TODO: Implement port reading */
    (void)PortId;
    return 0;
}

/* Write Port */
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level)
{
    /* TODO: Implement port writing */
    (void)PortId;
    (void)Level;
}

/* Flip Channel */
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId)
{
    Dio_LevelType currentLevel = Dio_ReadChannel(ChannelId);
    Dio_LevelType newLevel = (currentLevel == STD_HIGH) ? STD_LOW : STD_HIGH;
    Dio_WriteChannel(ChannelId, newLevel);
    return newLevel;
}
