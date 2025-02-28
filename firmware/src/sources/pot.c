/*
Reads data from potentiometer using ADC.
*/

#include <msp430g2553.h>
#include "../headers/pot.h"

#define RES 1000.0f
#define PIN BIT7
#define CHANNEL INCH_6

void configure_pot() {
	P1SEL |= PIN;
	P1SEL2 |= PIN;
	// set default voltage reference
	ADC10CTL0 |= SREF_0;
	// set 16 cycles
	ADC10CTL0 |= ADC10SHT_2;
	// set on
	ADC10CTL0 |= ADC10ON;
	// set input channel
	ADC10CTL1 |= CHANNEL;
	// set sample source
	ADC10CTL1 |= SHS_0;
	// set clock divider
	ADC10CTL1 |= ADC10DIV_0;
	// set clock
	ADC10CTL1 |= ADC10SSEL_3;
	// repeat single channel conversion
	ADC10CTL1 |= CONSEQ_2;
	// enable analog input
	ADC10AE0 = PIN;
	// enable interruptions
	ADC10CTL0 |= ADC10IE;
	// enable conversion
	ADC10CTL0 |= ENC;
	// start conversion
	ADC10CTL0 |= ADC10SC;
}

#pragma vector = ADC10_VECTOR
__interrupt void on_adc_irs() {
	const unsigned int input = ADC10MEM;
	const float value = input / RES;
	event_pot_value(value);
}


