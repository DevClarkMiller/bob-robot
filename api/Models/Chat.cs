namespace BobRobotApi.Models;

public class Chat
{
	public Queue<string> UnitMessages { get; set; } = new();
	public Queue<string> ChatterMessages { get; set; } = new();
}