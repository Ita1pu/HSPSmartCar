#ifndef TYPES_H
#define TYPES_H


namespace ourTypes
{
    typedef unsigned char byte;
    typedef char smallPidVal;
    typedef int bigPidVal;


    struct pidData
    {
        char pid;
        char sizeOfData; //in utf8_byte
        char* data;
    };
}

#endif
