# ADC

For now only ADC 1&2 is used. I've tried using ADC1,2,3,4 in dual interleaved mode to double sampling frequency, but for unknown reasons I coudn't managed to get ADC3,4 working.

# DAC 

DAC is triggerred regulary by timer in 170MHz/17 = 5 MHz frequency. 4 sinewave are preconfigured, with varing samples per period.
Available 5000, 400, 200, 80, 40 samples, which corresponds to sinewave of frequency 1 kHz, 12.5 kHz, 25 kHz, 62.5 kHz, and 125 kHz respectively.

# Misc

## CubeMX and Code generation

Since Cube does not support main with .cpp extension LCRmeter.ioc should not be used. 
Instead use dummy project inside 'generator/' folder and copy-paste code.

## folder structure

- generator holds dummy CubeMX project for generating code
- Middlewares holds libraries provided by CubeMX
- tests holds unit tests, testing library, fakes, etc.
- USB_DEVICE folder supplied by CubeMX 
- libs - custom libraries used by this project
- Hardware holds hardware related files
- Core holds main.cpp and essential program files
- Drivers - mainly HAL library


## Pinout

see PCB schematic

## Peripheral assigment

I2C1 -> OLED display

DAC1 -> LCR sinewave generator
DAC2 -> customizable output (not implemented)

DMA1 -> ADC
DMA2 -> DAC1

TIM6 -> trigger for DAC1.

ADC1 & ADC2 - LCR measurments
ADC3 - oscilloscope
ADC5 - Vbat and temperature monitoring


