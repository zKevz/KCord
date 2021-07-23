#pragma once
#include "DiscordGuildChannel.hpp"

namespace Discord
{
	class DiscordTextChannelEdit : public DiscordGuildChannelEdit
	{
	public:
		Snowflake ParentId = 0;

		int RateLimitPerUser = 0;
		int DefaultAutoArchiveDuration = 0;

		bool IsNSFW = false;

		std::string Topic;

		DiscordChannelType Type = DiscordChannelType::GUILD_TEXT;

		Json Serialize() const override;
	};

	class DiscordTextChannel : public DiscordGuildChannel
	{
	public:
		void Edit(const std::function<void(DiscordTextChannelEdit*)>& func);

		Snowflake GetLastMessageId() const { return LastMessageId; }

		int GetRateLimitPerUser() const { return RateLimitPerUser; }
		int GetDefaultAutoArchiveDuration() const { return DefaultAutoArchiveDuration; }

		std::string GetTopic() const { return Topic; }
	private:
		Snowflake LastMessageId;

		int RateLimitPerUser = 0;
		int DefaultAutoArchiveDuration = 0;

		std::string Topic;

		DiscordTextChannel() {}
		DiscordTextChannel(DiscordClient* client, DiscordGuild* guild, const Json& json);

		friend class DiscordGuild;
		friend class DiscordClient;
		friend class DiscordMessage;
		friend class DiscordChannel;
	};
}