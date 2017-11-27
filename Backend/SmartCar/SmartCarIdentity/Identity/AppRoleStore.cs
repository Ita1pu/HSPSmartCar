using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore;
using SmartCarIdentity.Database;

namespace SmartCarIdentity.Identity
{
    public class AppRoleStore : IQueryableRoleStore<AppUserRole>
    {
        public ApplicationDbContext Context { get; set; }

        public IQueryable<AppUserRole> Roles => Context.Roles;

        public AppRoleStore(ApplicationDbContext context)
        {
            Context = context ?? throw new ArgumentException("Context");
        }
        
        public Task<IdentityResult> CreateAsync(AppUserRole role, CancellationToken cancellationToken)
        {
            if (role == null) throw new ArgumentException("Role");

            Context.Roles.Add(role);
            return Context.SaveChangesAsync();
        }

        public Task<IdentityResult> UpdateAsync(AppUserRole role, CancellationToken cancellationToken)
        {
            return Context.SaveChangesAsync();
        }

        public Task<IdentityResult> DeleteAsync(AppUserRole role, CancellationToken cancellationToken)
        {
            if (role == null) throw new ArgumentException("Role");

            Context.Roles.Remove(role);
            return Context.SaveChangesAsync();
        }

        public Task<string> GetRoleIdAsync(AppUserRole role, CancellationToken cancellationToken)
        {
            return Context.Roles.Find(role)?.Id;
        }

        public Task<string> GetRoleNameAsync(AppUserRole role, CancellationToken cancellationToken)
        {
            return Context.Roles.Find(role)?.Name;
        }

        public Task SetRoleNameAsync(AppUserRole role, string roleName, CancellationToken cancellationToken)
        {
            throw new NotImplementedException();
        }

        public Task<string> GetNormalizedRoleNameAsync(AppUserRole role, CancellationToken cancellationToken)
        {
            throw new NotImplementedException();
        }

        public Task SetNormalizedRoleNameAsync(AppUserRole role, string normalizedName, CancellationToken cancellationToken)
        {
            throw new NotImplementedException();
        }

        public Task<AppUserRole> FindByIdAsync(string roleId, CancellationToken cancellationToken)
        {
            return Context.Roles.FirstOrDefaultAsync(r => r.Id == roleId, cancellationToken: cancellationToken);
        }

        public Task<AppUserRole> FindByNameAsync(string roleName, CancellationToken cancellationToken)
        {
            return Context.Roles.FirstOrDefaultAsync(r => r.Name == roleName, cancellationToken: cancellationToken);
        }
        
        public void Dispose()
        {

        }
    }
}
