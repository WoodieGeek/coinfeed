CC = g++
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lboost_system -lssl -lcrypto -pthread

SOURCES = main.cpp lib/websocket.cpp

all:
	$(CC) $(CFLAGS) $(SOURCES) -o coin $(LDFLAGS)

clean:
	rm -f coin

