using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Claims;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using SmartCarIdentity.Identity;
using SmartCarIdentity.Models.Identity;

namespace SmartCarIdentity.Database
{
    public static class DatabaseInitializer
    {
        public static async void Initialize(IServiceProvider services)
        {
            var context = services.GetRequiredService<ApplicationDbContext>();
            var userManager = services.GetRequiredService<UserManager<AppUser>>();
            var roleManager = services.GetRequiredService<RoleManager<AppUserRole>>();

            await context.Database.EnsureCreatedAsync();

            //Databse was already seeded
            if (context.Users.Any()) return;

            var adminName = "admin";
            var adminPassword = "admin";

            AppUser adminUser = new AppUser{UserName = adminName };
            await userManager.CreateAsync(adminUser, adminPassword);

            AppUserRole role = new AppUserRole{Name = "Default"};
            await roleManager.CreateAsync(role);

            await userManager.AddToRoleAsync(adminUser, "Default");
        }
    }
}
