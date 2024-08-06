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

BOOST_AUTO_TEST_CASE(test_attributes)
{
  using namespace GCL::parsers::html;

  CHTMLAttribute test(ATTR_CLASS, "Value");
  BOOST_TEST(test == ATTR_CLASS);
  BOOST_TEST(test.attribute() == "class");
}

BOOST_AUTO_TEST_SUITE_END()
