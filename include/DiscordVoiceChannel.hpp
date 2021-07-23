#pragma once
#include "DiscordGuildChannel.hpp"

namespace Discord
{
	class DiscordVoiceChannelEdit : public DiscordGuildChannelEdit
	{
	public:
		int Bitrate = 0;
		int UserLimit = 0;

		DiscordVideoQualityMode VideoQualityMode = DiscordVideoQualityMode::AUTO;

		Json Serialize() const override;
	};

	class DiscordVoiceChannel : public DiscordGuildChannel
	{
	public:
		void Edit(const std::function<void(DiscordVoiceChannelEdit*)>& func);

		DiscordVideoQualityMode GetVideoQualityMode() const { return VideoQualityMode; }

		int GetBitrate() const { return Bitrate; }
		int GetUserLimit() const { return UserLimit; }
	private:
		DiscordVideoQualityMode VideoQualityMode = DiscordVideoQualityMode::AUTO;

		int Bitrate = 0;
		int UserLimit = 0;

		DiscordVoiceChannel() {}
		DiscordVoiceChannel(DiscordClient* client, DiscordGuild* guild, const Json& json);

		friend class DiscordGuild;
		friend class DiscordClient;
		friend class DiscordChannel;
	};
}
