#include <iostream>
#include "DiscordClient.hpp"

void OnSumCommand(const Discord::DiscordCommandContext& context)
{
    auto channel = context.GetChannel();

    int64_t num1, num2;

    if (context.GetArgument(1, &num1) &&
        context.GetArgument(2, &num2))
    {
        /*
        Discord::DiscordEmbed embed;
        embed.SetTitle("Sum");
        embed.AddField("Result", std::to_string(num1 + num2));
        embed.SetFooter("KCord");
        embed.PutTimestamp();
        embed.PutRandomColor();
        channel->Send(embed);
        */

        /* Both code above and below are equivalent */

        channel->Send(Discord::DiscordEmbed()
            .SetTitle("Sum")
            .AddField("Result", std::to_string(num1 + num2))
            .SetFooter("KCord")
            .PutTimestamp()
            .PutRandomColor());
    }
}

void OnEditChannelCommand(const Discord::DiscordCommandContext& context)
{
    std::string name;

    Ptr<Discord::DiscordGuild> guild = context.GetGuild();
    Ptr<Discord::DiscordChannel> channel = context.GetChannel();

    if (context.GetArgument(1, &name))
    {
        if (auto textChannel = std::dynamic_pointer_cast<Discord::DiscordTextChannel>(channel))
        {
            textChannel->Edit([&](Discord::DiscordTextChannelEdit* edit)
            {
                edit->Name = name;
            });

            Ptr<Discord::DiscordRole> role = guild->GetRoleByName("some role lol", false);

            if (!role)
                return;

            textChannel->EditPermission(role, [&](Discord::DiscordPermissionOverwrite* permission)
            {
                permission->Allow(Discord::DiscordPermissionsFlags::VIEW_CHANNEL);
                permission->Deny(Discord::DiscordPermissionsFlags::MANAGE_CHANNELS);
            });
            textChannel->Send("Done!");
        }
    }
}

void OnReactMessageCommand(const Discord::DiscordCommandContext& context)
{
    Ptr<Discord::DiscordChannel> channel = context.GetChannel();
    Ptr<Discord::DiscordMessage> message = context.GetMsg();
    Ptr<Discord::DiscordGuildMember> member = context.GetMember();

    channel->AddMessageReaction(channel->Send("lel"), Discord::DiscordEmoji::FromUnicode(u8"😂"));
    channel->AddMessageReaction(channel->SendFile("test.txt"), Discord::DiscordEmoji::FromUnicode(u8"😂"));
}

void OnMessage(const Discord::DiscordMessageEventInfo& info)
{
    Discord::DiscordClient* client = info.GetClient();

    Ptr<Discord::DiscordMessage> msg     = info.GetMsg();
    Ptr<Discord::DiscordUser>    user    = msg->GetAuthor();
    Ptr<Discord::DiscordGuild>   guild   = info.GetGuild();
    Ptr<Discord::DiscordChannel> channel = msg->GetChannel();

    if (user->IsBot())
        return;

    if (user->IsDiscordCertifiedModerator())
    {
        std::cout << "omg!\n";
    }

    client->InvokeCommand(msg);
}

void OnReady()
{
    std::cout << "Discord Bot is ready!\n";
}

int main(int argc, char* argv[]) 
{
    Discord::DiscordClientConfig config;
    config.Token = "ur-token";
    config.Prefix = "&";
    config.Threshold = 250;
    config.EnableDM = false;
    config.PrefixCaseSensitive = false;

    Discord::DiscordClient client(config);

    client.OnReady(OnReady);
    client.OnMessageCreated(OnMessage);

    /********************************************************/
    /*                      METHOD 1                        */

    Discord::DiscordCommandInfo info;
    info.RoleTarget = Discord::DiscordRoleTargetType::ANY;
    info.Description = "Sum 2 numbers!";
    info.RequireRoles = { "helper", "moderator" };
    info.RequirePermission = Discord::DiscordPermissionsFlags::KICK_MEMBERS | Discord::DiscordPermissionsFlags::BAN_MEMBERS;

    client.RegisterCommand("sum", OnSumCommand, info);
    /********************************************************/

    /********************************************************/
    /*                      METHOD 2                        */
    client.RegisterCommand("edit", OnEditChannelCommand, Discord::DiscordPermissionsFlags::ADMINISTRATOR);
    /********************************************************/

    /********************************************************/
     /*                      METHOD 3                        */
    client.RegisterCommand("react", OnReactMessageCommand);
    /********************************************************/

    Discord::DiscordGatewayPresence presence;
    presence.Status = Discord::DiscordStatus::DO_NOT_DISTURB;
    presence.StatusText = u8"dad 😳";
    presence.ActivityType = Discord::DiscordActivityType::LISTENING;

    client.SetPresence(presence);
    client.Run();
}