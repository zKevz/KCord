# Documentation

- [Channels Documentation](#Channels)
	- [Editing Channel](##Editing-Channel)
	- [Delete or Close Channel](##Delete-or-Close-Channel)
	- [Get Channel Messages](##Get-Channel-Messages)
	- [Get Channel Message](##Get-Channel-Message)
	- [Send Message](##Send-Message)
	- [Add Reaction To Message](##Add-Reaction)
	- [Remove Own Message Reaction](##Remove-Own-Reaction)
	- [Remove User Message Reaction](##Remove-User-Reaction)
	- [Get Users Reactions](##Get-Reactions)
	- [Remove All Reactions](##Remove-All-Reactions)
	- [Remove All Reactions For Specific Emoji](##Remove-All-Reactions-For-Specific-Emoji)
	- [Edit Message](##Edit-Message)

# Channels
- [Editing Channel](##Editing-Channel)
- [Delete or Close Channel](##Delete-or-Close-Channel)
- [Get Channel Messages](##Get-Channel-Messages)
- [Get Channel Message](##Get-Channel-Message)
- [Send Message](##Send-Message)
- [Add Reaction To Message](##Add-Reaction)
- [Remove Own Message Reaction](##Remove-Own-Reaction)
- [Remove User Message Reaction](##Remove-User-Reaction)
- [Get Users Reactions](##Get-Reactions)
- [Remove All Reactions](##Remove-All-Reactions)
- [Remove All Reactions For Specific Emoji](##Remove-All-Reactions-For-Specific-Emoji)
- [Edit Message](##Edit-Message)

## Editing Channel
In order to edit channel, you would need to cast it to a specific channel

For example if you want to edit a Text Channel, you would need to cast it to DiscordTextChannel object

*Info: DM Channel can't be edited*

```cpp
auto textChannel = std::dynamic_pointer_cast<Discord::DiscordTextChannel>(channel);

if (textChannel)
{
	// Do something
}

auto newsChannel = std::dynamic_pointer_cast<Discord::DiscordNewsChannel>(channel);

if (newsChannel)
{
	// Do something
}
```

After you have casted it, now you will be able to access `Edit` function

In `Edit` function you would need to past a lambda with the edit object as the lambda parameter, for example

```cpp
textChannel->Edit([](Discord::DiscordTextChannelEdit* edit)
{
	edit->Name = "name";
	edit->Position = 1;
});

newsChannel->Edit([](Discord::DiscordNewsChannelEdit* edit)
{
	edit->Topic = "topic!";
});
```

## Delete or Close Channel
Deleting or closing channel is as simple as just calling `DeleteOrClose` function

```cpp
channel->DeleteOrClose();
```

## Get Channel Messages
To get channel messages, you would need to call `GetMessages` function

This function has 3 parameters: 
- limit (integer) => messages limit, from 1 - 100
- id (snowflake / unique id) (optional) => message id
- direction (message direction) (optional) => direction (after, around, before)

Below code will retrieve 50 messages
```cpp
auto messages = channel->GetMessages(50);
```

Below code will retrieve 75 messages BEFORE message with id 69
```cpp
auto messages = channel->GetMessages(75, 69, Discord::MessageDirection::BEFORE);
```

## Get Channel Message
To get channel message, you would need to call `GetMessageById` with message id as its parameter

Why not name the function `GetMessage` instead of `GetMessageById` ? Well it's because `GetMessage` is reserved as macro in windows

Below code will retrieve message object whose id is 69
```cpp
auto message = GetMessageById(69);

if (message)
{
	// success
}
else
{
	std::cout << "Can't get message with id 69.\n";
}
```

## Send Message
To send message, you would need to call `Send` function

Why not name the function `SendMessage` instead of `Send` ? Well it's because `SendMessagae` is reserved as macro in windows

This function has 2 overloads:
- First:
	- Embed (DiscordEmbed) => message embed
- Second:
	- Message Content (string) => message content
	- TTS (bool) => determine whether message is TTS (text-to-speech) or not
	- Embed (DiscordEmbed) => message embed

Example:

```cpp
Discord::DiscordEmbed embed;
embed.SetTitle("Hello from KCord!");

channel->Send(embed);
```

```cpp
channel->Send("Hello World!");
```

You can also send file by calling `SendFile` and put the file location as its parameter.

```cpp
channel->SendFile("file.txt");
```

## Add Reaction
To add reaction to message, you would need to call `AddMessageReaction`

This function has many overloads, but the main things are:
- Message => Can be as id (Snowflake), or DiscordMessage object
- Emoji => DiscordEmoji object
- Callback (optional) => Callback when callback when it succeedd (lambda)

Example:

```cpp
auto message = channel->Send("The bot will react to this message!");

if (message)
{
	channel->AddMessageReaction(message, Discord::DiscordEmoji::FromUnicode(u8"??"));
}
```

```cpp
auto emoji = guild->GetEmojiByName("kcord");
auto message = channel->Send("The bot will react to this message!");

if (emoji && message)
{
	channel->AddMessageReaction(message, emoji);
}
```

## Remove Own Reaction
To remove own message reaction, you would need to call `RemoveOwnMessageReaction`

This function has many overloads, but the main things are:
- Message => Can be as id (Snowflake), or DiscordMessage object
- Emoji => DiscordEmoji object
- Callback (optional) => Callback when callback when it succeedd (lambda)

Example:

```cpp
channel->RemoveOwnMessageReaction(message, Discord::DiscordEmoji::FromUnicode(u8"??"));
```

## Remove User Reaction
To remove user message reaction, you would need to call `RemoveUserMessageReaction`

This function has many overloads, but the main things are:
- Message => Can be as id (Snowflake), or DiscordMessage object
- Emoji => DiscordEmoji object
- UserId => User id you want his reaction to be removed
- Callback (optional) => Callback when callback when it succeedd (lambda)

Example:

```cpp
channel->RemoveUserMessageReaction(message, Discord::DiscordEmoji::FromUnicode(u8"??"), target->GetId());
```

## Get Reactions
To get a list of users that reacted with this emoji you would need to call `GetMessageReactionUser` function

This function has many overloads, but the main things are:
- Message => Can be as id (Snowflake), or DiscordMessage object
- Emoji => DiscordEmoji object
- Callback (optional) => Callback when it succeed (lambda)

Example:

```cpp
auto users = channel->GetMessageReactionUser(message, Discord::DiscordEmoji::FromUnicode(u8"??"));
```

## Remove All Reactions
To remove all message reactions, you would need to call `RemoveAllMessageReactions` function

This function has 3 overloads, but it leads to:
- Message => Can be as id (Snowflake) or DiscordMessage object
- Callback (optional) => Callback when it succeed (lambda)

Example

```cpp
channel->RemoveAllMessageReactions(message);
```

## Remove All Reactions For Specific Emoji
To remove all message reactions with specific emoji, you would need to call `RemoveAllMessageReactionsForEmoji` function

This function has many overloads, but the main things are:
- Message => Can be as id (Snowflake), or DiscordMessage object
- Emoji => DiscordEmoji object
- Callback (optional) => Callback when it succeed (lambda)

Example:

```cpp
channel->RemoveAllMessageReactionsForEmoji(message, Discord::DiscordEmoji::FromUnicode(u8"??"));
```

## Edit Message