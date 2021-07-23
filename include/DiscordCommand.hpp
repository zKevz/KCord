#pragma once
#include <map>
#include <memory>
#include <iostream>
#include <functional>

#include "DiscordUtils.hpp"
#include "DiscordEventStruct.hpp"

namespace Discord
{
	static_assert(sizeof(long double) == 8, "Sizeof long double should've been 8, :/");

	enum class DiscordRoleTargetType
	{
		ANY,
		ALL
	};

	class DiscordCommandContext : public DiscordMessageEventInfo, public DiscordAbstractChannelEvent
	{
	public:
		template<typename T>
		bool GetArgument(size_t index, T* value, bool rest = false) const
		{
			static_assert(
				std::is_integral<T>::value ||
				std::is_floating_point<T>::value ||
				std::is_same<T, std::string>::value ||
				std::is_same<T, Ptr<DiscordUser>>::value ||
				std::is_same<T, Ptr<DiscordRole>>::value ||
				std::is_same<T, Ptr<DiscordChannel>>::value ||
				std::is_same<T, Ptr<DiscordGuildMember>>::value,
				"[DiscordCommandArgument] Invalid template type!"
				"Supported: integral type (int, short, etc), "
				"floating type (float, double, etc), std::string, "
				"Ptr<DiscordUser>, Ptr<DiscordRole>, "
				"Ptr<DiscordGuildMember> and Ptr<DiscordChannel>.");

			if (index >= Parameters.size())
			{
				return false;
			}

			std::string param = Parameters[index];

			if (rest)
			{
				for (size_t i = index + 1; i < Parameters.size(); ++i)
				{
					param += " " + Parameters[i];
				}
			}

			if constexpr (std::is_same<T, std::string>::value)
			{
				*value = *(T*)&param;
				return true;
			}
			else if constexpr (std::is_integral<T>::value)
			{
				if constexpr (std::is_unsigned<T>::value)
				{
					uint64_t val;
					if (DiscordUtils::ToUInt64(param, &val))
					{
						if (val < 0)
							return false;

						if constexpr (std::is_same<uint8_t, T>::value)
						{
							if (val > UINT8_MAX)
								return false;
						}
						else if constexpr (std::is_same<uint16_t, T>::value)
						{
							if (val > UINT16_MAX)
								return false;
						}
						else if constexpr (std::is_same<uint32_t, T>::value)
						{
							if (val > UINT32_MAX)
								return false;
						}
						else if constexpr (std::is_same<uint64_t, T>::value)
						{
							if (val > UINT64_MAX)
								return false;
						}
					}
					else return false;
					*value = static_cast<T>(val);
				}
				else
				{
					int64_t val;
					if (DiscordUtils::ToInt64(param, &val))
					{
						if constexpr (std::is_same<int8_t, T>::value)
						{
							if (val > INT8_MAX || val < INT8_MIN)
								return false;
						}
						else if constexpr (std::is_same<int16_t, T>::value)
						{
							if (val > INT16_MAX || val < INT16_MIN)
								return false;
						}
						else if constexpr (std::is_same<int32_t, T>::value)
						{
							if (val > INT_MAX || val < INT_MIN)
								return false;
						}
						else if constexpr (std::is_same<int64_t, T>::value)
						{
							if (val > INT64_MAX || val < INT64_MIN)
								return false;
						}
					}
					else return false;
					*value = static_cast<T>(val);
				}

				return true;
			}
			else if constexpr (std::is_floating_point<T>::value)
			{
				char* e;
				errno = 0;
				auto val = std::strtold(param.c_str(), &e);

				*value = static_cast<T>(val);

				if (*e != '\0' || errno != 0)
				{
					return false;
				}

				if constexpr (std::is_same<float, T>::value)
				{
					if (value > FLT_MAX || value < FLT_MIN)
						return false;
				}
				else if constexpr (std::is_same<double, T>::value || std::is_same<long double, T>::value)
				{
					if (value > DBL_MAX || value < DBL_MIN)
						return false;
				}

				return true;
			}
			else if constexpr (std::is_same<T, Ptr<DiscordUser>>::value || std::is_same<T, Ptr<DiscordGuildMember>>::value)
			{
				if (param.size() < 5)
					return false;

				if (param.back() == '>')
				{
					size_t offset = 3;

					if (param.find("<@!") != 0)
					{
						if (param.find("<@") != 0)
							return false;
						offset = 2;
					}

					uint64_t id = 0;

					std::string str = param.substr(offset, param.size() - offset - 1);

					if (DiscordUtils::ToUInt64(str, &id))
					{
						if (auto member = Guild->GetMemberById(id))
						{
							*value = static_cast<T>(member);
							return true;
						}
					}
				}
				return false;
			}
			else if constexpr (std::is_same<T, Ptr<DiscordRole>>::value)
			{
				if (param.size() < 5)
					return false;

				if (param.back() == '>')
				{
					if (param.find("<@&") == 0)
					{
						uint64_t id = 0;

						std::string str = param.substr(3, param.size() - 4);

						if (DiscordUtils::ToUInt64(str, &id))
						{
							if (auto role = Guild->GetRoleById(id))
							{
								*value = static_cast<T>(role);
								return true;
							}
						}
					}
				}
				return false;
			}
			else if constexpr (std::is_same<T, Ptr<DiscordChannel>>::value)
			{
				if (param.size() < 4)
					return false;

				if (param.back() == '>')
				{
					if (param.find("<#") == 0)
					{
						uint64_t id = 0;

						std::string str = param.substr(2, param.size() - 3);

						if (DiscordUtils::ToUInt64(str, &id))
						{
							if (auto channel = Guild->GetChannelById(id))
							{
								*value = static_cast<T>(channel);
								return true;
							}
						}
					}
				}
				return false;
			}
			else
			{
				throw "Invalid template type";
			}
		}
	private:
		std::vector<std::string> Parameters;

		friend class DiscordCommandService;
	};

	using DiscordCommandCallback = std::function<void(const DiscordCommandContext&)>;

	class DiscordCommandInfo
	{
	public:
		DiscordRoleTargetType RoleTarget = DiscordRoleTargetType::ANY;

		std::optional<int64_t> RequirePermission;
		std::optional<std::string> Description;
		std::optional<std::vector<std::string>> RequireRoles;
	private:
		DiscordCommandCallback Callback;

		friend class DiscordClient;
		friend class DiscordCommandService;
	};

	class DiscordCommandService
	{
	public:
		struct DiscordCommandServiceContainer
		{
			std::string Content;
			DiscordClient* Client = nullptr;
			Ptr<DiscordMessage> Message = nullptr;
		};

		std::map<std::string, DiscordCommandInfo> Commands;

		void Add(const std::string& command, const DiscordCommandInfo& callback);
		void Process(Ptr<DiscordCommandServiceContainer> ptr);
	};
}