flow of id
first when sw call
none in queue id = invalid
start id = groupid
done conversion 
#if queue enable
id = next group 
#else 
id = invalid
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
- fix add check is buffer is setup
only convert after called from api
or setup [ ]
- reset dma when not used
clean flag when remove queue or stop sw [ ]

- group state change to completed when using dma after 1 conversion in streaming mode [ ]
- add macro check api enabled [ ]

- main function now only allow if queue is available [x] 
- only main function can call next sw conversion [x]
- recall sw function after hw conversion raise [x]






FIX comming soon
big upgrade to 3.0
group when finished has to be read group before go to next conversion or start again
circular streaming will wait at adc_completed
then fill the buffer of their own to the result buffer and 
go to state stream_completed 
when in state adc_stream_completed -> call read group wwill return state busy and start again

linear streaming when finished and call read group will return to idle state 

the read group issue [x]
the next conversion [ ]


