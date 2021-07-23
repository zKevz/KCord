#pragma once
#include "Json.hpp"
#include "Snowflake.hpp"

namespace Discord
{
	enum DiscordPermissionsFlags : uint64_t
	{
		CREATE_INSTANT_INVITE = 1 << 0,
		KICK_MEMBERS = 1 << 1,
		BAN_MEMBERS = 1 << 2,
		ADMINISTRATOR = 1 << 3,
		MANAGE_CHANNELS = 1 << 4,
		MANAGE_GUILD = 1 << 5,
		ADD_REACTIONS = 1 << 6,
		VIEW_AUDIT_LOG = 1 << 7,
		PRIORITY_SPEAKER = 1 << 8,
		STREAM = 1 << 9,
		VIEW_CHANNEL = 1 << 10,
		SEND_MESSAGES = 1 << 11,
		SEND_TTS_MESSAGES = 1 << 12,
		MANAGE_MESSAGES = 1 << 13,
		EMBED_LINKS = 1 << 14,
		ATTACH_FILES = 1 << 15,
		READ_MESSAGE_HISTORY = 1 << 16,
		MENTION_EVERYONE = 1 << 17,
		USE_EXTERNAL_EMOJIS = 1 << 18,
		VIEW_GUILD_INSIGHTS = 1 << 19,
		CONNECT = 1 << 20,
		SPEAK = 1 << 21,
		MUTE_MEMBERS = 1 << 22,
		DEAFEN_MEMBERS = 1 << 23,
		MOVE_MEMBERS = 1 << 24,
		USE_VAD = 1 << 25,
		CHANGE_NICKNAME = 1 << 26,
		MANAGE_NICKNAMES = 1 << 27,
		MANAGE_ROLES = 1 << 28,
		MANAGE_WEBHOOKS = 1 << 29,
		MANAGE_EMOJIS = 1 << 30,
		USE_SLASH_COMMANDS = 1LL << 31,
		REQUEST_TO_SPEAK = 1LL << 32,
		MANAGE_THREADS = 1LL << 34,
		USE_PUBLIC_THREADS = 1LL << 35,
		USE_PRIVATE_THREADS = 1LL << 36
	};

	class DiscordPermissionOverwrite
	{
	public:
		void Deny(int64_t flag);
		void Allow(int64_t flag);

		bool IsDeny(int64_t flag);
		bool IsAllow(int64_t flag);

		Snowflake GetUserOrRoleId() const { return Id; }

		int IsRolePermission() const { return Type == 0; }
		int IsUserPermission() const { return Type == 1; }
		int64_t GetDenyBitset() const { return DenyBitset; }
		int64_t GetAllowBitset() const { return AllowBitset; }
	private:
		Snowflake Id;
		int Type = 0;
		int64_t AllowBitset = 0;
		int64_t DenyBitset = 0;

		Json Serialize() const;

		DiscordPermissionOverwrite() {}
		DiscordPermissionOverwrite(const Json& json);

		friend class DiscordChannel;
		friend class DiscordRoleInfo;
		friend class DiscordGuildChannel;
		friend class DiscordChannelCreate;
		friend class DiscordGuildChannelEdit;
	};
}
