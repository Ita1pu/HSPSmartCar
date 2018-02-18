#include "gps.h"
#include <time.h>

#define cmdLen 15

namespace gps{
  //The Register settings can be calculated by using the web-applet on
  //http://www.arduinoslovakia.eu/application/timer-calculator

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
 *This function configures the Timer 2 to set the timerFlags every 500ms
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
    Serial.print(" gI");
  return true;
}

LocationTimeService::LocationTimeService(COBDSPI* coProc){
  _coProc = coProc;
  _isInit = false;
}

bool LocationTimeService::Initialize(unsigned long baud){
  bool retVal = false;
  //Configure the additional use of EGNOS (to improve the GPS precision)
  uint8_t cmd[] = {0xB5, 0x62, 0x06, 0x16, 0x00, 0x08, 0x03, 0x07, 0x00, 0x00, 0x00, 0x00, 0x51, 0x7F, 0xEE };
  uint8_t tmpCtr = 0;
  do{
    retVal = _coProc->gpsInit(baud);
    delay(100);
    ++tmpCtr;
  }while(retVal == false && tmpCtr < 5);
  //send config command

  _coProc->setTarget(TARGET_GPS);
  _coProc->write(cmd, cmdLen);

  clockDeviation = 0;
  timerFlags = 0x0;

  //if previous commands succeeded, then configure clock interrupt
  if(retVal){
    retVal = initClockTimer();
  }
  _isInit = retVal;
  return retVal;
}

bool LocationTimeService::IsInitialized(){
  return _isInit;
}

int32_t LocationTimeService::GetLatitude(){
  return _gData.lat;
}

int32_t LocationTimeService::GetLongitude(){
  return _gData.lng;
}

uint8_t LocationTimeService::GetSat(){
  return _gData.sat;
}

uint32_t LocationTimeService::GetTime(){
  return _gData.time*10+clockDeviation;
}

uint32_t LocationTimeService::GetDate(){
  return _gData.date;
}

uint8_t* LocationTimeService::GetFlag(){
  return &timerFlags;
}

uint64_t LocationTimeService::GetEpochMs(){
  struct tm timeStruct;
  timeStruct.tm_year = 100 + (_gData.date%100); //Years since 1900
  timeStruct.tm_mon = ((_gData.date/100)%100) - 1; //The Month; January is 0, thus offset of 1;
  timeStruct.tm_mday = (_gData.date/10000)%100; //The day;
  timeStruct.tm_hour = _gData.time/1000000; //The hour of the day;
  timeStruct.tm_min = (_gData.time/10000)%100; //The minutes after the hour
  timeStruct.tm_sec = (_gData.time/100)%100; //The seconds after the minute
  timeStruct.tm_isdst = 0; //no Daylight Saving Time since UTC is unsigned

  //mktime(&timeStruct) gives epoch in seconds
  //this times 100  + _gData.time%100 gives epoch in 1/100 seconds
  //this times 10 + clockDeviation gives epoch in ms
  return (((mktime(&timeStruct)*100) + _gData.time%100) * 10)+clockDeviation;
}

bool LocationTimeService::RenewGPSData(){
  bool retVal = _coProc->gpsGetData(&_gData);
  //Satellite count has to be between 4 and 14 (theoretical minimum and maximum) for a good result
  if(_gData.sat > 14 && _gData.sat < 4){
    _gData.time = 0;
    _gData.date = 0;
    _gData.lat = 0;
    _gData.lng = 0;
  }
  clockDeviation = 0;
  return retVal;
}

bool LocationTimeService::StartFlagTimer(){
  return initFlagTimer();
}
}
