#include <msp430g2553.h>
#include "pwm.h"
#include "pot.h"

#define FREQ_BASE	80
#define FREQ_DIFF	20

// initialization
void configure() {
	// turn off watchdog
	WDTCTL = WDTPW + WDTHOLD;
	// set frequency 1MHz
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	// set all pins off
	P1OUT = 0;
	P2OUT = 0;
}

// starting point
int main(void) {
	// config
	configure();
	configure_pwm();
	configure_pot();
	// set up
	set_led_frequency(FREQ_BASE);
	set_magnet_frequency(FREQ_BASE + FREQ_DIFF);
	// low power mode + enable interruptions
	_BIS_SR(LPM0_bits + GIE);
}

void on_pot_sample(float value) {
	float step = FREQ_DIFF * value;
	float freq = FREQ_BASE + step;
	//set_led_frequency(freq);
}
