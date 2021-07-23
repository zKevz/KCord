#define _CRT_SECURE_NO_WARNINGS
#include "DiscordTimestamp.hpp"
#include <iomanip>
#include <sstream>
#include <iostream>

namespace Discord
{
	DiscordTimestamp::DiscordTimestamp(int seconds)
	{
		auto time = time_t(seconds);
		auto* t = std::localtime(&time);
		Year = t->tm_year;
		Month = t->tm_mon;
		Day = t->tm_mday;
		Hour = t->tm_hour;
		Minute = t->tm_min;
		Second = static_cast<double>(t->tm_sec);
	}

	DiscordTimestamp::DiscordTimestamp(const std::string& timestamp)
	{
		std::sscanf(timestamp.c_str(), "%d-%d-%dT%d:%d:%lfZ", &Year, &Month, &Day, &Hour, &Minute, &Second);
	}

	std::string DiscordTimestamp::GetString() const
	{
		tm time;
		time.tm_year = Year ;
		time.tm_mon = Month;
		time.tm_mday = Day;
		time.tm_hour = Hour;
		time.tm_min = Minute;
		time.tm_sec = static_cast<int>(Second);

		std::ostringstream ss;
		ss << std::put_time(&time, "%c");
		return ss.str();
	}

	std::string DiscordTimestamp::GetISO8601String() const
	{
		tm time;
		time.tm_year = Year;
		time.tm_mon = Month;
		time.tm_mday = Day;
		time.tm_hour = Hour;
		time.tm_min = Minute;
		time.tm_sec = static_cast<int>(Second);

		std::ostringstream ss;
		ss << std::put_time(&time, "%FT%TZ");
		return ss.str();
	}

	DiscordTimestamp DiscordTimestamp::Now()
	{
		time_t now = time(0);
		tm* t = gmtime(&now);

		DiscordTimestamp timestamp;
		timestamp.Year = t->tm_year;
		timestamp.Month = t->tm_mon;
		timestamp.Day = t->tm_mday;
		timestamp.Hour = t->tm_hour;
		timestamp.Minute = t->tm_min;
		timestamp.Second = static_cast<double>(t->tm_sec);

		return timestamp;
	}

	bool DiscordTimestamp::operator==(const DiscordTimestamp& other) const
	{
		return Year == other.Year && Month == other.Month && Day == other.Day && Hour == other.Hour && Minute == other.Minute && (int)Second == (int)other.Second;
	}

	bool DiscordTimestamp::operator!=(const DiscordTimestamp& other) const
	{
		return Year != other.Year || Month != other.Month || Day != other.Day || Hour != other.Hour || Minute != other.Minute || (int)Second != (int)other.Second;
	}

	static constexpr int64_t MinuteInSeconds = 60;
	static constexpr int64_t HourInSeconds = MinuteInSeconds * 60;
	static constexpr int64_t DayInSeconds = HourInSeconds * 24;
	static constexpr int64_t MonthInSeconds = DayInSeconds * 31;
	static constexpr int64_t YearInSeconds = MonthInSeconds * 12;

	DiscordTimestamp DiscordTimestamp::operator-(const DiscordTimestamp& other) const
	{
		tm time;
		time.tm_year = Year;
		time.tm_mon = Month;
		time.tm_mday = Day;
		time.tm_hour = Hour;
		time.tm_min = Minute;
		time.tm_sec = static_cast<int>(Second);

		time_t otherTotal = 
			((int64_t)other.Year * YearInSeconds) +
			((int64_t)other.Month * MonthInSeconds) +
			((int64_t)other.Day * DayInSeconds) +
			((int64_t)other.Hour * HourInSeconds) +
			((int64_t)other.Minute * MinuteInSeconds) +
			(int64_t)other.Second;

		time_t total = std::mktime(&time) - otherTotal;

		tm* t = std::gmtime(&total);

		DiscordTimestamp timestamp;
		timestamp.Year = t->tm_year;
		timestamp.Month = t->tm_mon;
		timestamp.Day = t->tm_mday;
		timestamp.Hour = t->tm_hour;
		timestamp.Minute = t->tm_min;
		timestamp.Second = static_cast<double>(t->tm_sec);

		return timestamp;
	}

	DiscordTimestamp DiscordTimestamp::operator+(const DiscordTimestamp& other) const
	{
		tm time;
		time.tm_year = Year;
		time.tm_mon = Month;
		time.tm_mday = Day;
		time.tm_hour = Hour;
		time.tm_min = Minute;
		time.tm_sec = static_cast<int>(Second);

		time_t otherTotal =
			((int64_t)other.Year * YearInSeconds) +
			((int64_t)other.Month * MonthInSeconds) +
			((int64_t)other.Day * DayInSeconds) +
			((int64_t)other.Hour * HourInSeconds) +
			((int64_t)other.Minute * MinuteInSeconds) +
			(int64_t)other.Second;

		time_t total = std::mktime(&time) + otherTotal;

		tm* t = std::gmtime(&total);

		DiscordTimestamp timestamp;
		timestamp.Year = t->tm_year;
		timestamp.Month = t->tm_mon;
		timestamp.Day = t->tm_mday;
		timestamp.Hour = t->tm_hour;
		timestamp.Minute = t->tm_min;
		timestamp.Second = static_cast<double>(t->tm_sec);

		return timestamp;
	}
}