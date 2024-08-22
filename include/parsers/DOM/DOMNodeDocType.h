#ifndef PARSERS_HTML_HTMLNODEDOCUMENTTYPE_H
#define PARSERS_HTML_HTMLNODEDOCUMENTTYPE_H

// Standard C++ library header files
#include <optional>
#include <string>

// Parsers library
#include "include/parsers/DOM/DOMNodeBase.h"

namespace GCL::parsers::DOM
{
  class CDOMNodeDocType final : public CDOMNodeBase
  {
   public:
    CDOMNodeDocType() = default;

    void name(string_type const &n) { name_ = n; }
    string_type const &name() const noexcept { return name; }

    void publicID(string_type const &pid) { publicIdentifier = pid; }
    string_type const &publicID() const noexcept { return publicIdentifier; }

    void systemID(string_type const &sid) { systemIdentifier = sid; }
    string_type const &systemID() const noexcept { return systemIdentifier; }

   private:
    CDOMNodeDocType(CDOMNodeDocType const &) = delete;
    CDOMNodeDocType(CDOMNodeDocType &&) = delete;
    CDOMNodeDocType &operator=(CDOMNodeDocType const &) = delete;
    CDOMNodeDocType &operator=(CDOMNodeDocType &&) = delete;

     string_type name_;
     string_type publicIdentifier;
     string_type systemIdentifier;
  }
}

#endif // PARSERS_HTML_HTMLNODEDOCUMENTTYPE_H
