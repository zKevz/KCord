#include "DiscordGuildChannel.hpp"
#include "DiscordGuild.hpp"

namespace Discord
{
	void DiscordGuildChannel::EditPermission(Snowflake id, const std::function<void(DiscordPermissionOverwrite*)>& func)
	{
		if (auto perm = Permissions.find(id); perm != Permissions.end())
		{
			auto copy = perm->second;
			func(&copy);

			DiscordRestAPI::Put("/channels/" + Id.GetValueString() + "/permissions/" + id.GetValueString(), copy.Serialize());
		}
	}

	void DiscordGuildChannel::EditPermission(const DiscordUser& user, const std::function<void(DiscordPermissionOverwrite*)>& func)
	{
		EditPermission(user.GetId(), func);
	}

	void DiscordGuildChannel::EditPermission(const Ptr<DiscordUser> user, const std::function<void(DiscordPermissionOverwrite*)>& func)
	{
		EditPermission(user->GetId(), func);
	}

	void DiscordGuildChannel::EditPermission(const DiscordRole& role, const std::function<void(DiscordPermissionOverwrite*)>& func)
	{
		EditPermission(role.GetId(), func);
	}

	void DiscordGuildChannel::EditPermission(const Ptr<DiscordRole> role, const std::function<void(DiscordPermissionOverwrite*)>& func)
	{
		EditPermission(role->GetId(), func);
	}

	void DiscordGuildChannel::DeletePermission(Snowflake id)
	{
		DiscordRestAPI::Delete("/channels/" + Id.GetValueString() + "/permissions/" + id.GetValueString());
	}

	void DiscordGuildChannel::DeletePermission(const DiscordUser& user)
	{
		DeletePermission(user.GetId());
	}

	void DiscordGuildChannel::DeletePermission(const Ptr<DiscordUser> user)
	{
		DeletePermission(user->GetId());
	}

	void DiscordGuildChannel::DeletePermission(const DiscordRole& role)
	{
		DeletePermission(role.GetId());
	}

	void DiscordGuildChannel::DeletePermission(const Ptr<DiscordRole> role)
	{
		DeletePermission(role->GetId());
	}

	DiscordGuildChannel::DiscordGuildChannel(DiscordClient* client, DiscordGuild* guild, const Json& json) : DiscordChannel(client, json), Guild(guild)
	{
		if (CHECK("guild_id"))
			GuildId = json["guild_id"].get<std::string>();

		if (CHECK("position"))
			Position = json["position"].get<int>();

		if (CHECK("permission_overwrites"))
		{
			const auto& permissionOverwrites = json["permission_overwrites"];
			for (const auto& object : permissionOverwrites)
			{
				DiscordPermissionOverwrite permission = DiscordPermissionOverwrite(object);
				Permissions.insert_or_assign(permission.Id, permission);
			}
		}

		if (CHECK("name"))
			Name = json["name"].get<std::string>();

		if (CHECK("nsfw"))
			bIsNSFW = json["nsfw"].get<bool>();

		if (CHECK("parent_id"))
			ParentId = json["parent_id"].get<std::string>();
	}
}
