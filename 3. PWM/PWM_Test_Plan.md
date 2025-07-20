# PWM AUTOSAR API Test Plan - Current Implementation Status

## Overview
**UPDATED**: Current implementation focuses on TIM2 channels (4-7) with specific test cases for idle state control and notifications. PWM restoration after idle state is a known issue under investigation.

## Test Environment
- **Target**: STM32F103C8T6
- **Clock**: 72MHz system clock
- **PWM Frequency**: 1kHz (default)
- **Timer Prescaler**: 72 (1MHz timer clock)  
- **Active Implementation**: TIM2 channels only (PA0-PA3)
- **Test Status**: Partial implementation with known issues

## Current Channel Configuration & Pin Mapping

### **IMPLEMENTED CHANNELS** (Currently Active)
| Channel | Timer | HW Channel | GPIO Pin | Test Function | Current Status |
|---------|-------|------------|----------|---------------|----------------|
| **Channel 4** | TIM2 | CH1 | **PA0** | Basic Operations + Notifications | ⚠️ Hardware Issue |
| **Channel 5** | TIM2 | CH2 | **PA1** | Duty Cycle Variations + Notifications | ✅ Working |
| **Channel 6** | TIM2 | CH3 | **PA2** | Idle State Control + Notifications | 🔄 Issue: PWM Restoration |
| **Channel 7** | TIM2 | CH4 | **PA3** | Idle State Control | 🔄 Issue: PWM Restoration |

### **CONFIGURED BUT NOT TESTED** 
| Channel | Timer | HW Channel | GPIO Pin | Status | Reason |
|---------|-------|------------|----------|---------|---------|
| **Channel 0-3** | TIM1 | CH1-4 | **PA8-PA11** | ⏸️ Not in current tests | Focus on TIM2 debugging |

### **DISABLED CHANNELS**
| Channel | Timer | Status | Reason |
|---------|-------|--------|---------|
| Channel 8-11 | TIM3 | **DISABLED** | Timer not enabled in configuration |
| Channel 12-15 | TIM4 | **DISABLED** | Timer not enabled in configuration |

## **CURRENT TEST IMPLEMENTATION STATUS**

### **Test 1: Basic Operations** ✅ **IMPLEMENTED & WORKING**
**Function**: `PWM_Test_Basic_Operations()`
**Pins Under Test**: **PA0 (Channel 4), PA1 (Channel 5)**
**Test Actions**:
- PA0: Set 25% duty cycle (0x2000) = 250μs HIGH, 750μs LOW
- PA1: Set 75% duty cycle (0x6000) = 750μs HIGH, 250μs LOW
**Expected Results**: Clean PWM signals at 1kHz on both pins
**Current Status**: ✅ Working for PA1, ⚠️ PA0 has hardware/config issues

### **Test 2: Duty Cycle Variations** ✅ **IMPLEMENTED & WORKING**  
**Function**: `PWM_Test_Duty_Cycle_Variations()`
**Pins Under Test**: **PA0 (Channel 4), PA1 (Channel 5)**
**Test Actions**:
- Convert 10% to duty cycle value using `Pwm_PercentageToDutyCycle(10)`
- Apply 10% duty cycle to both channels = 100μs HIGH, 900μs LOW
**Expected Results**: Both pins show identical low duty cycle waveforms
**Current Status**: ✅ Utility function working, PA1 shows correct output

### **Test 3: Idle State Control** 🔄 **IMPLEMENTED BUT NOT WORKING**
**Function**: `PWM_Test_Idle_State_Control()`
**Pins Under Test**: **PA2 (Channel 6), PA3 (Channel 7)**
**Test Actions**:
1. `Pwm_SetOutputToIdle(6)` → PA2 forced to HIGH (idle state)
2. `Pwm_SetOutputToIdle(7)` → PA3 forced to HIGH (idle state)  
3. `Pwm_SetDutyCycle(6, 0x6000)` → Attempt 75% PWM restoration
4. `Pwm_SetDutyCycle(7, 0x6000)` → Attempt 75% PWM restoration
**Expected Results**: Idle forces HIGH, then restore to 75% PWM
**Current Status**: ❌ **MAJOR ISSUE** - PWM restoration not working after idle

### **Test 4: Notifications** ✅ **IMPLEMENTED & CONFIGURED**
**Function**: `PWM_Test_Notifications()`
**Pins Under Test**: **PA0, PA1, PA2** (Channels 4, 5, 6)
**Test Actions**:
- PA0: Enable rising edge notifications → `counter_notification_rising++`
- PA1: Enable falling edge notifications → `counter_notification_failing++`
- PA2: Enable both edges notifications → `counter_notification_bothedge++`
**Expected Results**: Interrupt counters increment with PWM edges
**Current Status**: ✅ Functions implemented, requires PWM signals to test

