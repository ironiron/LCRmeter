# todo

[x] add connector for UART converter for debugging
[x] add testpoint for ADC measurments
[x] make oled from discrete parts, not module
[x] change connector for programmmer, and move it out of the top layer
- check if PCB can be smaller (especially batterry holder)
[x] add more Rseries options
[x] remove option without Rseries connected at all - it can fry PCB
[x] remove connection to 2nd ADCs
[x] Are there  better opamps?? 
[x] remove option for 1.5 V ref
[x] reduce number of caps, and increase its capacitance
[x] think about Rseries diagnosis
[x] make programming interface coherent with stlink or make something with proper connnection
- think about connections to from MCU 
[x] is there MCU with bigger RAM? (no there is none in g4 series)
[] add pulldown for the reset of oled
[] add mounting points for mechanical assembly
- double check connections on schematic


# Decisions background

74LVC1G66 Switches needs to be placed before Rseries so that ADC measurments do no take into account resistance of switches itself.

PCB should be 4 layer to improve EMC. Inner layers should be GND only, VCC is not so critical that needs a power plane.

Capacitors should be 0603 or lower

R series will not have any self diagnosis - too much hassle for little added accuracy

Filters are designed using Analog Devices tool  (https://tools.analog.com/en/filterwizard/)[https://tools.analog.com/en/filterwizard/]
![[media/filters.png]]
Requirments for filter is that 125KHz is a passband with optimally 0 dB ripples and no gain. Alternatively a simple RC filter can be used. Because ADC takes is too invasive for the signal (sampling period is cleary visible on osciloscope), at least internal op-amp is required.


Capacitors for uC are set to larger for same size, in the hope of improving noise performance. There is no simulation done for either option (many little plus one big, vs many bigger), so I do not know if it works, but there is no indication that either is better than other. ST's guidelines seems to be so called tradition of caps of 3 diffferent values.

Copper pour on top and bottom is not allowed, because it can create easi to overlook noisy cavities.

ADCs are on pins that can be mapped thru internal opamp