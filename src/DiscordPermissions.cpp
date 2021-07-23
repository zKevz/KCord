#include "DiscordPermissions.hpp"

namespace Discord
{
	void DiscordPermissionOverwrite::Deny(int64_t flag)
	{
		if (AllowBitset & flag)
			AllowBitset &= ~flag;
		DenyBitset |= flag;
	}

	void DiscordPermissionOverwrite::Allow(int64_t flag)
	{
		if (DenyBitset & flag)
			DenyBitset &= ~flag;
		AllowBitset |= flag;
	}

	bool DiscordPermissionOverwrite::IsDeny(int64_t flag)
	{
		return DenyBitset & flag;
	}

	bool DiscordPermissionOverwrite::IsAllow(int64_t flag)
	{
		return AllowBitset & flag;
	}

	Json DiscordPermissionOverwrite::Serialize() const
	{
		Json json;
		json["id"] = Id.GetValueString();
		json["type"] = Type;
		json["deny"] = DenyBitset;
		json["allow"] = AllowBitset;
		return json;
	}

	DiscordPermissionOverwrite::DiscordPermissionOverwrite(const Json& json)
	{
		if (CHECK("id"))
			Id = json["id"].get<std::string>();

		if (CHECK("type"))
			Type = json["type"].get<std::string>() == "role" ? 0 : 1;

		if (CHECK("deny"))
		{
			if (json["deny"].is_string())
				DenyBitset = std::stoll(json["deny"].get<std::string>());
			else
				DenyBitset = json["deny"].get<int64_t>();
		}

		if (CHECK("allow"))
		{
			if (json["allow"].is_string())
				AllowBitset = std::stoll(json["allow"].get<std::string>());
			else
				AllowBitset = json["allow"].get<int64_t>();
		}
	}
}