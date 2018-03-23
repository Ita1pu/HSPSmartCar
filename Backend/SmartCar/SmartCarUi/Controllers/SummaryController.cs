using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Newtonsoft.Json;
using SmartCar.Shared.Model;
using SmartCar.Shared.Rest;
using SmartCarUi.Extensions;
using SmartCarUi.Models;

namespace SmartCarUi.Controllers
{
    [Authorize]
    public class SummaryController : Controller
    {
        public async Task<IActionResult> Index()
        {
            var vehicles = await GetVehicles();

            var viewModel = new TripSummaryViewModel
            {
                Vehicles = vehicles,
                QueryYears = new List<int> { 2016, 2017, 2018 }
            };

            return View(viewModel);
        }

        [HttpPost]
        public async Task<IActionResult> CalculateSummary(int year, int vehicleId)
        {
            var vehicles = await GetVehicles();
            var summary = await GetSummary(year, vehicleId);

            var viewModel = new TripSummaryViewModel
            {
                Vehicles = vehicles,
                QueryYears = new List<int> { 2016, 2017, 2018 },
                Summary = summary
            };

            return View("Index", viewModel);
        }

        private async Task<List<Vehicle>> GetVehicles()
        {
            var client = await ApiTools.GetAuthenticatedClient(HttpContext);
            var response = await client.GetAsync("http://localhost:5001/api/vehicle");

            if (response.IsSuccessStatusCode)
            {
                var responseContent = await response.Content.ReadAsStringAsync();
                var trips = JsonConvert.DeserializeObject<List<Vehicle>>(responseContent);

                return trips;
            }

            return null;
        }

        private async Task<TripSummary> GetSummary(int year, int vehicleId)
        {
            var client = await ApiTools.GetAuthenticatedClient(HttpContext);
            var response = await client.GetAsync($"http://localhost:5001/api/statistics/summary/{year}/{vehicleId}");

            if (response.IsSuccessStatusCode)
            {
                var responseContent = await response.Content.ReadAsStringAsync();
                var summary = JsonConvert.DeserializeObject<TripSummary>(responseContent);

                return summary;
            }

            return null;
        }
    }
}