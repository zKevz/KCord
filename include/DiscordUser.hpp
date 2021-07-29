#pragma once
#include <string>

#include "Json.hpp"
#include "Snowflake.hpp"

namespace Discord
{
	enum class DiscordPremiumType
	{
		NONE,
		NITRO_CLASSIC,
		NITRO
	};

	enum class DiscordStatus
	{
		IDLE,
		DO_NOT_DISTURB,
		ONLINE,
		OFFLINE
	};

	enum DiscordUserFlags
	{
		NONE = 0,
		DISCORD_EMPLOYEE				= 1 << 0,
		PARTNERED_SERVER_OWNER			= 1 << 1,
		HYPESQUAD_EVENTS				= 1 << 2,
		BUG_HUNTER_LEVEL_1				= 1 << 3,
		HOUSE_BRAVERY					= 1 << 6,
		HOUSE_BRILLIANCE				= 1 << 7,
		HOUSE_BALANCE					= 1 << 8,
		EARLY_SUPPORTER					= 1 << 9,
		TEAM_USER						= 1 << 10,
		BUG_HUNTER_LEVEL_2				= 1 << 14,
		VERIFIED_BOT					= 1 << 16,
		EARLY_VERIFIED_BOT_DEVELOPER	= 1 << 17,
		DISCORD_CERTIFIED_MODERATOR		= 1 << 18
	};

	class DiscordUser
	{
	public:
		Snowflake GetId() const { return Id; }
		DiscordPremiumType GetPremiumType() const { return DiscordPremiumType; }

#pragma region DiscordUserFlags
		int GetFlags() const { return Flags; }
		int GetPublicFlags() const { return PublicFlags; }

		bool IsTeamUser() const { return Flags & DiscordUserFlags::TEAM_USER; }
		bool IsVerifiedBot() const { return Flags & DiscordUserFlags::VERIFIED_BOT; }
		bool IsHouseBravery() const { return Flags & DiscordUserFlags::HOUSE_BRAVERY; }
		bool IsHouseBalance() const { return Flags & DiscordUserFlags::HOUSE_BALANCE; }
		bool IsEarlySupporter() const { return Flags & DiscordUserFlags::EARLY_SUPPORTER; }
		bool IsHouseBrilliance() const { return Flags & DiscordUserFlags::HOUSE_BRILLIANCE; }
		bool IsDiscordEmployee() const { return Flags & DiscordUserFlags::DISCORD_EMPLOYEE; }
		bool IsBugHunterLevel1() const { return Flags & DiscordUserFlags::BUG_HUNTER_LEVEL_1; }
		bool IsBugHunterLevel2() const { return Flags & DiscordUserFlags::BUG_HUNTER_LEVEL_2; }
		bool IsOnHypeSquadEvents() const { return Flags & DiscordUserFlags::HYPESQUAD_EVENTS; }
		bool IsPartneredServerOwner() const { return Flags & DiscordUserFlags::PARTNERED_SERVER_OWNER; }
		bool IsDiscordCertifiedModerator() const { return Flags & DiscordUserFlags::DISCORD_CERTIFIED_MODERATOR; }
		bool IsEarlyVerifiedBotDeveloper() const { return Flags & DiscordUserFlags::EARLY_VERIFIED_BOT_DEVELOPER; }
#pragma endregion

		bool IsBot() const { return bIsBot; }
		bool IsSystem() const { return bIsSystem; }
		bool IsVerified() const { return bIsVerified; }
		bool IsMFAEnabled() const { return bIsMFAEnabled; }

		std::string GetEmail() const { return Email; }
		std::string GetUsername() const { return Username; }
		std::string GetFullName() const { return Username + "#" + Discriminator; }
		std::string GetAvatarHash() const { return AvatarHash; }
		std::string GetDiscriminator() const { return Discriminator; }
		std::string GetLocalLanguage() const { return LocalLanguage; }
		std::string GetMentionString() const { return "<@" + std::to_string(Id.GetValue()) + ">"; }

		std::string GetAvatarUrl();
	private:
		Snowflake Id;
		DiscordPremiumType DiscordPremiumType = DiscordPremiumType::NONE;

		int Flags = 0;
		int PublicFlags = 0;

		bool bIsBot = false;
		bool bIsSystem = false;
		bool bIsMFAEnabled = false;
		bool bIsVerified = false;

		std::string Username;
		std::string Discriminator;
		std::string AvatarHash;
		std::string LocalLanguage;
		std::string Email;

		DiscordUser() {}
		DiscordUser(const Json& json);

		friend class DiscordEmoji;
		friend class DiscordGuild;
		friend class DiscordClient;
		friend class DiscordInvite;
		friend class DiscordChannel;
		friend class DiscordMessage;
		friend class DiscordGuildBan;
		friend class DiscordPresence;
		friend class DiscordDMChannel;
		friend class DiscordGuildMember;
		friend class DiscordInviteCreatedEventInfo;
		friend class DiscordGroupDMChannel;
	};
}