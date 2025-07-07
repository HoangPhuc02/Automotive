/****************************************************************************************
*                                ADC_CFG.H                                              *
****************************************************************************************
* File Name   : Adc_Cfg.h
* Module      : Analog to Digital Converter (ADC)
* Description : AUTOSAR ADC driver configuration header file 
* Version     : 1.0.0
* Date        : 24/06/2025
* Author      : hoangphuc540202@gmail.com
* Github      : https://github.com/HoangPhuc02
****************************************************************************************/

#ifndef ADC_CFG_H
#define ADC_CFG_H

/****************************************************************************************
*                                 INCLUDE FILES                                        *
****************************************************************************************/
#include "Std_Types.h"
#include "Adc_Types.h"
/****************************************************************************************
*                              CONFIGURATION PARAMETERS                                 *
****************************************************************************************/


/* ADC Result Alignment */
#define ADC_RESULT_ALIGNMENT   ADC_ALIGN_RIGHT  /* Default alignment for all results */



/* Uncomment to use left alignment */
/* #define ADC_RESULT_ALIGNMENT   ADC_ALIGN_LEFT */

#define ADC_MAX_GROUPS 10 /* Maximum number of ADC groups */
#define ADC_MAX_CHANNEL 16 /* Maximum number of ADC channels */

/* ADC ISR Configuration */
#define ADC_MAX_HW_UNITS   2  /* Maximum number of ADC hardware units for ISR handling */


/***************************************************************************************
 *                              API CONFIGURATION                                      *
****************************************************************************************/

/****************************************************************************************
*                              ADC HW UNIT CONFIGURATION                                 *
****************************************************************************************/


extern Adc_GroupDefType Adc_GroupConfig[ADC_MAX_GROUPS];
extern Adc_HwUnitDefType Adc_HwUnitConfig[ADC_MAX_HW_UNITS];
extern Adc_ChannelDefType Adc_ChannelConfig[ADC_MAX_CHANNEL];
extern Adc_ConfigType Adc_Config;

/****************************************************************************************
*                              FUNCTION CALLBACK                                         *
****************************************************************************************/
/* Function Callback for Notify */
void Adc_ChannelX_Notification(void *context);
void Adc_TransferCplt(ADC_TypeDef* ADC);
void Adc_DmaTransferCplt(DMA_Channel_TypeDef* DMA_Channel);


#endif /* ADC_CFG_H */
/****************************************************************************************
*                                 END OF FILE                                          *
****************************************************************************************/