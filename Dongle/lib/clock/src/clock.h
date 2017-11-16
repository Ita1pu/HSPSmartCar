#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

/**
 *  Timing Class;
 *  This class provides methods for operations which have to do sth with time.
 *  E. g. setting timers and get the Time of day in UTC(GPS based).
 */
class Clock{
public:
  /**
   * Initialization Method Should be done at startup
   * @return Wether the initialization was successfull
   */
  bool Init();

  /**
   * Receive the UTC time of day;
   * @return Time of day as integer in following Format: HHMMSSmmm
   */
  uint32_t GetTime();

  /**
   * Start a timer
   * @param nr the number of the timer to start
   * @param millis the number of milliseconds the timer should run
   * @param flags the pointer to an integer where the bit specified by parameter nr is set to 1 after timer completion
   * @return Wether the setup was successfull
   */
  bool SetTimer(uint8_t nr, uint32_t millis, uint8_t* flags);

  /**
   * Start a timer
   * @param nr the number of the timer to start
   * @param millis the number of milliseconds the timer should run
   * @param callback the pointer to the function which shall be executed after timer completion
   * @return Wether the setup was successfull
   */
  bool SetTimer(uint8_t nr, uint32_t millis, void (*callback)(void));
protected:
private:
  uint8_t timeCounter; /** This variable counts how many times the time is updated without GPS*/
  uint32_t timeOfDay;
};


#endif
