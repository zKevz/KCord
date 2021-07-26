#pragma once
#pragma warning(disable : 4267)
#define _CRT_SECURE_NO_WARNINGS
#define KCORD_SHOW_REST_API_MESSAGE

#include "Json.hpp"
#include "DiscordGuild.hpp"
#include "DiscordOpcode.hpp"
#include "DiscordMessage.hpp"
#include "DiscordRestAPI.hpp"
#include "DiscordCommand.hpp"
#include "DiscordWebSocket.hpp"
#include "DiscordEventStruct.hpp"
#include "DiscordClientConfig.hpp"
#include "DiscordInteractivity.hpp"

namespace Discord
{
	using Callback = std::function<void()>;
	using GuildCallback = std::function<void(const DiscordEventInfo&)>;
	using MessageCallback = std::function<void(const DiscordMessageEventInfo&)>;
	using PresenceCallback = std::function<void(const DiscordPresenceEventInfo&)>;
	using ReactionCallback = std::function<void(const DiscordReactionEventInfo&)>;
	using GuildRoleCallback = std::function<void(const DiscordRoleEventInfo&)>;
	using DiscordUserCallback = std::function<void(const DiscordUserEventInfo&)>;
	using GuildMemberCallback = std::function<void(const DiscordMemberEventInfo&)>;
	using TypingStartCallback = std::function<void(const DiscordTypingStartEventInfo&)>;
	using GuildChannelCallback = std::function<void(const DiscordChannelEventInfo&)>;
	using InviteCreatedCallback = std::function<void(const DiscordInviteCreated&)>;
	using InviteDeletedCallback = std::function<void(const DiscordInviteDeletedEventInfo&)>;
	
	enum class DiscordCloseEvent
	{
		UNKNOWN_ERROR = 4000,

	};

	class DiscordClientEdit
	{
	public:
		std::string Username;
		std::optional<std::string> AvatarImageFile;

		Json Serialize() const;
	};

	class DiscordClient : public DiscordUser
	{
	public:
		DiscordClient(const DiscordClientConfig& config);
		~DiscordClient();

		void Run();
		void Edit(const std::function<void(DiscordClientEdit*)>& func);
		void LeaveGuild(Snowflake id);
		void LeaveGuild(const DiscordGuild& guild);
		void LeaveGuild(const Ptr<DiscordGuild> guild);
		void DeleteGuild(Snowflake id);
		void DeleteGuild(const DiscordGuild& guild);
		void DeleteGuild(const Ptr<DiscordGuild> guild);

		Ptr<DiscordUser> GetUserById(Snowflake id);
		Ptr<DiscordGuild> GetGuildById(Snowflake id);
		Ptr<DiscordChannel> GetChannel(Snowflake id);
		Ptr<DiscordDMChannel> GetOrCreateDMChannel(Snowflake id);
		Ptr<DiscordDMChannel> GetOrCreateDMChannel(const DiscordUser& user);
		Ptr<DiscordDMChannel> GetOrCreateDMChannel(const Ptr<DiscordUser> user);
		
		void InvokeCommand(Ptr<DiscordMessage> message);
		void RegisterCommand(const std::string& str, const DiscordCommandCallback& callback);
		void RegisterCommand(const std::string& str, const DiscordCommandCallback& callback, uint64_t permissionFlags);
		void RegisterCommand(const std::string& str, const DiscordCommandCallback& callback, std::optional<DiscordCommandInfo> info);
		void RegisterCommand(const std::string& str, const DiscordCommandCallback& callback, std::vector<std::string> roles, DiscordRoleTargetType type);

		void SetPresence(const DiscordGatewayPresence& presence) { Presence = presence; }

		void OnReady(const Callback& callback) { ReadyCallback = callback; }
		void OnTypingStart(const TypingStartCallback& callback) { TypingStartCallback = callback; }
		void OnPresenceUpdated(const PresenceCallback& callback) { PresenceUpdatedCallback = callback; }

		void OnMessageCreated(const MessageCallback& callback) { MessageCreatedCallback = callback; }
		void OnMessageUpdated(const MessageCallback& callback) { MessageUpdatedCallback = callback; }

		void OnReactionAdded(const ReactionCallback& callback) { ReactionAddedCallback = callback; }
		void OnReactionRemoved(const ReactionCallback& callback) { ReactionRemovedCallback = callback; }
		void OnAllReactionRemoved(const ReactionCallback& callback) { ReactionRemovedAllCallback = callback; }
		void OnSingleReactionFullyRemoved(const ReactionCallback& callback) { ReactionFullyRemovedCallback = callback; }

		void OnInviteCreated(const InviteCreatedCallback& callback) { InviteCreatedCallback = callback; }
		void OnInviteDeleted(const InviteDeletedCallback& callback) { InviteDeletedCallback = callback; }

