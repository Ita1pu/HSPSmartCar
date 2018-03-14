#include <Arduino.h>
#include <stdio.h>

#include <acc_reader.h>
#include <bluetooth.h>
#include <gps.h>
#undef abs
#include <obd.h>
#include <persistence.h>
#include <types.h>
#include <FreematicsSD.h>
#include <globalConfig.h>

#define SERIAL_BAUD_RATE 115200L
#define GPS_BAUD_RATE 115200L
#define INACT_TIME_MS 120000u

COBDSPI coproc;
AccReader* accSensor;
gps::LocationTimeService locSrv = gps::LocationTimeService(&coproc);

obd::ObdDevice* obdDev;

ourTypes::vid current_vid;
SDLib::SDClass SD;
persistence::File_System_Handler file_system;
persistence::Persistence p;

persistence::stdRetVal success = 0;
ProgrammMode currentMode;

ourTypes::pidData* pidCollection;

//btLog function
inline void btLog(uint16_t pid, uint32_t value){
  Serial.print('#');
  Serial.print(pid);
  Serial.print(':');
  Serial.print(value);
  Serial.print(';');
}
//function to upload Data to smartphone
void uploadBT();

void setup()
{
  //Instantiate Objects
    Serial.begin(SERIAL_BAUD_RATE);
    coproc.begin();
  //Initialize global variables
    currentMode.mode = LOGGING;
    currentMode.currentLoopCount = 0;
    Serial.println(F("Init start!"));
    //Initialize GPS Receiver
    success = locSrv.Initialize(GPS_BAUD_RATE);
    if(success != 0x00){
      do{
        delay(500);
        locSrv.RenewGPSData();
      //wait for GPS signal to be sane; sat Counter must be in [4; 14] which are the theoretical numbers of visible navigation satellites
      }while(locSrv.GetSat() < 4 || locSrv.GetSat() > 14);
    }
    //initialize persistence layer
    file_system.init(&SD);
    p.init(&locSrv, &file_system);
    success = p.GetInitStatus();
    if(success != NO_ERROR && success != NEW_LOGGING_FILE_CREATED){
      //sleep mode
      do{
        //endless loop
        Serial.print(F("Init failed!"));
        delay(0xFFFFFFFF);
        coproc.uninit();
        coproc.enterLowPowerMode();
      }while(true);
    }
    //Init Acceleration Sensor
    accSensor = new AccReader();
    success = accSensor->Initialize();
    if(success != 0x00){
      accSensor->Calibrate(false,true);
    }
    //initialize OBD reader
    obdDev = new obd::ObdDevice(&coproc);
    do{
      success = obdDev->initialize();
      if(success != 0x01){
        //wait until car is started for the first time
        coproc.enterLowPowerMode();
        delay(INACT_TIME_MS);
        coproc.leaveLowPowerMode();
        delay(50);
      }else{
        break;
      }
    }while(true);//wait until car is started

    pidCollection = obdDev->getPidArray();

    //Log Category E
    bool pidSucc = false;
    int32_t pidRetVal = 0;
    //EthanolPercent
    pidRetVal = obdDev->getValueOfPid(obd::EthanolPercent, pidSucc);
    if(pidSucc){
      p.create_logging_entry(locSrv.GetEpochMs(), obd::EthanolPercent, pidRetVal);
    }
    //Log Category F
    //FuelType
    pidRetVal = obdDev->getValueOfPid(obd::FuelType, pidSucc);
    if(pidSucc){
      p.create_logging_entry(locSrv.GetEpochMs(), obd::FuelType, pidRetVal);
    }
    //Correction factors
    pidRetVal = obdDev->getValueOfPid(obd::TestEquipConf1, pidSucc);
    if(pidSucc){
      p.create_logging_entry(locSrv.GetEpochMs(), obd::TestEquipConf1, pidRetVal);
    }
    pidRetVal = obdDev->getValueOfPid(obd::TestEquipConf2, pidSucc);
    if(pidSucc){
      p.create_logging_entry(locSrv.GetEpochMs(), obd::TestEquipConf2, pidRetVal);
    }
    //initialize flag timer for Main loop
    locSrv.StartFlagTimer();
}

