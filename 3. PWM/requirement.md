8.2 Type definitions
 8.2.1 Pwm_ChannelType
 [SWS_Pwm_00106] Definition of datatype Pwm_ChannelType 
Name
 Pwm_ChannelType
 Kind
 Type
 Derived from
 uint
 Range
 8..32 bit
This is implementation specific but
 not all values may be valid within
 the type. This type shall be
 chosen in order to have the most
 efficient implementation on a
 specific microcontroller platform.
 Description
 Numeric identifier of a PWM channel.
 Available via
 Pwm.h
 
 
  8.2.2 Pwm_PeriodType
 [SWS_Pwm_00107] Definition of datatype Pwm_PeriodType 
Name
 Pwm_PeriodType
 Kind
 Type
 Derived from
 uint
 Range
 8..32 bit
Implementation specific. This type
 shall be chosen in order to have
 the most efficient implementation
 on a specific microcontroller
 platform.
 Description
 Definition of the period of a PWM channel.
 Available via
 Pwm.h
 8.2.3 Pwm_OutputStateType
 [SWS_Pwm_00108] Definition of datatype Pwm_OutputStateType 
Name
 Pwm_OutputStateType
 Kind
 Enumeration
 Range
 PWM_HIGH
 0x00
 The PWMchannel is in high state.
 PWM_LOW
 0x01
 The PWMchannel is in low state.
 Description
 Output state of a PWM channel.
 Available via
 Pwm.h
 8.2.4 Pwm_EdgeNotificationType
 [SWS_Pwm_00109] Definition of datatype Pwm_EdgeNotificationType 
Name
 Pwm_EdgeNotificationType
 Kind
 Enumeration
 Range
 PWM_RISING_EDGE
 0x00
 Notification will be called when a rising edge
 occurs on the PWM output signal.
 PWM_FALLING_EDGE
 0x01
 Notification will be called when a falling edge
 occurs on the PWM output signal.
 22 of 66
 Document ID 37: AUTOSAR_CP_SWS_PWMDriver
Specification of PWM Driver
 AUTOSAR CPR24-11
 PWM_BOTH_EDGES
 0x02
 Notification will be called when either a rising
 edge or falling edge occur on the PWM
 output signal.
 Description
 Definition of the type of edge notification of a PWM channel.
 Available via
 Pwm.
 
 8.2.5 Pwm_ChannelClassType
 [SWS_Pwm_00110] Definition of datatype Pwm_ChannelClassType 
Name
 Pwm_ChannelClassType
 Kind
 Enumeration
 Range
 PWM_VARIABLE_PERIOD
 0x00
 The PWMchannel has a variable period. The
 duty cycle and the period can be changed.
 PWM_FIXED_PERIOD
 0x01
 The PWMchannel has a fixed period. Only
 the duty cycle can be changed.
 PWM_FIXED_PERIOD_
 SHIFTED
 0x02
 The PWMchannel has a fixed shifted period.
 Impossible to change it ( only if supported by
 hardware)
 Description
 Defines the class of a PWM channel
 Available via
 Pwm.h
 8.2.6 Pwm_ConfigType
 [SWS_Pwm_00111] Definition of datatype Pwm_ConfigType 
Name
 Pwm_ConfigType
 Kind
 Structure
 Elements
 Hardware dependent structure.
 Type
Comment
 The contents of the initialization data structure are hardware specific.
 Description
 This is the type of data structure containing the initialization data for the PWM driver.
 Available via
 Pwm.h
 23 of 66
 Document ID 37: AUTOSAR_CP_SWS_PWMDriver
