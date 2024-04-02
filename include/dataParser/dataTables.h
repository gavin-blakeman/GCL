//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                dataParser/dataParser_SAPBKG.h
// SUBSYSTEM:           Data File Parser
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
// OVERVIEW:            Private class: Implementation of dataTables.
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-04-02 GGB - File Created
//
//**********************************************************************************************************************************

#ifndef GCL_DATAPARSER_DATATABLES_H
#define GCL_DATAPARSER_DATATABLES_H

// Standard C++ header files

#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include <utility>

namespace GCL::dataParser   // Intended to not be visible except though the dataParser class.
{
  using field_t = std::string;
  using dataLine_t = std::pair<std::size_t, std::vector<field_t>>;
  using dataFile_t = std::vector<dataLine_t>;
  struct dataTable_t
  {
    std::optional<std::string> tableName;
    dataLine_t headings;
    dataFile_t data;
  };

  class CDataTables
  {
  public:
    using value_type = dataTable_t;
    using reference = value_type &;
    using const_reference = value_type const &;
    using collection_type = std::vector<value_type>;
    using size_type = collection_type::size_type;
    using iterator = collection_type::iterator;
    using const_iterator = collection_type::const_iterator;
    using reverse_iterator = collection_type::reverse_iterator;

    CDataTables() = default;
    ~CDataTables() = default;
    CDataTables(CDataTables const &) = default;
    CDataTables(CDataTables &&) = default;
    CDataTables &operator=(CDataTables const &) = default;
    CDataTables &operator=(CDataTables &&) = default;
    CDataTables(dataTable_t &&);

    value_type &operator[](size_type);

    bool empty() const noexcept { return dataTables.empty(); }
    size_type size() const noexcept { return dataTables.size(); }

    void clear() noexcept { dataTables.clear(); }

    reference front() { return dataTables.front(); }
    reference back() { return dataTables.back(); }
    const_reference front() const { return dataTables.front(); }
    const_reference back() const { return dataTables.back(); }

    void push_back(value_type &);
    void push_back(value_type &&);



    iterator begin() { return dataTables.begin(); }
    iterator end() { return dataTables.end(); }
    const_iterator begin() const { return dataTables.cbegin(); }
    const_iterator end() const { return dataTables.cend(); }
    const_iterator cbegin() const { return dataTables.cbegin(); }
    const_iterator cend() const { return dataTables.cend(); }
    reverse_iterator rbegin() { return dataTables.rbegin(); }
    reverse_iterator rend() { return dataTables.rend(); }


  protected:
    std::vector<dataTable_t> dataTables;
  private:
  };

}

#endif // GCL_DATAPARSER_DATATABLES_H
