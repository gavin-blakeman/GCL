#ifndef GCL_PARSERS_HTMLLEXER_H
#define GCL_PARSERS_HTMLLEXER_H

#include "include/parsers/lexer.h"

namespace GCL::parsers
{
  class CHTMLLexer : public CLexer
  {
    public:
      CHTMLLexer(std::istream &is);
      virtual ~CHTMLLexer() = default;

    private:
      CHTMLLexer() = delete;
      CHTMLLexer(CHTMLLexer const &) = delete;
      CHTMLLexer(CHTMLLexer &&) = delete;
      CHTMLLexer &operator=(CHTMLLexer const &) = delete;
      CHTMLLexer &operator=(CHTMLLexer &&) = delete;

      virtual void next() override;

      void id();
      void whitespace();
      void tag();
      void comment();
      void value();
      void text();

  };

} // namesapce

#endif
