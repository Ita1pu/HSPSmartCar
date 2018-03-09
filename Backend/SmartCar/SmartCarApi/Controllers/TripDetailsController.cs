using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Newtonsoft.Json;
using SmartCar.Shared.Database;
using SmartCarApi.Extensions;
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

        [HttpGet]
        [Route("gpstrend/{tripId}")]
        public IActionResult GetGpsTrend(int tripId)
        {
            var user = _repo.GetUser(User);

            if (user == null) return Unauthorized();

            var trip = _db.Trips.Include(t => t.Vehicle)
                .Include(t => t.TripData).ThenInclude(d => d.SignalType)
                .FirstOrDefault(t => t.User.Id == user.Id && t.TripId == tripId);

            if (trip != null)
            {
                var tripStatistic = new TripStatistic();
                var gpsTrend = tripStatistic.GetGpsTrend(trip);
                return Ok(JsonConvert.SerializeObject(gpsTrend.OrderBy(i => i.Item1)));
            }

            return NotFound();
        }

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
    }
}