using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartCar.Shared.Rest
{
    public class SummaryEntry
    {
        public TimeSpan TimeDriven { get; set; } = new TimeSpan();

        public double TotalDistance { get; set; } = 0;

        public double AvgFuelConsumption { get; set; } = 0;
    }

    public class TripSummary
    {
        public Dictionary<string, SummaryEntry> Summary { get; set; }

        public List<Tuple<string, double>> DistanceHistogram { get; set; }

        public List<Tuple<string, double>> TimeHistogram { get; set; }
    }
}
