using BobRobotApi.Services;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;

namespace BobRobotApi.Controllers;

[Authorize]
[ApiController]
[Route("[controller]")]
public class ChatController : ControllerBase
{
    private readonly ILogger<ChatController> _logger;
	private readonly IChatService _chatService;
	private readonly ITokenService _tokenService;

    public ChatController(ILogger<ChatController> logger, IChatService chatService, ITokenService tokenService)
    {
        _logger = logger;
		_chatService = chatService;
		_tokenService = tokenService;
    }

	[HttpGet]
	public async Task<IActionResult> GetAllChats(Guid unitGuid) {
		var chats = _chatService.GetAllChats(unitGuid);
		return Ok(chats);
	}

	[HttpGet]
	public async Task<IActionResult> GetChat(Guid unitGuid) {
		var claims = await _tokenService.GetClaims(HttpContext);
		var role = _tokenService.GetRole(claims);

		if (role == Constants.Roles.Admin) return BadRequest("Admins have no incoming chats");
		
		string? chat = (role == Constants.Roles.Unit) 
			? _chatService.GetUnitChats(unitGuid) 
			: _chatService.GetChatterChats(unitGuid);

		if (chat is null) return NotFound("No chats queued up");
		return Ok(chat);
	}

	[HttpPost]
	public async Task<IActionResult> SendChat([FromQuery] Guid unitGuid, [FromQuery] string message) {
		var claims = await _tokenService.GetClaims(HttpContext);
		var role = _tokenService.GetRole(claims);

		switch (role) {
			case Constants.Roles.Admin: return BadRequest("Admins have no incoming chats");
			case Constants.Roles.Unit: 
				_chatService.SendChatToChatter(unitGuid, message);
				return Ok("Chat sent to Chatter");
			case Constants.Roles.Chatter:
				_chatService.SendChatToUnit(unitGuid, message);
				return Ok("Chat sent to Unit");
			default:
				return BadRequest("Unknown Role");
		}
	}
}
