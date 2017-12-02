#include "../../../src/globalConfig.h"

#include "gps.h"

LocationService::LocationService(COBDSPI* coProc){
  _coProc = coProc;
  _isInit = false;
}

bool LocationService::Initialize(unsigned long baud){
  bool retVal = false;

  #ifdef FREEMATICS_GPS
      Serial.println(baud);
      retVal = _coProc -> gpsInit(baud);
  #else
      _coProc->gpsSendCommand(GPS_INIT_CMD);
      retVal = true;
  #endif
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
}
