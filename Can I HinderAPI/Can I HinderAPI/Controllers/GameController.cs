using Microsoft.AspNetCore.Mvc;
using System.Security.Cryptography;
//this is to create session ids so if multiple instances of the game is ran then the pwas can connect to the correct instance of that game.
namespace Can_I_HinderAPI.Controllers
{
    [ApiController]
    [Route("api/game")]
    public class GameController : ControllerBase
    {
        //endpoint to create a new game session which generates a 6 digit code and returns to the game.
        [HttpPost("create-session")]
        public ActionResult<object> CreateSession()
        {
            var sessionId = GenerateCode(6);
            return Ok(new { sessionId });
        }
        //generates a session code for the game using a specified alphanumeric code which doesnt use confusing charactoers
        //like 0, O ,1 and l to make it easier for players to see
        private static string GenerateCode(int length)
        {
            const string chars = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";

            var bytes = RandomNumberGenerator.GetBytes(length);
            var result = new char[length];

            for (int i = 0; i < length; i++)
            {
                result[i] = chars[bytes[i] % chars.Length];
            }

            return new string(result);
        }
    }
}