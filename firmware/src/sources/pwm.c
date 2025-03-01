/*
frequency = 1 MHz
divider = 2
1 sec = 500 000
*/

#include <msp430g2553.h>
#include "pwm.h"

#define PORT_OUT		P2OUT
#define PORT_DIR		P2DIR
#define PIN_LED			BIT4
#define PIN_EM			BIT5
#define PWM_DUTY_LED	0.3
#define PWM_DUTY_EM		0.3
#define CLOCK			500000

// pwm state
typedef enum {
	OFF = 0,
	ON = 1,
} state_t;

// pwm struct
typedef struct {
	state_t state;
	unsigned char pin;
	unsigned int full;
	unsigned int duty;
} pwm_t;

static int calculate_schedule(pwm_t * pwm, int current);

static pwm_t em = {
	OFF, PIN_EM, 0, 0
};

static pwm_t led = {
	OFF, PIN_LED, 0, 0
};

void configure_pwm() {
	// configure out port
	PORT_DIR |= PIN_LED + PIN_EM;
	// sum-main clock + freq divider 2 + continues mode + init
	TA1CTL = TASSEL_2 + ID_1 + MC_2 + TACLR;
}

void set_magnet_frequency(unsigned char frequency) {
	// calculate pwm cycles
	em.full = CLOCK / frequency;
	em.duty = em.full * PWM_DUTY_EM;
	// configure timer counter
	TA1CCR0 = calculate_schedule(&em, TA1CCR0);
	// enable timer interruptions
	TA1CCTL0 |= CCIE;
}

void set_led_frequency(unsigned char frequency) {
	// calculate pwm cycles
	led.full = CLOCK / frequency;
	led.duty = led.full * PWM_DUTY_LED;
	// configure timer counter
	TA1CCR1 = calculate_schedule(&led, TA1CCR1);
	// enable timer interruptions	
	TA1CCTL1 |= CCIE;
}

static int get_pwm_phase(pwm_t * pwm) {
	switch (pwm->state) {
		case OFF:
			return pwm->full - pwm->duty;
		case ON:
			return pwm->duty;
	}
}

static int normalize_counter(long counter) {
	if (counter >= 0xFFFF) {
		return (int) (counter - 0xFFFF - 1);
	} else {
		return (int) counter;
	}
}

static int calculate_schedule(pwm_t * pwm, int current) {
	unsigned int shift = get_pwm_phase(pwm);
	return normalize_counter(current + shift);
}


static void toggle_state(pwm_t * pwm) {
	switch (pwm->state) {
		case OFF:
			pwm->state = ON;
			break;
		case ON:
			pwm->state = OFF;
			break;
	}	
}

static void set_output(pwm_t * pwm) {
	PORT_OUT ^= pwm->pin;
}

#pragma vector = TIMER1_A1_VECTOR
__interrupt void on_timer1_ccr1(void) {
	toggle_state(&led);
	set_output(&led);
	// setup timer
	TA1CCR1 = calculate_schedule(&led, TA1CCR1);
	// reset interruption flag
	TA1CCTL1 &= ~CCIFG;
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void on_timer1_ccr0(void) {
	toggle_state(&em);
	set_output(&em);
	// setup timer
	TA1CCR0 = calculate_schedule(&em, TA1CCR0);
	// reset interruption flag
	TA1CCTL0 &= ~CCIFG;
}
