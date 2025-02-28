/*
frequency = 1 MHz
divider = 4
1 sec = 250 000
pwm time = 5 %
*/

#include <msp430g2553.h>
#include "../headers/em.h"

#define PIN BIT4
#define CLOCK 250000
#define PWM_TIME 0.05

void configure_magnet() {
	// configure out port
	P1DIR |= PIN;
	// enable timer interruptions
	TA0CCTL0 = CCIE;
	TA0CCTL1 = CCIE;
	// sum-main clock + freq divider 2 + up mode + init
	TA0CTL = TASSEL_2 + ID_2 + MC_1 + TACLR;
}

void set_magnet_frequency(float freq) {
	const unsigned long pwm_full_time = CLOCK / freq;
	const unsigned long pwm_duty_time = pwm_full_time * PWM_TIME;
	// configure timer counter
	TA0CCR0 = pwm_full_time;
	TA0CCR1 = pwm_duty_time;
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void on_timer0_ccr0(void) {
	// turn on ports
	P1OUT |= PIN;
	// reset interruption flag
	TA0CCTL0 &= ~CCIFG;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void on_timer0_ccr1(void) {
	// turn off ports
	P1OUT &= ~PIN;
	// reset interruption flag
	TA0CCTL1 &= ~CCIFG;
}
