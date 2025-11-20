#include "WebSocketClient.h"


using json = nlohmann::json; // for JSON parsing
using tcp = boost::asio::ip::tcp; // for TCP sockets
namespace websocket = boost::beast::websocket; // for WebSocket protocol

WebsocketClient::WebsocketClient() : m_resolver(m_ioc) // initialize DNS resolver with io_context (event loop) so it can resolve hostnames

{
}

WebsocketClient::~WebsocketClient()
{
	close();
}

void WebsocketClient::connect(const std::string& host, const std::string& port)
{
	m_running = true;
	// start the background thread so not to interrupt game loop
	m_thread = std::thread(&WebsocketClient::run, this, host, port);
}
// stops the background thread
void WebsocketClient::close()
{
	m_running = false;
	if (m_ws) {
		boost::system::error_code ec;
		m_ws->close(websocket::close_code::normal, ec);

	}
	if (m_thread.joinable()) {
		m_thread.join();
	}
}

void WebsocketClient::setOnMessage(std::function<void(const std::string&,const std::string&)> callback)
{
	onMessage = callback; // this is so it'll define what happens when a message is recieved (spawning an eneny)
}
// runs in background thread
void WebsocketClient::run(const std::string& host, const std::string& port)
{
	try {
		auto const results = m_resolver.resolve(host, port);
		m_ws = std::make_unique<websocket::stream<tcp::socket>>(m_ioc);
		
		boost::asio::connect(m_ws->next_layer(), results.begin(), results.end()); // connect the underlying TCP socket
		m_ws->handshake(host, "/"); // perform the WebSocket handshake with the server

		std::cout << "Connected to WebSocket server at " << host << ":" << port << std::endl;
		readLoop(); // stays here reading in a background thread
	}
	catch (const std::exception& e) { // connection failed 
		std::cout << "WebSocket connection failed: " << e.what() << std::endl;
		m_running = false;
	}
}

void WebsocketClient::readLoop()
{
	try {
		while (m_running) {
			boost::beast::flat_buffer buffer; // for saving until all data has been read
			m_ws->read(buffer);
			std::string message = boost::beast::buffers_to_string(buffer.data());
			try {
				auto data = json::parse(message);
				std::string action = data.value("action", "");
				std::string effect = data.value("effect", "");
				if (onMessage) {
					onMessage(action, effect);
				}
			}
			catch (...) {
				std::cout << "Failed to parse JSON: " << message << std::endl;
			}
		}
	}
	catch (const std::exception& e) {
		std::cout << "Read error: " << e.what() << std::endl;
	}
}
