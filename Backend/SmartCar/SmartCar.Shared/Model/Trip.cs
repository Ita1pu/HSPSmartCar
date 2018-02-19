﻿using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartCar.Shared.Model
{
    public class Trip
    {
        [Key]
        public int TripId { get; set; }

        public Vehicle Vehicle { get; set; }

        public DateTime TripStart { get; set; }

        public TimeSpan Duration { get; set; }

        public double Distance { get; set; }
    }
}