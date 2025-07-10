# PWM Driver - AUTOSAR Compliant Implementation

## Overview

This is a complete AUTOSAR-compliant PWM (Pulse Width Modulation) driver implementation for STM32F103C8T6 microcontroller using the Standard Peripheral Library (SPL). The driver follows AUTOSAR 4.4.0 specifications and provides a full hardware abstraction layer for PWM functionality.

## Directory Structure

```
PWM_Driver/
├── BSW/
│   └── MCAL/
│       └── PWM/
│           ├── Inc/                    # Header files
│           │   ├── Pwm.h              # Main PWM driver API
│           │   ├── Pwm_Types.h        # All type definitions
│           │   ├── Pwm_Cfg.h          # Configuration constants
│           │   ├── Pwm_Hw.h           # Hardware abstraction API
│           │   ├── Det.h              # Development Error Tracer stub
│           │   └── SchM_Pwm.h         # Schedule Manager stub
│           ├── Src/                    # Source files
│           │   ├── Pwm.c              # Main PWM driver implementation
│           │   ├── Pwm_Cfg.c          # Configuration data
│           │   └── Pwm_Hw.c           # Hardware abstraction implementation
│           └── Examples/               # Example usage
│               └── Pwm_Example.c      # Usage examples
```

## File Descriptions

### Header Files

- **Pwm.h**: Main PWM driver header containing all AUTOSAR API function prototypes
- **Pwm_Types.h**: Contains all typedef definitions, enums, and structures
- **Pwm_Cfg.h**: Configuration constants and macros
- **Pwm_Hw.h**: Hardware abstraction layer with STM32 SPL-based APIs
- **Det.h**: Development Error Tracer stub for error reporting
- **SchM_Pwm.h**: Schedule Manager stub for critical sections

### Source Files

- **Pwm.c**: Main PWM driver implementation with AUTOSAR APIs
- **Pwm_Cfg.c**: Configuration data and validation functions
- **Pwm_Hw.c**: Hardware abstraction layer implementation using STM32 SPL

### Examples

- **Pwm_Example.c**: Complete usage examples for all PWM driver features

## Features

### AUTOSAR Compliance
- Full AUTOSAR 4.4.0 specification compliance
- All required API functions implemented
- Development error detection support
- Version information API
- Proper error handling and reporting

### Hardware Support
- STM32F103C8T6 microcontroller support
- Timer 1, 2, 3, 4 hardware abstraction
- Up to 8 PWM channels (configurable)
- Variable and fixed period channel support
- Hardware-based PWM generation

### Key Features
- **Channel Management**: Up to 8 PWM channels with individual configuration
- **Period Control**: Variable and fixed period support
- **Duty Cycle Control**: 16-bit duty cycle resolution (0x0000 to 0x8000)
- **Output Control**: Configurable polarity and idle states
- **Notification Support**: Edge-based interrupt notifications
- **Phase Shift**: Phase shift support for advanced applications
- **Safety Features**: Development error detection and validation

## API Functions

### Initialization Functions
```c
void Pwm_Init(const Pwm_ConfigType* ConfigPtr);
void Pwm_DeInit(void);
```

### Channel Control Functions
```c
void Pwm_SetDutyCycle(Pwm_ChannelType ChannelId, uint16 DutyCycle);
void Pwm_SetPeriodAndDuty(Pwm_ChannelType ChannelId, Pwm_PeriodType Period, uint16 DutyCycle);
void Pwm_SetOutputToIdle(Pwm_ChannelType ChannelId);
Pwm_OutputStateType Pwm_GetOutputState(Pwm_ChannelType ChannelId);
```

### Notification Functions
```c
void Pwm_EnableNotification(Pwm_ChannelType ChannelId, Pwm_EdgeNotificationType Notification);
void Pwm_DisableNotification(Pwm_ChannelType ChannelId);
```

### Version Information
```c
void Pwm_GetVersionInfo(Std_VersionInfoType* VersionInfo);
```

## Configuration

### Hardware Configuration
The driver supports the following timers:
- **Timer 1**: 4 channels (PA8, PA9, PA10, PA11)
- **Timer 2**: 4 channels (PA0, PA1, PA2, PA3)
- **Timer 3**: 4 channels (PA6, PA7, PB0, PB1)
- **Timer 4**: 4 channels (PB6, PB7, PB8, PB9)

