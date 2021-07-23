#include "DiscordEmbed.hpp"
#include <iostream>

namespace Discord
{
	DiscordEmbed& DiscordEmbed::PutTimestamp()
	{
		Timestamp = DiscordTimestamp::Now();

		return *this;
	}

	DiscordEmbed& DiscordEmbed::SetUrl(const std::string& url)
	{
		Url = url;

		return *this;
	}

	DiscordEmbed& DiscordEmbed::SetTitle(const std::string& title)
	{
		if (title.size() >= 256)
		{
#ifdef _DEBUG
			std::cout << "Embed error: Title more than 256 characters\n";
#endif
			return *this;
		}
		Title = title;

		return *this;
	}

	DiscordEmbed& DiscordEmbed::SetImageUrl(const std::string& url)
	{
		Image.Url = url;

		return *this;
	}

	DiscordEmbed& DiscordEmbed::SetVideoUrl(const std::string& url)
	{
		Video.Url = url;

		return *this;
	}

	DiscordEmbed& DiscordEmbed::SetDescription(const std::string& description)
	{
		if (description.size() >= 4096)
		{
#ifdef _DEBUG
			std::cout << "Embed error: Description more than 4096 characters\n";
#endif
			return *this;
		}
		Description = description;

		return *this;
	}

	DiscordEmbed& DiscordEmbed::SetThumbnailUrl(const std::string& thumbnailUrl)
	{
		Thumbnail.Url = thumbnailUrl;

		return *this;
	}

	DiscordEmbed& DiscordEmbed::SetColor(const DiscordColor& color)
	{
		Color = color;

		return *this;
	}

	DiscordEmbed& DiscordEmbed::PutRandomColor()
	{
		Color = DiscordColor(DiscordUtils::Random(255), DiscordUtils::Random(255), DiscordUtils::Random(255));

		return *this;
	}

	DiscordEmbed& DiscordEmbed::SetFooter(const std::string& text, const std::string& iconUrl)
	{
		Footer.Text = text;
		Footer.IconUrl = iconUrl;

		return *this;
	}

	DiscordEmbed& DiscordEmbed::SetAuthor(const std::string& name, const std::string& url, const std::string& iconUrl)
	{
		Author.Url = url;
		Author.Name = name;
		Author.IconUrl = iconUrl;

		return *this;
	}

	DiscordEmbed& DiscordEmbed::AddField(const std::string& name, const std::string& value, bool bInlined)
	{
		if (Fields.size() >= 25)
		{
#ifdef _DEBUG
			std::cout << "Embed error: Field size more than 25\n";
#endif
			return *this;
		}
		DiscordEmbedField field;
		field.Name = name;
		field.Value = value;
		field.bInline = bInlined;
		Fields.emplace_back(field);

		return *this;
	}

	Json DiscordEmbed::Serialize() const
	{
		size_t size = Title.size() + Description.size() + Footer.Text.size() + Author.Name.size();

		for (const auto& field : Fields)
			size += field.Name.size() + field.Value.size();

		if (size >= 6000)
		{
#ifdef _DEBUG
			std::cout << "Embed error: Total embed size more than 6000 characters\n";
#endif
			return "";
		}

		Json json;
		json["type"] = static_cast<int>(DiscordEmbedType::RICH);

		if (!Title.empty())
			json["title"] = Title;

		if (!Description.empty())
			json["description"] = Description;

		if (!Url.empty())
			json["url"] = Url;

		if (!Timestamp.Null())
			json["timestamp"] = Timestamp.GetISO8601String();

		if (!Color.Null())
			json["color"] = Color.ToInt32();

		if (!Footer.Text.empty())
			json["footer"] = Footer.Serialize();

		if (!Image.Url.empty())
			json["image"] = Image.Serialize();

		if (!Thumbnail.Url.empty())
			json["thumbnail"] = Thumbnail.Serialize();

		if (!Video.Url.empty())
			json["video"] = Video.Serialize();

		if (!Author.Name.empty())
			json["author"] = Author.Serialize();

		if (!Fields.empty())
		{
			auto fields = Json::array();

			for (auto& field : Fields)
			{
				if (!field.Name.empty() && !field.Value.empty())
				{
					fields.emplace_back(field.Serialize());
				}
			}
			json["fields"] = fields;
		}

		return json;
	}

