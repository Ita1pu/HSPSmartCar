using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Newtonsoft.Json;
using SmartCar.Shared.Database;
using SmartCar.Shared.Model;
using SmartCarApi.Extensions;
using SmartCarApi.Statistics.Advanced;
using SmartCarApi.Statistics.Basic;

namespace SmartCarApi.Controllers
{
    /// <summary>
    /// Provides an api for trip management.
    /// </summary>
    /// <seealso cref="Microsoft.AspNetCore.Mvc.Controller" />
    [Authorize]
    [Route("api/trip")]
    public class TripController : Controller
    {
        private Repository _repo;
        private ApplicationDbContext _db;

        /// <summary>
        /// Initializes a new instance of the <see cref="TripController"/> class.
        /// </summary>
        /// <param name="dbContext">The database context.</param>
        public TripController(ApplicationDbContext dbContext)
        {
            _repo = new Repository(dbContext);
            _db = dbContext;
        }

        /// <summary>
        /// Gets the trips of the specified user filtered by date.
        /// </summary>
        /// <param name="rangeStart">The query range start.</param>
        /// <param name="rangeEnd">The query range end.</param>
        [HttpGet]
        [Route("{rangeStart}/{rangeEnd}")]
        public IActionResult Get(string rangeStart, string rangeEnd)
        {
            var user = _repo.GetUser(User);

            if (user == null) return Unauthorized();

            try
            {
                var startDate = DateTime.Parse(rangeStart);
                var endDate = DateTime.Parse(rangeEnd);

                var trips = _db.Trips.Include(t => t.Vehicle).Where(t => t.User.Id == user.Id
                                                                         && t.TripStart > startDate &&
                                                                         t.TripStart < endDate);

                return Ok(JsonConvert.SerializeObject(trips));
            }
            catch (Exception ex)
            {
                return BadRequest();
            }
        }

        /// <summary>
        /// Gets the trip identified by the given trip id.
        /// </summary>
        /// <param name="tripId">The trip identifier.</param>
        [HttpGet]
        [Route("{tripId}")]
        public IActionResult GetTrip(int tripId)
        {
            var user = _repo.GetUser(User);

            if (user == null) return Unauthorized();

            var trip = _db.Trips.Include(t => t.Vehicle).FirstOrDefault(t => t.User.Id == user.Id && t.TripId == tripId);

            if (trip != null)
            {
                return Ok(JsonConvert.SerializeObject(trip));
            }

            return NotFound();
        }

        /// <summary>
        /// Deletes the specified trip.
        /// </summary>
        /// <param name="tripId">The trip identifier.</param>
        [HttpDelete]
        [Route("{tripId}")]
        public async Task<IActionResult> Delete(int tripId)
        {
            var user = _repo.GetUser(User);

            if (user == null) return Unauthorized();

            var trip = _db.Trips.Include(t => t.TripData)
                .FirstOrDefault(t => t.User.Id == user.Id && t.TripId == tripId);

            if (trip != null)
            {
                _db.TripData.RemoveRange(trip.TripData);
                _db.Trips.Remove(trip);
                await _db.SaveChangesAsync();

                return Ok();
            }

            return NotFound();
        }
        
        /// <summary>
        /// Sets the vehicle of the given trip identified by the trip id.
        /// </summary>
        /// <param name="trip">The trip thats vehicle shall be set.</param>
        [HttpPut]
        [Route("setVehicle")]
        public async Task<IActionResult> SetVehicle([FromBody]Trip trip)
        {
            var user = _repo.GetUser(User);

            if (user == null) return Unauthorized();

            var dbTrip = _db.Trips.Include(t => t.Vehicle)
                .Where(t => t.User.Id == user.Id && t.TripId == trip.TripId)
                .Include(t => t.TripData).ThenInclude(td => td.SignalType).FirstOrDefault();

            var dbVehicle =
                _db.Vehicles.FirstOrDefault(v => v.Owner.Id == user.Id && v.VehicleId == trip.Vehicle.VehicleId);

            if (dbTrip != null)
            {
                dbTrip.Vehicle = dbVehicle;
                RecalculateFuelConsumption(dbTrip);

                await _db.SaveChangesAsync();

                return Ok();
            }

            return NotFound();
        }

        private void RecalculateFuelConsumption(Trip trip)
        {
            var tripStatistic = new TripStatistic();
            var fuelConsumption = new FuelConsumption(trip.Vehicle?.FuelType == FuelType.Diesel);
            var speedTrend = tripStatistic.GetSpeedTrendObd(trip);
            fuelConsumption.CalcFuelConsumption(trip, speedTrend);
        }
    }
}