### Channel Configuration
Each channel can be configured for:
- **Channel Class**: Variable period, fixed period, or fixed period shifted
- **Default Period**: Initial period value
- **Default Duty Cycle**: Initial duty cycle value
- **Polarity**: High or low active
- **Idle State**: Output state when idle
- **Notification**: Edge-based notifications

### System Configuration
- **System Frequency**: 72 MHz (STM32F103C8T6 maximum)
- **Timer Prescaler**: Configurable per timer
- **Period Range**: 1 to 65535 timer ticks
- **Duty Cycle Range**: 0x0000 (0%) to 0x8000 (100%)

## Usage Examples

### Basic PWM Control
```c
#include "Pwm.h"
#include "Pwm_Cfg.h"

int main(void)
{
    // Initialize PWM driver
    Pwm_Init(&Pwm_Config);
    
    // Set Channel 0 to 50% duty cycle
    Pwm_SetDutyCycle(PWM_CHANNEL_0, 0x4000);
    
    // Set Channel 1 to 25% duty cycle  
    Pwm_SetDutyCycle(PWM_CHANNEL_1, 0x2000);
    
    while(1)
    {
        // Main application loop
    }
    
    return 0;
}
```

### Servo Motor Control
```c
// Control servo motor (50Hz, 1-2ms pulse width)
void ServoControl(void)
{
    // Set servo to 0° position (1ms pulse)
    Pwm_SetPeriodAndDuty(PWM_CHANNEL_0, 20000, 0x0CCD);
    
    // Set servo to 90° position (1.5ms pulse)
    Pwm_SetPeriodAndDuty(PWM_CHANNEL_0, 20000, 0x1333);
    
    // Set servo to 180° position (2ms pulse)
    Pwm_SetPeriodAndDuty(PWM_CHANNEL_0, 20000, 0x1999);
}
```

### LED Brightness Control
```c
// Control LED brightness
void LedBrightness(void)
{
    uint16 brightness;
    
    // Gradual brightness increase
    for (brightness = 0; brightness <= 0x8000; brightness += 0x0800)
    {
        Pwm_SetDutyCycle(PWM_CHANNEL_0, brightness);
        // Add delay for visible effect
    }
}
```

### Notification Usage
```c
// PWM notification function
void Pwm_NotificationChannel0(void)
{
    // Handle PWM edge notification
    // Toggle LED, increment counter, etc.
}

void SetupNotifications(void)
{
    // Enable rising edge notification
    Pwm_EnableNotification(PWM_CHANNEL_0, PWM_RISING_EDGE);
    
    // Enable both edge notification
    Pwm_EnableNotification(PWM_CHANNEL_1, PWM_BOTH_EDGES);
}
```

## Hardware Abstraction Layer

The hardware abstraction layer provides STM32 SPL-based functions for:

### Timer Management
```c
Std_ReturnType Pwm_Hw_InitHwUnit(Pwm_HwUnitType HwUnit, const Pwm_HwUnitConfigType* ConfigPtr);
Std_ReturnType Pwm_Hw_DeInitHwUnit(Pwm_HwUnitType HwUnit);
```

### Channel Control
```c
Std_ReturnType Pwm_Hw_InitChannel(Pwm_ChannelType ChannelId, const Pwm_ChannelConfigType* ChannelConfig);
Std_ReturnType Pwm_Hw_SetDutyCycle(Pwm_ChannelType ChannelId, uint16 DutyCycle);
Std_ReturnType Pwm_Hw_SetPeriodAndDuty(Pwm_ChannelType ChannelId, Pwm_PeriodType Period, uint16 DutyCycle);
```

### Output Control
```c
Std_ReturnType Pwm_Hw_SetOutputToIdle(Pwm_ChannelType ChannelId);
Pwm_OutputStateType Pwm_Hw_GetOutputState(Pwm_ChannelType ChannelId);
```

### Notification Control
```c
Std_ReturnType Pwm_Hw_EnableNotification(Pwm_ChannelType ChannelId, Pwm_EdgeNotificationType Notification);
Std_ReturnType Pwm_Hw_DisableNotification(Pwm_ChannelType ChannelId);
```

## Type Definitions

