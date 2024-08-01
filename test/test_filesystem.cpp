#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <list>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <fmt/chrono.h>

#include <GCL>

#include "include/filesystem.h"

BOOST_AUTO_TEST_SUITE(sqlWriter_test)

BOOST_AUTO_TEST_CASE(get_tempFileName)
{
  using namespace GCL;

  std::filesystem::path tfn;

  BOOST_REQUIRE_NO_THROW(tfn = temporaryFilename(10));

  // The file should have no extension.

  BOOST_TEST(tfn.extension().empty());
  BOOST_TEST(!tfn.filename().empty());
  BOOST_TEST(!tfn.parent_path().empty());

  // Check length

  std::string temp = tfn.filename().string();
  BOOST_TEST(temp.size() == 10);

  // Check uniqueness and a directory exists.

  BOOST_TEST(std::filesystem::exists(tfn.parent_path()));
  BOOST_TEST(!std::filesystem::exists(tfn));
}

BOOST_AUTO_TEST_CASE(expandFileName_string)
{
  std::string szTest = "Test-{%Y-%m}";
  GCL::date_t date;
  std::string szExpected = fmt::format("Test-{:%Y-%m}", date.date());

  std::string szResult = GCL::expandFileName(szTest).native();

  BOOST_TEST(szResult == szExpected);

  szTest = "Test-{:%Y-%m}";
  szExpected = fmt::format("Test-{:%Y-%m}", date.date());

  szResult = GCL::expandFileName(szTest).native();

  BOOST_TEST(szResult == szExpected);
}

BOOST_AUTO_TEST_SUITE_END()
