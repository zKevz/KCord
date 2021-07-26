#pragma once
#include <functional>

#include "Json.hpp"
#include "DiscordMessage.hpp"
#include <atomic>

namespace Discord
{
	struct DiscordInteractivityPredicate
	{
		Snowflake AuthorId;
		Snowflake ChannelId;
	};

	class DiscordInteractivityResult
	{
	public:
		bool IsSuccess = false;
		Ptr<DiscordMessage> Message = nullptr;
	};

	class DiscordInteractivityContainer
	{
	public:
		Ptr<DiscordInteractivityResult> Result = nullptr;
		std::function<bool(DiscordInteractivityPredicate*)>* Predicate = nullptr;
	};

	class DiscordInteractivityService
	{
	public:
		void Invoke(Ptr<DiscordMessage> message);

		DiscordInteractivityResult WaitForMessage(std::function<bool(DiscordInteractivityPredicate*)> predicate, DiscordTimeDuration duration);

	private:
		std::map<std::function<bool(DiscordInteractivityPredicate*)>*, Ptr<DiscordInteractivityResult>> Functions;
	};
}