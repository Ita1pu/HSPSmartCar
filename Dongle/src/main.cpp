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
#include <bluetooth.h>

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
bluetooth::BtUploader bt_uploader;

persistence::stdRetVal success = 0;
ProgrammMode currentMode;

ourTypes::pidData* pidCollection;

char in[10];
//btLog function
inline void btLog(uint16_t pid, uint32_t value){
  Serial.print('#');
  Serial.print(pid);
  Serial.print(':');
  Serial.print(value);
  Serial.print(';');
}

void setup()
{
  Serial.begin(SERIAL_BAUD_RATE);
  file_system.init(&SD);
  Serial.println("After file system");
  p.init(&locSrv, &file_system);
  Serial.println("After persistence");
  bt_uploader.init(&p);
  Serial.println("After bt_uploader");
}

void loop()
{
  bt_uploader.upload_size = 0;
  bt_uploader.read_upload_size_and_date();
  Serial.println(bt_uploader.upload_size);
  bt_uploader.upload_bt();
  delay(500);
}
