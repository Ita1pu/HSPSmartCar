using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Configuration;
using SmartCarUi.Extensions;
using SmartCarUi.Models;

namespace SmartCarUi.Controllers
{
    [Authorize]
    public class HeatmapController : Controller
    {
        private readonly IConfiguration _configuration;

        public HeatmapController(IConfiguration configuration)
        {
            _configuration = configuration;
        }

        public IActionResult Index()
        {
            var autoRange = new RangeViewModel
            {
                RangeStart = DateTime.Parse("01.01." + DateTime.Now.Year),
                RangeEnd = DateTime.Parse("31.12." + DateTime.Now.Year)
            };

            return View(autoRange);
        }
        
        [HttpPost]
        public async Task<IActionResult> GetHeatmap([FromBody]dynamic data)
        {
            var rangeStart = DateTime.Parse(data["rangeStart"].ToString());
            var rangeEnd = DateTime.Parse(data["rangeEnd"].ToString());

            var requestUri =
                $"{_configuration["Api"]}/api/statistics/heatmap/{rangeStart.ToShortDateString()}/{rangeEnd.ToShortDateString()}";

            var client = await ApiTools.GetAuthenticatedClient(HttpContext);
            var response =
                await client.GetAsync(
                    $"{_configuration["Api"]}/api/statistics/heatmap/{rangeStart.ToShortDateString()}/{rangeEnd.ToShortDateString()}");

            if (response.IsSuccessStatusCode)
            {
                var responseContent = await response.Content.ReadAsStringAsync();
                return Ok(responseContent);
            }

            return BadRequest();
        }
    }
}