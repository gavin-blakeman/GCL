#ifndef PARSERS_HTML_HTMLNODECOMMENT_H
#define PARSERS_HTML_HTMLNODECOMMENT_H

// Standard C++ header files
#include <string>

// GCL header files
#include "include/parsers/DOM/DOMNodeBase.h"

namespace GCL::parsers::DOM
{
  class CDOMNodeComment : public CDOMNodeBase
  {
    public:
    CDOMNodeComment(CDOMNodeComment *p, string_type const &s) : CDOMNodeBase(p), value(s) {}
      virtual ~CDOMNodeComment() = default;

      string_type comment() const noexcept { return value; }


    private:
      CDOMNodeComment() = delete;

      string_type value;
  };
}

#endif // PARSERS_HTML_HTMLNODECOMMENT_H
