#ifndef PERSISTENCE_ERROR_CODES_H
#define PERSISTENCE_ERROR_CODES_H
#include <stdint.h>
namespace persistence
{

  typedef unsigned char stdRetVal;

  #define NO_ERROR                        0x00
  //MAPPING
  #define MVID_NEW_ENTRY_CREATED          0x01
  #define MVID_MAP_ERROR                  0xFF

  //Return Error Codes
  #define MAPFILE_FULL                    0xFE
  #define UNDEFINED_ERROR                 0xFF

  //Logfile Error Codes
  #define NEW_LOGGING_FILE_CREATED            0x02

};
#endif
