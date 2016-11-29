#include "http_parser.h"

// Implements a *very* basic HTTP parser
// This parser will choke on valid HTTP Request-Line (GET   /    HTTP/1.0)
std::tuple<std::string, std::string> HttpParser::parse(std::istream& stream)
{
  char buffer[2048];

  // Extract the METHOD from the HTTP Request-Line
  stream.getline(buffer, sizeof(buffer) - 1, ' ');
  std::string method(buffer);

  // Extract URI from HTTP Request-Line
  stream.getline(buffer, sizeof(buffer) - 1, ' ');
  std::string uri(buffer);

  return std::tuple<std::string, std::string>{method, uri};
}
