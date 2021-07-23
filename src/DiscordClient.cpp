#include "DiscordClient.hpp"

namespace Discord
{
	void DiscordClient::OnDefaultHelpCommand(const Discord::DiscordCommandContext& context)
	{
		DiscordEmbed embed;
		embed.SetAuthor("Help", "", GetAvatarUrl())
			.SetFooter("KCord")
			.PutTimestamp()
			.PutRandomColor();

		for (const auto& command : CommandService.Commands)
		{
			std::string str = "*No description provided*";

			if (command.second.Description.has_value())
			{
				str = "Description: " + command.second.Description.value();
			}

			embed.AddField(command.first, str);
		}

		context.GetChannel()->Send(embed);
	}

	DiscordClient::DiscordClient(const DiscordClientConfig& config) : Config(config)
	{
		std::cout << "KCord, Discord API (c), made by kevz#2211\n";

		DiscordRestAPI::Token = config.Token;

		WebSocket = new DiscordWebSocket();
		WebSocket->Initialize();
		WebSocket->SetFailedHandler([&]() { Reconnect(); });
		WebSocket->SetClosedHandler([&](short code, const std::string& reason) { OnWebsocketClosed(code, reason); });
		WebSocket->SetMessageHandler([&](DiscordWebSocket::WebSocketMessage msg)
		{
#ifdef _DEBUG
			auto start = std::chrono::high_resolution_clock::now();
#endif
			OnMessageReceived(msg->get_payload());
#ifdef _DEBUG
			auto end = std::chrono::high_resolution_clock::now();
			std::cout << "Handling message took " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "\n";
#endif
		});

		RegisterCommand("help", [&](const Discord::DiscordCommandContext& context) { OnDefaultHelpCommand(context); });
	}

	DiscordClient::~DiscordClient()
	{
		if (WebSocket)
		{
			delete WebSocket;
		}
	}

	void DiscordClient::Run()
	{
		WebSocket->Run();
	}

	void DiscordClient::Edit(const std::function<void(DiscordClientEdit*)>& func)
	{
		DiscordClientEdit edit;
		edit.Username = Username;

		func(&edit);

		Json json = DiscordRestAPI::Patch("/users/@me", edit.Serialize(), [&]()
		{
			Username = edit.Username;
		});

		if (!json.is_null())
		{
			*static_cast<DiscordUser*>(this) = DiscordUser(json);
		}
	}

	void DiscordClient::LeaveGuild(Snowflake id)
	{
		DiscordRestAPI::Delete("/users/@me/guilds/" + id.GetValueString());
	}

	void DiscordClient::LeaveGuild(const DiscordGuild& guild)
	{
		LeaveGuild(guild.Id);
	}

	void DiscordClient::LeaveGuild(const Ptr<DiscordGuild> guild)
	{
		LeaveGuild(guild->Id);
	}

	void DiscordClient::DeleteGuild(Snowflake id)
	{
		if (auto iterator = Guilds.find(id); iterator != Guilds.end())
		{
			DiscordRestAPI::Delete("/guilds/" + id.GetValueString());
		}
	}

	void DiscordClient::DeleteGuild(const Ptr<DiscordGuild> guild)
	{
		DeleteGuild(guild->Id);
	}

	void DiscordClient::DeleteGuild(const DiscordGuild& guild)
	{
		DeleteGuild(guild.Id);
	}

	void DiscordClient::InvokeCommand(Ptr<DiscordMessage> message)
	{
		if (!Config.EnableDM)
		{
			if (std::dynamic_pointer_cast<DiscordDMChannel>(message->Channel) ||
				std::dynamic_pointer_cast<DiscordGroupDMChannel>(message->Channel))
			{
				return;
			}
		}

		if (message->HasPrefix(Config.Prefix, Config.PrefixCaseSensitive))
		{
			Ptr<DiscordCommandService::DiscordCommandServiceContainer> ptr(new DiscordCommandService::DiscordCommandServiceContainer);
			ptr->Client = this;
			ptr->Message = message;
			ptr->Content = message->Content.substr(Config.Prefix.size());

			CommandService.Process(ptr);
		}
	}

	void DiscordClient::RegisterCommand(const std::string& str, const DiscordCommandCallback& callback)
	{
		RegisterCommand(str, callback, {});
	}

