



TIM 20 for trigerring ADC

presclaer 7 -1
period 40 but shoultd be 4 -1
OC2 pulse 4-1
oc3 poulse 2-1

gives period of 170/7/4 = ca. 6 mHz

ADC clocks at 170/4 = 42,5 Mhz and takes 15 cycles, so 42,5/15 =  2.83 Mhz is a maximal sampling frequency

so let's make prescaler 3 times bigger


RepetitionCounter is used so UEV is set after repetirion counter elapsed


ADC1 - TRG 0 TIM20 -> OC2

Dma in disabled continous mode, so DMA is not cilcular and will take only a given number of points

Make sure dmaen in fcgr is enabled


TIM20 base:

(APB2PERIPH_BASE + 0x5000UL)
((PERIPH_BASE + 0x00010000UL) + 0x5000UL)
(( (0x40000000UL) + 0x00010000UL) + 0x5000UL)

0x40000000UL
0x00010000UL
0x00005000UL
=
0x40015000UL


Maybe add extra buffer space, and just use interrupt to stop timer????

OK the timer trigerring ADC is one time only, as the trigger is to start conversion, so if later DMA is used to transfer the data, then the sequence is running constantly without external trigger