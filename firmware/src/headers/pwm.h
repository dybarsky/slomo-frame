/*
LED strip control.
Frequency of PWM can be changed.
*/

#ifndef HEADER_PWM
#define HEADER_PWM

void configure_pwm();
void set_led_frequency(unsigned char frequency);
void set_magnet_frequency(unsigned char frequency);

#endif // HEADER_PWM
