#include "DiscordDMChannel.hpp"

namespace Discord
{
	void DiscordDMChannel::AddRecipient(const Ptr<DiscordUser> user, const std::string& token)
	{
		Json json;
		json["nickname"] = user->Username;
		json["access_token"] = token;
		DiscordRestAPI::Put("/channels/" + Id.GetValueString() + "/recipients/" + user->Id.GetValueString(), json);
	}

	void DiscordDMChannel::AddRecipient(const DiscordUser& user, const std::string& token)
	{
		AddRecipient(std::make_shared<DiscordUser>(user), token);
	}

	void DiscordDMChannel::RemoveRecipient(Snowflake id)
	{
		DiscordRestAPI::Delete("/channels/" + Id.GetValueString() + "/recipients/" + id.GetValueString());
	}

	void DiscordDMChannel::RemoveRecipient(const Ptr<DiscordUser> user)
	{
		RemoveRecipient(user->Id);
	}

	void DiscordDMChannel::RemoveRecipient(const DiscordUser& user)
	{
		RemoveRecipient(user.Id);
	}

	DiscordDMChannel::DiscordDMChannel(DiscordClient* client, const Json& json) : DiscordChannel(client, json)
	{
		if (CHECK("last_message_id"))
			LastMessageId = json["last_message_id"].get<std::string>();

		if (CHECK("recipients"))
		{
			const auto& recipients = json["recipients"];
			for (const auto& object : recipients)
			{
				DiscordUser user = DiscordUser(object);
				Recipients.insert_or_assign(user.Id, user);
			}
		}
	}
}