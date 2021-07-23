#include "DiscordChannelCategory.hpp"

namespace Discord
{
    void DiscordChannelCategory::Edit(const std::function<void(DiscordGuildChannelEdit*)>& func)
    {
        DiscordGuildChannelEdit edit;
        edit.Name = Name;
        edit.Permissions = Permissions;
        edit.Position = Position;

        func(&edit);

        DiscordRestAPI::Patch("/channels/" + Id.GetValueString(), edit.Serialize());
    }
}