#include "DiscordEmoji.hpp"
#include "DiscordGuild.hpp"

namespace Discord
{
	DiscordEmoji DiscordEmoji::FromUnicode(const std::string& unicode)
	{
		DiscordEmoji emoji;
		emoji.Name = unicode;
		return emoji;
	}

	DiscordEmoji::DiscordEmoji(const Json& json)
	{
		if (CHECK("id"))
			Id = json["id"].get<std::string>();

		if (CHECK("name"))
			Name = json["name"].get<std::string>();

		if (CHECK("user"))
		{
			User = DiscordUser(json["user"]);
		}

		if (CHECK("roles"))
		{
			const auto& roles = json["roles"];
			for (const auto& object : roles)
			{
				RolesAllowed.emplace_back(object.get<std::string>());
			}
		}

		if (CHECK("require_colons"))
			bRequireColons = json["require_colons"].get<bool>();

		if (CHECK("managed"))
			bIsManaged = json["managed"].get<bool>();

		if (CHECK("animated"))
			bIsAnimated = json["animated"].get<bool>();

		if (CHECK("available"))
			bIsAvailable = json["available"].get<bool>();
	}

	Json DiscordEmojiCreate::Serialize() const
	{
		Json json;
		json["name"] = Name;
		json["image"] = DiscordUtils::GetImageData(ImageFilePath);

		for (const auto& role : RolesAllowed)
		{
			json["roles"].emplace_back(role.GetValueString());
		}

		return json;
	}

	Json DiscordEmojiEdit::Serialize() const
	{
		Json json;
		json["name"] = Name;
		json["roles"] = Json::array();

		for (const auto& id : RolesAllowed)
			json["roles"].emplace_back(id.GetValueString());

		return json;
	}
}