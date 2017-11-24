#include "../../../src/globalConfig.h"

#include "clock.h"
#include "timer.h"
#include "ClockUtil.h"

bool Clock::Initialize(COBDSPI* coProc, unsigned long baud){
  bool retVal = false;
  if(coProc != NULL){
    Clock_Util::clVar = this;
    _coProc = coProc;
#ifdef FREEMATICS_GPS
    _coProc -> gpsInit(baud);
#else
    _coProc->gpsSendCommand(GPS_INIT_CMD);
#endif
    if(getGPStoInt()){
      retVal = SetTimer(2, CLOCK_RESOLUTION_MS, Clock_Util::timeReroute);
    }else{
      retVal = false;
    }
  }
  return retVal;
}

uint32_t Clock::GetTime(){
  return _timeOfDay;
}

uint32_t Clock::GetDate(){
  return _date;
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
  _timeOfDay += CLOCK_RESOLUTION_MS;
  //clean seconds
  uint32_t tmpSecs = _timeOfDay % 100000;
  if(tmpSecs >= 60000){
    getGPStoInt();
  }
}

void Clock::updateFlags(){
  *_flags |= 1;
}

bool Clock::getGPStoInt(){
  bool retVal = false;
  GPS_DATA gData;
  retVal = _coProc->gpsGetData(&gData);
  _timeOfDay = gData.time;
  _date = gData.date;
  return retVal;
}
