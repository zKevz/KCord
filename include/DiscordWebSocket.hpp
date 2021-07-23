#pragma once
#include "Json.hpp"
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_client.hpp>

namespace Discord
{
	static std::string DiscordGatewayURL = "wss://gateway.discord.gg/?v=6&encoding=json";

	enum class DiscordWebSocketCloseCode
	{
		UNKNOWN_ERROR = 4000,
		UNKNOWN_OPCODE = 4001,
		DECODE_ERROR = 4002,
		NOT_AUTHENTICATED = 4003,
		AUTHENTICATION_FAILED = 4004,
		ALREADY_AUTHENTICATED = 4005,
		INVALID_SEQUENCE = 4007,
		RATE_LIMITED = 4008,
		SESSION_TIMED_OUT = 4009,
		INVALID_SHARD = 4010,
		SHARDING_REQUIRED = 4011,
		INVALID_API_VERSION = 4012,
		INVALID_INTENTS = 4013,
		DISALLOWED_INTENTS = 4014,
	};

	class DiscordWebSocket
	{
	public:
		using WebSocketClient = websocketpp::client<websocketpp::config::asio_tls_client>;
		using WebSocketContext = Ptr<boost::asio::ssl::context>;
		using WebSocketMessage = websocketpp::config::asio_client::message_type::ptr;
		using WebSocketConnection = websocketpp::connection<websocketpp::config::asio_tls_client>;
		using WebSocketConnectionHDL = websocketpp::connection_hdl;

		void Run();
		void Initialize();
		void Stop(const std::string& reason);

		void Send(const std::string& msg);
		void SetFailedHandler(const std::function<void()>& callback);
		void SetMessageHandler(const std::function<void(WebSocketMessage)>& callback);
		void SetClosedHandler(const std::function<void(short, const std::string&)>& callback);
	private:
		
		WebSocketClient WSClient;
		Ptr<WebSocketConnection> WSConnection;

		std::function<void()> FailCallback;
		std::function<void(WebSocketMessage)> MessageCallback;
		std::function<void(short, const std::string&)> CloseCallback;
	};
}