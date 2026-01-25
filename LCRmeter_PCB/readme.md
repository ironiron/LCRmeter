# todo

- add connector for UART converter for debugging
- add testpoint for ADC measurments
- make oled from discrete parts, not module
- change connector for programmmer, and move it out of the top layer
- check if PCB can be smaller (especially batterry holder)
- add more Rseries options
- remove option without Rseries connected at all - it can fry PCB
- remove connection to 2nd ADCs
- Are there  better opamps??
- remove option for 1.5 V ref
- reduce number of caps, and increase its capacitance
- think about Rseries diagnosis
- make programming interface coherent with stlink or make something with proper connnection
- think about connections to from MCU
- is there MCU with bigger RAM?

# Decision documentation

74LVC1G66 Switches needs to be placed before Rseries so that ADC measurments do no take into account resistance of switches itself.

PCB should be 4 layer to improve EMC. inner layers should be GND only, VCC is not so critical that needs a power plane.

Capacitors should be 0603 or lower


