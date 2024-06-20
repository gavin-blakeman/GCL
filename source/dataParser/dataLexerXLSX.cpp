//**********************************************************************************************************************************
//
// PROJECT:             General Class Library
// FILE:                dataParser/dataLexerXLSX.cpp
// SUBSYSTEM:           Excel File Parser
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
// OVERVIEW:            Implementation of a configuration file reader.
//
// CLASSES INCLUDED:
//
// HISTORY:             2024-03-14 GGB - File Created
//
//**********************************************************************************************************************************

#include "include/dataParser/dataLexerXLSX.h"



namespace GCL
{
  /// @brief      Class constructor.
  /// @param[in]  dt: The data tables to populate with the information from the excel workbook.
  /// @param[in]  fn: The file name of the excel file.s
  /// @throws     None.

  CDataLexerXLSX::CDataLexerXLSX(CDataTokeniser::dataTables_t &dt, std::filesystem::path const &fn)
    : dataTables(dt), fileName(fn)
  {

  }

  /// @brief      Parses/lexes the excel file.
  /// @throws
  /// @version    2024-03-14/GGB - Function created,

  void CDataLexerXLSX::processParseFile()
  {
    xlnt::workbook wb;
    wb.load(fileName);

    // Iterate over all the workshets.

    for(auto ws: wb)
    {
      dataTables.push_back({});
      dataTables.back().tableName = ws.title();
      processSheet(dataTables.back(), ws);

    }
  }

  /// @brief      Parses/lexes the specific spreadsheet.
  /// @param[out] dataTable: The table to receive the information. The line number if the data corresponds to the row in the table.
  /// @param[in]  ws: The worksheet to process.
  /// @throws
  /// @version    2024-03-14/GGB - Function created.

  void CDataLexerXLSX::processSheet(CDataTokeniser::dataTable_t &dataTable, xlnt::worksheet ws)
  {
    /* There are several ways to approach extracting the data. The data is going to be extracted in row wise form. However
     * The structure of the data can be significant and to maintain the table format it is necessary to add some additional
     * context to ensure that the format is maintained.
     * It is assumed that the data is tabular with headings across the top and data in the rows below. The use of headings
     * is optional.
     * Note it is also possible to have headings going down and data in the columns across, but this may be better done in a
     * different function and adding API functionality to convert to a table for a column rather than row oriented table.
     *
     * All rows need to be iterated. This is generally easy given the functions provided.
     * If the data is well structured, (all columns filled, from A1) then this is an easy task. However if
     * there is missing data or blank columns, then this can be more difficult and probably needs to be approached in a dynamic
     * fashion. The challenge is to identify the empty columns and ensure that the data all stays in the same columns.
     *
     * The approach taken is to just read the data into the vector. (See the xlnt example). While this is not memory efficient,
     * it defers the problem to the calling application. The calling application can then use column matching to determine the
     * columns required and extract the column data. Once the calling application is finished with the data it can throw it away.
     * xlnt does not support move semantics for cell data, so copies are inevitable. For this reason it may be better to deal with
     * the native types stored in the cell rather than relying on string conversion.
     * Maintaining the native types adds additional overhead that may offset the string conversions.
     */

    for (auto row: ws.rows(false))
    {
      dataTable.data.push_back({});
      bool firstPass = true;

      for (auto cell: row)
      {
        if (firstPass)
        {
          dataTable.data.back().first = cell.row();
          firstPass = false;
        }

        dataTable.data.back().second.emplace_back(cell.to_string());
      }
    }
  }
}
