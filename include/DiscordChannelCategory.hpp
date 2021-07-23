#pragma once
#include "DiscordGuildChannel.hpp"

namespace Discord
{
	class DiscordChannelCategory : public DiscordGuildChannel
	{
	public:
		void Edit(const std::function<void(DiscordGuildChannelEdit*)>& func);

		DiscordChannelCategory() {}
		DiscordChannelCategory(DiscordClient* client, DiscordGuild* guild, const Json& json) : DiscordGuildChannel(client, guild, json) {}

		friend class DiscordChannel;
	};
}
