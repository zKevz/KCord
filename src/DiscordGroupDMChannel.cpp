#include "DiscordGroupDMChannel.hpp"
#include "DiscordRestAPI.hpp"

namespace Discord
{
	void DiscordGroupDMChannel::Edit(const std::function<void(DiscordGroupDMChannelEdit*)>& func)
	{
		DiscordGroupDMChannelEdit edit;
		edit.Name = Name;

		func(&edit);

		DiscordRestAPI::Patch("/channels/" + Id.GetValueString(), edit.Serialize());
	}

	void DiscordGroupDMChannel::AddRecipient(const Ptr<DiscordUser> user, const std::string& token)
	{
		Json json;
		json["nickname"] = user->Username;
		json["access_token"] = token;
		DiscordRestAPI::Put("/channels/" + Id.GetValueString() + "/recipients/" + user->Id.GetValueString(), json);
	}

	void DiscordGroupDMChannel::AddRecipient(const DiscordUser& user, const std::string& token)
	{
		AddRecipient(std::make_shared<DiscordUser>(user), token);
	}

	void DiscordGroupDMChannel::RemoveRecipient(Snowflake id)
	{
		DiscordRestAPI::Delete("/channels/" + Id.GetValueString() + "/recipients/" + id.GetValueString());
	}

	void DiscordGroupDMChannel::RemoveRecipient(const Ptr<DiscordUser> user)
	{
		RemoveRecipient(user->Id);
	}

	void DiscordGroupDMChannel::RemoveRecipient(const DiscordUser& user)
	{
		RemoveRecipient(user.Id);
	}

	DiscordGroupDMChannel::DiscordGroupDMChannel(DiscordClient* client, const Json& json) : DiscordChannel(client, json)
	{
		if (CHECK("owner_id"))
			OwnerId = json["owner_id"].get<std::string>();

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

		if (CHECK("name"))
			Name = json["name"].get<std::string>();

		if (CHECK("icon"))
			IconHash = json["icon"].get<std::string>();
	}

	Json DiscordGroupDMChannelEdit::Serialize() const
	{
		Json json;
		json["name"] = Name;
		return json;
	}
}