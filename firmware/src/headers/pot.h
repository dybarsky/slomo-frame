/*
Potentiometer control.
Read data is float in range [0; 1]
*/

#ifndef HEADER_POT
#define HEADER_POT

void configure_pot();
void event_pot_value(float value);

#endif // HEADER_POT
