using System;
using System.Collections.Generic;
using System.Diagnostics;
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
    [Authorize]
    [Route("api/tripdetails")]
    public class TripDetailsController : Controller
    {
        private Repository _repo;
        private ApplicationDbContext _db;

        /// <summary>
        /// Initializes a new instance of the <see cref="TripDetailsController"/> class.
        /// </summary>
        /// <param name="dbContext">The database context.</param>
        public TripDetailsController(ApplicationDbContext dbContext)
        {
            _repo = new Repository(dbContext);
            _db = dbContext;
        }

        /// <summary>
        /// Gets the GPS trend.
        /// </summary>
        /// <param name="tripId">The trip identifier.</param>
        [HttpGet]
        [Route("gpstrend/{tripId}")]
        public IActionResult GetGpsTrend(int tripId)
        {
            var user = _repo.GetUser(User);

            if (user == null) return Unauthorized();

            var trip = _db.Trips.Include(t => t.Vehicle).Include(t => t.TripData).ThenInclude(d => d.SignalType)
                .FirstOrDefault(t => t.User.Id == user.Id && t.TripId == tripId);

            if (trip != null)
            {
                var tripStatistic = new TripStatistic();
                var gpsTrend = tripStatistic.GetGpsTrend(trip);
                return Ok(JsonConvert.SerializeObject(gpsTrend.OrderBy(i => i.Item1)));
            }

            return NotFound();
        }

        /// <summary>
        /// Gets the speed trend based on OBD data.
        /// </summary>
        /// <param name="tripId">The trip identifier.</param>
        [HttpGet]
        [Route("speedtrend/obd/{tripId}")]
        public IActionResult GetSpeedTrendObd(int tripId)
        {
            var user = _repo.GetUser(User);

            if (user == null) return Unauthorized();

            var trip = _db.Trips.Include(t => t.Vehicle)
                .Include(t => t.TripData).ThenInclude(d => d.SignalType)
                .FirstOrDefault(t => t.User.Id == user.Id && t.TripId == tripId);

            if (trip != null)
            {
                var tripStatistic = new TripStatistic();
                var speedTrend = tripStatistic.GetSpeedTrendObd(trip);
                return Ok(JsonConvert.SerializeObject(speedTrend.OrderBy(i => i.Item1)));
            }

            return NotFound();
        }

        /// <summary>
        /// Gets the speed trend based on GPS data.
        /// </summary>
        /// <param name="tripId">The trip identifier.</param>
        [HttpGet]
        [Route("speedtrend/gps/{tripId}")]
        public IActionResult GetSpeedTrendGps(int tripId)
        {
            var user = _repo.GetUser(User);

            if (user == null) return Unauthorized();

            var trip = _db.Trips.Include(t => t.Vehicle)
                .Include(t => t.TripData).ThenInclude(d => d.SignalType)
                .FirstOrDefault(t => t.User.Id == user.Id && t.TripId == tripId);

            if (trip != null)
            {
                var tripStatistic = new TripStatistic();
                var speedTrend = tripStatistic.GetSpeedTrendGps(trip);
                return Ok(JsonConvert.SerializeObject(speedTrend.OrderBy(i => i.Item1)));
            }

            return NotFound();
        }

        /// <summary>
        /// Gets the RPM trend for the given trip.
        /// </summary>
        /// <param name="tripId">The trip identifier.</param>
        [HttpGet]
        [Route("rpmtrend/{tripId}")]
        public IActionResult GetRpmTrend(int tripId)
        {
            var user = _repo.GetUser(User);

            if (user == null) return Unauthorized();

            var trip = _db.Trips.Include(t => t.Vehicle)
                .Include(t => t.TripData).ThenInclude(d => d.SignalType)
                .FirstOrDefault(t => t.User.Id == user.Id && t.TripId == tripId);

            if (trip != null)
            {
                var tripStatistic = new TripStatistic();
                var speedTrend = tripStatistic.GetRpmTrend(trip);
                return Ok(JsonConvert.SerializeObject(speedTrend.OrderBy(i => i.Item1)));
            }

            return NotFound();
        }

        /// <summary>
        /// Gets the fuel trend for the given trip.
        /// </summary>
        /// <param name="tripId">The trip identifier.</param>
        [HttpGet]
        [Route("fueltrend/{tripId}")]
        public IActionResult GetFuelTrend(int tripId)
        {
            var user = _repo.GetUser(User);

            if (user == null) return Unauthorized();

            var trip = _db.Trips.Include(t => t.Vehicle)
                .Include(t => t.TripData).ThenInclude(d => d.SignalType)
                .FirstOrDefault(t => t.User.Id == user.Id && t.TripId == tripId);

            if (trip != null)
            {
                var tripStatistic = new TripStatistic();
                var fuelConsumption = new FuelConsumption(trip.Vehicle?.FuelType == FuelType.Diesel);
                var speedTrend = tripStatistic.GetSpeedTrendObd(trip);

                var consumption = fuelConsumption.CalcFuelConsumption(trip, speedTrend);

                return Ok(JsonConvert.SerializeObject(consumption.OrderBy(c => c.Item1)));
            }

            return NotFound();
        }
    }
}