#pragma once
#include "DiscordGuildChannel.hpp"

namespace Discord
{
	class DiscordNewsChannelEdit : public DiscordGuildChannelEdit
	{
	public:
		Snowflake ParentId;

		int DefaultAutoArchiveDuration = 0;

		bool IsNSFW = false;

		std::string Topic;

		DiscordChannelType Type;

		Json Serialize() const override;
	};

	class DiscordNewsChannel : public DiscordGuildChannel
	{
	public:
		void Edit(const std::function<void(DiscordNewsChannelEdit*)>& func);

		Snowflake GetLastMessageId() const { return LastMessageId; }

		int GetDefaultAutoArchiveDuration() const { return DefaultAutoArchiveDuration; }

		std::string GetTopic() const { return Topic; }
	private:
		Snowflake LastMessageId;

		int DefaultAutoArchiveDuration = 0;

		std::string Topic;

		DiscordNewsChannel() {}
		DiscordNewsChannel(DiscordClient* client, DiscordGuild* guild, const Json& json);

		friend class DiscordGuild;
		friend class DiscordClient;
		friend class DiscordChannel;
	};
}
