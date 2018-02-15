using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartCar.Shared.Model
{
    public enum Signal
    {
        EngineCoolantTemp,
        EngineRpm,
        VehicleSpeed,
        RunTimeSineEngineStart,
        DistTraveledWithMalfuncIndicaLamp,
        FuelTankLvlInput,
        AbsBarometricPressure,
        AbientAirTemp,
        FuleType,
        EthanolPercent,
        RelativeAcceleratorPedalPos,
        EngineOilTemp,
        EngineFuleRate,
        DriverTourqueDemandEngine,
        ActualTorqueEngine,
        EngineTorqueRef,
        EngineRuntime,
        GpsLatitude,
        GpsLongitude,
        GpsSatCount,
        AccelXAxis,
        AccelYAxis,
        AccelZAxis,
        GyroXAxis,
        GyroYAxis,
        GyroZAxis
    }

    public class SignalMap
    {
        [Key]
        public int MapId { get; set; }

        public int SignalId { get; set; }

        public Signal SignalName { get; set; }
    }
}
