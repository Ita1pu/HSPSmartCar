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



persistence::Persistence *p = malloc(sizeof(persistence::Persistence));
vid_mapper::Vid_mapper *mapper = malloc(sizeof(vid_mapper::Vid_mapper));
const ourTypes::vid current_vid = {'A','1','B','2','C','3','D','4','E','5','F','6',
                             'G','7','H','8','I',};
uint32_t current_time = 0xDEADBEEF;

void setup()
{
    mapper = new vid_mapper::Vid_mapper(&current_vid);
    p = new persistence::Persistence(&current_vid, current_time, mapper);
}

void loop()
{

}
