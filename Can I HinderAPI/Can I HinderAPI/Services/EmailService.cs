namespace CanI_HinderAPI.Services;

using SendGrid;
using SendGrid.Helpers.Mail;

public class EmailService
{
    private readonly IConfiguration _config;

    public EmailService(IConfiguration config)
    {
        _config = config;
    }

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

        msg.AddTo(new EmailAddress(email));

        await client.SendEmailAsync(msg);
    }
}

