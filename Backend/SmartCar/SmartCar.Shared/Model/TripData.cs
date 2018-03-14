using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartCar.Shared.Model
{
    public class TripData
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int TripDataId { get; set; }

        public int Mvid { get; set; }

        public DateTime Timestamp { get; set; }

        public SignalMap SignalType { get; set; }

        public int Value { get; set; }

        public override string ToString()
        {
            return $"TripData: Mvid {Mvid}, Timestamp {Timestamp}, SignalType {SignalType?.SignalName}, Value {Value}";
        }
    }
}
