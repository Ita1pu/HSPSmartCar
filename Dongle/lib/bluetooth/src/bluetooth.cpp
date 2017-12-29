#include "bluetooth.h"

using namespace Bluetooth;

bluetooth::bluetooth(std::string deviceName)
{
    BTSerial.begin(115200);
    this->_deviceName = deviceName;
}

uint8_t bluetooth::getStatus(){
    return this->_status;
}

std::string bluetooth::getDeviceName(){
    return this->_deviceName;
}

stdRetVal bluetooth::connect(){
    return INIT_SUCCESS;
}

stdRetVal bluetooth::makeViseble()
{
    return 0;
}