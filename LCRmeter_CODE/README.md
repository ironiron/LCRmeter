
I2C2 -> OLED display

DAC1 -> PA4 -> measuring

DMA1 -> ADC
DMA2 -> DAC1 //todo add double buffer mode to improve bus latency.

Up to 2.2 V DAC is working corectlly with current implementation 142 Khz average (TIM6 pres is 15)

#TODO 

- change ADC_freq to 60Mhz tochieve max sampling time (200ns)
- check disabled buffer for DAC

#ADC

For now let's use only ADC 1&2. if bandwidth is not wide enough then let's use ADC 3,4 as well. 


## CubeMX and Code generation

Since Cube does not support main with .cpp extension LCRmeter.ioc should not be used. 
Instead use dummy project inside 'generator/' folder and copy-paste code.

### folder structure

- generator holds dummy CubeMX project for generating code
- Middlewares holds libraries provided by CubeMX
- tests holds unit tests, testing library, fakes, etc.
- USB_DEVICE folder supplied by CubeMX 
- libs - custom libraries used by this project
- Hardware holds hardware related files
- Core holds main.cpp and essential program files
- Drivers - mainly HAL library
