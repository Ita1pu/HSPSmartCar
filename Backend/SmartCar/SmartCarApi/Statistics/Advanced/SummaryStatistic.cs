using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using SmartCar.Shared.Model;
using SmartCar.Shared.Rest;

namespace SmartCarApi.Statistics.Advanced
{
    public class SummaryStatistic
    {
        public TripSummary CalculateTripSummary(List<Trip> trips)
        {
            var summary = new TripSummary();

            summary.Summary = GetSummary(trips);
            summary.DistanceHistogram = GetDistanceHistogram(trips);

            return summary;
        }

        private Dictionary<string, SummaryEntry> GetSummary(List<Trip> trips)
        {
            var tripGroups = trips.GroupBy(t => t.TripStart.Month);
            return null;
        }

        private List<Tuple<string, double>> GetDistanceHistogram(List<Trip> trips)
        {

            return null;
        }
    }
}
