using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using SmartCar.Shared.Model;
using SmartCar.Shared.Rest;

namespace SmartCarApi.Statistics.Advanced
{
    public class HeatmapStatistic
    {
        public List<GpsCoordinate> GetHeatmapData(IEnumerable<Trip> trips)
        {
            var gpsData = new List<GpsCoordinate>();

            foreach (var trip in trips)
            {
                int status = 0; 
                double tempLongitude = 0;
                double tempLatitude = 0;

                foreach (var tripEntry in trip.TripData.OrderBy(td => td.Timestamp))
                {
                    if (tripEntry?.SignalType?.SignalName == Signal.GpsLongitude)
                    {
                        tempLongitude = tripEntry.Value / (double)(1000 * 1000);
                        status += 1;
                    }

                    if (tripEntry?.SignalType?.SignalName == Signal.GpsLatitude)
                    {
                        tempLatitude = tripEntry.Value / (double)(1000 * 1000);
                        status += 2;
                    }

                    if (status >= 3)
                    {
                        status = 0;
                        gpsData.Add(new GpsCoordinate(tempLatitude, tempLongitude));
                    }
                }
            }

            return gpsData;
        }
    }
}
