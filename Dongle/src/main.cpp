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
#include <SD.h>

persistence::Persistence *p;
persistence::Vid_mapper *mapper;
persistence::File_System_Handler *file_system;
const ourTypes::vid current_vid = {'A','1','B','2','C','3','D','4','E','5','F','6',
                             'G','7','H','8','I',};
uint32_t current_time = 0xDEADBEEF;

bool success = false;
void setup()
{
    Serial.begin(9600);
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
}

void loop()
{
    //Serial.println("Hallo");
    delay(500);
    //Serial.println(p->GetInitStatus());
    // Serial.println("Hu!");
    // delay(500);
}
