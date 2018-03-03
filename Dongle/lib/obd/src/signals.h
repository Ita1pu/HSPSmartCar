#ifndef SIGNALS_H
#define SIGNALS_H

namespace obd
{
    enum PidNames {EngineCoolantTemp, EngineRpm, VehicleSpeed, RunTimeSineEngineStart, DistTraveledWithMalfuncIndicaLamp,
    FuelTankLvlInput, AbsBarometricPressure, AmbientAirTemp, FuelType, EthanolPercent, RelativAcceleratorPedalPos,
    EngineOilTemp, EngineFuelRate, DriverTorqueDemandEngine, ActualTorqueEngine, EngineTorqueRef, EngineRunTime};

    const PROGMEM unsigned char PidNumbers[] = {0x5, 0x0C, 0x0D, 0x1F, 0x21, 0x2F, 0x33, 0x46, 0x51, 0x52, 0x5A, 0x5C, 0x5E,
        0x61, 0x62, 0x63, 0x7F};

    //    const PROGMEM unsigned char PidNumbers[] = {0x5, 0x0C, 0x0D, 0x21, 0x7F};



    /**
        Full name: Engine coolant temperature, Range:[-40; 215]°C, Size: 1 byte, Polling category: C (normal)
    */
//    const PROGMEM unsigned char EngineCoolantTemp         =          (0x5);

    /**
        Full name: Engine RPM, Range: [0; 16 383.75]rpm, Size: 2 byte, Polling category: A (very fast)
    */
//    const PROGMEM unsigned char EngineRpm             =             (0x0C);

    /**
        Full name: Vehicle speed, Range: [0; 255]km/h, Size: 1 byte, Polling category: A (very fast)
    */
//    const PROGMEM unsigned char VehicleSpeed           =            (0x0D);

    /**
        Full name: Run time since engine start, Range: [0; 65 535]seconds, Size: 2 byte, Polling category: C (normal)
    */
//    const PROGMEM unsigned char RunTimeSineEngineStart      =       (0x1F);


    /**
        Full name: Distance traveled with malfunction indicator lamp (MIL), Range: [0; 65 535]km, Size: 2 byte, Polling category: D (slow)
    */
//    const PROGMEM unsigned char DistTraveledWithMalfuncIndicaLamp = (0x21);

    /**
        Full name: Fuel tank level input, Range: [0; 100]%, Size: 1 byte, Polling category: D (slow)
    */
//    const PROGMEM unsigned char FuelTankLvlInput        =           (0x2F);

    /**
        Full name: Absolute barometric pressure, Range: [0; 255]kPa, Size: 1 byte, Polling category: C (normal)
    */
//    const PROGMEM unsigned char AbsBarometricPressure       =       (0x33);


    /**
        Full name: Ambient ari temperature, Range: [-40; 215]°C, Size: 1 byte, Polling category: C (normal)
    */
//    const PROGMEM unsigned char AmbientAirTemp         =            (0x46);

    /**
        Full name: Fuel type, Range: https://en.wikipedia.org/wiki/OBD-II_PIDs#Fuel_Type_Coding, Size: 1 byte, Polling category: F (only in new vehicle)
    */
//    const PROGMEM unsigned char FuelType             =              (0x51);

    /**
        Full name: Ethanol fuel %, Range: [0; 100]%, Size: 1 byte, Polling category: E (on startup)
    */
//    const PROGMEM unsigned char EthanolPercent          =           (0x52);

    /**
        Full name: Relative accelerator pedal position, Range: [0; 100]%, Size: 1 byte, Polling category: A (very fast)
    */
//    const PROGMEM unsigned char RelativAcceleratorPedalPos    =     (0x5A);

    /**
        Full name: Engine oil temperature, Range: [-40; 210]°C, Size: 1 byte, Polling category: C (normal)
    */
//    const PROGMEM unsigned char EngineOilTemp          =            (0x5C);

    /**
        Full name: Engine fuel rate, Range: [0; 3 276.75]L/h, Size: 2 byte, Polling category: A (very fast)
    */
//    const PROGMEM unsigned char EngineFuelRate          =           (0x5E);


    /**
        Full name: Driver's demand engine-percent torque, Range: [-125; 125]%, Size: 1 byte: A (very fast)
    */
//    const PROGMEM unsigned char DriverTorqueDemandEngine      =     (0x61);

    /**
        Full name: Actual engine-percent torque, Range: [-125; 125]%, Size: 1 byte, Polling category: A (very fast)
    */
//    const PROGMEM unsigned char ActualTorqueEngine        =         (0x62);

    /**
        Full name: Engine reference torque, Range: [0; 65 535]Nm, Size: 2 byte, Polling category: A (very fast) or F if it is constant
    */
//    const PROGMEM unsigned char EngineTorqueRef        =            (0x63);


//todo: special case....
    /**
        Full name: Engine run time, Range: todo: not clear yet but 13bytes, Polling category: C (normal)
    */
//    const PROGMEM unsigned char EngineRunTime          =            (0x7F);

    /**
        M. Schleinkofer: added ID values for GPS, acceleration values, gyroscope values
    */
    /**
        Full name: geographical latitude in degree * 1000000; e.g. 48215559 => 48,215559°; Range: [-180000000; 180000000]
    */
    const unsigned char GpsLatitude          =             (0xF0);
    /**
        Full name: geographical longitude in degree * 1000000; e.g. 48215559 => 48,215559°; Range: [-180000000; 180000000]
    */
    const unsigned char GpsLongitude         =             (0xF1);
    /**
        Full name: number of available navigation sattelites; Range: [0; 24]
    */
    const unsigned char GpsSatCount          =             (0xF2);
    /**
        Full name: acceleration value of x axis in 1/1000 of gravitational force; Range: [to be checked]
    */
    const unsigned char AccelXAxis           =             (0xF3);
    /**
        Full name: acceleration value of y axis in 1/1000 of gravitational force; Range: [to be checked]
    */
    const unsigned char AccelYAxis           =             (0xF4);
    /**
        Full name: acceleration value of z axis in 1/1000 of gravitational force; Range: [to be checked]
    */
    const unsigned char AccelZAxis           =             (0xF5);
    /**
        Full name: gyroscope value of x axis in 1/1000 of degrees per second; Range: [to be checked]
    */
    const unsigned char GyroXAxis            =             (0xF6);
    /**
        Full name: gyroscope value of y axis in 1/1000 of degrees per second; Range: [to be checked]
    */
    const unsigned char GyroYAxis            =             (0xF7);
    /**
        Full name: gyroscope value of z axis in 1/1000 of degrees per second; Range: [to be checked]
    */
    const unsigned char GyroZAxis            =             (0xF8);

}//end namespace obd
#endif
