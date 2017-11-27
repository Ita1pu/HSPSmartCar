using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;

namespace SmartCarIdentity.Identity
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
