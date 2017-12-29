#include "timer.h"
#include <stdlib.h>
#include <avr/interrupt.h>

void (*Timer::func1)();
void (*Timer::func2)();
uint16_t Timer::msecs1;
uint16_t Timer::msecs2;
volatile uint16_t Timer::tcnt1;
volatile uint16_t Timer::tcnt2;

bool Timer::set(uint8_t timerNr, uint16_t ms, void (*f)()){
  if(f == 0x0){
    return false;
  }

  switch (timerNr) {
    case 1:
      return set1(ms,f);
    case 2:
      return set2(ms,f);
    default:
      return false;
  }
}

bool Timer::set1(uint16_t ms, void (*f)()){
  //Disable Timer1 Interrupts
  TIMSK1 &= ~(1<<TOIE1);
  TIMSK1 &= ~(1<<OCIE1A);
  TIMSK1 &= ~(1<<OCIE1B);
  //Set CTC mode
  TCCR1A &= ~((1<<WGM11) | (1<<WGM10));
  TCCR1B = ((TCCR1B&0xE7) | (1<<WGM12));
  //Set prescaler to 64
  TCCR1B &= ~((1<<CS12)| (1 << CS11) | (1<<CS10));
  TCCR1B |= ((1<<CS11) | (1<<CS10));
  //Clear Interrupt flags
  TIFR1 = 0x0;

  if(ms == 0)
    msecs1 = 1;
  else
    msecs1 = ms;

  func1 = f;
  if(func1 != NULL){
    return true;
  }else{
    return false;
  }
}

bool Timer::set2(uint16_t ms, void (*f)()){
  //Disable Timer1 Interrupts
  TIMSK2 &= ~(1<<TOIE2);
  TIMSK2 &= ~(1<<OCIE2A);
  TIMSK2 &= ~(1<<OCIE2B);
  //Set CTC mode
  TCCR2A = (1<<WGM21);
  TCCR2B = 0x0;
  //Set prescaler to 64
  TCCR2B &= ~((1<<CS22)| (1 << CS21) | (1<<CS20));
  TCCR2B |= (1<<CS22);
  //Clear Interrupt flags
  TIFR2 = 0x0;

  if(ms == 0)
    msecs2 = 0;
  else
    msecs2 = ms;

  func2 = f;
  if(func2 != NULL){
    return true;
  }else{
    return false;
  }
}

void Timer::start(uint8_t timerNr){
  switch (timerNr) {
    case 1: //start timer 1
      //Set clear counter and enable compare interrupt
      OCR1A = 250;
      TIMSK1 |= (1<<OCIE1A);
    break;
    case 2: //start timer 2
      OCR2A = 250;
      TIMSK2 |= (1<<OCIE2A);
    break;
    default:
    return;
  }
}

void Timer::stop(uint8_t timerNr){
  switch(timerNr){
    case 1:
    //disable compare Interrupt
      TIMSK1 &= ~(1<<OCIE1A);
    break;
    case 2:
    //disable compare Interrupts
      TIMSK2 &= ~(1<<OCIE2A);
    break;
    default:
    return;
  }
}

void Timer::__overflow1(){
  ++tcnt1;
  if(tcnt1 >= msecs1){
    (*func1)();
    tcnt1 = 0;
  }
}
void Timer::__overflow2(){
  ++tcnt2;
  if(tcnt2 >= msecs2){
    (*func2)();
    tcnt2 = 0;
  }
}
ISR(TIMER1_COMPA_vect){
  Timer::__overflow1();
}
ISR(TIMER2_COMPA_vect){
  Timer::__overflow2();
}
