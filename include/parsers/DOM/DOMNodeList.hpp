//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// SUBSYSTEM:           DOM
// FILE:                DOMNodeList.hpp
// LANGUAGE:            C++
// TARGET OS:           None.
// NAMESPACE:           GCL
// AUTHOR:              Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2024 Gavin Blakeman.
//                      This file is part of the General Class Library (GCL)
//
//                      GCL is free software: you can redistribute it and/or modify it under the terms of the GNU General
//                      Public License as published by the Free Software Foundation, either version 2 of the License, or
//                      (at your option) any later version.
//
//                      GCL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
//                      implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
//                      for more details.
//
//                      You should have received a copy of the GNU General Public License along with GCL.  If not,
//                      see <http://www.gnu.org/licenses/>.
//
// OVERVIEW:            Class that represents the DOM.
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-08-17 GGB - File Created
//
//**********************************************************************************************************************************/

#ifndef PARSERS_DOM_DOMNODELIST_HPP
#define PARSERS_DOM_DOMNODELIST_HPP

// Standard C++ library headers
#include <memory>
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

    void push_back(value_type &&val)
    {
      nodes.push_back(std::move(val));
    }

  private:
    list_type nodes;

  };
}


#endif /* PARSERS_DOM_DOMNODELIST_HPP */
