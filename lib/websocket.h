#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <string>

namespace beast = boost::beast;          // from <boost/beast.hpp>
namespace websocket = beast::websocket;  // from <boost/beast/websocket.hpp>
namespace net = boost::asio;             // from <boost/asio.hpp>
namespace ssl = net::ssl;                // from <boost/asio/ssl.hpp>
using tcp = net::ip::tcp;                // from <boost/asio/ip/tcp.hpp>

class WebSocketClient {


public:
    WebSocketClient(const std::string& host, const std::string& port);
    void Connect();
    void Send(const std::string& message);
    std::string Read();
    void Close();

private:
    std::string Host_;
    std::string Port_;
    net::io_context Ioc_;
    ssl::context Ctx_;
    websocket::stream<beast::ssl_stream<beast::tcp_stream>> Ws_;
};