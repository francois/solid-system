a.out: main.cpp Makefile
	g++ main.cpp -l pqxx -l pq

server: server.cpp Makefile
	g++ server.cpp -o server

.PHONY: all clean

all: server a.out

clean:
	rm -f a.out server
