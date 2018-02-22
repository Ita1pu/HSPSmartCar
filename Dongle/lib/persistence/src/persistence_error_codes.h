#ifndef PERSISTENCE_ERROR_CODES_H
#define PERSISTENCE_ERROR_CODES_H
#include <stdint.h>
namespace persistence
{

  typedef unsigned char stdRetVal;

  #define NO_ERROR                        0x00
  //Return Error Codes
  #define MVID_COUNTER_FULL               0xFE
  #define UNDEFINED_ERROR                 0xFF

  //Logfile Error Codes
  #define NEW_LOGGING_FILE_CREATED            0x02

};
#endif
