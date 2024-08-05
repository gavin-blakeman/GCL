#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST

#include <filesystem>
#include <fstream>
#include <list>
#include <sstream>
#include <vector>
#include <string>
#include <tuple>

#include "include/parsers/html/htmlDocument.h"
#include "include/parsers/html/htmlParser.h"

BOOST_AUTO_TEST_SUITE(htmlParser_test)

BOOST_AUTO_TEST_CASE(lTag)
{
  using namespace GCL::parsers;
  using namespace GCL::parsers::html;

  CHTMLDocument testDocument;
  std::stringstream stream;
  stream << "<html>";

  CHTMLParser testParser(stream, testDocument);
  BOOST_REQUIRE_NO_THROW(testParser.parseDocument());
  BOOST_TEST(testDocument.root->type() == "html");
}

BOOST_AUTO_TEST_CASE(test_rTag)
{
  using namespace GCL::parsers;
  using namespace GCL::parsers::html;

  std::stringstream stream;
  std::vector<CToken> tokens;

  stream << "<html dir=\"ltr\">value</html>";

//  CHTMLLexer<std::vector> lexer(stream, tokens);

//  lexer.getTokens();

//  BOOST_TEST(tokens.size() == 11);
  //BOOST_TEST(tokens[0].type() == L_TAG_OPEN);
//  BOOST_TEST(tokens[1].type() == ID);
  //BOOST_TEST(tokens[11].type() = TT_EOF);
}




BOOST_AUTO_TEST_SUITE_END()
