#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <ext/stdio_filebuf.h>
#include <fstream>

#include "http_parser.h"

class ConnectionHandler
{
	public:
		virtual ~ConnectionHandler() {}
		virtual void handle(std::istream&) = 0;
};

class Server {
  public:
    Server(int);
    void run(ConnectionHandler&);

  private:
    int open_socket();

    int m_port;
    bool m_running;
};

Server::Server(int port)
{
  m_port = port;
  m_running = true;
}

int Server::open_socket() {
	int sockfd;
  if ( -1 == ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) )
  {
    perror("socket");
    exit(1);
  }

  int yes = 1;
  if ( -1 == setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) )
  {
    perror("setsockopt");
    exit(1);
  }

  struct sockaddr_in hints;
  memset(&hints, 0, sizeof(hints));
  hints.sin_family = AF_INET;
  hints.sin_addr.s_addr = INADDR_ANY;
  hints.sin_port = htons(m_port);
  if ( -1 == bind(sockfd, (const sockaddr*)&hints, sizeof(hints)) )
  {
    perror("bind");
    exit(1);
  }


  if ( -1 == listen(sockfd, 1) )
  {
    perror("listen");
    exit(1);
  }

  return sockfd;
}

void Server::run(ConnectionHandler& handler) {
  int sockfd = this->open_socket();
  int clientfd;

  std::cerr << "Running on port " << m_port << ", file descriptor " << sockfd << std::endl;

  while(m_running) {
    clientfd = accept(sockfd, NULL, NULL);
    if (errno != 0)
    {
      perror("accept");
      continue;
    }

		__gnu_cxx::stdio_filebuf<char> filebuf(clientfd, std::ios::in);
		std::istream is(&filebuf);
		handler.handle(is);

    close(clientfd);
    std::cerr << "Received connection" << std::endl;
  }

  close(sockfd);
}

class PGBrowserApp : public ConnectionHandler
{
  public:
    virtual void handle(std::istream& stream)
    {
      std::tuple<std::string, std::string> result = parser.parse(stream);
      std::cout << std::get<0>(result) << ", " << std::get<1>(result) << std::endl;
    }

  private:
      HttpParser parser;
};

int main()
{
  PGBrowserApp app;

  Server server(3000);
  server.run(app);

  return 0;
}
