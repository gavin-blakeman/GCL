

#include <SCL>

#include "include/parsers/tokenType.h"

namespace GCL::parsers
{
  enum htmlTokenTypes : token_id
  {
    L_TAG_OPEN = TT_NEXT,     // <
    L_TAG_CLOSE,              // </
    R_TAG_OPEN,               // >
    R_TAG_CLOSE,              // />
    L_TAG_DOCTYPE,            // <!
    COMMENT_OPEN,             // <!---
    COMMENT_CLOSE,            // --->
    ASSIGN,                   // =
    ID,
    VALUE,
    TEXT,
  };
} // namespace
