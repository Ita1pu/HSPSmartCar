#include "clock.h"
#include "timer.h"
#include "ClockUtil.h"

#include <time.h>

Clock::Clock(COBDSPI* coProc){
  Clock_Util::clVar = this;
  _coProc = coProc;
}

bool Clock::Initialize(LocationService* locService, uint8_t nr){
  bool retVal = false;
  if(_coProc != NULL && locService->IsInitialized()){
    if(getGPStoInt()){
      retVal = SetTimer(nr, CLOCK_RESOLUTION_MS, Clock_Util::timeReroute);
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

uint64_t Clock::GetEpochMs(){
  return _epoch;
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
  _epoch += CLOCK_RESOLUTION_MS;
  //clean seconds
  //get deviation to CLOCK_UPDATE_MS milliseconds
  uint32_t tmpSecs = _timeOfDay % CLOCK_UPDATE_MS;
  if(tmpSecs > (CLOCK_UPDATE_MS - CLOCK_RESOLUTION_MS) || tmpSecs < CLOCK_RESOLUTION_MS){
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
  //Satellite count has to be between 4 and 14 (theoretical minimum and maximum) for a good result

  if(retVal && gData.sat <= 14 && gData.sat >= 4){
    _timeOfDay = gData.time*10;
    _date = gData.date;
    setEpoch(gData);
  }
  return retVal;
}

void Clock::setEpoch(GPS_DATA gData){
  struct tm timeStruct;
  timeStruct.tm_year = 100 + (gData.date%100); //Years since 1900
  timeStruct.tm_mon = ((gData.date/100)%100) - 1; //The Month; January is 0, thus offset of 1;
  timeStruct.tm_mday = (gData.date/10000)%100; //The day;
  timeStruct.tm_hour = gData.time/1000000; //The hour of the day;
  timeStruct.tm_min = (gData.time/10000)%100; //The minutes after the hour
  timeStruct.tm_sec = (gData.time/100)%100; //The seconds after the minute
  timeStruct.tm_isdst = 0; //no Daylight Saving Time since UTC is unsigned

  _epoch = mktime(&timeStruct) + gData.time%100;
  _epoch *= 10;
}
