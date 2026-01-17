using System.Collections.Concurrent;
using AutoGen.Core;
using AutoGen.Gemini;
using BobRobotApi.Config;
using BobRobotApi.Models;
using Microsoft.Extensions.Options;

namespace BobRobotApi.Services;

public class ChatService : IChatService
{
	private ConcurrentDictionary<Guid, Chat> _chats = new();
	private MiddlewareStreamingAgent<GeminiChatAgent>? _geminiAgent;

	public ChatService(IOptions<GeminiApiConfig> geminiApiConfig) {
		var apiConf = geminiApiConfig.Value;

		_geminiAgent = new GeminiChatAgent(
			name: apiConf.Name,
			model: apiConf.ModelName,
			apiKey: apiConf.Key,
			systemMessage: apiConf.SystemPrompt)
		.RegisterMessageConnector()
		.RegisterPrintMessage();
	}

	private Chat GetOrInitChat (Guid unitGuid) =>
		_chats.GetOrAdd(unitGuid, _ =>  { 
			var chat = new Chat();
			return chat;
		});

	private Queue<string> GetChatQueue(Guid unitGuid, bool toUnit) {
		var chat = GetOrInitChat(unitGuid);
		return toUnit ? chat.UnitMessages : chat.ChatterMessages;
	}

	private async Task<string?> AIResponse(Chat chat) {
		if (_geminiAgent == null) return null;

		var messages = chat.ChatHistory.Select(msg => 
			msg.IsUser 
				? new TextMessage(Role.User, msg.Message)
				: new TextMessage(Role.Assistant, msg.Message)
		).ToList<IMessage>();

		var lastMessage = messages.LastOrDefault();
		if (lastMessage == null) return null;

		var response = await _geminiAgent.SendAsync(lastMessage, messages.SkipLast(1).ToList());
		var resp = response.GetContent();

		if (resp is null) return null;

		chat.ChatHistory.Add(new ChatHistoryItem { IsUser = false, Message = resp });
		return resp;
	}

	private string? GetChat(Guid unitGuid, bool toUnit) {
		var chatQueue = GetChatQueue(unitGuid, toUnit);
		return chatQueue.Count > 0 ? chatQueue.Dequeue() : null;
	}

	private Chat SendChat(Guid unitGuid, string message, bool toUnit) {
		var chat = GetOrInitChat(unitGuid);

		var isUser = !toUnit;
		chat.ChatHistory.Add(new ChatHistoryItem { IsUser = isUser, Message = message });

		var chatQueue = toUnit ? chat.UnitMessages : chat.ChatterMessages;
		chatQueue.Enqueue(message);
		
		return chat;
	}

	public Chat GetAllChats(Guid unitGuid) => GetOrInitChat(unitGuid);

	public string? GetUnitChats(Guid unitGuid) => GetChat(unitGuid, true);
	public string? GetChatterChats(Guid unitGuid) => GetChat(unitGuid, false);

	public void SendChatToUnit(Guid unitGuid, string message) => SendChat(unitGuid, message, true);
	public async Task SendChatToChatter(Guid unitGuid, string message) { 
		var chat = SendChat(unitGuid, message, false); 

		if (chat.IsUsingAIChats) {
			var newMessage = await AIResponse(chat);
			if (newMessage is not null) { 
				SendChatToUnit(unitGuid, newMessage);
				chat.ChatterMessages.Dequeue();
			}
		}
	}

	public void SetIsUsingAIChats(Guid unitGuid, bool value) {
		var chat = GetOrInitChat(unitGuid);
		chat.IsUsingAIChats = value;
	}

	public bool GetIsUsingAIChats(Guid unitGuid) => GetOrInitChat(unitGuid).IsUsingAIChats;
}

public interface IChatService
{
	public Chat GetAllChats(Guid unitGuid);
	public string? GetUnitChats(Guid unitGuid);
	public string? GetChatterChats(Guid unitGuid);
	public void SendChatToUnit(Guid unitGuid, string message);
	public Task SendChatToChatter(Guid unitGuid, string message);
	public void SetIsUsingAIChats(Guid unitGuid, bool value);
	public bool GetIsUsingAIChats(Guid unitGuid);
}