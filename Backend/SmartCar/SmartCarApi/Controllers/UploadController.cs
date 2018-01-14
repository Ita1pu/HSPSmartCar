using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Newtonsoft.Json;
using SmartCarApi.DataParser;

namespace SmartCarApi.Controllers
{
    [Route("api/[controller]/[action]")]
    public class UploadController : Controller
    {
        [HttpPost]
        public IActionResult Logfile(IFormFile logfile)
        {
            var parser = new LogfileParser();
            var parseResult = parser.Parse(logfile);
            
            return Ok(JsonConvert.SerializeObject(parseResult));
        }
    }
}
