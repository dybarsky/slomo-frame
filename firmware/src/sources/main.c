#include <msp430g2553.h>
#include "em.h"
#include "led.h"
#include "pot.h"

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
	configure_led();
	configure_pot();
	//configure_magnet();
	// set up
	set_led_frequency(5);
	//set_magnet_frequency(80);
	// low power mode + enable interruptions
	_BIS_SR(LPM0_bits + GIE);
}

void event_pot_value(float value) {
	// todo:
	//	- read pot value
	// 	- calculate frequency beat
	// 	- shift led frequency
}