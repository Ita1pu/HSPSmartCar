#include <Arduino.h>
//#include <StandardCplusplus.h>

#include <acc_reader.h>
#include <bluetooth.h>
#include <clock.h>
#include <gps.h>
#include <obd.h>
#include <persistence.h>

#include <FreematicsBase.h>

//#include <vector>
//#include <utility.h>


void testCaseA ();
void testCaseB ();
void testCaseC ();
void testCaseE ();
void testCaseF ();


COBDSPI* baseLayer;
obd::ObdDevice* obdDev;
char testCase = 'a';

void setup()
{
    Serial.begin(9600);
    Serial.println("Started");

    //OBD_PROTOCOLS odbType = PROTO_KWP2000_FAST; //bei andi: PROTO_KWP2000_5KBPS
                                                // michi: PROTO_ISO_9141_2
                                                //franz: PROTO_ISO_9141_2


    baseLayer = new COBDSPI();
//    Serial.println("Got base layer*");

    obdDev = new obd::ObdDevice(baseLayer);
//    Serial.println("Got odb object");

    baseLayer->begin();
//    Serial.println("base layer begun");

    delay(1000);
    if (obdDev->initialize() == true)
    {
//        Serial.println("obd initialiezed");
    }
    else
    {
//        Serial.println("obd not initialized");
    }
}

void loop()
{
    if (Serial.available() > 0)
    {
        testCase = (char)Serial.read();
//        Serial.print("i got a "), Serial.println(testCase);
    }
//    Serial.print("In loop. Testcase: "), Serial.print(testCase), Serial.println();
    Serial.println(testCase);
    delay(1000);


    if(obdDev->getClamp15State() == false)
    {
        Serial.println("Clamp 15:f");
        baseLayer->reset();
        if (obdDev->initialize() == false)
        {
            Serial.println("restart:f");
        }
    }
    else
    {
        Serial.println("clamp15:t");
        switch (testCase)
        {

            case 'a'://getValueOfPid own class
                testCaseA();
                break;

            case 'b'://print valid pids
                testCaseB();
                break;

            case 'c'://update pid vector and give it out
                testCaseC();
                break;

            case 'e'://get diagnostic trouble codes
                testCaseE();
                break;

            case 'f'://get vehicle identification number own class
                testCaseF();
                break;

            default:
                Serial.println("-");
                break;
        }
    }

    delay(5000);
}

void testCaseA ()
{
    //    Serial.println("Read one pid");
    bool status = false;
    int value = 200;
    value = obdDev->getValueOfPid(0x20, status);
    if (status == true)
    {
        //        Serial.print("Read sucessfull EngineCoolant Temp");
        Serial.print("0x20 =");
        Serial.println(value);
    }
    else
    {
        Serial.println("-");
        //Serial.println(value);
    }
    delay(2000);
}

void testCaseB ()
{
    Serial.println("supported Pids:");
    for (unsigned char i=1; i<=0x7F; ++i)
    {
        if (obdDev->isPidValid(i) == true)
        {
            Serial.println(i, HEX);
        }
    }
    delay(10000);
}

void testCaseC ()
{
    //update pid vector and give it out
    //    Serial.println("update pid vectors and print them");
    ourTypes::pidData* pids = obdDev->getPidArray();
    char pidAmount = 0;


    pidAmount = obdDev->updateVeryFastPids();
    if (pidAmount > 0)
    {
        Serial.println("vf");
        for (unsigned char i=0; i<(unsigned char)pidAmount; ++i)
        {
            //            Serial.print("Pid ");
            Serial.print((pids[i]).pid, HEX);
            //            Serial.print(" value: ");
            Serial.print(" ");
            Serial.println((pids[i]).value);
        }
        //        Serial.println("\n");
    }

    pidAmount = obdDev->updateNormalPids();
    if (pidAmount > 0)
    {
        Serial.println("n");
        for (unsigned char i=0; i<(unsigned char)pidAmount; ++i)
        {
            //            Serial.print("Pid ");
            Serial.print((pids[i]).pid, HEX);
            //            Serial.print(" value: ");
            Serial.print(" ");
            Serial.println((pids[i]).value);
        }
        //        Serial.println("\n");
    }

    pidAmount = obdDev->updateSlowPids();
    if (pidAmount > 0)
    {
        Serial.println("s");
        for (unsigned char i=0; i<(unsigned char)pidAmount; ++i)
        {
            //            Serial.print("Pid ");
            Serial.print((pids[i]).pid, HEX);
            //            Serial.print(" value: ");
            Serial.print(" ");
            Serial.println((pids[i]).value);
        }
        //        Serial.println("\n");
    }

}


void testCaseE ()
{
    //get diagnostic trouble codes
    //    Serial.println("read trouble codes");
    unsigned char amount = 0;
    ourTypes::dtcData* dtcs = obdDev->getDiagnositcTroubleCodes(amount);

    if (dtcs != nullptr && amount != 0)
    {
        Serial.print("got #"), Serial.println(amount);
        for (unsigned int i=0; i<amount; ++i)
        {
            Serial.print("Code: "), Serial.println(dtcs[i], DEC);
        }
        //        Serial.println("\n");
        delete dtcs;
    }
    else
    {
        Serial.println("-");
    }
}

void testCaseF ()
{
    //get vehicle identification number
    //    Serial.println("read vin");
    char* vehicleIdent = obdDev->getVehicleIdentificationNumber();

    if (vehicleIdent != nullptr)
    {
        Serial.println("VIN:");
        for (unsigned char i=0; i<ourTypes::lengthOfVehicleIdentificationNumber; ++i)
        {
            Serial.print(vehicleIdent[i]);
        }
        Serial.println("\n");
        delete[] vehicleIdent;
    }
    else
    {
        Serial.println("no");
    }
}

//programm 14634 bytes
//data 575 bytes
