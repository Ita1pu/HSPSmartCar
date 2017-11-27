#include "obd.h"

namespace obd
{

ObdDevice::ObdDevice(COBDSPI* baseLayer) : baseLayer(baseLayer)
{

}

bool ObdDevice::initialize()
{
    veryFastPids = new std::vector<ourTypes::pidData>;
    fastPids = new std::vector<ourTypes::pidData>;
    normalPids = new std::vector<ourTypes::pidData>;
    slowPids = new std::vector<ourTypes::pidData>;
    if ((veryFastPids == nullptr) || (fastPids == nullptr) || (normalPids == nullptr) || (slowPids == nullptr))
    {
        return false;
    }

    veryFastPids->push_back(ourTypes::pidData {EngineCoolantTemp, 0});
    veryFastPids->push_back(ourTypes::pidData {VehicleSpeed, 0});
    veryFastPids->push_back(ourTypes::pidData {RelativAcceleratorPedalPos, 0});
    veryFastPids->push_back(ourTypes::pidData {EngineFuelRate, 0});
    veryFastPids->push_back(ourTypes::pidData {DriverTorqueDemandEngine, 0});
    veryFastPids->push_back(ourTypes::pidData {ActualTorqueEngine, 0});
    veryFastPids->push_back(ourTypes::pidData {EngineTorqueRef, 0});

    normalPids->push_back(ourTypes::pidData {EngineCoolantTemp, 0});
    normalPids->push_back(ourTypes::pidData {RunTimeSineEngineStart, 0});
    normalPids->push_back(ourTypes::pidData {AbsBarometricPressure, 0});
    normalPids->push_back(ourTypes::pidData {AmbientAirTemp, 0});
    normalPids->push_back(ourTypes::pidData {EngineOilTemp, 0});

    slowPids->push_back(ourTypes::pidData {DistTraveledWithMalfuncIndicaLamp, 0});
    slowPids->push_back(ourTypes::pidData {FuelTankLvlInput, 0});

    return baseLayer->init();
}

void ObdDevice::uninit()
{
    baseLayer->uninit();
}

void ObdDevice::reset()
{
    baseLayer->reset();
}

void ObdDevice::end()
{
    baseLayer->end();
}

int ObdDevice::getValueOfPid(char pid, bool& successful)
{
    successful = false;

    int value = 0;
    if (baseLayer->readPID(pid, value) == true)
    {
        successful = true;
    }

    return value;
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







std::vector<ourTypes::pidData>* ObdDevice::getVeryFastPids()
{
    return veryFastPids;
}

std::vector<ourTypes::pidData>* ObdDevice::getFastPids()
{
    return fastPids;
}

std::vector<ourTypes::pidData>* ObdDevice::getNormalPids()
{
    return normalPids;
}

std::vector<ourTypes::pidData>* ObdDevice::getSlowPids()
{
    return slowPids;
}


/*!
This function reads out the diagnostic trouble codes that currently are in the system. If less than 10 are read, there are not more,
if 10 are read there could be more. Every diagnostic trouble code is represented by 5 digits. First is a letter follwod by 4 digits.
With freematic library only the digits are available. The category letter is not available.
@return A vector of pidData. Every pidData element represents a trouble code. If nullptr is returned something went wrong.
*/
std::vector<ourTypes::dtcData>* ObdDevice::getDiagnositcTroubleCodes()
{
    uint16_t* readCodes = new uint16_t[ourTypes::MAXTROUBLECODES];
    std::vector<ourTypes::dtcData>* dtcVector = new std::vector<ourTypes::dtcData>;

    if (readCodes == nullptr || dtcVector == nullptr)
    {
        return nullptr;
    }

    char readTroubleCodesCount = baseLayer->readDTC(readCodes, ourTypes::MAXTROUBLECODES);

    if (readTroubleCodesCount == 0)
    {
        return nullptr;
    }
    else
    {
        for (int i=0; i<readTroubleCodesCount; ++i)
        {
            if (readCodes[i] != 0)
            {
                dtcVector->push_back(readCodes[i]);
            }
        }
        return dtcVector;
    }
}

void ObdDevice::clearDiagnosticTroubleCodes()
{
    baseLayer->clearDTC();
}

/**
@return An char[] containing the 13 bits of the vehicle identification numbers.
*/
char* ObdDevice::getVehicleIdentificationNumber()
{
    char* buffer = new char[ourTypes::lengthOfVehicleIdentificationNumber];

    bool status = baseLayer->getVIN(buffer, ourTypes::lengthOfVehicleIdentificationNumber);

    if (status == true)
    {
        return buffer;
        //todo: clean this mess up after it is used
    }
    else
    {
        return nullptr;
    }
}









}//end namespace obd
