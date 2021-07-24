#include "DiscordChannel.hpp"
#include "DiscordGuild.hpp"

namespace Discord
{
	DiscordChannel* DiscordChannel::Create(DiscordClient* client, DiscordGuild* guild, const Json& data)
	{
		DiscordChannel* piece = new DiscordChannel(client, data);
		DiscordChannel* channel = nullptr;

		switch (piece->Type)
		{
			case DiscordChannelType::DM:
			{
				channel = new DiscordDMChannel(client, data);
			} break;
			case DiscordChannelType::GROUP_DM:
			{
				channel = new DiscordGroupDMChannel(client, data);
			} break;
			case DiscordChannelType::GUILD_NEWS:
			{
				channel = new DiscordNewsChannel(client, guild, data);
			} break;
			case DiscordChannelType::GUILD_STORE:
			{
				channel = new DiscordStoreChannel(client, guild, data);
			} break;
			case DiscordChannelType::GUILD_TEXT:
			{
				channel = new DiscordTextChannel(client, guild, data);
			} break;
			case DiscordChannelType::GUILD_VOICE:
			{
				channel = new DiscordVoiceChannel(client, guild, data);
			} break;
			case DiscordChannelType::GUILD_CATEGORY:
			{
				channel = new DiscordChannelCategory(client, guild, data);
			} break;
		}
		delete piece;

		return channel;
	}

	Ptr<DiscordMessage> DiscordChannel::Send(const DiscordEmbed& embed)
	{
		return Send("", false, embed);
	}

	Ptr<DiscordMessage> DiscordChannel::Send(const std::string& msg, bool tts, std::optional<DiscordEmbed> embed)
	{
		Ptr<DiscordMessage> ptr;

		Json json;
		json["tts"] = tts;
		json["content"] = msg;

		if (embed.has_value())
		{
			json["embeds"] = Json::array();
			json["embeds"].emplace_back(embed->Serialize());
		}

		Json result = DiscordRestAPI::Post("/channels/" + Id.GetValueString() + "/messages", json);

		if (!result.is_null())
		{
			ptr.reset(new DiscordMessage(Client, result));
		}

		return ptr;
	}

	Ptr<DiscordMessage> DiscordChannel::SendFile(const std::string& file)
	{
		Ptr<DiscordMessage> ptr;

		if (!std::filesystem::exists(file))
		{
			throw "File" + file + " not found.";
		}

		Json json;
		json["file"] = file;

		auto result = DiscordRestAPI::PostFile("/channels/" + Id.GetValueString() + "/messages", file, json);

		if (!result.is_null())
		{
			ptr.reset(new DiscordMessage(Client, result));
		}

		return ptr;
	}

	void DiscordChannel::PinMessage(Snowflake id)
	{
		DiscordRestAPI::Put("/channels/" + Id.GetValueString() + "/pins/" + id.GetValueString(), {});
	}

	void DiscordChannel::PinMessage(const DiscordMessage& message)
	{
		PinMessage(message.Id);
	}

	void DiscordChannel::EditMessage(Snowflake id, const std::function<void(DiscordMessageEdit*)>& func)
	{
		if (auto ptr = GetMessageById(id))
		{
			EditMessage(ptr.operator*(), func);
		}
	}

	void DiscordChannel::EditMessage(Ptr<DiscordMessage> message, const std::function<void(DiscordMessageEdit*)>& func)
	{
		DiscordMessageEdit edit;
		edit.Flags = message->Flags;
		edit.Embeds = message->Embeds;
		edit.Content = message->Content;

		func(&edit);

		DiscordRestAPI::Patch("/channels/" + Id.GetValueString() + "/messages/" + message->Id.GetValueString(), edit.Serialize(), [&]()
		{
			message->Flags = edit.Flags;
			message->Embeds = edit.Embeds;
			message->Content = edit.Content;
		});
	}

	void DiscordChannel::EditMessage(DiscordMessage& message, const std::function<void(DiscordMessageEdit*)>& func)
	{
		EditMessage(std::make_shared<DiscordMessage>(message), func);
	}

	void DiscordChannel::UnpinMessage(Snowflake id)
	{
		DiscordRestAPI::Delete("/channels/" + Id.GetValueString() + "/pins/" + id.GetValueString());
	}

	void DiscordChannel::UnpinMessage(const Ptr<DiscordMessage> message)
	{
		UnpinMessage(message->Id);
	}

	void DiscordChannel::UnpinMessage(const DiscordMessage& message)
	{
		UnpinMessage(message.Id);
	}

	void DiscordChannel::DeleteMessage(Snowflake id)
	{
		DiscordRestAPI::Delete("/channels/" + Id.GetValueString() + "/messages/" + id.GetValueString());
	}

