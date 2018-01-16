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
#define SERIAL_BAUD_RATE 115200L
#define GPS_BAUD_RATE 115200L

AccReader* accSensor;
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
    coproc.begin();
    locSrv = new LocationService(&coproc);
    clck = new Clock(&coproc);
    accSensor = new AccReader();

    flags = 0;
    Serial.println("Init start!");
    success = accSensor->Initialize();
    /*success = locSrv->Initialize(GPS_BAUD_RATE);
    if(success){
      int ctr = 0;
      do{
        locSrv->RenewGPSData();
        ++ctr;
        delay(500);
        Serial.print(locSrv->GetSat());
        Serial.println(" Clock waiting");
      }while(!clck->Initialize(locSrv, CLOCK_TIMER_NR));
    }
    //success = clck->SetTimer(FLAG_TIMER_NR, 2000, &flags);*/
    if(!success){
      Serial.println("Init failed!");
    }else{
      accSensor->Calibrate(true, true);
    }
}

float acc[3];
float gyr[3];
float mag[3];
long int cpt = 0;
void loop()
{
  for(int i = 0; i < 3; ++i){
    acc[i] = accSensor->GetAccelerationAxis(i);
    gyr[i] = accSensor->GetAngle(i);
    mag[i] = accSensor->GetMagnet(i);
  }
  Serial.print (cpt++,DEC);
  Serial.print ("\t");
  // Accelerometer
  Serial.print (acc[0],DEC);
  Serial.print ("\t");
  Serial.print (acc[1],DEC);
  Serial.print ("\t");
  Serial.print (acc[2],DEC);
  Serial.print ("\t");

  // Gyroscope
  Serial.print (gyr[0],DEC);
  Serial.print ("\t");
  Serial.print (gyr[1],DEC);
  Serial.print ("\t");
  Serial.print (gyr[2],DEC);
  Serial.print ("\t");

  //Magnetometer
  Serial.print (mag[0],DEC);
  Serial.print ("\t");
  Serial.print (mag[1],DEC);
  Serial.print ("\t");
  Serial.print (mag[2],DEC);
  Serial.println ("\t");

    /*unsigned long start;
    unsigned long stop;
    unsigned long total;
    // put your main code here, to run repeatedly:
    //if(flags){
//int32_t tmp;
      //flags = 0;
      start = millis();
        for(int i = 0; i < 1000; ++i){
          //tmp = locSrv->GetLatitude();
          accSensor->Calibrate(true,true);
        }
      stop = millis();
      total = stop-start;
      //Serial.print(tmp);
      Serial.print(" Time: ");
      Serial.println(total);
    //}*/
    delay(100);

}
