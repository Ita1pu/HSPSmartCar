#include <Arduino.h>
#include <StandardCplusplus.h>

#include <acc_reader.h>
#include <bluetooth.h>
#include <clock.h>
#include <gps.h>
#include <obd.h>
#include <persistence.h>

#include <FreematicsBase.h>

#include <vector>
#include <utility.h>


void testCaseA ();
void testCaseB ();
void testCaseC ();
void testCaseD ();
void testCaseE ();
void testCaseF ();
void testCaseG ();

COBDSPI* baseLayer;
obd::ObdDevice* obdDev;
char testCase = 'A';

void setup()
{
    Serial.begin(9600);
    Serial.println("Started");

    //OBD_PROTOCOLS odbType = PROTO_KWP2000_FAST; //bei andi: PROTO_KWP2000_5KBPS
                                                // michi: PROTO_ISO_9141_2
                                                //franz: PROTO_ISO_9141_2

    baseLayer = new COBDSPI();
    Serial.println("Got base layer*");

    obdDev = new obd::ObdDevice(baseLayer);
    Serial.println("Got odb object");

    baseLayer->begin();
    Serial.println("base layer begun");

    /*
    bool baseState = false;
    while(baseState == false)
    {
        baseState = baseLayer->init(odbType);
        Serial.println("base initiializing");
    }
    Serial.println("base layer initialiezed");
    */


    if (obdDev->initialize() == true)
    {
        Serial.println("obd initialiezed");
    }
    else
    {
        Serial.println("obd not initialized");
    }
}

void loop()
{
    if (Serial.available() > 0)
    {
        testCase = (char)Serial.read();
        Serial.print("i got a "), Serial.println(testCase);
    }
    Serial.print("In loop. Testcase: "), Serial.print(testCase), Serial.println();


    if(obdDev->getClamp15State() == false)
    {
        Serial.println("Clamp 15 is false, try to restart it");
        baseLayer->reset();
        if (obdDev->initialize() == false)
        {
            Serial.println("restarting failed");
        }
    }
    else
    {
        switch (testCase)
        {
            case 'A'://getValueOfPid own class -> geht
                testCaseA();
                break;

            case 'B'://getValueOfPid freematics -> geht
                testCaseB();
                break;

            case 'C'://update pid vector and give it out -> geht//erzeugt aber anscheinend einen neustart seit jetztD wtf
                testCaseC();
                break;

            case 'D'://check if pid is valid, freematics / must work for own aswell, because is uesed in test 2 -> geht
                testCaseD();
                break;

            case 'E'://get diagnostic trouble codes -> failed
                testCaseE();
                break;

            case 'F'://get vehicle identification number own class -> failed
                testCaseF();
                break;

            case 'G'://get vehicle identification number freematics -> failed
                    testCaseG();
                break;
            default:
                Serial.println("thats bad shouldnt be here");
                break;
        }
    }

    delay(5000);
}

void testCaseA ()
{
    //getValueOfPid own class -> geht
    Serial.println("Read one pid");
    bool status = false;
    int value = 200;
    value = obdDev->getValueOfPid(0x20, status);
    if (status == true)
    {
        Serial.print("Read sucessfull EngineCoolant Temp");
        Serial.println(value);
    }
    else
    {
        Serial.println("read nothing ");
        Serial.println(value);
    }
    delay(2000);
}

void testCaseB ()
{
    //getValueOfPid freematics -> geht
    Serial.println("read one pid direct with freematics");
    int value = 100;
    bool status = baseLayer->readPID(obd::VehicleSpeed, value);
    value = obdDev->getValueOfPid(obd::VehicleSpeed, status);
    if (status == true)
    {
        Serial.print("Read sucessfull EngineCoolant Temp ");
        Serial.println(value);
    }
    else
    {
        Serial.println("read nothing");
        Serial.println(value);
    }
}

