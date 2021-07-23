#pragma once
#include "DiscordChannel.hpp"

namespace Discord
{
	class DiscordGroupDMChannelEdit
	{
	public:
		std::string Name;

		Json Serialize() const;
	};

	class DiscordGroupDMChannel : public DiscordChannel
	{
	public:
		void Edit(const std::function<void(DiscordGroupDMChannelEdit*)>& func);

		Snowflake GetOwnerId() const { return OwnerId; }
		Snowflake GetLastMessageId() const { return LastMessageId; }

		std::string GetName() const { return Name; }
		std::string GetIconHash() const { return IconHash; }

		const std::map<Snowflake, DiscordUser>& GetRecipients() const { return Recipients; }
	private:
		Snowflake OwnerId;
		Snowflake LastMessageId;

		std::string Name;
		std::string IconHash;

		std::map<Snowflake, DiscordUser> Recipients;

		DiscordGroupDMChannel() {}
		DiscordGroupDMChannel(DiscordClient* client, const Json& json);

		friend class DiscordGuild;
		friend class DiscordClient;
		friend class DiscordChannel;
	};
}
