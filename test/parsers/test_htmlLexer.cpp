#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <tuple>

#include "include/parsers/html/htmlLexer.h"

class CLexerTest : public GCL::parsers::html::CHTMLLexer
{
public:
  CLexerTest(std::istream &is, std::vector<GCL::parsers::CToken> &t) : CHTMLLexer(is, t) {}
};

BOOST_AUTO_TEST_SUITE(parser_htmlLexer_test)


BOOST_AUTO_TEST_CASE(constructor_and_destructor)
{
  std::stringstream stream;
  std::vector<GCL::parsers::CToken> tokens;

  stream << "<html dir=""ltr"">";

  CLexerTest lexer(stream, tokens);
}


BOOST_AUTO_TEST_CASE(test_getTokens)
{
  std::stringstream stream;
  std::vector<GCL::parsers::CToken> tokens;

  stream << "<html dir=\"ltr\">value</html>";

  CLexerTest lexer(stream, tokens);

  lexer.getTokens();
}

BOOST_AUTO_TEST_CASE(test_match)
{
  std::stringstream stream;
  std::vector<GCL::parsers::CToken> tokens;

  stream << "<html dir=""ltr"">value</html>";

  CLexerTest lexer(stream, tokens);
  lexer.getTokens();
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
