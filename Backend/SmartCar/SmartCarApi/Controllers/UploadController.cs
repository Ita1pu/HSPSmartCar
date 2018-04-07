using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Newtonsoft.Json;
using SmartCar.Shared.Database;
using SmartCar.Shared.Rest;
using SmartCarApi.DataParser;
using SmartCarApi.Extensions;
using SmartCarApi.Statistics.Basic;

namespace SmartCarApi.Controllers
{
    /// <summary>
    /// Provides an api for the upload of trip data.
    /// </summary>
    /// <seealso cref="Microsoft.AspNetCore.Mvc.Controller" />
    [Authorize]
    [Route("api/upload/[action]")]
    public class UploadController : Controller
    {
        private Repository _repo;

        /// <summary>
        /// Initializes a new instance of the <see cref="UploadController"/> class.
        /// </summary>
        /// <param name="dbContext">The database context.</param>
        public UploadController(ApplicationDbContext dbContext)
        {
            _repo = new Repository(dbContext);
        }

        /// <summary>
        /// Parses the given trip data logfile.
        /// </summary>
        /// <param name="logfile">The logfile with trip data that shall be parsed.</param>
        [HttpPost]
        [ProducesResponseType(400)]
        [ProducesResponseType(typeof(ParseResult), 200)]
        public IActionResult Logfile(IFormFile logfile)
        {
            var user = _repo.GetUser(User);
            if (user == null) return Unauthorized();

            var parser = new TripParser(_repo.DbContext, user);
            var parseResult = parser.ParseLogfile(logfile);

            return Ok(JsonConvert.SerializeObject(parseResult));
        }
    }
}
