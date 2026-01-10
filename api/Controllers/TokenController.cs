using BobRobotApi.Models;
using BobRobotApi.Services;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;

namespace BobRobotApi.Controllers;

[Authorize]
[ApiController]
[Route("[controller]")]
public class TokenController : ControllerBase
{
    private readonly ILogger<ChatController> _logger;
	private readonly ITokenService _tokenService;
	private readonly string _adminTokenKey;

    public TokenController(ILogger<ChatController> logger, ITokenService tokenService, IConfiguration config)
    {
        _logger = logger;
		_tokenService = tokenService;
		_adminTokenKey = config.GetSection("AppSettings").GetValue<string>("AdminTokenKey")!;
    }

	private async Task<IActionResult> CreateTokenGeneric(Guid? unitGuid, int tokenLifetime, string wantedRole) {
		var claims = await _tokenService.GetClaims(HttpContext);
		bool isAdmin = _tokenService.GetRole(claims) == Constants.Roles.Admin;

		if (!isAdmin)
			return BadRequest($"Incorrect role for creating {wantedRole} tokens");

		string token = _tokenService.CreateToken(unitGuid, wantedRole, tokenLifetime);
		return Ok(token);
	}

	[HttpPost("createToken")]
	public async Task<IActionResult> CreateToken([FromQuery]Guid unitGuid) {
		return await CreateTokenGeneric(unitGuid, Constants.Token.TokenLifetime, Constants.Roles.Unit);
	}

	[HttpPost("createChatterToken")]
	public async Task<IActionResult> CreateChatterToken() 
		=> await CreateTokenGeneric(null, Constants.Token.TokenLifetime, Constants.Roles.Chatter);

	[HttpPost("createAdminToken")]
	[AllowAnonymous]
	public IActionResult CreateAdminToken([FromQuery]string adminTokenKey) {
		if (adminTokenKey == _adminTokenKey)
			return Ok(_tokenService.CreateToken(null, Constants.Roles.Admin));
		return BadRequest("Incorrect Admin Token Key");
	}
}
