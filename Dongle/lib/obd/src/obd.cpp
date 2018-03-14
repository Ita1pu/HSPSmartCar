#include "obd.h"
//#include <globalConfig.h>
namespace obd
{

ObdDevice::ObdDevice(COBDSPI* baseLayer) : baseLayer(baseLayer)
{

}

ObdDevice::~ObdDevice()
{
    if (pidArray != nullptr)
    {
        delete pidArray;
    }
    /*
    if (dtcVector != nullptr)
    {
        delete dtcVector;
    }
    */
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
    identifyMaxPidArrayLength();
    Serial.print(maxLengthPidArray), Serial.println("Entrys");
    pidArray = new ourTypes::pidData[maxLengthPidArray];
    return (pidArray == nullptr) ? false : true;
}

/*!
This function get the values of the very fast pids via obd. It writes the pairs (pid, value) into the pid array.
@return -1 if an error occured, 0 if nothing was read and everything >0 determines the amount of pids in the array.
*/
char ObdDevice::updateVeryFastPids()
{
    //>>>>>>>very fast pids<<<<<<<
    unsigned char retVal = 0;
    bool queryState = false;
    obd::PidNames pidName;

    pidName = obd::PidNames::EngineRpm;
    char currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        (pidArray[retVal]).pid = currentPid;
        pidArray[retVal++].value = getValueOfPid(currentPid, queryState);
        if (queryState == false) //reading of pid value didn't work, neglect last read values
        {
            --retVal;
        }
    }

    pidName = obd::PidNames::VehicleSpeed;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        pidArray[retVal].pid = currentPid;
        pidArray[retVal++].value = getValueOfPid(currentPid, queryState);
        if (queryState == false) //reading of pid value didn't work, neglect last read values
        {
            --retVal;
        }
    }

    pidName = obd::PidNames::RelativAcceleratorPedalPos;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        pidArray[retVal].pid = currentPid;
        pidArray[retVal++].value = getValueOfPid(currentPid, queryState);
        if (queryState == false) //reading of pid value didn't work, neglect last read values
        {
            --retVal;
        }
    }

    pidName = obd::PidNames::EngineFuelRate;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        pidArray[retVal].pid = currentPid;
        pidArray[retVal++].value = getValueOfPid(currentPid, queryState);
        if (queryState == false) //reading of pid value didn't work, neglect last read values
        {
            --retVal;
        }
    }

    pidName = obd::PidNames::DriverTorqueDemandEngine;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        pidArray[retVal].pid = currentPid;
        pidArray[retVal++].value = getValueOfPid(currentPid, queryState);
        if (queryState == false) //reading of pid value didn't work, neglect last read values
        {
            --retVal;
        }
    }

    pidName = obd::PidNames::ActualTorqueEngine;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        pidArray[retVal].pid = currentPid;
        pidArray[retVal++].value = getValueOfPid(currentPid, queryState);
        if (queryState == false) //reading of pid value didn't work, neglect last read values
        {
            --retVal;
        }
    }

    pidName = obd::PidNames::EngineTorqueRef;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        pidArray[retVal].pid = currentPid;
        pidArray[retVal++].value = getValueOfPid(currentPid, queryState);
        if (queryState == false) //reading of pid value didn't work, neglect last read values
        {
            --retVal;
        }
    }
    //Consumption additions
    pidName = obd::PidNames::EngineLoad;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        pidArray[retVal].pid = currentPid;
        pidArray[retVal++].value = getValueOfPid(currentPid, queryState);
        if (queryState == false) //reading of pid value didn't work, neglect last read values
        {
            --retVal;
        }
    }
    pidName = obd::PidNames::MAFAirFlow;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        pidArray[retVal].pid = currentPid;
        pidArray[retVal++].value = getValueOfPid(currentPid, queryState);
        if (queryState == false) //reading of pid value didn't work, neglect last read values
        {
            --retVal;
        }
    }
    //fuelAirRatios
    pidName = obd::PidNames::OxySensor01;
    for(uint8_t i = 0; i < 8; i++){
      currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName+(unsigned char)i);
      if (baseLayer->isValidPID(currentPid))
      {
          pidArray[retVal].pid = currentPid;
          pidArray[retVal++].value = getValueOfPid(currentPid, queryState);
          if (queryState == false) //reading of pid value didn't work, neglect last read values
          {
            --retVal;
          }
      }
    }
    pidName = obd::PidNames::OxySensor11;
    for(uint8_t i = 0; i < 8; i++){
      currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName+(unsigned char)i);
      if (baseLayer->isValidPID(currentPid))
      {
          pidArray[retVal].pid = currentPid;
          pidArray[retVal++].value = getValueOfPid(currentPid, queryState);
          if (queryState == false) //reading of pid value didn't work, neglect last read values
          {
            --retVal;
          }
      }
    }
    
    //Serial.println(retVal);
    if (retVal > maxLengthPidArray)
    {
        return -1; //something happend that should not be possible
    }
    else
    {
        return (retVal < 0) ? 0 : retVal; //minimum is 0
    }
}