	void DiscordChannel::DeleteMessage(const Ptr<DiscordMessage> message)
	{
		DeleteMessage(message->Id);
	}

	void DiscordChannel::DeleteMessage(const DiscordMessage& message)
	{
		DeleteMessage(message.Id);
	}

	void DiscordChannel::AddMessageReaction(Snowflake id, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback)
	{
		DiscordRestAPI::Put("/channels/" + Id.GetValueString() + "/messages/" + id.GetValueString() + "/reactions/" + emoji->GetString() + "/@me", {}, asyncCallback);
	}

	void DiscordChannel::AddMessageReaction(Snowflake id, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback)
	{
		AddMessageReaction(id, std::make_shared<DiscordEmoji>(emoji), asyncCallback);
	}

	void DiscordChannel::AddMessageReaction(const Ptr<DiscordMessage> message, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback)
	{
		AddMessageReaction(message->Id, emoji, asyncCallback);
	}

	void DiscordChannel::AddMessageReaction(const Ptr<DiscordMessage> message, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback)
	{
		AddMessageReaction(message->Id, std::make_shared<DiscordEmoji>(emoji), asyncCallback);
	}

	void DiscordChannel::AddMessageReaction(const DiscordMessage& message, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback)
	{
		AddMessageReaction(message.Id, emoji, asyncCallback);
	}

	void DiscordChannel::AddMessageReaction(const DiscordMessage& message, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback)
	{
		AddMessageReaction(message.Id, std::make_shared<DiscordEmoji>(emoji), asyncCallback);
	}

	void DiscordChannel::RemoveOwnMessageReaction(Snowflake id, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback)
	{
		DiscordRestAPI::Delete("/channels/" + Id.GetValueString() + "/messages/" + id.GetValueString() + "/reactions/" + emoji->GetString() + "/@me", asyncCallback);
	}

	void DiscordChannel::RemoveOwnMessageReaction(Snowflake id, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback)
	{
		RemoveOwnMessageReaction(id, std::make_shared<DiscordEmoji>(emoji), asyncCallback);
	}

	void DiscordChannel::RemoveOwnMessageReaction(const Ptr<DiscordMessage> message, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback)
	{
		RemoveOwnMessageReaction(message->Id, emoji, asyncCallback);
	}

	void DiscordChannel::RemoveOwnMessageReaction(const Ptr<DiscordMessage> message, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback)
	{
		RemoveOwnMessageReaction(message->Id, std::make_shared<DiscordEmoji>(emoji), asyncCallback);
	}

	void DiscordChannel::RemoveOwnMessageReaction(const DiscordMessage& message, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback)
	{
		RemoveOwnMessageReaction(message.Id, emoji, asyncCallback);
	}

	void DiscordChannel::RemoveOwnMessageReaction(const DiscordMessage& message, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback)
	{
		RemoveOwnMessageReaction(message.Id, std::make_shared<DiscordEmoji>(emoji), asyncCallback);
	}

	void DiscordChannel::RemoveUserMessageReaction(Snowflake id, const Ptr<DiscordEmoji> emoji, Snowflake userId, const AsyncCallback& asyncCallback)
	{
		DiscordRestAPI::Delete("/channels/" + Id.GetValueString() + "/messages/" + id.GetValueString() + "/reactions/" + emoji->GetString() + "/" + userId.GetValueString(), asyncCallback);
	}

	void DiscordChannel::RemoveUserMessageReaction(Snowflake id, const DiscordEmoji& emoji, Snowflake userId, const AsyncCallback& asyncCallback)
	{
		RemoveUserMessageReaction(id, std::make_shared<DiscordEmoji>(emoji), userId, asyncCallback);
	}

	void DiscordChannel::RemoveUserMessageReaction(const Ptr<DiscordMessage> message, const Ptr<DiscordEmoji> emoji, Snowflake userId, const AsyncCallback& asyncCallback)
	{
		RemoveUserMessageReaction(message->Id, emoji, userId, asyncCallback);
	}

	void DiscordChannel::RemoveUserMessageReaction(const Ptr<DiscordMessage> message, const DiscordEmoji& emoji, Snowflake userId, const AsyncCallback& asyncCallback)
	{
		RemoveUserMessageReaction(message->Id, std::make_shared<DiscordEmoji>(emoji), userId, asyncCallback);
	}

	void DiscordChannel::RemoveUserMessageReaction(const DiscordMessage& message, const Ptr<DiscordEmoji> emoji, Snowflake userId, const AsyncCallback& asyncCallback)
	{
		RemoveUserMessageReaction(message.Id, emoji, userId, asyncCallback);
	}

