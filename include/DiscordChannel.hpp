#pragma once
#include <map>
#include <string>
#include <optional>
#include <filesystem>

#include "Snowflake.hpp"
#include "DiscordUser.hpp"
#include "DiscordEmbed.hpp"
#include "DiscordInvite.hpp"
#include "DiscordMessage.hpp"
#include "DiscordRestAPI.hpp"
#include "DiscordTimestamp.hpp"
#include "DiscordPermissions.hpp"
#include "DiscordChannelType.hpp"

namespace Discord
{
	enum class DiscordVideoQualityMode
	{
		AUTO = 1,
		FULL = 2
	};

	class DiscordGuildChannelEdit
	{
	public:
		int Position = 0;

		std::string Name;

		std::map<Snowflake, DiscordPermissionOverwrite> Permissions;

		virtual Json Serialize() const;
	};

	class DiscordChannelCreate
	{
	public:
		std::optional<Snowflake> ParentId;

		std::optional<int> Bitrate = 0; // voice channel only
		std::optional<int> Position = 0;
		std::optional<int> UserLimit = 0; // voice channel only
		std::optional<int> RateLimitPerUser = 0;

		std::optional<bool> IsNSFW = false;

		std::string Name;
		std::optional<std::string> Topic;
		std::optional<std::map<Snowflake, DiscordPermissionOverwrite>> Permissions;

		std::optional<DiscordChannelType> Type = DiscordChannelType::GUILD_TEXT;

		Json Serialize() const;
	};

	class DiscordChannel
	{
	public:
		static DiscordChannel* Create(DiscordClient* client, DiscordGuild* guild, const Json& data);

		DiscordMessage Send(const DiscordEmbed& embed);
		DiscordMessage Send(const std::string& msg = "", bool tts = false, std::optional<DiscordEmbed> embed = {});
		DiscordMessage SendFile(const std::string& file);

		void PinMessage(Snowflake id);
		void PinMessage(const Ptr<DiscordMessage> message);
		void PinMessage(const DiscordMessage& message);

		void EditMessage(Snowflake id, const std::function<void(DiscordMessageEdit*)>& func);
		void EditMessage(Ptr<DiscordMessage> message, const std::function<void(DiscordMessageEdit*)>& func);
		void EditMessage(DiscordMessage& message, const std::function<void(DiscordMessageEdit*)>& func);

		void UnpinMessage(Snowflake id);
		void UnpinMessage(const Ptr<DiscordMessage> message);
		void UnpinMessage(const DiscordMessage& message);

		void DeleteMessage(Snowflake id);
		void DeleteMessage(const Ptr<DiscordMessage> message);
		void DeleteMessage(const DiscordMessage& message);
		void DeleteMessages(const std::vector<Snowflake>& ids);

		void AddMessageReaction(Snowflake id, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback = {});
		void AddMessageReaction(Snowflake id, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback = {});
		void AddMessageReaction(const Ptr<DiscordMessage> message, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback = {});
		void AddMessageReaction(const Ptr<DiscordMessage> message, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback = {});
		void AddMessageReaction(const DiscordMessage& message, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback = {});
		void AddMessageReaction(const DiscordMessage& message, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback = {});

		void RemoveOwnMessageReaction(Snowflake id, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback = {});
		void RemoveOwnMessageReaction(Snowflake id, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback = {});
		void RemoveOwnMessageReaction(const Ptr<DiscordMessage> message, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback = {});
		void RemoveOwnMessageReaction(const Ptr<DiscordMessage> message, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback = {});
		void RemoveOwnMessageReaction(const DiscordMessage& message, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback = {});
		void RemoveOwnMessageReaction(const DiscordMessage& message, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback = {});

		void RemoveUserMessageReaction(Snowflake id, const Ptr<DiscordEmoji> emoji, Snowflake userId, const AsyncCallback& asyncCallback = {});
		void RemoveUserMessageReaction(Snowflake id, const DiscordEmoji& emoji, Snowflake userId, const AsyncCallback& asyncCallback = {});
		void RemoveUserMessageReaction(const Ptr<DiscordMessage> message, const Ptr<DiscordEmoji> emoji, Snowflake userId, const AsyncCallback& asyncCallback = {});
		void RemoveUserMessageReaction(const Ptr<DiscordMessage> message, const DiscordEmoji& emoji, Snowflake userId, const AsyncCallback& asyncCallback = {});
		void RemoveUserMessageReaction(const DiscordMessage& message, const Ptr<DiscordEmoji> emoji, Snowflake userId, const AsyncCallback& asyncCallback = {});
		void RemoveUserMessageReaction(const DiscordMessage& message, const DiscordEmoji& emoji, Snowflake userId, const AsyncCallback& asyncCallback = {});

		void RemoveAllMessageReactions(Snowflake id, const AsyncCallback& asyncCallback = {});
		void RemoveAllMessageReactions(const Ptr<DiscordMessage> message, const AsyncCallback& asyncCallback = {});
		void RemoveAllMessageReactions(const DiscordMessage& message, const AsyncCallback& asyncCallback = {});

		void RemoveAllMessageReactionsForEmoji(Snowflake id, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback = {});
		void RemoveAllMessageReactionsForEmoji(Snowflake id, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback = {});
		void RemoveAllMessageReactionsForEmoji(const Ptr<DiscordMessage> message, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback = {});
		void RemoveAllMessageReactionsForEmoji(const Ptr<DiscordMessage> message, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback = {});
		void RemoveAllMessageReactionsForEmoji(const DiscordMessage& message, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback = {});
		void RemoveAllMessageReactionsForEmoji(const DiscordMessage& message, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback = {});

		std::vector<DiscordUser> GetMessageReactionUser(Snowflake id, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback = {});
		std::vector<DiscordUser> GetMessageReactionUser(Snowflake id, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback = {});
		std::vector<DiscordUser> GetMessageReactionUser(const Ptr<DiscordMessage> message, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback = {});
		std::vector<DiscordUser> GetMessageReactionUser(const Ptr<DiscordMessage> message, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback = {});
		std::vector<DiscordUser> GetMessageReactionUser(const DiscordMessage& message, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback = {});
		std::vector<DiscordUser> GetMessageReactionUser(const DiscordMessage& message, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback = {});

		void DeleteOrClose();
		void TriggerTypingIndicator();

		std::vector<DiscordInvite> GetInvites();
		std::vector<DiscordMessage> GetMessages(int limit = 50, std::optional<Snowflake> id = {}, std::optional<MessageDirection> direction = {});
		std::vector<DiscordMessage> GetPinnedMessages();

		Ptr<DiscordMessage> GetMessageById(Snowflake id);

		Snowflake GetId() const { return Id; }
		DiscordChannelType GetType() const { return Type; }

		virtual ~DiscordChannel() {}
	private:
		Snowflake Id;		
		DiscordChannelType Type = DiscordChannelType::GUILD_TEXT;

		DiscordClient* Client = nullptr;

		DiscordChannel() {}
		DiscordChannel(DiscordClient* client, const Json& json);

		friend class DiscordGuild;
		friend class DiscordClient;
		friend class DiscordMessage;

		friend class DiscordDMChannel;
		friend class DiscordTextChannel;
		friend class DiscordNewsChannel;
		friend class DiscordStoreChannel;
		friend class DiscordVoiceChannel;
		friend class DiscordGuildChannel;
		friend class DiscordGroupDMChannel;
		friend class DiscordChannelCategory;
	};
}