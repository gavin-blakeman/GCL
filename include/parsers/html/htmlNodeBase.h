#ifndef PARSERS_HTML_HTMLNODEBASE_H
#define PARSERS_HTML_HTMLNODEBASE_H

namespace GCL::parsers::html
{
  class CHTMLNodeBase
  {
    public:
      CHTMLNodeBase(CHTMLNodeBase *parent) : parentNode(parent) {}
      virtual ~CHTMLNodeBase() = default;

    private:
      CHTMLNodeBase() = delete;
      CHTMLNodeBase(CHTMLNodeBase const &) = delete;
      CHTMLNodeBase(CHTMLNodeBase &&) = delete;
      CHTMLNodeBase &operator=(CHTMLNodeBase const &) = delete;
      CHTMLNodeBase &operator=(CHTMLNodeBase &&) = delete;

      CHTMLNodeBase *parentNode = nullptr;
  };
}

#endif // PARSERS_HTML_HTMLNODEBASE_H
