#include "include/parsers/token.h"

#include <string>

namespace GCL::parsers
{

  SCL::bimap<token_id, std::string> CToken::tokenStrings;

  CToken::CToken(token_id type, std::string const &val, std::size_t row, std::size_t col)
   {
    tokenType = type;
    tokenValue = val;
    tokenRow = row;
    tokenCol = col;
  }

  std::string CToken::to_string() const
  {
    return std::string ("(" + std::to_string(tokenRow) + ", " + std::to_string(tokenCol) + ")"
                        + " " + (tokenStrings.contains_LHS(tokenType) ? tokenStrings.RHS(tokenType) : std::to_string(tokenType))
                        + " " + tokenValue);
  }


} // namespace
