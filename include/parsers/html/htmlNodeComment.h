#ifndef PARSERS_HTML_HTMLNODECOMMENT_H
#define PARSERS_HTML_HTMLNODECOMMENT_H

// Standard C++ header files
#include <string>

// GCL header files
#include "include/parsers/html/htmlNodeBase.h"

namespace GCL::parsers::html
{
  class CHTMLNodeComment : public CHTMLNodeBase
  {
    public:
      CHTMLNodeComment(CHTMLNodeBase *p, std::string const &s) : CHTMLNodeBase(p), value(s) {}
      virtual ~CHTMLNodeComment() = default;

      std::string comment() const noexcept { return value; }

      virtual nodeType_t nodeType() const noexcept { return NT_COMMENT; }

    private:
      CHTMLNodeComment() = delete;

      std::string value;
  };
}

#endif // PARSERS_HTML_HTMLNODECOMMENT_H
