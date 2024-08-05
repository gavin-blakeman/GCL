#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include <filesystem>
#include <fstream>
#include <list>
#include <sstream>
#include <vector>
#include <string>
#include <tuple>

#include "include/parsers/html/htmlLexer.hpp"

// Use a derived class to allow access to the virtual and protected
// functions.
class CLexerTest : public GCL::parsers::html::CHTMLLexer<std::list>
{
public:
  CLexerTest(std::istream &is, std::list<GCL::parsers::CToken> &t) : CHTMLLexer(is, t) {}
};

BOOST_AUTO_TEST_SUITE(parser_htmlLexer_test)

BOOST_AUTO_TEST_CASE(constructor_and_destructor)
{
  std::stringstream stream;
  std::list<GCL::parsers::CToken> tokens;

  stream << R"(<html dir=""ltr"">)";

  CLexerTest lexer(stream, tokens);
}

BOOST_AUTO_TEST_CASE(test_getTokens)
{
  using namespace GCL::parsers;
  using namespace GCL::parsers::html;

  std::stringstream stream;
  std::vector<CToken> tokens;

  stream << R"(<html dir=\"ltr\">value</html>)";

  CHTMLLexer<std::vector> lexer(stream, tokens);

  lexer.getTokens();

  BOOST_TEST(tokens.size() == 11);
  BOOST_TEST(tokens[0].type() == L_TAG_OPEN);
  BOOST_TEST(tokens[1].type() == ID);
  BOOST_TEST(tokens[11].type() = TT_EOF);
}

BOOST_AUTO_TEST_CASE(test_file)
{
  using namespace GCL::parsers;
  using namespace GCL::parsers::html;

  std::stringstream stream;
  std::vector<CToken> tokens;

//  stream << R"(<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd"><html>)";
  stream <<
#include "../../generated/testPage.cpp"
;

  CHTMLLexer<std::vector> lexer(stream, tokens);

  lexer.getTokens();

  BOOST_TEST(tokens.size() == 4470);
}


BOOST_AUTO_TEST_SUITE_END()
