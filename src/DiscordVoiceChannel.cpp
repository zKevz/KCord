#include "DiscordVoiceChannel.hpp"

namespace Discord
{
	void DiscordVoiceChannel::Edit(const std::function<void(DiscordVoiceChannelEdit*)>& func)
	{
		DiscordVoiceChannelEdit edit;
		edit.Bitrate = Bitrate;
		edit.Name = Name;
		edit.Permissions = Permissions;
		edit.Position = Position;
		edit.UserLimit = UserLimit;
		edit.VideoQualityMode = VideoQualityMode;

		func(&edit);

		DiscordRestAPI::Patch("/channels/" + Id.GetValueString(), edit.Serialize());
	}

	DiscordVoiceChannel::DiscordVoiceChannel(DiscordClient* client, DiscordGuild* guild, const Json& json) : DiscordGuildChannel(client, guild, json)
	{
		if (CHECK("bitrate"))
			Bitrate = json["bitrate"].get<int>();

		if (CHECK("user_limit"))
			UserLimit = json["user_limit"].get<int>();

		if (CHECK("video_quality_mode"))
			VideoQualityMode = static_cast<DiscordVideoQualityMode>(json["video_quality_mode"].get<int>());
	}

	Json DiscordVoiceChannelEdit::Serialize() const
	{
		Json json = DiscordGuildChannelEdit::Serialize();
		json["birate"] = Bitrate;
		json["user_limit"] = UserLimit;
		json["video_quality_mode"] = static_cast<int>(VideoQualityMode);
		return json;
	}
}