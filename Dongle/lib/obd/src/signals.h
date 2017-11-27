#ifndef SIGNALS_H
#define SIGNALS_H

namespace obd
{
    /**
        Full name: Engine coolant temperature, Range:[-40; 215]°C, Size: 1 byte, Polling category: C (normal)
    */
    #define EngineCoolantTemp                    (0x5)

    /**
        Full name: Engine RPM, Range: [0; 16 383.75]rpm, Size: 2 byte, Polling category: A (very fast)
    */
    #define EngineRpm                           (0x0C)

    /**
        Full name: Vehicle speed, Range: [0; 255]km/h, Size: 1 byte, Polling category: A (very fast)
    */
    #define VehicleSpeed                        (0x0D)

    /**
        Full name: Run time since engine start, Range: [0; 65 535]seconds, Size: 2 byte, Polling category: C (normal)
    */
    #define RunTimeSineEngineStart              (0x1F)


    /**
        Full name: Distance traveled with malfunction indicator lamp (MIL), Range: [0; 65 535]km, Size: 2 byte, Polling category: D (slow)
    */
    #define DistTraveledWithMalfuncIndicaLamp   (0x21)

    /**
        Full name: Fuel tank level input, Range: [0; 100]%, Size: 1 byte, Polling category: D (slow)
    */
    #define FuelTankLvlInput                    (0x2F)

    /**
        Full name: Absolute barometric pressure, Range: [0; 255]kPa, Size: 1 byte, Polling category: C (normal)
    */
    #define AbsBarometricPressure               (0x33)


    /**
        Full name: Ambient ari temperature, Range: [-40; 215]°C, Size: 1 byte, Polling category: C (normal)
    */
    #define AmbientAirTemp                      (0x46)

    /**
        Full name: Fuel type, Range: https://en.wikipedia.org/wiki/OBD-II_PIDs#Fuel_Type_Coding, Size: 1 byte, Polling category: F (only in new vehicle)
    */
    #define FuelType                            (0x51)

    /**
        Full name: Ethanol fuel %, Range: [0; 100]%, Size: 1 byte, Polling category: E (on startup)
    */
    #define EthanolPercent                      (0x52)

    /**
        Full name: Relative accelerator pedal position, Range: [0; 100]%, Size: 1 byte, Polling category: A (very fast)
    */
    #define RelativAcceleratorPedalPos          (0x5A)

    /**
        Full name: Engine oil temperature, Range: [-40; 210]°C, Size: 1 byte, Polling category: C (normal)
    */
    #define EngineOilTemp                       (0x5C)

    /**
        Full name: Engine fuel rate, Range: [0; 3 276.75]L/h, Size: 2 byte, Polling category: A (very fast)
    */
    #define EngineFuelRate                      (0x5E)


    /**
        Full name: Driver's demand engine-percent torque, Range: [-125; 125]%, Size: 1 byte: A (very fast)
    */
    #define DriverTorqueDemandEngine            (0x61)

    /**
        Full name: Actual engine-percent torque, Range: [-125; 125]%, Size: 1 byte, Polling category: A (very fast)
    */
    #define ActualTorqueEngine                  (0x62)

    /**
        Full name: Engine reference torque, Range: [0; 65 535]Nm, Size: 2 byte, Polling category: A (very fast) or F if it is constant
    */
    #define EngineTorqueRef                     (0x63)


//todo: special case....
    /**
        Full name: Engine run time, Range: todo: not clear yet but 13bytes, Polling category: C (normal)
    */
    #define EngineRunTime                       (0x7F)

}//end namespace obd
#endif