Specification of PWM Driver
 AUTOSAR CPR24-11
 8.2.7 Pwm_PowerStateRequestResultType
 [SWS_Pwm_00165] Definition of datatype Pwm_PowerStateRequestResultType
 Name
 Pwm_PowerStateRequestResultType
 Kind
 Enumeration
 Range
 PWM_SERVICE_
 ACCEPTED
 0x00
 Power state change executed.
 PWM_NOT_INIT
 0x01
 PWMModule not initialized.
 PWM_SEQUENCE_
 ERROR
 0x02
 Wrong API call sequence.
 PWM_HW_FAILURE
 0x03
 The HWmodule has a failure which prevents
 it to enter the required power state.
 PWM_POWER_STATE_
 NOT_SUPP
 0x04
 PWMModule does not support the requested
 power state.
 PWM_TRANS_NOT_
 POSSIBLE
 0x05
 PWMModule cannot transition directly from
 the current power state to the requested
 power state or the HW peripheral is still busy.
 Description
 Result of the requests related to power state transitions.
 Available via
 Pwm.h
 8.2.8 Pwm_PowerStateType
 [SWS_Pwm_00197] Definition of datatype Pwm_PowerStateType
 Upstream requirements: SRS_Pwm_12293, SRS_Pwm_12378
 Name
 Pwm_PowerStateType
 Kind
 Enumeration
 Range
 1..255
power modes with decreasing power
 consumptions.
 PWM_FULL_POWER
 0x00
 Full Power
 Description
 Power state currently active or set as target power state.
 Available via
 Pwm.h
 Mandatory parameters:
 Assigned HW channel
 Default value for period
 24 of 66
 Document ID 37: AUTOSAR_CP_SWS_PWMDriver
Specification of PWM Driver
 AUTOSAR CPR24-11
 Default value for duty cycle
 Polarity (high or low)
 Idle state high or low
 Channel class:– Fixed period– Fixed period, shifted (if supported by hardware)– Variable period
 Optional parameters (if supported by hardware):
 Channel phase shift
 Reference channel for phase shift
 Microcontroller specific channel propertie
 
 
 8.3 Function definitions
 8.3.1 Pwm_Init
 [SWS_Pwm_00095] Definition of API function Pwm_Init 
Service Name
 Pwm_Init
 Syntax
 void Pwm_Init (
 const Pwm_ConfigType* ConfigPtr
 )
 Service ID [hex]
 0x00
 Sync/Async
 Synchronous
 Reentrancy
 Non Reentrant
 Parameters (in)
 ConfigPtr
 Pointer to configuration set
 Parameters (inout)
 None
 Parameters (out)
 None
 Return value
 None
 Description
 Service for PWM initialization.
 Available via
 Pwm.h
 25 of 66
 
  8.3.2 Pwm_DeInit
 [SWS_Pwm_00096] Definition of API function Pwm_DeInit 
Service Name
 Pwm_DeInit
 Syntax
 void Pwm_DeInit (
 void
 )
 Service ID [hex]
 0x01
 Sync/Async
 Synchronous
 Reentrancy
 Non Reentrant
 Parameters (in)
 None
 Parameters (inout)
 None
 Parameters (out)
 None
 Return value
 None
 Description
 Service for PWM De-Initialization.
 Available via
 Pwm.hardware
 
  8.3.3 Pwm_SetDutyCycle
 [SWS_Pwm_91000] Definition of API function Pwm_SetDutyCycle 
Service Name
 Pwm_SetDutyCycle
 Syntax
 void Pwm_SetDutyCycle (
 Pwm_ChannelType ChannelNumber,
 uint16 DutyCycle
 )
 Service ID [hex]
 0x02
 Sync/Async
 Asynchronous
 Reentrancy
 Reentrant for different channel numbers
 Parameters (in)
 ChannelNumber
 Numeric identifier of the PWM
 DutyCycle
 Min=0x0000 Max=0x8000
 Parameters (inout)
 None
 Parameters (out)
 None
 Return value
 None
 Description
 Service sets the duty cycle of the PWM channel.
 Available via
 Pwm.h
 28 of 66


8.3.4 Pwm_SetPeriodAndDuty
 [SWS_Pwm_91001] Definition of API function Pwm_SetPeriodAndDuty 
Service Name
 Pwm_SetPeriodAndDuty
 Syntax
 void Pwm_SetPeriodAndDuty (
 Pwm_ChannelType ChannelNumber,
 Pwm_PeriodType Period,
 uint16 DutyCycle
 )
 Service ID [hex]
 0x03
 Sync/Async
 Asynchronous
 Reentrancy
 Reentrant for different channel numbers
 Parameters (in)
 ChannelNumber
 Numeric identifier of the PWM
 Period
 Period of the PWM signal
 DutyCycle
 Min=0x0000 Max=0x8000
 Parameters (inout)
 None
 Parameters (out)
 None
 Return value
 None
 Description
 Service sets the period and the duty cycle of a PWM channel
 Available via
 Pwm.h
 
  8.3.5 Pwm_SetOutputToIdle
 [SWS_Pwm_91002] Definition of API function Pwm_SetOutputToIdle 
