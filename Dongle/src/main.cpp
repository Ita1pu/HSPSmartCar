#include <Arduino.h>
#include <stdio.h>

#include <acc_reader.h>
#include <bluetooth.h>
#include <clock.h>
#include <gps.h>
#undef abs
#include <obd.h>
#include <persistence.h>
#include <types.h>
#include <vid_mapper.h>
#include <FreematicsSD.h>
#include <globalConfig.h>

#define CLOCK_TIMER_NR 2
#define FLAG_TIMER_NR 1
#define LOOP_DURATION 200
#define SERIAL_BAUD_RATE 115200L
#define GPS_BAUD_RATE 115200L


AccReader* accSensor;
LocationService* locSrv = 0;
Clock* clck = 0;
COBDSPI coproc;
uint8_t flags;

obd::ObdDevice* obdDev;

SDLib::SDClass *SD;
persistence::Persistence *p;
persistence::Vid_mapper *mapper;
persistence::File_System_Handler *file_system;
ourTypes::vid current_vid;

persistence::stdRetVal success = 0;
ProgrammMode currentMode;

std::vector<ourTypes::pidData>* vFastPids;
std::vector<ourTypes::pidData>* fastPids;
std::vector<ourTypes::pidData>* normalPids;
std::vector<ourTypes::pidData>* slowPids;

//function to upload Data to smartphone
void uploadBT();
void logData(std::vector<ourTypes::pidData>* pids, uint64_t* msEpoch);

void setup()
{
  //Instantiate Objects
    Serial.begin(SERIAL_BAUD_RATE);
    coproc.begin();
    locSrv = new LocationService(&coproc);
    clck = new Clock(&coproc);
    accSensor = new AccReader();
  //Initialize global variables
    flags = 0;
    currentMode.bluetooth = false;
    currentMode.mode = LOGGING;
    currentMode.currentLoopCount = 0;
    Serial.println(F("Init start!"));
    //Init Acceleration Sensor
    success = accSensor->Initialize();
    if(success != 0x00){
      accSensor->Calibrate(false,true);
      Serial.print(F(" S"));
      Serial.print(freeMemory());
    }
    //Initialize GPS Receiver
    success = locSrv->Initialize(GPS_BAUD_RATE);
    if(success != 0x00){
      int ctr = 0;
      do{
        locSrv->RenewGPSData();
        ++ctr;
        delay(500);
        Serial.print(F(" C"));
      //wait for GPS signal to initialize clock
      }while(!clck->Initialize(locSrv, CLOCK_TIMER_NR));
    }
    //initialize flag timer for Main loop
    success = clck->SetTimer(FLAG_TIMER_NR, LOOP_DURATION, &flags);
    //initialize OBD reader
    obdDev = new obd::ObdDevice(&coproc);
    success = obdDev->initialize();
    if(success == 0x01){
      //obd init succeeded
      char* tmpVid = obdDev->getVehicleIdentificationNumber();
      //transfer vid to struct
      for(int i = 0; i < ourTypes::lengthOfVehicleIdentificationNumber; i++){
        current_vid.x[i] = tmpVid[i];
      }
      vFastPids = obdDev->getVeryFastPids();
      fastPids = obdDev->getFastPids();
      normalPids = obdDev->getNormalPids();
      slowPids = obdDev->getSlowPids();
      Serial.print(F(" O"));
    }
    //initialize persistence layer
    SD = new SDClass();
    file_system = new persistence::File_System_Handler(SD);
    mapper = new persistence::Vid_mapper(&current_vid);
    p = new persistence::Persistence(&current_vid, clck, mapper, file_system);
    success = p->GetInitStatus();
    if(success == NO_ERROR || success == NEW_LOGGING_FILE_CREATED){
      Serial.println(F(" P"));
      //Maybe sleep mode
    }else{
      Serial.print(F(" Pf"));
    }
    //TODO: Log Category E
    bool pidSucc = false;
    int32_t pidRetVal = 0;
    //EthanolPercent
    pidRetVal = obdDev->getValueOfPid(obd::EthanolPercent, pidSucc);
    if(pidSucc){
      p->create_logging_entry(clck->GetEpochMs(), obd::EthanolPercent, pidRetVal);
    }
    //TODO: Log Category F
    //FuelType
    pidRetVal = obdDev->getValueOfPid(obd::FuelType, pidSucc);
    if(pidSucc){
      p->create_logging_entry(clck->GetEpochMs(), obd::FuelType, pidRetVal);
    }
}

void loop()
{
  //TODO: Check if bluetooth is connected and check for mode
  if(currentMode.mode == UPLOAD){
    uploadBT();
    delay(200);
    //when finished uploading
    currentMode.mode == LOGGING;
  }else{
    //check if timer has reached its limit
    if(flags){
      //reset flags
      flags = 0;
      uint64_t currEpo = clck->GetEpochMs();

      //log very fast / Category A
      if(obdDev->updateVeryFastPids()){
        logData(vFastPids, &currEpo);
      }
      //push acceleration values into the pid vector; the values are given as 1/1000 of gravitational acceleration
      p->create_logging_entry(currEpo, obd::AccelXAxis, accSensor->GetAccelerationAxis(0)*1000);
      p->create_logging_entry(currEpo, obd::AccelYAxis, accSensor->GetAccelerationAxis(1)*1000);
      p->create_logging_entry(currEpo, obd::AccelZAxis, accSensor->GetAccelerationAxis(2)*1000);

      //The values for the module comparison are used to not poll all pid categories at once when counter = 0
      //log fast / Category B
      if(currentMode.currentLoopCount%5 == 1){
        if(locSrv->RenewGPSData()){
          //log GPS data
          int32_t currLat = locSrv->GetLatitude();
          p->create_logging_entry(currEpo, obd::GpsLatitude, currLat);
          int32_t currLon = locSrv->GetLongitude();
          p->create_logging_entry(currEpo, obd::GpsLongitude, currLon);
        }
        //log fast OBD data
        if(obdDev->updateFastPids()){
          logData(fastPids, &currEpo);
        }
      }

      //log normal / Category C
      if(currentMode.currentLoopCount%50 == 2){
        if(obdDev->updateNormalPids()){
          logData(normalPids, &currEpo);
        }
      }

      //logSlowest / Category D
      if(currentMode.currentLoopCount%1500 == 3){
        if(obdDev->updateSlowPids()){
          logData(slowPids, &currEpo);
        }
      }

      //TODO: check BT for LOGGING
      if(currentMode.bluetooth == true){
        //write to bluetooth
      }
      if(!obdDev->getClamp15State()){
        //TODO: go to sleep if clamp 15 is not closed
      }
      currentMode.currentLoopCount++;
      //cycle every 5 minutes
      currentMode.currentLoopCount%=1500;
    }
  }
}

void logData(std::vector<ourTypes::pidData>* pids, uint64_t* msEpoch){
  for(auto &currTup : *pids){
    p->create_logging_entry(*msEpoch, currTup.pid, currTup.value);
  }
}

void uploadBT(){
  return;
}
