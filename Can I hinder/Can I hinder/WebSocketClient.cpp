#include "WebSocketClient.h"




WebsocketClient::WebsocketClient() : m_resolver(m_ioc) // initialize DNS resolver with io_context (event loop) so it can resolve hostnames

{
}

WebsocketClient::~WebsocketClient()
{
	close();
}

void WebsocketClient::connect(const std::string& host, const std::string& port, const std::string& session, bool useTls)
{
	m_running = true;
	// start the background thread so not to interrupt game loop
	m_thread = std::thread(&WebsocketClient::run, this, host, port, session, useTls);
}
// stops the background thread
void WebsocketClient::close()
{
	m_running = false;

	boost::system::error_code ec;
	if (m_wss) 
		m_wss->close(websocket::close_code::normal, ec);
	if (m_ws)
		m_ws->close(websocket::close_code::normal, ec);

	if(m_thread.joinable()) {
		m_thread.join();
	}
}

void WebsocketClient::setOnMessage(std::function<void(const std::string& ,const std::string&,const std::string&)> callback)
{
	onMessage = callback; // this is so it'll define what happens when a message is recieved (spawning an eneny)
}
std::string WebsocketClient::createSession()
{
	try
	{
		std::string host = "can-i-hinder-api-dkafh4amdxhxa4ha.germanywestcentral-01.azurewebsites.net";
		std::string port = "443";
		std::string target = "/api/game/create-session";

		boost::asio::io_context ioc;
		ssl::context ctx{ ssl::context::tlsv12_client };

		tcp::resolver resolver{ ioc };
		auto const results = resolver.resolve(host, port);

		beast::ssl_stream<beast::tcp_stream> stream{ ioc, ctx };

		boost::asio::connect(stream.next_layer().socket(), results.begin(), results.end());

		stream.handshake(ssl::stream_base::client);

		http::request<http::string_body> req{ http::verb::post, target, 11 };
		req.set(http::field::host, host);
		req.set(http::field::user_agent, "CanIhinderGame");

		http::write(stream, req);

		beast::flat_buffer buffer;
		http::response<http::string_body> res;

		http::read(stream, buffer, res);

		auto json = nlohmann::json::parse(res.body());

		std::string session = json["sessionId"];

		std::cout << "Session created: " << session << std::endl;

		return session;
	}
	catch (const std::exception& e)
	{
		std::cout << "Failed to create session: " << e.what() << std::endl;
		return "ERROR";
	}
}
// runs in background thread
void WebsocketClient::run(const std::string& host, const std::string& port, const std::string& session, bool useTls)
{
	
	try {
		// Resolve the host and port to get a list of endpoints
		const auto results = m_resolver.resolve(host, port);

		if (useTls) {
		  //TLS for my azure hosted websocket
			m_ssl_ctx.set_default_verify_paths();
			// Set the verification mode to verify the server's certificate
			m_ssl_ctx.set_verify_mode(ssl::verify_peer);
			// Load the CA certificate to verify the server's certificate against trusted CAs... this broke the connection until this was added!!!!
			m_ssl_ctx.load_verify_file("ASSETS/certs/cacert.pem");
			// Create the WebSocket stream with SSL
			m_wss = std::make_unique<websocket::stream<beast::ssl_stream<tcp::socket>>>(m_ioc, m_ssl_ctx);
			// Set a timeout on the underlying TCP socket
			boost::asio::connect(beast::get_lowest_layer(*m_wss), results.begin(), results.end());

			// SNI for Azure/custom domains
			if (!SSL_set_tlsext_host_name(m_wss->next_layer().native_handle(), host.c_str()))
				throw std::runtime_error("Failed to set SNI hostname");
			// Verify cert matches hostname
			m_wss->next_layer().set_verify_callback(ssl::host_name_verification(host));
			m_wss->next_layer().handshake(ssl::stream_base::client);
			std::string path = "/?session=" + session;
			// Perform WebSocket handshake
			std::cout << "Joining session: " << session << std::endl;
			m_wss->handshake(host, path);

			std::cout << "Connected to WSS: wss://" 
          << host << ":" << port 
          << path << "\n";
		}
		else {
			//for local testing 
			m_ws = std::make_unique<websocket::stream<tcp::socket>>(m_ioc);
			// Set a timeout on the underlying TCP socket
			boost::asio::connect(m_ws->next_layer(), results.begin(), results.end());
			std::string path = "/?session=" + session;
			// Perform WebSocket handshake

			std::cout << "Joining session: " << session << std::endl;
			m_ws->handshake(host, path);

			std::cout << "Connected to WS: ws://"
				<< host << ":" << port
				<< path << "\n";
		}

		readLoop(useTls);
	}
	// If any error occurs during connection or handshake, it will be caught here
	catch (const std::exception& e) {
		std::cout << "WebSocket connection failed: " << e.what() << "\n";
		m_running = false;
	}
	
}

void WebsocketClient::readLoop(bool useTls)
{
	try {
		boost::beast::flat_buffer buffer; // for saving until all data has been read
		while (m_running) {
			buffer.clear();
			if (useTls) 
				m_wss->read(buffer);
            else       
				m_ws->read(buffer);
			std::string message = boost::beast::buffers_to_string(buffer.data());
			try {
				auto data = json::parse(message);
				std::string user = data.value("user", "");
				std::string action = data.value("action", "");
				std::string effect = data.value("effect", "");
				if (onMessage) {
					onMessage(user, action, effect);
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
