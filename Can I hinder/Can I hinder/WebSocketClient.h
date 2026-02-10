#pragma once
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/ssl.hpp>
#include <nlohmann/json.hpp>
#include <openssl/ssl.h>

#include <thread>
#include <functional>
#include <atomic>
#include <iostream>
namespace beast = boost::beast;
using json = nlohmann::json; // for JSON parsing
namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;
namespace websocket = beast::websocket; // for WebSocket protocol

class WebsocketClient {
   public:
       WebsocketClient();
	   ~WebsocketClient();

	   void connect(const std::string& host, const std::string& port, bool useTls);
	   void close();
	   void setOnMessage(std::function<void(const std::string&, const std::string&)> callback);

   private:

	   void run(const std::string& host, const std::string& port, bool useTls);
	   void readLoop(bool useTls);
	   // Callback function to handle incoming messages, takes action and effect as parameters
	   std::function<void(const std::string&, const std::string&)> onMessage;
	   // Background thread for running the WebSocket client
	   std::thread m_thread;
	   //flag so all threads know when to stop
	   std::atomic<bool> m_running{ false };

	   boost::asio::io_context m_ioc;
	   boost::asio::ip::tcp::resolver m_resolver;
	   // For non-secure WebSocket (ws://) local testing
	   std::unique_ptr<websocket::stream<tcp::socket>>m_ws;
	   // For secure WebSocket (wss://) Azure hosting
	   std::unique_ptr< boost::beast::websocket::stream< boost::beast::ssl_stream<boost::asio::ip::tcp::socket>> > m_wss;
	   // SSL certificate verification for secure WebSocket
	   boost::asio::ssl::context m_ssl_ctx{ boost::asio::ssl::context::tlsv12_client };
};
