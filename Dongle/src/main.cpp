#include <Arduino.h>
#include <stdio.h>

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
#define SERIAL_BAUD_RATE 9600
#define GPS_BAUD_RATE 115200L

LocationService* locSrv = 0;
Clock* clck = 0;
COBDSPI coproc;
uint8_t flags;
persistence::Persistence *p;
persistence::Vid_mapper *mapper;
persistence::File_System_Handler *file_system;
const ourTypes::vid current_vid = {'A','1','B','2','C','3','D','4','E','5','F','6',
                             'G','7','H','8','I',};
uint32_t current_time = 0xDEADBEEF;

bool success = false;
void setup()
{
    Serial.begin(SERIAL_BAUD_RATE);
    SDLib::SDClass *SD = new SDClass();
    file_system = new persistence::File_System_Handler(SD);
    mapper = new persistence::Vid_mapper(&current_vid);
    p = new persistence::Persistence(&current_vid, current_time, mapper, file_system);
    // SD->begin(SD_CS_PIN);
    // SD->mkdir("Hallo");
    // File TestFile = SD->open("Hallo/test.txt", FILE_WRITE);

    // if (TestFile)
    // {
    //     TestFile.println("Test");
    //     TestFile.close();
    //   Serial.println("Sucess");
    // }
    // else
    // {
    //     Serial.println("Fail");
    // }

    coproc.begin();
    locSrv = new LocationService(&coproc);

    clck = new Clock(&coproc);
    Serial.println("Init started!");
    if(locSrv->Initialize(GPS_BAUD_RATE)){
      Serial.println("Initialization done!\n");
    }else{
      Serial.println("Init failed!");
    }
    do{
      locSrv->RenewGPSData();
      delay(500);
      Serial.println("Clock waiting!");
    }while(!clck->Initialize(locSrv, CLOCK_TIMER_NR));

    Serial.println("Clock initialized!");

    if(clck->SetTimer(FLAG_TIMER_NR, 1500, &flags)){
      Serial.println("Flag timer set!");
    }
    Serial.println(clck->GetDate());
}

void loop()
{
    // put your main code here, to run repeatedly:
    if(flags){
      flags = 0;
      locSrv->RenewGPSData();
      Serial.println("Tick");
      Serial.print("Latitude: ");
      Serial.println(locSrv->GetLatitude());
      Serial.print("Longitude: ");
      Serial.println(locSrv->GetLongitude());
      Serial.print("Sat-Ctr: ");

      Serial.println(locSrv->GetSat());
      Serial.print("Time :");
      Serial.println(clck->GetTime());
    }
    //delay(1000);
}
