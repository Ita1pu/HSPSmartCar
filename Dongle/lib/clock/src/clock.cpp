#include "clock.h"
#include "timer.h"
#include "ClockUtil.h"

bool Clock::Initialize(){
  bool retVal = false;

  Clock_Util::clVar = this;
  _timeOfDay = 235959000;
  //TODO: Implementation
  //Get GPS_time into _timeOfDay
  //Setup timer for _timeOfDay with CLOCK_RESOLUTION_MS millis
  retVal = SetTimer(2, CLOCK_RESOLUTION_MS, Clock_Util::timeReroute);
  _timeCounter = 0;
  return retVal;
}

uint32_t Clock::GetTime(){
  if(_timeCounter >= 65535){
    //TODO: Get GPS_time int _timeOfDay
    _timeCounter = 0;
  }
  return _timeOfDay;
}

bool Clock::SetTimer(uint8_t nr, uint16_t millis, uint8_t *flags){
  bool retVal = false;
  if(nr < 1 || nr > 2){
    return false;
  }
  if(millis == 0){
    return false;
  }
  _flags = flags;
  retVal = Timer::set(nr, millis, Clock_Util::flagReroute);
  if(retVal){
    Timer::start(nr);
  }
  return retVal;
}

bool Clock::SetTimer(uint8_t nr, uint16_t millis, void (*callback)()){
  bool retVal = false;
  retVal = Timer::set(nr, millis, callback);
  if(retVal){
    Timer::start(nr);
  }
  return retVal;
}

void Clock::updateTime(){
  ++_timeCounter;
  _timeOfDay += CLOCK_RESOLUTION_MS;
  //clean seconds
  uint32_t tmpSecs = _timeOfDay % 100000;
  if(tmpSecs >= 60000){
    tmpSecs -= 60000;
    _timeOfDay -= 60000;
    _timeOfDay += 100000;
  }
  //clean minutes
  uint32_t tmpMin = _timeOfDay % 10000000;
  if(tmpMin >= 6000000){
    tmpMin -= 6000000;
    _timeOfDay -= 6000000;
    _timeOfDay += 10000000;
  }
  //clean hours
  if(_timeOfDay >= 240000000){
    //TODO: Get GPS_time int _timeOfDay; use this as temporal workaround
    _timeOfDay -= 240000000;
  }
}

void Clock::updateFlags(){
  *_flags |= 1;
}

uint32_t Clock::parseGPStoInt(int time){
  //TODO: Implementation;
  return 0;
}
