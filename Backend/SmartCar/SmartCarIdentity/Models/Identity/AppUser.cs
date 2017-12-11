using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using Microsoft.AspNetCore.Identity;
using SmartCarIdentity.Models.Identity;

namespace SmartCarIdentity.Identity
{
    public class AppUser : IdentityUser
    {
        public AppUser()
        {
            Id = Guid.NewGuid().ToString();
        }
        
        public virtual ICollection<AppUserRole> Roles { get; } = new List<AppUserRole>();
    }
}
