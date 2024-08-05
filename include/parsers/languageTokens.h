#ifndef PARSERS_TOKENTYPES_H
#define PARSERS_TOKENTYPES_H

// Standard C+ library header files
#include <cstdint>

// parsers library
#include "include/parsers/token.h"

namespace GCL::parsers
{
  enum tokenType_t : CToken::tokenID_t { TT_NOP, TT_EOF, TT_NEXT };
}

#endif // PARSERS_TOKENTYPES_H
