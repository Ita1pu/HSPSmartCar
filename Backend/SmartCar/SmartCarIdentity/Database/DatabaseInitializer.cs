using System;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
using Microsoft.Extensions.DependencyInjection;
using SmartCar.Shared.Database;
using SmartCar.Shared.Model;
using SmartCar.Shared.Model.Identity;

namespace SmartCarIdentity.Database
{
    public static class DatabaseInitializer
    {
        public static async void Initialize(IServiceProvider services)
        {
            var context = services.GetRequiredService<ApplicationDbContext>();

            await context.Database.EnsureCreatedAsync();

            //Databse was already seeded
            if (context.Users.Any()) return;

            //Create default users
            await InitializeUsers(services);

            //Create signal map
            await InitializeSignalMap(services);
        }

        private static async Task InitializeUsers(IServiceProvider services)
        {
            var userManager = services.GetRequiredService<UserManager<AppUser>>();
            var roleManager = services.GetRequiredService<RoleManager<AppUserRole>>();

            var adminName = "admin";
            var adminPassword = "admin";

            AppUser adminUser = new AppUser { UserName = adminName };
            await userManager.CreateAsync(adminUser, adminPassword);

            AppUserRole role = new AppUserRole { Name = "Default" };
            await roleManager.CreateAsync(role);

            await userManager.AddToRoleAsync(adminUser, "Default");
        }

        private static async Task InitializeSignalMap(IServiceProvider services)
        {
            var context = services.GetRequiredService<ApplicationDbContext>();

            context.SignalMap.Add(new SignalMap { SignalId = 0x05, SignalName = Signal.EngineCoolantTemp });
            context.SignalMap.Add(new SignalMap { SignalId = 0x0C, SignalName = Signal.EngineRpm });
            context.SignalMap.Add(new SignalMap { SignalId = 0x0D, SignalName = Signal.VehicleSpeed });
            context.SignalMap.Add(new SignalMap { SignalId = 0x1F, SignalName = Signal.RunTimeSineEngineStart });
            context.SignalMap.Add(new SignalMap { SignalId = 0x21, SignalName = Signal.DistTraveledWithMalfuncIndicaLamp });
            context.SignalMap.Add(new SignalMap { SignalId = 0x2F, SignalName = Signal.FuelTankLvlInput });
            context.SignalMap.Add(new SignalMap { SignalId = 0x33, SignalName = Signal.AbsBarometricPressure });
            context.SignalMap.Add(new SignalMap { SignalId = 0x46, SignalName = Signal.AbientAirTemp });
            context.SignalMap.Add(new SignalMap { SignalId = 0x51, SignalName = Signal.FuleType });
            context.SignalMap.Add(new SignalMap { SignalId = 0x52, SignalName = Signal.EthanolPercent });
            context.SignalMap.Add(new SignalMap { SignalId = 0x5A, SignalName = Signal.RelativeAcceleratorPedalPos });
            context.SignalMap.Add(new SignalMap { SignalId = 0x5C, SignalName = Signal.EngineOilTemp });
            context.SignalMap.Add(new SignalMap { SignalId = 0x5E, SignalName = Signal.EngineFuleRate });
            context.SignalMap.Add(new SignalMap { SignalId = 0x61, SignalName = Signal.DriverTourqueDemandEngine });
            context.SignalMap.Add(new SignalMap { SignalId = 0x62, SignalName = Signal.ActualTorqueEngine });
            context.SignalMap.Add(new SignalMap { SignalId = 0x63, SignalName = Signal.EngineTorqueRef });
            context.SignalMap.Add(new SignalMap { SignalId = 0x7F, SignalName = Signal.EngineRuntime });
            context.SignalMap.Add(new SignalMap { SignalId = 0xF0, SignalName = Signal.GpsLatitude });
            context.SignalMap.Add(new SignalMap { SignalId = 0xF1, SignalName = Signal.GpsLongitude });
            context.SignalMap.Add(new SignalMap { SignalId = 0xF2, SignalName = Signal.GpsSatCount });
            context.SignalMap.Add(new SignalMap { SignalId = 0xF3, SignalName = Signal.AccelXAxis });
            context.SignalMap.Add(new SignalMap { SignalId = 0xF4, SignalName = Signal.AccelYAxis });
            context.SignalMap.Add(new SignalMap { SignalId = 0xF5, SignalName = Signal.AccelZAxis });
            context.SignalMap.Add(new SignalMap { SignalId = 0xF6, SignalName = Signal.GyroXAxis });
            context.SignalMap.Add(new SignalMap { SignalId = 0xF7, SignalName = Signal.GyroYAxis });
            context.SignalMap.Add(new SignalMap { SignalId = 0xF8, SignalName = Signal.GyroZAxis });

            await context.SaveChangesAsync();
        }
    }
}
