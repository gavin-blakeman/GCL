#ifndef GCL_PARSERS_TOKENTYPE_H
#define GCL_PARSERS_TOKENTYPE_H

#include <cstdint>
#include <string>


namespace GCL::parsers
{
  using token_id = std::uint8_t;
  using token_type = std::string;

  enum ETokenType : token_id
  {
    TT_EOF,
    TT_NEXT,
  };

} // namespace


#endif // GCL_PARSERS_TOKENTYPE_H
