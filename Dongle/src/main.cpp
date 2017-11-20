#include <Arduino.h>
#include <stdio.h>

#include <acc_reader.h>
#include <bluetooth.h>
#include <clock.h>
#include <gps.h>
#include <obd.h>
#include <persistence.h>

Persistence *p;
char str[100];
char stat = 0;
print(char *numstring, int number)
{
  Serial.print(numstring);
  sprintf(str, "%x", number);
  Serial.println(number);
}
void setup()
{
    p = new Persistence();
    Serial.begin(9600);
    print("SPI_MOSI_PIN: ", SPI_MOSI_PIN);
    print("SPI_MISO_PIN: ", SPI_MISO_PIN);
    print("SPI_SCK_PIN: ", SPI_SCK_PIN);
    print("SD_CHIP_SELECT_PIN: ", SD_CHIP_SELECT_PIN);
    Serial.println("Success");
    // put your setup code here, to run once:
}

void loop()
{
    // if (p->getStatus())
    // {
    //   p->stop();
    // }
    // else
    // {
    //   p->init();
    // }
    // sprintf(str, "%x", p->getStatus());
    // Serial.println(str);
    // delay(500);
    // put your main code here, to run repeatedly:
}
