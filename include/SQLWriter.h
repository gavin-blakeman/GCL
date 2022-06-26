//*********************************************************************************************************************************
//
// PROJECT:							General Class Library
// FILE:								SQLWriter
// SUBSYSTEM:						Database library
// LANGUAGE:						C++
// TARGET OS:						None - Standard C++
// NAMESPACE:						GCL::sqlWriter
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2013-2022 Gavin Blakeman.
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
// OVERVIEW:            The file provides a class for generating/composing/writing SQL queries using a simplified approach that
//                      does not require knowledge of SQL.
//                      The class does not communicate with the database server directly, but does provide functions to create the
//                      SQL command strings to perform the database access.
//                      Typical select query would be written as follows:
//
// CLASSES INCLUDED:    CSQLWriter
//
// HISTORY:             2022-05-01 GGB - Added support for "Returning"
//                      2022-04-11 GGB - Converted to std::filesystem
//                      2021-04-13 GGB - Added call functionality.
//                      2020-04-25 GGB - Added offset functionality.
//                      2019-12-08 GGB - Added UPSERT functionality for MYSQL.
//                      2015-09-22 GGB - AIRDAS 2015.09 release
//                      2013-01-26 GGB - Development of class for application AIRDAS
//
//*********************************************************************************************************************************

#ifndef GCL_SQLWRITER_H
#define GCL_SQLWRITER_H

#ifndef GCL_CONTROL

  // Standard Header Files

#include <cstdint>
#include <filesystem>
#include <initializer_list>
#include <iterator>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

  // Miscellaneous Library header files

#include "include/any.hpp"

/// @page page2 SQL Writer
/// @tableofcontents
/// The SQL Writer provides a class that enables the construction of SQL queries without having to write the entire query, or
/// to understand the SQL language.
/// @section sec1 Overview
/// The SQL Writer is a single class that implements an SQL query writer (or composer) (opposite of parser) to allow easy
/// writing of SQL queries programatically. The class is constructed that a map (representation) of the database is constructed in
/// the class. The class is then used to generate the SQL queries with minimial input, including checking of column and table names,
/// with exceptions to indicate issues.
/// @section sec2 class CSQLWriter
/// There is a single class that implements the SQL Writer.
/// This is placed in the GCL::sqlWriter namespace.

namespace GCL
{
  enum operator_t
  {
    eq,             ///< equals
    gt,             ///< greater than
    lt,             ///< less than
    gte,            ///< greater than equal to
    lte,            ///< less than equal to
    neq,
    nse,
    in,
    between,
    nin
  };

  enum logicalOperator_t
  {
    AND,
    OR,
    XOR,
    NOT
  };

  class sqlWriter
  {
  private:
    struct SColumnData
    {
      std::pair<std::string, std::string> columnName;           ///< TableName, ColumnName
      bool unique;
    };
    typedef std::map<std::string, SColumnData> TColumnMap;

    struct STableData
    {
      std::pair<std::string, std::string> tableName;            ///<
      TColumnMap columnData;
    };
    typedef std::map<std::string, STableData> TDatabaseMap;

  public:
    using columnNumber_t = std::uint32_t;
    enum EOrderBy
    {
      ASC,            ///< Ascending
      DESC            ///< Descending
    };
    enum EDialect
    {
      MYSQL,          ///< MySQL Specific
      ORACLE,         ///< Oracle Specific
      MICROSOFT,      ///< Microsoft specific
      POSTGRE,        ///< Postgre database
    };
    enum EJoin
    {
      JOIN_INNER,     ///< Inner Join
      JOIN_RIGHT,     ///< Outer Right Join
      JOIN_LEFT,      ///< Outer Left Join
      JOIN_FULL,      ///< Full outer join
      JOIN_SELF,      ///< Self Join - This should not be passes to a function, but rather use the self-join function.
    };

    class bindValue
    {
    private:
      std::string value;

      bindValue() = delete;
    public:
      bindValue(std::string const &st) : value(st) {}

      std::string to_string() const { return value; }
    };

#include "sqlWriter_typedef.inc"
#include "sqlWriter_variables.inc"

  private:
    bool verifyOperator(std::string const &) const;

  protected:
    void setTableMap(std::string const &, std::string const &);
    void setColumnMap(std::string const &, std::string const &, std::string const &);

    std::string createSelectQuery() const;
    std::string createInsertQuery() const;
    std::string createUpdateQuery() const;
    std::string createDeleteQuery() const;
    std::string createUpsertQuery() const;
    std::string createCall() const;

    std::string createGroupByClause() const;
    std::string createOrderByClause() const;
    std::string createSelectClause() const;
    std::string createFromClause() const;
    std::string createJoinClause() const;
    std::string createWhereClause() const;
    std::string createSetClause() const;
    std::string createLimitClause() const;

