#include "DiscordGuild.hpp"
#include "DiscordUtils.hpp"
#include "DiscordGuildChannel.hpp"

#include <iostream>
#include <chrono>

namespace Discord
{
    DiscordGuild::DiscordGuild(DiscordClient* client, const Json& json) : Client(client)
    {
        auto start = std::chrono::high_resolution_clock::now();

        if (CHECK("id"))
            Id = json["id"].get<std::string>();

        if (CHECK("owner_id"))
            OwnerId = json["owner_id"].get<std::string>();

        if (CHECK("afk_timeout"))
            AFKTimeout = json["afk_timeout"].get<int>();

        if (CHECK("name"))
            Name = json["name"].get<std::string>();

        if (CHECK("icon"))
            Icon = json["icon"].get<std::string>();

        if (CHECK("icon_hash"))
            IconHash = json["icon_hash"].get<std::string>();

        if (CHECK("splash"))
            Splash = json["splash"].get<std::string>();

        if (CHECK("discovery_splash"))
            DiscoverySplash = json["discovery_splash"].get<std::string>();

        if (CHECK("owner"))
            bIsOwner = json["owner"].get<bool>();

        if (CHECK("owner_id"))
            OwnerId = json["owner_id"].get<std::string>();

        if (CHECK("permissions"))
            Permissions = json["permissions"].get<std::string>();

        if (CHECK("afk_channel_id"))
            AFKChannelId = json["afk_channel_id"].get<std::string>();

        if (CHECK("afk_timeout"))
            AFKTimeout = json["afk_timeout"].get<int>();

        if (CHECK("widget_enabled"))
            bIsWidgetEnabled = json["widget_enabled"].get<bool>();

        if (CHECK("widget_channel_id"))
            WidgetChannelId = json["widget_channel_id"].get<std::string>();

        if (CHECK("verification_level"))
            VerificationLevel = static_cast<Discord::DiscordVerificationLevel>(json["verification_level"].get<int>());

        if (CHECK("default_message_notifications"))
            DefaultMessageNotification = static_cast<Discord::DiscordMessageNotificationLevel>(json["default_message_notifications"].get<int>());

        if (CHECK("explicit_content_filter"))
            ExplicitContentFilterLevel = static_cast<Discord::DiscordExplicitContentFilterLevel>(json["explicit_content_filter"].get<int>());

        if (CHECK("roles"))
        {
            const auto& roles = json["roles"];
            for (const auto& object : roles)
            {
                Ptr<DiscordRole> role(new DiscordRole(object));
                Roles.insert_or_assign(role->Id, role);
            }
        }
        
        if (CHECK("emojis"))
        {
            const auto& emojis = json["emojis"];
            for (const auto& object : emojis)
            {
                Ptr<DiscordEmoji> emoji(new DiscordEmoji(object));
                Emojis.insert_or_assign(emoji->Id, emoji);
            }
        }

        if (CHECK("features"))
        {
            const auto& features = json["features"];
            for (const auto& object : features)
            {
                GuildFeatures.emplace_back(object.get<std::string>());
            }
        }

        if (CHECK("mfa_level"))
        {
            MFALevel = static_cast<Discord::DiscordMFALevel>(json["mfa_level"].get<int>());
        }

        if (CHECK("application_id"))
            ApplicationId = json["application_id"].get<std::string>();

        if (CHECK("system_channel_id"))
            SystemChannelId = json["system_channel_id"].get<std::string>();

        if (CHECK("system_channel_flags"))
            SystemChannelFlags = json["system_channel_flags"].get<int>();

        if (CHECK("rules_channel_id"))
            RulesChannelId = json["rules_channel_id"].get<std::string>();

        if (CHECK("joined_at"))
            JoinedAt = json["joined_at"].get<std::string>();

        if (CHECK("large"))
            bIsLarge = json["large"].get<bool>();

        if (CHECK("unavailable"))
            bIsUnavailable = json["unavailable"].get<bool>();

        if (CHECK("member_count"))
            MemberCount = json["member_count"].get<int>();

        if (CHECK("voice_states"))
        {
            const auto& voiceStates = json["voice_states"];
            for (const auto& object : voiceStates)
            {
                DiscordVoiceState voiceState = DiscordVoiceState(object);
                VoiceStates.emplace_back(voiceState);
            }
        }

        if (CHECK("members"))
        {
            const auto& members = json["members"];
            for (const auto& object : members)
            {
                Ptr<DiscordGuildMember> member(new DiscordGuildMember(object));
                Members.insert_or_assign(member->Id, member);
            }
        }

        if (CHECK("channels"))
        {
            const auto& channels = json["channels"];
            for (const auto& object : channels)
            {
                DiscordChannel* channel = DiscordChannel::Create(Client, this, object);

                if (channel)
                {
                    Channels.insert_or_assign(channel->Id, Ptr<DiscordChannel>(channel));
                }
                else delete channel;
            }
        }

        if (CHECK("presences"))
        {
            const auto& presences = json["presences"];
            for (const auto& object : presences)
            {
                Ptr<DiscordPresence> presence(new DiscordPresence(object));
                Presences.insert_or_assign(presence->User.Id, presence);
            }
        }

        if (CHECK("max_presences"))
            MaxPresences = json["max_presences"].get<int>();

        if (CHECK("max_members"))
            MaxMembers = json["max_members"].get<int>();

        if (CHECK("vanity_url_code"))
            VanityUrlCode = json["vanity_url_code"].get<std::string>();

        if (CHECK("description"))
            Description = json["description"].get<std::string>();

        if (CHECK("banner"))
            BannerHash = json["banner"].get<std::string>();

        if (CHECK("premium_tier"))
            PremiumTier = static_cast<Discord::DiscordPremiumTier>(json["premium_tier"].get<int>());

        if (CHECK("premium_subscription_count"))
            PremiumSubscriptionCount = json["premium_subscription_count"].get<int>();

        if (CHECK("preferred_locale"))
            PreferredLocale = json["preferred_locale"].get<std::string>();

        if (CHECK("public_updates_channel_id"))
            PublicUpdatesChannelId = json["public_updates_channel_id"].get<std::string>();

        if (CHECK("max_video_channel_users"))
            MaxVideoChannelUsers = json["max_video_channel_users"].get<int>();

        if (CHECK("approximate_member_count"))
            ApproximateMemberCount = json["approximate_member_count"].get<int>();

        if (CHECK("approximate_presence_count"))
            ApproximatePresenceCount = json["approximate_presence_count"].get<int>();

        if (CHECK("welcome_screen"))
            WelcomeScreen = Ptr<DiscordWelcomeScreen>(new DiscordWelcomeScreen(json["welcome_screen"]));
        
        if (CHECK("nsfw_level"))
            NSFWLevel = static_cast<Discord::DiscordNSFWLevel>(json["nsfw_level"].get<int>());

        if (CHECK("stage_instances"))
        {
            const auto& stageInstances = json["stage_instances"];
            for (const auto& object : stageInstances)
            {
                DiscordStageInstance stageInstance = DiscordStageInstance(object);
                StageInstances.emplace_back(stageInstance);
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
    }

    Ptr<DiscordRole> DiscordGuild::GetRoleById(Snowflake id) const
    {
        if (auto it = Roles.find(id); it != Roles.end())
        {
            return it->second;
        }
        return nullptr;
    }

    Ptr<DiscordRole> DiscordGuild::GetRoleByName(const std::string& name, bool caseSensitive) const
    {
        for (const auto& role : Roles)
        {
            if (DiscordUtils::StringEqual(name, role.second->Name, caseSensitive))
            {
                return role.second;
            }
        }
        return nullptr;
    }

    Ptr<DiscordRole> DiscordGuild::GetRoleByPredicate(const std::function<bool(Ptr<DiscordRole>)>& predicate) const
    {
        for (const auto& role : Roles)
        {
            if (predicate(role.second))
            {
                return role.second;
            }
        }
        return nullptr;
    }

    void DiscordGuild::Edit(const std::function<void(DiscordGuildEdit*)>& func)
    {
        DiscordGuildEdit edit;
        edit.AFKChannelId = AFKChannelId;
        edit.AFKTimeout = AFKTimeout;
        edit.Description = Description;
        edit.VerificationLevel = VerificationLevel;
        edit.DefaultMessageNotificationLevel = DefaultMessageNotification;
        edit.ExplicitContentFilterLevel = ExplicitContentFilterLevel;
        edit.GuildFeatures = GuildFeatures;
        edit.Name = Name;
        edit.OwnerId = OwnerId;
        edit.PreferredLocale = PreferredLocale;
        edit.PublicUpdatesChannelId = PublicUpdatesChannelId;
        edit.RulesChannelId = RulesChannelId;
        edit.SystemChannelFlags = SystemChannelFlags;
        edit.SystemChannelId = SystemChannelId;

        func(&edit);

        DiscordRestAPI::Patch("/guilds/" + Id.GetValueString(), edit.Serialize());
    }

    void DiscordGuild::AddMember(Snowflake id, const DiscordGuildMemberCreate& member)
    {
        DiscordRestAPI::Put("/guilds/" + Id.GetValueString() + "/members/" + id.GetValueString(), member.Serialize());
    }

    void DiscordGuild::BanUser(Snowflake id, std::optional<std::string> reason, std::optional<int> deleteMessageDays)
    {
        Json json;

        if (reason.has_value())
            json["reason"] = reason.value();

        if (deleteMessageDays.has_value())
            json["delete_message_days"] = deleteMessageDays.value();

        DiscordRestAPI::Put("/guilds/" + Id.GetValueString() + "/bans/" + id.GetValueString(), json);
    }

    void DiscordGuild::BanUser(const DiscordGuildMember& member, std::optional<std::string> reason, std::optional<int> deleteMessageDays)
    {
        BanUser(member.Id, reason, deleteMessageDays);
    }

    void DiscordGuild::BanUser(const Ptr<DiscordGuildMember> member, std::optional<std::string> reason, std::optional<int> deleteMessageDays)
    {
        BanUser(member->Id, reason, deleteMessageDays);
    }

    void DiscordGuild::KickMember(Snowflake id)
    {
        RemoveMember(id);
    }

    void DiscordGuild::KickMember(const DiscordGuildMember& member)
    {
        RemoveMember(member.Id);
    }

    void DiscordGuild::KickMember(const Ptr<DiscordGuildMember> member)
    {
        RemoveMember(member->Id);
    }

    void DiscordGuild::UnbanUser(Snowflake id)
    {
         DiscordRestAPI::Delete("/guilds/" + Id.GetValueString() + "/bans/" + id.GetValueString());
    }

    void DiscordGuild::UnbanUser(const DiscordUser& user)
    {
        UnbanUser(user.Id);
    }

    void DiscordGuild::UnbanUser(const Ptr<DiscordUser> user)
    {
        UnbanUser(user->Id);
    }

    void DiscordGuild::RemoveMember(Snowflake id)
    {
        DiscordRestAPI::Delete("/guilds/" + Id.GetValueString() + "/members/" + id.GetValueString());
    }

    void DiscordGuild::RemoveMember(const DiscordGuildMember& member)
    {
        RemoveMember(member.Id);
    }

    void DiscordGuild::RemoveMember(const Ptr<DiscordGuildMember> member)
    {
        RemoveMember(member->Id);
    }

    void DiscordGuild::EditMember(Snowflake id, const std::function<void(DiscordGuildMemberEdit*)>& func)
    {
        if (auto member = GetMemberById(id))
            EditMember(member, func);
    }

    void DiscordGuild::EditMember(const DiscordGuildMember& member, const std::function<void(DiscordGuildMemberEdit*)>& func)
    {
        EditMember(std::make_shared<DiscordGuildMember>(member), func);
    }

    void DiscordGuild::EditMember(const Ptr<DiscordGuildMember> member, const std::function<void(DiscordGuildMemberEdit*)>& func)
    {
        DiscordGuildMemberEdit edit;
        edit.Deaf = member->bIsDeaf;
        edit.Mute = member->bIsMuted;
        edit.Nickname = member->Nickname;
        edit.Roles = member->RoleIds;

        func(&edit);

        DiscordRestAPI::Patch("/guilds/" + Id.GetValueString() + "/members/" + member->Id.GetValueString(), edit.Serialize());
    }

    void DiscordGuild::EditCurrentNickname(const std::string& nickname)
    {
        Json json;
        json["nick"] = nickname;
        DiscordRestAPI::Patch("/guilds/" + Id.GetValueString() + "/members/@me/nick", json);
    }

    void DiscordGuild::AddMemberRole(Snowflake id, Snowflake roleId)
    {
        DiscordRestAPI::Put("/guilds/" + Id.GetValueString() + "/members/" + id.GetValueString() + "/roles/" + roleId.GetValueString(), {});
    }

    void DiscordGuild::AddMemberRole(const DiscordGuildMember& member, Snowflake roleId)
    {
        AddMemberRole(member.Id, roleId);
    }

    void DiscordGuild::AddMemberRole(const Ptr<DiscordGuildMember> member, Snowflake roleId)
    {
        AddMemberRole(member->Id, roleId);
    }

    void DiscordGuild::RemoveMemberRole(Snowflake id, Snowflake roleId)
    {
        DiscordRestAPI::Delete("/guilds/" + Id.GetValueString() + "/members/" + id.GetValueString() + "/roles/" + roleId.GetValueString());
    }

    void DiscordGuild::RemoveMemberRole(const DiscordGuildMember& member, Snowflake roleId)
    {
        RemoveMemberRole(member.Id, roleId);
    }

    void DiscordGuild::RemoveMemberRole(const Ptr<DiscordGuildMember> member, Snowflake roleId)
    {
        RemoveMemberRole(member->Id, roleId);
    }

    void DiscordGuild::CreateRole(const DiscordRoleInfo& role)
    {
        DiscordRestAPI::Post("/guilds/" + Id.GetValueString() + "/roles", role.Serialize());
    }

    void DiscordGuild::CreateEmoji(const DiscordEmojiCreate& emoji)
    {
        DiscordRestAPI::Post("/guilds/" + Id.GetValueString() + "/emojis", emoji.Serialize());
    }

    void DiscordGuild::CreateChannel(const DiscordChannelCreate& channel)
    {
        DiscordRestAPI::Post("/guilds/" + Id.GetValueString() + "/channels", channel.Serialize());
    }

    void DiscordGuild::EditRole(Snowflake id, const std::function<void(DiscordRoleInfo*)>& func)
    {
        EditRole(GetRoleById(id), func);
    }

    void DiscordGuild::EditRole(const DiscordRole& role, const std::function<void(DiscordRoleInfo*)>& func)
    {
        EditRole(std::make_shared<DiscordRole>(role), func);
    }

    void DiscordGuild::EditRole(const Ptr<DiscordRole> role, const std::function<void(DiscordRoleInfo*)>& func)
    {
        DiscordRoleInfo edit;
        edit.Name = role->Name;
        edit.Color = role->Color;
        edit.IsHoisted = role->bIsHoist;
        edit.Permission = role->Permissions;
        edit.IsMentionable = role->bIsMentionable;

        func(&edit);

        DiscordRestAPI::Patch("/guilds/" + Id.GetValueString() + "/roles/" + role->Id.GetValueString(), edit.Serialize());
    }

    void DiscordGuild::DeleteRole(Snowflake id)
    {
        DiscordRestAPI::Delete("/guilds/" + Id.GetValueString() + "/roles/" + id.GetValueString());
    }

    void DiscordGuild::DeleteRole(const DiscordRole& role)
    {
        DeleteRole(role.Id);
    }

    void DiscordGuild::DeleteRole(const Ptr<DiscordRole> role)
    {
        DeleteRole(role->Id);
    }

    void DiscordGuild::EditRolePosition(Snowflake id, int position)
    {
        Json json;
        json["id"] = id.GetValueString();
        json["position"] = position;
        DiscordRestAPI::Patch("/guilds/" + Id.GetValueString() + "/roles", json);
    }

    void DiscordGuild::EditRolePosition(const DiscordRole& role, int position)
    {
        EditRolePosition(role.Id, position);
    }

    void DiscordGuild::EditRolePosition(const Ptr<DiscordRole> role, int position)
    {
        EditRolePosition(role->Id, position);
    }

    void DiscordGuild::EditChannelPosition(Snowflake id, int position, std::optional<bool> lockPermissions, std::optional<Snowflake> parentId)
    {
        Json json;
        json["id"] = id.GetValueString();
        json["position"] = position;

        if (lockPermissions.has_value())
            json["lock_permissions"] = lockPermissions.value();

        if (parentId.has_value())
            json["parent_id"] = parentId.value().GetValueString();

        DiscordRestAPI::Patch("/guilds/" + Id.GetValueString() + "/channels", json);
    }

    void DiscordGuild::EditChannelPosition(const Ptr<DiscordChannel> channel, int position, std::optional<bool> lockPermissions, std::optional<Snowflake> parentId)
    {
        EditChannelPosition(channel->Id, position, lockPermissions, parentId);
    }

    void DiscordGuild::EditChannelPosition(const DiscordChannel& channel, int position, std::optional<bool> lockPermissions, std::optional<Snowflake> parentId)
    {
        EditChannelPosition(channel.Id, position, lockPermissions, parentId);
    }

    void DiscordGuild::EditEmoji(Snowflake id, const std::function<void(DiscordEmojiEdit*)>& func)
    {
        Ptr<DiscordEmoji> emoji = GetEmojiById(id);

        if (!emoji)
            return;

        DiscordEmojiEdit edit;
        edit.Name = emoji->Name;
        edit.RolesAllowed = emoji->RolesAllowed;

        func(&edit);

        DiscordRestAPI::Patch("/guild/" + Id.GetValueString() + "/emojis/" + id.GetValueString(), edit.Serialize());
    }

    void DiscordGuild::DeleteEmoji(Snowflake id)
    {
        if (auto iterator = Emojis.find(id); iterator != Emojis.end())
        {
            DiscordRestAPI::Delete("/guild/" + Id.GetValueString() + "/emojis/" + id.GetValueString());
        }
    }

    Ptr<DiscordEmoji> DiscordGuild::GetEmojiById(Snowflake id) const
    {
        if (auto iterator = Emojis.find(id); iterator != Emojis.end())
        {
            return iterator->second;
        }
        return nullptr;
    }

    Ptr<DiscordEmoji> DiscordGuild::GetEmojiByName(const std::string& name, bool caseSensitive) const
    {
        for (const auto& emoji : Emojis)
        {
            if (DiscordUtils::StringEqual(name, emoji.second->Name, caseSensitive))
            {
                return emoji.second;
            }
        }
        return nullptr;
    }

    Ptr<DiscordEmoji> DiscordGuild::GeEmojiByPredicate(const std::function<bool(Ptr<DiscordEmoji>)>& predicate) const
    {
        for (const auto& emoji : Emojis)
        {
            if (predicate(emoji.second))
            {
                return emoji.second;
            }
        }
        return nullptr;
    }

    Ptr<DiscordChannel> DiscordGuild::GetChannelById(Snowflake id) const
    {
        if (auto it = Channels.find(id); it != Channels.end())
        {
            return it->second;
        }
        return nullptr;
    }

    Ptr<DiscordChannel> DiscordGuild::GetChannelByName(const std::string& name, bool caseSensitive) const
    {
        for (const auto& channelPairs : Channels)
        {
            const auto& channel = channelPairs.second;
            
            if (auto gc = std::dynamic_pointer_cast<DiscordGuildChannel>(channel))
            {
                if (gc->GetName().size() != name.size())
                    continue;

                if (DiscordUtils::StringEqual(name, gc->GetName(), caseSensitive))
                    return channel;
            }
        }
        return nullptr;
    }

    Ptr<DiscordChannel> DiscordGuild::GetChannelByPredicate(const std::function<bool(Ptr<DiscordChannel>)>& predicate) const
    {
        for (const auto& channel : Channels)
        {
            if (predicate(channel.second))
            {
                return channel.second;
            }
        }
        return nullptr;
    }

    Ptr<DiscordPresence> DiscordGuild::GetUserPresenceById(Snowflake id) const
    {
        if (auto iterator = Presences.find(id); iterator != Presences.end())
        {
            return iterator->second;
        }
        return nullptr;
    }

    Ptr<DiscordPresence> DiscordGuild::GetUserPresenceByPredicate(const std::function<bool(Ptr<DiscordPresence>)> predicate) const
    {
        for (const auto& presence : Presences)
        {
            if (predicate(presence.second))
            {
                return presence.second;
            }
        }
        return nullptr;
    }

    Ptr<DiscordGuildMember> DiscordGuild::GetMemberById(Snowflake id)
    {
        if (auto iterator = Members.find(id); iterator != Members.end())
        {
            return iterator->second;
        }
        else
        {
            Json json;
            if (DiscordRestAPI::Get("/guilds/" + Id.GetValueString() + "/members/" + id.GetValueString(), &json))
            {
                Ptr<DiscordGuildMember> member(new DiscordGuildMember(json));
                Members.insert_or_assign(member->Id, member);
                return member;
            }
        }
        return nullptr;
    }

    Ptr<DiscordGuildMember> DiscordGuild::GetMemberByUserName(const std::string& name, bool caseSensitive)
    {
        for (const auto& member : Members)
        {
            if (DiscordUtils::StringEqual(name, member.second->Username, caseSensitive))
            {
                return member.second;
            }
        }

        return nullptr;
    }

    Ptr<DiscordGuildMember> DiscordGuild::GetMemberByPredicate(const std::function<bool(Ptr<DiscordGuildMember>)>& predicate)
    {
        for (const auto& member : Members)
        {
            if (predicate(member.second))
            {
                return member.second;
            }
        }
        return nullptr;
    }

    std::vector<DiscordGuildBan> DiscordGuild::GetBans()
    {
        std::vector<DiscordGuildBan> result;

        Json json;
        if (DiscordRestAPI::Get("/guilds/" + Id.GetValueString() + "/bans", &json))
        {
            for (const auto& object : json)
                result.emplace_back(DiscordGuildBan(object));
        }

        return result;
    }

    Ptr<DiscordGuildBan> DiscordGuild::GetBan(Snowflake id)
    {
        Ptr<DiscordGuildBan> ptr;

        Json json;
        if (DiscordRestAPI::Get("/guilds/" + Id.GetValueString() + "/bans/" + id.GetValueString(), &json))
        {
            ptr.reset(new DiscordGuildBan(json));
        }

        return ptr;
    }

    Ptr<DiscordWelcomeScreen> DiscordGuild::GetWelcomeScreen() const
    {
        if (WelcomeScreen)
            return WelcomeScreen;

        Json json;
        if (DiscordRestAPI::Get("/guilds/" + Id.GetValueString() + "/welcome-screen", &json))
        {
            return Ptr<DiscordWelcomeScreen>(new DiscordWelcomeScreen(json));
        }

        return nullptr;
    }

    DiscordVoiceState::DiscordVoiceState(const Json& json)
    {
        if (CHECK("guild_id"))
            GuildId = json["guild_id"].get<std::string>();

        if (CHECK("channel_id"))
            ChannelId = json["channel_id"].get<std::string>();
        
        if (CHECK("user_id"))
            UserId = json["user_id"].get<std::string>();

        if (CHECK("member"))
            Member = DiscordGuildMember(json["guild_member"]);

        if (CHECK("session_id"))
            SessionId = json["session_id"].get<std::string>();

        if (CHECK("deaf"))
            bIsDeaf = json["deaf"].get<bool>();

        if (CHECK("mute"))
            bIsMuted = json["mute"].get<bool>();

        if (CHECK("self_deaf"))
            bIsSelfDeaf = json["self_deaf"].get<bool>();

        if (CHECK("self_mute"))
            bIsSelfMuted = json["self_mute"].get<bool>();

        if (CHECK("self_video"))
            bIsSelfVideo = json["self_video"].get<bool>();

        if (CHECK("supress"))
            bIsSuppressed = json["suppress"].get<bool>();

        if (CHECK("self_stream"))
            bIsSelfStream = json["self_stream"].get<bool>();

        if (CHECK("request_to_speak_timestamp"))
            RequestToSpeakTimestamp = json["request_to_speak_timestamp"].get<std::string>();
    }

    DiscordPresence::DiscordPresence(const Json& json)
    {
        if (CHECK("user"))
            User = DiscordUser(json["user"]);

        if (CHECK("guild_id"))
            GuildId = json["guild_id"].get<std::string>();

        if (CHECK("status"))
        {
            std::string status = json["status"].get<std::string>();

            if (status == "dnd")
                Status = DiscordStatus::DO_NOT_DISTURB;
            else if (status == "idle")
                Status = DiscordStatus::IDLE;
            else if (status == "offline")
                Status = DiscordStatus::OFFLINE;
            else Status = DiscordStatus::ONLINE;
        }

        if (CHECK("activites"))
        {
            const auto& activites = json["activites"];
            for (const auto& object : activites)
            {
                DiscordActivity activity = DiscordActivity(object);
                Activities.emplace_back(activity);
            }
        }

        if (CHECK("client_status"))
            ClientStatus = Discord::DiscordClientStatus(json["client_status"]);
    }

    DiscordClientStatus::DiscordClientStatus(const Json& json)
    {
        if (CHECK("web"))
            Web = json["web"].get<std::string>();

        if (CHECK("mobile"))
            Mobile = json["mobile"].get<std::string>();

        if (CHECK("desktop"))
            Desktop = json["desktop"].get<std::string>();
    }

    DiscordWelcomeScreen::DiscordWelcomeScreen(const Json& json)
    {
        if (CHECK("description"))
            Description = json["description"].get<std::string>();

        if (CHECK("welcome_channels"))
        {
            const auto& channels = json["welcome_channels"];
            for (const auto& object : channels)
            {
                DiscordWelcomeScreenChannel channel = DiscordWelcomeScreenChannel(object);
                WelcomeChannels.emplace_back(channel);
            }
        }
    }

    DiscordWelcomeScreenChannel::DiscordWelcomeScreenChannel(const Json& json)
    {
        if (CHECK("channel_id"))
            ChannelId = json["channel_id"].get<std::string>();

        if (CHECK("description"))
            Description = json["description"].get<std::string>();

        if (CHECK("emoji_id"))
            EmojiId = json["emoji_id"].get<std::string>();

        if (CHECK("emoji_name"))
            EmojiName = json["emoji_name"].get<std::string>();
    }

    DiscordStageInstance::DiscordStageInstance(const Json& json)
    {
        if (CHECK("id"))
            Id = json["id"].get<std::string>();

        if (CHECK("guild_id"))
            GuildId = json["guild_id"].get<std::string>();

        if (CHECK("channel_id"))
            ChannelId = json["channel_id"].get<std::string>();

        if (CHECK("topic"))
            Topic = json["topic"].get<std::string>();

        if (CHECK("privacy_level"))
            DiscordPrivacyLevel = static_cast<Discord::DiscordPrivacyLevel>(json["privacy_level"].get<int>());

        if (CHECK("discoverable_disabled"))
            bDiscoverableDisabled = json["discoverable_disabled"].get<bool>();
    }

    Json DiscordGuildEdit::Serialize() const
    {
        Json json;
        json["name"] = Name;
        json["verification_level"] = static_cast<int>(VerificationLevel);
        json["default_message_notifications"] = static_cast<int>(DefaultMessageNotificationLevel);
        json["explicit_content_filter"] = static_cast<int>(ExplicitContentFilterLevel);
        json["afk_channel_id"] = AFKChannelId.GetValueString();
        json["afk_timeout"] = AFKTimeout;
        json["owner_id"] = OwnerId.GetValueString();
        json["system_channel_id"] = SystemChannelId.GetValueString();
        json["system_channel_flags"] = SystemChannelFlags;
        json["rules_channel_id"] = RulesChannelId.GetValueString();
        json["public_updates_channel_id"] = PublicUpdatesChannelId.GetValueString();
        json["preferred_locale"] = PreferredLocale;
        json["features"] = Json::array();

        for (const auto& feature : GuildFeatures)
            json["features"].emplace_back(feature);

        json["description"] = Description;

        return json;
    }

    Json DiscordGatewayPresence::Serialize() const
    {
        Json json;

        if (Since.has_value())
            json["since"] = Since.value();

        if (StatusText.has_value() && ActivityType.has_value())
        {
            json["game"] =
            {
                { "name", StatusText.value() },
                { "type", static_cast<int>(ActivityType.value()) }
            };
        }

        std::string status;
        switch (Status)
        {
            case DiscordStatus::DO_NOT_DISTURB: status = "dnd"; break;
            case DiscordStatus::IDLE: status = "idle"; break;
            case DiscordStatus::OFFLINE: status = "offline"; break;
            case DiscordStatus::ONLINE: status = "online"; break;
        }

        json["status"] = status;
        json["afk"] = IsAFK;

        return json;
    }
}