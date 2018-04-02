using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore.Scaffolding.Internal;
using SmartCar.Shared.Model;
using SmartCar.Shared.Rest;

namespace SmartCarApi.Statistics.Advanced
{
    public class SummaryStatistic
    {
        public TripSummary CalculateTripSummary(List<Trip> trips)
        {
            var summary = new TripSummary
            {
                Summary = GetSummary(trips),
                DistanceHistogram = GetDistanceHistogram(trips),
                TimeHistogram = GetTimeHistogram(trips)
            };
            
            return summary;
        }

        private Dictionary<string, SummaryEntry> GetSummary(List<Trip> trips)
        {
            var summary = new Dictionary<string, SummaryEntry>();
            var tripGroups = trips.GroupBy(t => t.TripStart.Month);

            foreach (var tripGroup in tripGroups)
            {
                var monthDate = new DateTime(1, tripGroup.Key, 1);
                var entry = GetSummaryEntry(tripGroup);
                summary.Add(monthDate.ToString("MMMM"), entry);
            }

            return summary;
        }

        private SummaryEntry GetSummaryEntry(IGrouping<int, Trip> trips)
        {
            var entry = new SummaryEntry();

            foreach (var trip in trips)
            {
                entry.AvgFuelConsumption += trip.FuelConsumption;
                entry.TotalDistance += trip.Distance;
                entry.TimeDriven = entry.TimeDriven.Add(TimeSpan.FromSeconds(trip.Duration.TotalSeconds));
            }

            entry.AvgFuelConsumption = entry.AvgFuelConsumption / trips.Count();
            return entry;
        }

        private List<Tuple<string, double>> GetDistanceHistogram(List<Trip> trips)
        {
            var distanceHistogramm = new List<Tuple<string, double>>();
            var tripGroups = trips.GroupBy(t => t.TripStart.Month);

            foreach (var tripGroup in tripGroups)
            {
                var monthDate = new DateTime(1, tripGroup.Key, 1);
                var totalDistance = 0d;

                foreach (var trip in tripGroup)
                {
                    totalDistance += trip.Distance;
                }

                distanceHistogramm.Add(new Tuple<string, double>(monthDate.ToString("MMMM"), totalDistance));
            }

            return distanceHistogramm;
        }

        private List<Tuple<string, double>> GetTimeHistogram(List<Trip> trips)
        {
            var distanceHistogramm = new List<Tuple<string, double>>();
            var tripGroups = trips.GroupBy(t => t.TripStart.Month);

            foreach (var tripGroup in tripGroups)
            {
                var monthDate = new DateTime(1, tripGroup.Key, 1);
                var totalMinutes = 0d;

                foreach (var trip in tripGroup)
                {
                    totalMinutes += trip.Duration.TotalMinutes;
                }

                distanceHistogramm.Add(new Tuple<string, double>(monthDate.ToString("MMMM"), totalMinutes));
            }

            return distanceHistogramm;
        }
    }
}