    std::string to_string(whereTest_t const &) const;
    std::string to_string(whereLogical_t const &) const;
    std::string to_string(whereVariant_t const &) const;
    std::string to_string(parameter const &) const;

  public:
    operator std::string() const { return string(); }

    void setDialect(EDialect d) {dialect = d;}

    void resetQuery();
    void resetWhere();
    void resetValues();

    sqlWriter &call(std::string const &, std::initializer_list<parameter>);
    sqlWriter &count(std::string const &, std::string const & = "");
    sqlWriter &deleteFrom(std::string const &);
    sqlWriter &distinct();
    sqlWriter &from(std::string const &, std::string const & = "");
    sqlWriter &from(std::initializer_list<std::string>);
    sqlWriter &groupBy(std::string const &);
    sqlWriter &groupBy(columnNumber_t);
    sqlWriter &groupBy(std::initializer_list<std::string>);
    sqlWriter &groupBy(std::initializer_list<columnNumber_t>);
    sqlWriter &insertInto(std::string, std::initializer_list<std::string>);
    sqlWriter &insertInto(std::string);
    sqlWriter &join(std::initializer_list<parameterJoin>);
    sqlWriter &limit(std::uint64_t);
    sqlWriter &max(std::string const &, std::string const & = "");
    sqlWriter &min(std::string const &, std::string const & = "");
    sqlWriter &offset(std::uint64_t);
    sqlWriter &orderBy(std::string, EOrderBy);
    sqlWriter &orderBy(std::initializer_list<std::pair<std::string, EOrderBy>>);
    sqlWriter &returning(std::string const &);
    sqlWriter &returning(std::initializer_list<std::string>);
    sqlWriter &select();
    sqlWriter &select(std::initializer_list<std::string>);
    sqlWriter &select(std::string const &, std::initializer_list<std::string>);
    sqlWriter &selectDistinct(std::initializer_list<std::string>);
    sqlWriter &selectDistinct(std::string const &, std::initializer_list<std::string>);
    sqlWriter &selfJoin(std::string const &, std::string const &);
    sqlWriter &set(std::string const &, parameter const &);
    sqlWriter &set(std::initializer_list<parameterPair>);
    sqlWriter &update(std::string const &);
    sqlWriter &upsert(std::string const &);
    sqlWriter &forShare() { forShare_ = true; return *this; }
    sqlWriter &forUpdate() {forUpdate_ = true; return *this; }

    /// @brief      Adds a single where clause to the where list.
    /// @param[in]  columnName: The columnName to add
    /// @param[in]  oper: The operator to add
    /// @param[in]  value: The value to add.
    /// @returns    (*this)
    /// @throws     None.
    /// @version    2020-09-09/GGB - Changed to a templated forwarding reference.
    /// @version    2017-08-21/GGB - Function created.

    template<typename T>
    sqlWriter &where(std::string const &columnName, operator_t oper, T &&value)
    {
      whereClause_ = where_v(columnName, oper, value);

      return (*this);
    }

    sqlWriter &where(whereVariant_t &&);

    sqlWriter &values(std::initializer_list<parameterStorage>);
    sqlWriter &values(valueStorage &&);

    std::string string() const;

    virtual void readMapFile(std::filesystem::path const &);

    virtual bool createTable(std::string const &tableName);
    virtual bool createColumn(std::string const &tableName, std::string const &columnName);

    std::string getColumnMappedName(std::string const &) const;
    std::string getTableMappedName(std::string const &) const;

    static std::string sum(std::string const &);



  }; // class sqlWriter

  std::string to_string(sqlWriter::bindValue const &);
  sqlWriter::whereVariant_t where_v(std::string, operator_t, sqlWriter::parameterVector_t);
  sqlWriter::whereVariant_t where_v(std::string, operator_t, sqlWriter::parameter);
  sqlWriter::whereVariant_t where_v(std::string, operator_t, sqlWriter::pointer_t);
  sqlWriter::whereVariant_t where_v(std::string, operator_t, sqlWriter &&);
  sqlWriter::whereVariant_t where_v(sqlWriter::whereVariant_t, logicalOperator_t, sqlWriter::whereVariant_t);


  template<>
  inline std::string any::Manager_external<GCL::sqlWriter::bindValue>::S_toString(any const *anyp)
  {
    auto ptr = static_cast<GCL::sqlWriter::bindValue const *>(anyp->dataStorage.heapPointer);
    return GCL::to_string(*ptr);
  }

}  // namespace GCL





#endif // GCL_CONTROL

#endif // GCL_SQLWRITER
