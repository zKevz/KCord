#include "DiscordRestAPI.hpp"

#include <future>
#include <iostream>
#include <cpr/cpr.h>

namespace Discord
{
	struct HTTPData
	{
		Json Data {};
		AsyncCallback Callback {};

		std::string Path {};
	};

	std::string DiscordRestAPI::Token;

	static std::string Authorization = "Authorization";

	bool DiscordRestAPI::Get(const std::string& path, Json* out)
	{
		try
		{
			cpr::Header header =
			{
				{ "Authorization", "Bot " + Token },
				{ "User-Agent", "KCord" },
				{ "Content-Type", "application/json" }
			};

			auto response = cpr::Get(
				cpr::Url("https://discord.com/api/v6" + path),
				header
			);

			if (response.status_code != 200 &&
				response.status_code != 201 &&
				response.status_code != 204)
			{
#ifdef _DEBUG
				std::cout << "(" << path << ") Rest API Get error (" << response.status_code << ") (" << response.text << ")\n";
#endif
				return false;
			}

			*out = Json::parse(response.text);
			return true;
		}
		catch (const std::exception&)
		{
			return false;
		}
	}

	Json DiscordRestAPI::Put(const std::string& path, const Json& json, const AsyncCallback& asyncCallback)
	{
		try
		{
			std::string dump = json.dump();

			cpr::Header header =
			{
				{ "Authorization", "Bot " + Token },
				{ "User-Agent", "KCord" },
				{ "Content-Type", "application/json" },
				{ "Content-Length", std::to_string(dump.size()) }
			};

			auto response = cpr::Put(
				cpr::Url("https://discord.com/api/v6" + path),
				cpr::Body(dump),
				header
			);

			if (response.status_code != 200 &&
				response.status_code != 201 &&
				response.status_code != 204)
			{
				std::cout << "(" << path << ") Rest API Put error (" << response.status_code << ") (" << response.text << ")\n";
				return Json {};
			}
			else
			{
				if (asyncCallback)
					asyncCallback;
			}

			return Json::parse(response.text);
		}
		catch (const std::exception&)
		{
			return Json {};
		}
	}

	Json DiscordRestAPI::Post(const std::string& path, const Json& json, const AsyncCallback& asyncCallback)
	{
		try
		{
			cpr::Header header =
			{
				{ "Authorization", "Bot " + Token },
				{ "User-Agent", "KCord" },
				{ "Content-Type", "application/json" }
			};

			auto response = cpr::Post(
				cpr::Url("https://discord.com/api/v6" + path),
				cpr::Body(json.dump()),
				header
			);

			if (response.status_code != 200 &&
				response.status_code != 201 &&
				response.status_code != 204)
			{
				std::cout << "(" << path << ") Rest API Post error (" << response.status_code << ") (" << response.text << ")\n";
				return Json {};
			}
			else
			{
				if (asyncCallback)
					asyncCallback;
			}

			return Json::parse(response.text);
		}
		catch (const std::exception&)
		{
			return Json {};
		}
	}

	Json DiscordRestAPI::Patch(const std::string& path, const Json& json, const AsyncCallback& asyncCallback)
	{
		try
		{
			cpr::Header header =
			{
				{ "Authorization", "Bot " + Token },
				{ "User-Agent", "KCord" },
				{ "Content-Type", "application/json" }
			};

			auto response = cpr::Patch(
				cpr::Url("https://discord.com/api/v6" + path),
				cpr::Body(json.dump()),
				header
			);

			if (response.status_code != 200 &&
				response.status_code != 201 &&
				response.status_code != 204)
			{
				std::cout << "(" << path << ") Rest API Patch error (" << response.status_code << ") (" << response.text << ")\n";
				return Json {};
			}
			else
			{
				if (asyncCallback)
					asyncCallback;
			}

			return Json::parse(response.text);
		}
		catch (const std::exception&)
		{
			return Json {};
		}
	}

	Json DiscordRestAPI::Delete(const std::string& path, const AsyncCallback& asyncCallback)
	{
		try
		{
			cpr::Header header =
			{
				{ "Authorization", "Bot " + Token },
				{ "User-Agent", "KCord" },
				{ "Content-Type", "application/json" }
			};

			auto response = cpr::Delete(
				cpr::Url("https://discord.com/api/v6" + path),
				header
			);

			if (response.status_code != 200 &&
				response.status_code != 201 &&
				response.status_code != 204)
			{
				std::cout << "(" << path << ") Rest API Delete error (" << response.status_code << ") (" << response.text << ")\n";
				return Json {};
			}
			else
			{
				if (asyncCallback)
					asyncCallback;
			}

			return Json::parse(response.text);
		}
		catch (const std::exception&)
		{
			return Json {};
		}
	}

	Json DiscordRestAPI::PostFile(const std::string& path, const std::string& file, const Json& json, const AsyncCallback& asyncCallback)
	{
		try
		{
			cpr::Header header =
			{
				{ "Authorization", "Bot " + Token },
				{ "User-Agent", "KCord" },
				{ "Content-Type", "multipart/form-data" }
			};

			cpr::Multipart multipart({
				cpr::Part("file", cpr::File(file)),
				cpr::Part("payload_json", json.dump())
			});

			auto response = cpr::Post(
				cpr::Url("https://discord.com/api/v6" + path),
				multipart,
				header
			);

			if (response.status_code != 200 &&
				response.status_code != 201 &&
				response.status_code != 204)
			{
				std::cout << "(" << path << ") Rest API Post File error (" << response.status_code << ") (" << response.text << ")\n";
				return Json {};
			}
			else
			{
				if (asyncCallback)
					asyncCallback();
			}

			return Json::parse(response.text);
		}
		catch (const std::exception&)
		{
			return Json {};
		}
	}
}