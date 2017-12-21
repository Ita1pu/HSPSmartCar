#include "acc_reader.h"

#define CORRECTION_FACTOR 9.58

bool AccReader::Initialize(){
  _isFirst = true;
  _sensor.memsInit(true);

  _isInit = true;
  return _isInit;
}

float AccReader::GetAccelerationAxis(uint8_t ct){
  if(ct >= 3 && _isInit == false)
    return -999.999;

  float accs[3];
  bool success = _sensor.memsRead(accs, NULL, NULL, NULL, NULL);
  if(success){
    //correct the value to m/s² according to https://freematics.com/forum/viewtopic.php?f=11&t=1982&hilit=acceleration#p3917
    return accs[ct]*CORRECTION_FACTOR;
  }else{
    return -999.999;
  }
}

float AccReader::GetAccelerationMag(){
  float accs[3];
  float tmp = 0.0f;
  bool success = _sensor.memsRead(accs, NULL, NULL, NULL, NULL);
  if(success){
    for(int i = 0; i < 3; i++){
      tmp += accs[i]*accs[i];
    }
    //correct the value to m/s² according to https://freematics.com/forum/viewtopic.php?f=11&t=1982&hilit=acceleration#p3917
    return sqrtf(tmp)*CORRECTION_FACTOR;
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
