#include "acc_reader.h"
#include <math.h>

bool AccReader::Initialize(){
  _isFirst = true;
  _isInit = _sensor.memsInit();

  return _isInit;
}

float AccReader::GetAccelerationAxis(uint8_t ct){
  if(ct >= 3 && _isInit == false)
    return -999.999;

  float accs[3];
  bool success = _sensor.memsRead(accs, 0, 0);
  if(success){
    return accs[ct];
  }else{
    return -999.999;
  }
}

float AccReader::GetAccelerationMag(){
  float accs[3];
  float tmp = 0.0f;
  bool success = _sensor.memsRead(accs, 0, 0);
  if(success){
    for(int i = 0; i < 3; i++){
      tmp += accs[i]*accs[i];
    }
    return sqrt(tmp);
  }else{
    return -999.999;
  }
}

float AccReader::GetAngle(uint8_t ct){
  if(ct >= 3 && _isInit == false)
    return -999.999;

  float angs[3];
  bool success = _sensor.memsRead(0, angs, 0);
  if(success){
    return angs[ct];
  }else{
    return -999.999;
  }
}

float AccReader::GetMagnet(uint8_t ct){
  if(ct >= 3 && _isInit == false)
    return -999.999;

  float mags[3];
  bool success = _sensor.memsRead(0, 0, mags);
  if(success){
    return mags[ct];
  }else{
    return -999.999;
  }
}
