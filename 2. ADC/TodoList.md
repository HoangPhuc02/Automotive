# test
## streaming
multi channel circular continuous [x]
multi channel cir linear		[x]
multi channel cir continuous single  []

# not test 
oneshot circular
oneshot linear

# todo
- dma circular not set state to ADC_Completed after complete conversion [ ]
- fix add check is buffer is reset
only reset when called from api
or reset [ ]
- reset dma when not used
clean flag when remove queue or stop sw

- group state change to completed when using dma after 1 conversion in streaming mode [ ]


