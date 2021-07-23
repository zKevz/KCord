#include "DiscordInvite.hpp"

namespace Discord
{
	DiscordInvite::DiscordInvite(const Json& json)
	{
		if (CHECK("code"))
			Code = json["code"].get<std::string>();

		if (CHECK("inviter"))
			Inviter = DiscordUser(json["inviter"]);

		if (CHECK("target_type"))
			TargetType = static_cast<DiscordInviteTargetType>(json["target_type"].get<int>());

		if (CHECK("target_user"))
			TargetUser = DiscordUser(json["target_user"]);

		if (CHECK("approximate_presence_count"))
			ApproximatePresenceCount = json["approximate_presence_count"].get<int>();

		if (CHECK("approximate_member_count"))
			ApproximateMemberCount = json["approximate_member_count"].get<int>();

		if (CHECK("expires_at"))
			ExpiresAt = json["expires_at"].get<std::string>();
	}
}