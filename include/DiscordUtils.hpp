#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

#include "DiscordBase64Utils.hpp"

namespace Discord
{
	class DiscordUtils
	{
	public:
		static int Random(int max);
		static int Random(int min, int max);

		static bool IsNumber(const std::string& str);
		static bool ToInt64(const std::string& str, int64_t* value);
		static bool ToUInt64(const std::string& str, uint64_t* value);
		static bool StringEqual(const std::string& str1, const std::string& str2, bool caseSensitive = true);
		static bool StringEndsWith(const std::string& str, const std::string& str2);
		
		static std::string URLEncode(const std::string& str);
		static std::string GetImageData(const std::string& file);
		static std::string GetBase64File(const std::string& file, std::string* formatOut);

		static std::vector<std::string> SplitString(const std::string& str, const std::string& delimiter);
	};
}
