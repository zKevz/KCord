#pragma once
#include <string>
#include <vector>

#include "Json.hpp"
#include "DiscordUtils.hpp"
#include "DiscordColor.hpp"
#include "DiscordTimestamp.hpp"

namespace Discord
{
	enum class DiscordEmbedType
	{
		RICH,
		IMAGE,
		VIDEO,
		GIFV,
		ARTICLE,
		LINK
	};

	class DiscordEmbedFooter
	{
	public:
		Json Serialize() const;

		std::string GetText() const { return Text; }
		std::string GetIconUrl() const { return IconUrl; }
		std::string GetProxyIconUrl() const { return ProxyIconUrl; }
	private:
		std::string Text;
		std::string IconUrl;
		std::string ProxyIconUrl;

		DiscordEmbedFooter() {}
		DiscordEmbedFooter(const Json& json);

		friend class DiscordEmbed;
	};

	class DiscordEmbedMedia
	{
	public:
		Json Serialize() const;

		std::string GetUrl() const { return Url; }
		std::string GetProxyUrl() const { return ProxyUrl; }
		
		int GetWidth() const { return Width; }
		int GetHeight() const { return Height; }
	private:
		std::string Url;
		std::string ProxyUrl;

		int Width = 0;
		int Height = 0;

		DiscordEmbedMedia() {}
		DiscordEmbedMedia(const Json& json);

		friend class DiscordEmbed;
	};

	class DiscordEmbedAuthor
	{
	public:
		Json Serialize() const;
		std::string GetUrl() const { return Url; }
		std::string GetName() const { return Name; }
		std::string GetIconUrl() const { return IconUrl; }
		std::string GetProxyIconUrl() const { return ProxyIconUrl; }
	private:
		std::string Url;
		std::string Name;
		std::string IconUrl;
		std::string ProxyIconUrl;

		DiscordEmbedAuthor() {}
		DiscordEmbedAuthor(const Json& json);

		friend class DiscordEmbed;
	};

	class DiscordEmbedField
	{
	public:
		Json Serialize() const;
		bool IsInlined() const { return bInline; }
		std::string GetName() const { return Name; }
		std::string GetValue() const { return Value; }
	private:
		bool bInline = false;
		std::string Name;
		std::string Value;

		DiscordEmbedField() {}
		DiscordEmbedField(const Json& json);

		friend class DiscordEmbed;
	};

	class DiscordEmbed
	{
	public:
		DiscordEmbed& PutTimestamp();
		DiscordEmbed& PutRandomColor();

		DiscordEmbed& SetUrl(const std::string& url);
		DiscordEmbed& SetTitle(const std::string& title);
		DiscordEmbed& SetImageUrl(const std::string& url);
		DiscordEmbed& SetVideoUrl(const std::string& url);
		DiscordEmbed& SetDescription(const std::string& description);
		DiscordEmbed& SetThumbnailUrl(const std::string& thumbnailUrl);

		DiscordEmbed& SetColor(const DiscordColor& color);

		DiscordEmbed& SetFooter(const std::string& text, const std::string& iconUrl = "");
		DiscordEmbed& SetAuthor(const std::string& name, const std::string& url = "", const std::string& iconUrl = "");

		DiscordEmbed& AddField(const std::string& name, const std::string& value, bool bInlined = false);

		Json Serialize() const;

		DiscordEmbed() {}
	private:
		std::string Url;
		std::string Title;
		std::string Description;

		std::vector<DiscordEmbedField> Fields;

		DiscordEmbedMedia Image;
		DiscordEmbedMedia Video;
		DiscordEmbedMedia Thumbnail;

		DiscordEmbedAuthor Author;

		DiscordEmbedFooter Footer;

		DiscordColor Color;
		DiscordTimestamp Timestamp;
		DiscordEmbedType Type = DiscordEmbedType::RICH;

		DiscordEmbed(const Json& json);

		friend class DiscordMessage;
	};
}