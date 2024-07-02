#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include <vector>
#include <string>
#include <tuple>

#include "include/dateTime.h"

BOOST_AUTO_TEST_SUITE(dateTime_test)

std::vector<std::tuple<std::string, bool, std::string, std::chrono::year, std::chrono::month, std::chrono::day>> dates =
{
  { "2020-05-30", true, "YYYY-MM-DD", std::chrono::year(2020), std::chrono::month(5), std::chrono::day(30) },
  //{ "This is a test", false, "", 0, 0, 0 },
//  { "2020/05/30", true, "YYYY/MM/DD", 2020, 5, 30 },
//  { "30/05/2020", true, "DD/MM/YYYY", 2020, 5, 30 },
//  { "30/5/2020", true, "DD/M/YYYY", 2020, 5, 30 },
//  { "30/5/20", true, "DD/M/YY", 2020, 5, 30 },
  //{ "2021-02-29", false, "", 0, 0, 0 },     // Invalid date. Not 29 days in feb 2021.
};

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

  for (auto &testDate: dates)
  {
    BOOST_REQUIRE_NO_THROW(isDate(std::get<0>(testDate)));

    if (std::get<1>(testDate))
    {
      BOOST_TEST(isDate(std::get<0>(testDate)));
    }
    else
    {
      BOOST_TEST(!isDate(std::get<0>(testDate)));
    }
  }
}

//BOOST_AUTO_TEST_CASE(parseDate_test)
//{
//  using namespace GCL;
//
//  for (auto &testDate: dates)
//  {
//    if (std::get<1>(testDate))
//    {
//      BOOST_REQUIRE_NO_THROW(parseDate(std::get<0>(testDate)));
//      date_t date = std::get<0>(testDate);
//      std::chrono::year_month_day ymd{std::get<3>(testDate), std::get<4>(testDate), std::get<5>(testDate)};
//      BOOST_TEST(date == ymd);
//    }
//    else
//    {
//      BOOST_REQUIRE_THROW(parseDate(std::get<0>(testDate)), std::runtime_error);
//    }
//  }
//}

//BOOST_AUTO_TEST_CASE(parseDate_withFormat)
//{
//  using namespace GCL;
//
//  std::chrono::time_point<std::chrono::system_clock> testResult(2020/May/30);
//
//  for (auto &testDate: dates)
//  {
//    if (testDate.second)
//    {
//      BOOST_REQUIRE_NO_THROW(parseDate(std::get<0>(testDate)));
//      BOOST_TEST(parseDate(std::get<0>(testDate), "YYYY-MM-DD") == testResult);
//    }
//    else
//    {
//      BOOST_REQUIRE_THROW(parse(testDate.first), std::runtime_error);
//    }
//  }
//}

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
