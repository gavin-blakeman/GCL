#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include "include/parsers/codePoint.h"

BOOST_AUTO_TEST_SUITE(parser_codePoint_test)

BOOST_AUTO_TEST_CASE(constructors_and_destructor)
{
  using namespace GCL::parsers;

  BOOST_REQUIRE_NO_THROW(codePoint_t cp);
  BOOST_REQUIRE_NO_THROW(codePoint_t cp1('A'));
}

BOOST_AUTO_TEST_CASE(test_is_equal)
{
  using namespace GCL::parsers;

  codePoint_t cp('a');
  BOOST_TEST(cp == 'a');

}

BOOST_AUTO_TEST_CASE(test_isalpha)
{
  using namespace GCL::parsers;

  BOOST_TEST(codePoint_t('a').isalpha());
  BOOST_TEST(codePoint_t('M').isalpha());
  BOOST_TEST(!codePoint_t('4').isalpha());
  BOOST_TEST(!codePoint_t('!').isalpha());
  BOOST_TEST(!codePoint_t(':').isalpha());
}

BOOST_AUTO_TEST_CASE(test_islower)
{
  using namespace GCL::parsers;

  BOOST_TEST(codePoint_t('a').islower());
  BOOST_TEST(!codePoint_t('M').islower());
  BOOST_TEST(!codePoint_t('Z').islower());
  BOOST_TEST(codePoint_t('x').islower());
  BOOST_TEST(!codePoint_t('T').islower());
}

BOOST_AUTO_TEST_CASE(test_isupper)
{
  using namespace GCL::parsers;

  BOOST_TEST(!codePoint_t('a').isupper());
  BOOST_TEST(codePoint_t('M').isupper());
  BOOST_TEST(codePoint_t('Z').isupper());
  BOOST_TEST(!codePoint_t('x').isupper());
  BOOST_TEST(codePoint_t('T').isupper());
}

BOOST_AUTO_TEST_CASE(test_tolower)
{
  using namespace GCL::parsers;

  BOOST_TEST(codePoint_t('a').tolower() == 'a');
  BOOST_TEST(codePoint_t('M').tolower() == 'm');
  BOOST_TEST(codePoint_t('Z').tolower() == 'z');
  BOOST_TEST(codePoint_t('x').tolower() == 'x');
  BOOST_TEST(codePoint_t('T').tolower() == 't');
}


BOOST_AUTO_TEST_SUITE_END()
