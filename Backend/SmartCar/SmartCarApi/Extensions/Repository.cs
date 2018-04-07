using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Claims;
using System.Threading.Tasks;
using SmartCar.Shared.Database;
using SmartCar.Shared.Model.Identity;

namespace SmartCarApi.Extensions
{
    public class Repository
    {
        public ApplicationDbContext DbContext { get; set; }

        public Repository(ApplicationDbContext dbContext)
        {
            DbContext = dbContext;
        }

        public AppUser GetUser(ClaimsPrincipal user)
        {
            var userId = user.Claims.FirstOrDefault(c => c.Type == "sub")?.Value;
            return DbContext.Users.Find(userId);
        }
    }
}
