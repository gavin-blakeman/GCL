/*
 * DOMNodeList.hpp
 *
 *  Created on: 17 Aug 2024
 *      Author: gavin
 */

#ifndef PARSERS_DOM_DOMNODELIST_HPP
#define PARSERS_DOM_DOMNODELIST_HPP

// Standard C++ library headers
#include <vector>

namespace GCL::parsers::DOM
{
  class CDOMNodeBase;

  class CNodeList
  {
  public:
    using value_type = std::unique_ptr<CDOMNodeBase>;
    using list_type = std::vector<value_type>;
    using iterator = list_type::iterator;
    using const_iterator = list_type::const_iterator;

    CNodeList() = default;
    ~CNodeList() = default;

    CDOMNodeBase *item(std::size_t index) { return nodes[index].get(); }
    std::size_t length() const noexcept { return nodes.size(); }

    bool empty() const noexcept { return nodes.empty(); }

    iterator begin() { return nodes.begin(); }
    const_iterator begin() const { return nodes.cbegin(); }
    iterator end() { return nodes.end(); }
    const_iterator end() const { return nodes.cend(); }

  private:
    list_type nodes;

  };
}


#endif /* PARSERS_DOM_DOMNODELIST_HPP */
