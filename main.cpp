#include "lib/websocket.h"
#include "lib/feed_processing.h"
#include "lib/json.hpp"
#include <iostream>

int main() {
    try {
        const std::string host = "ws-feed.exchange.coinbase.com";
        const std::string port = "443";
        nlohmann::json subscribe = {{"type", "subscribe"}, {"product_ids", {"BTC-USD"}}, {"channels", {"ticker"}}};

        WebSocketClient client{host, port};
        client.Connect();
        client.Send(subscribe.dump());
        std::cout << client.Read() << std::endl;
        FeedProcessing processing;
        const size_t kIterations = 100;
        for (size_t i = 0; i < kIterations; ++i) {
            const std::string msg = client.Read();
            processing.ProcessFeed(msg);
        }
        client.Close();

        std::ofstream emaLog;
        emaLog.open("ema.csv", std::ios::out | std::ios::app);
        emaLog << "EMA(Price),EMA(MidPrice)\n";
        const auto& ema = processing.GetPriceEMA();
        const auto& emaMid = processing.GetMidPriceEMA();
        for (size_t i = 0; i < ema.size(); ++i) {
            emaLog << ema[i] << "," << emaMid[i] << "\n";
        }
        emaLog.close();

    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
