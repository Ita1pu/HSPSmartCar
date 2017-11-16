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
        ourTypes::smallPidVal getEngineRpm();
        ourTypes::smallPidVal getVehicleSpeed();
        ourTypes::smallPidVal getRunTimeSineEngineStart();

        ourTypes::smallPidVal getDistTraveledWithMalfuncIndicaLamp();
        ourTypes::smallPidVal getFuelTankLvlInput();
        ourTypes::smallPidVal getAbsBarometricPressure();


        ourTypes::smallPidVal getAmbientAirTemp();
        ourTypes::smallPidVal getFuelType();
        ourTypes::smallPidVal getEthanolPercent();
        ourTypes::smallPidVal getRelativAcceleratorPedalPos();
        ourTypes::smallPidVal getEngineOilTemp();
        ourTypes::smallPidVal getEngineFuelRate();
        ourTypes::smallPidVal getDriverTorqueDemandEngine();
        ourTypes::smallPidVal getActualTorqueEngine();
        ourTypes::smallPidVal getEngineTorqueRef();
        ourTypes::smallPidVal getEngineRunTime();


    private:



};
};

#endif
