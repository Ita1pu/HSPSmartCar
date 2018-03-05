using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using SmartCar.Shared.Database;
using SmartCarApi.Extensions;

namespace SmartCarApi.Controllers
{
    [Authorize]
    [Route("api/trips")]
    public class TripController : Controller
    {
        private Repository _repo;
        private ApplicationDbContext _db;

        public TripController(ApplicationDbContext dbContext)
        {
            _repo = new Repository(dbContext);
            _db = dbContext;
        }

        [HttpGet]
        public IActionResult Get()
        {
            var user = _repo.GetUser(User);

            if (user == null) return Unauthorized();
            return View();
        }
    }
}