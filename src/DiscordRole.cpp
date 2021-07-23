#include "DiscordRole.hpp"

namespace Discord
{
	DiscordRole::DiscordRole(const Json& json)
	{
		if (CHECK("id"))
			Id = json["id"].get<std::string>();

		if (CHECK("name"))
			Name = json["name"].get<std::string>();

		if (CHECK("color"))
			Color = json["color"].get<int>();

		if (CHECK("hoist"))
			bIsHoist = json["hoist"].get<bool>();

		if (CHECK("position"))
			Position = json["position"].get<int>();

		if (CHECK("permissions"))
			Permissions = json["permissions"].get<int>();

		if (CHECK("managed"))
			bIsManaged = json["managed"].get<bool>();

		if (CHECK("mentionable"))
			bIsMentionable = json["mentionable"].get<bool>();

		if (CHECK("tags"))
			RoleTags = DiscordRoleTags(json["tags"]);
	}

	DiscordRoleTags::DiscordRoleTags(const Json& json)
	{
		if (CHECK("bot_id"))
			BotId = json["bot_id"].get<std::string>();

		if (CHECK("integration_id"))
			IntegrationId = json["integration_id"].get<std::string>();

		if (CHECK("premium_subscriber"))
			bIsPremiumSubscriber = true;
	}

	Json DiscordRoleInfo::Serialize() const
	{
		Json json;

		if (Name.has_value())
			json["name"] = Name.value();

		if (Permission.has_value())
			json["permissions"] = Permission.value();

		if (Color.has_value())
			json["color"] = Color->ToInt32();

		if (IsHoisted.has_value())
			json["hoist"] = IsHoisted.value();

		if (IsMentionable.has_value())
			json["mentionable"] = IsMentionable.value();

		return json;
	}
}