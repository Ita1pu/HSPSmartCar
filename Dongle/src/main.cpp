#include <Arduino.h>
#include <stdio.h>

#include <acc_reader.h>
#include <bluetooth.h>
#include <gps.h>
#undef abs
#include <obd.h>
#include <persistence.h>
#include <types.h>
#include <vid_mapper.h>
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
persistence::Vid_mapper mapper;
persistence::File_System_Handler file_system;
persistence::Persistence p;

persistence::stdRetVal success = 0;
ProgrammMode currentMode;

ourTypes::pidData* pidCollection;

//function to upload Data to smartphone
void uploadBT();

void setup()
{
  //Instantiate Objects
    Serial.begin(SERIAL_BAUD_RATE);
    coproc.begin();
    accSensor = new AccReader();
  //Initialize global variables
    currentMode.bluetooth = false;
    currentMode.mode = LOGGING;
    currentMode.currentLoopCount = 0;
    Serial.println(F("Init start!"));
    //Init Acceleration Sensor
    success = accSensor->Initialize();
    if(success != 0x00){
      accSensor->Calibrate(false,true);
    }
    //Initialize GPS Receiver
    success = locSrv.Initialize(GPS_BAUD_RATE);
    if(success != 0x00){
      do{
        delay(500);
        locSrv.RenewGPSData();
      //wait for GPS signal to be sane; sat Counter must be in [4; 14] which are the theoretical numbers of visible navigation satellites
      }while(locSrv.GetSat() < 4 || locSrv.GetSat() > 14);
    }
    //initialize OBD reader
    obdDev = new obd::ObdDevice(&coproc);
    success = obdDev->initialize();
    if(success == 0x01){
      //obd init succeeded
      char* tmpVid = obdDev->getVehicleIdentificationNumber();
      //transfer vid to struct
      if(tmpVid != nullptr){
        for(int i = 0; i < ourTypes::lengthOfVehicleIdentificationNumber; i++){
          current_vid.x[i] = tmpVid[i];
        }
      }else{
          current_vid = {'0', 'A', '1', 'B', '2', 'C', '3', 'D', '4', 'E', '5', 'F', '6', '7', '8', '9', '0'};
      }
      pidCollection = obdDev->getPidArray();
    }
    //initialize persistence layer
    file_system.init(&SD);
    mapper.initialize(&current_vid);
    p.init(&current_vid, &locSrv, &mapper, &file_system);
    success = p.GetInitStatus();
    if(success == NO_ERROR || success == NEW_LOGGING_FILE_CREATED){
      //sleep mode
      do{
        //endless loop
        delay(0xFFFFFFFF);
        coproc.uninit();
        coproc.enterLowPowerMode();
      }while(true);
    }
    //TODO: Log Category E
    bool pidSucc = false;
    int32_t pidRetVal = 0;
    //EthanolPercent
    pidRetVal = obdDev->getValueOfPid(obd::EthanolPercent, pidSucc);
    if(pidSucc){
      p.create_logging_entry(locSrv.GetEpochMs(), obd::EthanolPercent, pidRetVal);
    }
    //TODO: Log Category F
    //FuelType
    pidRetVal = obdDev->getValueOfPid(obd::FuelType, pidSucc);
    if(pidSucc){
      p.create_logging_entry(locSrv.GetEpochMs(), obd::FuelType, pidRetVal);
    }
        //initialize flag timer for Main loop
        locSrv.StartFlagTimer();
}

void loop()
{
  //TODO: Check if bluetooth is connected and check for mode
  if(currentMode.mode == UPLOAD){
    //uploadBT();
    delay(200);
    //when finished uploading
    currentMode.mode = LOGGING;
  }else if(currentMode.mode == LOGGING){
    //check if timer has reached its limit
    if(timerFlags){
      Serial.print(F(" L"));
      Serial.print(freeMemory());
      //reset flags
      timerFlags = 0;
      //uint64_t currEpo = clck->GetEpochMs();
      Serial.print(currentMode.currentLoopCount);
      Serial.print(": ");
      Serial.print(locSrv.GetTime());

      //log very fast / Category A
      char pidLen = obdDev->updateVeryFastPids();
      if(pidLen > 0){
        for(char i = 0; i < pidLen; i++){
          p.create_logging_entry(locSrv.GetEpochMs(), pidCollection[i].pid, pidCollection[i].value);
        }
      }
      //log acceleration values; the values are given as 1/1000 of gravitational acceleration
      p.create_logging_entry(locSrv.GetEpochMs(), obd::AccelXAxis, accSensor->GetAccelerationAxis(0)*1000);
      p.create_logging_entry(locSrv.GetEpochMs(), obd::AccelYAxis, accSensor->GetAccelerationAxis(1)*1000);
      p.create_logging_entry(locSrv.GetEpochMs(), obd::AccelZAxis, accSensor->GetAccelerationAxis(2)*1000);

      //The values for the module comparison are used to not poll all pid categories at once when counter = 0
      //log fast / Category B
      if(currentMode.currentLoopCount%5 == 1){
        if(locSrv.RenewGPSData()){
          //log GPS data
          int32_t currLat = locSrv.GetLatitude();
          if(currLat < 100){
            Serial.print(currLat);
          }
          p.create_logging_entry(locSrv.GetEpochMs(), obd::GpsLatitude, currLat);
          int32_t currLon = locSrv.GetLongitude();
          if(currLat < 100){
            Serial.print(currLon);
          }
          p.create_logging_entry(locSrv.GetEpochMs(), obd::GpsLongitude, currLon);
        }
        //log fast OBD data
        //no fast pids selected
      }

      //log normal / Category C
      if(currentMode.currentLoopCount%50 == 2){
        pidLen = obdDev->updateNormalPids();
        if(pidLen > 0){
          for(char i = 0; i < pidLen; i++){
            p.create_logging_entry(locSrv.GetEpochMs(), pidCollection[i].pid, pidCollection[i].value);
          }
        }
      }

      //logSlowest / Category D
      if(currentMode.currentLoopCount%1500 == 3){
        pidLen = obdDev->updateSlowPids();
        if(pidLen > 0){
          for(char i = 0; i < pidLen; i++){
            p.create_logging_entry(locSrv.GetEpochMs(), pidCollection[i].pid, pidCollection[i].value);
          }
        }
      }

      //TODO: check BT for LOGGING
      if(currentMode.bluetooth == true){
        //write to bluetooth
      }
      if(!obdDev->getClamp15State()){
        //go to sleep for 2 min if clamp 15 is not closed
        currentMode.mode = SLEEP;
        p.close_logging_file();
        coproc.enterLowPowerMode();
      }
      currentMode.currentLoopCount++;
      //cycle every 5 minutes
      currentMode.currentLoopCount%=1500;
      Serial.println();
    }
  }else{
    //currentMode.mode == SLEEP
    delay(INACT_TIME_MS);
    coproc.leaveLowPowerMode();
    delay(50);
    if(obdDev->getClamp15State()){
      //if Car is still off
      coproc.enterLowPowerMode();
    }else{
      currentMode.currentLoopCount = 0;
      currentMode.mode = LOGGING;
      p.init(&current_vid, &locSrv, &mapper, &file_system);
    }
  }
}

void uploadBT(){
  return;
}
