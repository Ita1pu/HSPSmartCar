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
    public class AppUserStore : IQueryableUserStore<AppUser>, 
        IUserPasswordStore<AppUser>,
        IUserRoleStore<AppUser>
    {
        public ApplicationDbContext Context { get; set; }

        public IQueryable<AppUserRole> Roles => Context.Roles;
        public IQueryable<AppUser> Users => Context.Users;

        public AppUserStore(ApplicationDbContext context)
        {
            Context = context ?? throw new ArgumentException("Context");
        }
        
        public Task<string> GetUserIdAsync(AppUser user, CancellationToken cancellationToken)
        {
            throw new NotImplementedException();
        }

        public Task<string> GetUserNameAsync(AppUser user, CancellationToken cancellationToken)
        {
            throw new NotImplementedException();
        }

        public Task SetUserNameAsync(AppUser user, string userName, CancellationToken cancellationToken)
        {
            throw new NotImplementedException();
        }

        public Task<string> GetNormalizedUserNameAsync(AppUser user, CancellationToken cancellationToken)
        {
            throw new NotImplementedException();
        }

        public Task SetNormalizedUserNameAsync(AppUser user, string normalizedName, CancellationToken cancellationToken)
        {
            throw new NotImplementedException();
        }

        public Task<IdentityResult> CreateAsync(AppUser user, CancellationToken cancellationToken)
        {
            if (user == null) throw new ArgumentException("User");

            Context.Users.Add(user);
            return Context.SaveChangesAsync();
        }

        public Task<IdentityResult> UpdateAsync(AppUser user, CancellationToken cancellationToken)
        {
            return Context.SaveChangesAsync();
        }

        public Task<IdentityResult> DeleteAsync(AppUser user, CancellationToken cancellationToken)
        {
            if (user == null) throw new ArgumentException("User");

            var tmpUser = Users.Where(u => u.Id == user.Id).FirstOrDefault();

            if (tmpUser != null)
                Context.Users.Remove(tmpUser);

            return Context.SaveChangesAsync();
        }

        public Task<AppUser> FindByIdAsync(string userId, CancellationToken cancellationToken)
        {
            return Context.Users.FirstOrDefaultAsync(u => u.Id == userId);
        }

        public Task<AppUser> FindByNameAsync(string userName, CancellationToken cancellationToken)
        {
            return Context.Users.FirstOrDefaultAsync(u => u.UserName == userName);
        }

        public Task SetPasswordHashAsync(AppUser user, string passwordHash, CancellationToken cancellationToken)
        {
            user.PasswordHash = passwordHash;
            return Task.FromResult(0);
        }

        public Task<string> GetPasswordHashAsync(AppUser user, CancellationToken cancellationToken)
        {
            return Task.FromResult(user.PasswordHash);
        }

        public Task<bool> HasPasswordAsync(AppUser user, CancellationToken cancellationToken)
        {
            return Task.FromResult(user.PasswordHash != null);
        }
        
        public Task AddToRoleAsync(AppUser user, string roleName, CancellationToken cancellationToken)
        {
            var role = Roles.SingleOrDefault(r => r.Name == roleName);

            if (role != null)
                user.Roles.Add(role);

            await Context.SaveChangesAsync();
        }

        public Task RemoveFromRoleAsync(AppUser user, string roleName, CancellationToken cancellationToken)
        {
            var role = Roles.SingleOrDefault(r => r.Name == roleName);

            if (role != null)
                user.Roles.Remove(role);

            await Context.SaveChangesAsync();
        }

        public Task<IList<string>> GetRolesAsync(AppUser user, CancellationToken cancellationToken)
        {
            if (user == null) throw new ArgumentException("User");

            var query = from userRole in user.Roles
                select userRole.Name;

            return await Task.FromResult(query.ToList());
        }

        public Task<bool> IsInRoleAsync(AppUser user, string roleName, CancellationToken cancellationToken)
        {
            return Task.FromResult(user.Roles.Any(r => r.Name == roleName));
        }

        public Task<IList<AppUser>> GetUsersInRoleAsync(string roleName, CancellationToken cancellationToken)
        {
            throw new NotImplementedException();
        }

        public void Dispose()
        {

        }
    }
}
