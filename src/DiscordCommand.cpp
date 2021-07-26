#include "DiscordCommand.hpp"
#include "DiscordUtils.hpp"
#include "DiscordClient.hpp"

namespace Discord
{
	void DiscordCommandService::Add(const std::string& command, const DiscordCommandInfo& callback)
	{
		Commands.insert_or_assign(command, callback);
	}

	void DiscordCommandService::Process(Ptr<DiscordCommandServiceContainer> ptr)
	{
		auto parameters = DiscordUtils::SplitString(ptr->Content, " ");

		if (parameters.empty())
			return;

		if (auto iterator = Commands.find(parameters[0]); iterator != Commands.end())
		{
			DiscordCommandContext params;
			params.Guild = ptr->Message->GetGuild();
			params.Client = ptr->Client;
			params.Message = ptr->Message;
			params.Channel = ptr->Message->GetChannel();
			params.Parameters = parameters;
			params.InteractivityService = &ptr->Client->GetInteractivityService();

			if (params.Guild)
			{
				if (!params.Message->GetWebhookId().GetValue() &&
					!params.Message->GetApplicationId().GetValue())
				{
					params.Member = params.Guild->GetMemberById(params.Message->GetAuthor()->GetId());

					if (params.Member)
					{
						if (iterator->second.RequirePermission.has_value() && iterator->second.RequirePermission.value())
						{
							bool success = false;

							for (const auto& id : params.Member->GetRoleIds())
							{
								auto role = params.Guild->GetRoleById(id);

								if (!role)
									continue;

								if (role->GetPermissionBitSet() & iterator->second.RequirePermission.value())
								{
									success = true;
									break;
								}
							}

							if (!success)
								return;
						}

						if (iterator->second.RequireRoles.has_value() && iterator->second.RequireRoles.value().size())
						{
							auto predicate = [&](const std::string& name) -> bool
							{
								bool success = false;

								for (const auto& id : params.Member->GetRoleIds())
								{
									auto role = params.Guild->GetRoleById(id);

									if (!role)
										continue;

									if (DiscordUtils::StringEqual(role->GetName(), name, false))
									{
										success = true;
										break;
									}
								}

								return success;
							};

							auto vec = iterator->second.RequireRoles.value();

							if (iterator->second.RoleTarget == DiscordRoleTargetType::ANY)
							{
								if (!std::any_of(vec.begin(), vec.end(), predicate))
									return;
							}
							else
							{
								if (!std::all_of(vec.begin(), vec.end(), predicate))
									return;
							}
						}
					}
				}
			}

			iterator->second.Callback(&params);
		}
	}

	void DiscordCommandContext::Reply(const DiscordEmbed& embed)
	{
		Reply("", false, embed);
	}

	void DiscordCommandContext::Reply(const std::string& msg, bool tts, std::optional<DiscordEmbed> embed)
	{
		if (auto channel = GetChannel())
		{
			channel->Send(msg, tts, embed);
		}
	}

	void DiscordCommandContext::ReplyAndDeleteAfter(const DiscordEmbed& embed, DiscordTimeDuration duration)
	{
		ReplyAndDeleteAfter("", false, embed, duration);
	}
	
	void DiscordCommandContext::ReplyAndDeleteAfter(const std::string& msg, bool tts, DiscordTimeDuration duration)
	{
		ReplyAndDeleteAfter(msg, tts, {}, duration);
	}

	void DiscordCommandContext::ReplyAndDeleteAfter(const std::string& msg, bool tts, std::optional<DiscordEmbed> embed, DiscordTimeDuration duration)
	{
		if (auto channel = GetChannel())
		{
			static auto Callback = [](std::shared_ptr<DiscordChannel> ch, std::shared_ptr<DiscordMessage> ptr, DiscordTimeDuration duration)
			{
				std::this_thread::sleep_for(duration - std::chrono::seconds(2));
				ch->DeleteMessage(ptr);
			};
			Callback(channel, channel->Send(msg, tts, embed), duration);
		}
	}
}