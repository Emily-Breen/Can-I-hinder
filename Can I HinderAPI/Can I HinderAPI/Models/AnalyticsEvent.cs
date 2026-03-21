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
    //payload json string that can store additional data about an event to be flexible for storing urls or error messages
    [MaxLength(4096)]
    public string? PayloadJson { get; set; }
    // Timestamp for when the event was created, defaulting to the current UTC time when a new event is instantiated.
    public DateTime CreatedUtc { get; set; } = DateTime.UtcNow;
    //navigates to the user that made the event if the user is logged in can get more info about the user for analytics like how many users are using the app and what they are using most
    //also can be null 
    public User? User { get; set; }
}