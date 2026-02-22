using System.ComponentModel.DataAnnotations;

namespace CanI_HinderAPI.Models;
// Represents a user preference, which is a key-value pair associated with a user.
public class UserPreference
{
    public int Id { get; set; }
    public int UserId { get; set; }

    [Required, MaxLength(32)]
    public string Key { get; set; } = "";

    [Required, MaxLength(2048)]
    public string Value { get; set; } = "";

    public DateTime UpdatedUtc { get; set; } = DateTime.UtcNow;

    public User? User { get; set; }
}