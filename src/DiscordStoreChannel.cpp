#include "DiscordStoreChannel.hpp"

namespace Discord
{
    Json DiscordStoreChannelEdit::Serialize() const
    {
        Json json = DiscordGuildChannelEdit::Serialize();
        json["nsfw"] = IsNSFW;
        json["parent_id"] = ParentId.GetValueString();
        return json;
    }

    void DiscordStoreChannel::Edit(const std::function<void(DiscordStoreChannelEdit*)>& func)
    {
        DiscordStoreChannelEdit edit;
        edit.IsNSFW = bIsNSFW;
        edit.Name = Name;
        edit.ParentId = ParentId;
        edit.Permissions = Permissions;
        edit.Position = Position;

        func(&edit);

        DiscordRestAPI::Patch("/channels/" + Id.GetValueString(), edit.Serialize());
    }
}