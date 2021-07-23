#pragma once
namespace Discord
{
	class DiscordColor
	{
	public:
		int R = 0;
		int G = 0;
		int B = 0;

		DiscordColor() {}
		DiscordColor(int r, int g, int b) : R(r), G(g), B(b) {}
		DiscordColor(int value)
		{
			R = value & 255;
			G = (value >> 8) & 255;
			B = (value >> 16) & 255;
		}

		int ToInt32() const
		{
			return R | G << 8 | B << 16;
		}

		bool Null() const
		{
			return R == 0 && G == 0 && B == 0;
		}

		bool operator==(const DiscordColor& other) const
		{
			return R == other.R && G == other.G && B == other.B;
		}

		bool operator!=(const DiscordColor& other) const
		{
			return R != other.R || G != other.G || B != other.B;
		}
	};
}