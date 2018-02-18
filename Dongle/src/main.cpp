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

#define CLOCK_TIMER_NR 2
#define FLAG_TIMER_NR 1
#define LOOP_DURATION 200
#define SERIAL_BAUD_RATE 115200L
#define GPS_BAUD_RATE 115200L


COBDSPI coproc;
AccReader* accSensor;
gps::LocationTimeService locSrv = gps::LocationTimeService(&coproc);

obd::ObdDevice* obdDev;

ourTypes::vid current_vid;
//SDLib::SDClass SD;
//persistence::Vid_mapper mapper;
//persistence::File_System_Handler file_system;
//persistence::Persistence p;

persistence::stdRetVal success = 0;
ProgrammMode currentMode;

std::vector<ourTypes::pidData>* vFastPids;
std::vector<ourTypes::pidData>* fastPids;
std::vector<ourTypes::pidData>* normalPids;
std::vector<ourTypes::pidData>* slowPids;

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
      Serial.print(F(" S"));
      Serial.print(freeMemory());
    }
    //Initialize GPS Receiver
    success = locSrv.Initialize(GPS_BAUD_RATE);
    if(success != 0x00){
      do{
        delay(500);
        locSrv.RenewGPSData();
        Serial.print(F(" G"));
        Serial.print(freeMemory());
      //wait for GPS signal to be sane; sat Counter must be in [4; 14] which are the theoretical numbers of visible navigation satellites
    }while(locSrv.GetSat() < 4 || locSrv.GetSat() > 14);
    }else{
      Serial.print(F(" Gf"));
      Serial.print(freeMemory());
    }
    //initialize OBD reader
    obdDev = new obd::ObdDevice(&coproc);
    success = obdDev->initialize();
    if(success == 0x01){
      //obd init succeeded
      char* tmpVid = obdDev->getVehicleIdentificationNumber();
      //transfer vid to struct
      unsigned char zeroCt = 0;
      for(int i = 0; i < ourTypes::lengthOfVehicleIdentificationNumber; i++){
        current_vid.x[i] = tmpVid[i];
        if(tmpVid[i] == 0x00)
          zeroCt++;
      }
      if(zeroCt >= lengthOfVehicleIdentificationNumber){
        current_vid = {'0', 'A', '1', 'B', '2', 'C', '3', 'D', '4', 'E', '5', 'F', '6', '7', '8', '9', '0'};
      }
      vFastPids = obdDev->getVeryFastPids();
      fastPids = obdDev->getFastPids();
      normalPids = obdDev->getNormalPids();
      slowPids = obdDev->getSlowPids();
      Serial.print(F(" O"));
      Serial.print(freeMemory());
    }else{
      Serial.print(F(" Of"));
      Serial.print(freeMemory());
    }
    //initialize persistence layer
    /*file_system.init(&SD);
    mapper.initialize(&current_vid);
    p.init(&current_vid, clck, &mapper, &file_system);
    success = p.GetInitStatus();
    if(success == NO_ERROR || success == NEW_LOGGING_FILE_CREATED){
      Serial.println(F(" P"));
      Serial.print(freeMemory());
      //Maybe sleep mode
    }else{
      Serial.print(F(" Pf"));
      Serial.print(freeMemory());
    }
    //TODO: Log Category E
    bool pidSucc = false;
    int32_t pidRetVal = 0;
    //EthanolPercent
    pidRetVal = obdDev->getValueOfPid(obd::EthanolPercent, pidSucc);
    if(pidSucc){
      p.create_logging_entry(clck->GetEpochMs(), obd::EthanolPercent, pidRetVal);
    }
    //TODO: Log Category F
    //FuelType
    pidRetVal = obdDev->getValueOfPid(obd::FuelType, pidSucc);
    if(pidSucc){
      p.create_logging_entry(clck->GetEpochMs(), obd::FuelType, pidRetVal);
    }*/
        //initialize flag timer for Main loop
        locSrv.StartFlagTimer();
}

void loop()
{
  //TODO: Check if bluetooth is connected and check for mode
  if(currentMode.mode == UPLOAD){
    uploadBT();
    delay(200);
    //when finished uploading
    //currentMode.mode == LOGGING;
  }else{
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
      if(!obdDev->updateVeryFastPids()){
        Serial.print(F(" VFPf"));
        //logData(vFastPids, &currEpo);
      }
      if(accSensor->GetAccelerationAxis(0)*1000 > 10000){
        Serial.print(F(" ACCf"));
      }
      if(accSensor->GetAccelerationAxis(1)*1000 > 10000){
        Serial.print(F(" ACCf"));
      }
      if(accSensor->GetAccelerationAxis(2)*1000 > 10000){
        Serial.print(F(" ACCf"));
      }
      //push acceleration values into the pid vector; the values are given as 1/1000 of gravitational acceleration
      //p.create_logging_entry(currEpo, obd::AccelXAxis, accSensor->GetAccelerationAxis(0)*1000);
      //p.create_logging_entry(currEpo, obd::AccelYAxis, accSensor->GetAccelerationAxis(1)*1000);
      //p.create_logging_entry(currEpo, obd::AccelZAxis, accSensor->GetAccelerationAxis(2)*1000);

      //The values for the module comparison are used to not poll all pid categories at once when counter = 0
      //log fast / Category B
      if(currentMode.currentLoopCount%5 == 1){
        if(locSrv.RenewGPSData()){
          //log GPS data
          int32_t currLat = locSrv.GetLatitude();
          if(currLat < 100){
            Serial.print(currLat);
          }
          //p.create_logging_entry(currEpo, obd::GpsLatitude, currLat);
          int32_t currLon = locSrv.GetLongitude();
          if(currLat < 100){
            Serial.print(currLon);
          }
          //p.create_logging_entry(currEpo, obd::GpsLongitude, currLon);
        }
        //log fast OBD data
        if(!obdDev->updateFastPids()){
          //logData(fastPids, &currEpo);
          Serial.print(F(" FPf"));
        }
      }

      //log normal / Category C
      if(currentMode.currentLoopCount%50 == 2){
        if(!obdDev->updateNormalPids()){
          //logData(normalPids, &currEpo);
          Serial.print(F(" NPf"));
        }
      }

      //logSlowest / Category D
      if(currentMode.currentLoopCount%1500 == 3){
        if(!obdDev->updateSlowPids()){
          //logData(slowPids, &currEpo);
          Serial.print(F(" SPf"));
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
      Serial.println();
    }
  }
}

/*void logData(std::vector<ourTypes::pidData>* pids, uint64_t* msEpoch){
  for(auto &currTup : *pids){
    p.create_logging_entry(*msEpoch, currTup.pid, currTup.value);
  }
}*/

void uploadBT(){
  return;
}
