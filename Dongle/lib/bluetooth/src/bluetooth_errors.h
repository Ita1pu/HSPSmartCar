#ifndef BLUETOOTH_ERRORS_H
#define BLUETOOTH_ERRORS_H

namespace Bluetooth{
    typedef unsigned char stdRetVal;
    
    #define STATUS_NO_ERROR                             0x00

    #define INIT_SUCCESS                                0x00
    #define INIT_FAILED                                 0xFF
    #define INIT_ENABLE_FAILED                          0xFE
};

#endif