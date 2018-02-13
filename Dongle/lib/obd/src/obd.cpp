#include "obd.h"

namespace obd
{

ObdDevice::ObdDevice(COBDSPI* baseLayer) : baseLayer(baseLayer)
{

}

ObdDevice::~ObdDevice()
{
    if (veryFastPids != nullptr)
    {
        delete veryFastPids;
    }
    if (fastPids != nullptr)
    {
            delete fastPids;
    }
    if (normalPids != nullptr)
    {
        delete normalPids;
    }
    if (slowPids != nullptr)
    {
        delete slowPids;
    }


}

bool ObdDevice::initialize()
{
    if (wasAlreadyInitialiesed == true)
    {
        baseLayer->uninit();
//        Serial.println("Base was already initialiezed, uninit will be called");
        if (baseLayer->init(lastUsedProtocol) == true)
        {
            Clamp15 = true;
//            Serial.println("initialies with the lased used one worked");
            return true;
        }
        else
        {
            Clamp15 = false;
//            Serial.println("initialies with the lased used one failed");
            return false;
        }
    }

    //check all possible protokolls
    if (baseLayer->init(PROTO_AUTO) == true)
    {
        Clamp15 = true;
//        Serial.println("base initialized protocol is PROTO_AUTO");
        lastUsedProtocol = PROTO_AUTO;
    }
    else if (baseLayer->init(PROTO_ISO_9141_2) == true)
    {
        Clamp15 = true;
//        Serial.println("base initialized protocol is PROTO_ISO_9141_2");
        lastUsedProtocol = PROTO_ISO_9141_2;
    }
    else if (baseLayer->init(PROTO_KWP2000_5KBPS) == true)
    {
        Clamp15 = true;
//        Serial.println("base initialized protocol is PROTO_KWP2000_5KBPS");
        lastUsedProtocol = PROTO_KWP2000_5KBPS;
    }
    else if (baseLayer->init(PROTO_KWP2000_FAST) == true)
    {
        Clamp15 = true;
//        Serial.println("base initialized protocol is PROTO_KWP2000_FAST");
        lastUsedProtocol = PROTO_KWP2000_FAST;
    }
    else if (baseLayer->init(PROTO_CAN_11B_500K) == true)
    {
        Clamp15 = true;
//        Serial.println("base initialized protocol is PROTO_CAN_11B_500K");
        lastUsedProtocol = PROTO_CAN_11B_500K;
    }
    else if (baseLayer->init(PROTO_CAN_29B_500K) == true)
    {
        Clamp15 = true;
//        Serial.println("base initialized protocol is PROTO_CAN_29B_500K");
        lastUsedProtocol = PROTO_CAN_29B_500K;
    }
    else if (baseLayer->init(PROTO_CAN_29B_250K) == true)
    {
        Clamp15 = true;
//        Serial.println("base initialized protocol is PROTO_CAN_29B_250K");
        lastUsedProtocol = PROTO_CAN_29B_250K;
    }
    else if (baseLayer->init(PROTO_CAN_11B_250K) == true)
    {
        Clamp15 = true;
//        Serial.println("base initialized protocol is PROTO_CAN_11B_250K");
        lastUsedProtocol = PROTO_CAN_11B_250K;
    }

    if (Clamp15 == false)
    {
        return false;
//        Serial.println("not initialiesed");
    }
    else
    {
        wasAlreadyInitialiesed = true;
//        Serial.println("now initialiesed first time");
    }

    return fillPidVectors();
}

bool ObdDevice::fillPidVectors()
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
    if (baseLayer->isValidPID(obd::EngineRpm))
    {
        veryFastPids->push_back(ourTypes::pidData {obd::EngineRpm, 0});
    }
    if (baseLayer->isValidPID(obd::VehicleSpeed))
    {
        veryFastPids->push_back(ourTypes::pidData {obd::VehicleSpeed, 0});
    }
    if (baseLayer->isValidPID(obd::RelativAcceleratorPedalPos))
    {
        veryFastPids->push_back(ourTypes::pidData {obd::RelativAcceleratorPedalPos, 0});
    }
    if (baseLayer->isValidPID(obd::EngineFuelRate))
    {
        veryFastPids->push_back(ourTypes::pidData {obd::EngineFuelRate, 0});
    }
    if (baseLayer->isValidPID(obd::DriverTorqueDemandEngine))
    {
        veryFastPids->push_back(ourTypes::pidData {obd::DriverTorqueDemandEngine, 0});
    }
    if (baseLayer->isValidPID(obd::ActualTorqueEngine))
    {
        veryFastPids->push_back(ourTypes::pidData {obd::ActualTorqueEngine, 0});
    }
    if (baseLayer->isValidPID(obd::EngineTorqueRef))
    {
        veryFastPids->push_back(ourTypes::pidData {obd::EngineTorqueRef, 0});
    }

