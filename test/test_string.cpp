
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

// Standard C++ libraries
#include <iomanip>
#include <list>
#include <string>
#include <vector>

#include "../include/stringFunctions.h"

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

  BOOST_REQUIRE_NO_THROW(tokeniseString(opStr, testStr));
  BOOST_TEST(reqStr == opStr);

  testStr = "Tokenize this string";
  opStr.clear();
  BOOST_REQUIRE_NO_THROW(tokeniseString(opStr, testStr, " "));
  BOOST_TEST(reqStr == opStr);

  testStr = "REL  CNF  GMCO GMPS MACM PRC  SETC";
  opStr.clear();
  BOOST_REQUIRE_NO_THROW(tokeniseString(opStr, testStr, " "));
  BOOST_TEST(opStr.size() == 10);
  BOOST_TEST(opStr[0] == "REL");
  BOOST_TEST(opStr[2] == "CNF");
}

BOOST_AUTO_TEST_CASE(test_cleanDecimal)
{
  using namespace GCL;
  class cspd : public std::numpunct<char>  // 'Comma separated, point decimal
  {
  protected:
    virtual string_type do_grouping() const override { return "\3"; }
    virtual char do_thousands_sep() const override { return ','; }
    virtual char do_decimal_point() const override { return '.'; }
  public:
    cspd() : numpunct() {}
  };

  class pscd : public std::numpunct<char>  // 'point separated, comma decimal
  {
  protected:
    virtual string_type do_grouping() const override { return "\3"; }
    virtual char do_thousands_sep() const override { return '.'; }
    virtual char do_decimal_point() const override { return ','; }
  public:
    pscd() : numpunct() {}
  };

  class sscd : public std::numpunct<char>  // 'space separated, comma decimal
  {
  protected:
    virtual string_type do_grouping() const override { return "\3"; }
    virtual char do_thousands_sep() const override { return ' '; }
    virtual char do_decimal_point() const override { return ','; }
  public:
    sscd() : numpunct() {}
  };

  class sspd : public std::numpunct<char>  // 'space seperated, point decimal
  {
  protected:
    virtual string_type do_grouping() const override { return "\3"; }
    virtual char do_thousands_sep() const override { return ' '; }
    virtual char do_decimal_point() const override { return '.'; }
  public:
    sspd() : numpunct() {}
  };

  class nspd : public std::numpunct<char>  // no seperated, point decimal
  {
  protected:
    virtual string_type do_grouping() const override { return "\0"; }
    virtual char do_decimal_point() const override { return '.'; }
  public:
    nspd() : numpunct() {}
  };

  class nscd : public std::numpunct<char>  // no seperated, comma decimal
  {
  protected:
    virtual string_type do_grouping() const override { return "\0"; }
    virtual char do_decimal_point() const override { return ','; }
  public:
    nscd() : numpunct() {}
  };


  std::ostringstream strm;
  strm.precision(10);

  std::string szTest("123,456.00");
  std::string szResult("123456.00");
  cleanDecimal(szTest);
  BOOST_TEST(szTest == szResult);

  szTest.push_back('-');
  szResult.insert(0, "-");
  cleanDecimal(szTest);
  BOOST_TEST(szTest == szResult);

  double dTest = 123456.789;
  szResult = "123456.789";

  strm.imbue(std::locale(strm.getloc(), new cspd));
  strm << dTest;
  szTest = strm.str();
  cleanDecimal(szTest);
  BOOST_TEST(szTest == szResult);

  // Change locale to ".xxx.xxx,00"

  strm.str("");
  strm.imbue(std::locale(strm.getloc(), new pscd));
  strm << dTest;
  szTest = strm.str();
  cleanDecimal(szTest);
  BOOST_TEST(szTest == szResult);

  strm.str("");
  strm.imbue(std::locale(strm.getloc(), new sscd));
  strm << dTest;
  szTest = strm.str();
  cleanDecimal(szTest);
  BOOST_TEST(szTest == szResult);
}


BOOST_AUTO_TEST_SUITE_END()
