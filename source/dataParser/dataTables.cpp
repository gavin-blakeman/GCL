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

#include "include/dataParser/dataTables.h"

#include "include/error.h"

namespace GCL::dataParser
{
  /// @brief      Constructor taking a datatable.
  /// @param[in]  dataTable: The dataTableto construct with.
  /// @throws
  /// @version    2024-04-02/GGB - Function created.

  CDataTables::CDataTables(dataTable_t &&dataTable)
  {
    dataTables.push_back(std::move(dataTable));
  }

  /// @brief      Operator []. Returns a dataTable. Throws RUNTIME_ASSERT if the indx is out of range.
  /// @param[in]  indx: The index of the table to retrieve.
  /// @throws     CRuntimeAssert
  /// @version    2024-04-02/GGB - Function created.

  CDataTables::value_type &CDataTables::operator[](size_type indx)
  {
    RUNTIME_ASSERT(indx < dataTables.size(), "Invalid index.");

    return dataTables[indx];
  }

  /// @brief      Copy inserts a record to the back of the collection.
  /// @param[in]  record: The record to insert at the back.
  /// @throws
  /// @version    2024-04-02/GGB - Function created.

  void CDataTables::push_back(value_type &record)
  {
    dataTables.push_back(record);
  }

  /// @brief      Move inserts a record to the back of the collection.
  /// @param[in]  record: The record to insert at the back.
  /// @throws
  /// @version    2024-04-02/GGB - Function created.

  void CDataTables::push_back(value_type &&record)
  {
    dataTables.push_back(record);
  }
}
