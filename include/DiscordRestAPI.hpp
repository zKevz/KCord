#pragma once
#define _SILENCE_CXX17_C_HEADER_DEPRECATION_WARNING
#include <string>
#include <fstream>

#include "Json.hpp"

namespace Discord
{
	using AsyncCallback = std::function<void()>;
	class DiscordRestAPI
	{
	public:
		static std::string Token;

		static bool Get(const std::string& path, Json* out);
		static Json Put(const std::string& path, const Json& json, const AsyncCallback& asyncCallback = {});
		static Json Post(const std::string& path, const Json& json, const AsyncCallback& asyncCallback = {});
		static Json Patch(const std::string& path, const Json& json, const AsyncCallback& asyncCallback = {});
		static Json Delete(const std::string& path, const AsyncCallback& asyncCallback = {});
		static Json PostFile(const std::string& path, const std::string& file, const Json& json, const AsyncCallback& asyncCallback = {});

		friend class DiscordClient;
		friend class DiscordChannel;
	};
}