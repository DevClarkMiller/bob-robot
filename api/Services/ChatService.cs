using System.Collections.Concurrent;
using BobRobotApi.Models;

namespace BobRobotApi.Services;

public class ChatService : IChatService
{
	private ConcurrentDictionary<Guid, Chat> _chats = new();

	private Chat GetOrInitChat (Guid unitGuid) =>
		_chats.GetOrAdd(unitGuid, _ => new Chat());

	private Queue<string> GetChatQueue(Guid unitGuid, bool toUnit) {
		var chat = GetOrInitChat(unitGuid);
		return toUnit ? chat.UnitMessages : chat.ChatterMessages;
	}

	public Chat GetAllChats(Guid unitGuid) => GetOrInitChat(unitGuid);

	private string? GetChat(Guid unitGuid, bool toUnit) {
		var chatQueue = GetChatQueue(unitGuid, toUnit);
		return chatQueue.Count > 0 ? chatQueue.Dequeue() : null;
	}

	private void SendChat(Guid unitGuid, string message, bool toUnit) {
		var chatQueue = GetChatQueue(unitGuid, toUnit);
		chatQueue.Enqueue(message);
	}

	public string? GetUnitChats(Guid unitGuid) => GetChat(unitGuid, true);
	public string? GetChatterChats(Guid unitGuid) => GetChat(unitGuid, false);

	public void SendChatToUnit(Guid unitGuid, string message) => SendChat(unitGuid, message, true);
	public void SendChatToChatter(Guid unitGuid, string message) => SendChat(unitGuid, message, false);
}

public interface IChatService
{
	public Chat GetAllChats(Guid unitGuid);
	public string? GetUnitChats(Guid unitGuid);
	public string? GetChatterChats(Guid unitGuid);
	public void SendChatToUnit(Guid unitGuid, string message);
	public void SendChatToChatter(Guid unitGuid, string message);
}