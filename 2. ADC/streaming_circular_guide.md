/****************************************************************************************
*                    STREAMING CIRCULAR MODE - IMPLEMENTATION GUIDE                    *
****************************************************************************************
* This document explains the streaming circular mode implementation that prevents
* advancement to the next group in the queue
****************************************************************************************/

## 🎯 **STREAMING CIRCULAR MODE BEHAVIOR**

### **Key Requirement:**
> The ADC must **continuously sample the same group** in a **circular buffer** and **never advance to the next group** in the queue.

---

## 📋 **IMPLEMENTATION DETAILS**

### **1. ISR Channel Sequencing Logic:**

```c
// ✅ CORRECT: Streaming Circular Mode
if (AdcGroup->Adc_GroupAccessMode == ADC_ACCESS_MODE_STREAMING &&
    AdcGroup->Adc_StreamBufferMode == ADC_STREAM_BUFFER_CIRCULAR) {
    
    if (AdcGroup->Adc_SamplesResultCounter >= AdcGroup->Adc_StreamNumSamples) {
        /* Buffer full - wrap around to beginning (circular) */
        AdcGroup->Adc_SamplesResultCounter = 0;  // ⭐ WRAP AROUND
        groupCompleted = 1;  // Notify but keep running
    }
    
    /* ⭐ ALWAYS continue with next sample */
    configure_next_channel();
    start_conversion();
    // ❌ NEVER stops, ❌ NEVER goes to next group
}
```

### **2. Deferred Processing Logic:**

```c
// ✅ STREAMING CIRCULAR: Never advance to next group
if (AdcGroup->Adc_GroupAccessMode == ADC_ACCESS_MODE_STREAMING &&
    AdcGroup->Adc_StreamBufferMode == ADC_STREAM_BUFFER_CIRCULAR) {
    
    NextConversion = 0;  // ⭐ NEVER advance to next group
    AdcGroup->Adc_Status = ADC_COMPLETED;
    
    // Call notification for buffer wrap but keep running
    AdcHw_CallNotification(CurrGroup);
    
    // ❌ Do NOT call AdcHw_HandleNextConversion()
}
```

---

## 🔄 **OPERATION FLOW**

### **Continuous Sampling Sequence:**
```
Start Group 0
    ↓
Sample 0: Ch0 → Ch1 → Ch2
    ↓
Sample 1: Ch0 → Ch1 → Ch2
    ↓
...
    ↓
Sample 99: Ch0 → Ch1 → Ch2
    ↓
Buffer Full → Notification → Wrap to Sample 0
    ↓
Sample 0: Ch0 → Ch1 → Ch2 (overwrites old data)
    ↓
...continues forever on Group 0...
    ↓
❌ NEVER advances to Group 1, 2, etc.
```

---

## 📊 **COMPARISON WITH OTHER MODES**

| Mode | Buffer Behavior | Group Advancement | Use Case |
|------|----------------|-------------------|----------|
| **Streaming Circular** | ♻️ Wraps around | ❌ Never | Continuous monitoring |
| **Streaming Linear** | 🛑 Stops when full | ✅ Yes | Burst capture |
| **Single Access** | 📦 One sample only | ✅ Yes | Event-driven |

---

## 🎛️ **CONFIGURATION EXAMPLE**

```c
/* Configure ADC Group for Streaming Circular */
adcGroup->Adc_GroupAccessMode = ADC_ACCESS_MODE_STREAMING;
adcGroup->Adc_StreamBufferMode = ADC_STREAM_BUFFER_CIRCULAR;  // ⭐ KEY
adcGroup->Adc_GroupConvMode = ADC_CONV_MODE_CONTINUOUS;

/* Buffer setup */
adcGroup->Adc_NbrOfChannel = 3;           // 3 channels per sample
adcGroup->Adc_StreamNumSamples = 100;     // 100 samples in buffer
adcGroup->Adc_ValueResultPtr = buffer;    // Circular buffer pointer

/* Result: 300 total elements (3 × 100) with wrap-around */
```

---

## ⚡ **PERFORMANCE CHARACTERISTICS**

### **Timing:**
- **Channel-to-channel gap:** ~1-2µs (ISR context)
- **Sample-to-sample gap:** ~1-2µs (ISR context)  
- **Buffer wrap notification:** ~ms (main context)
- **Group switching:** Never happens

### **Memory Usage:**
```c
Buffer Size = Adc_NbrOfChannel × Adc_StreamNumSamples × sizeof(uint16)
Example: 3 channels × 100 samples × 2 bytes = 600 bytes
```

### **CPU Usage:**
- **ISR load:** Minimal (~1-2µs per interrupt)
- **Main function load:** Only on buffer wrap events
- **No queue management overhead:** Group never changes

---

## 🚨 **IMPORTANT CONSIDERATIONS**

### **1. Queue Management:**
```c
// ❌ Other groups in queue will NEVER be processed
Queue: [Group0, Group1, Group2]
       ↑
Current (stays here forever)

// ✅ If you need other groups, use separate ADC units
ADC1: Group0 (Streaming Circular) 
ADC2: Group1, Group2 (Normal queue processing)
```

### **2. Data Overwrite:**
```c
// ⚠️ Old data gets overwritten when buffer wraps
Sample 0: [1000, 2000, 3000] ← Gets overwritten after 100 samples
Sample 1: [1001, 2001, 3001]
...
Sample 99: [1099, 2099, 3099]
Sample 0: [1100, 2100, 3100] ← New data overwrites old Sample 0
```

### **3. Stopping the Mode:**
```c
// To stop streaming circular mode:
Adc_StopGroupConversion(ADC_GROUP_0);

// To restart:
Adc_StartGroupConversion(ADC_GROUP_0);
```

---

## 🎯 **USE CASES**

### **✅ Perfect for:**
- **Continuous sensor monitoring** (temperature, voltage, current)
- **Real-time signal processing** (audio, vibration)
- **Circular data logging** with fixed memory
- **Streaming applications** with overwrite tolerance

### **❌ Not suitable for:**
- **Event-driven sampling** (use single access)
- **Multi-group sequential processing** (use linear mode)
- **Critical data preservation** (use linear with larger buffer)

---

## 🔧 **DEBUGGING TIPS**

```c
/* Monitor current buffer position */
uint32 currentPos = AdcGroup->Adc_SamplesResultCounter;
uint32 currentCh = AdcGroup->Adc_ChannelConversionId;

/* Check if buffer wrapped recently */
static uint32 lastWrapCount = 0;
if (bufferWrapCount > lastWrapCount) {
    // Buffer wrapped - process data
    lastWrapCount = bufferWrapCount;
}

/* Verify continuous operation */
assert(AdcGroup->Adc_Status == ADC_COMPLETED);  // Should always be running
```

This implementation ensures your streaming circular mode **never advances to the next group** and provides **continuous, uninterrupted sampling** with **optimal performance**! 🚀
