#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include <set>
#include <string>

#include "include/functions.h"

BOOST_AUTO_TEST_SUITE(functions_test)

BOOST_AUTO_TEST_CASE(test_includeExclude)
{
  using namespace GCL;

  std::set<int> include = { 1, 2, 3};
  std::set<int> exclude = { 3, 4, 5};

  BOOST_TEST(includeExclude(include, exclude, 1));    // Include
  BOOST_TEST(includeExclude(include, exclude, 3));    // Include 3 in both. Should return true.
  BOOST_TEST(!includeExclude(include, exclude, 4));   // Exclude
  BOOST_TEST(!includeExclude(include, exclude, 7));   // Not in either, exclude.

  include.clear();
  BOOST_TEST(includeExclude(include, exclude, 1));    // Include, not excluded
  BOOST_TEST(!includeExclude(include, exclude, 3));    // Exclude, empty include
  BOOST_TEST(!includeExclude(include, exclude, 4));   // Exlude
  BOOST_TEST(includeExclude(include, exclude, 7));   // Include, empty include.
}

BOOST_AUTO_TEST_SUITE_END()
