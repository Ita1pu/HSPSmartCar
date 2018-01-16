#include "acc_reader.h"
#include <math.h>

bool AccReader::Initialize(){
  _isFirst = true;
  _isInit = _sensor.memsInit();
  accOffset[0] = 0.0f;
  accOffset[1] = 0.0f;
  accOffset[2] = 0.0f;

  return _isInit;
}

float AccReader::GetAccelerationAxis(uint8_t ct){
  if(ct >= 3 && _isInit == false)
    return -999.999;

  float accs[3];
  bool success = _sensor.memsRead(accs, 0, 0);
  if(success){
    //remove offset
    return accs[ct]-accOffset[ct];
  }else{
    return -999.999;
  }
}

float AccReader::GetAccelerationMag(){
  float accs[3];
  float tmp = 0.0f;
  bool success = _sensor.memsRead(accs, 0, 0);
  if(success){
    //remove offset
    accs[0] -= accOffset[0];
    accs[1] -= accOffset[1];
    accs[2] -= accOffset[2];

    for(int i = 0; i < 3; i++){
      //calculate euclidian vector length
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

void AccReader::Calibrate(bool accel, bool gyro){
  bool succ;
  float tmpacc[3];
  float tmpgyr[3];
  if(accel){
    succ = _sensor.memsRead(tmpacc, 0, 0);
    if(succ){
      accOffset[0] = tmpacc[0];
      accOffset[1] = tmpacc[1];
      accOffset[2] = tmpacc[2];
    }
  }
  if(gyro){
    succ = _sensor.memsRead(0, tmpgyr, 0);
    if(succ){
      _sensor.offsetGyro(tmpgyr);
    }
  }
}
