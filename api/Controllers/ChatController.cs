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
}