	void DiscordClient::RegisterCommand(const std::string& str, const DiscordCommandCallback& callback, uint64_t permissionFlags)
	{
		DiscordCommandInfo info;
		info.RequirePermission = permissionFlags;
		RegisterCommand(str, callback, info);
	}

	void DiscordClient::RegisterCommand(const std::string& string, const DiscordCommandCallback& callback, std::optional<DiscordCommandInfo> info)
	{
		DiscordCommandInfo cmd;

		if (info.has_value())
			cmd = info.value();

		cmd.Callback = callback;

		CommandService.Add(string, cmd);
	}

	void DiscordClient::RegisterCommand(const std::string& str, const DiscordCommandCallback& callback, std::vector<std::string> roles, DiscordRoleTargetType type)
	{
		DiscordCommandInfo info;
		info.RoleTarget = type;
		info.RequireRoles = roles;
		RegisterCommand(str, callback, info);
	}

	Ptr<DiscordUser> DiscordClient::GetUserById(Snowflake id)
	{
		Ptr<DiscordUser> ptr;

		Json json;
		if (DiscordRestAPI::Get("/users/" + id.GetValueString(), &json))
		{
			ptr.reset(new DiscordUser(json));
		}

		return ptr;
	}

	Ptr<DiscordGuild> DiscordClient::GetGuildById(Snowflake id)
	{
		Ptr<DiscordGuild> ptr;

		if (auto iterator = Guilds.find(id); iterator != Guilds.end())
		{
			return iterator->second;
		}
		else
		{
			Json json;
			if (DiscordRestAPI::Get("/guilds/" + id.GetValueString(), &json))
			{
				ptr.reset(new DiscordGuild(const_cast<DiscordClient*>(this), json));
				Guilds.insert_or_assign(ptr->Id, ptr);
			}
		}

		return ptr;
	}

	Ptr<DiscordDMChannel> DiscordClient::GetOrCreateDMChannel(Snowflake id)
	{
		if (auto iterator = DMChannels.find(id); iterator != DMChannels.end())
		{
			return iterator->second;
		}
		else
		{
			Json json;
			json["recipient_id"] = id.GetValueString();
			Json result = DiscordRestAPI::Post("/users/@me/channels", json);

			if (result != Json {})
			{
				return Ptr<DiscordDMChannel>(new DiscordDMChannel(this, result));
			}
		}
		return nullptr;
	}

	Ptr<DiscordDMChannel> DiscordClient::GetOrCreateDMChannel(const DiscordUser& user)
	{
		return Ptr<DiscordDMChannel>();
	}

	Ptr<DiscordDMChannel> DiscordClient::GetOrCreateDMChannel(const Ptr<DiscordUser> user)
	{
		return Ptr<DiscordDMChannel>();
	}

	void DiscordClient::Send(const std::string& msg)
	{
		WebSocket->Send(msg);
	}

	void DiscordClient::SendIdentify()
	{
#if defined(_WIN32) || defined(_WIN64)
		std::string os = "Windows";
#elif defined(linux) || defined(__linux) || defined(__linux__)
		std::string os = "Linux";
#elif defined(unix) || defined(__unix) || defined(__unix__)
		std::string os = "Unix";
#elif defined(__APPLE__) || defined(__MACH__)
		std::string os = "macOS";
#elif defined __FreeBSD__
		std::string os = "FreeBSD";
#else
#error Unsupported OS
#endif

		Json json =
		{
			{ "op", 2 },
			{ "d", 
				{
					{ "token", Config.Token },
					{ "properties", 
						{
							{ "$os", os },
							{ "$browser", "KCord" },
							{ "$device", "KCord" }
						}
					},
					{ "compress", false },
					{ "large_threshold", Config.Threshold },
					{ "intents", Config.Intents }
				}
			}
		};

		if (Presence.has_value())
		{
			json["d"]["presence"] = Presence->Serialize();
		}

		Send(json.dump());
	}

	void DiscordClient::SendHeartbeat()
	{
		bHeartbeatFailed = true;

		Json json =
		{
			{ "op", 1 },
			{ "d", LastSequenceNumber }
		};

		Send(json.dump());
	}

