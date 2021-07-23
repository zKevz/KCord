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