	DiscordEmbed::DiscordEmbed(const Json& json)
	{
		if (CHECK("title"))
			Title = json["title"].get<std::string>();

		if (CHECK("type"))
		{
			std::string type = json["type"].get<std::string>();
			if (type == "rich")
				Type = DiscordEmbedType::RICH;
			else if (type == "image")
				Type = DiscordEmbedType::IMAGE;
			else if (type == "video")
				Type = DiscordEmbedType::VIDEO;
			else if (type == "gifv")
				Type = DiscordEmbedType::GIFV;
			else if (type == "article")
				Type = DiscordEmbedType::ARTICLE;
			else Type = DiscordEmbedType::LINK;
		}

		if (CHECK("description"))
			Description = json["description"].get<std::string>();

		if (CHECK("url"))
			Url = json["url"].get<std::string>();

		if (CHECK("timestamp"))
			Timestamp = json["timestamp"].get<std::string>();

		if (CHECK("color"))
			Color = json["color"].get<int>();

		if (CHECK("footer"))
			Footer = DiscordEmbedFooter(json["footer"]);
		
		if (CHECK("image"))
			Image = DiscordEmbedMedia(json["image"]);

		if (CHECK("thumbnail"))
			Thumbnail = DiscordEmbedMedia(json["thumbnail"]);

		if (CHECK("video"))
			Video = DiscordEmbedMedia(json["video"]);

		if (CHECK("author"))
			Author = DiscordEmbedAuthor(json["author"]);

		if (CHECK("fields"))
		{
			const auto& fields = json["fields"];
			for (const auto& object : fields)
			{
				DiscordEmbedField field = DiscordEmbedField(object);
				Fields.emplace_back(field);
			}
		}
	}

	Json DiscordEmbedFooter::Serialize() const
	{
		Json json;
		json["text"] = Text;

		if (!IconUrl.empty())
			json["icon_url"] = IconUrl;

		if (!ProxyIconUrl.empty())
			json["proxy_icon_url"] = ProxyIconUrl;

		return json;
	}

	DiscordEmbedFooter::DiscordEmbedFooter(const Json& json)
	{
		if (CHECK("text"))
			Text = json["text"].get<std::string>();

		if (CHECK("icon_url"))
			IconUrl = json["icon_url"].get<std::string>();

		if (CHECK("proxy_icon_url"))
			ProxyIconUrl = json["proxy_icon_url"].get<std::string>();
	}

	Json DiscordEmbedField::Serialize() const
	{
		Json json;
		json["name"] = Name;
		json["value"] = Value;
		json["inline"] = bInline;
		return json;
	}

	DiscordEmbedField::DiscordEmbedField(const Json& json)
	{
		if (CHECK("name"))
			Name = json["name"].get<std::string>();

		if (CHECK("value"))
			Value = json["value"].get<std::string>();

		if (CHECK("inline"))
			bInline = json["inline"].get<bool>();
	}

	Json DiscordEmbedAuthor::Serialize() const
	{
		Json json;
		json["name"] = Name;

		if (!Url.empty())
			json["url"] = Url;

		if (!IconUrl.empty())
			json["icon_url"] = IconUrl;
		
		if (!ProxyIconUrl.empty())
			json["proxy_icon_url"] = ProxyIconUrl;

		return json;
	}

	DiscordEmbedAuthor::DiscordEmbedAuthor(const Json& json)
	{
		if (CHECK("name"))
			Name = json["name"].get<std::string>();

		if (CHECK("url"))
			Url = json["url"].get<std::string>();

		if (CHECK("icon_url"))
			IconUrl = json["icon_url"].get<std::string>();

		if (CHECK("proxy_icon_url"))
			ProxyIconUrl = json["proxy_icon_url"].get<std::string>();
	}

	Json DiscordEmbedMedia::Serialize() const
	{
		Json json;
		json["url"] = Url;

		if (!ProxyUrl.empty())
			json["proxy_url"] = ProxyUrl;

		if (Width)
			json["width"] = Width;
		
		if (Height)
			json["height"] = Height;

		return json;
	}

	DiscordEmbedMedia::DiscordEmbedMedia(const Json& json)
	{
		if (CHECK("url"))
			Url = json["url"].get<std::string>();

		if (CHECK("proxy_url"))
			ProxyUrl = json["proxy_url"].get<std::string>();

		if (CHECK("height"))
			Height = json["height"].get<int>();

		if (CHECK("width"))
			Width = json["width"].get<int>();
	}
}