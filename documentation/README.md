# Documentation

- [Channels Documentation](#channels)
	- [Editing Channel](#editing-channel)
	- [Delete or Close Channel](#delete-or-close-channel)
	- [Get Channel Messages](#get-channel-messages)
	- [Get Channel Message](#get-channel-message)
	- [Send Message](#send-message)
	- [Add Message Reaction](#add-reaction)
	- [Remove Own Message Reaction](#remove-own-reaction)
	- [Remove User Message Reaction](#remove-user-reaction)
	- [Get Users Reactions](#get-reactions)
	- [Remove All Reactions](#remove-all-reactions)
	- [Remove All Reactions For Specific Emoji](#remove-all-reactions-for-specific-emoji)
	- [Edit Message](#edit-message)
	- [Delete Message](#delete-message)
	- [Bulk Delete Messages](#bulk-delete-messages)
	- [Edit Channel Permission](#edit-permission)
	- [Get Channel Invites](#get-invites)
	- [Delete Channel Permission](#delete-permission)
	- [Get Pinned Messages](#get-pinned-messages)
	- [Pin Message](#pin-message)
	- [Unpin Message](#unpin-message)
	- [Add Recipient](#add-recipient)
	- [Remove Recipient](#remove-recipient)
- [Guild Documentation](#guild)
	- [Create Channel](#create-channel)
	- [Edit Channel Position](#edit-channel-position)
	- [Create Emoji](#create-emoji)
	- [Edit Emoji](#edit-emoji)
	- [Delete Emoji](#delete-emoji)
	- [Add Member](#add-member)
	- [Edit Member](#edit-member)
	- [Edit Current Nickname](#edit-current-nickname)
	- [Add Role To Member](#add-member-role)
	- [Remove Role From Member](#remove-member-role)
	- [Remove Member](#remove-member)
	- [Get Banned User List](#get-bans)
	- [Get Banned User](#get-ban)
	- [Ban Member](#ban-member)
	- [Kick Member](#kick-member)
	- [Unban User](#unban-user)
	- [Create Role](#create-role)
	- [Edit Role Position](#edit-role-position)
	- [Edit Role](#edit-role)
	- [Delete role](#delete-role)

# Channels
- [Editing Channel](#editing-channel)
- [Delete or Close Channel](#delete-or-close-channel)
- [Get Channel Messages](#get-channel-messages)
- [Get Channel Message](#get-channel-message)
- [Send Message](#send-message)
- [Add Message Reaction](#add-reaction)
- [Remove Own Message Reaction](#remove-own-reaction)
- [Remove User Message Reaction](#remove-user-reaction)
- [Get Users Reactions](#get-reactions)
- [Remove All Reactions](#remove-all-reactions)
- [Remove All Reactions For Specific Emoji](#remove-all-reactions-for-specific-emoji)
- [Edit Message](#edit-message)
- [Delete Message](#delete-message)
- [Bulk Delete Messages](#bulk-delete-messages)
- [Edit Channel Permission](#edit-permission)
- [Get Channel Invites](#get-invites)
- [Delete Channel Permission](#delete-permission)
- [Get Pinned Messages](#get-pinned-messages)
- [Pin Message](#pin-message)
- [Unpin Message](#unpin-message)
- [Add Recipient](#add-recipient)
- [Remove Recipient](#remove-recipient)

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
To edit message, you would need to call `EditMessage` function

This function has 3 overloads, but it leads to:
- Message => Can be as id (Snowflake) or DiscordMessage object
- Lambda (DiscordMessageEdit) => Lambda

Example:

```cpp
channel->EditMessage(message, [](Discord::DiscordMessageEdit* edit)
{
	edit->Content = "Edited!";
});
```

```cpp
channel->EditMessage(1299783912387123, [](Discord::DiscordMessageEdit* edit)
{
	edit->Content = "Hello World!";
});
```

## Delete Message
To delete message you would need to call `DeleteMessage` function

This function has 3 overloads, but it leads to:
- Message => Can be as id (Snowflake) or DiscordMessage object

Example:

```cpp
channel->DeleteMessage(102378123716231732);
```

## Bulk Delete Messages
To bulk delete messages you would need to call `DeleteMessages` function

This function require list of message id to delete

Example:

```cpp
channel->DeleteMessages({ 868339337048252497, 868339123128252, 8681393378123681 });
```

## Edit Permission
In order to edit channel permission, you would need to cast it to a specific channel

For example if you want to edit a Text Channel permission, you would need to cast it to DiscordTextChannel object

*Info: DM Channel and Group DM Channel can't be edited*

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

After you have casted it, now you will be able to access `EditPermission` function

This function has 3 overloads and it leads to:
- User / Role => Can be as id (Snowflake) or as an object
- Lambda (DiscordPermissionOverwrite) => Lambda

Example:

```cpp
channel->EditPermission(34805882584209186, [](Discord::DiscordPermissionOverwrite* perm)
{
	perm->Allow(Discord::DiscordPermissionsFlags::VIEW_CHANNEL);
	perm->Deny(Discord::DiscordPermissionsFlags::ADD_REACTIONS);
});
```

The code above will change the permission of User/Role with id 34805882584209186

That User/Role can view channel but can't add message reaction

## Get Invites
To get list of invite, you would need to call `GetInvites` function

Example:

```cpp
auto invites = channel->GetInvites();
```

## Delete Permission
In order to delete Role / User permission, you would need to cast it to a specific channel

For example if you want to delete permission in Text Channel, you would need to cast it to DiscordTextChannel object

*Info: DM Channel and Group DM Channel can't be used*

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

After you have casted it, now you will be able to access `DeletePermission` function

This function has 3 overloads but it leads to:
- Role / User => Can be as id (Snowflake) or DiscordUser object or DiscordRole object

Example:

```cpp
channel->DeletePermission(12345678911);
```

## Get Pinned Messages
To get pinned messages you would need to call `GetPinnedMessages` function

Example:

```cpp
auto pinnedMessages = channel->GetPinnedMessages();
```

## Pin Message
To pin message you would need to call `PinMessage` function

This function has many overloads but it leads to:
- Message => Can be as id (Snowflake) or DiscordMessage object

## Unpin Message
To unpin message you would need to call `UnpinMessage` function

This function has many overloads but it leads to:
- Message => Can be as id (Snowflake) or DiscordMessage object

## Add Recipient
This will only work in DM / Group DM channels

To add recipient you would need to call `AddRecipient` function

This function has 2 overloads which just leads to:
- User => DiscordUser object
- Access Token => Access Token string

Example:

```cpp
pDMChannel->AddRecipient(user, "token");
groupDMChannel->AddRecipient(user, "token");
```

## Remove Recipient
This will only work in DM / Group DM channels

To add recipient you would need to call `RemoveRecipient` function

This function has 3 overloads which leads to:
- User => Can be as id (Snowflake) or DiscordUser object

Example:

```cpp
pDMChannel->RemoveRecipient(69420);
```

# Guild
- [Create Channel](#create-channel)
- [Edit Channel Position](#edit-channel-position)
- [Create Emoji](#create-emoji)
- [Edit Emoji](#edit-emoji)
- [Delete Emoji](#delete-emoji)
- [Add Member](#add-member)
- [Edit Member](#edit-member)
- [Edit Current Nickname](#edit-current-nickname)
- [Add Role To Member](#add-member-role)
- [Remove Role From Member](#remove-member-role)
- [Remove Member](#remove-member)
- [Get Banned User List](#get-bans)
- [Get Banned User](#get-ban)
- [Ban Member](#ban-member)
- [Kick Member](#kick-member)
- [Unban User](#unban-user)
- [Create Role](#create-role)
- [Edit Role Position](#edit-role-position)
- [Edit Role](#edit-role)
- [Delete role](#delete-role)

## Create Channel
To create channel, you would need to call `CreateChannel` function and pass a `DiscordChannelCreate` object

Example:

```cpp
DiscordChannelCreate create;
create.Name = "channel name";
create.Topic = "channel topic";
// u can access more in property that `DiscordChannelCreate` has

guild->CreateChanel(create);
```

## Edit Channel Position
To edit channel position, you would need to call `EditChannelPosition` function

This function has 3 overloads which leads to:
- Channel => Can be as id (Snowflake) or DiscordChannel object
- Position => Channel position
- Lock Permission => boolean, syncs the permission overwrites with the new parent, if moving to a new category
- Parent Id => Id (Snowflake), the new parent ID for the channel that is moved

## Create Emoji
To create emoji, you would need to call `CreateEmoji` function and pass a `DiscordEmojiCreate` object

Example:

```cpp
DiscordEmojiCreate create;
create.Name = "pog";
create.ImageFilePath = "pog.png";

guild->CreateEmoji(create);
```

## Edit Emoji
To edit emoji, you would need to call `EditEmoji` function

This function has 3 overloads which leads to:
- Emoji => Can be as id (Snowflake) or DiscordEmoji object
- Lambda (DiscordEmojiEdit) => Lambda

Example:

```cpp
guild->EditEmoji(198371319327891938, [](Discord::DiscordEmojiEdit* edit)
{
	edit->Name = "pog_new_name";
});
```

## Delete Emoji
To delete emoji, you would need to call `DeleteEmoji` function

This function has 3 overloads but it leads to:
- Emoji => Can be as id (Snowflake) or DiscordEmoji object

Example:

```cpp
guild->DeleteEmoji(1234512731763872);
```

## Add Member
To add member, you would need to call `AddMember` function

This function has 2 parameters:
- Id => Snowflake, the member's id you want to add
- Create Object => DiscordGuildMemberCreate, info object

Example:

```cpp
DiscordGuildMemberCreate create;
create.AccessToken = "token";
create.Nickname = "nickname";

guild->AddMember(12345678919839132, create);
```

## Edit Member
To edit member, you would need to call `EditMember` function

This function has 3 overloads which leads to:
- Member => Can be as id (Snowflake) or DiscordGuildMember object
- Lambda (DiscordGuildMemberEdit) => Lambda

Example:

```cpp
guild->EditMember(1231987389123, [](Discord::DiscordGuildMemberEdit* edit)
{
	edit->Mute = true;
	edit->Deaf = true;
	edit->Nickname = "nickname";
});
```

## Edit Current Nickname
To edit current client nickname, you would need to call `EditCurrentNickname` and pass a `nickname` string

Example:

```cpp
guild->EditCurrentNickname("nickname");
```

## Add Member Role
To add role to member, you would need to call `AddMemberRole` function

This function has 3 overloads which leads to:
- Member => Can be as Id (Snowflake) or DiscordGuildMember object
- Role Id => Id (Snowflake), the role id

Example:

```cpp
guild->AddMemberRole(12371681739182, 129371982739817312);
```

## Remove Member Role
To remove role from member, you would need to call `RemoveMemberRole` function

This function has 3 overloads which leads to:
- Member => Can be as Id (Snowflake) or DiscordGuildMember object
- Role Id => Id (Snowflake), the role id

Example:

```cpp
guild->RemoveMemberRole(12371681739182, 129371982739817312);
```

## Remove Member
To remove member, you would need to call `RemoveMember` function

This function has 3 overloads which leads to:
- Member => Can be as Id (Snowflake) or DiscordGuildMember object

Example:

```cpp
guild->RemoveMember(123761632123);
```

## Get Bans
To get banned user list, you would need to call `GetBans` function

Example:

```cpp
auto list = guild->GetBans();
```

## Get Ban
To get specified banned user, you would need to call `GetBan` function and pass a `Id`

Example:

```cpp
auto ban = guild->GetBan(11987391873198371);
```

## Ban Member
To ban member, you would need to call `BanMember` function

This function has 3 overloads, but it leads to:
- Member => Can be as id (Snowflake) or DiscordGuildMember object
- Reason => Ban reason (string)
- Delete Message Days => number of days to delete messages for (0-7)

Example:

```cpp
guild->BanMember(92873198739187381723, "noob", 7);
```

## Kick Member
To kick member, you would need to call `KickMember` function

This function has 3 overloads, but it leads to:
- Member => Can be as id (Snowflake) or DiscordGuildMember object

Example:

```cpp
guild->KickMember(1273197391837123);
```

## Unban User
To unban user, you would need to call `UnbanUser` function

This function has 3 overloads, but it leads to:
- User => Can be as id (Snowflake) or DiscordUser object

Example:

```cpp
guild->UnbanUser(1273197391837123);
```

## Create Role
To create role, you would need to call `CreateRole` function and pass a `DiscordRoleInfo` object

Example:

```cpp
DiscordRoleInfo create;
create.Name = "role-name";
create.Role = DiscordColor(255, 0, 0); // red

guild->CreateRole(create);
```

## Edit Role Position
To edit role position, you would need to call `EditRolePosition` function

This function has 3 overloads which leads to:
- Role => Can be as id (Snowflake), or DiscordRole object
- Position => New position

Example:

```cpp
guild->EditRolePosition(128371283781973, 1);
```

## Edit Role
To edit role, you would need to call `EditRole` function

This function has 3 overloads which leads to:
- Role => Can be as id (Snowflake), or DiscordRole object
- Lambda (DiscordRoleInfo) => Lambda

Example:

```cpp
guild->EditRole(118739812938139132, [](Discord::DiscordRoleInfo* edit)
{
	edit->Name = "new-name";
	edit->Color = DiscordColor(0, 0, 255); // blue
});
```

## Delete Role
To delete role, you would need to call `DeleteRole` function

This function has 3 overloads which leads to:
- Role => Can be as id (Snowflake), or DiscordRole object

Example:

```cpp
guild->DeleteRole(127319827391283712);
```
