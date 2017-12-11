using System;
using System.Collections.Generic;
using Microsoft.AspNetCore.Identity;
using SmartCarIdentity.Identity;

namespace SmartCarIdentity.Models.Identity
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
