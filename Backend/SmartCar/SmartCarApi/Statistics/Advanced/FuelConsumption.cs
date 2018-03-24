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
        private readonly double _airFuelRatio; //otto 14.7       //diesel 14.5
        private readonly double _fuelDensity; //otto 747.5 g/l   //diesel 832.5 g/l

        private int _correctionFactorOxiSensor=0;
        private int _correctionFactorAirFlow = 0;

        List<Tuple<DateTime, double>> _fuelRateInLproH = new List<Tuple<DateTime, double>>();

        List<Tuple<DateTime, Signal, int>> _oxySensorDataComplete = new List<Tuple<DateTime, Signal, int>>();
        List<Tuple<DateTime, double>> _oxySensorDataRefined = new List<Tuple<DateTime, double>>();
        List<Tuple<DateTime, double>> _airMass = new List<Tuple<DateTime, double>>();

        List<Tuple<DateTime, double>> _engineLoad = new List<Tuple<DateTime, double>>();
        List<Tuple<DateTime, double>> _engineRPM = new List<Tuple<DateTime, double>>();


        public FuelConsumption(bool isDiesel=true)
        {
            if (isDiesel)
            {
                _airFuelRatio = 15.5;
                _fuelDensity = 832.5;
            }
            else
            {
                _airFuelRatio = 14.7;
                _fuelDensity = 747.5;
            }
        }

        public List<Tuple<DateTime,double>> CalcFuelConsumption(Trip trip, List<Tuple<DateTime, double>> speedTrend=null)//returns list with date und l/h
        {
            try
            {
                Prepair();
                ExtractNeededInformations(trip);

                if (_fuelRateInLproH.Count == 0) //got fuelrate NOT over obd calc it 
                {
                    RefineOxyInformations();

                    if (speedTrend == null)
                    {
                        TripStatistic tripStatistic = new TripStatistic();
                        speedTrend = tripStatistic.GetSpeedTrendObd(trip);
                    }

                    _fuelRateInLproH = CalcFuelRate();
                }

                var fuelRateInLpro100Km = new List<Tuple<DateTime, double>>();
                double summForAverage = 0;

                for (int i = 0; i < Math.Min(_fuelRateInLproH.Count, speedTrend.Count); ++i)
                {
                    double tmp;
                    if (speedTrend[i].Item2 < 2
                    ) //to filter out to small values and 0 values that will destroy the calculation
                    {
                        tmp = 0;
                    }
                    else
                    {
                        tmp = _fuelRateInLproH[i].Item2 * 100 / speedTrend[i].Item2; //convert from l/h in l/100km
                    }

                    summForAverage += tmp;
                    fuelRateInLpro100Km.Add(new Tuple<DateTime, double>(_fuelRateInLproH[i].Item1, tmp));
                }

                trip.FuelConsumption = summForAverage / Math.Min(_fuelRateInLproH.Count, speedTrend.Count);

                return _fuelRateInLproH;
            }
            catch (Exception)
            {
                return default(List<Tuple<DateTime,double>>);
            }
        }

        private void ExtractNeededInformations(Trip trip)
        {
            foreach (var tripData in trip.TripData.OrderBy(td => td.Timestamp))
            {
                if (tripData.SignalType?.SignalName == Signal.EngineFuelRate)
                {
                    _fuelRateInLproH.Add(new Tuple<DateTime, double>(tripData.Timestamp, tripData.Value));
                    continue;
                }

                if (tripData.SignalType?.SignalName == Signal.TestEquipConf1)
                {
                    _correctionFactorOxiSensor = tripData.Value;
                    continue;
                }

                if (tripData.SignalType?.SignalName == Signal.OxySensor01 ||
                    tripData.SignalType?.SignalName == Signal.OxySensor02 ||
                    tripData.SignalType?.SignalName == Signal.OxySensor03 ||
                    tripData.SignalType?.SignalName == Signal.OxySensor04 ||
                    tripData.SignalType?.SignalName == Signal.OxySensor05 ||
                    tripData.SignalType?.SignalName == Signal.OxySensor06 ||
                    tripData.SignalType?.SignalName == Signal.OxySensor07 ||
                    tripData.SignalType?.SignalName == Signal.OxySensor08 ||
                    tripData.SignalType?.SignalName == Signal.OxySensor11 ||
                    tripData.SignalType?.SignalName == Signal.OxySensor12 ||
                    tripData.SignalType?.SignalName == Signal.OxySensor13 ||
                    tripData.SignalType?.SignalName == Signal.OxySensor14 ||
                    tripData.SignalType?.SignalName == Signal.OxySensor15 ||
                    tripData.SignalType?.SignalName == Signal.OxySensor16 ||
                    tripData.SignalType?.SignalName == Signal.OxySensor17 ||
                    tripData.SignalType?.SignalName == Signal.OxySensor18)
                {
                    _oxySensorDataComplete.Add(new Tuple<DateTime, Signal, int>(tripData.Timestamp, tripData.SignalType.SignalName, tripData.Value));
                    continue;
                }

                if (tripData.SignalType?.SignalName == Signal.MAFAirFlow)
                {
                    if (_correctionFactorAirFlow==0)
                    {
                        _airMass.Add(new Tuple<DateTime, double>(tripData.Timestamp, tripData.Value * (1/1000)));
                    }
                    else
                    {
                        _airMass.Add(new Tuple<DateTime, double>(tripData.Timestamp, tripData.Value*(1000* _correctionFactorAirFlow)));
                    }
                    continue;
                }

                if (tripData.SignalType?.SignalName == Signal.TestEquipConf2)
                {
                    _correctionFactorAirFlow = tripData.Value;
                    continue;
                }

                if (tripData.SignalType?.SignalName == Signal.EngineLoad)
                {
                    _engineLoad.Add(new Tuple<DateTime, double>(tripData.Timestamp, tripData.Value));
                    continue;
                }

                if (tripData.SignalType?.SignalName == Signal.EngineRpm)
                {
                    _engineRPM.Add(new Tuple<DateTime, double>(tripData.Timestamp, tripData.Value));
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

                Signal first = _oxySensorDataComplete[bigCnt].Item2;
                tempSum = 0x0000FFFF & _oxySensorDataComplete[bigCnt].Item3;

                while (bigCnt + smallCnt < _oxySensorDataComplete.Count && _oxySensorDataComplete[bigCnt + smallCnt].Item2 != first)
                {
                    //first 2 bytes are killed because they are wrong thanks to parsing from freematics
                    tempSum += 0x0000FFFF & _oxySensorDataComplete[bigCnt + smallCnt].Item3;
                    smallCnt++;
                }
                _oxySensorDataRefined.Add(new Tuple<DateTime, double>(_oxySensorDataComplete[bigCnt].Item1, tempSum));
                bigCnt += smallCnt;
            }
            while (bigCnt < _oxySensorDataComplete.Count);

            //correct scaling of sensor
            List<Tuple<DateTime, double>> tempArry = new List<Tuple<DateTime, double>>();
            foreach (var item in _oxySensorDataRefined)
            {
                if (_correctionFactorOxiSensor == 0)
                {
                    double tmp = item.Item2 * ((double)2 / 65535);
                    tempArry.Add(new Tuple<DateTime, double>(item.Item1, tmp));
                }
                else
                {
                    double tmp = item.Item2 * ((double)_correctionFactorOxiSensor / 65535);
                    tempArry.Add(new Tuple<DateTime, double>(item.Item1, tmp));
                }
            }
            _oxySensorDataRefined = tempArry;
        }

        private void Prepair()
        {
            //clean up form last trip
            _correctionFactorOxiSensor = 0;
            _correctionFactorAirFlow = 0;

            _fuelRateInLproH.Clear();

            _oxySensorDataComplete.Clear();
            _oxySensorDataRefined.Clear();
            _airMass.Clear();

            _engineLoad.Clear();
            _engineRPM.Clear();
        }

        private List<Tuple<DateTime, double>> CalcFuelRate()
        {
            List<Tuple<DateTime, double>> returnValue = new List<Tuple<DateTime, double>>();

            if (_airMass.Count == 0)
            {
                CalcAirFlow();
            }

            for (int i = 0; i < Math.Min(_oxySensorDataRefined.Count, _airMass.Count); ++i)
            {
                returnValue.Add(new Tuple<DateTime, double>(_airMass[i].Item1, 3600 * _airMass[i].Item2 / (_oxySensorDataRefined[i].Item2 * _airFuelRatio * _fuelDensity)));
            }
            return returnValue;
        }

        private void CalcAirFlow()
        {
            Func<double, double> AfFull = x => ((double)3 * x / 175) - 2;
            Func<double, double> AfIdle = x => ((double)11 * x / 3150) - 4 / 9;
            Func<double, double> LoadFull = x => 100;
            Func<double, double> LoadIdle = x => 0.000264901 * x + 25.5728;

            for(int i=0; i<Math.Min(_engineLoad.Count, _engineRPM.Count); ++i)
            {
                double rpm = _engineRPM[i].Item2;
                double tmp = AfIdle(rpm) + (AfFull(rpm) - AfIdle(rpm)) / (LoadFull(rpm) - LoadIdle(rpm)) * (_engineLoad[i].Item2 -LoadIdle(rpm));
                _airMass.Add(new Tuple<DateTime, double>(_engineRPM[i].Item1, tmp));
            }
        }
    }
}
