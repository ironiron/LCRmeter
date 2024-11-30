
I2C2 -> OLED display

DAC1 -> PA4 -> measuring

There seems ho be some HW 

DMA1 -> ADC
DMA2 -> DAC1 //todo add double buffer mode to improve bus latency.

#TODO 

- there seems to be some problem with DAC converter. needs to improve buffer driving capabilties.
as is now - shorting it with inductor, it creates a flat line. (also starting ADC cconversion makes sinewave basicallly a sharped version)

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






85-1,5v on dac
127-2v on dac
182 - 1.88v