#pragma once
#include <string>
#include <time.h>

namespace Discord
{
	class DiscordTimestamp
	{
	public:
		DiscordTimestamp() {}
		DiscordTimestamp(int seconds);
		DiscordTimestamp(const std::string& timestamp);

		bool Null() const { return Year == 0 && Month == 0 && Day == 0 && Hour == 0 && Minute == 0 && Second == 0; }

		int GetYear() const { return Year; }
		int GetMonth() const { return Month; }
		int GetDay() const { return Day; }
		int GetHour() const { return Hour; }
		int GetMinute() const { return Minute; }
		double GetSecond() const { return Second; }

		std::string GetString() const;
		std::string GetISO8601String() const;

		static DiscordTimestamp Now();

		bool operator==(const DiscordTimestamp& other) const;
		bool operator!=(const DiscordTimestamp& other) const;

		DiscordTimestamp operator-(const DiscordTimestamp& other) const;
		DiscordTimestamp operator+(const DiscordTimestamp& other) const;
	private:
		int Year = 0;
		int Month = 0;
		int Day = 0;
		int Hour = 0;
		int Minute = 0;
		double Second = 0;
	};
}