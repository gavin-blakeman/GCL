#ifndef GCL_PSTRING_H
#define GCL_PSTRING_H

#include <cstdint>
#include <string>

namespace GCL
{
  template<typename T = std::uint8_t, typename charType = char>
  class pstring
  {
  public:
    struct pstring_t
    {
      T stringLength;
      charType *string;
    };

    pstring() : stringLength(0), string(nullptr) {}
    pstring(pstring const &rhs) : pstring()
    {
      if (rhs.stringLength)
      {
        string = new charType[rhs.stringLength];
        stringLength = rhs.stringLength;
      }
    }
    ~pstring()
    {
      if (string)
      {
        delete string;
        string = nullptr;
      }
    }

    pstring &operator=(std::string const &str)
    {
      if (stringLength != 0)
      {
        delete [] string;
        string = nullptr;
      }
      string = new charType[str.length()];
      stringLength = str.length();
      memcpy(string, str.data(), str.length);
    }


  private:

  };
}


# endif /* GCL_PSTRING_H */
