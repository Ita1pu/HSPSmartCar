using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using SmartCar.Shared;
using Microsoft.AspNetCore.Http;

namespace SmartCarApi.DataParser
{
    public class LogfileParser
    {
        public ParseResult Parse(IFormFile file)
        {
            var result = new ParseResult();

            result.Success = true;
            result.ParsedTrips = 2;

            return result;
        }
    }
}
