#pragma once
#include "DiscordUser.hpp"
#include "DiscordTimestamp.hpp"

namespace Discord
{
	enum class DiscordInviteTargetType
	{
		STREAM = 1,
		EMBEDDED_APPLICATION = 2
	};

	class DiscordInvite
	{
	public:
		int Uses = 0;
		int MaxAge = 0;
		int MaxUses = 0;
		int ApproximateMemberCount = 0;
		int ApproximatePresenceCount = 0;

		bool IsTemporary = false;

		std::string Code;

		DiscordUser Inviter;
		DiscordUser TargetUser;

		DiscordTimestamp CreatedAt;
		DiscordTimestamp ExpiresAt;
		DiscordInviteTargetType TargetType = DiscordInviteTargetType::STREAM;

		DiscordInvite() {}
		DiscordInvite(const Json& json);

		friend class DiscordChannel;
	};
}