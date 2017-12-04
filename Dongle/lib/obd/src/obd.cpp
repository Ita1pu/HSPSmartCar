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

    //not the selected pid signals are (if supported from the current car) added to their classes
    //>>>>>>>very fast pids<<<<<<<
    if (baseLayer->isValidPID(EngineRpm))
    {
        veryFastPids->push_back(ourTypes::pidData {EngineRpm, 0});
    }
    if (baseLayer->isValidPID(VehicleSpeed))
    {
        veryFastPids->push_back(ourTypes::pidData {VehicleSpeed, 0});
    }
    if (baseLayer->isValidPID(RelativAcceleratorPedalPos))
    {
        veryFastPids->push_back(ourTypes::pidData {RelativAcceleratorPedalPos, 0});
    }
    if (baseLayer->isValidPID(EngineFuelRate))
    {
        veryFastPids->push_back(ourTypes::pidData {EngineFuelRate, 0});
    }
    if (baseLayer->isValidPID(DriverTorqueDemandEngine))
    {
        veryFastPids->push_back(ourTypes::pidData {DriverTorqueDemandEngine, 0});
    }
    if (baseLayer->isValidPID(ActualTorqueEngine))
    {
        veryFastPids->push_back(ourTypes::pidData {ActualTorqueEngine, 0});
    }
    if (baseLayer->isValidPID(EngineTorqueRef))
    {
        veryFastPids->push_back(ourTypes::pidData {EngineTorqueRef, 0});
    }

    //>>>>>>>>normal pids<<<<<<<<<<<<
    if (baseLayer->isValidPID(EngineCoolantTemp))
    {
        normalPids->push_back(ourTypes::pidData {EngineCoolantTemp, 0});
    }
    if (baseLayer->isValidPID(RunTimeSineEngineStart))
    {
        normalPids->push_back(ourTypes::pidData {RunTimeSineEngineStart, 0});
    }
    if (baseLayer->isValidPID(AbsBarometricPressure))
    {
        normalPids->push_back(ourTypes::pidData {AbsBarometricPressure, 0});
    }
    if (baseLayer->isValidPID(AmbientAirTemp))
    {
        normalPids->push_back(ourTypes::pidData {AmbientAirTemp, 0});
    }
    if (baseLayer->isValidPID(EngineOilTemp))
    {
        normalPids->push_back(ourTypes::pidData {EngineOilTemp, 0});
    }

    //>>>>>>>slow pids<<<<<<<<<<<
    if (baseLayer->isValidPID(DistTraveledWithMalfuncIndicaLamp))
    {
        slowPids->push_back(ourTypes::pidData {DistTraveledWithMalfuncIndicaLamp, 0});
    }
    if (baseLayer->isValidPID(FuelTankLvlInput))
    {
        slowPids->push_back(ourTypes::pidData {FuelTankLvlInput, 0});
    }

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
    return updatePidVector(veryFastPids);
}

bool ObdDevice::updateFastPids()
{
    return updatePidVector(fastPids);
}

bool ObdDevice::updateNormalPids()
{
    return updatePidVector(normalPids);
}

bool ObdDevice::updateSlowPids()
{
    return updatePidVector(slowPids);
}

/*!
This function trys to pair the pids from a vector in groups of 4 (if there are not enought to always
get full groups, the last group is smaller) and than get their value from freematics lib.
@return True if there was no error with freematics and all values are updated
*/
bool ObdDevice::updatePidVector(std::vector<ourTypes::pidData>* pidVector)
{
    bool retVal = true;

    int pidAmount = pidVector->size();
    byte pids[4];
    int runner = 0;
    byte currentPidCnt = 0;

    while(runner < pidAmount)
    {
        pids[currentPidCnt] = (pidVector->at(runner)).pid;
        currentPidCnt++;
        runner++;

        if ((currentPidCnt >= 3) || (runner >= pidAmount-1)) //maximum of pids that could be asked at one is 4 -> see frematicsOne.h || there are less than 4 pids left
        {
            int pidValues[4];
            byte readPids = baseLayer->readPID(pids, currentPidCnt, pidValues);
            if (readPids == currentPidCnt)//I read less then the expected amount of pids, so something went wrong
            {
                retVal = false;
                break;
            }

            for (int i=0; i<currentPidCnt; ++i)//write the new values to the vector
            {
                (pidVector->at(runner)).value = pidValues[i];
            }

            currentPidCnt = 0;
        }
    }
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
