#include "DiscordMessage.hpp"
#include "DiscordClient.hpp"

namespace Discord
{
	bool DiscordMessage::HasPrefix(const char& prefix, bool caseSensitive)
	{
		if (Content.empty())
			return false;

		if (caseSensitive)
			return Content[0] == prefix;
		else
			return std::toupper(Content[0]) == std::toupper(prefix);
	}

	bool DiscordMessage::HasPrefix(const std::string& prefix, bool caseSensitive)
	{
		if (Content.empty())
			return false;

		if (Content.size() < prefix.size())
			return false;

		if (caseSensitive)
			return Content.find(prefix) == 0;
		else
		{
			for (size_t i = 0; i < prefix.size(); ++i)
			{
				if (std::toupper(Content[i]) != std::toupper(prefix[i]))
					return false;
			}
			return true;
		}
	}

	DiscordMessage::DiscordMessage(DiscordClient* client, const Json& json)
	{
		if (CHECK("id"))
			Id = json["id"].get<std::string>();

		if (CHECK("guild_id"))
		{
			Guild = client->GetGuildById(json["guild_id"].get<std::string>());
		}
		
		if (CHECK("channel_id"))
		{
			if (Guild)
			{
				Channel = Guild->GetChannelById(Snowflake(json["channel_id"].get<std::string>()));
			}
			if (!Channel)
			{
				Json channelJson;
				if (DiscordRestAPI::Get("/channels/" + json["channel_id"].get<std::string>(), &channelJson))
				{
					Channel = Ptr<DiscordChannel>(DiscordChannel::Create(client, Guild.get(), channelJson));
				}
			}
		}

		if (CHECK("author"))
			Author = Ptr<DiscordUser>(new DiscordUser(json["author"]));

		if (CHECK("content"))
			Content = json["content"].get<std::string>();

		if (CHECK("timestamp"))
			Timestamp = json["timestamp"].get<std::string>();

		if (CHECK("edited_timestamp"))
			EditedTimestamp = json["edited_timestamp"].get<std::string>();

		if (CHECK("tts"))
			bIsTTS = json["tts"].get<bool>();

		if (CHECK("mention_everyone"))
			bMentionEveryone = json["mention_everyone"].get<bool>();

		if (CHECK("mentions"))
		{
			const auto& mentions = json["mentions"];
			for (const auto& object : mentions)
			{
				DiscordUser user = DiscordUser(object);
				Mentions.emplace_back(user);
			}
		}

		if (CHECK("mention_roles"))
		{
			const auto& mentionRoles = json["mention_roles"];
			for (const auto& object : mentionRoles)
			{
				DiscordRole role = DiscordRole(object);
				MentionRoles.emplace_back(role);
			}
		}

		if (CHECK("mention_channels"))
		{
			const auto& mentionChannels = json["mention_channels"];
			for (const auto& object : mentionChannels)
			{
				DiscordChannelMention mention = DiscordChannelMention(object);
				MentionChannels.emplace_back(mention);
			}
		}

		if (CHECK("attachments"))
		{
			const auto& attachments = json["attachments"];
			for (const auto& object : attachments)
			{
				DiscordAttachment attachment = DiscordAttachment(object);
				Attachments.emplace_back(attachment);
			}
		}

		if (CHECK("embeds"))
		{
			const auto& embeds = json["embeds"];
			for (const auto& object : embeds)
			{
				DiscordEmbed embed = DiscordEmbed(object);
				Embeds.emplace_back(embed);
			}
		}

		if (CHECK("reactions"))
		{
			const auto& reactions = json["reactions"];
			for (const auto& object : reactions)
			{
				DiscordMessageReaction reaction = DiscordMessageReaction(object);
				Reactions.emplace_back(reaction);
			}
		}

		if (CHECK("pinned"))
			bIsPinned = json["pinned"].get<bool>();

		if (CHECK("webhook_id"))
			WebhookId = json["webhook_id"].get<std::string>();

		if (CHECK("type"))
			Type = static_cast<DiscordMessageType>(json["type"].get<int>());

		if (CHECK("activity"))
			DiscordActivity = DiscordMessageActivity(json["activity"]);

		if (CHECK("application_id"))
			ApplicationId = json["application_id"].get<std::string>();

		if (CHECK("message_reference"))
			MessageReference = DiscordMessageReference(json["message_reference"]);

		if (CHECK("flags"))
			Flags = json["flags"].get<int>();
	}

	DiscordAttachment::DiscordAttachment(const Json& json)
	{
		if (CHECK("id"))
			Id = json["id"].get<std::string>();

		if (CHECK("filename"))
			FileName = json["filename"].get<std::string>();

		if (CHECK("content_type"))
			ContentType = json["content_type"].get<std::string>();

		if (CHECK("size"))
			Size = json["size"].get<int>();

		if (CHECK("url"))
			Url = json["url"].get<std::string>();

		if (CHECK("proxy_url"))
			ProxyUrl = json["proxy_url"].get<std::string>();

		if (CHECK("height"))
			Height = json["height"].get<int>();

		if (CHECK("width"))
			Width = json["width"].get<int>();
	}

	DiscordMessageReaction::DiscordMessageReaction(const Json& json)
	{
		if (CHECK("count"))
			Count = json["count"].get<int>();
		
		if (CHECK("me"))
			Me = json["me"].get<bool>();

		if (CHECK("emoji"))
			Emoji = DiscordEmoji(json["emoji"]);
	}

	DiscordMessageActivity::DiscordMessageActivity(const Json& json)
	{
		if (CHECK("type"))
			Type = static_cast<DiscordMessageActivityType>(json["type"].get<int>());

		if (CHECK("party_id"))
			PartyId = json["party_id"].get<std::string>();
	}

	DiscordMessageReference::DiscordMessageReference(const Json& json)
	{
		if (CHECK("message_id"))
			MessageId = json["message_id"].get<std::string>();

		if (CHECK("channel_id"))
			ChannelId = json["channel_id"].get<std::string>();

		if (CHECK("guild_id"))
			GuildId = json["guild_id"].get<std::string>();

		if (CHECK("fail_if_not_exists"))
			bFailIfNotExist = json["fail_if_not_exists"].get<bool>();
	}

	Json DiscordMessageEdit::Serialize() const
	{
		Json json;
		json["flags"] = Flags;
		json["embeds"] = Json::array();

		for (const auto& embed : Embeds)
		{
			json["embeds"].emplace_back(embed.Serialize());
		}
		
		json["content"] = Content;
		return json;
	}
}