	void DiscordClient::StartHeartbeating()
	{
		std::thread t([&]()
		{
			while (true)
			{
				if (HeartbeatInterval <= 0)
					break;
				if (DiscordClock::now() - LastHeartbeatSent >= std::chrono::milliseconds(HeartbeatInterval))
				{
					if (bHeartbeatFailed)
					{
						break;
					}
					LastHeartbeatSent = DiscordClock::now();
					SendHeartbeat();
				}
			}
		});
		t.detach();
	}

	void DiscordClient::OnWebsocketClosed(short code, const std::string& reason)
	{
		std::cout << "Websocket closed! Reason: " << reason << "\n";

		switch (static_cast<DiscordWebSocketCloseCode>(code))
		{
			case DiscordWebSocketCloseCode::ALREADY_AUTHENTICATED:
			{
				throw "Already authenticated! This is API bugs, please report this.";
			} break;
			case DiscordWebSocketCloseCode::AUTHENTICATION_FAILED:
			{
				throw "Invalid token";
			} break;
			case DiscordWebSocketCloseCode::DECODE_ERROR:
			{
				throw "Invalid payload! This is API bugs, please report this.";
			} break;
			case DiscordWebSocketCloseCode::DISALLOWED_INTENTS:
			{
				throw "Disallowed intents! You may have tried to specify an intent that you have not enabled or are not approved for.";
			} break;
			case DiscordWebSocketCloseCode::INVALID_API_VERSION:
			{
				throw "Invalid API Version! This is API bugs, please report this.";
			} break;
			case DiscordWebSocketCloseCode::INVALID_INTENTS:
			{
				throw "Invalid intents.";
			} break;
			case DiscordWebSocketCloseCode::INVALID_SHARD:
			{
				throw "Invalid shard.";
			} break;
			case DiscordWebSocketCloseCode::NOT_AUTHENTICATED:
			{
				throw "Not authenticated! This is API bugs, please report this.";
			} break;
			default:
			{
				Reconnect();
			} break;
		}
	}

