#include "clock.h"

#include <../../freematics/src/FreematicsBase.h>

bool Clock::Init(){
  //TODO: Implementation
  //Get GPS_time into timeOfDay
  //Setup 100 millis timer for timeOfDay
  return false;
}

uint32_t Clock::GetTime(){
  if(timeCounter == 255){

      timeCounter = 0;
  }
  return timeOfDay;
}

bool Clock::SetTimer(uint8_t nr, uint32_t millis, uint8_t *flags){
//TODO: Implementation
  return false;
}

bool Clock::SetTimer(uint8_t nr, uint32_t millis, void (*callback)()){
  //TODO: Implementation
  return false;
}
