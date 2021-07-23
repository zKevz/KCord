#pragma once
#include <string>
#include <optional>

#include "Snowflake.hpp"
#include "DiscordUser.hpp"
#include "DiscordEmoji.hpp"
#include "DiscordChannel.hpp"
#include "DiscordActivity.hpp"
#include "DiscordGuildBan.hpp"
#include "DiscordGuildMember.hpp"

#include "DiscordDMChannel.hpp"
#include "DiscordChannelCategory.hpp"
#include "DiscordGroupDMChannel.hpp"
#include "DiscordNewsChannel.hpp"
#include "DiscordStoreChannel.hpp"
#include "DiscordTextChannel.hpp"
#include "DiscordVoiceChannel.hpp"
#include "DiscordGuildChannel.hpp"

namespace Discord
{
	enum class DiscordVerificationLevel
	{
		NONE,
		LOW,
		MEDIUM,
		HIGH,
		VERY_HIGH
	};

	enum class DiscordMessageNotificationLevel
	{
		ALL_MESSAGES,
		ONLY_MENTIONS
	};

	enum class DiscordExplicitContentFilterLevel
	{
		DISABLED,
		MEMBERS_WITHOUT_ROLES,
		ALL_MEMBERS
	};

	enum class DiscordMFALevel
	{
		NONE,
		ELEVATED
	};

	enum class DiscordPrivacyLevel
	{
		PUBLIC  = 1,
		GUILD_ONLY = 2
	};

	enum class DiscordNSFWLevel
	{
		DEFAULT,
		EXPLICIT,
		SAFE,
		AGE_RESTRICTED
	};

	enum class DiscordPremiumTier
	{
		NONE,
		TIER_1,
		TIER_2,
		TIER_3,
	};

	enum DiscordSystemChannelFlags
	{
		SUPPRESS_JOIN_NOTIFICATIONS = 1 << 0,
		SUPPRESS_PREMIUM_SUBSCRIPTIONS = 1 << 1,
		SUPPRESS_GUILD_REMINDER_NOTIFICATIONS = 1 << 2
	};

	class DiscordVoiceState
	{
	public:
		Snowflake GetGuildId() const { return GuildId; }
		Snowflake GetChannelId() const { return ChannelId; }
		Snowflake GetUserId() const { return UserId; }

		bool IsDeaf() const { return bIsDeaf; }
		bool IsMuted() const { return bIsMuted; }
		bool IsSelfDeaf() const { return bIsSelfDeaf; }
		bool IsSelfMuted() const { return bIsSelfMuted; }
		bool IsSelfStream() const { return bIsSelfStream; }
		bool IsSelfVideo() const { return bIsSelfVideo; }
		bool IsSuppressed() const { return bIsSuppressed; }

		std::string GetSessionId() const { return SessionId; }
		DiscordTimestamp GetRequestToSpeakTimestamp() const { return RequestToSpeakTimestamp; }

		DiscordGuildMember& GetMember() { return Member; }
	private:
		Snowflake GuildId;
		Snowflake ChannelId;
		Snowflake UserId;
		DiscordGuildMember Member;

		bool bIsDeaf = false;
		bool bIsMuted = false;
		bool bIsSelfDeaf = false;
		bool bIsSelfMuted = false;
		bool bIsSelfStream = false;
		bool bIsSelfVideo = false;
		bool bIsSuppressed = false;

		std::string SessionId;
		DiscordTimestamp RequestToSpeakTimestamp;

		DiscordVoiceState() {}
		DiscordVoiceState(const Json& json);

		friend class DiscordGuild;
	};

	class DiscordClientStatus
	{
	public:
		std::string GetWeb() const { return Web; }
		std::string GetMobile() const { return Mobile; }
		std::string GetDesktop() const { return Desktop; }
	private:
		std::string Web;
		std::string Mobile;
		std::string Desktop;

		DiscordClientStatus() {}
		DiscordClientStatus(const Json& json);

		friend class DiscordPresence;
	};

	class DiscordGatewayPresence
	{
	public:
		std::optional<int> Since = 0;
		std::optional<std::string> StatusText;
		std::optional<DiscordActivityType> ActivityType;

		bool IsAFK = false;
		DiscordStatus Status = DiscordStatus::ONLINE;

		Json Serialize() const;
	};

	class DiscordPresence
	{
	public:
		Snowflake GetGuildId() const { return GuildId; }
		DiscordStatus GetStatus() const { return Status; }
		DiscordClientStatus GetClientStatus() const { return ClientStatus; }
		const std::vector<DiscordActivity>& GetActivities() const { return Activities; }

		DiscordUser& GetUser() { return User; }
	private:
		Snowflake GuildId;
		DiscordUser User;
		DiscordStatus Status = DiscordStatus::ONLINE;
		DiscordClientStatus ClientStatus;
		std::vector<DiscordActivity> Activities;

