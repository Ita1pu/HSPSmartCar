using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Configuration;
using SmartCar.Shared.Model;
using SmartCar.Shared.Model.Identity;

namespace SmartCar.Shared.Database
{
    public class ApplicationDbContext : DbContext
    {
        #region Identity
        public DbSet<AppUser> Users { get; set; }
        public DbSet<IdentityUserClaim<string>> IdentityUserClaims { get; set; }
        public DbSet<IdentityUserRole<string>> IdentityUserRoles { get; set; }
        public DbSet<AppUserRole> Roles { get; set; }
        public DbSet<IdentityRoleClaim<string>> IdentityRoleClaims { get; set; }
        #endregion

        #region SmartCar
        public DbSet<Vehicle> Vehicles { get; set; }
        public DbSet<Trip> Trips { get; set; }
        public DbSet<TripData> TripData { get; set; }
        public DbSet<SignalMap> SignalMap { get; set; }
        #endregion

        public ApplicationDbContext(DbContextOptions<ApplicationDbContext> options) : base(options)
        {

        }

        protected override void OnModelCreating(ModelBuilder builder)
        {
            builder.Entity<AppUser>(i => {
                i.ToTable("Users");
                i.HasKey(x => x.Id);
            });
            builder.Entity<AppUserRole>(i => {
                i.ToTable("Role");
                i.HasKey(x => x.Id);
            });
            builder.Entity<IdentityUserRole<string>>(i => {
                i.ToTable("UserRole");
                i.HasKey(x => new { x.RoleId, x.UserId });
            });
            builder.Entity<IdentityUserClaim<string>>(i => {
                i.ToTable("UserClaims");
                i.HasKey(x => x.Id);
            });
            builder.Entity<IdentityRoleClaim<string>>(i => {
                i.ToTable("RoleClaims");
                i.HasKey(x => x.Id);
            });


        }
    }
}

