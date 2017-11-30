using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Claims;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
using SmartCarIdentity.Identity;
using SmartCarIdentity.Models.Identity;

namespace SmartCarIdentity.Database
{
    public static class DatabaseInitializer
    {
        public static async void Initialize(ApplicationDbContext context, UserManager<AppUser> userManager, RoleManager<AppUserRole> roleManager)
        {
            context.Database.EnsureCreated();

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
