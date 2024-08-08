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
<<<<<<< HEAD
  BOOST_REQUIRE_NO_THROW(testParser.parseDocument());
//  BOOST_TEST(testDocument.root->nodeType() == NT_ELEMENT);
=======
  //BOOST_REQUIRE_NO_THROW(testParser.parseDocument());
  //BOOST_TEST(testDocument.root->nodtype() == "html");
>>>>>>> 32b1331 (progressing changes)
}

BOOST_AUTO_TEST_CASE(test_parser)
{
  using namespace GCL::parsers;
  using namespace GCL::parsers::html;

  CHTMLDocument testDocument;
  std::stringstream stream;

  stream <<
#include "../../generated/testPage.cpp"
;

  CHTMLParser testParser(stream, testDocument);
<<<<<<< HEAD
//  BOOST_REQUIRE_NO_THROW(testParser.parseDocument());
//  BOOST_TEST(testDocument.root->nodeType() == NT_ELEMENT);
=======
  //BOOST_REQUIRE_NO_THROW(testParser.parseDocument());
  //BOOST_TEST(testDocument.root->nodeType() == NT_DOCTYPE);
>>>>>>> 32b1331 (progressing changes)
}




BOOST_AUTO_TEST_SUITE_END()
