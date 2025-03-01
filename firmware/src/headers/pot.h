/*
Potentiometer control.
Read data is float in range [0; 1]
*/

#ifndef HEADER_POT
#define HEADER_POT

void configure_pot();
void on_pot_sample(float value);

#endif // HEADER_POT
