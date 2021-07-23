#include "DiscordWebSocket.hpp"

namespace Discord
{
    std::mutex GMutex;

	void DiscordWebSocket::Initialize()
	{
#if defined(DISCORD_LOG) || defined(_DEBUG)
        WSClient.set_access_channels(websocketpp::log::alevel::connect);
        WSClient.set_access_channels(websocketpp::log::alevel::disconnect);
#else
        WSClient.set_access_channels(websocketpp::log::alevel::none);
#endif

        WSClient.set_tls_init_handler([](websocketpp::connection_hdl)
        {
            return websocketpp::lib::make_shared<websocketpp::lib::asio::ssl::context>(websocketpp::lib::asio::ssl::context::tls);
        });

        WSClient.set_fail_handler([&](WebSocketConnectionHDL hdl)
        {
            FailCallback();
        });

        WSClient.set_close_handler([&](WebSocketConnectionHDL hdl)
        {
            CloseCallback(WSConnection->get_remote_close_code(), WSConnection->get_remote_close_reason());
        });

        WSClient.set_message_handler([&](WebSocketConnectionHDL hdl, WebSocketMessage message)
        {
            std::thread t(MessageCallback, message);
            t.detach();
        });

        WSClient.init_asio();
	}

    void DiscordWebSocket::Stop(const std::string& reason)
    {
        WSClient.close(WSConnection, websocketpp::close::status::normal, reason);
    }

    void DiscordWebSocket::Run()
    {
        websocketpp::lib::error_code ec;
        WSConnection = WSClient.get_connection("wss://gateway.discord.gg/?encoding=json&v=6", ec);

        if (ec)
        {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
            return;
        }

        WSClient.connect(WSConnection);
        WSClient.run();
    }

    void DiscordWebSocket::Send(const std::string& msg)
    {
        WSClient.send(WSConnection, msg, websocketpp::frame::opcode::text);
    }

    void DiscordWebSocket::SetFailedHandler(const std::function<void()>& callback)
    {
        FailCallback = callback;
    }

    void DiscordWebSocket::SetClosedHandler(const std::function<void(short, const std::string&)>& callback)
    {
        CloseCallback = callback;
    }

    void DiscordWebSocket::SetMessageHandler(const std::function<void(WebSocketMessage)>& callback)
    {
        MessageCallback = callback;
    }
}
