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
    CDOMNodeDocType(CDOMNodeBase *parent, string_type const &, string_type const &, string_type const &);
    virtual ~CDOMNodeDocType() = default;

    virtual nodeType_e nodeType() const noexcept {return DOCUMENT_TYPE_NODE; }

    void name(string_type const &n) { name_ = n; }
    string_type const &name() const noexcept { return name_; }

    void publicIdentifier(string_type const &pid) { publicIdentifier_ = pid; }
    string_type const &publicIdentifier() const noexcept { return publicIdentifier_; }

    void systemIdentifier(string_type const &sid) { systemIdentifier_ = sid; }
    string_type const &systemIidentifier() const noexcept { return systemIdentifier_; }

   private:
    CDOMNodeDocType() = delete;
    CDOMNodeDocType(CDOMNodeDocType const &) = delete;
    CDOMNodeDocType(CDOMNodeDocType &&) = delete;
    CDOMNodeDocType &operator=(CDOMNodeDocType const &) = delete;
    CDOMNodeDocType &operator=(CDOMNodeDocType &&) = delete;

     string_type name_;
     string_type publicIdentifier_;
     string_type systemIdentifier_;
  };

}

#endif // PARSERS_HTML_HTMLNODEDOCUMENTTYPE_H
