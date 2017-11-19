#ifndef CLOCKUTIL_H
#define CLOCKUTIL_H
#include "clock.h"

namespace Clock_Util{
  Clock* clVar;
  void flagReroute(void){
    if(clVar != 0){
      clVar->updateFlags();
    }
  }
  void timeReroute(void){
    if(clVar != 0){
      clVar->updateTime();
    }
  }
}

#endif
