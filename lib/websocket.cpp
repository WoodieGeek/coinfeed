#include "websocket.h"
#include <iostream>

WebSocketClient::WebSocketClient(const std::string& host, const std::string& port)
    : Host_(host), Port_(port), Ctx_{ssl::context::tlsv12_client}, Ws_{Ioc_, Ctx_} {
    Ctx_.set_default_verify_paths();
    Ctx_.set_verify_mode(ssl::verify_peer);
}

void WebSocketClient::Connect() {
    tcp::resolver resolver{Ioc_};
    auto const results = resolver.resolve(Host_, Port_);

    auto ep = beast::get_lowest_layer(Ws_).connect(results.begin(), results.end());
    std::cout << "Connected to: " << ep->endpoint().address() << ":" << ep->endpoint().port() << std::endl;

    if (!SSL_set_tlsext_host_name(Ws_.next_layer().native_handle(), Host_.c_str())) {
        throw beast::system_error(
            beast::error_code(static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()),
            "Failed to set SNI Hostname");
    }

    std::cout << "Starting SSL handshake" << std::endl;
    Ws_.next_layer().handshake(ssl::stream_base::client);
    std::cout << "SSL handshake complete" << std::endl;

    std::string endpoint = Host_ + ":" + Port_;
    Ws_.handshake(endpoint, "/");
    std::cout << "WebSocket handshake complete" << std::endl;
}

void WebSocketClient::Send(const std::string& message) {
    Ws_.write(net::buffer(message));
}

std::string WebSocketClient::Read() {
    beast::flat_buffer buffer;
    Ws_.read(buffer);
    return beast::buffers_to_string(buffer.data());
}

void WebSocketClient::Close() {
    Ws_.close(websocket::close_code::normal);
    std::cout << "Connection closed" << std::endl;
}
