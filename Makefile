a.out: main.cpp Makefile
	g++ main.cpp -l pqxx -l pq

server: server.cpp Makefile
	g++ server.cpp -o server

.PHONY: all clean

all: server a.out

clean:
	rm -f a.out server http_parser_test

http_parser_test: http_parser.h http_parser.cpp http_parser_test.cpp
	g++ http_parser.cpp http_parser_test.cpp -o http_parser_test

test: http_parser_test
	./http_parser_test
