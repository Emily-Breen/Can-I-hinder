using CanI_HinderAPI.Data;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using System.Security.Claims;

namespace CanI_HinderAPI.Controllers;

[ApiController]
[Authorize]
[Route("api/user")]
// Controller for user-related endpoints, such as retrieving the current user's information.
public class UserController : ControllerBase
{
    private readonly AppDbContext _db;

    public UserController(AppDbContext db) => _db = db;
    // Endpoint to get the current authenticated user's information. Retrieves the user ID from the JWT claims, queries the database for the user's details, and returns them.
    [HttpGet("me")]
    public async Task<IActionResult> Me()
    {
        var id = int.Parse(User.FindFirstValue(ClaimTypes.NameIdentifier)!);
        var user = await _db.Users
            .Where(u => u.Id == id)
            .Select(u => new { u.Id, u.Username, u.CreatedUtc })
            .SingleAsync();

        return Ok(user);
    }
}