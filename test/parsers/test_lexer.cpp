#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include <sstream>
#include <vector>
#include <string>
#include <tuple>

#include "include/parsers/lexer.hpp"
#include "include/parsers/token.h"

BOOST_AUTO_TEST_SUITE(parser_lexer_test)

std::vector<GCL::parsers::CToken> tokens;

static std::vector<std::pair<GCL::parsers::CLexer<std::vector>::tokenID_t, std::string>> const tokenTypes = {};

class CLexerTest : public GCL::parsers::CLexer<std::vector>
{
public:
  CLexerTest(std::istream &is) : CLexer(is, tokenTypes.begin(), tokenTypes.end(), tokens) {}
  void nextToken() {}

  bool matchTest(const char c) { return match(c); }
  bool matchTest(std::string const &str) { return match(str); }
};


BOOST_AUTO_TEST_CASE(constructor_and_destructor)
{
  std::stringstream stream;

  stream << "This is a test.";

  CLexerTest lexer(stream);
}

BOOST_AUTO_TEST_CASE(test_match)
{
  std::stringstream stream;

  stream << "This is a /test.";

  CLexerTest lexer(stream);

  BOOST_TEST(lexer.matchTest('T'));

  BOOST_TEST(lexer.matchTest("This"));

}


BOOST_AUTO_TEST_SUITE_END()
