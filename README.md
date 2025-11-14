# About

My try at LCR measurment via sinewave and voltage measurment. Generally, it is working, however PCB layout is not great, and ADC is not used optymally, so measuring inductance is more or less futile.
Results are a bit floaty. There is an unpolished oscilloscope mode.

Seems like PCB needs a bit of rerouting.


# Measuring principle. 

See \ref misc folder for more in depth walkthrough. The principle is to measure voltage across known resistor and a DUT.
Since it is sinewave that is used for measurments we can get phase angle, loss angle, etc. 

Now measurment settings (frequency and series resistor), are done manually, but maybe implement automatic one in the future?
eg. if loss angle is small then the capacitance can be calculated with greater accuracy than the ESR. If inductor is to be measured then the measuring frequency should be higher. 


![PCB bottom view](LCRmeter_PCB/pdfs_pictures/lcrmeter_bottom.png "PCB bottom view")


![PCB top view](./LCRmeter_PCB/pdfs_pictures/lcrmeter_top.png "PCB top view")


# TODO 

- add under sampling on ADC to increase range of frequency for sinewave
- Add automatic frequency and Rseries adjustment for best possible accuracy.
- reroute PCB
- Add edge detection for oscilloscope mode
- add handling of arbitrary waveform and PWM outputs
- Remove USB stuff - not needed
- 