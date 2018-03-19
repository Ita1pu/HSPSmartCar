using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using SmartCar.Shared.Model;
using SmartCarApi.Statistics.Basic;

namespace SmartCarApi.Statistics.Advanced
{
    public class FuelConsumption
    {
        private double _AirFuelRatio; //otto 14.7       //diesel 14.5
        private double _FuelDensity; //otto 747.5 g/l   //diesel 832.5 g/l
        private bool _IsDiesel;
        private int _CorrectionFactorOxiSensor=0;
        private int _CorrectionFactorAirFlow = 0;

        List<Tuple<DateTime, double>> _FuelRateInLproH = new List<Tuple<DateTime, double>>();

        List<Tuple<DateTime, Signal, int>> _OxySensorDataComplete = new List<Tuple<DateTime, Signal, int>>();
        List<Tuple<DateTime, double>> _OxySensorDataRefined = new List<Tuple<DateTime, double>>();
        List<Tuple<DateTime, double>> _AirMass = new List<Tuple<DateTime, double>>();

        List<Tuple<DateTime, double>> _EngineLoad = new List<Tuple<DateTime, double>>();
        List<Tuple<DateTime, double>> _EngineRPM = new List<Tuple<DateTime, double>>();


        public FuelConsumption(bool IsDisel=true)
        {
            _IsDiesel = IsDisel;

            if (IsDisel == true)
            {
                _AirFuelRatio = 15.5;
                _FuelDensity = 832.5;
            }
            else
            {
                _AirFuelRatio = 14.7;
                _FuelDensity = 747.5;
            }
        }

        public List<Tuple<DateTime,double>> CalcFuelConsumption(Trip trip, List<Tuple<DateTime, double>> _SpeedTrend=null)//returns list with date und l/h
        {
            Prepair();
            ExtractNeededInformations(trip);

            if (_FuelRateInLproH.Count == 0)//got fuelrate NOT over obd calc it 
            {
                RefineOxyInformations();

                if (_SpeedTrend == null)
                {
                    TripStatistic _TripStatistic = new TripStatistic();
                    _SpeedTrend = _TripStatistic.GetSpeedTrendObd(trip);
                }

                _FuelRateInLproH = CalcFuelRate();
            }

            var _FuelRateInLpro100km = new List<Tuple<DateTime, double>>();
            double _SummForAverage = 0;

            for (int i = 0; i < Math.Min(_FuelRateInLproH.Count, _SpeedTrend.Count); ++i)
            {
                double tmp;
                if (_SpeedTrend[i].Item2 < 2)//to filter out to small values and 0 values that will destroy the calculation
                {
                    tmp = 0;
                }
                else
                {
                    tmp = _FuelRateInLproH[i].Item2 * 100 / _SpeedTrend[i].Item2;//convert from l/h in l/100km
                }
                _SummForAverage += tmp;
                _FuelRateInLpro100km.Add(new Tuple<DateTime, double>(_FuelRateInLproH[i].Item1, tmp));
            }

            trip.FuelConsumption = _SummForAverage / Math.Min(_FuelRateInLproH.Count, _SpeedTrend.Count);

            return _FuelRateInLproH;
            
        }

        private void ExtractNeededInformations(Trip trip)
        {
            foreach (var _tripData in trip.TripData.OrderBy(td => td.Timestamp))
            {
                if (_tripData?.SignalType?.SignalName == Signal.EngineFuelRate)
                {
                    _FuelRateInLproH.Add(new Tuple<DateTime, double>(_tripData.Timestamp, _tripData.Value));
                    continue;
                }

                if (_tripData?.SignalType?.SignalName == Signal.TestEquipConf1)
                {
                    _CorrectionFactorOxiSensor = _tripData.Value;
                    continue;
                }

                if (_tripData?.SignalType?.SignalName == Signal.OxySensor01 ||
                    _tripData?.SignalType?.SignalName == Signal.OxySensor02 ||
                    _tripData?.SignalType?.SignalName == Signal.OxySensor03 ||
                    _tripData?.SignalType?.SignalName == Signal.OxySensor04 ||
                    _tripData?.SignalType?.SignalName == Signal.OxySensor05 ||
                    _tripData?.SignalType?.SignalName == Signal.OxySensor06 ||
                    _tripData?.SignalType?.SignalName == Signal.OxySensor07 ||
                    _tripData?.SignalType?.SignalName == Signal.OxySensor08 ||
                    _tripData?.SignalType?.SignalName == Signal.OxySensor11 ||
                    _tripData?.SignalType?.SignalName == Signal.OxySensor12 ||
                    _tripData?.SignalType?.SignalName == Signal.OxySensor13 ||
                    _tripData?.SignalType?.SignalName == Signal.OxySensor14 ||
                    _tripData?.SignalType?.SignalName == Signal.OxySensor15 ||
                    _tripData?.SignalType?.SignalName == Signal.OxySensor16 ||
                    _tripData?.SignalType?.SignalName == Signal.OxySensor17 ||
                    _tripData?.SignalType?.SignalName == Signal.OxySensor18)
                {
                    _OxySensorDataComplete.Add(new Tuple<DateTime, Signal, int>(_tripData.Timestamp, _tripData.SignalType.SignalName, _tripData.Value));
                    continue;
                }

                if (_tripData?.SignalType?.SignalName == Signal.MAFAirFlow)
                {
                    if (_CorrectionFactorAirFlow==0)
                    {
                        _AirMass.Add(new Tuple<DateTime, double>(_tripData.Timestamp, _tripData.Value * (1/1000)));
                    }
                    else
                    {
                        _AirMass.Add(new Tuple<DateTime, double>(_tripData.Timestamp, _tripData.Value*(1000* _CorrectionFactorAirFlow)));
                    }
                    continue;
                }

                if (_tripData?.SignalType?.SignalName == Signal.TestEquipConf2)
                {
                    _CorrectionFactorAirFlow = _tripData.Value;
                    continue;
                }

                if (_tripData?.SignalType?.SignalName == Signal.EngineLoad)
                {
                    _EngineLoad.Add(new Tuple<DateTime, double>(_tripData.Timestamp, _tripData.Value));
                    continue;
                }

                if (_tripData?.SignalType?.SignalName == Signal.EngineRpm)
                {
                    _EngineRPM.Add(new Tuple<DateTime, double>(_tripData.Timestamp, _tripData.Value));
                    continue;
                }
            }
        }

