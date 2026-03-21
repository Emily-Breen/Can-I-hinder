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
    // Private field for the database context, which is used to query user information from the database.
    private readonly AppDbContext _db;
    // constructor that takes the db context and initalises the private field with the dependency (dependancy = AppDbContext) and is added to the services in program.cs
    public UserController(AppDbContext db) => _db = db;
    // Endpoint to get the current authenticated user's information. Retrieves the user ID from the JWT claims, queries the database for the user's details, and returns them.
    [HttpGet("me")]
    public async Task<IActionResult> Me()
    {
        // get the users id from the jwt claims and query the db for the users info and returns it.
        // this is for pwa to check if user is logged in can also be used for a profile page if I have time to make one.
        var id = int.Parse(User.FindFirstValue(ClaimTypes.NameIdentifier)!);
        var user = await _db.Users
            .Where(u => u.Id == id)
            .Select(u => new { u.Id, u.Username, u.CreatedUtc })
            .SingleAsync();

        return Ok(user);
    }
}