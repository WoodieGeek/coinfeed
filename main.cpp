#include "lib/websocket.h"
#include <iostream>

int main() {
    try {
        // Required set up
        //auto const host = "ws-feed-public.sandbox.exchange.coinbase.com";
        auto const host = "ws-feed.exchange.coinbase.com";
        auto const port = "443";
        auto const text = "{\"type\": \"subscribe\", \"product_ids\": [\"BTC-USD\"], \"channels\": [\"ticker\"]}";

        WebSocketClient client{host, port};
        client.Connect();
        client.Send(text);
        for (size_t i = 0; i < 10; ++i) {
            std::cout << client.Read() << std::endl;
        }
        client.Close();

    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
