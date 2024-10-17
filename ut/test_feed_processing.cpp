#include <gtest/gtest.h>
#include "../lib/feed_processing.h"


TEST(FeedProcessingTest, BasicEMAComputation) {
    FeedProcessing fp;
    fp.ProcessFeed(R"({"price":"100.0", "best_bid":"100.0", "best_ask":"102.0", "time":"2024-10-17T08:10:10.000Z"})");
    fp.ProcessFeed(R"({"price":"101.0", "best_bid":"101.0", "best_ask":"103.0", "time":"2024-10-17T08:10:15.000Z"})");
    fp.ProcessFeed(R"({"price":"101.0", "best_bid":"101.0", "best_ask":"103.0", "time":"2024-10-17T08:10:15.000Z"})");
    fp.ProcessFeed(R"({"price":"102.0", "best_bid":"102.0", "best_ask":"103.0", "time":"2024-10-17T08:10:20.000Z"})");
    
    EXPECT_NEAR(fp.GetPriceEMA(), 100.5, 0.01);  // Adjust parameters and precision as per your need
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
