using CanI_HinderAPI.Models;
using System.ComponentModel.DataAnnotations;

namespace CanI_HinderAPI.Models;
// Represents an analytics event that can be logged for a user or anonymously.
public class AnalyticsEvent
{
    public int Id { get; set; }
    public int? UserId { get; set; }

    [Required, MaxLength(64)]
    public string EventType { get; set; } = "";

    [MaxLength(4096)]
    public string? PayloadJson { get; set; }

    public DateTime CreatedUtc { get; set; } = DateTime.UtcNow;

    public User? User { get; set; }
}