#pragma once
#include "DiscordChannel.hpp"

namespace Discord
{
	class DiscordDMChannel : public DiscordChannel
	{
	public:
		void AddRecipient(const Ptr<DiscordUser> user, const std::string& token);
		void AddRecipient(const DiscordUser& user, const std::string& token);

		void RemoveRecipient(Snowflake id);
		void RemoveRecipient(const Ptr<DiscordUser> user);
		void RemoveRecipient(const DiscordUser& user);

		Snowflake GetLasMessageId() const { return LastMessageId; }
		const std::map<Snowflake, DiscordUser>& GetRecipients() const { return Recipients; }
	private:
		Snowflake LastMessageId;
		std::map<Snowflake, DiscordUser> Recipients;

		DiscordDMChannel() {}
		DiscordDMChannel(DiscordClient* client, const Json& json);

		friend class DiscordGuild;
		friend class DiscordClient;
		friend class DiscordChannel;
	};
}