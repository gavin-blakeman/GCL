#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

// Standard C++ libraries.
#include <filesystem>
#include <fstream>
#include <list>
#include <sstream>
#include <vector>
#include <string>
#include <tuple>

#include "include/parsers/html/htmlAttributes.h"

BOOST_AUTO_TEST_SUITE(parser_html_attributes)

BOOST_AUTO_TEST_CASE(constructor_and_destructor)
{
  using namespace GCL::parsers::html;

  CHTMLAttribute test1("class", "Value");
  BOOST_TEST(test1.attribute() == "class");
  BOOST_TEST(test1.value() == "Value");

  CHTMLAttribute test2(ATTR_CLASS, "Value");
  BOOST_TEST(test2.attribute() == "class");
  BOOST_TEST(test2.value() == "Value");

  std::string szTest("Value");
  CHTMLAttribute test3(ATTR_CLASS, std::move(szTest));

  BOOST_TEST(szTest.empty());
  BOOST_TEST(test3.attribute() == "class");
  BOOST_TEST(test3.value() == "Value");
}


BOOST_AUTO_TEST_CASE(test_equality)
{
  using namespace GCL::parsers::html;

  CHTMLAttribute test1("class", "Value");
  CHTMLAttribute test2(ATTR_CLASS, "Value");

  BOOST_TEST(test1 == test2);

  BOOST_TEST(test1 == "class");

  BOOST_TEST(test2 == ATTR_CLASS);
}

BOOST_AUTO_TEST_CASE(test_match)
{
//  std::stringstream stream;
//  std::list<GCL::parsers::CToken> tokens;

//  stream << "<html dir=""ltr"">value</html>";

//  CLexerTest lexer(stream, tokens);
//  lexer.getTokens();
}

BOOST_AUTO_TEST_CASE(test_file)
{

//  std::ifstream ifs;
//
//  std::cout << std::filesystem::current_path() << std::endl;
//  std::filesystem::path path{"test_data/MM60.htm"};
//  if (!std::filesystem::is_regular_file(path))
//  {
//    RUNTIME_ERROR("Unable to open test file.");
//  }
//
//  ifs.open(path);
//  if (!ifs.good())
//  {
//    CODE_ERROR();
//  }
//
//  std::vector<GCL::parsers::CToken> tokens;
//  GCL::parsers::CHTMLLexer lexer(ifs, tokens);
//
//  lexer.getTokens();
//
//  std::filesystem::path ofn{"test_data/outputFile.tok"};
//  std::fstream ofs;

//  ofs.open(ofn, std::ios_base::out | std::ios_base::trunc);
//  if (!ofs.good())
//  {
//    CODE_ERROR();
//  }

//  for(auto const &t: tokens)
//  {
//    ofs << t.to_string() << std::endl;
//    std::cout << t.to_string() << std::endl;
//  }
//  ofs.close();
//  std::cout << std::filesystem::current_path() << std::endl;

}


BOOST_AUTO_TEST_SUITE_END()
