using CanI_HinderAPI.Data;
using Microsoft.AspNetCore.Authentication.JwtBearer;
using Microsoft.EntityFrameworkCore;
using Microsoft.IdentityModel.Tokens;
using Microsoft.OpenApi.Models;
using System.Text;

var builder = WebApplication.CreateBuilder(args);

//Database setup using SQL Server , with connection string from appsettings.json
builder.Services.AddDbContext<AppDbContext>(options =>
    options.UseSqlServer(builder.Configuration.GetConnectionString("Default")));

// CORS setup, allowing origins specified in appsettings.json under "AllowedOrigins" (as an array of strings)
const string CorsPolicy = "PwaCors";
var allowedOrigins = builder.Configuration.GetSection("AllowedOrigins").Get<string[]>() ?? Array.Empty<string>();

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
if (!string.IsNullOrWhiteSpace(jwtKey))
{
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
// Swagger setup for API documentation, including JWT support in the UI
builder.Services.AddEndpointsApiExplorer();
// Configure Swagger to include JWT authentication support in the UI, allowing users to enter a token for testing authenticated endpoints.
builder.Services.AddSwaggerGen(c =>
{
    //adds a security definition for JWT Bearer tokens, which tells Swagger how to include the token in API requests
    c.AddSecurityDefinition("Bearer", new OpenApiSecurityScheme
    {
        Name = "Authorization",
        Type = SecuritySchemeType.Http,
        Scheme = "bearer",
        BearerFormat = "JWT",
        In = ParameterLocation.Header,
        Description = "Enter: Bearer {your JWT token}"
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

app.UseHttpsRedirection();

app.UseCors(CorsPolicy);

//its important that UseAuthentication comes before UseAuthorization, as the authentication middleware needs to run first to set the user context before authorization checks are performed.
app.UseAuthentication();
app.UseAuthorization();

app.MapControllers();

app.Run();