void testCaseC ()
{
    //update pid vector and give it out -> geht
    Serial.println("update pid vectors and print them");
    bool statusSlow = obdDev->updateSlowPids();
    bool statusNormal = obdDev->updateNormalPids();
    bool statusFast = obdDev->updateFastPids();
    bool statusVeryFast = obdDev->updateVeryFastPids();

    if (statusSlow == true && statusNormal == true && statusFast == true && statusVeryFast == true)
    {
        Serial.println("slow pids");
        std::vector<ourTypes::pidData>* pids = obdDev->getSlowPids();
        for (unsigned int i=0; i<pids->size(); ++i)
        {
            Serial.print("Pid ");
            Serial.print((pids->at(i)).pid, HEX);
            Serial.print(" value: ");
            Serial.println((pids->at(i)).value);
        }
        Serial.println("\n");


        Serial.println("normal pids");
        pids = obdDev->getNormalPids();
        for (unsigned int i=0; i<pids->size(); ++i)
        {
            Serial.print("Pid ");
            Serial.print((pids->at(i)).pid, HEX);
            Serial.print(" value: ");
            Serial.println((pids->at(i)).value);
        }
        Serial.println("\n");

        Serial.println("fast pids");
        pids = obdDev->getFastPids();
        for (unsigned int i=0; i<pids->size(); ++i)
        {
            Serial.print("Pid ");
            Serial.print((pids->at(i)).pid, HEX);
            Serial.print(" value: ");
            Serial.println((pids->at(i)).value);
        }
        Serial.println("\n");

        Serial.println("very fast pids");
        pids = obdDev->getVeryFastPids();
        for (unsigned int i=0; i<pids->size(); ++i)
        {
            Serial.print("Pid ");
            Serial.print((pids->at(i)).pid, HEX);
            Serial.print(" value: ");
            Serial.println((pids->at(i)).value);
        }
        Serial.println("\n");
    }
    else
    {
        Serial.println("updat didn't work\n");
    }
}

void testCaseD ()
{
    //check if pid is valid, freematics / must work for own aswell, because is uesed in test 2 -> geht
    Serial.println("check if a pid is a vaild one");
    bool stat = baseLayer->isValidPID(obd::EngineCoolantTemp);

    if (stat == true)
    {
        Serial.println("is valid pid");
    }
    else
    {
        Serial.println("not valid pid\n");
    }
}

void testCaseE ()
{
    //get diagnostic trouble codes -> failed
    Serial.println("read trouble codes");
    std::vector<ourTypes::dtcData>* dtcs = obdDev->getDiagnositcTroubleCodes();

    if (dtcs->size() != 0)
    {
        Serial.print("got diagnostic trouble codes #"), Serial.println(dtcs->size());
        for (unsigned int i=0; i<dtcs->size(); ++i)
        {
            Serial.print("Trouble Code: "), Serial.println((dtcs->at(i)), DEC);
        }
        Serial.println("\n");
        delete dtcs;
    }
    else
    {
        Serial.println("got no trouble codes\n");
    }
}

void testCaseF ()
{
    //get vehicle identification number own class -> failed
    Serial.println("read vin");
    char* vehicleIdent = obdDev->getVehicleIdentificationNumber();

    if (vehicleIdent != nullptr)
    {
        Serial.println("got vehicle identification");
        for (unsigned char i=0; i<ourTypes::lengthOfVehicleIdentificationNumber; ++i)
        {
            Serial.print(vehicleIdent[i]);
        }
        Serial.println("\n");
        delete[] vehicleIdent;
    }
    else
    {
        Serial.println("got NO vehicle identifcation number\n");
    }
}

void testCaseG ()
{
    //get vehicle identification number freematics -> failed
    Serial.println("read vin with freematics");
    char vehicleIdent[ourTypes::lengthOfVehicleIdentificationNumber];
    bool state = baseLayer->getVIN(vehicleIdent, 14);

    if (state == true)
    {
        Serial.println("got vehicle identification");
        for (unsigned char i=0; i<ourTypes::lengthOfVehicleIdentificationNumber; ++i)
        {
            Serial.print(vehicleIdent[i]);
        }
        Serial.println("\n");
    }
    else
    {
        Serial.println("got NO vehicle identifcation number\n");
    }
}
