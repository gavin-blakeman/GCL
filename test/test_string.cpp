#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include <list>
#include <string>
#include <vector>

#include "include/string.hpp"

BOOST_AUTO_TEST_SUITE(sqlWriter_test)

BOOST_AUTO_TEST_CASE(tokenise_toList)
{
  using namespace GCL;
  std::string testStr = "Tokenize.this.string";
  std::list<std::string> reqStr = {"Tokenize", "this", "string"};
  std::list<std::string> opStr;

  BOOST_REQUIRE_NO_THROW(tokeniseString(opStr, testStr););
  BOOST_TEST(reqStr == opStr);
}

BOOST_AUTO_TEST_CASE(tokenise_toVector)
{
  using namespace GCL;
  std::string testStr = "Tokenize.this.string";
  std::vector<std::string> reqStr = {"Tokenize", "this", "string"};
  std::vector<std::string> opStr;

  BOOST_REQUIRE_NO_THROW(tokeniseString(opStr, testStr););
  BOOST_TEST(reqStr == opStr);
}

BOOST_AUTO_TEST_SUITE_END()
