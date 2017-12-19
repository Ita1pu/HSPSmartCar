#include "acc_reader.h"

bool AccReader::Initialize(){
  _isFirst = true;
  _sensor.memsInit(true);

  _isInit = true;
  return _isInit;
}

float AccReader::GetAcceleration(uint8_t ct){
  if(ct >= 3 && _isInit == false)
    return -999.999;

  float accs[3];
  bool success = _sensor.memsRead(accs, NULL, NULL, NULL, NULL);
  if(success){
    return accs[ct];
  }else{
    return -999.999;
  }
}

float AccReader::GetAngle(uint8_t ct){
  if(ct >= 3 && _isInit == false)
    return -999.999;

  float angs[3];
  bool success = _sensor.memsRead(NULL, angs, NULL, NULL, NULL);
  if(success){
    return angs[ct];
  }else{
    return -999.999;
  }
}
