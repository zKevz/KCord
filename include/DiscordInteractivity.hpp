#pragma once
#include <atomic>
#include <functional>

#include "Json.hpp"
#include "DiscordEmoji.hpp"
#include "DiscordMessage.hpp"

namespace Discord
{
	struct DiscordInteractivityPredicate
	{
		Snowflake AuthorId;
		Snowflake ChannelId;
		Snowflake MessageId;
		DiscordEmoji Emoji;
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
		void InvokeIncomingMessage(Ptr<DiscordMessage> message);
		void InvokeIncomingReaction(Ptr<DiscordMessage> message, Snowflake authorId, const DiscordEmoji& emoji);

		DiscordInteractivityResult WaitForMessage(DiscordTimeDuration duration = std::chrono::seconds(0));
		DiscordInteractivityResult WaitForMessage(std::function<bool(DiscordInteractivityPredicate*)> predicate, DiscordTimeDuration duration = std::chrono::seconds(0));

		DiscordInteractivityResult WaitForReaction(DiscordTimeDuration duration = std::chrono::seconds(0));
		DiscordInteractivityResult WaitForReaction(std::function<bool(DiscordInteractivityPredicate*)> predicate, DiscordTimeDuration duration = std::chrono::seconds(0));
	private:
		using DiscordInteractivityMap = std::map<std::function<bool(DiscordInteractivityPredicate*)>*, Ptr<DiscordInteractivityResult>>;

		DiscordInteractivityResult WaitIncoming(DiscordInteractivityMap& map, std::function<bool(DiscordInteractivityPredicate*)> predicate, DiscordTimeDuration duration);

		DiscordInteractivityMap IncomingMessageFunctions;
		DiscordInteractivityMap IncomingReactionFunctions;
	};
}