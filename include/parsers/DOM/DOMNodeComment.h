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
    CDOMNodeComment(CDOMNodeBase *p, string_type const &s) : CDOMNodeBase(p), value(s) {}
    virtual ~CDOMNodeComment() = default;

    virtual string_type nodeName() const { return string_type("#comment"); }

    string_type comment() const noexcept { return value; }

    /*! @brief       Returns the type of the node.
     *  @returns     The type of the node.
     */
    virtual nodeType_e nodeType() const noexcept { return COMMENT_NODE; }




  private:
    CDOMNodeComment() = delete;

    string_type value;
  };
}

#endif // PARSERS_HTML_HTMLNODECOMMENT_H
