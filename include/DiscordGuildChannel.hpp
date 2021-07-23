#pragma once
#include "DiscordRestAPI.hpp"
#include "DiscordChannel.hpp"

namespace Discord
{
	class DiscordGuild;
	class DiscordGuildChannel : public DiscordChannel
	{
	public:
		void EditPermission(Snowflake id, const std::function<void(DiscordPermissionOverwrite*)>& func);
		void EditPermission(const DiscordUser& user, const std::function<void(DiscordPermissionOverwrite*)>& func);
		void EditPermission(const Ptr<DiscordUser> user, const std::function<void(DiscordPermissionOverwrite*)>& func);
		void EditPermission(const DiscordRole& role, const std::function<void(DiscordPermissionOverwrite*)>& func);
		void EditPermission(const Ptr<DiscordRole> role, const std::function<void(DiscordPermissionOverwrite*)>& func);

		void DeletePermission(Snowflake id);
		void DeletePermission(const DiscordUser& user);
		void DeletePermission(const Ptr<DiscordUser> user);
		void DeletePermission(const DiscordRole& role);
		void DeletePermission(const Ptr<DiscordRole> user);

		Snowflake GetGuildId() const { return GuildId; }
		Snowflake GetParentId() const { return ParentId; }

		int GetPosition() const { return Position; }

		bool IsNSFW() const { return bIsNSFW; }

		std::string GetName() const { return Name; }

		const std::map<Snowflake, DiscordPermissionOverwrite>& GetPermissions() const { return Permissions; }
	private:
		Snowflake GuildId;
		Snowflake ParentId;
		DiscordGuild* Guild = nullptr;

		int Position = 0;
		bool bIsNSFW = false;

		std::string Name;
		std::map<Snowflake, DiscordPermissionOverwrite> Permissions;

		DiscordGuildChannel() {}
		DiscordGuildChannel(DiscordClient* client, DiscordGuild* guild, const Json& json);

		friend class DiscordGuild;
		friend class DiscordClient;
		friend class DiscordChannel;
		friend class DiscordTextChannel;
		friend class DiscordNewsChannel;
		friend class DiscordStoreChannel;
		friend class DiscordVoiceChannel;
		friend class DiscordGroupDMChannel;
		friend class DiscordChannelCategory;
	};
}
