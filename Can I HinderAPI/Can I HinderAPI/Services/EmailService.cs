namespace CanI_HinderAPI.Services;

using SendGrid;
using SendGrid.Helpers.Mail;
// Service for sending emails, specifically for password reset functionality, using the SendGrid API.
public class EmailService
{
    //private field to hold the config, which is used to access the SendGrid API key.
    private readonly IConfiguration _config;
    //constructor that uses the config 
    public EmailService(IConfiguration config)
    {
        _config = config;
    }
    // Method to send a password reset email. It makes the email content and then uses the SendGrid client to send the email.
    public async Task SendPasswordReset(string email, string resetLink)
    {
        var client = new SendGridClient(_config["SendGrid:ApiKey"]);

        var msg = new SendGridMessage()
        {
            From = new EmailAddress("c00288668@setu.ie", "Can I Hinder"),
            Subject = "Reset your password for Can I Hinder",
            PlainTextContent = $"Reset your password: {resetLink}",
            HtmlContent = $"Click <a href='{resetLink}'>here</a> to reset your password."
        };
        // Add the recipient's email address to the message.
        msg.AddTo(new EmailAddress(email));
        // Send the email asynchronously using the SendGrid client.
        await client.SendEmailAsync(msg);
    }
}

