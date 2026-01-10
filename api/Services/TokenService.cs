using System.Security.Claims;
using System.IdentityModel.Tokens.Jwt;
using Microsoft.IdentityModel.Tokens;
using System.Text;
using Microsoft.AspNetCore.Authentication;
using BobRobotApi.Models;

namespace BobRobotApi.Services;

public class TokenService : ITokenService
{
	private readonly byte[] jwtKey;

	public TokenService(IConfiguration configuration){
		string jwtSecret = configuration.GetSection("AppSettings").GetValue<string>("Secret") 
			?? throw new Exception("Missing JWT Secret in appsettings");
	
		jwtKey = Encoding.ASCII.GetBytes(jwtSecret!);
	}

	public string CreateToken(Guid? unitGuid, string role, int daysToExpire = 7){
		var tokenHandler = new JwtSecurityTokenHandler();

		bool isUnit = unitGuid is not null;

		var tokenDescriptor = new SecurityTokenDescriptor {
			Subject = new ClaimsIdentity(new Claim[]
			{
				new Claim(ClaimTypes.NameIdentifier, isUnit ? unitGuid.ToString()! : role),
				new Claim(ClaimTypes.Role, role)
			}),
			Expires = DateTime.UtcNow.AddDays(daysToExpire),
			SigningCredentials = new SigningCredentials(new SymmetricSecurityKey(jwtKey), 
				SecurityAlgorithms.HmacSha256Signature
			),
		};

		var token = tokenHandler.CreateToken(tokenDescriptor);
		return tokenHandler.WriteToken(token);
	}

	public async Task<IEnumerable<Claim>> GetClaims(HttpContext httpContext) {
		AuthenticateResult authResult = await httpContext.AuthenticateAsync("Bearer");
		return authResult.Principal!.Identities.FirstOrDefault()?.Claims!;
	}

	public Guid GetUnitGuid(IEnumerable<Claim> claims) {
		string unitGuid = claims?.FirstOrDefault(c => c.Type == ClaimTypes.NameIdentifier)?.Value!;
		return Guid.Parse(unitGuid);
	}

	public string GetRole(IEnumerable<Claim> claims) 
		=> claims?.FirstOrDefault(c => c.Type == ClaimTypes.Role)?.Value!;
}

public interface ITokenService
{
	string CreateToken(Guid? unitGuid,  string role, int daysToExpire = 7);
	Task<IEnumerable<Claim>> GetClaims(HttpContext httpContext);
	Guid GetUnitGuid(IEnumerable<Claim> claims);
	string GetRole(IEnumerable<Claim> claims);
}