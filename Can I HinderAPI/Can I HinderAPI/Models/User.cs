using System.ComponentModel.DataAnnotations;

namespace CanI_HinderAPI.Models;
// Represents a user of the system, with credentials for authentication.
public class User
{
    public int Id { get; set; }

    [Required, MaxLength(64)]
    public string Username { get; set; } = "";

    [Required, MaxLength(128)]
    public string Email { get; set; } = "";

    [Required]
    public string PasswordHash { get; set; } = "";

    public DateTime CreatedUtc { get; set; } = DateTime.UtcNow;
    
    public string? PasswordResetToken { get; set; }
    public DateTime? PasswordResetExpires { get; set; }

}