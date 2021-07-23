#include "DiscordActivity.hpp"

namespace Discord
{
	DiscordActivity::DiscordActivity(const Json& json)
	{
		if (CHECK("name"))
			Name = json["name"].get<std::string>();

		if (CHECK("type"))
			Type = static_cast<DiscordActivityType>(json["type"].get<int>());
		
		if (CHECK("url"))
			Url = json["url"].get<std::string>();

		if (CHECK("created_at"))
			CreatedAt = json["created_at"].get<int>();

		if (CHECK("timestamps"))
			DiscordActivityTimestamp = Discord::DiscordActivityTimestamp(json["timestamps"]);

		if (CHECK("application_id"))
			ApplicationId = json["application_id"].get<std::string>();

		if (CHECK("details"))
			Details = json["details"].get<std::string>();

		if (CHECK("state"))
			State = json["state"].get<std::string>();

		if (CHECK("emoji"))
			Emoji = DiscordActivityEmoji(json["emoji"]);

		if (CHECK("party"))
			Party = DiscordActivityParty(json["party"]);

		if (CHECK("assets"))
			Assets = DiscordActivityAssets(json["assets"]);

		if (CHECK("secrets"))
			Secrets = DiscordActivitySecrets(json["secrets"]);

		if (CHECK("instance"))
			bIsInstanced = json["instance"].get<bool>();

		if (CHECK("flags"))
			Flags = json["flags"].get<int>();

		if (CHECK("buttons"))
		{
			const auto& buttons = json["buttons"];
			for (const auto& object : buttons)
			{
				DiscordActivityButton button = DiscordActivityButton(object);
				Buttons.emplace_back(button);
			}
		}
	}

	DiscordActivityTimestamp::DiscordActivityTimestamp(const Json& json)
	{
		if (CHECK("start"))
			Start = json["start"].get<int>();

		if (CHECK("end"))
			End = json["end"].get<int>();
	}

	DiscordActivityEmoji::DiscordActivityEmoji(const Json& json)
	{
		if (CHECK("name"))
			Name = json["name"].get<std::string>();

		if (CHECK("id"))
			Id = json["id"].get<std::string>();

		if (CHECK("animated"))
			bIsAnimated = json["animated"].get<bool>();
	}

	DiscordActivityParty::DiscordActivityParty(const Json& json)
	{
		if (CHECK("id"))
			Id = json["id"].get<std::string>();

		if (CHECK("size"))
		{
			CurrentSize = json["size"][0];
			MaxSize = json["size"][1];
		}
	}

	DiscordActivityAssets::DiscordActivityAssets(const Json& json)
	{
		if (CHECK("large_image"))
			LargeImage = json["large_image"].get<std::string>();

		if (CHECK("large_text"))
			LargeText = json["large_text"].get<std::string>();

		if (CHECK("small_image"))
			SmallImage = json["small_image"].get<std::string>();

		if (CHECK("small_text"))
			SmallText = json["small_text"].get<std::string>();
	}

	DiscordActivitySecrets::DiscordActivitySecrets(const Json& json)
	{
		if (CHECK("join"))
			Join = json["join"].get<std::string>();

		if (CHECK("spectate"))
			Spectate = json["spectate"].get<std::string>();

		if (CHECK("match"))
			Match = json["match"].get<std::string>();
	}

	DiscordActivityButton::DiscordActivityButton(const Json& json)
	{
		if (CHECK("url"))
			Url = json["url"].get<std::string>();

		if (CHECK("label"))
			Label = json["label"].get<std::string>();
	}
}