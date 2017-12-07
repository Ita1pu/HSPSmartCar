#include <Arduino.h>

#include <acc_reader.h>
#include <bluetooth.h>
#include <clock.h>
#include <gps.h>
#include <obd.h>
#include <persistence.h>

#define CLOCK_TIMER_NR 2
#define FLAG_TIMER_NR 1
#define SERIAL_BAUD_RATE 9600
#define GPS_BAUD_RATE 115200L

LocationService* locSrv = 0;
Clock* clck = 0;
COBDSPI coproc;
uint8_t flags;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(SERIAL_BAUD_RATE);
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
