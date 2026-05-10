#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include <cstdint>
#include <string>
#include <string_view>

#include "include/base32.h"

BOOST_AUTO_TEST_SUITE(base32_test)

BOOST_AUTO_TEST_CASE(test_B32_to_value_char)
{
	using namespace GCL;

	BOOST_TEST(B32_to_value('M') == 12);
}

BOOST_AUTO_TEST_CASE(test_value_to_B32_uint8)
{
	using namespace GCL;

	BOOST_TEST(value_to_B32(static_cast<std::uint8_t>(12)) == 'M');
}


BOOST_AUTO_TEST_CASE(test_B32_to_value_string)
{
  using namespace GCL;

  std::string testString("MZXW6TYB");

  BOOST_TEST(B32_to_value(testString) == 439956229889);
}

BOOST_AUTO_TEST_CASE(test_value_to_B32_uint64)
{
	using namespace GCL;
	union test_t
	{
		char testChar[8];
		std::uint64_t U64;
	};

	test_t testData {.U64 = 0};
	testData.U64 += 'f';
	testData.U64 <<= 8;
	testData.U64 += 'o';
	testData.U64 <<= 8;
	testData.U64 += 'o';
	testData.U64 <<= 8;
	testData.U64 += 'b';
	testData.U64 <<= 8;
	testData.U64 += 'a';

	std::string rv  = value_to_B32(testData.U64);

	BOOST_TEST(rv == std::string("MZXW6YTB"));

}

BOOST_AUTO_TEST_SUITE_END()