	void DiscordClient::OnDispatch(const Json& json, const Json& data)
	{
		auto sequence = json["s"];

		if (!sequence.is_null())
			LastSequenceNumber = sequence.get<int>();

		auto t = json["t"];

		if (!t.is_null())
		{
			std::string type = t.get<std::string>();

			if (type == "READY")
			{
				SessionId = data["session_id"].get<std::string>();

				Json json;
				if (DiscordRestAPI::Get("/users/@me", &json))
				{
					*static_cast<DiscordUser*>(this) = DiscordUser(json);

					if (ReadyCallback)
						ReadyCallback();
				}
				else
				{
					throw "Can't get the client current user";
				}
			}
			else if (type == "CHANNEL_CREATE")
			{
				DiscordChannel* channel = DiscordChannel::Create(this, nullptr, data);

				if (!channel)
					return;

				Ptr<DiscordChannel> ptr(channel);

				if (auto gc = dynamic_cast<DiscordGuildChannel*>(channel))
				{
					Ptr<DiscordGuild> guild = GetGuildById(gc->GuildId);

					if (!guild)
					{
						return;
					}

					gc->Guild = guild.get();

					guild->Channels.insert_or_assign(channel->Id, ptr);

					if (ChannelCreatedCallback)
					{
						DiscordChannelEventInfo info;
						info.Guild = guild;
						info.Client = this;
						info.Channel = ptr;
						ChannelCreatedCallback(info);
					}
				}
				else
				{
					if (ChannelCreatedCallback)
					{
						DiscordChannelEventInfo info;
						info.Guild = nullptr;
						info.Client = this;
						info.Channel = ptr;
						ChannelCreatedCallback(info);
					}
				}
			}
			else if (type == "CHANNEL_UPDATE")
			{
				DiscordChannel* channel = DiscordChannel::Create(this, nullptr, data);
				
				if (!channel)
					return;

				if (auto gc = dynamic_cast<DiscordGuildChannel*>(channel))
				{
					Ptr<DiscordGuild> guild = GetGuildById(gc->GuildId);

					if (!guild)
					{
						return;
					}

					gc->Guild = guild.get();

					if (auto iterator = guild->Channels.find(channel->Id); iterator != guild->Channels.end())
					{
						iterator->second.reset(channel);

						if (ChannelUpdatedCallback)
						{
							DiscordChannelEventInfo info;
							info.Guild = guild;
							info.Client = this;
							info.Channel = iterator->second;
							ChannelUpdatedCallback(info);
						}
					}
				}
				else
				{
					if (ChannelUpdatedCallback)
					{
						DiscordChannelEventInfo info;
						info.Guild = nullptr;
						info.Client = this;
						info.Channel = Ptr<DiscordChannel>(channel);
						ChannelUpdatedCallback(info);
					}
					delete channel;
				}
			}
			else if (type == "CHANNEL_DELETE")
			{
				if (CHECK_M(data, "id") && CHECK_M(data, "guild_id"))
				{
					DiscordChannel* channel = DiscordChannel::Create(this, nullptr, data);

					if (!channel)
						return;

					if (auto gc = dynamic_cast<DiscordGuildChannel*>(channel))
					{
						Ptr<DiscordGuild> guild = GetGuildById(gc->GuildId);

						if (guild)
						{
							if (auto iterator = guild->Channels.find(gc->Id); iterator != guild->Channels.end())
							{
								if (ChannelRemovedCallback)
								{
									DiscordChannelEventInfo info;
									info.Guild = guild;
									info.Client = this;
									info.Channel = iterator->second;
									ChannelRemovedCallback(info);
								}

								guild->Channels.erase(iterator);
							}
						}
					}
					else
					{
						if (ChannelRemovedCallback)
						{
							DiscordChannelEventInfo info;
							info.Guild = nullptr;
							info.Client = this;
							info.Channel = Ptr<DiscordChannel>(channel);
							ChannelRemovedCallback(info);
						}
					}

					delete channel;
				}
			}
			else if (type == "GUILD_CREATE")
			{
				Ptr<DiscordGuild> guild(new DiscordGuild(this, data));
				Guilds.insert_or_assign(guild->Id, guild);

				if (GuildCreatedCallback)
				{
					DiscordEventInfo info;
					info.Guild = guild;
					info.Client = this;
					GuildCreatedCallback(info);
				}
			}
			else if (type == "GUILD_UPDATE")
			{
				DiscordGuild* guild = new DiscordGuild(this, data);

				if (auto iterator = Guilds.find(guild->Id); iterator != Guilds.end())
				{
					iterator->second.reset(guild);

					if (GuildUpdatedCallback)
					{
						DiscordEventInfo info;
						info.Guild = iterator->second;
						info.Client = this;
						GuildUpdatedCallback(info);
					}
				}
			}
			else if (type == "GUILD_DELETE")
			{
				bool unavailable = data["unavailable"];

				if (!unavailable)
				{
					Snowflake guildId = data["id"].get<std::string>();

					if (auto iterator = Guilds.find(guildId); iterator != Guilds.end())
					{
						if (GuildRemovedCallback)
						{
							DiscordEventInfo info;
							info.Guild = iterator->second;
							info.Client = this;
							GuildRemovedCallback(info);
						}

						Guilds.erase(iterator);
					}
				}
			}
			else if (type == "GUILD_BAN_ADD")
			{
				if (BanAddedCallback)
				{
					Ptr<DiscordGuild> guild = GetGuildById(data["guild_id"].get<std::string>());

					if (!guild)
						return;

					DiscordUserEventInfo ban;
					ban.User = Ptr<DiscordUser>(new DiscordUser(data["user"]));
					ban.Guild = guild;
					ban.Client = this;
					BanAddedCallback(ban);
				}
			}
			else if (type == "GUILD_BAN_REMOVE")
			{
				if (BanRemovedCallback)
				{
					Ptr<DiscordGuild> guild = GetGuildById(data["guild_id"].get<std::string>());

					if (!guild)
						return;

					DiscordUserEventInfo ban;
					ban.User = Ptr<DiscordUser>(new DiscordUser(data["user"]));
					ban.Guild = guild;
					ban.Client = this;
					BanRemovedCallback(ban);
				}
			}
			else if (type == "GUILD_EMOJIS_UPDATE")
			{
				Ptr<DiscordGuild> guild = GetGuildById(data["guild_id"].get<std::string>());

				if (!guild)
					return;
				
				const auto& emojis = data["emojis"];
				for (const auto& object : emojis)
				{
					Ptr<DiscordEmoji> emoji(new DiscordEmoji(object));
					guild->Emojis.insert_or_assign(emoji->Id, emoji);
				}
				
				if (EmojiUpdatedCallback)
				{
					DiscordEventInfo info;
					info.Guild = guild;
					info.Client = this;
					EmojiUpdatedCallback(info);
				}
			}
			else if (type == "GUILD_MEMBER_ADD")
			{
				Ptr<DiscordGuild> guild = GetGuildById(data["guild_id"].get<std::string>());

				if (!guild)
					return;

				Ptr<DiscordGuildMember> member(new DiscordGuildMember(data));

				guild->Members.insert({ member->Id, member });

				if (MemberAddedCallback)
				{
					DiscordMemberEventInfo info;
					info.Guild = guild;
					info.Client = this;
					info.Member = member;
					MemberAddedCallback(info);
				}
			}
			else if (type == "GUILD_MEMBER_REMOVE")
			{
				Ptr<DiscordGuild> guild = GetGuildById(data["guild_id"].get<std::string>());

				if (!guild)
					return;

				Ptr<DiscordUser> user(new DiscordUser(data["user"]));

				if (auto iterator = guild->Members.find(user->Id); iterator != guild->Members.end())
				{
					if (MemberRemovedCallback)
					{
						DiscordUserEventInfo info;
						info.User = user;
						info.Guild = guild;
						info.Client = this;
						MemberRemovedCallback(info);
					}

					guild->Members.erase(iterator);
				}
			}
			else if (type == "GUILD_MEMBER_UPDATE")
			{
				Ptr<DiscordGuild> guild = GetGuildById(data["guild_id"].get<std::string>());

				if (!guild)
					return;

				DiscordGuildMember* member = new DiscordGuildMember(data);

				if (auto iterator = guild->Members.find(member->Id); iterator != guild->Members.end())
				{
					iterator->second.reset(member);

					if (MemberUpdatedCallback)
					{
						DiscordMemberEventInfo info;
						info.Guild = guild;
						info.Client = this;
						info.Member = iterator->second;
						MemberUpdatedCallback(info);
					}
				}
				else delete member;
			}
			else if (type == "GUILD_MEMBERS_CHUNK")
			{
				Ptr<DiscordGuild> guild = GetGuildById(data["guild_id"].get<std::string>());

				if (!guild)
					return;

				const auto& members = data["members"];
				for (const auto& object : members)
				{
					Ptr<DiscordGuildMember> member(new DiscordGuildMember(object));
					guild->Members.insert_or_assign(member->Id, member);
				}

				if (CHECK_M(data, "presences"))
				{
					const auto& presences = data["presences"];
					for (const auto& object : presences)
					{
						Ptr<DiscordPresence> presence(new DiscordPresence(object));
						guild->Presences.insert_or_assign(presence->User.Id, presence);
					}
				}

				if (GuildMembersChunkCallback)
				{
					DiscordEventInfo info;
					info.Guild = guild;
					info.Client = this;
					GuildMembersChunkCallback(info);
				}
			}
			else if (type == "GUILD_ROLE_CREATE")
			{
				Ptr<DiscordGuild> guild = GetGuildById(data["guild_id"].get<std::string>());

				if (!guild)
					return;

				Ptr<DiscordRole> role(new DiscordRole(data["role"]));
				guild->Roles.insert({ role->Id, role });
				
				if (RoleAddedCallback)
				{
					DiscordRoleEventInfo info;
					info.Role = role;
					info.Guild = guild;
					info.Client = this;
					RoleAddedCallback(info);
				}
			}
			else if (type == "GUILD_ROLE_DELETE")
			{
				Ptr<DiscordGuild> guild = GetGuildById(data["guild_id"].get<std::string>());

				if (!guild)
					return;

				Snowflake roleId = data["role_id"].get<std::string>();

				if (auto iterator = guild->Roles.find(roleId); iterator != guild->Roles.end())
				{
					if (RoleRemovedCallback)
					{
						DiscordRoleEventInfo info;
						info.Role = iterator->second;
						info.Guild = guild;
						info.Client = this;
						RoleRemovedCallback(info);
					}
					guild->Roles.erase(iterator);
				}
			}
			else if (type == "GUILD_ROLE_UPDATE")
			{
				Ptr<DiscordGuild> guild = GetGuildById(data["guild_id"].get<std::string>());

				if (!guild)
					return;

				DiscordRole* role = new DiscordRole(data["role"]);

				if (auto iterator = guild->Roles.find(role->Id); iterator != guild->Roles.end())
				{
					iterator->second.reset(role);

					if (RoleUpdatedCallback)
					{
						DiscordRoleEventInfo info;
						info.Role = iterator->second;
						info.Guild = guild;
						info.Client = this;
						RoleUpdatedCallback(info);
					}
				}
				else delete role;
			}
			else if (type == "INVITE_CREATE")
			{
				Ptr<DiscordGuild> guild = GetGuildById(data["guild_id"].get<std::string>());

				if (!guild)
					return;

				Ptr<DiscordChannel> channel = guild->GetChannelById(data["channel_id"].get<std::string>());

				if (!channel)
					return;

				DiscordInviteCreated invite;
				invite.Guild = guild;
				invite.Client = this;
				invite.Channel = channel;

				if (CHECK_M(data, "code"))
					invite.Code = data["code"].get<std::string>();

				if (CHECK_M(data, "created_at"))
					invite.CreatedAt = data["created_at"].get<std::string>();

				if (CHECK_M(data, "inviter"))
					invite.Inviter = DiscordUser(data["inviter"]);
				
				if (CHECK_M(data, "max_age"))
					invite.MaxAge = data["max_age"].get<int>();

				if (CHECK_M(data, "max_uses"))
					invite.MaxUses = data["max_uses"].get<int>();

				if (CHECK_M(data, "target_type"))
					invite.TargetType = static_cast<DiscordInviteTargetType>(data["target_type"].get<int>());

				if (CHECK_M(data, "temporary"))
					invite.IsTemporary = data["temporary"].get<bool>();

				if (CHECK_M(data, "uses"))
					invite.Uses = data["uses"].get<int>();

				if (InviteCreatedCallback)
				{
					InviteCreatedCallback(invite);
				}
			}
			else if (type == "INVITE_DELETE")
			{
				if (InviteDeletedCallback)
				{
					Ptr<DiscordGuild> guild = GetGuildById(data["guild_id"].get<std::string>());

					if (!guild)
						return;

					Ptr<DiscordChannel> channel = guild->GetChannelById(data["channel_id"].get<std::string>());

					if (!channel)
						return;

					DiscordInviteDeletedEventInfo invite;
					invite.Guild = guild;
					invite.Client = this;
					invite.Channel = channel;

					if (CHECK_M(data, "code"))
						invite.Code = data["code"].get<std::string>();

					InviteDeletedCallback(invite);
				}
			}
			else if (type == "MESSAGE_CREATE")
			{
				if (MessageCreatedCallback)
				{
					Ptr<DiscordMessage> message(new DiscordMessage(this, data));

					if (!message->Channel)
					{
						return;
					}

					DiscordMessageEventInfo info;
					info.Guild = message->Guild;
					info.Client = this;
					info.Message = message;

					if (info.Guild)
					{
						info.Member = info.Guild->GetMemberById(message->Author->Id);
					}

					MessageCreatedCallback(info);
				}
			}
			else if (type == "MESSAGE_UPDATE")
			{
				if (MessageUpdatedCallback)
				{
					Ptr<DiscordMessage> message(new DiscordMessage(this, data));

					if (!message->Channel)
					{
						return;
					}

					DiscordMessageEventInfo info;
					info.Guild = message->Guild;
					info.Client = this;
					info.Message = message;

					if (info.Guild)
					{
						info.Member = info.Guild->GetMemberById(message->Author->Id);
					}

					MessageUpdatedCallback(info);
				}
			}
			else if (type == "MESSAGE_REACTION_ADD")
			{
				if (ReactionAddedCallback)
				{
					if (CHECK_M(data, "emoji") &&
						CHECK_M(data, "user_id") &&
						CHECK_M(data, "channel_id") &&
						CHECK_M(data, "message_id"))
					{
						Snowflake userId = data["user_id"].get<std::string>();
						Snowflake channelId = data["channel_id"].get<std::string>();
						Snowflake messageId = data["message_id"].get<std::string>();

						DiscordReactionEventInfo info;
						info.Client = this;

						if (CHECK_M(data, "guild_id"))
						{
							info.Guild = GetGuildById(data["guild_id"].get<std::string>());
						}

						info.Emoji = Ptr<DiscordEmoji>(new DiscordEmoji(data["emoji"]));

						Json userJson;
						Json messageJson;

						if ((info.User = GetUserById(userId)) &&
							DiscordRestAPI::Get("/channels/" + channelId.GetValueString() + "/messages/" + messageId.GetValueString(), &messageJson))
						{
							info.User = Ptr<DiscordUser>(new DiscordUser(userJson));
							info.Message = Ptr<DiscordMessage>(new DiscordMessage(this, messageJson));

							ReactionAddedCallback(info);
						}
					}
				}
			}
			else if (type == "MESSAGE_REACTION_REMOVE")
			{
				if (ReactionRemovedCallback)
				{
					if (CHECK_M(data, "emoji") &&
						CHECK_M(data, "user_id") &&
						CHECK_M(data, "channel_id") &&
						CHECK_M(data, "message_id"))
					{
						Snowflake userId = data["user_id"].get<std::string>();
						Snowflake channelId = data["channel_id"].get<std::string>();
						Snowflake messageId = data["message_id"].get<std::string>();

						DiscordReactionEventInfo info;
						info.Client = this;

						if (CHECK_M(data, "guild_id"))
						{
							info.Guild = GetGuildById(data["guild_id"].get<std::string>());
						}

						info.Emoji = Ptr<DiscordEmoji>(new DiscordEmoji(data["emoji"]));

						Json userJson;
						Json messageJson;

						if (DiscordRestAPI::Get("/users/" + userId.GetValueString(), &userJson) &&
							DiscordRestAPI::Get("/channels/" + channelId.GetValueString() + "/messages/" + messageId.GetValueString(), &messageJson))
						{
							info.User = Ptr<DiscordUser>(new DiscordUser(userJson));
							info.Message = Ptr<DiscordMessage>(new DiscordMessage(this, messageJson));

							ReactionRemovedCallback(info);
						}
					}
				}
			}
			else if (type == "MESSAGE_REACTION_REMOVE_ALL")
			{
				if (ReactionRemovedAllCallback)
				{
					if (CHECK_M(data, "channel_id") &&
						CHECK_M(data, "message_id"))
					{
						Snowflake channelId = data["channel_id"].get<std::string>();
						Snowflake messageId = data["message_id"].get<std::string>();

						DiscordReactionEventInfo info;
						info.Client = this;

						if (CHECK_M(data, "guild_id"))
						{
							info.Guild = GetGuildById(data["guild_id"].get<std::string>());
						}

						Json messageJson;

						if (DiscordRestAPI::Get("/channels/" + channelId.GetValueString() + "/messages/" + messageId.GetValueString(), &messageJson))
						{
							info.Message = Ptr<DiscordMessage>(new DiscordMessage(this, messageJson));

							ReactionRemovedAllCallback(info);
						}
					}
				}
			}
			else if (type == "MESSAGE_REACTION_REMOVE_EMOJI")
			{
				if (ReactionFullyRemovedCallback)
				{
					if (CHECK_M(data, "emoji") &&
						CHECK_M(data, "channel_id") &&
						CHECK_M(data, "message_id"))
					{
						Snowflake channelId = data["channel_id"].get<std::string>();
						Snowflake messageId = data["message_id"].get<std::string>();

						DiscordReactionEventInfo info;
						info.Client = this;

						if (CHECK_M(data, "guild_id"))
						{
							info.Guild = GetGuildById(data["guild_id"].get<std::string>());
						}

						info.Emoji = Ptr<DiscordEmoji>(new DiscordEmoji(data["emoji"]));

						Json messageJson;

						if (DiscordRestAPI::Get("/channels/" + channelId.GetValueString() + "/messages/" + messageId.GetValueString(), &messageJson))
						{
							info.Message = Ptr<DiscordMessage>(new DiscordMessage(this, messageJson));

							ReactionFullyRemovedCallback(info);
						}
					}
				}
			}
			else if (type == "PRESENCE_UPDATE")
			{
				Ptr<DiscordGuild> guild = GetGuildById(data["guild_id"].get<std::string>());

				if (!guild)
				{
					return;
				}

				DiscordPresence* presence = new DiscordPresence(data);

				if (auto iterator = guild->Presences.find(presence->User.Id); iterator != guild->Presences.end())
				{
					iterator->second.reset(presence);

					if (PresenceUpdatedCallback)
					{
						DiscordPresenceEventInfo info;
						info.Client = this;
						info.Guild = guild;
						info.Presence = iterator->second;

						PresenceUpdatedCallback(info);
					}
				}
				else delete presence;
			}
			else if (type == "TYPING_START")
			{
				if (TypingStartCallback)
				{
					if (CHECK_M(data, "channel_id") &&
						CHECK_M(data, "user_id") &&
						CHECK_M(data, "timestamp"))
					{
						DiscordTypingStartEventInfo info;
						info.Client = this;
						info.Timestamp = data["timestamp"].get<int>();

						if (CHECK_M(data, "guild_id"))
						{
							info.Guild = GetGuildById(data["guild_id"].get<std::string>());

							if (CHECK_M(data, "member"))
							{
								info.Member = Ptr<DiscordGuildMember>(new DiscordGuildMember(data["member"]));
							}
						}

						Json userJson;
						Json channelJson;

						if (DiscordRestAPI::Get("/users/" + data["user_id"].get<std::string>(), &userJson) &&
							DiscordRestAPI::Get("/channels/" + data["channel_id"].get<std::string>(), &channelJson))
						{
							info.User = Ptr<DiscordUser>(new DiscordUser(userJson));
							info.Channel = Ptr<DiscordChannel>(DiscordChannel::Create(this, info.Guild.get(), data));

							TypingStartCallback(info);
						}
					}
				}
			}
			else if (type == "USER_UPDATE")
			{
				DiscordUserEventInfo info;
				info.User = Ptr<DiscordUser>(new DiscordUser(data));
				info.Client = this;

				for (auto& guild : Guilds)
				{
					for (auto& member : guild.second->Members)
					{
						if (member.second->Id == info.User->Id)
						{
							member.second->UpdateUser(info.User);
						}
					}
				}

				if (UserUpdatedCallback)
					UserUpdatedCallback(info);
			}
		}
	}

#define JSON_NOEXCEPTION 

