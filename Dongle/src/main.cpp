#include <Arduino.h>
#include <stdio.h>

#include <mainHelpers.h>
#include <acc_reader.h>
#include <bluetooth.h>
#include <clock.h>
#include <gps.h>
#include <obd.h>
#include <persistence.h>
#include <types.h>
#include <vid_mapper.h>
#include <FreematicsSD.h>

#define CLOCK_TIMER_NR 2
#define FLAG_TIMER_NR 1
#define LOOP_DURATION 100
#define SERIAL_BAUD_RATE 115200L
#define GPS_BAUD_RATE 115200L

AccReader* accSensor;
LocationService* locSrv = 0;
Clock* clck = 0;
COBDSPI coproc;
uint8_t flags;

SDLib::SDClass *SD;
persistence::Persistence *p;
persistence::Vid_mapper *mapper;
persistence::File_System_Handler *file_system;
const ourTypes::vid current_vid = {'A','1','B','2','C','3','D','4','E','5','F','6',
                             'G','7','H','8','I',};

persistence::stdRetVal success = 0;
ProgrammMode currentMode;

//function to upload Data to smartphone
void uploadBT();
bool carRunning();

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
    Serial.println("Init start!");
    //Init Acceleration Sensor
    success = accSensor->Initialize();
    if(success != 0x00){
      accSensor->Calibrate(false,true);
    }
    //Initialize GPS Receiver
    success = locSrv->Initialize(GPS_BAUD_RATE);
    if(success != 0x00){
      int ctr = 0;
      do{
        locSrv->RenewGPSData();
        ++ctr;
        delay(500);
        Serial.println(" Clock waiting");
      //wait for GPS signal to initialize clock
      }while(!clck->Initialize(locSrv, CLOCK_TIMER_NR));
    }
    //initialize flag timer for Main loop
    success = clck->SetTimer(FLAG_TIMER_NR, LOOP_DURATION, &flags);
    //initialize OBD reader

    //initialize persistence
    SD = new SDClass();
    file_system = new persistence::File_System_Handler(SD);
    mapper = new persistence::Vid_mapper(&current_vid);
    p = new persistence::Persistence(&current_vid, clck, mapper, file_system);
    success = p->GetInitStatus();
    //initialize Persistence Layer
    if(success == 0x00){
      Serial.println("Init failed!");
      //Maybe sleep mode
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
      int32_t currLat = locSrv->GetLatitude();
      int32_t currLon = locSrv->GetLongitude();
      int64_t currEpo = clck->GetEpochMs();
      int32_t currTim = clck->GetTime();
      int32_t currDat = clck->GetDate();
      //TODO:logFastest

      //TODO: logSecond
      switch (currentMode.currentLoopCount) {
        case 0:
        break;
        case 1:
        break;
        case 2:
        break;
      }

      //TODO: logSlowest
      if(currentMode.currentLoopCount%10 == 0){

      }
      p->create_logging_entry(currEpo, ,);

      //TODO: check BT for LOGGING
      if(currentMode.bluetooth == true){
        //write to bluetooth
      }
      if(!carRunning()){
        //go to sleep
      }
    }
  }

}
