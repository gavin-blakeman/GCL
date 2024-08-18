/*
 * htmlCollection.hpp
 *
 *  Created on: 17 Aug 2024
 *      Author: gavin
 */

#ifndef PARSERS_DOM_HTMLCOLLECTION_HPP
#define PARSERS_DOM_HTMLCOLLECTION_HPP

// Standard C++ library header files.
#include <memory>
#include <vector>

namespace GCL::parsers::DOM
{
  class CDOMNodeBase;

  class CHTMLCollection
  {
  public:
    CHTMLCollection() = default;
    ~CHTMLCollection() = default;

    std::size_t length() const noexcept { return collection.size(); }
    CDOMNodeBase *item(std::size_t indx) { return collection[indx]; }
    CDOMNodeBase *namedItem(std::string const &) {};


  private:
    std::vector<CDOMNodeBase *> collection;
  };

}

#endif /* PARSERS_DOM_HTMLCOLLECTION_HPP */
