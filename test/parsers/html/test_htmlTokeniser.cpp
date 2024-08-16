#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST

#include <sstream>

#include "include/parsers/html/htmlTokeniser.h"

class CTestTokeniser final : public GCL::parsers::html::CHTMLTokeniser
{
public:
  CTestTokeniser(std::istream &is) : CHTMLTokeniser(is) {}

  void test_processData() { consume(); processData(); }
};

BOOST_AUTO_TEST_SUITE(htmlTokeniser_test)

BOOST_AUTO_TEST_CASE(test_constructor_and_destructor)
{
  using namespace GCL::parsers::html;

  std::stringstream stream;
  stream << "<html>";

  BOOST_REQUIRE_NO_THROW(CHTMLTokeniser tokeniser(stream));
}

/* Testing of the parsing routines can either be done by calling getToken() or by calling each relevant
 * process... routine. The process routines only progress one character at a time, but it is easier to
 * check the exit state and ensure all code pathways are followed.
 */

BOOST_AUTO_TEST_CASE(test_startTag)
{
  using namespace GCL::parsers::html;

  std::stringstream stream;
  stream << "<html>";

  CHTMLTokeniser tokeniser(stream);

  CHTMLToken token = tokeniser.getToken();
  BOOST_TEST(token.type() == CHTMLToken::TT_TAG_START);
  bool test = token.name() == CHTMLToken::string_type({'h', 't', 'm', 'l'});
  BOOST_TEST(test);
  BOOST_TEST(!token.isSelfClosing());
  token = tokeniser.getToken();
  BOOST_TEST(token.type() == CHTMLToken::TT_EOF);

  std::stringstream stream2;
  stream2 << "<html/>";

  CHTMLTokeniser tokeniser2(stream2);

  token = tokeniser2.getToken();
  BOOST_TEST(token.type() == CHTMLToken::TT_TAG_START);
  test = token.name() == CHTMLToken::string_type({'h', 't', 'm', 'l'});
  BOOST_TEST(test);
  BOOST_TEST(token.isSelfClosing());

}

BOOST_AUTO_TEST_CASE(test_endTag)
{
  using namespace GCL::parsers::html;

  std::stringstream stream;
  stream << "</html>";

  CHTMLTokeniser tokeniser(stream);

  CHTMLToken token = tokeniser.getToken();
  BOOST_TEST(token.type() == CHTMLToken::TT_TAG_END);
  bool test = token.name() == CHTMLToken::string_type({'h', 't', 'm', 'l'});
  BOOST_TEST(test);
  BOOST_REQUIRE_THROW(!token.isSelfClosing(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_docType)
{
  using namespace GCL::parsers::html;

  std::stringstream stream;
  stream << "<!DOCTYPE html>";
  CHTMLTokeniser tokeniser(stream);
  CHTMLToken token = tokeniser.getToken();
  BOOST_TEST(token.type() == CHTMLToken::TT_DOCTYPE);
  bool test = token.name() == CHTMLToken::string_type({'h', 't', 'm', 'l'});
  BOOST_TEST(test);

  std::stringstream stream2;
  stream2 << "<!DOCTYPE html SYSTEM \"about:legacy-compat\">";
  CHTMLTokeniser tokeniser2(stream2);
  token = tokeniser2.getToken();
  BOOST_TEST(token.type() == CHTMLToken::TT_DOCTYPE);
  test = token.name() == CHTMLToken::string_type({'h', 't', 'm', 'l'});
  BOOST_TEST(test);
}


BOOST_AUTO_TEST_SUITE_END()
