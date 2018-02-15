using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartCar.Shared.Model
{
    public class TripData
    {
        [Key]
        public int TripDataId { get; set; }

        public SignalMap SignalType { get; set; }

        public int Value { get; set; }
    }
}