    //>>>>>>>>normal pids<<<<<<<<<<<<
    if (baseLayer->isValidPID(obd::EngineCoolantTemp))
    {
        normalPids->push_back(ourTypes::pidData {obd::EngineCoolantTemp, 0});
    }
    if (baseLayer->isValidPID(obd::RunTimeSineEngineStart))
    {
        normalPids->push_back(ourTypes::pidData {obd::RunTimeSineEngineStart, 0});
    }
    if (baseLayer->isValidPID(obd::AbsBarometricPressure))
    {
        normalPids->push_back(ourTypes::pidData {obd::AbsBarometricPressure, 0});
    }
    if (baseLayer->isValidPID(obd::AmbientAirTemp))
    {
        normalPids->push_back(ourTypes::pidData {obd::AmbientAirTemp, 0});
    }
    if (baseLayer->isValidPID(obd::EngineOilTemp))
    {
        normalPids->push_back(ourTypes::pidData {obd::EngineOilTemp, 0});
    }

    //>>>>>>>slow pids<<<<<<<<<<<
    if (baseLayer->isValidPID(obd::DistTraveledWithMalfuncIndicaLamp))
    {
        slowPids->push_back(ourTypes::pidData {obd::DistTraveledWithMalfuncIndicaLamp, 0});
    }
    if (baseLayer->isValidPID(obd::FuelTankLvlInput))
    {
        slowPids->push_back(ourTypes::pidData {obd::FuelTankLvlInput, 0});
    }

//    Serial.print("Supported very fast: "); Serial.println(veryFastPids->size());
//    Serial.print("Supported fast: "); Serial.println(fastPids->size());
//    Serial.print("Supported normal: "); Serial.println(normalPids->size());
//    Serial.print("Supported slow: "); Serial.println(slowPids->size());
    return true;
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
    if (baseLayer->isValidPID(pid) == false)
    {
        successful = false;
        return 0;
    }

    successful = false;
    int value = 0;

    for (int i=0; i<ourTypes::maxRetries; ++i)
    {
        if (baseLayer->readPID(pid, value) == true)
        {
            successful = true;
            break;
        }
    }

    if(successful == false)
    {
        Clamp15 = false;
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
This function updates the values from pids in a given vector.
@return True if there was no error with freematics and all values are updated
*/
bool ObdDevice::updatePidVector(std::vector<ourTypes::pidData>* pidVector)
{
    bool retVal = true;
    bool temp = false;
    for(unsigned int i=0; i<pidVector->size(); ++i)
    {
        (pidVector->at(i)).value = getValueOfPid((pidVector->at(i)).pid, temp);
        if (temp == false)
        {
            retVal = false;
            delay(5); //wait short time or the odb is to much triggered
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
    //todo: eventuell auch hier dafür sorgen, dass der VECTOR zerstört wird wenn man dieses objekt zerstört
    //hier ist eine maximale Anzahl an Versuchen nicht sinvoll, weil wenn kein Fehler vorliegt 0 zurück gegeben wird, ebenso wenn steuergerät nicht verfügbar ist
    uint16_t* readCodes = new uint16_t[ourTypes::MAXTROUBLECODES];
    std::vector<ourTypes::dtcData>* dtcVector = new std::vector<ourTypes::dtcData>;

    if (readCodes == nullptr || dtcVector == nullptr)
    {
        return nullptr;
    }

    std::vector<ourTypes::dtcData>* returnVal = nullptr;
    char readTroubleCodesCount = baseLayer->readDTC(readCodes, ourTypes::MAXTROUBLECODES);
    Serial.print("I read #"), Serial.print(readTroubleCodesCount+10), Serial.println(" many trouble codes");
    if (readTroubleCodesCount == 0)
    {
        returnVal = nullptr;
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
        returnVal = dtcVector;
    }
    delete[] readCodes;
    return returnVal;
}

void ObdDevice::clearDiagnosticTroubleCodes()
{
    //hier geht maximale anzahl an versuche auch nicht, weil freematics einfach lösch befehl sendet und es keine rückmeldung über status gibt
    baseLayer->clearDTC();
}

/**
@return An char[] containing the 13 bytes of the vehicle identification numbers.
*/
char* ObdDevice::getVehicleIdentificationNumber()
{
    //todo: auch hier maximale anzahl an versuchen einfügen und wenn diese überschritten wird clamp 15 auf falsch setzen
    char* buffer = new char[ourTypes::bufferForGettingVIN];

    if (buffer == nullptr)
    {
        return nullptr;
    }

    bool status = baseLayer->getVIN(buffer, ourTypes::bufferForGettingVIN);

    if (status == true)
    {
        char* VIN = new char[ourTypes::lengthOfVehicleIdentificationNumber];
        if (VIN == nullptr)
        {
            return nullptr;
        }

        for (int i=0; i<ourTypes::lengthOfVehicleIdentificationNumber; ++i)
        {
            VIN[i] = buffer[i];
        }

        delete[] buffer;
        return VIN;
        //todo: clean this mess up after it is used
    }
    else
    {
        return nullptr;
    }
}


bool ObdDevice::getClamp15State()
{
    return Clamp15;
}






}//end namespace obd
