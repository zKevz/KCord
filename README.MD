# KCord
C++ Unofficial wrapper for the Discord API

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

    client->InvokeCommand(msg);
}

void OnPingCommand(const Discord::DiscordCommandContext& context)
{
    context.GetChannel()->Send("Pong!");
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

# Contributor
<<<<<<< HEAD
[Me](https://github.com/zKevz)
=======
me
>>>>>>> 2e3d3b08cfa0052e5f9a898ae33bd1f7e5da5b5e

[free#1234](https://github.com/smhman)

[Abood#1550](https://github.com/AboodTBR)

<<<<<<< HEAD
[Alexander#9673](https://github.com/Alexander9673)
=======
[Alexander#9673](https://github.com/Alexander9673)
>>>>>>> 2e3d3b08cfa0052e5f9a898ae33bd1f7e5da5b5e
