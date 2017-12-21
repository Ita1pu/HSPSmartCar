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
    accSensor = new AccReader();
    success = accSensor->Initialize();
    if(!success){
      Serial.println("Init failed!");
    }
}

/*void loop()
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
      Serial.println((uint32_t)clck->GetEpochMs());
    }
    //delay(1000);
}
*/


long int cpt=0;
// Main loop, read and display data
void loop()
{

  // _______________
  // ::: Counter :::

  // Display data counter
  Serial.print (cpt++,DEC);
  Serial.print ("\t");



  // ____________________________________
  // :::  accelerometer and gyroscope :::

  // Read accelerometer and gyroscope


  // Create 16 bits values from 8 bits data

  // Accelerometer
  float ax=accSensor->GetAccelerationAxis(0);
  float ay=accSensor->GetAccelerationAxis(1);
  float az=accSensor->GetAccelerationAxis(2);

  // Gyroscope
  float gx=accSensor->GetAngle(0);
  float gy=accSensor->GetAngle(1);
  float gz=accSensor->GetAngle(2);

    // Display values

  // Accelerometer
  Serial.print("ax: ");
  Serial.print (ax);
  Serial.print ("\tay: ");
  Serial.print (ay);
  Serial.print ("\taz: ");
  Serial.print (az);
  Serial.print ("\tgx: ");

  // Gyroscope
  Serial.print (gx);
  Serial.print ("\tgy: ");
  Serial.print (gy);
  Serial.print ("\tgz: ");
  Serial.print (gz);
  Serial.print ("\t");



  // End of line
  Serial.println("");
  delay(2000);
}
