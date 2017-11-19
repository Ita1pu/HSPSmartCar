#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/**
 *  Timer Namespace;
 *  This namespace provides functions to trigger callback functions after a fixed period of time
 */
namespace Timer{
  /**
   * Configure a timer
   * @param timerNr the number of the timer to configure
   * @param ms the number of milliseconds the timer should run; Only even numbers are allowed
   * @param f the pointer to a callback function
   */
  bool set(uint8_t timerNr,uint16_t ms, void (*f)());
  /**
   * Start a timer
   * @param timerNr the number of the timer to start
   */
  void start(uint8_t timerNr);
  /**
   * Stop a timer
   * @param timerNr the number of the timer to stop
   */
  void stop(uint8_t timerNr);

  /**
   * Utility functions and variables;
   */
  void __overflow1();
  void __overflow2();
  bool set1(uint16_t ms, void (*f)());
  bool set2(uint16_t ms, void (*f)());
  extern void (*func1)();
  extern void (*func2)();
  extern uint16_t msecs1;
  extern uint16_t msecs2; //32bit counter is used because timer 2 runs twice as fast as timer 1
  extern volatile uint16_t tcnt1;
  extern volatile uint16_t tcnt2;
};

#endif
