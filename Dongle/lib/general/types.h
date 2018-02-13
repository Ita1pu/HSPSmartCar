#ifndef TYPES_H
#define TYPES_H


namespace ourTypes
{
    const char MAXTROUBLECODES = 10;
    const char lengthOfVehicleIdentificationNumber = 17; //in short VIN
    const int bufferForGettingVIN = 150; //freematics whant this to be >=OBD_RECV_BUF_SIZE which is 80 in their example
    const char maxRetries = 10;

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
