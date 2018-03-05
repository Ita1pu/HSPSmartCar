using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using SmartCar.Shared.Model;
using SmartCarUi.Models;

namespace SmartCarUi.Controllers
{
    [Authorize]
    public class TripController : Controller
    {
        public IActionResult Index()
        {
            var tripView = new TripsViewModel
            {
                RangeStart = DateTime.Parse("01.01." + DateTime.Now.Year),
                RangeEnd = DateTime.Parse("31.12." + DateTime.Now.Year)
            };
            
            tripView.Trips = GetTrips(tripView.RangeStart, tripView.RangeEnd);

            return View(tripView);
        }

        public IActionResult ShowTrips(TripsViewModel tripsView)
        {
            tripsView.Trips = GetTrips(tripsView.RangeStart, tripsView.RangeEnd);
            return View("Index", tripsView);
        }

        public List<Trip> GetTrips(DateTime rangeStart, DateTime rangeEnd)
        {
            return null;
        }
    }
}