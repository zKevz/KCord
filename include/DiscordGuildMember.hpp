#pragma once
#include <string>
#include <vector>
#include <optional>

#include "Snowflake.hpp"
#include "DiscordUser.hpp"
#include "DiscordTimestamp.hpp"

namespace Discord
{
	class DiscordGuildMemberCreate
	{
	public:
		std::string AccessToken;

		std::optional<bool> Mute;
		std::optional<bool> Deaf;
		std::optional<std::string> Nickname;
		std::optional<std::vector<Snowflake>> Roles;

		Json Serialize() const;
	};

	class DiscordGuildMemberEdit
	{
	public:
		std::optional<bool> Mute;
		std::optional<bool> Deaf;
		std::optional<Snowflake> ChannelId;
		std::optional<std::string> Nickname;
		std::optional<std::vector<Snowflake>> Roles;

		Json Serialize() const;
	};

	class DiscordGuildMember : public DiscordUser
	{
	public:
		bool IsDeaf() const { return bIsDeaf; }
		bool IsMuted() const { return bIsMuted; }
		bool IsPending() const { return bIsPending; }

		void UpdateUser(Ptr<DiscordUser> user);

		DiscordTimestamp GetJoinedAtTimestamp() const { return JoinedAt; }
		DiscordTimestamp GetPremiumSinceTimestamp() const { return PremiumSince; }

		std::string GetNickname() const { return Nickname; }
		std::string GetPermissionsString() const { return Permissions; }
		
		const std::vector<Snowflake>& GetRoleIds() const { return RoleIds; }
	private:
		bool bIsDeaf = false;
		bool bIsMuted = false;
		bool bIsPending = false;

		DiscordTimestamp JoinedAt;
		DiscordTimestamp PremiumSince;

		std::string Nickname;
		std::string Permissions;

		std::vector<Snowflake> RoleIds;

		DiscordGuildMember() {}
		DiscordGuildMember(const Json& json);

		friend class DiscordGuild;
		friend class DiscordClient;
		friend class DiscordVoiceState;
	};
}