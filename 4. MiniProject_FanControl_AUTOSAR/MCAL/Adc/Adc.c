/*
 * =====================================================
 *  AUTOSAR ADC Driver - Implementation
 * =====================================================
 */

#include "Adc.h"

/* ADC Register Base Address */
#define ADC1_BASE       0x40012400UL
#define RCC_BASE        0x40021000UL

/* ADC Register Offsets */
#define ADC_SR_OFFSET   0x00    /* Status Register */
#define ADC_CR1_OFFSET  0x04    /* Control Register 1 */
#define ADC_CR2_OFFSET  0x08    /* Control Register 2 */
#define ADC_SMPR2_OFFSET 0x10   /* Sample Time Register 2 */
#define ADC_SQR3_OFFSET 0x34    /* Regular Sequence Register 3 */
#define ADC_DR_OFFSET   0x4C    /* Data Register */

/* RCC Register Offsets */
#define RCC_APB2ENR_OFFSET 0x18

/* ADC Control Bits */
#define ADC_CR2_ADON    (1UL << 0)    /* ADC On */
#define ADC_CR2_SWSTART (1UL << 30)   /* Start Conversion */
#define ADC_SR_EOC      (1UL << 1)    /* End of Conversion */

/* Clock Enable Bits */
#define RCC_APB2ENR_ADC1EN (1UL << 9)  /* ADC1 Clock Enable */

/* Module State */
static boolean Adc_Initialized = FALSE;
static Adc_StatusType Adc_GroupStatus[1] = {ADC_IDLE};

/* Initialize ADC */
void Adc_Init(const Adc_ConfigType* ConfigPtr)
{
    uint32 rccReg = 0;
    
    (void)ConfigPtr;  /* Suppress unused parameter warning */
    
    /* Enable ADC1 clock */
    rccReg = *((volatile uint32*)(RCC_BASE + RCC_APB2ENR_OFFSET));
    rccReg |= RCC_APB2ENR_ADC1EN;
    *((volatile uint32*)(RCC_BASE + RCC_APB2ENR_OFFSET)) = rccReg;
    
    /* Configure ADC */
    /* Set sample time for channel 0 (55.5 cycles) */
    *((volatile uint32*)(ADC1_BASE + ADC_SMPR2_OFFSET)) = 0x3;  /* 011 = 55.5 cycles */
    
    /* Configure regular sequence - channel 0 */
    *((volatile uint32*)(ADC1_BASE + ADC_SQR3_OFFSET)) = 0;     /* Channel 0 */
    
    /* Enable ADC */
    *((volatile uint32*)(ADC1_BASE + ADC_CR2_OFFSET)) = ADC_CR2_ADON;
    
    /* Wait for ADC to stabilize */
    for (volatile uint32 i = 0; i < 1000; i++);
    
    Adc_Initialized = TRUE;
}

/* Start Group Conversion */
Std_ReturnType Adc_StartGroupConversion(Adc_GroupType Group)
{
    if (!Adc_Initialized || Group >= 1)
    {
        return E_NOT_OK;
    }
    
    /* Start conversion */
    *((volatile uint32*)(ADC1_BASE + ADC_CR2_OFFSET)) |= ADC_CR2_SWSTART;
    Adc_GroupStatus[Group] = ADC_BUSY;
    
    return E_OK;
}

/* Stop Group Conversion */
void Adc_StopGroupConversion(Adc_GroupType Group)
{
    if (Group < 1)
    {
        Adc_GroupStatus[Group] = ADC_IDLE;
    }
}

/* Read Group */
Std_ReturnType Adc_ReadGroup(Adc_GroupType Group, Adc_ValueType* DataBufferPtr)
{
    if (!Adc_Initialized || Group >= 1 || DataBufferPtr == NULL_PTR)
    {
        return E_NOT_OK;
    }
    
    if (Adc_GroupStatus[Group] != ADC_STREAM_COMPLETED)
    {
        return E_NOT_OK;
    }
    
    /* Read ADC result */
    *DataBufferPtr = (Adc_ValueType)(*((volatile uint32*)(ADC1_BASE + ADC_DR_OFFSET)) & 0xFFF);
    Adc_GroupStatus[Group] = ADC_IDLE;
    
    return E_OK;
}

/* Get Group Status */
Adc_StatusType Adc_GetGroupStatus(Adc_GroupType Group)
{
    if (Group >= 1)
    {
        return ADC_IDLE;
    }
    
    /* Check if conversion is complete */
    if (Adc_GroupStatus[Group] == ADC_BUSY)
    {
        uint32 statusReg = *((volatile uint32*)(ADC1_BASE + ADC_SR_OFFSET));
        if (statusReg & ADC_SR_EOC)
        {
            Adc_GroupStatus[Group] = ADC_STREAM_COMPLETED;
        }
    }
    
    return Adc_GroupStatus[Group];
}

/* Read Channel (Simplified interface) */
Adc_ValueType Adc_ReadChannel(Adc_ChannelType ChannelId)
{
    Adc_ValueType result = 0;
    
    (void)ChannelId;  /* For now, only channel 0 supported */
    
    if (!Adc_Initialized)
    {
        return 0;
    }
    
    /* Start conversion */
    Adc_StartGroupConversion(0);
    
    /* Wait for completion */
    while (Adc_GetGroupStatus(0) != ADC_STREAM_COMPLETED)
    {
        /* Wait for conversion */
    }
    
    /* Read result */
    Adc_ReadGroup(0, &result);
    
    return result;
}
