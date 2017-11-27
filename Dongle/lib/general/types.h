#ifndef TYPES_H
#define TYPES_H


namespace ourTypes
{
    const char MAXTROUBLECODES = 10;
    const char lengthOfVehicleIdentificationNumber = 13;

    //typedef unsigned char byte;
    typedef char smallPidVal;
    typedef int bigPidVal;
    typedef uint16_t dtcData;//DiagnisticTroubleCode Data

    struct pidData
    {
        char pid;
        int value;
    };

}

#endif
