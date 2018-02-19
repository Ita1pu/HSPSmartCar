using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authentication;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Newtonsoft.Json;
using SmartCar.Shared.Model;
using SmartCar.Shared.Rest;
using SmartCarUi.Extensions;

namespace SmartCarUi.Controllers
{
    [Authorize]
    public class VehicleController : Controller
    {
        public async Task<IActionResult> Index()
        {
            //Set result from previous operation
            if (!string.IsNullOrEmpty(TempData["resultMessage"] as string))
            {
                ViewBag.Successful = TempData["successful"];
                ViewBag.ResultMessage = TempData["resultMessage"];
            }

            var vehicles = await GetUserVehicles();

            return View(vehicles);
        }

        [HttpPost]
        public async Task<IActionResult> AddVehicle(Vehicle vehicle)
        {
            var client = await ApiTools.GetAuthenticatedClient(HttpContext);
            var content = ApiTools.GetHttpContent(vehicle);

            var response = await client.PostAsync("http://localhost:5001/api/vehicle", content);

            if (response.IsSuccessStatusCode)
            {
                TempData["successful"] = true;
                TempData["resultMessage"] = "Vehicle added.";
            }
            else
            {
                TempData["successful"] = false;
                TempData["resultMessage"] = "Error occured when adding the vehicle.";
            }

            return RedirectToAction("Index");
        }

        public async Task<IActionResult> RemoveVehicle(int vehicleId)
        {
            var client = await ApiTools.GetAuthenticatedClient(HttpContext);

            var response = await client.DeleteAsync("http://localhost:5001/api/vehicle/" + vehicleId);

            if (response.IsSuccessStatusCode)
            {
                TempData["successful"] = true;
                TempData["resultMessage"] = "Vehicle removed.";
            }
            else
            {
                TempData["successful"] = false;
                TempData["resultMessage"] = "Error occured when removing the vehicle.";
            }

            return RedirectToAction("Index");
        }

        private async Task<IEnumerable<Vehicle>> GetUserVehicles()
        {
            var client = await ApiTools.GetAuthenticatedClient(HttpContext);

            var response = await client.GetAsync("http://localhost:5001/api/vehicle");

            if (response.IsSuccessStatusCode)
            {
                var responseContent = await response.Content.ReadAsStringAsync();
                var vehicles = JsonConvert.DeserializeObject<IEnumerable<Vehicle>>(responseContent);

                return vehicles;
            }

            return null;
        }
    }
}