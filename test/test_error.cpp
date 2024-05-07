#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include <list>
#include <string>
#include <vector>

#include "include/error.h"

BOOST_AUTO_TEST_SUITE(error_test)

BOOST_AUTO_TEST_CASE(SIGNAL_test)
{
  using namespace GCL;

  BOOST_REQUIRE_THROW(SIGNAL(1), CSignal);

  try
  {
    SIGNAL(2);
  }
  catch(CSignal const&s)
  {
    BOOST_TEST(s.signal() == 2);
  }
}



BOOST_AUTO_TEST_SUITE_END()
