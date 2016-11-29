#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

class Server {
  public:
    Server(int);
    void run();

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

#define HTTP_RESPONSE "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/html\r\nContent-Length: 22\r\n\r\n<h1>Hello World</h1>\r\n"

void Server::run() {
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

    write(clientfd, HTTP_RESPONSE, sizeof(HTTP_RESPONSE) - 1);
    if (errno != 0) perror("write to client");
    close(clientfd);
    std::cerr << "Received connection" << std::endl;
  }

  close(sockfd);
}


int main()
{
  Server server(3000);
  server.run();

  return 0;
}
