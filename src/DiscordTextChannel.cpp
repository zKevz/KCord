#include "DiscordTextChannel.hpp"

namespace Discord
{
	void DiscordTextChannel::Edit(const std::function<void(DiscordTextChannelEdit*)>& func)
	{
		DiscordTextChannelEdit edit;
		edit.DefaultAutoArchiveDuration = DefaultAutoArchiveDuration;
		edit.IsNSFW = bIsNSFW;
		edit.Name = Name;
		edit.ParentId = ParentId;
		edit.Permissions = Permissions;
		edit.Position = Position;
		edit.RateLimitPerUser = RateLimitPerUser;
		edit.Topic = Topic;
		edit.Type = Type;

		func(&edit);

		DiscordRestAPI::Patch("/channels/" + Id.GetValueString(), edit.Serialize());
	}

	DiscordTextChannel::DiscordTextChannel(DiscordClient* client, DiscordGuild* guild, const Json& json) : DiscordGuildChannel(client, guild, json)
	{
		if (CHECK("topic"))
			Topic = json["topic"].get<std::string>();

		if (CHECK("last_message_id"))
			LastMessageId = json["last_message_id"].get<std::string>();

		if (CHECK("rate_limit_per_user"))
			RateLimitPerUser = json["rate_limit_per_user"].get<int>();

		if (CHECK("default_auto_archive_duration"))
			DefaultAutoArchiveDuration = json["default_auto_archive_duration"].get<int>();
	}

	Json DiscordTextChannelEdit::Serialize() const
	{
		Json json = DiscordGuildChannelEdit::Serialize();
		json["type"] = static_cast<int>(Type);
		json["topic"] = Topic;
		json["nsfw"] = IsNSFW;
		json["rate_limit_per_user"] = RateLimitPerUser;
		json["parent_id"] = ParentId.GetValueString();
		json["default_auto_archive_duration"] = DefaultAutoArchiveDuration;
		return json;
	}
}