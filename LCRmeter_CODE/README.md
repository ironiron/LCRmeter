Currently MCP47FEB is used on PCB, however part of development process was done with MCP4725, so the libs stayed in filesystem for potential future use.

I2C1 -> DAC
I2C2 -> OLED display

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
