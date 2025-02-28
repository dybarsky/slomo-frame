/*
frequency = 1 MHz
divider = 4
1 sec = 250 000
pwm duty = 50 %
*/

#include <msp430g2553.h>
#include "../headers/led.h"

#define PIN BIT7
#define CLOCK 250000
#define PWM_DUTY 0.5

void configure_led() {
	// configure out port
	P1DIR |= PIN;
	// enable timer interruptions
	TA1CCTL0 = CCIE;
	TA1CCTL1 = CCIE;
	// sum-main clock + freq divider 2 + up mode + init
	TA0CTL = TASSEL_2 + ID_2 + MC_1 + TACLR;
}

void set_led_frequency(float freq) {
	const unsigned long pwm_full_time = CLOCK / freq;
	const unsigned long pwm_duty_time = pwm_full_time * PWM_DUTY;
	// configure timer counter
	TA1CCR0 = pwm_full_time;
	TA1CCR1 = pwm_duty_time;
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void on_timer1_ccr0(void) {
	// turn on ports
	P1OUT |= PIN;
	// reset interruption flag
	TA1CCTL0 &= ~CCIFG;
}

#pragma vector = TIMER1_A1_VECTOR
__interrupt void on_timer1_ccr1(void) {
	// turn off ports
	P1OUT &= ~PIN;
	// reset interruption flag
	TA1CCTL1 &= ~CCIFG;
}