	void DiscordClient::OnMessageReceived(std::string msg)
	{
		try
		{
			Json json = Json::parse(msg);

			if (json.is_null())
			{
				std::cout << "json null: " << msg << "\n";
				return;
			}

			auto data = json["d"];
			int opcode = json["op"];

#ifdef _DEBUG
			std::cout << "Received opcode: " << opcode << "\n";
#endif

			switch (static_cast<DiscordOpcode>(opcode))
			{
				case DiscordOpcode::DISPATCH:
				{
					OnDispatch(json, data);
				} break;
				case DiscordOpcode::HEARTBEAT:
				{
					SendHeartbeat();
				} break;
				case DiscordOpcode::RECONNECT:
				{
					Reconnect();
				} break;
				case DiscordOpcode::INVALID_SESSION:
				{
					SendIdentify();
				} break;
				case DiscordOpcode::HELLO:
				{
					if (data.is_null())
						break;
					if (auto interval = data["heartbeat_interval"]; !interval.is_null())
					{
						HeartbeatInterval = data["heartbeat_interval"].get<int>();
						StartHeartbeating();
						if (!SessionId.empty())
						{
							Json json;
							json["op"] = 6;
							json["data"] =
							{
								{ "token", Config.Token },
								{ "session_id", SessionId },
								{ "seq", LastSequenceNumber }
							};
							Send(json.dump());
						}
						else
						{
							SendIdentify();
						}
					}
				} break;
				case DiscordOpcode::HEARTBEAT_ACK:
				{
					bHeartbeatFailed = false;
				} break;
			}
		}
		catch (const Json::parse_error& ex)
		{
			std::cout << "ERROR: " << ex.what() << " (" << ex.byte << ")\nERROR MESSAGE: |" << msg << "|\n";
		}
	}

	void DiscordClient::Reconnect()
	{
		SessionId.clear();
		WebSocket->Stop("Reconnecting");
		WebSocket->Run();
	}

	Json DiscordClientEdit::Serialize() const
	{
		Json json;
		json["username"] = Username;

		if (AvatarImageFile.has_value())
			json["avatar"] = DiscordUtils::GetImageData(AvatarImageFile.value());

		return json;
	}
}