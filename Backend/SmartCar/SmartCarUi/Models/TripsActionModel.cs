using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using SmartCar.Shared.Model;

namespace SmartCarUi.Models
{
    public class TripSelector
    {
        public bool IsSelected { get; set; }
        public Trip Trip { get; set; }
    }

    public class TripsActionModel
    {
        public string Action { get; set; }

        public int SelectedVehicleId { get; set; }

        public List<TripSelector> Trips { get; set; }
    }
}
