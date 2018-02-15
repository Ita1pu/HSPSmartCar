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
    public class Vehicle
    {
        [Key]
        public int VehicleId { get; set; }

        public AppUser Owner { get; set; }

        public string VID { get; set; }

        public int MVID { get; set; }

        public string Brand { get; set; }

        public string Model { get; set; }
    }
}
