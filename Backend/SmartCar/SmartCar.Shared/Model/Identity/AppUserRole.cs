using System;
using System.Collections.Generic;
using Microsoft.AspNetCore.Identity;

namespace SmartCar.Shared.Model.Identity
{
    public class AppUserRole : IdentityRole
    {
        public AppUserRole()
        {
            Id = Guid.NewGuid().ToString();
        }

        public virtual ICollection<AppUser> Users { get; } = new List<AppUser>();
    }
}
