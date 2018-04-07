using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Security.Claims;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authentication;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Configuration;
using Newtonsoft.Json;
using SmartCar.Shared;
using SmartCar.Shared.Rest;
using SmartCarUi.Extensions;

namespace SmartCarUi.Controllers
{
    [Authorize]
    public class UploadController : Controller
    {
        private readonly IConfiguration _configuration;

        public UploadController(IConfiguration configuration)
        {
            _configuration = configuration;
        }

        public IActionResult Index()
        {
            return View();
        }

        [HttpPost]
        public async Task<IActionResult> UploadLogfile(IFormFile logfile)
        {
            var client = await ApiTools.GetAuthenticatedClient(HttpContext);

            using (var content = new MultipartFormDataContent())
            {
                var postStream = new MemoryStream();
                await logfile.CopyToAsync(postStream);
                
                content.Add(new ByteArrayContent(postStream.ToArray()), "logfile", logfile.FileName);
                var response = await client.PostAsync($"{_configuration["Api"]}/api/upload/logfile", content);
                
                if (response.IsSuccessStatusCode)
                {
                    var responseContent = await response.Content.ReadAsStringAsync();
                    var parseResult = JsonConvert.DeserializeObject<ParseResult>(responseContent);

                    return View("Index", parseResult);
                }

                return View("Index", new ParseResult{Success = false});
            }
        }
    }
}