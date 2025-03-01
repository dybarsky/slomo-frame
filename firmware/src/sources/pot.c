/*
frequency = 1 MHz
divider = 4
1 sec = 250 000
frequency = 10 Hz = 25 000
*/

#include <msp430g2553.h>
#include "pot.h"


#define PORT_SEL	P1SEL
#define PORT_SEL2	P2SEL2
#define PIN_POT		BIT6
#define CHANNEL		INCH_6
#define COUNTER		25000
#define RESOLUTION	1000

void configure_pot() {
	// ADC
	// Set pin as input
	PORT_SEL |= PIN_POT;
	// Set pint as analog
	PORT_SEL2 |= PIN_POT;
	// set voltage reference, 16 cycles, on
	ADC10CTL0 |= SREF_0 + ADC10SHT_1 + ADC10ON;
	// set channel, sample source, clock divider, clock, single channel conversion
	ADC10CTL1 |= CHANNEL + SHS_0 + ADC10DIV_0 + ADC10SSEL_0 + CONSEQ_0;
	// enable analog input
	ADC10AE0 = PIN_POT;
	// enable interruptions
	ADC10CTL0 |= ADC10IE;
	// Timer
	// sum-main clock + freq divider 2 + up mode + init
	TA0CTL = TASSEL_2 + ID_3 + MC_1 + TACLR;
	// counter
	TA0CCR0 = COUNTER;
	// enable interruptions
	TA0CCTL0 |= CCIE;
}

static void start_adc() {
	// start conversion
	ADC10CTL0 |= ADC10SC + ENC;
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void on_timer0_ccr0(void) {
	// request adc sample
	start_adc();
	// reset interruption flag
	TA0CCTL0 &= ~CCIFG;
}

#pragma vector = ADC10_VECTOR
__interrupt void on_adc_irs() {
	double input = ADC10MEM;
	float value = input / RESOLUTION;
	if (value > 1) {
		value = 1;
	}
	on_pot_sample(value);
	// clear interruption flag 
	ADC10CTL0 &= ~ADC10IFG;
}
