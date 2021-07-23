#pragma once
#include <string>

class Snowflake
{
public:
	Snowflake() {}
	Snowflake(unsigned long long val) : Value(val) {}
	Snowflake(const std::string& msg) : Value(std::stoull(msg)) {}

	bool operator==(const Snowflake& other) const
	{
		return Value == other.Value;
	}

	bool operator==(const unsigned long long& value) const
	{
		return Value == value;
	}

	bool operator<(const Snowflake& other) const
	{
		return Value < other.Value;
	}

	unsigned long long GetValue() const { return Value; }
	std::string GetValueString() const { return std::to_string(Value); }
private:
	unsigned long long Value = 0;
};

