#pragma once
#include <map>
#include <string>

#include "Snowflake.hpp"
#include "DiscordRole.hpp"
#include "DiscordUser.hpp"

namespace Discord
{
	class DiscordEmojiEdit
	{
	public:
		Json Serialize() const;

		std::string Name;
		std::vector<Snowflake> RolesAllowed;
	};

	class DiscordGuild;
	class DiscordEmoji
	{
	public:
		static DiscordEmoji FromUnicode(const std::string& unicode);

		Snowflake GetId() const { return Id; }

		bool RequireColons() const { return bRequireColons; }
		bool IsManaged() const { return bIsManaged; }
		bool IsAnimated() const { return bIsAnimated; }
		bool IsAvailable() const { return bIsAvailable; }

		std::string GetName() const { return Name; }
		std::string GetString() const { return Id.GetValue() ? Name + ":" + Id.GetValueString() : Name;  }

		DiscordUser& GetUser() { return User; }

	private:
		Snowflake Id;
		DiscordUser User; // User who created this emoji

		bool bRequireColons = false;
		bool bIsManaged = false;
		bool bIsAnimated = false;
		bool bIsAvailable = false;

		std::string Name;
		std::vector<Snowflake> RolesAllowed;

		DiscordEmoji() {}
		DiscordEmoji(const Json& json);

		friend class DiscordGuild;
		friend class DiscordClient;
		friend class DiscordMessageReaction;
	};
}