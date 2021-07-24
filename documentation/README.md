# Documentation

- [Channels Documentation](#Channels)
	- [Editing Channel](##Editing-Channel)

# Channels
- [Editing Channel](##Editing-Channel)

## Editing Channel
In order to edit channel, you would need to cast it to a specific channel

For example if you want to edit a Text Channel, you would need to cast it to DiscordTextChannel object.



```cpp
auto textChannel = std::dynamic_pointer_cast<Discord::DiscordTextChannel>(channel);

if (textChannel)
{
	// Do something
}
```

After you have casted it