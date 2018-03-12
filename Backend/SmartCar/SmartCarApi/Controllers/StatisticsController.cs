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
using SmartCarApi.Statistics.Advanced;

namespace SmartCarApi.Controllers
{
    /// <summary>
    /// Provides an api for getting different statistical data.
    /// </summary>
    /// <seealso cref="Microsoft.AspNetCore.Mvc.Controller" />
    [Authorize]
    [Route("api/statistics")]
    public class StatisticsController : Controller
    {
        private Repository _repo;
        private ApplicationDbContext _db;

        /// <summary>
        /// Initializes a new instance of the <see cref="StatisticsController"/> class.
        /// </summary>
        /// <param name="dbContext">The database context.</param>
        public StatisticsController(ApplicationDbContext dbContext)
        {
            _repo = new Repository(dbContext);
            _db = dbContext;
        }

        /// <summary>
        /// Gets the heatmap data for the calling user in the given timespan.
        /// </summary>
        /// <param name="rangeStart">The query range start.</param>
        /// <param name="rangeEnd">The query range end.</param>
        [HttpGet]
        [Route("heatmap/{rangeStart}/{rangeEnd}")]
        public IActionResult GetHeatmapData(string rangeStart, string rangeEnd)
        {
            var user = _repo.GetUser(User);

            if (user == null) return Unauthorized();

            try
            {
                var startDate = DateTime.Parse(rangeStart);
                var endDate = DateTime.Parse(rangeEnd);

                var trips = _db.Trips.Where(t => t.User.Id == user.Id && t.TripStart > startDate && t.TripStart < endDate)
                    .Include(t => t.TripData).ThenInclude(td => td.SignalType);

                var hmStatistic = new HeatmapStatistic();
                var gps = hmStatistic.GetHeatmapData(trips);

                return Ok(JsonConvert.SerializeObject(gps));
            }
            catch (Exception ex)
            {
                return BadRequest();
            }
        }
    }
}