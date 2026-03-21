using CanI_HinderAPI.Data;
using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.EntityFrameworkCore;
using Microsoft.IdentityModel.Tokens;
using Microsoft.OpenApi.Models;
using CanI_HinderAPI.Services;
using System.Text;

//builder = initialises the application and sets up the services and middleware that use incoming requests to the API from Pwa and game clients.
var builder = WebApplication.CreateBuilder(args);

//Database setup using SQL Server , with connection string from appsettings.json
builder.Services.AddDbContext<AppDbContext>(options =>
    options.UseSqlServer(builder.Configuration.GetConnectionString("Default")));

// CORS setup, allowing origins specified in appsettings.json under "AllowedOrigins" (as an array of strings)
const string CorsPolicy = "PwaCors";
var allowedOrigins = builder.Configuration.GetSection("AllowedOrigins").Get<string[]>() ?? Array.Empty<string>();
// if no allowed origins are specified, it can default to allowing all origins for development purposes but prod for explicit config 
builder.Services.AddCors(options =>
{
    options.AddPolicy(CorsPolicy, policy =>
    {
        policy.WithOrigins(allowedOrigins)
              .AllowAnyHeader()
              .AllowAnyMethod();
    });
});

// JWT Authentication setup, with parameters from appsettings.json under "Jwt" section (Key, Issuer, Audience)
var jwtKey = builder.Configuration["Jwt:Key"];
var jwtIssuer = builder.Configuration["Jwt:Issuer"];
var jwtAudience = builder.Configuration["Jwt:Audience"];

// Only enforce JWT config if present to allow flexibility in development without authentication if desired
if (string.IsNullOrWhiteSpace(jwtKey))
{
    if (builder.Environment.IsProduction())
        throw new InvalidOperationException("JWT signing key not configured (Jwt:Key).");
}
else
{
    // config for JWT auth that validates the tokens based on jwtKey, jwtIssuer, and jwtAudiencw with a clock to allow for
    // one minute of skew to account for time differences between token issed and validation.
    builder.Services.AddAuthentication(JwtBearerDefaults.AuthenticationScheme)
        .AddJwtBearer(options =>
        {
            options.TokenValidationParameters = new TokenValidationParameters
            {
                ValidateIssuer = true,
                ValidIssuer = jwtIssuer,

                ValidateAudience = true,
                ValidAudience = jwtAudience,

                ValidateIssuerSigningKey = true,
                IssuerSigningKey = new SymmetricSecurityKey(Encoding.UTF8.GetBytes(jwtKey)),

                ValidateLifetime = true,
                ClockSkew = TimeSpan.FromMinutes(1)
            };
        });
}
// Even if JWT is not configured, we still add the Authorization services to allow for [Authorize] attributes in controllers without causing errors.
builder.Services.AddAuthorization();
// Add controllers for API endpoints
builder.Services.AddControllers();
//Add 
builder.Services.AddScoped<EmailService>();
// Swagger setup for API documentation, including JWT support in the UI
builder.Services.AddEndpointsApiExplorer();
// Configure Swagger to include JWT authentication support in the UI, allowing users to enter a token for testing authenticated endpoints.
builder.Services.AddSwaggerGen(c =>
{
    c.SwaggerDoc("v1", new OpenApiInfo { Title = "Can I HinderAPI", Version = "v1" });
    //adds a security definition for JWT Bearer tokens, which tells Swagger how to include the token in API requests
    c.AddSecurityDefinition("Bearer", new OpenApiSecurityScheme
    {
        Description = "JWT Authorization header using the Bearer scheme. Example: \"Bearer {token}\"",
        Name = "Authorization",
        In = ParameterLocation.Header,
        Type = SecuritySchemeType.Http,
        Scheme = "bearer",
        BearerFormat = "JWT"
    });
    // Adds a global security requirement that applies the "Bearer" scheme to all API endpoints, meaning that by default all endpoints will require a valid JWT token unless explicitly overridden.
    c.AddSecurityRequirement(new OpenApiSecurityRequirement
    {
        {
            new OpenApiSecurityScheme
            {
                Reference = new OpenApiReference
                {
                    Type = ReferenceType.SecurityScheme,
                    Id = "Bearer"
                }
            },
            Array.Empty<string>()
        }
    });
});

var app = builder.Build();

// Enable Swagger UI in development environment for easy testing and documentation of API endpoints. In production, this can be disabled for security reasons.
if (app.Environment.IsDevelopment())
{
    app.UseSwagger();
    app.UseSwaggerUI();
}
//https redirection to make sure traffic is secure
app.UseHttpsRedirection();
//enable routing to map incoming requests from controllers
app.UseRouting();
//Enable cors with a policy that allows requests from allowed origins from appsettings,json
app.UseCors(CorsPolicy);

//its important that UseAuthentication comes before UseAuthorization, as the authentication middleware needs to run first to set the user context before authorization checks are performed.
app.UseAuthentication();
app.UseAuthorization();
// Map controller endpoints to handle incoming API requests based on the defined routes in the controllers.
app.MapControllers();
//this is for applying any pending database migrations at startip so that the azure sql database is always up to date.
using (var scope = app.Services.CreateScope())
{
    var db = scope.ServiceProvider.GetRequiredService<AppDbContext>();
    var logger = scope.ServiceProvider.GetRequiredService<ILogger<Program>>();

    try
    {
        logger.LogInformation("Applying database migrations...");
        db.Database.Migrate();
        logger.LogInformation("Database migrations applied successfully.");
    }
    catch (Exception ex)
    {
        logger.LogError(ex, "Database migration failed.");
        throw;
    }
}
app.Run();