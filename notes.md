# 1.5V ref
Voltage from op-amp seems to not be strng enough - instead of flat 1.5V I see a sine same similar as in DAC generated, but lower by few milivolts.

I guess new solution needs to be made.

## zener diode with 470 ohm
Works good overall however highly dependent on supply voltage. Zener 2V. Would have to work on fixed voltage, but it's powered by battery, so it's dead-end.

## connected to GND 
Works good with caps, but with coil there are negative spikes. Some good diode can be inserted for protection. WIll work only with upper half of sinewave, since ADC cannot measure negative voltages. Algorithm seems robust as of now so it can be futher investigated.
For now caculations needs to be retested. I got negative resistance somehow as a result. 

## Ref Ics
to be checked



# ideas to think

- shouldn't there be a series resistor and 1nF cap on each of ADC channels?