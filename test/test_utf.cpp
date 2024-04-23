#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

// Standard C++ library headers
#include <sstream>
#include <vector>
#include <tuple>

#include "include/utf.h"

BOOST_AUTO_TEST_SUITE(utf_test)

BOOST_AUTO_TEST_CASE(getBom_test)
{
  using namespace GCL;

  std::stringstream strm(std::ios::in | std::ios::out | std::ios::binary);
  std::vector<std::tuple<utf_e, std::vector<std::uint8_t>, bool>> testVector =
  {
    { UTF_8, { 0xEF, 0xBB, 0xBF }, true},
    { UTF_16BE, { 0xFE, 0xFF }, true},
    { UTF_16LE,  { 0xFF, 0xFE }, true},
    { UTF_NONE, { 0xDD, 0x4E }, false },
    { UTF_NONE, { 0xED, 0x67, 0x54, 0x78, 0x88, 0x00, 0x12}, false },
  };


  for (auto const &test: testVector)
  {
    strm.flush();

    for (auto const &val: std::get<1>(test))
    {
      strm << val;
    }

    BOOST_TEST(getBOM(strm) == std::get<0>(test));
  };

}

BOOST_AUTO_TEST_SUITE_END()
