using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using SmartCar.Shared.Model;
using SmartCar.Shared.Rest;


namespace SmartCarApi.Statistics.Basic
{
    public class TripStatistic
    {
        public TripStatistic()
            //eventuell trip im konstruktor übergeben... aber haben wir nicht so designed
        {
        
        }

        public void CalculateBasicData(Trip trip)
        {
            trip.Duration = trip.TripData[trip.TripData.Count-1].Timestamp - trip.TripData[0].Timestamp;

            List<Tuple<DateTime, GpsCoordinate>> trace = GetGpsTrend(trip);
            for (int i=0; i<trace.Count-1; ++i)
            {
                trip.Distance += trace[i].Item2.GetDistance(trace[i + 1].Item2);
            }

            trip.AvgSpeed = (trip.Distance/1000) / Math.Max(trip.Duration.Hours, 1);

            trip.StartLocation = trace[0].Item2;
            trip.EndLocation = trace[trace.Count - 1].Item2;
        }

        
        public List<Tuple<DateTime, GpsCoordinate>> GetGpsTrend(Trip trip)
        {
            int status = 0; //+1=longitude is set       +2=latitude is set
            int tempLongitude = 0;
            int tempLatitude = 0;
            
            List<Tuple<DateTime, GpsCoordinate>> returnValue = new List<Tuple<DateTime, GpsCoordinate>>();
            foreach (var currentTripData in trip.TripData)
            {

                if (currentTripData.SignalType.SignalName == Signal.GpsLongitude)
                {
                    tempLongitude = currentTripData.Value;
                    status += 1;
                }

                if (currentTripData.SignalType.SignalName == Signal.GpsLatitude)
                {
                    tempLatitude = currentTripData.Value;
                    status += 2;
                }
                
                if (status >=3)
                {
                    status = 0;
                    GpsCoordinate tempGpsCoordinate = new GpsCoordinate(tempLatitude, tempLongitude);
                    returnValue.Add(Tuple.Create<DateTime, GpsCoordinate>(currentTripData.Timestamp, tempGpsCoordinate));
                }
            }
            return returnValue;
        }
        

        public List<Tuple<DateTime, double>> GetSpeedTrendObd(Trip trip)
        {
            List<Tuple<DateTime, double>> returnValue = new List<Tuple<DateTime, double>>();
            foreach (var currentTripData in trip.TripData)
            {
                if (currentTripData.SignalType.SignalName == Signal.VehicleSpeed)
                {
                    returnValue.Add(Tuple.Create<DateTime, double>(currentTripData.Timestamp, currentTripData.Value));
                }
            }
            return returnValue;
        }

        public List<Tuple<DateTime, double>> GetSpeedTrendGps(Trip trip)
        {
            List<Tuple<DateTime, double>> returnValue = new List<Tuple<DateTime, double>>();

            List<Tuple<DateTime, GpsCoordinate>> trace = GetGpsTrend(trip);
            for (int i = 0; i < trace.Count - 1; ++i)
            {
                double distance = trace[i].Item2.GetDistance(trace[i + 1].Item2);
                TimeSpan timeSpan = trace[i + 1].Item1- trace[i].Item1;
                double speed = (distance / 1000) / timeSpan.Hours;
                returnValue.Add(Tuple.Create<DateTime, double>(trace[i].Item1, speed));
            }

            return returnValue;
        }
    }
}
