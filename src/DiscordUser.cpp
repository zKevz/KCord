#include "DiscordUser.hpp"
#include <iostream>

namespace Discord
{
	std::string DiscordUser::GetAvatarUrl()
	{
		return "https://cdn.discordapp.com/avatars/" + Id.GetValueString() + "/" + AvatarHash + ".png?size=1024";
	}

	DiscordUser::DiscordUser(const Json& json)
	{
		if (CHECK("id"))
			Id = json["id"].get<std::string>();

		if (CHECK("username"))
			Username = json["username"].get<std::string>();

		if (CHECK("discriminator"))
			Discriminator = json["discriminator"].get<std::string>();

		if (CHECK("avatar"))
			AvatarHash = json["avatar"].get<std::string>();

		if (CHECK("bot"))
			bIsBot = json["bot"].get<bool>();

		if (CHECK("system"))
			bIsSystem = json["system"].get<bool>();

		if (CHECK("mfa_enabled"))
			bIsMFAEnabled = json["mfa_enabled"].get<bool>();

		if (CHECK("locale"))
			LocalLanguage = json["locale"].get<std::string>();

		if (CHECK("verified"))
			bIsVerified = json["verified"].get<bool>();

		if (CHECK("email"))
			Email = json["email"].get<std::string>();

		if (CHECK("flags"))
			Flags = json["flags"].get<int>();

		if (CHECK("premium_type"))
			DiscordPremiumType = static_cast<Discord::DiscordPremiumType>(json["premium_type"].get<int>());

		if (CHECK("public_flags"))
			PublicFlags = json["public_flags"].get<int>();
	}
}