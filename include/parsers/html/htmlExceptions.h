#ifndef PARSERS_HTML_HTMLEXCEPTION_H
#define PARSERS_HTML_HTMLEXCEPTION_H

// Standard C++ library header files
#include <stdexcept>
#include <string>

namespace GCL::parsers::html
{
  class html_error : public std::runtime_error
  {
  public:
    html_error() : std::runtime_error("HTML Error") {}
    html_error(std::string const &s) : std::runtime_error(s) {}
  };

  class parse_error : public html_error
  {
  public:
    parse_error() : html_error("parse error") {}
    parse_error(std::string const &s) : html_error(s) {}
  };

  inline void PARSE_ERROR(std::string const &s) { throw parse_error(s); }
}

#endif // PARSERS_HTML_HTMLEXCEPTION_H
