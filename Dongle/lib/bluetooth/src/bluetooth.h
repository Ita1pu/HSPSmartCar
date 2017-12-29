#ifndef BLUETOOTH_H
#define BLUETOOTH_H
#include <types.h>
#include "bluetooth_errors.h"
#include <stdint.h>
#include <vector>
#include <string>
#include <SoftwareSerial.h>
#define M_RXD_PIN               30
#define M_TXD_PIN               31
SoftwareSerial BTSerial(M_RXD_PIN, M_TXD_PIN);

namespace Bluetooth{
    class bluetooth{
private:
    uint8_t _status = STATUS_NO_ERROR;
    std::string _deviceName = NULL;

public:
    bluetooth(std::string deviceName);
    stdRetVal connect();
    stdRetVal makeViseble();
    stdRetVal disconnect();
    stdRetVal makeInvisible();
    stdRetVal sendData(std::vector<uint8_t>);

    std::string getDeviceName();
    uint8_t getStatus();

};
};
#endif