		DiscordPresence() {}
		DiscordPresence(const Json& json);

		friend class DiscordGuild;
		friend class DiscordClient;
	};

	class DiscordWelcomeScreenChannel
	{
	public:
		Snowflake GetEmojiId() const { return EmojiId; }

		std::string GetEmojiName() const { return EmojiName; }
		std::string GetChannelId() const { return ChannelId; }
		std::string GetDescription() const { return Description; }
	private:
		Snowflake EmojiId;
		std::string EmojiName;
		std::string ChannelId;
		std::string Description;

		DiscordWelcomeScreenChannel() {}
		DiscordWelcomeScreenChannel(const Json& json);

		friend class DiscordWelcomeScreen;
	};

	class DiscordWelcomeScreen
	{
	public:
		std::string GetDescription() const { return Description; }
		const std::vector<DiscordWelcomeScreenChannel>& GetWelcomeChannels() const { return WelcomeChannels; }
	private:
		std::string Description;
		std::vector<DiscordWelcomeScreenChannel> WelcomeChannels;

		DiscordWelcomeScreen() {}
		DiscordWelcomeScreen(const Json& json);

		friend class DiscordGuild;
	};

	class DiscordStageInstance
	{
	public:
		Snowflake GetId() const { return Id; }
		Snowflake GetGuildId() const { return GuildId; }
		Snowflake GetChannelId() const { return ChannelId; }
		DiscordPrivacyLevel GetPrivacyLevel() const { return DiscordPrivacyLevel; }

		std::string GetTopic() const { return Topic; }
		bool IsDiscoverableDisabled() const { return bDiscoverableDisabled; }
	private:
		Snowflake Id;
		Snowflake GuildId;
		Snowflake ChannelId;
		DiscordPrivacyLevel DiscordPrivacyLevel = DiscordPrivacyLevel::PUBLIC;

		bool bDiscoverableDisabled = false;

		std::string Topic;

		DiscordStageInstance() {}
		DiscordStageInstance(const Json& json);

		friend class DiscordGuild;
		friend class DiscordInvite;
		friend class DiscordClient;
	};

	class DiscordGuildEdit
	{
	public:
		Snowflake OwnerId;
		Snowflake AFKChannelId;
		Snowflake RulesChannelId;
		Snowflake SystemChannelId;
		Snowflake PublicUpdatesChannelId;

		int AFKTimeout = 0;
		int SystemChannelFlags = 0;

		std::string Name;
		std::string Description;
		std::string PreferredLocale;
		std::vector<std::string> GuildFeatures;
		
		DiscordVerificationLevel VerificationLevel = DiscordVerificationLevel::NONE;
		DiscordExplicitContentFilterLevel ExplicitContentFilterLevel = DiscordExplicitContentFilterLevel::DISABLED;
		DiscordMessageNotificationLevel DefaultMessageNotificationLevel = DiscordMessageNotificationLevel::ALL_MESSAGES;

		Json Serialize() const;
	};

	class DiscordClient;
	class DiscordGuild
	{
	public:
		void Edit(const std::function<void(DiscordGuildEdit*)>& func);
		void EditCurrentNickname(const std::string& nickname);

		void AddMember(Snowflake id, const DiscordGuildMemberCreate& member);

		void BanUser(Snowflake id, std::optional<std::string> reason = {}, std::optional<int> deleteMessageDays = {});
		void BanUser(const DiscordGuildMember& member, std::optional<std::string> reason = {}, std::optional<int> deleteMessageDays = {});
		void BanUser(const Ptr<DiscordGuildMember> member, std::optional<std::string> reason = {}, std::optional<int> deleteMessageDays = {});

		void KickMember(Snowflake id);
		void KickMember(const DiscordGuildMember& member);
		void KickMember(const Ptr<DiscordGuildMember> member);

		void UnbanUser(Snowflake id);
		void UnbanUser(const DiscordUser& user);
		void UnbanUser(const Ptr<DiscordUser> user);

		void RemoveMember(Snowflake id);
		void RemoveMember(const DiscordGuildMember& member);
		void RemoveMember(const Ptr<DiscordGuildMember> member);

		void EditMember(Snowflake id, const std::function<void(DiscordGuildMemberEdit*)>& func);
		void EditMember(const DiscordGuildMember& member, const std::function<void(DiscordGuildMemberEdit*)>& func);
		void EditMember(const Ptr<DiscordGuildMember> member, const std::function<void(DiscordGuildMemberEdit*)>& func);

		void AddMemberRole(Snowflake id, Snowflake roleId);
		void AddMemberRole(const DiscordGuildMember& member, Snowflake roleId);
		void AddMemberRole(const Ptr<DiscordGuildMember> member, Snowflake roleId);

