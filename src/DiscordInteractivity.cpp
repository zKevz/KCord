#include "DiscordInteractivity.hpp"
#include "DiscordChannel.hpp"

#include <mutex>
#include <thread>

namespace Discord
{
	std::mutex GlobalMutex;

	void DiscordInteractivityService::Invoke(Ptr<DiscordMessage> message)
	{
		DiscordInteractivityPredicate predicate;
		predicate.AuthorId = message->GetAuthor()->GetId();
		predicate.ChannelId = message->GetChannel()->GetId();

		GlobalMutex.lock();

		for (auto& function : Functions)
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

	DiscordInteractivityResult DiscordInteractivityService::WaitForMessage(std::function<bool(DiscordInteractivityPredicate*)> predicate, DiscordTimeDuration duration)
	{
		auto ptr = &predicate;

		GlobalMutex.lock();
		Functions.insert({ ptr, nullptr });
		GlobalMutex.unlock();

		auto now = DiscordClock::now();
			
		while (DiscordClock::now() - now < duration - std::chrono::seconds(2) && !Functions[ptr])
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		auto result = Functions[ptr];

		GlobalMutex.lock();
		Functions.erase(ptr);
		GlobalMutex.unlock();

		if (result)
		{
			return *result;
		}

		return DiscordInteractivityResult();
	}
}