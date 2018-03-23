using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using SmartCar.Shared.Model;
using SmartCar.Shared.Rest;

namespace SmartCarUi.Models
{
    public class TripSummaryViewModel
    {
        public List<int> QueryYears { get; set; }

        public List<Vehicle> Vehicles { get; set; }

        public TripSummary Summary { get; set; }
    }
}
