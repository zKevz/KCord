#pragma once
#include <string>
#include <vector>

#include "Snowflake.hpp"
#include "DiscordUser.hpp"

namespace Discord
{
	enum class DiscordActivityType
	{
		GAME,
		STREAMING,
		LISTENING,
		WATCHING,
		CUSTOM,
		COMPETING
	};


	enum DiscordActivityFlags
	{
		INSTANCE = 1 << 0,
		JOIN = 1 << 1,
		SPECTATE = 1 << 2,
		JOIN_REQUEST = 1 << 3,
		SYNC = 1 << 4,
		PLAY = 1 << 5
	};

	class DiscordActivityTimestamp
	{
	public:
		int End = 0;
		int Start = 0;

		DiscordActivityTimestamp() {}
		DiscordActivityTimestamp(const Json& json);
	};

	class DiscordActivityEmoji
	{
	public:
		Snowflake Id;
		bool bIsAnimated = false;
		std::string Name;

		DiscordActivityEmoji() {}
		DiscordActivityEmoji(const Json& json);
	};

	class DiscordActivityParty
	{
	public:
		std::string Id;
		int MaxSize = 0;
		int CurrentSize = 0;

		DiscordActivityParty() {}
		DiscordActivityParty(const Json& json);
	};

	class DiscordActivityAssets
	{
	public:
		std::string LargeImage;
		std::string LargeText;
		std::string SmallImage;
		std::string SmallText;

		DiscordActivityAssets() {}
		DiscordActivityAssets(const Json& json);
	};

	class DiscordActivitySecrets
	{
	public:
		std::string Join;
		std::string Spectate;
		std::string Match;

		DiscordActivitySecrets() {}
		DiscordActivitySecrets(const Json& json);
	};

	class DiscordActivityButton
	{
	public:
		std::string Url;
		std::string Label;

		DiscordActivityButton() {}
		DiscordActivityButton(const Json& json);
	};

	class DiscordActivity
	{
	public:
		Snowflake ApplicationId;

		DiscordActivityType Type = DiscordActivityType::GAME;
		DiscordActivityEmoji Emoji;
		DiscordActivityParty Party;
		DiscordActivityAssets Assets;
		DiscordActivitySecrets Secrets;
		DiscordActivityTimestamp DiscordActivityTimestamp;

		int Flags = 0;
		int CreatedAt = 0;

		bool bIsInstanced = false;

		std::string Url;
		std::string Name;
		std::string Details;
		std::string State;
		std::vector<DiscordActivityButton> Buttons;

		DiscordActivity() {}
		DiscordActivity(const Json& json);
	};
}