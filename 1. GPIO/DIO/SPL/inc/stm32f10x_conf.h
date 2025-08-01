/**
 ******************************************************************************
 * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_conf.h
 * @author  MCD Application Team
 * @version V3.6.0
 * @date    20-September-2021
 * @brief   Library configuration file.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2011 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_CONF_H
#define __STM32F10x_CONF_H

/* Includes ------------------------------------------------------------------*/
/* Run Time Environment will set specific #define for each selected module below */

#ifdef RTE_DEVICE_STDPERIPH_ADC
#include "stm32f10x_adc.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_BKP
#include "stm32f10x_bkp.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_CAN
#include "stm32f10x_can.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_CEC
#include "stm32f10x_cec.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_CRC
#include "stm32f10x_crc.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_DAC
#include "stm32f10x_dac.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_DBGMCU
#include "stm32f10x_dbgmcu.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_DMA
#include "stm32f10x_dma.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_EXTI
#include "stm32f10x_exti.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_FLASH
#include "stm32f10x_flash.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_FSMC
#include "stm32f10x_fsmc.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_GPIO
#include "stm32f10x_gpio.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_I2C
#include "stm32f10x_i2c.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_IWDG
#include "stm32f10x_iwdg.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_PWR
#include "stm32f10x_pwr.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_RCC
#include "stm32f10x_rcc.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_RTC
#include "stm32f10x_rtc.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_SDIO
#include "stm32f10x_sdio.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_SPI
#include "stm32f10x_spi.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_TIM
#include "stm32f10x_tim.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_USART
#include "stm32f10x_usart.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_WWDG
#include "stm32f10x_wwdg.h"
#endif
#ifdef RTE_DEVICE_STDPERIPH_FRAMEWORK
#include "misc.h" /* High level functions for NVIC and SysTick (add-on to CMSIS functions) */
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below to expanse the "assert_param" macro in the
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */

/* Exported macro ------------------------------------------------------------*/
#ifdef USE_FULL_ASSERT

/**
 * @brief  The assert_param macro is used for function's parameters check.
 * @param  expr: If expr is false, it calls assert_failed function which reports
 *         the name of the source file and the source line number of the call
 *         that failed. If expr is true, it returns no value.
 * @retval None
 */
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
void assert_failed(uint8_t *file, uint32_t line);
#else
#define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif /* __STM32F10x_CONF_H */
