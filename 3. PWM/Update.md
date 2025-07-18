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