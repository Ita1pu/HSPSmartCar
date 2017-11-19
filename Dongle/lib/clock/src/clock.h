#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

#define CLOCK_RESOLUTION_MS 100

/**
 *  Timing Class;
 *  This class provides methods for operations which have to do sth with time.
 *  E. g. setting timers and get the Time of day in UTC(GPS based).
 *  Attention: Normally the Timer 2 is used to update the _timeOfDay
 */
class Clock{
public:
  /**
   * Initialization Method Should be done at startup
   * @return Wether the initialization was successfull
   */
  bool Initialize();

  /**
   * Receive the UTC time of day;
   * @return Time of day as integer in following Format: HHMMSSmmm
   */
  uint32_t GetTime();

  /**
   * Start a timer
   * @param nr the number of the timer to start
   * @param millis the number of milliseconds the timer should run
   * @param flags the pointer to an integer which is set to 1 after timer completion
   * @return Wether the setup was successfull
   */
  bool SetTimer(uint8_t nr, uint16_t millis, uint8_t* flags);

  /**
   * Start a timer
   * Does not work in parallel with SetTimeTicker
   * @param nr the number of the timer to start
   * @param millis the number of milliseconds the timer should run
   * @param callback the pointer to the function which shall be executed after timer completion
   * @return Wether the setup was successfull
   */
  bool SetTimer(uint8_t nr, uint16_t millis, void (*callback)(void));

  /**
   * Set the flag in _flags
   * This method should not be called by other methods or Functions.
   */
  void updateFlags();

  /**
   * Update the _timeOfDay
   * This method should not be called by other methods or Functions.
   */
  void updateTime();
protected:
private:
  uint32_t parseGPStoInt(int time);
  uint8_t* _flags;
  uint16_t _timeCounter; /** This variable counts how many times the time is updated without GPS*/
  uint32_t _timeOfDay;
};

#endif
