#pragma once
#include <fstream>
#include "DiscordEmoji.hpp"
#include "DiscordEmbed.hpp"
#include "DiscordRestAPI.hpp"
#include "DiscordChannelType.hpp"

namespace Discord
{
	enum class DiscordMessageType
	{
		DEFAULT = 0,
		RECIPIENT_ADD = 1,
		RECIPIENT_REMOVE = 2,
		CALL = 3,
		CHANNEL_NAME_CHANGE = 4,
		CHANNEL_ICON_CHANGE = 5,
		CHANNEL_PINNED_MESSAGE = 6,
		GUILD_MEMBER_JOIN = 7,
		USER_PREMIUM_GUILD_SUBSCRIPTION = 8,
		USER_PREMIUM_GUILD_SUBSCRIPTION_TIER_1 = 9,
		USER_PREMIUM_GUILD_SUBSCRIPTION_TIER_2 = 10,
		USER_PREMIUM_GUILD_SUBSCRIPTION_TIER_3 = 11,
		CHANNEL_FOLLOW_ADD = 12,
		GUILD_DISCOVERY_DISQUALIFIED = 14,
		GUILD_DISCOVERY_REQUALIFIED = 15,
		GUILD_DISCOVERY_GRACE_PERIOD_INITIAL_WARNING = 16,
		GUILD_DISCOVERY_GRACE_PERIOD_FINAL_WARNING = 17,
		THREAD_CREATED = 18,
		REPLY = 19,
		APPLICATION_COMMAND  = 20,
		THREAD_STARTER_MESSAGE = 21,
		GUILD_INVITE_REMINDER = 22
	};

	enum class DiscordMessageActivityType
	{
		JOIN = 1,
		SPECTATE = 2,
		LISTEN = 3,
		JOIN_REQUEST = 5
	};

	enum class MessageDirection
	{
		AROUND,
		BEFORE,
		AFTER
	};

	enum DiscordMessageFlags
	{
		CROSSPOSTED				= 1 << 0,
		IS_CROSSPOST			= 1 << 1,
		SUPPRESS_EMBEDS			= 1 << 2,
		SOURCE_MESSAGE_DELETED	= 1 << 3,
		URGENT					= 1 << 4,
		HAS_THREAD				= 1 << 5,
		EPHEMERAL				= 1 << 6,
		LOADING					= 1 << 7
	};

	class DiscordGuild;
	class DiscordClient;
	class DiscordChannel;

	class DiscordAttachment
	{
	public:
		Snowflake GetId() const { return Id; }
		
		std::string GetUrl() const { return Url; }
		std::string GetProxyUrl() const { return ProxyUrl; }
		std::string GetFileName() const { return FileName; }
		std::string GetContentType() const { return ContentType; }

		int GetSize() const { return Size; }
		int GetWidth() const { return Width; }
		int GetHeight() const { return Height; }
	private:
		Snowflake Id;

		std::string Url;
		std::string ProxyUrl;
		std::string FileName;
		std::string ContentType;

		int Size = 0;
		int Width = 0;
		int Height = 0;

		DiscordAttachment() {}
		DiscordAttachment(const Json& json);

		friend class DiscordMessage;
	};
	
	class DiscordMessageReaction
	{
	public:
		bool Reacted() const { return Me; }
		int  GetReactionCount() const { return Count; }
		const DiscordEmoji& GetEmoji() const { return Emoji; }
	private:
		int Count = 0;
		bool Me = false;
		DiscordEmoji Emoji;

		DiscordMessageReaction() {}
		DiscordMessageReaction(const Json& json);

		friend class DiscordMessage;
	};

	class DiscordMessageActivity
	{
	public:
		std::string GetPartyId() const { return PartyId; }
		DiscordMessageActivityType GetType() const { return Type; }
	private:
		std::string PartyId;
		DiscordMessageActivityType Type = DiscordMessageActivityType::JOIN;

		DiscordMessageActivity() {}
		DiscordMessageActivity(const Json& json);

		friend class DiscordMessage;
	};