	void DiscordChannel::RemoveUserMessageReaction(const DiscordMessage& message, const DiscordEmoji& emoji, Snowflake userId, const AsyncCallback& asyncCallback)
	{
		RemoveUserMessageReaction(message.Id, std::make_shared<DiscordEmoji>(emoji), userId, asyncCallback);
	}

	void DiscordChannel::RemoveAllMessageReactions(Snowflake id, const AsyncCallback& asyncCallback)
	{
		DiscordRestAPI::Delete("/channels/" + Id.GetValueString() + "/messages/" + id.GetValueString() + "/reactions/", asyncCallback);
	}

	void DiscordChannel::RemoveAllMessageReactions(const Ptr<DiscordMessage> message, const AsyncCallback& asyncCallback)
	{
		RemoveAllMessageReactions(message->Id, asyncCallback);
	}

	void DiscordChannel::RemoveAllMessageReactions(const DiscordMessage& message, const AsyncCallback& asyncCallback)
	{
		RemoveAllMessageReactions(message.Id, asyncCallback);
	}

	void DiscordChannel::RemoveAllMessageReactionsForEmoji(Snowflake id, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback)
	{
		DiscordRestAPI::Delete("/channels/" + Id.GetValueString() + "/messages/" + id.GetValueString() + "/reactions/" + emoji->GetString(), asyncCallback);
	}

	void DiscordChannel::RemoveAllMessageReactionsForEmoji(Snowflake id, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback)
	{
		RemoveAllMessageReactionsForEmoji(id, std::make_shared<DiscordEmoji>(emoji), asyncCallback);
	}

	void DiscordChannel::RemoveAllMessageReactionsForEmoji(const Ptr<DiscordMessage> message, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback)
	{
		RemoveAllMessageReactionsForEmoji(message->Id, emoji, asyncCallback);
	}

	void DiscordChannel::RemoveAllMessageReactionsForEmoji(const Ptr<DiscordMessage> message, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback)
	{
		RemoveAllMessageReactionsForEmoji(message->Id, std::make_shared<DiscordEmoji>(emoji), asyncCallback);
	}

	void DiscordChannel::RemoveAllMessageReactionsForEmoji(const DiscordMessage& message, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback)
	{
		RemoveAllMessageReactionsForEmoji(message.Id, emoji, asyncCallback);
	}

	void DiscordChannel::RemoveAllMessageReactionsForEmoji(const DiscordMessage& message, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback)
	{
		RemoveAllMessageReactionsForEmoji(message.Id, std::make_shared<DiscordEmoji>(emoji), asyncCallback);
	}

	std::vector<DiscordUser> DiscordChannel::GetMessageReactionUser(Snowflake id, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback)
	{
		std::vector<DiscordUser> result;

		Json json;
		if (DiscordRestAPI::Get("/channels/" + Id.GetValueString() + "/messages/" + id.GetValueString() + "/reactions/" + emoji->GetString(), &json))
		{
			for (const auto& object : json)
			{
				result.emplace_back(DiscordUser(object));
			}
		}

		return result;
	}

	std::vector<DiscordUser> DiscordChannel::GetMessageReactionUser(Snowflake id, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback)
	{
		return GetMessageReactionUser(id, std::make_shared<DiscordEmoji>(emoji), asyncCallback);
	}

	std::vector<DiscordUser> DiscordChannel::GetMessageReactionUser(const Ptr<DiscordMessage> message, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback)
	{
		return GetMessageReactionUser(message->Id, emoji, asyncCallback);
	}

	std::vector<DiscordUser> DiscordChannel::GetMessageReactionUser(const Ptr<DiscordMessage> message, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback)
	{
		return GetMessageReactionUser(message->Id, std::make_shared<DiscordEmoji>(emoji), asyncCallback);
	}

	std::vector<DiscordUser> DiscordChannel::GetMessageReactionUser(const DiscordMessage& message, const Ptr<DiscordEmoji> emoji, const AsyncCallback& asyncCallback)
	{
		return GetMessageReactionUser(message.Id, emoji, asyncCallback);
	}

	std::vector<DiscordUser> DiscordChannel::GetMessageReactionUser(const DiscordMessage& message, const DiscordEmoji& emoji, const AsyncCallback& asyncCallback)
	{
		return GetMessageReactionUser(message.Id, std::make_shared<DiscordEmoji>(emoji), asyncCallback);
	}

	void DiscordChannel::PinMessage(const Ptr<DiscordMessage> message)
	{
		PinMessage(message->Id);
	}

	void DiscordChannel::DeleteOrClose()
	{
		DiscordRestAPI::Delete("/channels/" + Id.GetValueString());
	}

