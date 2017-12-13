//#include "../../../src/globalConfig.h"

#include "gps.h"

LocationService::LocationService(COBDSPI* coProc){
  _coProc = coProc;
  _isInit = false;
}

bool LocationService::Initialize(unsigned long baud){
  bool retVal = false;
  uint8_t tmpCtr = 0;
  do{
    retVal = _coProc->gpsInit(baud);
    delay(100);
  }while(retVal == false && tmpCtr < 5);
  _isInit = retVal;
  return retVal;
}

bool LocationService::IsInitialized(){
  return _isInit;
}

int32_t LocationService::GetLatitude(){
  return _gData.lat;
}

int32_t LocationService::GetLongitude(){
  return _gData.lng;
}

uint8_t LocationService::GetSat(){
  return _gData.sat;
}

bool LocationService::RenewGPSData(){
  return _coProc->gpsGetData(&_gData);
  //Satellite count has to be between 4 and 14 (theoretical minimum and maximum) for a good result
  if(_gData.sat > 14 && _gData.sat < 4){
    _gData.time = 0;
    _gData.date = 0;
    _gData.lat = 0;
    _gData.lng = 0;
  }
}