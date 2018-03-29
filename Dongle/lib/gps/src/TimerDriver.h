#ifndef TIMERDRIVER_H
#define TIMERDRIVER_H
#include <FreematicsONE.h>

namespace gps{
  /**
   *This function configures the Timer 1 to set the timerFlags every 500ms
   *@return Returns true when finished.
   */
  inline bool initFlagTimer(){
    //Disable Timer1 Interrupts
    TIMSK1 &= ~(1<<TOIE1);
    TIMSK1 &= ~(1<<OCIE1A);
    TIMSK1 &= ~(1<<OCIE1B);
    //Set CTC mode
    TCCR1A &= ~((1<<WGM11) | (1<<WGM10));
    TCCR1B = ((TCCR1B&0xE7) | (1<<WGM12));
    //Set prescaler to 256
    TCCR1B &= ~((1<<CS12)| (1<< CS11) | (1<<CS10));
    TCCR1B |= (1<<CS12);
    //Clear Interrupt flags
    TIFR1 = 0x0;

    //Set clear counter and enable compare interrupt
    OCR1A = 31249;
    TIMSK1 |= (1<<OCIE1A);
    return true;
  }
  /**
   *This function stops the Timer 1 to set the Flags every 500ms
   */
  inline void stopFlagTimer(){
    //Turn off timer(clock) by zeroing CS10 to CS12
    TCCR1B &= ~((1<<CS12) | (1<<CS11) | (1<<CS10));
    //Turn off Interrupt
    TIMSK1 &= ~(1<<OCIE1A);
  }

  /**
   *This function configures the Timer 2 to update the clockDeviation every 8ms
   *@return Returns true when finished.
   */
  inline bool initClockTimer(){
    //Disable Timer2 Interrupts
    TIMSK2 &= ~(1<<TOIE2);
    TIMSK2 &= ~(1<<OCIE2A);
    TIMSK2 &= ~(1<<OCIE2B);
    //Set CTC mode
    TCCR2A = (1<<WGM21);
    TCCR2B = 0x0;
    //Set prescaler to 1024
    TCCR2B &= ~((1<<CS22)| (1 << CS21) | (1<<CS20));
    TCCR2B |= ((1<<CS22)|(1<<CS21)|(1<<CS20));
    //Clear Interrupt flags
    TIFR2 = 0x0;

    //set compare register to 124 to have clock every 8 ms
    OCR2A = 124;
    TIMSK2 |= (1<<OCIE2A);
    return true;
  }
  /**
   *This function stops the Timer 2
   */
  inline void stopClockTimer(){
    //Turn off timer(clock) by zeroing CS20 to CS22
    TCCR2B &= ~((1<<CS22) | (1<<CS21) | (1<<CS20));
    //Turn off Interrupt
    TIMSK2 &= ~(1<<OCIE2A);
  }
}

#endif
