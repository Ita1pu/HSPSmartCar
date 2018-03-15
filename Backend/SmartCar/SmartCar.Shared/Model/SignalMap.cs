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
        EngineLoad,
        EngineCoolantTemp,
        EngineRpm,
        VehicleSpeed,
        MAFAirFlow,
        RunTimeSineEngineStart,
        DistTraveledWithMalfuncIndicaLamp,
        OxySensor01,
        OxySensor02,
        OxySensor03,
        OxySensor04,
        OxySensor05,
        OxySensor06,
        OxySensor07,
        OxySensor08,
        FuelTankLvlInput,
        AbsBarometricPressure,
        OxySensor11,
        OxySensor12,
        OxySensor13,
        OxySensor14,
        OxySensor15,
        OxySensor16,
        OxySensor17,
        OxySensor18,
        AmbientAirTemp,
        TestEquipConf1,
        TestEquipConf2,
        FuelType,
        EthanolPercent,
        RelativAcceleratorPedalPos,
        EngineOilTemp,
        EngineFuelRate,
        DriverTorqueDemandEngine,
        ActualTorqueEngine,
        EngineTorqueRef,
        EngineRunTime,
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