		void RemoveMemberRole(Snowflake id, Snowflake roleId);
		void RemoveMemberRole(const DiscordGuildMember& member, Snowflake roleId);
		void RemoveMemberRole(const Ptr<DiscordGuildMember> member, Snowflake roleId);

		void CreateRole(const DiscordRoleInfo& role);
		void CreateChannel(const DiscordChannelCreate& channel);

		void EditRole(Snowflake id, const std::function<void(DiscordRoleInfo*)>& func);
		void EditRole(const DiscordRole& role, const std::function<void(DiscordRoleInfo*)>& func);
		void EditRole(const Ptr<DiscordRole> role, const std::function<void(DiscordRoleInfo*)>& func);

		void DeleteRole(Snowflake id);
		void DeleteRole(const DiscordRole& role);
		void DeleteRole(const Ptr<DiscordRole> role);

		void EditRolePosition(Snowflake id, int position);
		void EditRolePosition(const DiscordRole& role, int position);
		void EditRolePosition(const Ptr<DiscordRole> role, int position);

		void EditChannelPosition(Snowflake id, int position, std::optional<bool> lockPermissions = {}, std::optional<Snowflake> parentId = {});
		void EditChannelPosition(const DiscordChannel& channel, int position, std::optional<bool> lockPermissions = {}, std::optional<Snowflake> parentId = {});
		void EditChannelPosition(const Ptr<DiscordChannel> channel, int position, std::optional<bool> lockPermissions = {}, std::optional<Snowflake> parentId = {});

		void EditEmoji(Snowflake id, const std::function<void(DiscordEmojiEdit*)>& func);
		void DeleteEmoji(Snowflake id);

		Ptr<DiscordRole> GetRoleById(Snowflake id) const;
		Ptr<DiscordRole> GetRoleByName(const std::string& name, bool caseSensitive = true) const;
		Ptr<DiscordRole> GetRoleByPredicate(const std::function<bool(Ptr<DiscordRole>)>& predicate) const;

		Ptr<DiscordEmoji> GetEmojiById(Snowflake id) const;
		Ptr<DiscordEmoji> GetEmojiByName(const std::string& name, bool caseSensitive = true) const;
		Ptr<DiscordEmoji> GeEmojiByPredicate(const std::function<bool(Ptr<DiscordEmoji>)>& predicate) const;

		Ptr<DiscordChannel> GetChannelById(Snowflake id) const;
		Ptr<DiscordChannel> GetChannelByName(const std::string& name, bool caseSensitive = true) const;
		Ptr<DiscordChannel> GetChannelByPredicate(const std::function<bool(Ptr<DiscordChannel>)>& predicate) const;

		Ptr<DiscordPresence> GetUserPresenceById(Snowflake id) const;
		Ptr<DiscordPresence> GetUserPresenceByPredicate(const std::function<bool(Ptr<DiscordPresence>)> predicate) const;

		Ptr<DiscordGuildMember> GetMemberById(Snowflake id);
		Ptr<DiscordGuildMember> GetMemberByUserName(const std::string& name, bool caseSensitive = true);
		Ptr<DiscordGuildMember> GetMemberByPredicate(const std::function<bool(Ptr<DiscordGuildMember>)>& predicate);

		std::vector<DiscordGuildBan> GetBans();
		Ptr<DiscordGuildBan> GetBan(Snowflake id);
		Ptr<DiscordWelcomeScreen> GetWelcomeScreen() const;

		Snowflake GetId() const { return Id; }
		Snowflake GetOwnerId() const { return OwnerId; }
		Snowflake GetAfkChannelId() const { return AFKChannelId; }
		Snowflake GetWidgetChannelId() const { return WidgetChannelId; }
		Snowflake GetApplicationId() const { return ApplicationId; }
		Snowflake GetSystemChannelId() const { return SystemChannelId; }
		Snowflake GetRulesChannelId() const { return RulesChannelId; }
		Snowflake GetPublicUpdatesChannelId() const { return PublicUpdatesChannelId; }

		const DiscordTimestamp& GetJoinedAtTimestamp() const { return JoinedAt; }

		DiscordMFALevel GetMFALevel() const { return MFALevel; }
		DiscordNSFWLevel GetNSFWLevel() const { return NSFWLevel; }
		DiscordPremiumTier GetPremiumTierType() const { return PremiumTier; }
		DiscordVerificationLevel GetVerificationLevel() const { return VerificationLevel; }
		DiscordMessageNotificationLevel GetMessageNotificationLevel() const { return DefaultMessageNotification; }
		DiscordExplicitContentFilterLevel GetExplicitContentFilterLevel() const { return ExplicitContentFilterLevel; }

