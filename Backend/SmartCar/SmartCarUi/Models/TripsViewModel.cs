using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using SmartCar.Shared.Model;

namespace SmartCarUi.Models
{
    public class TripsViewModel
    {
        public DateTime RangeStart { get; set; }

        public DateTime RangeEnd { get; set; }

        public IEnumerable<Vehicle> Vehicles { get; set; }

        public IEnumerable<Trip> Trips { get; set; }
    }
}
