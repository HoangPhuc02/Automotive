/*
 * =====================================================
 *  AUTOSAR DIO Driver - Interface Header
 * =====================================================
 */

#ifndef DIO_H
#define DIO_H

#include "Std_Types.h"

/* DIO Channel Type */
typedef uint8 Dio_ChannelType;

/* DIO Port Type */
typedef uint8 Dio_PortType;

/* DIO Level Type */
typedef uint8 Dio_LevelType;

/* DIO Port Level Type */
typedef uint16 Dio_PortLevelType;

/* DIO Level Values */
#define STD_LOW     0u
#define STD_HIGH    1u

/* DIO Channel IDs for this project */
#define DioConf_DioChannel_LED_STATUS    45u    /* PC13 */

/* Function Prototypes */
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId);
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level);
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId);
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level);
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId);

#endif /* DIO_H */
