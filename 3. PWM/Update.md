// 16/7/2025
- Update all IDs
- Remove PWM_E_PARAM_CONFIG and replace by PWM_E_INIT_FAILED (correct to AUTOSAR)
- Change old Pwm_ValidateInit -> Pwm_ValidateCfgPtr check if config pointer is valid
- Fix Pwm_ValidateInit -> check if pwm init is called by check Pwm_DriverState
- Update Autosar link check error 
    - [SWS_Pwm_00047] :  Check if the channel number is valid
    - [SWS_Pwm_00117] : Check if function init is called
- Change ChannelId -> ChannelNumber
- Function updated
    - Pwm_EnableNotification
- Remove parameter 
TIM_Instace in hwunit
- Add callback
- Add edge notification 
- Remove Runtime data 
- Change read output 
- Add test for testing

// 18/7/2025

- Fixed Interrupt Notification issue
    - **Problem**: When disabling notification for one channel, it accidentally turned off notifications for other channels in the same PWM group
    - **Solution**: Created a flag system to track notification status of each channel
        - Only turn off group interrupts when all notifications are off
        - Only turn on group interrupts when at least one notification is on
    - **Note**: The hardware has 4 PWM units with 16 channels total. We use 8-bit arrays to track status, which makes it easy to expand later if needed
        - Pwm_UpdateInterruptUsers for storing flags
        -How to get mask of TIM_CHANNEL_x = x<<2 so we will use this mechanisim to get bit position for status array
        (TIM_CHANNEL >> 2) get the bit position x then 1 << x to enable corresponding status

// 20/7/2025
- Update SetOutputToIdle
 - Solution Using Force output compare function 
 ```c
 case TIM_Channel_2:
            if (Pwm_ChannelConfig[ChannelId].IdleState == PWM_HIGH) {
                TIM_ForcedOC2Config(TIM_Instance, TIM_ForcedAction_Active);    // Force HIGH
            } else {
                TIM_ForcedOC2Config(TIM_Instance, TIM_ForcedAction_InActive);  // Force LOW
            }
 ```
- Add parameter IdleStateSet for checking if idle state is set
    - **Note** if set can call PWM_SetDutyCycle will work with the last duty cycle   
- Remove unused parameter in channel config
- Update test


- Can't do 
 [SWS_Pwm_10051]
 Upstream requirements: SRS_BSW_00323, SRS_BSW_00386
 If development error detection for the Pwm module is enabled, and a development
 error occurs, then the corresponding PWM function shall report the error to the Default
 Error Tracer.
 [SWS_Pwm_20051]
 Upstream requirements: SRS_BSW_00323, SRS_BSW_00386
 If development error detection for the Pwm module is enabled, and a development
 error occurs, then the corresponding PWM function shall skip the desired functionality
 in order to avoid any corruptions of data or hardware registers leaving the function
 without any actions.