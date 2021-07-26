# KCord
C++ Unofficial wrapper for the Discord API

[![KCord Discord Server](https://discord.com/api/guilds/868134117965955162/embed.png?style=shield)](https://discord.gg/YQg8x29MPw)

# Documentation
You can see the documentation in [documentation tree](https://github.com/zKevz/KCord/tree/main/documentation)

For simple example, you can take a look at [this](https://github.com/zKevz/KCord/blob/main/examples/SimpleBot.cpp)

Or you can check below code
```cpp
#include <iostream>
#include <DiscordClient.hpp>

void OnMessage(const Discord::DiscordMessageEventInfo& info)
{
    Ptr<Discord::DiscordMessage> msg     = info.GetMsg();
    Ptr<Discord::DiscordUser>    user    = msg->GetAuthor();

    if (user->IsBot())
        return;

    info.GetClient()->InvokeCommand(msg);
}

void OnPingCommand(Discord::DiscordCommandContext* context)
{
    context->GetChannel()->Send("Pong!");
}

int main()
{
    Discord::DiscordClientConfig config;
    config.Token = "ur-token";
    config.Prefix = "&";
    config.Threshold = 250;
    config.EnableDM = false;
    config.PrefixCaseSensitive = false;

    Discord::DiscordClient client(config);

    client.OnReady([]() { std::cout << "Bot is ready!\n"; });
    client.OnMessageCreated(OnMessage);

    client.RegisterCommand("ping", OnPingCommand);
    client.Run();
}

```

# Misc
This library is not done yet, and there is plenty features to add, such as:

- Voice, from connecting to sending data, etc

- More framework

However, this library provides you with Interactivity.

```cpp
channel->Send("Say any word!");
    
auto predicate = [&](Discord::DiscordInteractivityPredicate* p)
{
    return p->AuthorId == author->GetId() && 
           p->ChannelId == channel->GetId();
};

auto result = context->GetInteractivity()->WaitForMessage(predicate, 10s);
// Wait for message whose author id is `author->GetId()` and channel id is `channel->GetId()`

if (result.IsSuccess)
{
    context->Reply("You said " + result.Message->GetContent() + "!");
    // Send the result
}
```

# Contributor
[Me](https://github.com/zKevz)

[free#1234](https://github.com/smhman)

[Abood#1550](https://github.com/AboodTBR)

[Alexander#9673](https://github.com/Alexander9673)

# Question?
Please join this discord server to ask questions related to this library

[![KCord Discord Server](https://discord.com/api/guilds/868134117965955162/embed.png?style=banner3)](https://discord.gg/YQg8x29MPw)