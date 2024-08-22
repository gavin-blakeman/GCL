//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// SUBSYSTEM:           Parsers
// FILE:                htmlTokens.cpp
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
// OVERVIEW:            Class provides a generic token used for lexing files.
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-06-18 GGB - File Created
//
//**********************************************************************************************************************************

#include "include/parsers/html/htmlTokens.h"

// Standard C++ header files

namespace GCL::parsers::html
{
  CHTMLToken::CHTMLToken(token_type tt) : tokenType(tt)
  {
    switch(tokenType)
    {
      case TT_DOCTYPE:
      {
        tokenValue = tokenContentDocType_t();
        break;
      }
      case TT_TAG_START:
      case TT_TAG_END:
      {
        tokenValue = tokenTag_t();
        break;
      }
      case TT_CHARACTER:
      {
        tokenValue = tokenCharacter_t();
        break;
      }
      case TT_COMMENT:
      {
        tokenValue = tokenComment_t();
        break;
      }
      default:
      {
        // No error.
        break;
      }
    }
  }

  CHTMLToken::CHTMLToken(token_type tt, char_type const &c) : tokenType(tt)
  {
    switch(tokenType)
    {
      case TT_DOCTYPE:
      {
        tokenValue = tokenContentDocType_t();
        break;
      }
      case TT_TAG_START:
      case TT_TAG_END:
      {
        tokenValue = tokenTag_t();
        break;
      }
      case TT_CHARACTER:
      {
        tokenValue = tokenCharacter_t({c});
        break;
      }
      case TT_COMMENT:
      {
        tokenValue = tokenComment_t({c});
        break;
      }
      default:
      {
        // No error.
        break;
      }
    }
  }

  void CHTMLToken::forceQuirks(bool b)
  {
    if (tokenType == TT_DOCTYPE)
    {
      if (!std::holds_alternative<tokenContentDocType_t>(tokenValue))
      {
        tokenValue = tokenContentDocType_t();
      }
      tokenContentDocType_t &token = std::get<tokenContentDocType_t>(tokenValue);
      token.forceQuirksFlag = true;
    }
    else
    {
      CODE_ERROR();
    }
  }

  bool CHTMLToken::forceQuirks() const
  {
    RUNTIME_ASSERT(std::holds_alternative<tokenContentDocType_t>(tokenValue), "Incorrect type held");
    RUNTIME_ASSERT(tokenType == TT_DOCTYPE, "Incorrect type held");

    return std::get<tokenContentDocType_t>(tokenValue).forceQuirksFlag;
  }

  bool CHTMLToken::hasPublicIdentifier() const
  {
    RUNTIME_ASSERT(std::holds_alternative<tokenContentDocType_t>(tokenValue), "Incorrect type held");
    RUNTIME_ASSERT(tokenType == TT_DOCTYPE, "Incorrect type held");

    return std::get<tokenContentDocType_t>(tokenValue).publicIdentifier.has_value();
  }

  bool CHTMLToken::hasSystemIdentifier() const
  {
    RUNTIME_ASSERT(std::holds_alternative<tokenContentDocType_t>(tokenValue), "Incorrect type held");
    RUNTIME_ASSERT(tokenType == TT_DOCTYPE, "Incorrect type held");

    return std::get<tokenContentDocType_t>(tokenValue).systemIdentifier.has_value();
    }

  void CHTMLToken::appendName(char_type const &v)
  {
    switch (tokenType)
    {
      case TT_DOCTYPE:
      {
        if (!std::holds_alternative<tokenContentDocType_t>(tokenValue))
        {
          tokenValue = tokenContentDocType_t();
        }
        tokenContentDocType_t &token = std::get<tokenContentDocType_t>(tokenValue);
        token.name.push_back(v);
        break;
      }
      case TT_TAG_START:
      case TT_TAG_END:
      {
        if (!std::holds_alternative<tokenTag_t>(tokenValue))
        {
          tokenValue = tokenTag_t();
        }
        tokenTag_t &token = std::get<tokenTag_t>(tokenValue);
        token.name.push_back(v);
        break;
      }
      default:
      {
        CODE_ERROR();
      }
    }
  }

  CHTMLToken::string_type const &CHTMLToken::name() const
  {
    switch (tokenType)
    {
      case TT_DOCTYPE:
      {
        if (!std::holds_alternative<tokenContentDocType_t>(tokenValue))
        {
          tokenValue = tokenContentDocType_t();
        }
        tokenContentDocType_t &token = std::get<tokenContentDocType_t>(tokenValue);
        return token.name;
        break;
      }
      case TT_TAG_START:
      case TT_TAG_END:
      {
        if (!std::holds_alternative<tokenTag_t>(tokenValue))
        {
          tokenValue = tokenTag_t();
        }
        tokenTag_t &token = std::get<tokenTag_t>(tokenValue);
        return token.name;
        break;
      }
      default:
      {
        CODE_ERROR();
      }
    }
  }

