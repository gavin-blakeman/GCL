#ifndef PARSERS_DOM_DOMEXCEPTION_H
#define PARSERS_DOM_DOMEXCEPTION_H

// Standard C++ library header files
#include <stdexcept>
#include <string>

namespace GCL::parsers::DOM
{
  class dom_error : public std::runtime_error
  {
  public:
    dom_error() : std::runtime_error("DOM Error") {}
    dom_error(std::string const &s) : std::runtime_error(s) {}
  };

//  class parse_error : public dom_error
//  {
//  public:
//    parse_error() : dom_error("parse error") {}
//    parse_error(std::string const &s) : dom_error(s) {}
//  };
//
//  inline void PARSE_ERROR(std::string const &s) { throw parse_error(s); }
}

#endif // PARSERS_DOM_DOMEXCEPTION_H
