using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartCar.Shared.Rest
{
    public class SummaryEntry
    {
        public TimeSpan TimeDriven { get; set; }

        public double TotalDistance { get; set; }

        public double AvgFuelConsumption { get; set; }
    }

    public class TripSummary
    {
        public Dictionary<string, SummaryEntry> Summary { get; set; }

        public List<Tuple<string, double>> DistanceHistogram { get; set; }
    }
}