  void CHTMLToken::appendPublicIdentifier(char_type const &v)
  {
    if (tokenType == TT_DOCTYPE)
    {
      if (!std::holds_alternative<tokenContentDocType_t>(tokenValue))
      {
        tokenValue = tokenContentDocType_t();
      }
      tokenContentDocType_t &token = std::get<tokenContentDocType_t>(tokenValue);
      if (!token.publicIdentifier)
      {
        token.publicIdentifier = string_type{v};
      }
      else
      {
        (*token.publicIdentifier).push_back(v);
      }
    }
    else
    {
      CODE_ERROR();
    }
  }

  void CHTMLToken::appendSystemIdentifier(char_type const &v)
  {
    if (tokenType == TT_DOCTYPE)
    {
      if (!std::holds_alternative<tokenContentDocType_t>(tokenValue))
      {
        tokenValue = tokenContentDocType_t();
      }
      tokenContentDocType_t &token = std::get<tokenContentDocType_t>(tokenValue);
      if (!token.publicIdentifier)
      {
        token.systemIdentifier = string_type(1, v);
      }
      else
      {
        (*token.systemIdentifier).push_back(v);
      }
    }
    else
    {
      CODE_ERROR();
    }
  }

  bool CHTMLToken::isSelfClosing() const
  {
    if (tokenType == TT_TAG_START)
    {
      if (!std::holds_alternative<tokenTag_t>(tokenValue))
      {
        tokenValue = tokenTag_t{};
      }
      tokenTag_t &token = std::get<tokenTag_t>(tokenValue);
      return token.selfClosingFlag;
    }
    else
    {
      CODE_ERROR();
    }
  }

  void CHTMLToken::type(token_type tt)
  {
    tokenType = tt;

    switch(tokenType)
    {
      case TT_DOCTYPE:
      {
        if (!std::holds_alternative<tokenContentDocType_t>(tokenValue))
        {
          tokenValue = tokenContentDocType_t();
        }
        break;
      }
      case TT_TAG_START:
      case TT_TAG_END:
      {
        if (!std::holds_alternative<tokenTag_t>(tokenValue))
        {
          tokenValue = tokenTag_t();
        }
        break;
      }
      case TT_CHARACTER:
      {
        if (!std::holds_alternative<tokenCharacter_t>(tokenValue))
        {
          tokenValue = tokenCharacter_t();
        }
        break;
      }
      case TT_COMMENT:
      {
        if (!std::holds_alternative<tokenComment_t>(tokenValue))
        {
          tokenValue = tokenComment_t();
        }
        break;
      }
      case TT_EOF:
      case TT_NONE:
      {
        if (!std::holds_alternative<std::monostate>(tokenValue))
        {
          tokenValue = std::monostate{};
        }
        break;
      }
      default:
      {
        CODE_ERROR();
      }
    }
  }

  void CHTMLToken::selfClosing(bool b)
  {
    if (tokenType == TT_TAG_START)
    {
      if (!std::holds_alternative<tokenTag_t>(tokenValue))
      {
        tokenValue = tokenTag_t{};
      }
      tokenTag_t &token = std::get<tokenTag_t>(tokenValue);
      token.selfClosingFlag = true;
    }
    else
    {
      CODE_ERROR();
    }
  }

  void CHTMLToken::appendData(char_type const &v)
  {
    switch (tokenType)
    {
      case TT_CHARACTER:
      {
        if (!std::holds_alternative<tokenCharacter_t>(tokenValue))
        {
          tokenValue = tokenCharacter_t(v);
        }
        else
        {
          std::get<tokenCharacter_t>(tokenValue).data = v;
        }
        break;
      }
      case TT_COMMENT:
      {
        if (!std::holds_alternative<tokenComment_t>(tokenValue))
        {
          tokenValue = tokenComment_t();
        }
        tokenComment_t &token = std::get<tokenComment_t>(tokenValue);
        token.data.push_back(v);
        break;
      }
      default:
      {
        CODE_ERROR();
      }
    }
  }

  void CHTMLToken::appendData(string_type const &s)
  {
    switch (tokenType)
    {
      case TT_COMMENT:
      {
        if (!std::holds_alternative<tokenComment_t>(tokenValue))
        {
          tokenValue = tokenComment_t();
        }
        tokenComment_t &token = std::get<tokenComment_t>(tokenValue);
        token.data.append(s);
        break;
      }
      default:
      {
        CODE_ERROR();
      }
    }
  }

  void CHTMLToken::setSystemIdentifierEmpty()
  {
    if (tokenType == TT_DOCTYPE)
    {
      if (!std::holds_alternative<tokenContentDocType_t>(tokenValue))
      {
        tokenValue = tokenContentDocType_t();
      }
      tokenContentDocType_t &token = std::get<tokenContentDocType_t>(tokenValue);
      token.systemIdentifier = string_type({});

    }
    else
    {
      CODE_ERROR();
    }
  }

  void CHTMLToken::setPublicIdentifierEmpty()
  {
    if (tokenType == TT_DOCTYPE)
    {
      if (!std::holds_alternative<tokenContentDocType_t>(tokenValue))
      {
        tokenValue = tokenContentDocType_t();
      }
      tokenContentDocType_t &token = std::get<tokenContentDocType_t>(tokenValue);
      token.publicIdentifier = string_type({});
    }
    else
    {
      CODE_ERROR();
    }
  }

  std::ostream &operator<<(std::ostream &os, CHTMLToken const &t)
  {
    os << std::endl;
    return os;
  }

} // namespace
