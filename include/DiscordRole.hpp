#pragma once
#include <string>
#include <optional>

#include "Json.hpp"
#include "Snowflake.hpp"
#include "DiscordColor.hpp"
#include "DiscordPermissions.hpp"

namespace Discord
{
	class DiscordRoleInfo
	{
	public:
		std::optional<int> Permission;
		std::optional<bool> IsHoisted;
		std::optional<bool> IsMentionable;
		std::optional<std::string> Name;
		std::optional<DiscordColor> Color;

		Json Serialize() const;
	};

	class DiscordRoleTags
	{
	public:
		Snowflake GetBotId() const { return BotId; }
		Snowflake GetIntegrationId() const { return IntegrationId; }
		bool IsPremiumSubscriber() const { return bIsPremiumSubscriber; }
	private:
		Snowflake BotId;
		Snowflake IntegrationId;
		bool bIsPremiumSubscriber = false;

		DiscordRoleTags() {}
		DiscordRoleTags(const Json& json);
		friend class DiscordRole;
	};

	class DiscordRole
	{
	public:
		Snowflake GetId() const { return Id; }

		DiscordColor GetColor() const { return Color; }
		DiscordRoleTags GetRoleTags() const { return RoleTags; }

		int GetPosition() const { return Position; }
		int GetPermissionBitSet() const { return Permissions; }

		bool IsHoisted() const { return bIsHoist; }
		bool IsManaged() const { return bIsManaged; }
		bool IsMentionable() const { return bIsMentionable; }

		std::string GetName() const { return Name; }
	private:
		Snowflake Id;
		DiscordColor Color;
		DiscordRoleTags RoleTags;

		int Position = 0;
		int Permissions = 0;

		bool bIsHoist = false;
		bool bIsManaged = false;
		bool bIsMentionable = false;

		std::string Name;

		DiscordRole() {}
		DiscordRole(const Json& json);

		friend class DiscordGuild;
		friend class DiscordEmoji;
		friend class DiscordClient;
		friend class DiscordMessage;
	};
}