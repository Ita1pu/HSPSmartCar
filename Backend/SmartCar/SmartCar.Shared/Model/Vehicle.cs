using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore.Metadata.Internal;
using SmartCar.Shared.Model.Identity;
using System.ComponentModel.DataAnnotations;

namespace SmartCar.Shared.Model
{
    public enum FuelType
    {
        Petrol,
        Diesel
    }

    public class Vehicle
    {
        [Key]
        public int VehicleId { get; set; }

        public AppUser Owner { get; set; }

        public string Vid { get; set; }

        public string LicencePlate { get; set; }

        public string Brand { get; set; }

        public string Model { get; set; }

        public bool IsDefault { get; set; }

        public FuelType FuelType { get; set; }
    }
}
