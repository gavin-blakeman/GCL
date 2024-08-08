#ifndef GCL_PARSERS_HTML_NODETYPES_H
#define GCL_PARSERS_HTML_NODETYPES_H

// Standard C++ library header files.
#include <cstdint>

namespace GCL::parsers::html
{
  using nodeType_t = unsigned int;

  constexpr nodeType_t NT_BASE = 0;
  constexpr nodeType_t NT_DOCTYPE = 1;
  constexpr nodeType_t NT_ELEMENT = 2;
  constexpr nodeType_t NT_COMMENT = 3;
}


#endif // GCL_PARSERS_HTML_NODETYPES_H
