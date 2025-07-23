# AUTOSAR Fan Control System - Update v.1.0

**Date**: July 24, 2025  
**Target**: STM32F103C8T6 Blue Pill  
**Architecture**: AUTOSAR Classic Platform
**Author**: hoangphuc540202@gmail.com  
**Repo**: https://github.com/HoangPhuc02/AUTOSAR-Classic-ECU-Development
---
## Hardware Setup
- **Temperature**: PA0 (LM35 sensor)
- **Fan PWM**: PA8 (variable speed control)  
- **Status LED**: PC13 (built-in + external)

## 🐛 Critical Bugs Found

### 1. ADC Calibration Missing ⚠️ **CRITICAL**
**Location**: `Adc_Hw.c` lines 134-137  
**Problem**: ADC stuck in BUSY state, system halts  
**Cause**: Calibration sequence commented out  
**Fix**: Uncomment ADC calibration code

## ⚙️ Current System Status

**Clock**: 72MHz system, 12MHz ADC (✅ within spec)  
**Port Config**: PA0(ADC), PA8(PWM), PC13(DIO) ✅  
**ADC Clock Prescaler**: Fixed to RCC_PCLK2_Div6 ✅  
**LED Hardware Analysis**: Current-sinking vs sourcing behavior identified ✅  
**DIO Channel Mapping**: Fixed PA13→PC13 mapping issue ✅  
**Main Issue**: System stuck in `IoHwAb_ReadTemperature()` due to missing ADC calibration

## 🔧 Quick Fixes Needed

1. **Enable ADC calibration** in `Adc_Hw.c:134-137` (Critical)

## Temperature Conversion
```c
// LM35: 10mV/°C, 3.3V ref, 12-bit ADC
Temperature_C = (ADC_Value * 805) / 1000;
```

---
## 📋 To-Do List

### 🔨 Code Quality
- [ ] **Clean code**: Remove commented code and debug prints
- [ ] **Code formatting**: Apply consistent indentation and naming
- [ ] **Add error handling**: Check return values from MCAL functions
- [ ] **Documentation**: Update function headers and comments

### 🏗️ Architecture Improvements
- [ ] **Update IoHwAb structure**: Better organize hardware abstraction layer
- [ ] **Separate configuration**: Move configs to dedicated files
- [ ] **Layer separation**: Ensure clean AUTOSAR layer boundaries
- [ ] **Module initialization**: Add proper init sequence validation

### ⚡ Performance Enhancements
- [ ] **Add DMA for ADC**: Use DMA1 for continuous temperature reading
- [ ] **Implement ADC interrupts**: Non-blocking temperature conversion
- [ ] **PWM optimization**: Use timer interrupts for smooth fan control
- [ ] **Background processing**: Move sensor reading to interrupt context

### 🧪 Testing & Validation
- [ ] **Unit tests**: Create test cases for each IoHwAb function
- [ ] **Hardware-in-loop**: Test with actual temperature sensor
- [ ] **Edge case handling**: Test extreme temperature values
- [ ] **System integration**: Full end-to-end testing


---

