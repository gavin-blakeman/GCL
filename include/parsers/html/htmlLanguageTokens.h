//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// SUBSYSTEM:           Parsers::HTML Parser
// FILE:                htmlTokens.h
// LANGUAGE:            C++
// TARGET OS:           None.
// NAMESPACE:           GCL
// AUTHOR:              Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2024 Gavin Blakeman.
//                      This file is part of the General Class Library (GCL)
//
//                      GCL is free software: you can redistribute it and/or modify it under the terms of the GNU General
//                      Public License as published by the Free Software Foundation, either version 2 of the License, or
//                      (at your option) any later version.
//
//                      GCL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
//                      implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
//                      for more details.
//
//                      You should have received a copy of the GNU General Public License along with GCL.  If not,
//                      see <http://www.gnu.org/licenses/>.
//
// OVERVIEW:            Class that parsers the tokens
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-08-05 GGB - File Created
//
//**********************************************************************************************************************************/

#ifndef PARSERS_HTML_TOKENS_H
#define PARSERS_HTML_TOKENS_H

// parsers library
#include "include/parsers/token.h"
#include "include/parsers/languageTokens.h"

namespace GCL::parsers::html
{
  enum htmlTokenTypes : CToken::tokenID_t
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
    ATTRIBUTE,
  };

}

#endif // PARSERS_HTML_TOKENS_H
