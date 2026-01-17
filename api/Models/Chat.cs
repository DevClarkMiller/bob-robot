namespace BobRobotApi.Models;

public class Chat
{
	public Queue<string> UnitMessages { get; set; } = new();
	public Queue<string> ChatterMessages { get; set; } = new();
	public List<ChatHistoryItem> ChatHistory { get; set; } = new();
	public string SystemPrompt { get; set; } = string.Empty;
	public bool IsUsingAIChats { get; set; } = false;
}

public class ChatHistoryItem
{
	public required bool IsUser { get; set; }
	public required string Message { get; set; }
}