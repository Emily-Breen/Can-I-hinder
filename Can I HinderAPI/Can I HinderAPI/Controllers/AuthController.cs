using CanI_HinderAPI.Data;
using CanI_HinderAPI.Models;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Microsoft.IdentityModel.Tokens;
using System.IdentityModel.Tokens.Jwt;
using System.Security.Claims;
using System.Text;

namespace CanI_HinderAPI.Controllers;

[ApiController]
[Route("api/auth")]
// Controller for handling user authentication, including registration and login, using JWT tokens.
public class AuthController : ControllerBase
{
    private readonly AppDbContext _db;
    private readonly IConfiguration _config;
    private readonly PasswordHasher<User> _hasher = new();
    // Constructor that takes the database context and configuration as dependencies, which are injected by the framework.
    public AuthController(AppDbContext db, IConfiguration config)
    {
        _db = db;
        _config = config;
    }
    // Records for the request and response payloads for registration and login endpoints.
    public record RegisterRequest(string Username, string Password);
    public record LoginRequest(string Username, string Password);
    public record AuthResponse(string Token);
    // Endpoint for user registration. Validates the input, checks for existing username, hashes the password, saves the user, and returns a JWT token.
    [HttpPost("register")]
    public async Task<ActionResult<AuthResponse>> Register(RegisterRequest req)
    {
        var username = req.Username.Trim();

        if (username.Length < 3) return BadRequest("Username must be at least 3 characters.");
        if (req.Password.Length < 6) return BadRequest("Password must be at least 6 characters.");

        var exists = await _db.Users.AnyAsync(u => u.Username == username);
        if (exists) return Conflict("Username already exists.");

        var user = new User { Username = username };
        user.PasswordHash = _hasher.HashPassword(user, req.Password);

        _db.Users.Add(user);
        await _db.SaveChangesAsync();

        return Ok(new AuthResponse(CreateJwt(user)));
    }
    // Endpoint for user login. Validates the input, checks the credentials, and returns a JWT token if successful.
    [HttpPost("login")]
    public async Task<ActionResult<AuthResponse>> Login(LoginRequest req)
    {
        var username = req.Username.Trim();
        var user = await _db.Users.SingleOrDefaultAsync(u => u.Username == username);

        if (user is null) return Unauthorized("Invalid credentials.");

        var ok = _hasher.VerifyHashedPassword(user, user.PasswordHash, req.Password)
                 == PasswordVerificationResult.Success;

        if (!ok) return Unauthorized("Invalid credentials.");

        return Ok(new AuthResponse(CreateJwt(user)));
    }
    // Helper method to create a JWT token for a given user, using the configuration settings for issuer, audience, and signing key.
    private string CreateJwt(User user)
    {
        var issuer = _config["Jwt:Issuer"]!;
        var audience = _config["Jwt:Audience"]!;
        var key = _config["Jwt:Key"]!;

        var claims = new[]
        {
            new Claim(ClaimTypes.NameIdentifier, user.Id.ToString()),
            new Claim(ClaimTypes.Name, user.Username)
        };

        var signingKey = new SymmetricSecurityKey(Encoding.UTF8.GetBytes(key));
        var creds = new SigningCredentials(signingKey, SecurityAlgorithms.HmacSha256);

        var token = new JwtSecurityToken(
            issuer: issuer,
            audience: audience,
            claims: claims,
            expires: DateTime.UtcNow.AddHours(8),
            signingCredentials: creds
        );

        return new JwtSecurityTokenHandler().WriteToken(token);
    }
}