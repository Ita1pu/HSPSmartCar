using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Newtonsoft.Json;
using SmartCar.Shared.Database;
using SmartCarApi.Extensions;

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

                var trips = _db.Trips.Where(t => t.User.Id == user.Id
                    && t.TripStart >= startDate && t.TripStart <= endDate);

                return Ok(JsonConvert.SerializeObject(trips));
            }
            catch (Exception ex)
            {
                return BadRequest();
            }
        }
    }
}