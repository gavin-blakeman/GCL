#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include <vector>
#include <string>
#include <tuple>

#include <fmt/format.h>
#include <fmt/chrono.h>

#include "include/dateTime.h"

BOOST_TEST_DONT_PRINT_LOG_VALUE(GCL::date_t::valueType);

namespace GCL
{
  std::ostream &boost_test_print_type(std::ostream &ostr, GCL::date_t::valueType dv)
  {
    ostr << fmt::format("{:%Y-%m-%d}", dv);
    return ostr;
  }
}

BOOST_AUTO_TEST_SUITE(dateTime_test)

BOOST_AUTO_TEST_CASE(comparison_test)
{
  using namespace GCL;

  date_t date1("2020-04-01");
  date_t date2("2020-04-02");
  date_t date3("2020-04-01");

  BOOST_TEST(date1 == date3);
  BOOST_TEST(date1 != date2);
  BOOST_TEST(date1 < date2);
  BOOST_TEST(date2 > date1);
  BOOST_TEST(date1 <= date2);
  BOOST_TEST(date2 >= date1);
  BOOST_TEST(date1 >= date3);
  BOOST_TEST(date1 <= date3);
}

BOOST_AUTO_TEST_CASE(isDate_test)
{
  using namespace GCL;

//  for (auto &testDate: dates)
//  {
//    BOOST_REQUIRE_NO_THROW(isDate(std::get<0>(testDate)));
//
//    if (std::get<1>(testDate))
//    {
//      BOOST_TEST(isDate(std::get<0>(testDate)));
//    }
//    else
//    {
//      BOOST_TEST(!isDate(std::get<0>(testDate)));
//    }
//  }
}

BOOST_AUTO_TEST_CASE(parseDate_test_withformat)
{
  using namespace GCL;

  std::vector<std::pair<std::string, std::string>> testDates =
  {
     { "2020-05-30", "%Y-%m-%d" },
     { "2020/05/30", "%Y/%m/%d" },
     { "30/05/2020", "%d/%m/%Y" },
     { "30/5/2020", "%d/%m/%Y" },
     { "30/5/20", "%d/%m/%y" },
     { "30.05.2020", "%d.%m.%Y" },
  };
  date_t dateValue(std::chrono::sys_days(std::chrono::May/30/2020));

  for (auto &testDate: testDates)
  {
    BOOST_REQUIRE_NO_THROW(parseDate(testDate.first, testDate.second));
    BOOST_TEST(dateValue == parseDate(testDate.first, testDate.second));
  }
}

BOOST_AUTO_TEST_CASE(parseDate_test)
{
  using namespace GCL;

  std::vector<std::string> testDates =
  {
     "2020-05-30",
     "2020/05/30",
     "30/05/2020",
     "30/5/2020",
     "30/5/20",
     "30.05.2020",
  };
  date_t dateValue(std::chrono::sys_days(std::chrono::May/30/2020));

  for (auto &testDate: testDates)
  {
    BOOST_REQUIRE_NO_THROW(parseDate(testDate));
    BOOST_TEST(dateValue == parseDate(testDate));
  }

  testDates.clear();
  testDates =
  {
     "2020-5-09",
     "2020/5/9",
     "09/5/2020",
     "09/5/2020",
     "9/5/20",
     "09.05.2020",
  };
  dateValue = std::chrono::sys_days(std::chrono::May/9/2020);

  for (auto &testDate: testDates)
  {
    BOOST_REQUIRE_NO_THROW(parseDate(testDate));
    BOOST_TEST(dateValue == parseDate(testDate));
  }

}

BOOST_AUTO_TEST_CASE(date_t_defaultConstructor)
{

  BOOST_REQUIRE_NO_THROW(GCL::date_t date);
}

//BOOST_AUTO_TEST_CASE(date_t_stringConstructor)
//{
//  using namespace GCL;
//  using namespace std::chrono;
//
//  std::chrono::time_point<std::chrono::system_clock> testResult(2020/May/30);
//
//  for (auto &testDate: dates)
//  {
//    if (testDate.second)
//    {
//      BOOST_REQUIRE_NO_THROW(date_t::date_t(testDate.first));
//
//      date_t test(testDate.first);
//
//      BOOST_TEST(test == testResult);
//    }
//    else
//    {
//      BOOST_REQUIRE_THROW(date_t::date_t(testDate.first), std::runtime_error);
//    }
//  }
//}

//BOOST_AUTO_TEST_CASE(date_t_month)
//{
//  using namespace GCL;
//
//  for (auto &testDate: dates)
//  {
//    if (testDate.second)
//    {
//      BOOST_REQUIRE_NO_THROW(date_t::date_t(testDate.first));
//
//      date_t test(testDate.first);
//
//      BOOST_TEST(test.month == month_t("May"));
//    }
//    else
//    {
//      BOOST_REQUIRE_THROW(date_t::date_t(testDate.first), std::runtime_error);
//    }
//  }
//}

//BOOST_AUTO_TEST_CASE(date_t_year)
//{
//  using namespace GCL;
//
//  for (auto &testDate: dates)
//  {
//    if (testDate.second)
//    {
//      BOOST_REQUIRE_NO_THROW(date_t::date_t(testDate.first));
//
//      date_t test(testDate.first);
//
//      BOOST_TEST(test.year() == year_t(2020));
//    }
//    else
//    {
//      BOOST_REQUIRE_THROW(date_t::date_t(testDate.first), std::runtime_error);
//    }
//  }
//}


BOOST_AUTO_TEST_SUITE_END()
