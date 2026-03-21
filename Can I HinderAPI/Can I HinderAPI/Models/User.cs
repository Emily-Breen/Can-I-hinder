using System.ComponentModel.DataAnnotations;

namespace CanI_HinderAPI.Models;
// Represents a user of the system, with credentials for authentication.
public class User
{ //id for primary key and is incremented when a user is created
    public int Id { get; set; }

    [Required, MaxLength(64)]
    public string Username { get; set; } = "";

    [Required, MaxLength(128)]
    public string Email { get; set; } = "";

    [Required]
    public string PasswordHash { get; set; } = "";
    //timestamp using UTC (world standard time) when a user is created, and when user is registered
    public DateTime CreatedUtc { get; set; } = DateTime.UtcNow;
    // Fields for password reset functionality, including a token and its expiration time.
    public string? PasswordResetToken { get; set; }
    //the expiration time for the reset token, if the user tries to reset after the time they have to request a new one.
    public DateTime? PasswordResetExpires { get; set; }

}