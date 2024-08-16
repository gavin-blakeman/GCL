#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include <filesystem>
#include <sstream>
#include <string>

#include "include/parsers/html/htmlTokens.h"

BOOST_AUTO_TEST_SUITE(htmlTokens_test)

BOOST_AUTO_TEST_CASE(constructor_and_destructor)
{
  using namespace GCL::parsers::html;

  CHTMLToken token;
  BOOST_TEST(token.type() == CHTMLToken::TT_NONE);

  CHTMLToken t2(CHTMLToken::TT_CHARACTER);
  BOOST_TEST(t2.type() == CHTMLToken::TT_CHARACTER);
}

BOOST_AUTO_TEST_CASE(test_DOCTYPE)
{
  using namespace GCL::parsers::html;

  CHTMLToken token;
  BOOST_REQUIRE_NO_THROW(token.type(CHTMLToken::TT_DOCTYPE));
  BOOST_TEST(token.type() == CHTMLToken::TT_DOCTYPE);
  BOOST_TEST(!token.forceQuirks());
  BOOST_TEST(!token.hasSystemIdentifier());
  BOOST_TEST(!token.hasPublicIdentifier());

  token.forceQuirks(true);
  BOOST_TEST(token.forceQuirks());

  token.setPublicIdentifierEmpty();
  BOOST_TEST(token.hasPublicIdentifier());

  token.setSystemIdentifierEmpty();
  BOOST_TEST(token.hasSystemIdentifier());
}

BOOST_AUTO_TEST_CASE(test_TAG)
{
  using namespace GCL::parsers::html;

  CHTMLToken token;
  BOOST_REQUIRE_NO_THROW(token.type(CHTMLToken::TT_TAG_START));
  BOOST_TEST(token.type() == CHTMLToken::TT_TAG_START);
  BOOST_TEST(!token.isSelfClosing());

  token.selfClosing(true);
  BOOST_TEST(token.isSelfClosing());
}

BOOST_AUTO_TEST_SUITE_END()
