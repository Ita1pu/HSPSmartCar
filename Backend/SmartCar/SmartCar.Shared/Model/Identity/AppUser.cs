using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using Microsoft.AspNetCore.Identity;

namespace SmartCar.Shared.Model.Identity
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
