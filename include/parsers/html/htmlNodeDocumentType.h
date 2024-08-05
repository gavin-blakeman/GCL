#ifndef PARSERS_HTML_HTMLNODEDOCUMENTTYPE_H
#define PARSERS_HTML_HTMLNODEDOCUMENTTYPE_H

// Standard C++ library header files
#include <optional>
#include <string>

namespace GCL::parsers::html
{
  class CHTMLDocType final : public CHTMLNodeBase
  {
   public:
    using nodeType_t = CHTMLNodeBase::nodeType_t;

    CHTMLDocType() = default;

    void name(std::string const &n) { name_ = n; }
    std::string const &name() const { return name_; }

    void publicID(std::string const &pid) { publicIdentifier_ = pid; }
    std::string const &publicID() const { if (publicIdentifier) return publicIdentifier_; else  return std::string(); }

    virtual nodeType_t nodeType() const noexcept { return 1; }

   private:
    CHTMLDocType(CHTMLDocType const &) = delete;
    CHTMLDocType(CHTMLDocType &&) = delete;
    CHTMLDocType &operator=(CHTMLDocType const &) = delete;
    CHTMLDocType &operator=(CHTMLDocType &&) = delete;

     std::optional<std::string> name_;
     std::optional<std::string> publicIdentifier_;
     std::optional<std::string> systemIdentifier_;
     bool force_quirks_flag = false;
  }
}

#endif // PARSERS_HTML_HTMLNODEDOCUMENTTYPE_H
