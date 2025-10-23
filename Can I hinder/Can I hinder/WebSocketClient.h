#pragma once
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <nlohmann/json.hpp>
#include <thread>
#include <functional>
#include <atomic>
#include <iostream>

class WebsocketClient {
   public:
       WebsocketClient();
	   ~WebsocketClient();

	   void connect(const std::string& host, const std::string& port);
	   void close();
	   void setOnMessage(std::function<void(const std::string&, const std::string&)> callback);

   private:
	   void run(const std::string& host, const std::string& port);
	   void readLoop();

	   std::function<void(const std::string&, const std::string&)> onMessage;
	   std::thread m_thread;
	   std::atomic<bool> m_running{ false };

	   boost::asio::io_context m_ioc;
	   boost::asio::ip::tcp::resolver m_resolver;

	   std::unique_ptr<boost::beast::websocket::stream<boost::asio::ip::tcp::socket>> m_ws;
};
