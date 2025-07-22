/*
 * =====================================================
 *  AUTOSAR PWM Driver - Implementation
 * =====================================================
 */

#include "Pwm.h"

/* Timer1 Base Address */
#define TIM1_BASE       0x40012C00UL
#define RCC_BASE        0x40021000UL

/* Timer Register Offsets */
#define TIM_CR1_OFFSET  0x00    /* Control Register 1 */
#define TIM_CCMR1_OFFSET 0x18   /* Capture/Compare Mode Register 1 */
#define TIM_CCER_OFFSET 0x20    /* Capture/Compare Enable Register */
#define TIM_PSC_OFFSET  0x28    /* Prescaler */
#define TIM_ARR_OFFSET  0x2C    /* Auto-reload Register */
#define TIM_CCR1_OFFSET 0x34    /* Capture/Compare Register 1 */

/* RCC Register Offsets */
#define RCC_APB2ENR_OFFSET 0x18

/* Timer Control Bits */
#define TIM_CR1_CEN     (1UL << 0)      /* Counter Enable */
#define TIM_CCMR1_OC1M  (6UL << 4)      /* Output Compare 1 Mode (PWM mode 1) */
#define TIM_CCMR1_OC1PE (1UL << 3)      /* Output Compare 1 Preload Enable */
#define TIM_CCER_CC1E   (1UL << 0)      /* Capture/Compare 1 Output Enable */

/* Clock Enable Bits */
#define RCC_APB2ENR_TIM1EN (1UL << 11)  /* TIM1 Clock Enable */

/* Module State */
static boolean Pwm_Initialized = FALSE;

/* Initialize PWM */
void Pwm_Init(const Pwm_ConfigType* ConfigPtr)
{
    uint32 rccReg = 0;
    
    (void)ConfigPtr;  /* Suppress unused parameter warning */
    
    /* Enable TIM1 clock */
    rccReg = *((volatile uint32*)(RCC_BASE + RCC_APB2ENR_OFFSET));
    rccReg |= RCC_APB2ENR_TIM1EN;
    *((volatile uint32*)(RCC_BASE + RCC_APB2ENR_OFFSET)) = rccReg;
    
    /* Configure Timer1 for PWM */
    /* Prescaler: 72MHz / 72 = 1MHz */
    *((volatile uint32*)(TIM1_BASE + TIM_PSC_OFFSET)) = 71;
    
    /* Auto-reload: 1MHz / 100 = 10kHz PWM frequency */
    *((volatile uint32*)(TIM1_BASE + TIM_ARR_OFFSET)) = 99;
    
    /* Configure Channel 1 for PWM mode */
    *((volatile uint32*)(TIM1_BASE + TIM_CCMR1_OFFSET)) = TIM_CCMR1_OC1M | TIM_CCMR1_OC1PE;
    
    /* Enable Channel 1 output */
    *((volatile uint32*)(TIM1_BASE + TIM_CCER_OFFSET)) = TIM_CCER_CC1E;
    
    /* Set initial duty cycle to 0 */
    *((volatile uint32*)(TIM1_BASE + TIM_CCR1_OFFSET)) = 0;
    
    /* Enable counter */
    *((volatile uint32*)(TIM1_BASE + TIM_CR1_OFFSET)) = TIM_CR1_CEN;
    
    Pwm_Initialized = TRUE;
}

/* Set Duty Cycle */
void Pwm_SetDutyCycle(Pwm_ChannelType ChannelNumber, uint16 DutyCycle)
{
    uint32 compareValue = 0;
    
    if (!Pwm_Initialized || ChannelNumber != 0)
    {
        return;
    }
    
    /* Convert 16-bit duty cycle (0x0000-0x8000) to timer compare value */
    /* DutyCycle 0x8000 = 100%, 0x4000 = 50%, 0x0000 = 0% */
    compareValue = ((uint32)DutyCycle * 100) / 0x8000;
    
    /* Set compare register */
    *((volatile uint32*)(TIM1_BASE + TIM_CCR1_OFFSET)) = compareValue;
}

/* Set Period and Duty */
void Pwm_SetPeriodAndDuty(Pwm_ChannelType ChannelNumber, Pwm_PeriodType Period, uint16 DutyCycle)
{
    if (!Pwm_Initialized || ChannelNumber != 0)
    {
        return;
    }
    
    /* Set new period */
    *((volatile uint32*)(TIM1_BASE + TIM_ARR_OFFSET)) = Period - 1;
    
    /* Set duty cycle */
    Pwm_SetDutyCycle(ChannelNumber, DutyCycle);
}