### Basic Types
- `Pwm_ChannelType`: Channel identifier (uint8)
- `Pwm_PeriodType`: Period value (uint16)
- `Pwm_HwUnitType`: Hardware unit identifier (uint8)
- `Pwm_StateType`: Driver state (enum)

### Enumeration Types
- `Pwm_OutputStateType`: PWM_HIGH, PWM_LOW
- `Pwm_EdgeNotificationType`: PWM_RISING_EDGE, PWM_FALLING_EDGE, PWM_BOTH_EDGES
- `Pwm_ChannelClassType`: PWM_VARIABLE_PERIOD, PWM_FIXED_PERIOD, PWM_FIXED_PERIOD_SHIFTED

### Structure Types
- `Pwm_ConfigType`: Main configuration structure
- `Pwm_ChannelConfigType`: Channel configuration structure
- `Pwm_HwUnitConfigType`: Hardware unit configuration structure
- `Pwm_ChannelRuntimeType`: Channel runtime data structure
- `Pwm_HwUnitRuntimeType`: Hardware unit runtime data structure

## Error Handling

The driver includes comprehensive error detection:

### Development Errors
- `PWM_E_PARAM_CONFIG`: Invalid configuration parameter
- `PWM_E_UNINIT`: API called without initialization
- `PWM_E_PARAM_CHANNEL`: Invalid channel identifier
- `PWM_E_PERIOD_UNCHANGEABLE`: Period change on fixed period channel
- `PWM_E_ALREADY_INITIALIZED`: Driver already initialized
- `PWM_E_PARAM_POINTER`: Invalid pointer parameter

### Runtime Errors
- `PWM_E_DUTY_CYCLE_OUT_OF_RANGE`: Duty cycle value out of range
- `PWM_E_PERIOD_OUT_OF_RANGE`: Period value out of range
- `PWM_E_TIMEOUT`: Operation timeout
- `PWM_E_HW_FAILURE`: Hardware failure

## Configuration Options

### Development Configuration
- `PWM_DEV_ERROR_DETECT`: Enable/disable development error detection
- `PWM_VERSION_INFO_API`: Enable/disable version info API
- `PWM_NOTIFICATION_SUPPORTED`: Enable/disable notification support

### Feature Configuration
- `PWM_SET_PERIOD_AND_DUTY_API`: Enable/disable period and duty API
- `PWM_SET_OUTPUT_TO_IDLE_API`: Enable/disable output to idle API
- `PWM_GET_OUTPUT_STATE_API`: Enable/disable get output state API
- `PWM_ENABLE_PHASE_SHIFT`: Enable/disable phase shift support
- `PWM_ENABLE_VARIABLE_PERIOD`: Enable/disable variable period support

### Hardware Configuration
- `PWM_TIM1_ENABLED`: Enable/disable Timer 1
- `PWM_TIM2_ENABLED`: Enable/disable Timer 2
- `PWM_TIM3_ENABLED`: Enable/disable Timer 3
- `PWM_TIM4_ENABLED`: Enable/disable Timer 4

## Validation

The driver includes comprehensive validation:
- Configuration parameter validation
- Channel ID validation
- Duty cycle range validation
- Period range validation
- Pointer validation
- Hardware unit validation

## Dependencies

### Required Headers
- `Std_Types.h`: Standard AUTOSAR types
- `stm32f10x.h`: STM32F10x device header
- `stm32f10x_tim.h`: STM32F10x timer library
- `stm32f10x_rcc.h`: STM32F10x RCC library
- `stm32f10x_gpio.h`: STM32F10x GPIO library

### Optional Dependencies
- `Det.h`: Development Error Tracer (stubbed)
- `SchM_Pwm.h`: Schedule Manager (stubbed)

## Future Enhancements

Potential future enhancements include:
- Dead time support for complementary outputs
- Fault detection and protection
- Advanced synchronization features
- Power management integration
- Multi-core support
- Real-time constraints validation

## Version Information

- **Version**: 1.0.0
- **AUTOSAR Version**: 4.4.0
- **Target**: STM32F103C8T6
- **Compiler**: ARM GCC
- **Date**: 2025

## License

This PWM driver implementation is provided as an educational example and follows AUTOSAR specifications. Use at your own risk in production applications.
