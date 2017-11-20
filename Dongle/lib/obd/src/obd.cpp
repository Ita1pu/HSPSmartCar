#include "obd.h"

namespace obd
{

ObdDevice::ObdDevice()
{

}

bool ObdDevice::initialize()
{
    bool retVal = true;



    return retVal;
}

char* ObdDevice::getValueOfPid(char pid, int& size)
{
    



    return 0;
}

bool ObdDevice::updateVeryFastPids()
{
    bool retVal = true;



    return retVal;
}

bool ObdDevice::updateFastPids()
{
    bool retVal = true;



    return retVal;
}

bool ObdDevice::updateNormalPids()
{
    bool retVal = true;



    return retVal;
}

bool ObdDevice::updateSlowPids()
{
    bool retVal = true;



    return retVal;
}







std::vector<ourTypes::pidData> ObdDevice::getVeryFastPids()
{
    return veryFastPids;
}

std::vector<ourTypes::pidData> ObdDevice::getFastPids()
{
    return fastPids;
}

std::vector<ourTypes::pidData> ObdDevice::getNormalPids()
{
    return normalPids;
}

std::vector<ourTypes::pidData> ObdDevice::getSlowPids()
{
    return slowPids;
}


}//end namespace obd
