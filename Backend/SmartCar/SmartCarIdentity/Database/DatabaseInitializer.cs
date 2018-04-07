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

            context.SignalMap.Add(new SignalMap { SignalId = 0x04, SignalName = Signal.EngineLoad });
            context.SignalMap.Add(new SignalMap { SignalId = 0x05, SignalName = Signal.EngineCoolantTemp });
            context.SignalMap.Add(new SignalMap { SignalId = 0x0C, SignalName = Signal.EngineRpm });
            context.SignalMap.Add(new SignalMap { SignalId = 0x0D, SignalName = Signal.VehicleSpeed });
            context.SignalMap.Add(new SignalMap { SignalId = 0x10, SignalName = Signal.MAFAirFlow });
            context.SignalMap.Add(new SignalMap { SignalId = 0x1F, SignalName = Signal.RunTimeSineEngineStart });
            context.SignalMap.Add(new SignalMap { SignalId = 0x21, SignalName = Signal.DistTraveledWithMalfuncIndicaLamp });
            context.SignalMap.Add(new SignalMap { SignalId = 0x24, SignalName = Signal.OxySensor01 });
            context.SignalMap.Add(new SignalMap { SignalId = 0x25, SignalName = Signal.OxySensor02 });
            context.SignalMap.Add(new SignalMap { SignalId = 0x26, SignalName = Signal.OxySensor03 });
            context.SignalMap.Add(new SignalMap { SignalId = 0x27, SignalName = Signal.OxySensor04 });
            context.SignalMap.Add(new SignalMap { SignalId = 0x28, SignalName = Signal.OxySensor05 });
            context.SignalMap.Add(new SignalMap { SignalId = 0x29, SignalName = Signal.OxySensor06 });
            context.SignalMap.Add(new SignalMap { SignalId = 0x2A, SignalName = Signal.OxySensor07 });
            context.SignalMap.Add(new SignalMap { SignalId = 0x2B, SignalName = Signal.OxySensor08 });
            context.SignalMap.Add(new SignalMap { SignalId = 0x2F, SignalName = Signal.FuelTankLvlInput });
            context.SignalMap.Add(new SignalMap { SignalId = 0x33, SignalName = Signal.AbsBarometricPressure });
            context.SignalMap.Add(new SignalMap { SignalId = 0x34, SignalName = Signal.OxySensor11 });
            context.SignalMap.Add(new SignalMap { SignalId = 0x35, SignalName = Signal.OxySensor12 });
            context.SignalMap.Add(new SignalMap { SignalId = 0x36, SignalName = Signal.OxySensor13 });
            context.SignalMap.Add(new SignalMap { SignalId = 0x37, SignalName = Signal.OxySensor14 });
            context.SignalMap.Add(new SignalMap { SignalId = 0x38, SignalName = Signal.OxySensor15 });
            context.SignalMap.Add(new SignalMap { SignalId = 0x39, SignalName = Signal.OxySensor16 });
            context.SignalMap.Add(new SignalMap { SignalId = 0x3A, SignalName = Signal.OxySensor17 });
            context.SignalMap.Add(new SignalMap { SignalId = 0x3B, SignalName = Signal.OxySensor18 });
            context.SignalMap.Add(new SignalMap { SignalId = 0x46, SignalName = Signal.AmbientAirTemp });
            context.SignalMap.Add(new SignalMap { SignalId = 0x4F, SignalName = Signal.TestEquipConf1 });
            context.SignalMap.Add(new SignalMap { SignalId = 0x50, SignalName = Signal.TestEquipConf2 });
            context.SignalMap.Add(new SignalMap { SignalId = 0x51, SignalName = Signal.FuelType });
            context.SignalMap.Add(new SignalMap { SignalId = 0x52, SignalName = Signal.EthanolPercent });
            context.SignalMap.Add(new SignalMap { SignalId = 0x5A, SignalName = Signal.RelativAcceleratorPedalPos });
            context.SignalMap.Add(new SignalMap { SignalId = 0x5C, SignalName = Signal.EngineOilTemp });
            context.SignalMap.Add(new SignalMap { SignalId = 0x5E, SignalName = Signal.EngineFuelRate });
            context.SignalMap.Add(new SignalMap { SignalId = 0x61, SignalName = Signal.DriverTorqueDemandEngine });
            context.SignalMap.Add(new SignalMap { SignalId = 0x62, SignalName = Signal.ActualTorqueEngine });
            context.SignalMap.Add(new SignalMap { SignalId = 0x63, SignalName = Signal.EngineTorqueRef });
            context.SignalMap.Add(new SignalMap { SignalId = 0x7F, SignalName = Signal.EngineRunTime });
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
