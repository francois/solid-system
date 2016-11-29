#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <ext/stdio_filebuf.h>
#include <fstream>

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

class BasicHttpParser : public ConnectionHandler
{
  public:
    virtual void handle(std::istream& stream)
    {
      char buffer[2048];

      // HTTP defines end-of-line as \r\n; see https://www.w3.org/Protocols/rfc2616/rfc2616-sec2.html#sec2.2
      // The fact that we stop after at most 2048 bytes means the URLs we will parse can't be longer than
      // 2048 - (METHOD SPACE ... SPACE HTTP_PROTO), or about 2000 bytes. In practice, this is not going to
      // be an issue as we're doing a very dumb HTTP parser here.
      //
      // One other thing we're not doing here is parsing the request's headers and body. For the example,
      // we don't actually care about that. We only care about the request line, which contains the
      // useful information.
      stream.getline(buffer, sizeof(buffer) - 1, '\r');
      std::cerr << "Read " << stream.gcount() << " bytes" << std::endl;
      std::cout << "\"" << buffer << "\"" << std::endl;
    }
};

int main()
{
  BasicHttpParser parser;

  Server server(3000);
  server.run(parser);

  return 0;
}
