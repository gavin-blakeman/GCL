#ifndef BITVALUE_H
#define BITVALUE_H

  // Standard C++ library

#include <cstdint>

namespace GCL
{
  class bitValue
  {
  private:
    std::uint64_t value;
    std::uint8_t nob;

  protected:
  public:
    bitValue() : value(0), nob(0) {}
    bitValue(bitValue const &other) : value(other.value), nob(other.nob) {}
    bitValue(std::uint64_t v, std::uint8_t b) : value(v), nob(b) {}

    bitValue &operator=(bitValue const &other)
    {
      if (this != &other)
      {
        value = other.value;
        nob = other.nob;
      }
      return *this;
    }

    operator bool() const { return (value != 0); }
    operator std::uint64_t() const { return value; }
    std::uint64_t operator()() const { return value; }
  };
}

#endif // BITVALUE_H
