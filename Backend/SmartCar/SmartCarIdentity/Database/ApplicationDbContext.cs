using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Configuration;
using SmartCarIdentity.Identity;
using SmartCarIdentity.Models.Identity;

namespace SmartCarIdentity.Database
{
    public class ApplicationDbContext : DbContext
    {
        public DbSet<AppUser> Users { get; set; }
        public DbSet<AppUserRole> Roles { get; set; }

        public DbSet<IdentityUserClaim<string>> IdentityUserClaims { get; set; }
        public DbSet<IdentityUserRole<string>>  IdentityUserRoles { get; set; }

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
        }
    }
}

