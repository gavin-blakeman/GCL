#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include <sstream>
#include <vector>
#include <string>
#include <tuple>

#include "include/parsers/htmlLexer.h"

class CLexerTest : public GCL::parsers::CHTMLLexer
{
public:
  CLexerTest(std::istream &is) : CHTMLLexer(is) {}
};

BOOST_AUTO_TEST_SUITE(parser_htmlLexer_test)


BOOST_AUTO_TEST_CASE(constructor_and_destructor)
{
  std::stringstream stream;

  stream << "<html dir=""ltr"">";

  CLexerTest lexer(stream);
}


BOOST_AUTO_TEST_CASE(test_getTokens)
{
  std::stringstream stream;

  stream << "<html dir=\"ltr\">value</html>";

  CLexerTest lexer(stream);

  std::vector<GCL::parsers::CToken> tokens = lexer.getTokens();

  for (auto const &t: tokens)
  {
    std::cout << t.to_string() << std::endl;
  }

}

BOOST_AUTO_TEST_CASE(test_match)
{
  std::stringstream stream;

  stream << "<html dir=""ltr"">value</html>";

  CLexerTest lexer(stream);

}


BOOST_AUTO_TEST_SUITE_END()
