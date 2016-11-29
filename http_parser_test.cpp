#include <iostream>
#include <sstream>
#include "http_parser.h"

void assert_equal(std::string expected, std::string actual, int line)
{
  if (expected == actual) return;
  std::cerr << "Expected \"" << expected << "\" but found \"" << actual << "\" (line " << line << ")" << std::endl;
  exit(1);
}

#define assert(A, B) (assert_equal((A), (B), __LINE__))

void test001()
{
  HttpParser parser;
  std::tuple<std::string, std::string> result;

  std::istringstream stream("GET / HTTP/1.0");
  result = parser.parse(stream);
  assert("GET", std::get<0>(result));
  assert("/", std::get<1>(result));
}

void test002()
{
  HttpParser parser;
  std::tuple<std::string, std::string> result;

  std::istringstream stream("POST / HTTP/1.0");
  result = parser.parse(stream);
  assert("POST", std::get<0>(result));
  assert("/", std::get<1>(result));
}

void test003()
{
  HttpParser parser;
  std::tuple<std::string, std::string> result;

  std::istringstream stream("PATCH /employees/13 HTTP/1.0");
  result = parser.parse(stream);
  assert("PATCH", std::get<0>(result));
  assert("/employees/13", std::get<1>(result));
}

int main() {
  test001();
  test002();
  test003();

  exit(0);
}
