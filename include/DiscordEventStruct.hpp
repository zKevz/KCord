#pragma once
#include "DiscordUser.hpp"
#include "DiscordGuild.hpp"
#include "DiscordInvite.hpp"
#include "DiscordMessage.hpp"

namespace Discord
{
	class DiscordClient;

	class DiscordEventInfo
	{
	public:
		Ptr<DiscordGuild> GetGuild() const { return Guild; }
		DiscordClient* GetClient() const { return Client; }
	private:
		Ptr<DiscordGuild> Guild = nullptr;
		DiscordClient* Client = nullptr;
		friend class DiscordClient;
		friend class DiscordCommandContext;
		friend class DiscordCommandService;
	};

	class DiscordAbstractUserEvent
	{
	public:
		Ptr<DiscordUser> GetUser() const { return User; }

	private:
		Ptr<DiscordUser> User = nullptr;
		friend class DiscordClient;
	};

	class DiscordAbstractRoleEvent
	{
	public:
		Ptr<DiscordRole> GetRole() const { return Role; }
	private:
		Ptr<DiscordRole> Role = nullptr;
		friend class DiscordClient;
	};

	class DiscordAbstractGuildMemberEvent
	{
	public:
		Ptr<DiscordGuildMember> GetMember() const { return Member; }
	private:
		Ptr<DiscordGuildMember> Member = nullptr;
		friend class DiscordClient;
		friend class DiscordCommandService;
	};

	class DiscordAbstractMessageEvent
	{
	public:
		Ptr<DiscordMessage> GetMsg() const { return Message; }
	private:
		Ptr<DiscordMessage> Message = nullptr;
		friend class DiscordClient;
		friend class DiscordCommandService;
	};

	class DiscordAbstractChannelEvent
	{
	public:
		Ptr<DiscordChannel> GetChannel() const { return Channel; }
	private:
		Ptr<DiscordChannel> Channel = nullptr;
		friend class DiscordClient;
		friend class DiscordInviteCreated;
		friend class DiscordCommandService;
	};

	class DiscordAbstractEmojiEvent
	{
	public:
		Ptr<DiscordEmoji> GetEmoji() const { return Emoji; }
	private:
		Ptr<DiscordEmoji> Emoji;
		friend class DiscordClient;
	};

	class DiscordInviteCreated : public DiscordEventInfo, public DiscordAbstractChannelEvent
	{
	public:
		int Uses = 0;
		int MaxAge = 0;
		int MaxUses = 0;

		bool IsTemporary = false;

		std::string Code;

		DiscordUser Inviter;

		DiscordTimestamp CreatedAt;
		DiscordInviteTargetType TargetType = DiscordInviteTargetType::STREAM;

		DiscordInviteCreated() {}
	};

	class DiscordAbstractPresenceEvent
	{
	public:
		Ptr<DiscordPresence> GetPresence() const { return Presence; }
	private:
		Ptr<DiscordPresence> Presence = nullptr;
		friend class DiscordClient;
	};

	class DiscordTypingStartEventInfo : public DiscordEventInfo, public DiscordAbstractChannelEvent, 
										public DiscordAbstractUserEvent, public DiscordAbstractGuildMemberEvent
	{
	public:
		DiscordTimestamp Timestamp;
	};

	class DiscordInviteDeletedEventInfo : public DiscordEventInfo, public DiscordAbstractChannelEvent
	{
	public:
		std::string Code;
	};

	class DiscordUserEventInfo : public DiscordEventInfo, public DiscordAbstractUserEvent
	{
	};

	class DiscordChannelEventInfo : public DiscordEventInfo, public DiscordAbstractChannelEvent
	{
	};

	class DiscordMemberEventInfo : public DiscordEventInfo, public DiscordAbstractGuildMemberEvent
	{
	};
	
	class DiscordRoleEventInfo : public DiscordEventInfo, public DiscordAbstractRoleEvent
	{
	};

	class DiscordMessageEventInfo : public DiscordEventInfo, public DiscordAbstractMessageEvent, public DiscordAbstractGuildMemberEvent
	{
	private:
		friend class DiscordCommandService;
	};

	class DiscordReactionEventInfo : public DiscordEventInfo, public DiscordAbstractMessageEvent, 
									 public DiscordAbstractEmojiEvent, public DiscordAbstractUserEvent
	{
	};

	class DiscordPresenceEventInfo : public DiscordEventInfo, public DiscordAbstractPresenceEvent
	{
	};
}