	class DiscordMessageReference
	{
	public:
		Snowflake GetGuildId() const { return GuildId; }
		Snowflake GetMessageId() const { return MessageId; }
		Snowflake GetChannelId() const { return ChannelId; }
		bool DoesFailIfNotExist() const { return bFailIfNotExist; }
	private:
		Snowflake GuildId;
		Snowflake MessageId;
		Snowflake ChannelId;
		bool bFailIfNotExist = false;

		DiscordMessageReference() {}
		DiscordMessageReference(const Json& json);

		friend class DiscordMessage;
	};


	class DiscordChannelMention
	{
	public:
		Snowflake GetId() const { return Id; }
		Snowflake GetGuildId() const { return GuildId; }

		std::string GetName() const { return Name; }

		DiscordChannelType GetType() const { return Type; }

	private:
		Snowflake Id;
		Snowflake GuildId;

		std::string Name;

		DiscordChannelType Type = DiscordChannelType::GUILD_TEXT;

		DiscordChannelMention() {}
		DiscordChannelMention(const Json& json);

		friend class DiscordMessage;
	};

	class DiscordMessageEdit
	{
	public:
		int Flags = 0;

		std::string Content;
		std::vector<DiscordEmbed> Embeds;

		Json Serialize() const;
	};

	class DiscordMessage
	{
	public:
		Snowflake GetId() const { return Id; }
		Snowflake GetWebhookId() const { return WebhookId; }
		Snowflake GetApplicationId() const { return ApplicationId; }

		Ptr<DiscordUser> GetAuthor() const { return Author; }
		Ptr<DiscordGuild> GetGuild() const { return Guild; }
		Ptr<DiscordChannel> GetChannel() const { return Channel; }

		DiscordTimestamp GetTimestamp() const { return Timestamp; }
		DiscordTimestamp GetEditedTimestamp() const { return EditedTimestamp; }

		DiscordMessageType GetType() const { return Type; }
		DiscordMessageActivity GetActivity() const { return DiscordActivity; }
		DiscordMessageReference GetMessageReference() const { return MessageReference; }

		int GetFlags() const { return Flags; }

		bool IsTTS() const { return bIsTTS; }
		bool IsPinned() const { return bIsPinned; }
		bool MentionedEveryone() const { return bMentionEveryone; }

		std::string GetContent() const { return Content; }

		const std::vector<DiscordEmbed>& GetEmbeds() const { return Embeds; }
		const std::vector<DiscordUser>& GetMentionedUser() const { return Mentions; }
		const std::vector<DiscordRole>& GetMentionedRoles() const { return MentionRoles; }
		const std::vector<DiscordMessageReaction>& GetReactions() const { return Reactions; }
		const std::vector<DiscordAttachment>& GetAttachment() const { return Attachments; }
		const std::vector<DiscordChannelMention>& GetMentionedChannel() const { return MentionChannels; }

		bool HasPrefix(const char& prefix, bool caseSensitive = true);
		bool HasPrefix(const std::string& prefix, bool caseSensitive = true);
	private:
		Snowflake Id;
		Snowflake WebhookId;
		Snowflake ApplicationId;

		Ptr<DiscordUser> Author = nullptr;
		Ptr<DiscordGuild> Guild = nullptr;
		Ptr<DiscordChannel> Channel = nullptr;

		DiscordTimestamp Timestamp;
		DiscordTimestamp EditedTimestamp;

		DiscordMessageType Type = DiscordMessageType::DEFAULT;
		DiscordMessageActivity DiscordActivity;
		DiscordMessageReference MessageReference;

		int Flags = 0;

		bool bIsTTS = false;
		bool bIsPinned = false;
		bool bMentionEveryone = false;

		std::string Content;
		std::vector<DiscordUser> Mentions;
		std::vector<DiscordRole> MentionRoles;
		std::vector<DiscordEmbed> Embeds;
		std::vector<DiscordAttachment> Attachments;
		std::vector<DiscordChannelMention> MentionChannels;
		std::vector<DiscordMessageReaction> Reactions;

		DiscordMessage() {}
		DiscordMessage(DiscordClient* client, const Json& json);

		friend class DiscordClient;
		friend class DiscordChannel;
	};
}