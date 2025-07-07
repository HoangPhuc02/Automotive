/**********************************************************
* File Name   : main 
* Description : Main Source File
* Details     : Main file implementation.
* Version     : 1.0.0
* Date        : 20/06/2025
* Author      : hoangphuc540202@gmail.com
* Github      : https://github.com/HoangPhuc02
 **********************************************************/

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "misc.h" // For NVIC configuration
#include "Dio.h"
#include "Port.h"
#include "Adc.h"
#include "Adc_Types.h"

// ADC channel
typedef enum 
{
    ADC_CHANNEL_0 = 0, /*!< ADC Channel 0 */
    ADC_CHANNEL_1,     /*!< ADC Channel 1 */
    ADC_CHANNEL_2,     /*!< ADC Channel 2 */
    ADC_CHANNEL_3,     /*!< ADC Channel 3 */
    ADC_CHANNEL_4,     /*!< ADC Channel 4 */
    ADC_CHANNEL_5,     /*!< ADC Channel 5 */
    ADC_CHANNEL_6,     /*!< ADC Channel 6 */
    ADC_CHANNEL_7,     /*!< ADC Channel 7 */
    ADC_CHANNEL_8,     /*!< ADC Channel 8 */
    ADC_CHANNEL_9,     /*!< ADC Channel 9 */
    ADC_CHANNEL_10,    /*!< ADC Channel 10 */
    ADC_CHANNEL_11,    /*!< ADC Channel 11 */
    ADC_CHANNEL_12,    /*!< ADC Channel 12 */
    ADC_CHANNEL_13,    /*!< ADC Channel 13 */
    ADC_CHANNEL_14,    /*!< ADC Channel 14 */
    ADC_CHANNEL_15     /*!< ADC Channel 15 */
}Adc_ChannelId;
// extern const Port_ConfigType PortCfg_Port; 
void delay(uint16_t time)
{
    TIM_SetCounter(TIM2, 0);
    while (TIM_GetCounter(TIM2) < time)
        ;
}
extern const Port_ConfigType PortCfg_Port; 
extern Adc_ConfigType Adc_Config;
void Timer_Init(void);

int main()
{
    // Initialize the hardware
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    Timer_Init();
    Port_Init(&PortCfg_Port);
    Adc_Init(&Adc_Config);
    while (1)
    {
        // Start ADC conversion
        Adc_StartGroupConversion(Adc_Config.Groups[0].Adc_GroupId);

        // Wait for conversion to complete
        while (!Adc_GetGroupStatus(Adc_Config.Groups[0].Adc_GroupId));

        // Read ADC value
        Adc_ValueGroupType adcValue[2];
        Adc_ReadGroup(Adc_Config.Groups[0].Adc_GroupId, adcValue);

        // Do something with adcValue
        delay(500); // Delay for 500ms
    }
}

void Timer_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef tim;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_Period = 0xFFFF;
    tim.TIM_Prescaler = 8000 - 1; // Prescaler for 1ms tick
    TIM_TimeBaseInit(TIM2, &tim);
    TIM_Cmd(TIM2, ENABLE);
}
