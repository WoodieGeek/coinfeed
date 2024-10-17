CC = g++
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lboost_system -lssl -lcrypto -pthread

GTEST_LIBS = -lgtest -lgtest_main


APP_SOURCES = main.cpp lib/websocket.cpp lib/feed_processing.cpp
TEST_SOURCES = ut/test_feed_processing.cpp lib/feed_processing.cpp

APP_EXEC = coin
TEST_EXEC = coin_test

all: $(APP_EXEC)
$(APP_EXEC):
	$(CC) $(CFLAGS) $(APP_SOURCES) -o $(APP_EXEC) $(LDFLAGS)

test: $(TEST_EXEC)
$(TEST_EXEC):
	$(CC) $(CFLAGS) $(TEST_SOURCES) -o $(TEST_EXEC) $(LDFLAGS) $(GTEST_LIBS)
	./$(TEST_EXEC)

clean:
	rm -f $(APP_EXEC) $(TEST_EXEC)

