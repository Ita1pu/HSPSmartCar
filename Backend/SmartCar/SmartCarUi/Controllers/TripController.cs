using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Newtonsoft.Json;
using SmartCar.Shared.Model;
using SmartCarUi.Extensions;
using SmartCarUi.Models;

namespace SmartCarUi.Controllers
{
    [Authorize]
    public class TripController : Controller
    {
        public async Task<IActionResult> Index()
        {
            var tripView = new TripsViewModel
            {
                RangeStart = DateTime.Parse("01.01." + DateTime.Now.Year),
                RangeEnd = DateTime.Parse("31.12." + DateTime.Now.Year)
            };
            
            tripView.Trips = await GetTrips(tripView.RangeStart, tripView.RangeEnd);

            return View(tripView);
        }

        public async Task<IActionResult> ShowTrips(TripsViewModel tripsView)
        {
            tripsView.Trips = await GetTrips(tripsView.RangeStart, tripsView.RangeEnd);
            return View("Index", tripsView);
        }

        public async Task<List<Trip>> GetTrips(DateTime rangeStart, DateTime rangeEnd)
        {
            var client = await ApiTools.GetAuthenticatedClient(HttpContext);
            var response = await client.GetAsync($"http://localhost:5001/api/trip/{rangeStart.ToShortDateString()}/{rangeEnd.ToShortDateString()}");

            if(response.IsSuccessStatusCode)
            {
                var responseContent = await response.Content.ReadAsStringAsync();
                var trips = JsonConvert.DeserializeObject<List<Trip>>(responseContent);

                return trips;
            }

            return null;
        }
    }
}