		void OnGuildCreated(const GuildCallback& callback) { GuildCreatedCallback = callback; }
		void OnGuildUpdated(const GuildCallback& callback) { GuildUpdatedCallback = callback; }
		void OnGuildRemoved(const GuildCallback& callback) { GuildRemovedCallback = callback; }
		void OnEmojiUpdated(const GuildCallback& callback) { EmojiUpdatedCallback = callback; }
		void OnGuildMembersChunk(const GuildCallback& callback) { GuildMembersChunkCallback = callback; }
		void OnGuildIntegrationsUpdated(const GuildCallback& callback) { GuildIntegrationsUpdatedCallback = callback; }

		void OnUserUpdated(const DiscordUserCallback& callback) { UserUpdatedCallback = callback; }
		void OnGuildBanAdded(const DiscordUserCallback& callback) { BanAddedCallback = callback; }
		void OnGuildBanRemoved(const DiscordUserCallback& callback) { BanRemovedCallback = callback; }

		void OnGuildRoleAdded(const GuildRoleCallback& callback) { RoleAddedCallback = callback; }
		void OnGuildRoleRemoved(const GuildRoleCallback& callback) { RoleRemovedCallback = callback; }
		void OnGuildRoleUpdated(const GuildRoleCallback& callback) { RoleUpdatedCallback = callback; }

		void OnGuildMemberAdded(const GuildMemberCallback& callback) { MemberAddedCallback = callback; }
		void OnGuildMemberRemoved(const DiscordUserCallback& callback) { MemberRemovedCallback = callback; }
		void OnGuildMemberUpdated(const GuildMemberCallback& callback) { MemberUpdatedCallback = callback; }

		void OnChannelCreated(const GuildChannelCallback& callback) { ChannelCreatedCallback = callback; }
		void OnChannelUpdated(const GuildChannelCallback& callback) { ChannelUpdatedCallback = callback; }
		void OnChannelRemoved(const GuildChannelCallback& callback) { ChannelRemovedCallback = callback; }

		std::map<Snowflake, Ptr<DiscordGuild>>& GetGuilds() { return Guilds; }
		const std::map<Snowflake, Ptr<DiscordGuild>>& GetGuilds() const { return Guilds; }

		DiscordCommandService& GetCommandService() { return CommandService; }
		DiscordInteractivityService& GetInteractivityService() { return InteractivityService; }
	private:
		void Send(const std::string& msg);
		void Reconnect();
		void SendIdentify();
		void SendHeartbeat();
		void StartHeartbeating();

		void OnDispatch(const Json& json, const Json& data);
		void OnMessageReceived(std::string msg);
		void OnWebsocketClosed(short code, const std::string& reason);

		void OnDefaultHelpCommand(Discord::DiscordCommandContext* context);

		DiscordCommandService CommandService;
		DiscordInteractivityService InteractivityService;

		Callback ReadyCallback;

		PresenceCallback PresenceUpdatedCallback;

		TypingStartCallback TypingStartCallback;

		MessageCallback MessageCreatedCallback;
		MessageCallback MessageUpdatedCallback;

		ReactionCallback ReactionAddedCallback;
		ReactionCallback ReactionRemovedCallback;
		ReactionCallback ReactionRemovedAllCallback;
		ReactionCallback ReactionFullyRemovedCallback;

		InviteCreatedCallback InviteCreatedCallback;
		InviteDeletedCallback InviteDeletedCallback;

		DiscordUserCallback BanAddedCallback;
		DiscordUserCallback BanRemovedCallback;
		DiscordUserCallback UserUpdatedCallback;
		DiscordUserCallback MemberRemovedCallback;

		GuildCallback GuildCreatedCallback;
		GuildCallback GuildUpdatedCallback;
		GuildCallback GuildRemovedCallback;
		GuildCallback EmojiUpdatedCallback;
		GuildCallback GuildMembersChunkCallback;
		GuildCallback GuildIntegrationsUpdatedCallback;

		GuildRoleCallback RoleAddedCallback;
		GuildRoleCallback RoleRemovedCallback;
		GuildRoleCallback RoleUpdatedCallback;

		GuildMemberCallback MemberAddedCallback;
		GuildMemberCallback MemberUpdatedCallback;

		GuildChannelCallback ChannelCreatedCallback;
		GuildChannelCallback ChannelUpdatedCallback;
		GuildChannelCallback ChannelRemovedCallback;

		DiscordTimepoint LastHeartbeatSent = DiscordClock::now() - std::chrono::hours(1);

		DiscordWebSocket* WebSocket;
		DiscordClientConfig Config;

		int HeartbeatInterval = 0;
		int LastSequenceNumber = 0;

		bool bHeartbeatFailed = false;

		std::string SessionId;
		std::optional<DiscordGatewayPresence> Presence;
		std::map<Snowflake, Ptr<DiscordGuild>> Guilds;
		std::map<Snowflake, Ptr<DiscordDMChannel>> DMChannels;
	};
}