#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include <filesystem>
#include <sstream>
#include <string>

#include "include/parsers/html/htmlBuffer.hpp"

// Use a derived class to allow access to the virtual and protected
// functions.
class CBufferTest : public GCL::parsers::html::CHTMLBuffer
{
public:
  CBufferTest(std::istream &is) : CHTMLBuffer(is) {}

  char_type test_currentChar() { return currentChar; }
  void test_consume() { consume(); }
  void test_consume(std::size_t n) { consume(n); }
  bool test_match(string_type s, bool cs) { return match(s, cs); }
  void test_reconsume() { reconsume(); }
};

BOOST_AUTO_TEST_SUITE(htmlBuffer_test)

BOOST_AUTO_TEST_CASE(constructor_and_destructor)
{
  using namespace GCL::parsers::html;

  std::stringstream stream;
  stream << R"(<html dir=""ltr"">)";

  BOOST_REQUIRE_NO_THROW(CHTMLBuffer b(stream));
}

BOOST_AUTO_TEST_CASE(test_consume)
{
  using namespace GCL::parsers::html;

  std::stringstream stream;
  stream << R"(<html dir=\"ltr\">value</html>)";

  CBufferTest b(stream);

  BOOST_REQUIRE_NO_THROW(b.test_consume());
  BOOST_TEST(b.test_currentChar() == '<');
  b.test_consume();
  BOOST_TEST(b.test_currentChar() == 'h');
  b.test_consume();
  BOOST_TEST(b.test_currentChar() == 't');
}

BOOST_AUTO_TEST_CASE(test_consume2)
{
  using namespace GCL::parsers::html;

  std::stringstream stream;
  stream << R"(<html dir=\"ltr\">value</html>)";

  CBufferTest b(stream);

  BOOST_REQUIRE_NO_THROW(b.test_consume(5));
  BOOST_TEST(b.test_currentChar() == 'l');
  b.test_consume();
  BOOST_TEST(b.test_currentChar() == ' ');
  b.test_consume();
  BOOST_TEST(b.test_currentChar() == 'd');
}

BOOST_AUTO_TEST_CASE(test_match)
{
  using namespace GCL::parsers::html;

  std::stringstream stream;
  stream << R"(<html dir=\"ltr\">value</html>)";
  CBufferTest b(stream);
  CHTMLBuffer::string_type testStr{'H', 'T', 'M', 'L'};
  CHTMLBuffer::string_type testStr1{'h', 'T', 'm', 'L'};
  CHTMLBuffer::string_type testStr2{'h', 't', 'm', 'l'};

  BOOST_TEST(!b.test_match(testStr, true));
  b.test_consume();
  BOOST_TEST(b.test_match(testStr, false));
  BOOST_TEST(!b.test_match(testStr, true));

  BOOST_TEST(b.test_match(testStr1, false));
  BOOST_TEST(!b.test_match(testStr1, true));

  BOOST_TEST(b.test_match(testStr2, false));
  BOOST_TEST(b.test_match(testStr2, true));
}

BOOST_AUTO_TEST_CASE(test_reconsume)
{
  using namespace GCL::parsers::html;

  std::stringstream stream;
  stream << R"(<html dir=\"ltr\">value</html>)";

  CBufferTest b(stream);

  BOOST_REQUIRE_NO_THROW(b.test_consume());
  BOOST_TEST(b.test_currentChar() == '<');
  BOOST_REQUIRE_NO_THROW(b.test_reconsume());
  b.test_consume();
  BOOST_TEST(b.test_currentChar() == '<');
  b.test_consume();
  BOOST_TEST(b.test_currentChar() == 'h');

  b.test_consume(5);
  BOOST_TEST(b.test_currentChar() == 'd');
  b.test_reconsume();
  b.test_consume();
  b.test_consume();
  BOOST_TEST(b.test_currentChar() == 'i');
}


BOOST_AUTO_TEST_SUITE_END()
