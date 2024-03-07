#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include <list>
#include <string>
#include <vector>

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

BOOST_AUTO_TEST_SUITE_END()
