#ifndef PARSERS_TOKEN_H
#define PARSERS_TOKEN_H

#include <string>

#include <SCL>

#include "include/parsers/tokenType.h"

namespace GCL::parsers
{
  class CToken
  {
    public:
      static SCL::bimap<token_id, std::string> tokenStrings;

      CToken(token_id type, std::size_t row, std::size_t col);
      CToken(token_id type, std::string const &val, std::size_t row, std::size_t col);
      std::string to_string() const;

    private:
      token_id tokenType;
      std::string tokenValue;
      std::size_t tokenRow;
      std::size_t tokenCol;
  };

} // namespace

#endif // PARSERS_TOKEN_H