### **Tests NOT IMPLEMENTED**
- ❌ Output State Reading (`Pwm_GetOutputState()`)
- ❌ Edge Cases (0%, 100% duty cycles)
- ❌ Period and Duty setting (`Pwm_SetPeriodAndDuty()`)
- ❌ Multi-channel simultaneous testing
- ❌ TIM1 channels testing (PA8-PA11)

## **CRITICAL ISSUES IDENTIFIED**

### 🔴 **Issue #1: PWM Restoration After Idle State**
**Problem**: `Pwm_SetDutyCycle()` does not restore PWM operation after `Pwm_SetOutputToIdle()`
**Affected Pins**: PA2 (Channel 6), PA3 (Channel 7)
**Root Cause**: TIM_OCInit() restoration logic incomplete or incorrect
**Current Status**: Under investigation - multiple fix attempts failed
**Impact**: HIGH - Core functionality not working

### ⚠️ **Issue #2: PA0 (TIM2_CH1) Hardware Configuration**  
**Problem**: PA0 may not generate PWM signals correctly
**Affected Pins**: PA0 (Channel 4)
**Root Cause**: Unknown - could be GPIO config, timer config, or hardware issue
**Current Status**: Needs hardware verification with oscilloscope
**Impact**: MEDIUM - One channel affected

### 🔴 **Issue #3: Missing PWM Edge Enumeration Constants**
**Problem**: `PWM_RISING_EDGE`, `PWM_FALLING_EDGE`, `PWM_BOTH_EDGES` undefined
**Affected Functions**: `PWM_Test_Notifications()`
**Root Cause**: Missing include or incorrect enum definitions
**Current Status**: Compilation errors prevent testing
**Impact**: HIGH - Prevents notification testing

## **MAIN.C EXECUTION FLOW**

```c
int main()
{
    SystemClock_Config();     // 72MHz system clock setup
    NVIC_Config();           // Timer interrupts (currently DISABLED)
    Port_Init(&PortCfg_Port); // GPIO configuration for PWM pins
    Pwm_Init(&Pwm_Config);   // PWM driver initialization
    
    // CURRENT ACTIVE TEST: Notifications only
    PWM_Test_Notifications(); // Enable notifications (compilation issues)
    
    while(1) {
        // Infinite loop - no continuous testing
    }
}
```

**Active Test Functions**:
- ✅ `PWM_Test_Basic_Operations()` - Ready to run
- ✅ `PWM_Test_Duty_Cycle_Variations()` - Ready to run  
- 🔄 `PWM_Test_Idle_State_Control()` - Has restoration issue
- ❌ `PWM_Test_Notifications()` - Compilation errors

## **HARDWARE TEST SETUP**

### **Current Pin Connections Needed**
```
STM32F103C8T6 Test Setup:
┌─────────────────────────────────────────┐
│ PA0 (Channel 4) ── Oscilloscope Ch1    │ ← Test basic PWM output
│ PA1 (Channel 5) ── Oscilloscope Ch2    │ ← Test duty cycle variations  
│ PA2 (Channel 6) ── Oscilloscope Ch3    │ ← Test idle state control
│ PA3 (Channel 7) ── Oscilloscope Ch4    │ ← Test idle state control
│ GND ──────────── Oscilloscope Ground   │
└─────────────────────────────────────────┘
```

### **Test Procedure**
1. **Connect oscilloscope** to PA0-PA3 pins
2. **Flash firmware** with current test implementation  
3. **Run basic operations**: Call `PWM_Test_Basic_Operations()`
4. **Verify PA1 output**: Should show 75% duty cycle at 1kHz
5. **Check PA0 output**: May show issues - document findings
6. **Test idle state**: Call `PWM_Test_Idle_State_Control()`
7. **Verify idle behavior**: PA2/PA3 should go HIGH initially
8. **Check PWM restoration**: PA2/PA3 should return to PWM (currently fails)

## **NEXT DEVELOPMENT PRIORITIES**

### **Priority 1: Fix PWM Restoration** 🔴
- Debug `PwmHw_SetDutyCycle()` restoration logic
- Verify TIM_OCInit() configuration
- Test register-level timer state after idle
- Ensure proper timer enable state management

### **Priority 2: Fix PA0 Issues** ⚠️
- Hardware verification with oscilloscope
- Check GPIO configuration for PA0
- Verify TIM2_CH1 specific settings
- Compare working PA1 vs non-working PA0 config

### **Priority 3: Complete Notification Testing** 🔶
- Fix missing PWM edge enumeration constants
- Enable TIM2 interrupts in NVIC  
- Test interrupt generation and counting
- Verify notification function calls

### **Priority 4: Expand Test Coverage** 🔵
- Implement TIM1 channels testing (PA8-PA11)
- Add output state reading tests
- Add boundary condition testing  
- Add multi-channel simultaneous testing
