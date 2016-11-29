.PHONY: all clean test

all: server

run: server
	./server

server: test http_parser.h http_parser.cpp server.cpp Makefile
	g++ http_parser.cpp server.cpp -o server

clean:
	rm -f a.out server http_parser_test

http_parser_test: http_parser.h http_parser.cpp http_parser_test.cpp
	g++ http_parser.cpp http_parser_test.cpp -o http_parser_test

test: http_parser_test
	./http_parser_test