		std::string GetName() const { return Name; }
		std::string GetIconHash() const { return IconHash; }
		std::string GetSplashString() const { return Splash; }
		std::string GetDiscoverySplashString() const { return DiscoverySplash; }
		std::string GetVanityUrlCode() const { return VanityUrlCode; }
		std::string GetDescription() const { return Description; }
		std::string GetBannerHash() const { return BannerHash; }
		std::string GetPrefferedLocalLanguage() const { return PreferredLocale; }
		std::string GetPermissionsString() const { return Permissions; }

		const std::vector<std::string>& GetGuildStringFeatures() const { return GuildFeatures; }
		const std::vector<DiscordVoiceState>& GetVoiceStates() const { return VoiceStates; }
		const std::vector<DiscordStageInstance>& GetStageInstances() const { return StageInstances; }

		const std::map<Snowflake, Ptr<DiscordRole>>& GetRoles() const { return Roles; }
		const std::map<Snowflake, Ptr<DiscordEmoji>>& GetEmojis() const { return Emojis; }
		const std::map<Snowflake, Ptr<DiscordChannel>>& GetChannels() const { return Channels; }
		const std::map<Snowflake, Ptr<DiscordPresence>>& GetPresences() const { return Presences; }
		const std::map<Snowflake, Ptr<DiscordGuildMember>>& GetMembers() const { return Members; }

		int GetAFKTimeout() const { return AFKTimeout; }
		int GetMaxMembers() const { return MaxMembers; }
		int GetMemberCount() const { return MemberCount; }
		int GetMaxPresences() const { return MaxPresences; }
		int GetDiscordSystemChannelFlags() const { return SystemChannelFlags; }
		int GetMaxVideoChannelUsers() const { return MaxVideoChannelUsers; }
		int GetApproximateMemberCount() const { return ApproximateMemberCount; }
		int GetApproximatePresenceCount() const { return ApproximatePresenceCount; }
		int GetPremiumSubscriptionCount() const { return PremiumSubscriptionCount; }

		bool IsOwner() const { return bIsOwner; }
		bool IsWidgetEnabled() const { return bIsWidgetEnabled; }
		bool IsLarge() const { return bIsLarge; }
		bool IsUnavailable() const { return bIsUnavailable; }
	private:
		Snowflake Id;
		Snowflake OwnerId;
		Snowflake AFKChannelId;
		Snowflake WidgetChannelId;
		Snowflake ApplicationId;
		Snowflake SystemChannelId;
		Snowflake RulesChannelId;
		Snowflake PublicUpdatesChannelId;

		DiscordClient* Client = nullptr;
		DiscordTimestamp JoinedAt;

		DiscordMFALevel MFALevel = DiscordMFALevel::NONE;
		DiscordNSFWLevel NSFWLevel = DiscordNSFWLevel::DEFAULT;
		DiscordPremiumTier PremiumTier = DiscordPremiumTier::NONE;
		DiscordVerificationLevel VerificationLevel = DiscordVerificationLevel::NONE;
		DiscordMessageNotificationLevel DefaultMessageNotification = DiscordMessageNotificationLevel::ALL_MESSAGES;
		DiscordExplicitContentFilterLevel ExplicitContentFilterLevel = DiscordExplicitContentFilterLevel::DISABLED;

		std::string Name;
		std::string Icon;
		std::string IconHash;
		std::string Splash;
		std::string DiscoverySplash;
		std::string VanityUrlCode;
		std::string Description;
		std::string BannerHash;
		std::string PreferredLocale;
		std::string Permissions;

		std::vector<std::string> GuildFeatures;
		std::vector<DiscordVoiceState> VoiceStates;
		std::vector<DiscordStageInstance> StageInstances;
		Ptr<DiscordWelcomeScreen> WelcomeScreen;

		std::map<Snowflake, Ptr<DiscordRole>> Roles;
		std::map<Snowflake, Ptr<DiscordEmoji>> Emojis;
		std::map<Snowflake, Ptr<DiscordChannel>> Channels;
		std::map<Snowflake, Ptr<DiscordPresence>> Presences;
		std::map<Snowflake, Ptr<DiscordGuildMember>> Members;

		int AFKTimeout = 0;
		int MaxMembers = 0;
		int MemberCount = 0;
		int MaxPresences = 0;
		int SystemChannelFlags = 0;
		int MaxVideoChannelUsers = 0;
		int ApproximateMemberCount = 0;
		int ApproximatePresenceCount = 0;
		int PremiumSubscriptionCount = 0;

		bool bIsOwner = false;
		bool bIsWidgetEnabled = false;
		bool bIsLarge = false;
		bool bIsUnavailable = false;

		DiscordGuild() {}
		DiscordGuild(DiscordClient* client, const Json& json);

		friend class DiscordClient;
		friend class DiscordChannel;
		friend class DiscordGuildChannel;
		friend class DiscordCommandContext;
	};
}