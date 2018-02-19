using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Newtonsoft.Json;
using SmartCar.Shared.Database;
using SmartCar.Shared.Model;
using SmartCarApi.Extensions;
using SQLitePCL;

namespace SmartCarApi.Controllers
{
    [Authorize]
    [Route("api/[controller]")]
    public class VehicleController : Controller
    {
        private Repository _repo;
        private ApplicationDbContext _db;
        
        /// <summary>
        /// Initializes a new instance of the <see cref="VehicleController"/> class.
        /// </summary>
        /// <param name="dbContext">The database context.</param>
        public VehicleController(ApplicationDbContext dbContext)
        {
            _repo = new Repository(dbContext);
            _db = dbContext;
        }

        /// <summary>
        /// Gets a list of all vehicles of the authenticated user.
        /// </summary>
        [HttpGet]
        [ProducesResponseType(typeof(IEnumerable<Vehicle>), 200)]
        public IActionResult Get()
        {
            var user = _repo.GetUser(User);

            if (user == null) return Unauthorized();

            var vehicles = _db.Vehicles.Where(v => v.Owner.Id == user.Id);
            return Ok(JsonConvert.SerializeObject(vehicles));
        }

        /// <summary>
        /// Adds a new vehicle to the authenticated user.
        /// </summary>
        /// <param name="vehicle">The vehicle that shall be added.</param>
        [HttpPost]
        [ProducesResponseType(200)]
        [ProducesResponseType(400)]
        public async Task<IActionResult> Post([FromBody]Vehicle vehicle)
        {
            var user = _repo.GetUser(User);

            if (user == null) return Unauthorized();

            var existingVehicle = _db.Vehicles.FirstOrDefault(v => v.Owner.Id == user.Id && v.MVID == vehicle.MVID);

            if (existingVehicle == null)
            {
                vehicle.Owner = user;

                _db.Vehicles.Add(vehicle);
                await _db.SaveChangesAsync();

                return Ok();
            }

            return BadRequest();
        }

        /// <summary>
        /// Deletes the specified vehicle.
        /// </summary>
        /// <param name="vehicleId">The vehicle identifier.</param>
        [HttpDelete("{vehicleId}")]
        [ProducesResponseType(200)]
        [ProducesResponseType(404)]
        public async Task<IActionResult> Delete(int vehicleId)
        {
            var user = _repo.GetUser(User);

            if (user == null) return Unauthorized();

            var vehicle = _db.Vehicles.FirstOrDefault(v => v.Owner.Id == user.Id && v.VehicleId == vehicleId);

            if (vehicle != null) 
            {
                _db.Vehicles.Remove(vehicle);
                await _db.SaveChangesAsync();

                return Ok();
            }

            return NotFound();
        }
    }
}