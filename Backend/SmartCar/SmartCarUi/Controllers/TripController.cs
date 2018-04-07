using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Configuration;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using SmartCar.Shared.Model;
using SmartCarUi.Extensions;
using SmartCarUi.Models;

namespace SmartCarUi.Controllers
{
    [Authorize]
    public class TripController : Controller
    {
        private readonly IConfiguration _configuration;

        public TripController(IConfiguration configuration)
        {
            _configuration = configuration;
        }

        [HttpGet]
        public async Task<IActionResult> Index()
        {
            var tripView = new TripsViewModel
            {
                RangeStart = new DateTime(DateTime.Now.Year, 1, 1),
                RangeEnd = new DateTime(DateTime.Now.Year, 12, 31),
            };

            tripView.Trips = await GetTrips(tripView.RangeStart, tripView.RangeEnd);
            tripView.Vehicles = await GetVehicles();

            return View(tripView);
        }

        [HttpGet]
        public async Task<IActionResult> ShowTrips(TripsViewModel tripsView)
        {
            tripsView.Trips = await GetTrips(tripsView.RangeStart, tripsView.RangeEnd);
            return View("Index", tripsView);
        }

        [HttpGet]
        public async Task<IActionResult> TripDetails(int tripId)
        {
            var trip = await GetTrip(tripId);

            var tripDetailView = new TripViewModel
            {
                Trip = trip
            };

            return View(tripDetailView);
        }

        [HttpPost]
        public async Task<IActionResult> GetGpsTrend([FromBody]dynamic data)
        {
            int tripId = (int)data["tripId"];
            var client = await ApiTools.GetAuthenticatedClient(HttpContext);
            var response = await client.GetAsync($"{_configuration["Api"]}/api/tripdetails/gpstrend/{tripId}");

            if (response.IsSuccessStatusCode)
            {
                var responseContent = await response.Content.ReadAsStringAsync();
                return Ok(responseContent);
            }

            return null;
        }

        [HttpPost]
        public async Task<IActionResult> GetSpeedTrendObd([FromBody]dynamic data)
        {
            int tripId = (int)data["tripId"];
            var client = await ApiTools.GetAuthenticatedClient(HttpContext);
            var response = await client.GetAsync($"{_configuration["Api"]}/api/tripdetails/speedtrend/obd/{tripId}");

            if (response.IsSuccessStatusCode)
            {
                var responseContent = await response.Content.ReadAsStringAsync();
                return Ok(responseContent);
            }

            return null;
        }

        [HttpPost]
        public async Task<IActionResult> GetSpeedTrendGps([FromBody]dynamic data)
        {
            int tripId = (int)data["tripId"];
            var client = await ApiTools.GetAuthenticatedClient(HttpContext);
            var response = await client.GetAsync($"{_configuration["Api"]}/api/tripdetails/speedtrend/gps/{tripId}");

            if (response.IsSuccessStatusCode)
            {
                var responseContent = await response.Content.ReadAsStringAsync();
                return Ok(responseContent);
            }

            return null;
        }

        [HttpPost]
        public async Task<IActionResult> GetRpmTrend([FromBody]dynamic data)
        {
            int tripId = (int)data["tripId"];
            var client = await ApiTools.GetAuthenticatedClient(HttpContext);
            var response = await client.GetAsync($"{_configuration["Api"]}/api/tripdetails/rpmtrend/{tripId}");

            if (response.IsSuccessStatusCode)
            {
                var responseContent = await response.Content.ReadAsStringAsync();
                return Ok(responseContent);
            }

            return null;
        }

        [HttpPost]
        public async Task<IActionResult> GetFuelTrend([FromBody]dynamic data)
        {
            int tripId = (int)data["tripId"];
            var client = await ApiTools.GetAuthenticatedClient(HttpContext);
            var response = await client.GetAsync($"{_configuration["Api"]}/api/tripdetails/fueltrend/{tripId}");

            if (response.IsSuccessStatusCode)
            {
                var responseContent = await response.Content.ReadAsStringAsync();
                return Ok(responseContent);
            }

            return null;
        }

        [HttpPost]
        public async Task<IActionResult> ModifyTrips(TripsActionModel actionView)
        {
            switch (actionView.Action)
            {
                case "removeTrips":
                    await RemoveTrips(actionView);
                    break;
                case "setVehicle":
                    await SetVehicle(actionView);
                    break;
            }

            return RedirectToAction("Index");
        }

        private async Task RemoveTrips(TripsActionModel actionView)
        {
            var client = await ApiTools.GetAuthenticatedClient(HttpContext);

            foreach (var actionViewTrip in actionView.Trips)
            {
                if (actionViewTrip.IsSelected)
                {
                    await client.DeleteAsync($"{_configuration["Api"]}/api/trip/{actionViewTrip.Trip.TripId}");
                }
            }
        }

        private async Task SetVehicle(TripsActionModel actionView)
        {
            var client = await ApiTools.GetAuthenticatedClient(HttpContext);

            foreach (var actionViewTrip in actionView.Trips)
            {
                if (actionViewTrip.IsSelected)
                {
                    var content = ApiTools.GetHttpContent(new Trip
                    {
                        TripId = actionViewTrip.Trip.TripId,
                        Vehicle = new Vehicle { VehicleId = actionView.SelectedVehicleId }
                    });

                    await client.PutAsync($"{_configuration["Api"]}/api/trip/setVehicle", content);
                }
            }
        }

        private async Task<List<Trip>> GetTrips(DateTime rangeStart, DateTime rangeEnd)
        {
            var client = await ApiTools.GetAuthenticatedClient(HttpContext);
            var response = await client.GetAsync($"{_configuration["Api"]}/api/trip/{rangeStart.ToShortDateString()}/{rangeEnd.ToShortDateString()}");

            if (response.IsSuccessStatusCode)
            {
                var responseContent = await response.Content.ReadAsStringAsync();
                var trips = JsonConvert.DeserializeObject<List<Trip>>(responseContent);

                return trips.OrderBy(t => t.TripStart).ToList();
            }

            return new List<Trip>();
        }

        private async Task<List<Vehicle>> GetVehicles()
        {
            var client = await ApiTools.GetAuthenticatedClient(HttpContext);
            var response = await client.GetAsync($"{_configuration["Api"]}/api/vehicle");

            if (response.IsSuccessStatusCode)
            {
                var responseContent = await response.Content.ReadAsStringAsync();
                var trips = JsonConvert.DeserializeObject<List<Vehicle>>(responseContent);

                return trips;
            }

            return new List<Vehicle>();
        }

        private async Task<Trip> GetTrip(int tripId)
        {
            var client = await ApiTools.GetAuthenticatedClient(HttpContext);
            var response = await client.GetAsync($"{_configuration["Api"]}/api/trip/{tripId}");

            if (response.IsSuccessStatusCode)
            {
                var responseContent = await response.Content.ReadAsStringAsync();
                var trip = JsonConvert.DeserializeObject<Trip>(responseContent);

                return trip;
            }

            return new Trip();
        }
    }
}