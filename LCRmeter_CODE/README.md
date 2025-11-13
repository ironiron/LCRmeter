
I2C1 -> OLED display

DAC1 -> LCR sinewave generator
DAC2 -> customizable output 

DMA1 -> ADC
DMA2 -> DAC1

TIM6 -> trigger for DAC1.

ADC1 & ADC2 - LCR measurments
ADC3 - oscilloscope
ADC5 - Vbat and temperature monitoring
 

# what is working

The resistive load when connected to 0V works fine there's +- 3 ohm variance on result, however the alpha resolution is to small.
It has 16 degrees which is far to small. with dual ADC it can be moved to 8 degs. but it still not good. I guess frequency must be reduced (now it's ca. 121 KHz)
Up to 2.2 V DAC is working corectlly with current implementation 142 Khz average (TIM6 prescaler is 15)

However sometimes ESR of capacitor is negative. Not sure why.

#TODO 

- change ADC_freq to 60Mhz tochieve max sampling time (200ns)
- check disabled buffer for DAC
- consider adding under sampling on ADC to increase range of frequency for sinewave

#ADC

For now only ADC 1&2 is used. I've tried using ADC1,2,3,4 in dual interleaved mode to double sampling frequency, but for unknown reasons I coudn't managed to get ADC3,4 working.

#DAC1

DAC is triggerred regulary by timer in 170MHz/17 = 10 MHz frequency. 4 sinewave are preconfigured, with varing samples per period.
Available 400, 200, 80, 40 samples, which corresponds to sinewave of frequency 12.5 kHz, 25 kHz, 62.5 kHz, and 125 kHz respectively.


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


# Pinout

see PCB schematic