/*!
This function get the values of the normal pids via obd. It writes the pairs (pid, value) into the pid array.
@return -1 if an error occured, 0 if nothing was read and everything >0 determines the amount of pids in the array.
*/
char ObdDevice::updateNormalPids()
{
    //>>>>>>>>normal pids<<<<<<<<<<<<
    unsigned char retVal = 0;
    bool queryState = false;
    obd::PidNames pidName;

    pidName = obd::PidNames::EngineCoolantTemp;
    char currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        pidArray[retVal].pid = currentPid;
        pidArray[retVal++].value = getValueOfPid(currentPid, queryState);
        if (queryState == false) //reading of pid value didn't work, neglect last read values
        {
            --retVal;
        }
    }

    pidName = obd::PidNames::RunTimeSineEngineStart;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        pidArray[retVal].pid = currentPid;
        pidArray[retVal++].value = getValueOfPid(currentPid, queryState);
        if (queryState == false) //reading of pid value didn't work, neglect last read values
        {
            --retVal;
        }
    }

    pidName = obd::PidNames::AbsBarometricPressure;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        pidArray[retVal].pid = currentPid;
        pidArray[retVal++].value = getValueOfPid(currentPid, queryState);
        if (queryState == false) //reading of pid value didn't work, neglect last read values
        {
            --retVal;
        }
    }

    pidName = obd::PidNames::AmbientAirTemp;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        pidArray[retVal].pid = currentPid;
        pidArray[retVal++].value = getValueOfPid(currentPid, queryState);
        if (queryState == false) //reading of pid value didn't work, neglect last read values
        {
            --retVal;
        }
    }

    pidName = obd::PidNames::EngineOilTemp;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        pidArray[retVal].pid = currentPid;
        pidArray[retVal++].value = getValueOfPid(currentPid, queryState);
        if (queryState == false) //reading of pid value didn't work, neglect last read values
        {
            --retVal;
        }
    }

    if (retVal > maxLengthPidArray)
    {
        return -1; //something happend that should not be possible
    }
    else
    {
        return (retVal < 0) ? 0 : retVal; //minimum is 0
    }
}

/*!
This function get the values of the slow pids via obd. It writes the pairs (pid, value) into the pid array.
@return -1 if an error occured, 0 if nothing was read and everything >0 determines the amount of pids in the array.
*/
char ObdDevice::updateSlowPids()
{
    //>>>>>>>slow pids<<<<<<<<<<<
    unsigned char retVal = 0;
    bool queryState = false;
    obd::PidNames pidName;

    pidName = obd::PidNames::DistTraveledWithMalfuncIndicaLamp;
    unsigned char currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        pidArray[retVal].pid = currentPid;
        pidArray[retVal++].value = getValueOfPid(currentPid, queryState);
        if (queryState == false) //reading of pid value didn't work, neglect last read values
        {
            --retVal;
        }
    }

    pidName = obd::PidNames::FuelTankLvlInput;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        pidArray[retVal].pid = currentPid;
        pidArray[retVal++].value = getValueOfPid(currentPid, queryState);
        if (queryState == false) //reading of pid value didn't work, neglect last read values
        {
            --retVal;
        }
    }

    if (retVal > maxLengthPidArray)
    {
        return -1; //something happend that should not be possible
    }
    else
    {
        return (retVal < 0) ? 0 : retVal; //minimum is 0
    }
}

