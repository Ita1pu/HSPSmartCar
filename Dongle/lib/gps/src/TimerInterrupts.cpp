#include "gps.h"
#include <avr/interrupt.h>

//Attention: If you change this value, you have to change the code of function initClockTimer() in gps.cpp
//Nonmodularity was chosen to occupy less RAM
#define CLOCK_RESOLUTION_MS 8

uint8_t timerFlags;
uint16_t clockDeviation;

ISR(TIMER1_COMPA_vect){
    timerFlags = 0x1;
}

//Timer 2 measures the time since the last gps update
ISR(TIMER2_COMPA_vect){
  clockDeviation+=CLOCK_RESOLUTION_MS;
}
