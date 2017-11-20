#ifndef OBD_H
#define OBD_H


#include <types.h>


namespace obd {

class ObdDevice
{
    public:
        ObdDevice();
        void initialize();

        void updateVeryFastPids();
        void updateFastPids();
        void updateNormalPids();
        void updateSlowPids();

        ourTypes::smallPidVal getEngineCoolantTemp();
        ourTypes::bigPidVal getEngineRpm();
        ourTypes::smallPidVal getVehicleSpeed();
        ourTypes::bigPidVal getRunTimeSinceEngineStart();

        ourTypes::bigPidVal getDistTraveledWithMalfuncIndicaLamp();
        ourTypes::smallPidVal getFuelTankLvlInput();
        ourTypes::smallPidVal getAbsBarometricPressure();


        ourTypes::smallPidVal getAmbientAirTemp();
        ourTypes::smallPidVal getFuelType();
        ourTypes::smallPidVal getEthanolPercent();
        ourTypes::smallPidVal getRelativAcceleratorPedalPos();
        ourTypes::smallPidVal getEngineOilTemp();
        ourTypes::bigPidVal getEngineFuelRate();
        ourTypes::smallPidVal getDriverTorqueDemandEngine();
        ourTypes::smallPidVal getActualTorqueEngine();
        ourTypes::bigPidVal getEngineTorqueRef();

        //todo:
        //add missing
        //type for 13byte....
        //ourTypes::WTF getEngineRunTime();


    private:
        ourTypes::smallPidVal engineCoolantTemp;
        ourTypes::bigPidVal engineRpm;
        ourTypes::smallPidVal vehicleSpeed;
        ourTypes::bigPidVal runTimeSinceEngineStart;
        ourTypes::bigPidVal distTraveledWithMalfuncIndicaLamp;
        ourTypes::smallPidVal fuelTankLvlInput;
        ourTypes::smallPidVal absBarometricPressure;
        ourTypes::smallPidVal ambientAirTemp;
        ourTypes::smallPidVal fuelType;
        ourTypes::smallPidVal ethanolPercent;
        ourTypes::smallPidVal relativAcceleratorPedalPos;
        ourTypes::smallPidVal engineOilTemp;
        ourTypes::bigPidVal engineFuelRate;
        ourTypes::smallPidVal driverTorqueDemandEngine;
        ourTypes::smallPidVal actualTorqueEngine;
        ourTypes::bigPidVal engineTorqueRef;



};
};

#endif
