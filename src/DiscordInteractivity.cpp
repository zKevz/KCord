#include "DiscordInteractivity.hpp"
#include "DiscordChannel.hpp"

#include <mutex>
#include <thread>

namespace Discord
{
	std::mutex GlobalMutex;

	void DiscordInteractivityService::InvokeIncomingMessage(Ptr<DiscordMessage> message)
	{
		DiscordInteractivityPredicate predicate;
		predicate.AuthorId  = message->GetAuthor()->GetId();
		predicate.ChannelId = message->GetChannel()->GetId();

		GlobalMutex.lock();

		for (auto& function : IncomingMessageFunctions)
		{
			if (function.first->operator()(&predicate))
			{
				DiscordInteractivityResult result;
				result.Message = message;
				result.IsSuccess = true;

				function.second = std::shared_ptr<DiscordInteractivityResult>(new DiscordInteractivityResult(result));
			}
		}

		GlobalMutex.unlock();
	}

	void DiscordInteractivityService::InvokeIncomingReaction(Ptr<DiscordMessage> message, Snowflake authorId, const DiscordEmoji& emoji)
	{
		DiscordInteractivityPredicate predicate;
		predicate.Emoji = emoji;
		predicate.AuthorId = authorId;
		predicate.MessageId = message->GetId();
		predicate.ChannelId = message->GetChannel()->GetId();

		GlobalMutex.lock();

		for (auto& function : IncomingReactionFunctions)
		{
			if (function.first->operator()(&predicate))
			{
				DiscordInteractivityResult result;
				result.Message = message;
				result.IsSuccess = true;

				function.second = std::shared_ptr<DiscordInteractivityResult>(new DiscordInteractivityResult(result));
			}
		}

		GlobalMutex.unlock();
	}

	DiscordInteractivityResult DiscordInteractivityService::WaitIncoming(DiscordInteractivityMap& map, std::function<bool(DiscordInteractivityPredicate*)> predicate, DiscordTimeDuration duration)
	{
		auto ptr = &predicate;

		GlobalMutex.lock();

		map.insert({ ptr, nullptr });

		GlobalMutex.unlock();

		auto now = DiscordClock::now();

		while (DiscordClock::now() - now < duration - std::chrono::seconds(2) && !map[ptr])
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		auto result = map[ptr];

		GlobalMutex.lock();

		map.erase(ptr);

		GlobalMutex.unlock();

		if (result)
		{
			return *result;
		}

		return DiscordInteractivityResult();
	}

	DiscordInteractivityResult DiscordInteractivityService::WaitForMessage(DiscordTimeDuration duration)
	{
		return WaitForMessage([](auto) { return true; }, duration);
	}

	DiscordInteractivityResult DiscordInteractivityService::WaitForMessage(std::function<bool(DiscordInteractivityPredicate*)> predicate, DiscordTimeDuration duration)
	{
		return WaitIncoming(IncomingMessageFunctions, predicate, duration);
	}

	DiscordInteractivityResult DiscordInteractivityService::WaitForReaction(DiscordTimeDuration duration)
	{
		return WaitForReaction([](auto) { return true; }, duration);
	}

	DiscordInteractivityResult DiscordInteractivityService::WaitForReaction(std::function<bool(DiscordInteractivityPredicate*)> predicate, DiscordTimeDuration duration)
	{
		return WaitIncoming(IncomingReactionFunctions, predicate, duration);
	}
}