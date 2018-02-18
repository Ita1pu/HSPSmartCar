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
        public IActionResult Get()
        {
            var user = _repo.GetUser(User);

            if (user != null)
            {
                var vehicles = _db.Vehicles.Where(v => v.Owner.Id == user.Id);
                return Ok(JsonConvert.SerializeObject(vehicles));
            }

            return NotFound();
        }

        /// <summary>
        /// Adds a new vehicle to the authenticated user.
        /// </summary>
        /// <param name="vehicle">The vehicle that shall be added.</param>
        [HttpPost]
        public async Task<IActionResult> Post([FromBody]Vehicle vehicle)
        {
            var user = _repo.GetUser(User);

            if (user != null)
            {
                vehicle.Owner = user;

                _db.Vehicles.Add(vehicle);
                await _db.SaveChangesAsync();

                return Ok();
            }

            return NotFound();
        }
    }
}