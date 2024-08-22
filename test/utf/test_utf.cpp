#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

// Standard C++ library headers
#include <iostream>
#include <sstream>
#include <vector>
#include <tuple>

#include "include/utf/utf.h"

std::ostream &operator<<(std::ostream &os, std::vector<char8_t> const &vec)
{
  bool first = true;
  os << "{ ";
  for (auto v: vec)
  {
    if (first)
    {
     first = false;
    }
    else
    {
      os << ", ";
    }
    os << std::to_string(v);
  }
  os << " }";
  return os;
}

std::ostream &operator<<(std::ostream &os, std::vector<char16_t> const &vec)
{
  bool first = true;
  os << "{ ";
  for (auto v: vec)
  {
    if (first)
    {
     first = false;
    }
    else
    {
      os << ", ";
    }
    os << std::to_string(v);
  }
  os << " }";
  return os;
}

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

BOOST_AUTO_TEST_CASE(test_decodeUTF8)
{
  using namespace GCL;

  std::vector<std::uint8_t> testVector;
  std::uint32_t codePoint;

  testVector = {0x24};
  decodeUTF(testVector.begin(), testVector.end(), codePoint);
  BOOST_TEST(codePoint == 0x24);

  testVector = { 0xC2, 0xA3 };
  decodeUTF(testVector.begin(), testVector.end(), codePoint);
  BOOST_TEST(codePoint == 0xA3);

  testVector = { 0xD0, 0x98 };
  decodeUTF(testVector.begin(), testVector.end(), codePoint);
  BOOST_TEST(codePoint == 0x0418);

  testVector = { 0xE0, 0xA4, 0xB9 };
  decodeUTF(testVector.begin(), testVector.end(), codePoint);
  BOOST_TEST(codePoint == 0x0939);

  testVector = { 0xE2, 0x82, 0xAC };
  decodeUTF(testVector.begin(), testVector.end(), codePoint);
  BOOST_TEST(codePoint == 0x20AC);

  testVector = { 0xED, 0x95, 0x9C };
  decodeUTF(testVector.begin(), testVector.end(), codePoint);
  BOOST_TEST(codePoint == 0xD55C);

  testVector = { 0xF0, 0x90, 0x8D, 0x88 };
  decodeUTF(testVector.begin(), testVector.end(), codePoint);
  BOOST_TEST(codePoint == 0x10348);

  testVector = { 0xF4, 0x89, 0x9A, 0xB3 };
  decodeUTF(testVector.begin(), testVector.end(), codePoint);
  BOOST_TEST(codePoint == 0x1096B3);
}

BOOST_AUTO_TEST_CASE(test_decodeUTF16)
{
  using namespace GCL;

  std::vector<char16_t> testVector;
  std::uint32_t codePoint;

  testVector = {0x24};
  decodeUTF(testVector.begin(), testVector.end(), codePoint);
  BOOST_TEST(codePoint == 0x0024);

  testVector = {0x20AC};
  decodeUTF(testVector.begin(), testVector.end(), codePoint);
  BOOST_TEST(codePoint == 0x20AC);

  testVector = {0xD801, 0xDC37};
  decodeUTF(testVector.begin(), testVector.end(), codePoint);
  BOOST_TEST(codePoint == 0x10437);

  testVector = {0xD852, 0xDF62};
  decodeUTF(testVector.begin(), testVector.end(), codePoint);
  BOOST_TEST(codePoint == 0x24B62);
}

BOOST_AUTO_TEST_CASE(test_decodeUTF32)
{
  using namespace GCL;

  std::vector<char32_t> testVector;
  std::uint32_t codePoint;

  testVector = {0xD55C};
  decodeUTF(testVector.begin(), testVector.end(), codePoint);
  BOOST_TEST(codePoint == 0xD55C);
}

BOOST_AUTO_TEST_CASE(test_encodeUTF8)
{
  using namespace GCL;

  std::vector<char8_t> resultVector;
  std::vector<char8_t> testVector;

  encodeUTF(std::uint32_t(0x0024), resultVector);
  testVector = { 0x24};
  BOOST_TEST(testVector == resultVector);

  resultVector.clear();
  encodeUTF(std::uint32_t(0x00A3), resultVector);
  testVector = { 0xC2, 0xA3 };
  BOOST_TEST(testVector == resultVector);

  resultVector.clear();
  encodeUTF(std::uint32_t(0x0418), resultVector);
  testVector = { 0xD0, 0x98 };
  BOOST_TEST(testVector == resultVector);

  resultVector.clear();
  encodeUTF(std::uint32_t(0x0939), resultVector);
  testVector = { 0xE0, 0xA4, 0xB9 };
  BOOST_TEST(testVector == resultVector);

  resultVector.clear();
  encodeUTF(std::uint32_t(0x20AC), resultVector);
  testVector = { 0xE2, 0x82, 0xAC };
  BOOST_TEST(testVector == resultVector);

  resultVector.clear();
  encodeUTF(std::uint32_t(0xD55C), resultVector);
  testVector = { 0xED, 0x95, 0x9C };
  BOOST_TEST(testVector == resultVector);

  resultVector.clear();
  encodeUTF(std::uint32_t(0x10348), resultVector);
  testVector = { 0xF0, 0x90, 0x8D, 0x88 };
  BOOST_TEST(testVector == resultVector);

  resultVector.clear();
  encodeUTF(std::uint32_t(0x1096B3), resultVector);
  testVector = { 0xF4, 0x89, 0x9A, 0xB3};
  BOOST_TEST(testVector == resultVector);
}

BOOST_AUTO_TEST_CASE(test_encodeUTF16)
{
  using namespace GCL;

  std::vector<char16_t> resultVector;
  std::vector<char16_t> testVector;

  encodeUTF(std::uint32_t(0x0024), resultVector);
  testVector = {0x0024};
  BOOST_TEST(testVector == resultVector);

  resultVector.clear();
  encodeUTF(std::uint32_t(0x20AC), resultVector);
  testVector = {0x20AC};
  BOOST_TEST(testVector == resultVector);

  resultVector.clear();
  encodeUTF(std::uint32_t(0x10437), resultVector);
  testVector = {0xD801, 0xDC37};
  BOOST_TEST(testVector == resultVector);

  resultVector.clear();
  encodeUTF(std::uint32_t(0x24B62), resultVector);
  testVector = {0xD852, 0xDF62 };
  BOOST_TEST(testVector == resultVector);
}

BOOST_AUTO_TEST_CASE(test_encodeUTF32)
{
  using namespace GCL;

  std::vector<char32_t> testVector;
  std::vector<char32_t> resultVector;

  encodeUTF(0x20AC, testVector);
  resultVector = { 0x20AC };
  BOOST_TEST(testVector == resultVector);
}

BOOST_AUTO_TEST_SUITE_END()
