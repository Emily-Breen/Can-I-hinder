using CanI_HinderAPI.Models;
using Microsoft.EntityFrameworkCore;

namespace CanI_HinderAPI.Data;
//the AppDbContext class is the backbone of the data access layer,
//its for interacting with the db and it defines the tables used.
public class AppDbContext : DbContext
{
    //constructor that allows the options to be passed when the context is created which is done in program.cs when we add the db context to the services,
    //this allows us to configure the db connection string and other options from there.
    public AppDbContext(DbContextOptions<AppDbContext> options) : base(options) { }
    //DBset for users,user preferences and analytics events which reps the tables in the database and allows to query  and save instances 
    public DbSet<User> Users => Set<User>();
    public DbSet<UserPreference> UserPreferences => Set<UserPreference>();
    public DbSet<AnalyticsEvent> AnalyticsEvents => Set<AnalyticsEvent>();
    // This method sets up how the database tables are organized.
    // It runs when the model is created and defines things like indexes
    // and relationships between tables. (user and userpreferences = id in user, UserID in userpreferences points to a specific user)
    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        base.OnModelCreating(modelBuilder);

        modelBuilder.Entity<User>()
            .HasIndex(u => u.Username)
            .IsUnique();

        modelBuilder.Entity<UserPreference>()
            .HasIndex(p => new { p.UserId, p.Key })
            .IsUnique();
    }
}