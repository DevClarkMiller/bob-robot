namespace BobRobotApi.Config;

public class GeminiApiConfig
{
	public required string Name { get; set; }
	public required string ModelName { get; set; }
	public required string Key { get; set; }	
	public required string SystemPrompt { get; set; }
}