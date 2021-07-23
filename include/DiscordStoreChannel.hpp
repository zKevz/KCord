#pragma once
#include "DiscordGuildChannel.hpp"

namespace Discord
{
	class DiscordStoreChannelEdit : public DiscordGuildChannelEdit
	{
	public:
		Snowflake ParentId;
		bool IsNSFW = false;

		Json Serialize() const override;
	};

	class DiscordStoreChannel : public DiscordGuildChannel
	{
	public:
		void Edit(const std::function<void(DiscordStoreChannelEdit*)>& func);
	private:
		DiscordStoreChannel() {}
		DiscordStoreChannel(DiscordClient* client, DiscordGuild* guild, const Json& json) : DiscordGuildChannel(client, guild, json) {}

		friend class DiscordGuild;
		friend class DiscordClient;
		friend class DiscordChannel;
		friend class DiscordMessage;
	};
}