Service Name
 Pwm_SetOutputToIdle
 Syntax
 void Pwm_SetOutputToIdle (
 Pwm_ChannelType ChannelNumber
 )
 Service ID [hex]
 0x04
 Sync/Async
 Asynchronous
 Reentrancy
 Reentrant for different channel numbers
 Parameters (in)
 ChannelNumber
 Numeric identifier of the PWM
 Parameters (inout)
 None
 Parameters (out)
 None
 31 of 66
 Document ID 37: AUTOSAR_CP_SWS_PWMDriver
Specification of PWM Driver
 AUTOSAR CPR24-11
 Return value
 None
 Description
 Service sets the PWM output to the configured Idle state.
 Available via
 Pwm.h 
 
  8.3.6 Pwm_GetOutputState
 [SWS_Pwm_00100] Definition of API function Pwm_GetOutputState 
Service Name
 Pwm_GetOutputState
 Syntax
 Pwm_OutputStateType Pwm_GetOutputState (
 Pwm_ChannelType ChannelNumber
 )
 Service ID [hex]
 0x05
 Sync/Async
 Synchronous
 Reentrancy
 Reentrant for different channel numbers
 Parameters (in)
 ChannelNumber
 Numeric identifier of the PWM
 Parameters (inout)
 None
 Parameters (out)
 None
 Return value
 Pwm_OutputStateType
 PWM_HIGH The PWM output state is high
 PWM_LOW The PWM output state is low
 Description
 Service to read the internal state of the PWM output signal.
 Available via
 Pwm.h
 
  8.3.7 Pwm_DisableNotification
 [SWS_Pwm_91003] Definition of API function Pwm_DisableNotification 
Service Name
 Pwm_DisableNotification
 Syntax
 void Pwm_DisableNotification (
 Pwm_ChannelType ChannelNumber
 )
 Service ID [hex]
 0x06
 Sync/Async
 Asynchronous
 Reentrancy
 Reentrant for different channel numbers
 Parameters (in)
 ChannelNumber
 Numeric identifier of the PWM
 Parameters (inout)
 None
 Parameters (out)
 None
 Return value
 None
 Description
 Service to disable the PWM signal edge notification.
 Available via
 Pwm.h
 
 
 8.3.8 Pwm_EnableNotification
 [SWS_Pwm_91004] Definition of API function Pwm_EnableNotification 
Service Name
 Pwm_EnableNotification
 Syntax
 void Pwm_EnableNotification (
 Pwm_ChannelType ChannelNumber,
 Pwm_EdgeNotificationType Notification
 )
 Service ID [hex]
 0x07
 Sync/Async
 Asynchronous
 Reentrancy
 Reentrant for different channel numbers
 Parameters (in)
 ChannelNumber
 Numeric identifier of the PWM
 Notification
 Type of notification PWM_RISING_EDGE or PWM_FALLING_
 EDGEor PWM_BOTH_EDGES
 Parameters (inout)
 None
 Parameters (out)
 None
 Return value
 None
 Description
 Service to enable the PWM signal edge notification according to notification parameter.
 Available via
 Pwm.h
 
   8.3.13 Pwm_GetVersionInfo
 [SWS_Pwm_00103] Definition of API function Pwm_GetVersionInfo 
Service Name
 Pwm_GetVersionInfo
 Syntax
 void Pwm_GetVersionInfo (
 Std_VersionInfoType* versioninfo
 )
 Service ID [hex]
 0x08
 Sync/Async
 Synchronous
 Reentrancy
 Reentrant
 Parameters (in)
 None
 Parameters (inout)
 None
 Parameters (out)
 versioninfo
 Pointer to where to store the version information of this module.
 Return value
 None
 Description
 Service returns the version information of this module.
 Available via
 Pwm.h