	void DiscordChannel::DeleteMessages(const std::vector<Snowflake>& ids)
	{
		if (ids.empty())
		{
			return;
		}
		else if (ids.size() == 1)
		{
			if (auto message = GetMessageById(ids[0]))
				DeleteMessage(message.operator*());
		}
		else
		{
			Json json;
			json["messages"] = Json::array();

			for (const auto& id : ids)
				json["messages"].emplace_back(id.GetValueString());

			DiscordRestAPI::Post("/channels/{channel.id}/messages/bulk-delete", json);
		}
	}

	void DiscordChannel::TriggerTypingIndicator()
	{
		DiscordRestAPI::Post("/channels/" + Id.GetValueString() + "/typing", {});
	}

	std::vector<DiscordInvite> DiscordChannel::GetInvites()
	{
		std::vector<DiscordInvite> result;

		Json json;
		if (DiscordRestAPI::Get("/channels/" + Id.GetValueString() + "/invites", &json))
		{
			for (const auto& object : json)
				result.emplace_back(DiscordInvite(object));
		}

		return result;
	}

	std::vector<DiscordMessage> DiscordChannel::GetMessages(int limit, std::optional<Snowflake> id, std::optional<MessageDirection> direction)
	{
		std::string url = "/channels/" + Id.GetValueString() + "/messages?limit=" + std::to_string(limit);

		if (id.has_value() && direction.has_value())
		{
			std::string dir;
			switch (direction.value())
			{
				case MessageDirection::AFTER: dir = "after"; break;
				case MessageDirection::BEFORE: dir = "before"; break;
				case MessageDirection::AROUND: dir = "around"; break;
			}
			url += "&" + dir + "=" + id.value().GetValueString();
		}

		std::vector<DiscordMessage> messages;

		Json json;
		if (DiscordRestAPI::Get(url, &json))
		{
			for (const auto& object : json)
			{
				messages.emplace_back(DiscordMessage(Client, object));
			}
		}

		return messages;
	}

	std::vector<DiscordMessage> DiscordChannel::GetPinnedMessages()
	{
		std::vector<DiscordMessage> result;

		Json json;
		if (DiscordRestAPI::Get("/channels/" + Id.GetValueString() + "/pins", &json))
		{
			for (const auto& object : json)
				result.emplace_back(DiscordMessage(Client, object));
		}

		return result;
	}

	Ptr<DiscordMessage> DiscordChannel::GetMessageById(Snowflake id)
	{
		Ptr<DiscordMessage> ptr;

		Json json;
		if (DiscordRestAPI::Get("/channels/" + Id.GetValueString() + "/messages/" + id.GetValueString(), &json))
		{
			ptr.reset(new DiscordMessage(Client, json));
		}

		return ptr;
	}

	DiscordChannel::DiscordChannel(DiscordClient* client, const Json& json) : Client(client)
	{
		if (CHECK("id"))
			Id = json["id"].get<std::string>();

		if (CHECK("type"))
			Type = static_cast<DiscordChannelType>(json["type"].get<int>());
	}

	DiscordChannelMention::DiscordChannelMention(const Json& json)
	{
		if (CHECK("id"))
			Id = json["id"].get<std::string>();

		if (CHECK("guild_id"))
			GuildId = json["guild_id"].get<std::string>();

		if (CHECK("type"))
			Type = static_cast<DiscordChannelType>(json["type"].get<int>());

		if (CHECK("name"))
			Name = json["name"].get<std::string>();
	}

	Json DiscordGuildChannelEdit::Serialize() const
	{
		Json json;
		json["name"] = Name;
		json["position"] = Position;

		auto permissions = Json::array();

		for (const auto& permission : Permissions)
		{
			permissions.emplace_back(permission.second.Serialize());
		}

		json["permission_overwrites"] = permissions;

		return json;
	}

	Json DiscordChannelCreate::Serialize() const
	{
		Json json;
		json["name"] = Name;

		if (Type.has_value())
			json["type"] = static_cast<DiscordChannelType>(Type.value());

		if (Topic.has_value())
			json["topic"] = Topic.value();

		if (Bitrate.has_value())
			json["bitrate"] = Bitrate.value();

		if (UserLimit.has_value())
			json["user_limit"] = UserLimit.value();

		if (RateLimitPerUser.has_value())
			json["rate_limit_per_user"] = RateLimitPerUser.value();

		if (Position.has_value())
			json["position"] = Position.value();

		if (Permissions.has_value())
		{
			json["permission_overwrites"] = Json::array();

			for (const auto& permission : Permissions.value())
				json["permission_overwrites"].emplace_back(permission.second.Serialize());
		}

		if (ParentId.has_value())
			json["parent_id"] = ParentId.value().GetValueString();

		if (IsNSFW.has_value())
			json["nsfw"] = IsNSFW.value();

		return json;
	}
}