void loop()
{
  //TODO:check for mode
  if(currentMode.mode == UPLOAD){
    //uploadBT();
    delay(200);
    //when finished uploading
    currentMode.mode = LOGGING;
  }else if(currentMode.mode == LOGGING){
    //check if timer has reached its limit
    if(timerFlags){
      //reset flags
      timerFlags = 0;

      //log very fast / Category A
      char pidLen = obdDev->updateVeryFastPids();
      if(pidLen > 0){
        for(char i = 0; i < pidLen; i++){
          p.create_logging_entry(locSrv.GetEpochMs(), pidCollection[i].pid, pidCollection[i].value);
          //bluetooth logging
          if(pidCollection[i].pid == obd::EngineRpm || pidCollection[i].pid == obd::VehicleSpeed){
            btLog(pidCollection[i].pid, pidCollection[i].value);
          }
        }
      }
      //log acceleration values; the values are given as 1/1000 of gravitational acceleration
      p.create_logging_entry(locSrv.GetEpochMs(), obd::AccelXAxis, accSensor->GetAccelerationAxis(0)*1000);
      p.create_logging_entry(locSrv.GetEpochMs(), obd::AccelYAxis, accSensor->GetAccelerationAxis(1)*1000);
      p.create_logging_entry(locSrv.GetEpochMs(), obd::AccelZAxis, accSensor->GetAccelerationAxis(2)*1000);

      //The values for the module comparison are used to not poll all pid categories at once when counter = 0
      //log fast / Category B
      if(currentMode.currentLoopCount%3 == 1){
        if(locSrv.RenewGPSData()){
          //log GPS data
          p.create_logging_entry(locSrv.GetEpochMs(), obd::GpsLatitude, locSrv.GetLatitude());
          p.create_logging_entry(locSrv.GetEpochMs(), obd::GpsLongitude, locSrv.GetLongitude());
          p.create_logging_entry(locSrv.GetEpochMs(), obd::GpsAltitude, locSrv.GetAltitude());
        }
        //log fast OBD data
        //no fast pids selected
      }

      //log normal / Category C
      if(currentMode.currentLoopCount%30 == 2){
        pidLen = obdDev->updateNormalPids();
        if(pidLen > 0){
          for(char i = 0; i < pidLen; i++){
            p.create_logging_entry(locSrv.GetEpochMs(), pidCollection[i].pid, pidCollection[i].value);
            //btLog for temperatures
            if(pidCollection[i].pid == obd::EngineCoolantTemp || pidCollection[i].pid == obd::AmbientAirTemp || pidCollection[i].pid == obd::EngineOilTemp){
              btLog(pidCollection[i].pid, pidCollection[i].value);
            }
          }
        }
      }

      //logSlowest / Category D
      if(currentMode.currentLoopCount%600 == 3){
        pidLen = obdDev->updateSlowPids();
        if(pidLen > 0){
          for(char i = 0; i < pidLen; i++){
            p.create_logging_entry(locSrv.GetEpochMs(), pidCollection[i].pid, pidCollection[i].value);
          }
        }
      }

      if(!obdDev->getClamp15State()){
        //go to sleep for 2 min if clamp 15 is not closed
        currentMode.mode = SLEEP;
        Serial.print(F("Sl!"));
        locSrv.StopFlagTimer();
        locSrv.UnInit();
        obdDev->uninit();
        coproc.enterLowPowerMode();
      }
      currentMode.currentLoopCount++;
      //cycle every ~5 minutes
      currentMode.currentLoopCount%=600;
      Serial.println();
    }
  }else{
    //currentMode.mode == SLEEP
    delay(INACT_TIME_MS);
    coproc.leaveLowPowerMode();
    obdDev->initialize();
    Serial.println(F("Cs"));
    delay(50);
    if(!obdDev->getClamp15State()){
      //if Car is still off
      coproc.enterLowPowerMode();
    }else{
      currentMode.currentLoopCount = 0;
      currentMode.mode = LOGGING;
      bool success = locSrv.Initialize(GPS_BAUD_RATE);
      if(success != false){
        do{
          delay(500);
          locSrv.RenewGPSData();
        //wait for GPS signal to be sane; sat Counter must be in [4; 14] which are the theoretical numbers of visible navigation satellites
        }while(locSrv.GetSat() < 4 || locSrv.GetSat() > 14);
      }
      locSrv.StartFlagTimer();
      //EthanolPercent
      uint32_t pidRetVal = obdDev->getValueOfPid(obd::EthanolPercent, success);
      if(success){
        p.create_logging_entry(locSrv.GetEpochMs(), obd::EthanolPercent, pidRetVal);
      }
    }
  }
}

void uploadBT(){
  return;
}
