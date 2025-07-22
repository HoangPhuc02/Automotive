# ADC Development Status

## 📊 ID Flow Logic
**Initial SW call:**
- No queue → `id = invalid`
- Start → `id = groupid`

**After conversion:**
```
if (queue_enabled) {
    id = next_group;
} else {
    id = invalid;
}
```

## ✅ Testing Status
**Last Updated:** July 22, 2025

### 🔄 Streaming (Tested)
- [x] Multi-channel circular continuous
- [x] Multi-channel circular linear  
- [ ] Multi-channel circular continuous single

### ⏳ Not Yet Tested
- [ ] Oneshot circular
- [ ] Oneshot linear

### 🧪 Interrupt Mode Testing (Version 3.0)
**All tests passed on July 22, 2025:**
- [x] OneShot + Single mode
- [x] OneShot + Streaming Linear mode  
- [x] Continuous + Single mode
- [x] Continuous + Streaming Linear mode
- [x] Continuous + Streaming Circular mode

## 🚧 Current Issues & Tasks

### Critical Fixes Needed
- [ ] DMA circular mode doesn't set state to `ADC_Completed` after conversion
- [ ] Add buffer setup validation - only convert after API call or proper setup
- [ ] Reset DMA when unused - clean flags on queue removal or SW stop
- [ ] Group state changes to completed after 1 conversion in streaming mode (DMA)
- [ ] Add macro to check if API is enabled

### Completed Tasks (Version 3.0 - July 22, 2025)
- [x] Main function only allows execution when queue is available
- [x] Only main function can trigger next SW conversion
- [x] SW function recall after HW conversion interrupt
- [x] **Fixed OneShot streaming counter issue** - No longer exceeds 1 count
- [x] **Fixed auto-notification bug** - Automatic enable error resolved
- [x] **Fixed interrupt channel configuration** - Next channel read properly configured
- [x] **Fixed SW conversion control** - Can now properly disable software conversion
- [x] **Fixed channel switching** - ADC can now switch to next channel correctly

### System Components
- **Group ID management**
- **Group sequence handling** 
- **Group next logic**

### Cleanup Tasks
- [ ] Remove runtime data structures 

# MAJOR UPGRADE - Version 3.0

## 🔄 Group Processing Changes
When a group completes conversion, **must call read group function** before:
- Moving to next conversion 
- Restarting conversion cycle

The read group function will update data and set next state.

## 🔁 Circular Streaming Improvements
**Current State Flow:**
- After conversion completion → `adc_completed` state
- **Old approach:** `size = number_of_samples * number_of_channels`
- **New approach:** Read complete channel set → `adc_completed`

**Optimization Tasks:**
- [ ] Fix DMA init function
- [ ] Fix interrupt handler
- [ ] Convert interrupt-callable parameters to `volatile`
- [ ] Handle runtime group conversions

**State Machine:**
1. Copy data from private buffer to result buffer
2. Change state to `stream_completed`
3. In `adc_stream_completed` state:
   - `read_group()` call → returns `busy` status
   - Automatically starts new conversion cycle

## 📈 Linear Streaming Improvements
**When full conversion cycle completes:**
- Call `read_group()` function
- System automatically returns to `idle` state
- No automatic new conversion cycle start

## 📖 Group Reading Issues
- [x] Fixed group reading errors
- [x] Optimized buffer data reading performance  
- [x] Improved error handling for group reading
- [ ] Handle the next conversion