        private void RefineOxyInformations() //to create lambda
        {
            int bigCnt = 0;
            int tempSum = 0;

            //summ up multiple oxy sensors
            do
            {
                int smallCnt = 1;

                Signal first = _OxySensorDataComplete[bigCnt].Item2;
                tempSum = 0x0000FFFF & _OxySensorDataComplete[bigCnt].Item3;

                while (bigCnt + smallCnt < _OxySensorDataComplete.Count && _OxySensorDataComplete[bigCnt + smallCnt].Item2 != first)
                {
                    //first 2 bytes are killed because they are wrong thanks to parsing from freematics
                    tempSum += 0x0000FFFF & _OxySensorDataComplete[bigCnt + smallCnt].Item3;
                    smallCnt++;
                }
                _OxySensorDataRefined.Add(new Tuple<DateTime, double>(_OxySensorDataComplete[bigCnt].Item1, tempSum));
                bigCnt += smallCnt;
            }
            while (bigCnt < _OxySensorDataComplete.Count);

            //correct scaling of sensor
            List<Tuple<DateTime, double>> tempArry = new List<Tuple<DateTime, double>>();
            foreach (var item in _OxySensorDataRefined)
            {
                if (_CorrectionFactorOxiSensor == 0)
                {
                    double tmp = item.Item2 * ((double)2 / 65535);
                    tempArry.Add(new Tuple<DateTime, double>(item.Item1, tmp));
                }
                else
                {
                    double tmp = item.Item2 * ((double)_CorrectionFactorOxiSensor / 65535);
                    tempArry.Add(new Tuple<DateTime, double>(item.Item1, tmp));
                }
            }
            _OxySensorDataRefined = tempArry;
        }

        private void Prepair()
        {
            //clean up form last trip
            _CorrectionFactorOxiSensor = 0;
            _CorrectionFactorAirFlow = 0;

            _FuelRateInLproH.Clear();

            _OxySensorDataComplete.Clear();
            _OxySensorDataRefined.Clear();
            _AirMass.Clear();

            _EngineLoad.Clear();
            _EngineRPM.Clear();
        }

        private List<Tuple<DateTime, double>> CalcFuelRate()
        {
            List<Tuple<DateTime, double>> returnValue = new List<Tuple<DateTime, double>>();

            if (_AirMass.Count == 0)
            {
                CalcAirFlow();
            }

            for (int i = 0; i < Math.Min(_OxySensorDataRefined.Count, _AirMass.Count); ++i)
            {
                returnValue.Add(new Tuple<DateTime, double>(_AirMass[i].Item1, 3600 * _AirMass[i].Item2 / (_OxySensorDataRefined[i].Item2 * _AirFuelRatio * _FuelDensity)));
            }
            return returnValue;
        }

        private void CalcAirFlow()
        {
            Func<double, double> AfFull = x => ((double)3 * x / 175) - 2;
            Func<double, double> AfIdle = x => ((double)11 * x / 3150) - 4 / 9;
            Func<double, double> LoadFull = x => 100;
            Func<double, double> LoadIdle = x => 0.000264901 * x + 25.5728;

            for(int i=0; i<Math.Min(_EngineLoad.Count, _EngineRPM.Count); ++i)
            {
                double rpm = _EngineRPM[i].Item2;
                double tmp = AfIdle(rpm) + (AfFull(rpm) - AfIdle(rpm)) / (LoadFull(rpm) - LoadIdle(rpm)) * (_EngineLoad[i].Item2 -LoadIdle(rpm));
                _AirMass.Add(new Tuple<DateTime, double>(_EngineRPM[i].Item1, tmp));
            }
        }
    }
}