void ObdDevice::identifyMaxPidArrayLength()
{
    unsigned char tempCnt = 0;
    obd::PidNames pidName;
    //>>>>>>>very fast pids<<<<<<<
    pidName = obd::PidNames::EngineRpm;
    unsigned char currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        ++tempCnt;
    }

    pidName = obd::PidNames::VehicleSpeed;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        ++tempCnt;
    }

    pidName = obd::PidNames::RelativAcceleratorPedalPos;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        ++tempCnt;
    }

    pidName = obd::PidNames::EngineFuelRate;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        ++tempCnt;
    }

    pidName = obd::PidNames::DriverTorqueDemandEngine;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        ++tempCnt;
    }

    pidName = obd::PidNames::ActualTorqueEngine;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        ++tempCnt;
    }

    pidName = obd::PidNames::EngineTorqueRef;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        ++tempCnt;
    }
    //Consumption addition
    pidName = obd::PidNames::EngineLoad;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        ++tempCnt;
    }
    pidName = obd::PidNames::MAFAirFlow;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        ++tempCnt;
    }
    //fuelAirRatios
    pidName = obd::PidNames::OxySensor01;
    for(uint8_t i = 0; i < 8; i++){
      currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName + (unsigned char)i);
      if(baseLayer->isValidPID(currentPid)){
        ++tempCnt;
      }
    }
    pidName = obd::PidNames::OxySensor11;
    for(uint8_t i = 0; i < 8; i++){
      currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName + (unsigned char)i);
      if(baseLayer->isValidPID(currentPid)){
        ++tempCnt;
      }
    }

    maxLengthPidArray = (tempCnt > maxLengthPidArray) ? tempCnt : maxLengthPidArray;

    //>>>>>>>>normal pids<<<<<<<<<<<<
    tempCnt = 0;
    pidName = obd::PidNames::EngineCoolantTemp;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        ++tempCnt;
    }

    pidName = obd::PidNames::RunTimeSineEngineStart;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        ++tempCnt;
    }

    pidName = obd::PidNames::AbsBarometricPressure;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        ++tempCnt;
    }

    pidName = obd::PidNames::AmbientAirTemp;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        ++tempCnt;
    }

    pidName = obd::PidNames::EngineOilTemp;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        ++tempCnt;
    }
    maxLengthPidArray = (tempCnt > maxLengthPidArray) ? tempCnt : maxLengthPidArray;

    //>>>>>>>slow pids<<<<<<<<<<<
    tempCnt = 0;
    pidName = obd::PidNames::DistTraveledWithMalfuncIndicaLamp;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        ++tempCnt;
    }

    pidName = obd::PidNames::FuelTankLvlInput;
    currentPid = pgm_read_byte_near(obd::PidNumbers+(unsigned char)pidName);
    if (baseLayer->isValidPID(currentPid))
    {
        ++tempCnt;
    }
    maxLengthPidArray = (tempCnt > maxLengthPidArray) ? tempCnt : maxLengthPidArray;
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

ourTypes::pidData* ObdDevice::getPidArray()
{
    return pidArray;
}


/*!
This function reads out the diagnostic trouble codes that currently are in the system. If less than 10 are read, there are not more,
if 10 are read there could be more. Every diagnostic trouble code is represented by 5 digits. First is a letter follwod by 4 digits.
With freematic library only the digits are available. The category letter is not available.
@return A vector of pidData. Every pidData element represents a trouble code. If nullptr is returned something went wrong.
*/
ourTypes::dtcData* ObdDevice::getDiagnositcTroubleCodes(unsigned char& amount)
{
    amount = 0;
    //hier ist eine maximale Anzahl an Versuchen nicht sinvoll, weil wenn kein Fehler vorliegt 0 zurück gegeben wird, ebenso wenn steuergerät nicht verfügbar ist
    uint16_t* readCodes = new uint16_t[ourTypes::MAXTROUBLECODES];

    if (readCodes == nullptr)
    {
        return nullptr;
    }

    ourTypes::dtcData* returnVal = nullptr;

    unsigned char readTroubleCodesCount = baseLayer->readDTC(readCodes, ourTypes::MAXTROUBLECODES);
//    Serial.print("I read #"), Serial.print(readTroubleCodesCount, HEX), Serial.println("# codes");
    if (readTroubleCodesCount == 0)
    {
//        Serial.println("noDTC");
        returnVal = nullptr;
    }
    else
    {
//        Serial.println("DTCs");
        returnVal = new ourTypes::dtcData[readTroubleCodesCount];
        for (int i=0; i<readTroubleCodesCount; ++i)
        {
            if (readCodes[i] != 0)
            {
                returnVal[amount] = readCodes[i];
                ++amount;
            }
        }
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
    //todo: klären wie wir es jetzt machen zwecks abprüfen (alte vs. neue vin)
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

bool ObdDevice::isPidValid(unsigned char pid)
{
    return baseLayer->isValidPID(pid);
}

bool ObdDevice::getClamp15State()
{
    return Clamp15;
}






}//end namespace obd
