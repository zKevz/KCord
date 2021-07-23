#include "DiscordGuildMember.hpp"

namespace Discord
{
	void DiscordGuildMember::UpdateUser(Ptr<DiscordUser> user)
	{
		DiscordUser::DiscordUser(*user);
	}

	DiscordGuildMember::DiscordGuildMember(const Json& json) : DiscordUser(json["user"])
	{
		if (CHECK("nick"))
			Nickname = json["nick"].get<std::string>();

		if (CHECK("roles"))
		{
			const auto& roles = json["roles"];
			for (const auto& object : roles)
			{
				RoleIds.emplace_back(object.get<std::string>());
			}
		}

		if (CHECK("joined_at"))
			JoinedAt = json["joined_at"].get<std::string>();

		if (CHECK("premium_since"))
			PremiumSince = json["premium_since"].get<std::string>();

		if (CHECK("deaf"))
			bIsDeaf = json["deaf"].get<bool>();

		if (CHECK("mute"))
			bIsMuted = json["mute"].get<bool>();

		if (CHECK("pending"))
			bIsPending = json["pending"].get<bool>();

		if (CHECK("permissions"))
			Permissions = json["permissions"].get<std::string>();
	}

	Json DiscordGuildMemberCreate::Serialize() const
	{
		Json json;
		json["access_token"] = AccessToken;

		if (Nickname.has_value())
			json["nick"] = Nickname.value();

		if (Roles.has_value())
		{
			json["roles"] = Json::array();
			for (const auto& role : Roles.value())
			{
				json["roles"].emplace_back(role.GetValueString());
			}
		}

		if (Mute.has_value())
			json["mute"] = Mute.value();

		if (Deaf.has_value())
			json["deaf"] = Deaf.value();

		return json;
	}

	Json DiscordGuildMemberEdit::Serialize() const
	{
		Json json;

		if (ChannelId.has_value())
			json["channel_id"] = ChannelId.value().GetValueString();

		if (Nickname.has_value())
			json["nick"] = Nickname.value();

		if (Roles.has_value())
		{
			json["roles"] = Json::array();
			for (const auto& role : Roles.value())
			{
				json["roles"].emplace_back(role.GetValueString());
			}
		}

		if (Mute.has_value())
			json["mute"] = Mute.value();

		if (Deaf.has_value())
			json["deaf"] = Deaf.value();

		return json;
	}
}