#include "DiscordUtils.hpp"
#include <random>
#include <chrono>
#include <algorithm>
#include <sstream>
#include <iomanip>

namespace Discord
{
    std::mt19937 rng(static_cast<uint32_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    
    int DiscordUtils::Random(int max)
    {
        std::uniform_int_distribution<int> dist(0, max);
        return dist(rng);
    }

    int DiscordUtils::Random(int min, int max)
    {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }

    bool DiscordUtils::IsNumber(const std::string& str)
    {
        if (str.empty())
            return false;
        const auto& begin = str.begin();
        return std::all_of(begin + (*begin == '-' ? 1 : 0), str.end(), [](const char& c) { return std::isalpha(c); });
    }

    bool DiscordUtils::ToInt64(const std::string& str, int64_t* value)
    {
        errno = 0;
        const char* ptr = str.c_str();
        char* ptr2;

        *value = std::strtoll(ptr, &ptr2, 10);

        return ptr != ptr2 && errno != ERANGE;
    }

    bool DiscordUtils::ToUInt64(const std::string& str, uint64_t* value)
    {
        errno = 0;
        const char* ptr = str.c_str();
        char* ptr2;

        *value = std::strtoull(ptr, &ptr2, 10);

        return ptr != ptr2 && errno != ERANGE;
    }

    bool DiscordUtils::StringEqual(const std::string& str1, const std::string& str2, bool caseSensitive)
    {
        if (str1.size() != str2.size())
            return false;

        if (caseSensitive)
        {
            return str1 == str2;
        }
        else
        {
            for (size_t i = 0; i < str1.size(); ++i)
            {
                if (std::toupper(str1[i]) != std::toupper(str2[i]))
                    return false;
            }
            return true;
        }
    }

    bool DiscordUtils::StringEndsWith(const std::string& str, const std::string& str2)
    {
        return str.size() >= str2.size() && !str.compare(str.size() - str2.size(), str2.size(), str2);
    }

    std::string DiscordUtils::URLEncode(const std::string& str)
    {
        std::ostringstream escaped;
        escaped.fill('0');
        escaped << std::hex;

        for (std::string::const_iterator i = str.begin(), n = str.end(); i != n; ++i)
        {
            std::string::value_type c = (*i);

            // Keep alphanumeric and other accepted characters intact
            if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
            {
                escaped << c;
                continue;
            }

            // Any other characters are percent-encoded
            escaped << std::uppercase;
            escaped << '%' << std::setw(2) << int((unsigned char)c);
            escaped << std::nouppercase;
        }

        return escaped.str();
    }

    std::string DiscordUtils::GetImageData(const std::string& file)
    {
        std::string format;
        std::string base64 = GetBase64File(file, &format);

        return "data:image/" + format + ";base64," + base64;
    }

    std::string DiscordUtils::GetBase64File(const std::string& file, std::string* formatOut)
    {
        if (!std::filesystem::exists(file))
        {
            throw "File " + file + " not found!";
        }

        if (StringEndsWith(file, ".png"))
        {
            *formatOut = "png";
        }
        else if (StringEndsWith(file, ".jpeg") || StringEndsWith(file, ".jpg"))
        {
            *formatOut = "jpeg";
        }
        else if (StringEndsWith(file, ".gif"))
        {
            *formatOut = "gif";
        }
        else
        {
            throw "Invalid file extension. Supported: .png, .jpg / .jpeg, and .gif";
        }
        
        std::string result;
        std::ifstream fs(file, std::ios::binary);

        size_t size = std::filesystem::file_size(file);
        uint8_t* data = new uint8_t[size];

        if (!data)
            throw "Can't allocate memory";

        memset(data, 0, size);

        if (fs.read(reinterpret_cast<char*>(data), size))
        {
            result = Base64Encode(data, static_cast<unsigned int>(size));
        }
        else
        {
            throw "File successfully opened, but it can't be read (" + file + ")";
        }

        delete[] data;

        return result;
    }

    std::vector<std::string> DiscordUtils::SplitString(const std::string& str, const std::string& delimiter)
    {
        std::vector<std::string> tokens;

        size_t prev = 0,
            pos = 0;

        do
        {
            pos = str.find(delimiter, prev);

            if (pos == std::string::npos)
                pos = str.length();

            std::string token = str.substr(prev, pos - prev);

            if (!token.empty())
                tokens.push_back(token);

            prev = pos + delimiter.length();

        } while (pos < str.length() && prev < str.length());

        return tokens;
    }
}