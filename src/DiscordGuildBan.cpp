#include "DiscordGuildBan.hpp"

namespace Discord
{
	DiscordGuildBan::DiscordGuildBan(const Json& json)
	{
		if (CHECK("reason"))
			Reason = json["reason"].get<std::string>();

		if (CHECK("user"))
			User = Ptr<DiscordUser>(new DiscordUser(json["user"]));
	}
}