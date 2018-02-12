#ifndef MAINHELPERS_H
#define MAINHELPERS_H
#include <stdint.h>

enum OperatingMode{LOGGING, UPLOAD};

struct {
  OperatingMode mode;
  bool bluetooth;
  uint8_t currentLoopCount;
} typedef ProgrammMode;

#endif
