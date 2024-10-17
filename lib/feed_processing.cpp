#include "feed_processing.h"
#include "json.hpp"

#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <algorithm>

FeedProcessing::FeedProcessing()
    : LastPrice_(0),
      LastMidPrice_(0)
{
    Log_.open("log.csv", std::ios::out | std::ios::app);
    Log_ << "Time,Price,Bid.Price,Ask.Price\n";
}

void FeedProcessing::ProcessFeed(const std::string &jsonString)
{
    auto j = nlohmann::json::parse(jsonString);

    double price = std::stod(j["price"].get<std::string>());
    double bestBid = std::stod(j["best_bid"].get<std::string>());
    double bestAsk = std::stod(j["best_ask"].get<std::string>());
    std::string timeStr = j["time"].get<std::string>();
    if (Log_.is_open()) {
        Log_ << timeStr << "," << price << "," << bestBid << "," << bestAsk << "\n";
    }

    // Remove the 'Z' at the end for boost::posix_time
    timeStr.pop_back();

    // Replace 'T' with a space
    std::replace(timeStr.begin(), timeStr.end(), 'T', ' ');

    boost::posix_time::ptime epoch(boost::gregorian::date(1970,1,1));
    boost::posix_time::ptime pt = boost::posix_time::time_from_string(timeStr);

    const int kPeriodSeconds = 5;

    if (!LastInterval_) {
        LastInterval_ = pt + boost::posix_time::seconds(kPeriodSeconds);
        Prices_.emplace_back(price);
        MidPrices_.emplace_back((bestBid + bestAsk) / 2.0);
    }

    if (pt >= *LastInterval_) {
        Prices_.emplace_back(LastPrice_);
        MidPrices_.emplace_back(LastMidPrice_);
        *LastInterval_ += boost::posix_time::seconds(kPeriodSeconds);
    }
    LastPrice_ = price;
    LastMidPrice_ = (bestBid + bestAsk) / 2.0;
}

std::vector<double> FeedProcessing::CalculateEMA(const std::vector<double>& data) const {
    const double multiplier = 2.0 / (data.size() + 1.0);
    double ema = 0;
    std::vector<double> result;
    result.reserve(data.size());
    for (size_t i = 0; i < data.size(); i++) {
        if (i == 0) {
            ema = data[i];
        } else {
            ema = (data[i] - ema) * multiplier + ema;
        }
        result.emplace_back(ema);
    }
    return result;
}

std::vector<double> FeedProcessing::GetPriceEMA() {
    return CalculateEMA(Prices_);
}

std::vector<double> FeedProcessing::GetMidPriceEMA() {
    return CalculateEMA(MidPrices_);
}

FeedProcessing::~FeedProcessing()
{
    if (Log_.is_open()) {
        Log_.close();
    }
}
