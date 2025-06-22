/****************************************************************************************
*                                   PORT.H                                             *
****************************************************************************************
* File Name   : Dio.h
* Module      : Digital Input/Output (DIO)
* Description : AUTOSAR DIO driver header file
* Detail      : This file contains the definitions of data types,
*                GPIO mapping macros, and declarations of standard
*                AUTOSAR APIs for the DIO driver. This driver is
*                designed for STM32F103 using SPL.
* Version     : 1.0.0
* Date        : 20/06/2025
* Author      : hoangphuc540202@gmail.com
* Github      : https://github.com/HoangPhuc02
****************************************************************************************/

#ifndef PORT_H
#define PORT_H

/****************************************************************************************
*                                 INCLUDE FILES                                        *
****************************************************************************************/
#include "Std_Types.h"
#include "stm32f10x_gpio.h"
#include "Config.h"
/****************************************************************************************
*                              VERSION INFORMATION                                     *
****************************************************************************************/
/*
 Port is the same as port in the hardware,
 Channel is the same as pin in the hardware,
 ChannelGroup is a group of channels (pins) on the same port.
*/   

/**********************************************************
 * Định nghĩa các giá trị PortId cho các cổng GPIO
 **********************************************************/
#define PORT_ID_A   0   /* GPIOA */
#define PORT_ID_B   1   /* GPIOB */
#define PORT_ID_C   2   /* GPIOC */
#define PORT_ID_D   3   /* GPIOD */

/**********************************************************
 * MACRO TO IDENTIFY GPIO PORT FROM PORT ID
 **********************************************************/
#define PORT_GET_PORT(PortNum) \
    ((PortNum) == PORT_ID_A ? GPIOA : \
    (PortNum) == PORT_ID_B ? GPIOB : \
    (PortNum) == PORT_ID_C ? GPIOC : \
    (PortNum) == PORT_ID_D ? GPIOD : NULL)

/**********************************************************
 * Macro xác định Pin Mask cho từng chân GPIO (0-15)
 **********************************************************/
#define PORT_GET_PIN_MASK(PinNum)   (1U << (PinNum))

/**********************************************************
 * Định nghĩa các chế độ mode và trạng thái cho pin
 **********************************************************/
#define PORT_PIN_MODE_DIO       0
#define PORT_PIN_MODE_ADC       1
#define PORT_PIN_MODE_PWM       2
#define PORT_PIN_MODE_SPI       3
#define PORT_PIN_MODE_CAN       4
#define PORT_PIN_MODE_LIN       5

#define PORT_PIN_PULL_NONE      0
#define PORT_PIN_PULL_UP        1
#define PORT_PIN_PULL_DOWN      2

#define PORT_PIN_LEVEL_LOW      0
#define PORT_PIN_LEVEL_HIGH     1

/***************************************************************************************
 *                              TYPE DEFINITIONS                                        *
****************************************************************************************/



/*
    * @typedef Port_PinType
    * @brief Type for PORT pin ID.
    * @note This type is used to identify a specific pin in the PORT driver.
    *       (0..47: A0..A15, B0..B15, C0..C15)
*/
typedef uint8 Port_PinType;


/**
 * @typedef Port_PinModeType
 * @brief   Kiểu định danh cho mode chức năng của pin (DIO, ADC, PWM...)
 */
typedef uint8 Port_PinModeType;

/**
 * @struct Port_PinConfigType
 * @brief  Cấu hình cho từng chân pin (phần mềm sẽ sinh theo tool/hoặc config tay)
 */

/* Enum and Structure typedef*/
/**
 * @typedef Port_PinDirectionType
 * @brief   ENUM for Direction of Port Pin
 */
typedef enum {
    PORT_PIN_IN  = 0x00,   // Input  
    PORT_PIN_OUT = 0x01    // Output 
} Port_PinDirectionType;

/**
 * @struct Port_PinConfigType
 * @brief  Detailed configuration for each pin
 *         - PortNum: PORT_ID_A, PORT_ID_B, ...
 *         - PinNum: 0..15 (pin index in the port)
 *         - Mode: PORT_PIN_MODE_DIO, PORT_PIN_MODE_ADC, ...
 *         - Direction: PORT_PIN_IN / PORT_PIN_OUT
 *         - DirectionChangeable: 1 = Allow runtime direction change
 *         - Level: PORT_PIN_LEVEL_HIGH / PORT_PIN_LEVEL_LOW (only if output)
 *         - Pull: PORT_PIN_PULL_NONE / UP / DOWN (only if input)
 *         - ModeChangeable: 1 = Allow runtime mode change
 */
typedef struct {
    uint8  PortNum;            /**< 0=A, 1=B, 2=C, 3=D */
    uint8  PinNum;             /**< 0..15 */
    Port_PinModeType Mode;     /**< Chế độ chức năng */
    Port_PinDirectionType Direction;  /**< Chiều ban đầu */
    uint8  DirectionChangeable;/**< 1=cho phép đổi chiều runtime */
    uint8  Level;              /**< Giá trị mức ban đầu nếu output */
    uint8  Pull;               /**< Kiểu pull: none, up, down */
    uint8  ModeChangeable;     /**< 1=cho phép đổi mode runtime */
} Port_PinConfigType;

/**
 * @struct Port_ConfigType
 * @brief  Cấu trúc tổng hợp cấu hình tất cả các pin (gán khi init)
 */
typedef struct {
    const Port_PinConfigType* PinConfigs; /**< Con trỏ tới mảng cấu hình pin */
    uint16 PinCount;                      /**< Số lượng chân cấu hình */
} Port_ConfigType;

/**********************************************************
 * Macro định nghĩa phiên bản, vendor, module ID cho VersionInfo
 **********************************************************/
#define PORT_VENDOR_ID          1234U
#define PORT_MODULE_ID          81U
#define PORT_SW_MAJOR_VERSION   1U
#define PORT_SW_MINOR_VERSION   0U
#define PORT_SW_PATCH_VERSION   0U

/**********************************************************
 * Khai báo prototype các hàm API Port Driver AUTOSAR
 **********************************************************/

/**
 * @brief   Khởi tạo toàn bộ các Port/Pin
 * @param[in] ConfigPtr Con trỏ tới cấu hình port/pin
 */
void Port_Init(const Port_ConfigType* ConfigPtr);

/**
 * @brief   Đổi chiều một chân Port (nếu được phép)
 * @param[in] Pin        Số hiệu pin
 * @param[in] Direction  Chiều cần đặt
 */
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction);

/**
 * @brief   Làm tươi lại chiều tất cả các pin không cho đổi chiều runtime
 */
void Port_RefreshPortDirection(void);

/**
 * @brief   Lấy thông tin version của Port Driver
 * @param[out] versioninfo  Con trỏ tới cấu trúc Std_VersionInfoType để nhận version
 */
void Port_GetVersionInfo(Std_VersionInfoType* versioninfo);

/**
 * @brief   Đổi mode chức năng cho một chân pin (nếu cho phép)
 * @param[in] Pin    Số hiệu pin
 * @param[in] Mode   Mode chức năng cần set
 */
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode);

#endif /* PORT_H */
