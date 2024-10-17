#pragma once

#include <string>
#include <vector>
#include <optional>
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>

class FeedProcessing {
public:
    FeedProcessing();
    void ProcessFeed(const std::string& jsonString);
    std::vector<double> GetPriceEMA();
    std::vector<double> GetMidPriceEMA();
    ~FeedProcessing();

private:
    std::vector<double> CalculateEMA(const std::vector<double>& data) const;

private:
    std::ofstream Log_;
    std::vector<double> Prices_;
    std::vector<double> MidPrices_;
    std::vector<boost::posix_time::ptime> Times_;
    double LastPrice_;
    double LastMidPrice_;
    std::optional<boost::posix_time::ptime> LastInterval_;

};
