#pragma once
#include <string>
#include "DiscordIntents.hpp"

namespace Discord
{
	struct DiscordClientConfig
	{
		int Intents = DiscordIntents::ALL;
		int Threshold = 250;

		bool EnableDM = false;
		bool PrefixCaseSensitive = true;
		bool EnableMentionPrefix = false;

		std::string Token;
		std::string Prefix;
	};
}