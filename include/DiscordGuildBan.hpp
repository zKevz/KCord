#pragma once
#include "DiscordUser.hpp"

namespace Discord
{
	class DiscordGuildBan
	{
	private:
		std::string GetReason() { return Reason; }
		Ptr<DiscordUser> GetUser() { return User; }
	public:
		std::string Reason;
		Ptr<DiscordUser> User;

		DiscordGuildBan() {}
		DiscordGuildBan(const Json& json);
	};
}