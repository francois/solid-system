#include <tuple>
#include <istream>
#include <string>

class HttpParser
{
  public:
    std::tuple<std::string, std::string> parse(std::istream&);
};
