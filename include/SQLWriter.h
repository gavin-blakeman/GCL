//*********************************************************************************************************************************
//
// PROJECT:							General Class Library
// FILE:								SQLWriter
// SUBSYSTEM:						Database library
// LANGUAGE:						C++20
// TARGET OS:						None - Standard C++
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2013-2024 Gavin Blakeman.
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
// CLASSES INCLUDED:    sqlWriter
//
// HISTORY:             2023-09-23 GGB - Extended to support parameterised queries.
//                      2023-03-28 GGB - Changed use of GCL::any to std::variant to better support parameterised queries.
//                      2022-06-07 GGB - Expanded where clause functionality to support a broader range of statements
//                      2022-05-01 GGB - Added support for "Returning"
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

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <functional>
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

#include "boost/multiprecision/mpfr.hpp"

  // GCL Library header files

#include "include/dateTime.h"
#include "include/error.h"
#include "include/logger/defaultLogger.h"

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


  /// @brief The decimal_t type is used for numbers that must have and maintain precision to a definite number of digits. This is
  /// liked to the DECIMAL type used in databases. The MariaDB decimal type has 65 digits maximum and this is matched to the MariaDB
  /// 65 digits.
  /// @note The underlying implementation is larger than register size. All decimal_t function parameters should be passed by
  /// reference and not by value.

  using decimal_t = boost::multiprecision::number<boost::multiprecision::mpfr_float_backend<65>>;

  enum operator_t
  {
    eq,             ///< equals
    gt,             ///< greater than
    lt,             ///< less than
    gte,            ///< greater than equal to
    lte,            ///< less than equal to
    neq,
    nse,
    in,             ///< IN  - Parameter should be a set or vector.
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

  enum EOrderBy
  {
    ASC,            ///< Ascending
    DESC            ///< Descending
  };
  enum EJoin
  {
    JOIN_INNER,     ///< Inner Join
    JOIN_RIGHT,     ///< Outer Right Join
    JOIN_LEFT,      ///< Outer Left Join
    JOIN_FULL,      ///< Full outer join
    JOIN_SELF,      ///< Self Join - This should not be passes to a function, but rather use the self-join function.
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

    enum EDialect
    {
      MYSQL,          ///< MySQL Specific
      ORACLE,         ///< Oracle Specific
      MICROSOFT,      ///< Microsoft specific
      POSTGRE,        ///< Postgre database
    };

    enum parameterType_t
    {
      PT_NONE,
      PT_U8,
      PT_U16,
      PT_U32,
      PT_U64,
      PT_I8,
      PT_I16,
      PT_I32,
      PT_I64,
      PT_FLOAT,
      PT_DOUBLE,
      PT_DATE,
      PT_TIME,
      PT_DATETIME,
      PT_DECIMAL,
      PT_STRING,
    };


    class bindValue_t
    {
    public:
      bindValue_t(std::string const &st) : value(st) {}
      bool operator<(bindValue_t const &rhs) const
      {
        return (value < rhs.value);
      }

      std::string to_string() const
      {
        std::string rv;

        if ( (value.front() == ':') || (value.front() == '?') )
        {
          rv += value;
        }
        else
        {
          rv = ":" + value;
        }
        return rv;
      }
    private:
      std::string value;

      bindValue_t() = delete;
    };

    class columnRef
    {
    private:
      std::string value;

      columnRef() = delete;

    public:
      columnRef(std::string const &st) : value(st) {}
      columnRef(std::string const &alias, std::string const &column) : value(alias + "." + column) {}

      std::string to_string() const { return value; }

      operator std::string() const
      {
        return to_string();
      }
    };
  public:
    using parameter_t = std::variant<std::uint8_t,
                                     std::uint16_t,
                                     std::uint32_t,
                                     std::uint64_t,
                                     std::int8_t,
                                     std::int16_t,
                                     std::int32_t,
                                     std::int64_t,
                                     float,
                                     double,
                                     std::string,
                                     date_t,
                                     dateTime_t,
                                     time_t,
                                     /*std::vector<std::uint8_t> *, */
                                     bindValue_t,
                                     decimal_t>;

#include "sqlWriter_typedef.inc"
#include "sqlWriter_variables.inc"

  public:
    sqlWriter() = default;
    sqlWriter(sqlWriter const &) = default;
    sqlWriter(sqlWriter &&) = default;
//    sqlWriter &operator=(sqlWriter const &);
//    sqlWriter &operator=(sqlWriter &&);
//    ~sqlWriter();

    operator std::string() const { return string(); }

    sqlWriter &call(std::string const &, std::initializer_list<parameter_t>);
    std::size_t columnCount() const;
    parameterType_t columnType(std::size_t) const;
    sqlWriter &count(std::string const &, std::string const & = "");
    sqlWriter &deleteFrom(std::string const &);
    sqlWriter &distinct();
    sqlWriter &forShare() { forShare_ = true; return *this; }
    sqlWriter &forUpdate() {forUpdate_ = true; return *this; }
    sqlWriter &from(std::string const &, std::optional<std::string> = std::nullopt);
    sqlWriter &from(std::initializer_list<std::string>);
    sqlWriter &from(pointer_t, std::optional<std::string> = std::nullopt);
    sqlWriter &groupBy(std::string const &);
    sqlWriter &groupBy(columnNumber_t);
    sqlWriter &groupBy(std::initializer_list<std::string>);
    sqlWriter &groupBy(std::initializer_list<columnNumber_t>);
    bool hasBindValues() const;
    bool hasBindValues(whereTest_t const &) const;
    bool hasBindValues(whereLogical_t const &) const;
    bool hasBindValues(whereVariant_t const &) const;
    bool hasBindValues(valueType_t const &) const;
    bool hasBindValues(parameter_t const &) const;
    bool hasBindValues(valueStorage_t const &) const;
    sqlWriter &insertInto(std::string, std::initializer_list<std::string>);
    sqlWriter &insertInto(std::string);
    bool isInsertQuery() const { return queryType == qt_insert; }
    bool isSelectQuery() const { return queryType == qt_select; }
    bool isDeleteQuery() const { return queryType == qt_delete; }
    bool isUpdateQuery() const { return (queryType == qt_update) || (queryType == qt_insert); }
    bool isFunctionCall() const { return queryType == qt_call; }
    bool isReturning() const { return !returningFields_.empty(); }
    sqlWriter &join(std::initializer_list<parameterJoin>);
    sqlWriter &limit(std::uint64_t);
    sqlWriter &max(std::string const &, std::string const & = "");
    sqlWriter &min(std::string const &, std::string const & = "");
    sqlWriter &offset(std::uint64_t);
    sqlWriter &orderBy(std::string, EOrderBy);
    sqlWriter &orderBy(std::initializer_list<std::pair<std::string, EOrderBy>>);
    sqlWriter &preparedStatement(bool ps = true) { preparedStatement_ = ps; return *this; }
    [[nodiscard]] bool preparedStatement() noexcept { return preparedStatement_; }
    void resetQuery();
    void resetWhere();
    void resetValues();
    sqlWriter &returning(std::string const &);
    sqlWriter &returning(std::initializer_list<std::string>);
    std::size_t rowCount() const;
    sqlWriter &select(std::initializer_list<selectExpression_t>);
    sqlWriter &selfJoin(std::string const &, std::string const &);
    sqlWriter &set(std::string const &, parameter_t const &);
    sqlWriter &set(std::initializer_list<parameterPair>);
    void setDialect(EDialect d) {dialect = d;}
    bool shouldParameterise() const;
    bool shouldParameterise(whereTest_t const &) const;
    bool shouldParameterise(whereLogical_t const &) const;
    bool shouldParameterise(whereVariant_t const &) const;
    bool shouldParameterise(valueType_t const &) const;
    bool shouldParameterise(parameter_t const &) const;
    bool shouldParameterise(valueStorage_t const &) const;
    sqlWriter &update(std::string const &);
    sqlWriter &upsert(std::string const &);

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
    std::vector<parameterVariant_t> whereParameters() const;

    sqlWriter &values(std::initializer_list<parameterStorage>);
    sqlWriter &values(valueStorage_t &&);
    sqlWriter &values(pointer_t);
    [[nodiscard]] valueType_t const &values() const noexcept;

    std::string string() const;

    virtual void readMapFile(std::filesystem::path const &);

    virtual bool createTable(std::string const &tableName);
    virtual bool createColumn(std::string const &tableName, std::string const &columnName);

    std::string getColumnMappedName(std::string const &) const;
    std::string getTableMappedName(std::string const &) const;

    static std::string sum(std::string const &);

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
    std::string createWhereClause(bool = false) const;
    std::vector<std::reference_wrapper<sqlWriter::parameterVariant_t>> createWhereParameters() const;
    std::string createSetClause(bool = false) const;
    std::string createLimitClause() const;

    parameterType_t parameterType(parameter_t const &) const;

    std::string to_string(whereTest_t const &) const;
    std::string to_string(whereLogical_t const &) const;
    std::string to_string(whereVariant_t const &) const;

    std::string to_string(parameter_t const &) const;
    std::string to_string(groupBy_t const &) const;
    std::string to_string(bindValue_t const &);
    std::string to_string(columnRef const &);
    std::string to_string(selectExpression_t const &) const;

    std::string to_string(valueType_t const &) const;
    std::string to_string(valueStorage_t const &) const;

    std::vector<std::reference_wrapper<sqlWriter::parameterVariant_t>> to_parameter(whereTest_t const &) const;
    std::vector<std::reference_wrapper<sqlWriter::parameterVariant_t>> to_parameter(whereLogical_t const &) const;
    std::vector<std::reference_wrapper<sqlWriter::parameterVariant_t>> to_parameter(whereVariant_t const &) const;

  }; // class sqlWriter

  sqlWriter::whereVariant_t where_v(std::string, operator_t, sqlWriter::parameterVector_t);
  sqlWriter::whereVariant_t where_v(std::string, operator_t, sqlWriter::parameterSet_t);
  sqlWriter::whereVariant_t where_v(std::string, operator_t, sqlWriter::parameter_t);
  sqlWriter::whereVariant_t where_v(std::string, operator_t, sqlWriter::pointer_t);
  sqlWriter::whereVariant_t where_v(std::string, operator_t, sqlWriter &&);
  sqlWriter::whereVariant_t where_v(sqlWriter::whereVariant_t, logicalOperator_t, sqlWriter::whereVariant_t);

  /// @brief      Parameter variant to handle a parameterSet.
  /// @param[in]  col: The column name.
  /// @param[in]  op: The operator to apply. (Should be IN or NOT IN)
  /// @param[in]  param: The set containing the parameters.
  /// @returns    The where variant.
  /// @note       This convertss the set to a vector to handle the '<" problem created by the date class.
  /// @version    2023-10-19/GGB - Function created.

  template<typename T>
  sqlWriter::whereVariant_t where_v(std::string col, operator_t op, std::set<T> const &param)
  {
    RUNTIME_ASSERT((op == in) || (op == nin), "Only IN and NIN operators allowed with parameter sets.");

    sqlWriter::parameterSet_t paramSet;

    for (auto const p : param)
    {
      paramSet.emplace(p);
    }

    return {std::make_tuple(col, op, paramSet)};
  }


//  template<>
//  inline std::string any::Manager_external<GCL::sqlWriter::bindValue>::S_toString(any const *anyp)
//  {
//    auto ptr = static_cast<GCL::sqlWriter::bindValue const *>(anyp->dataStorage.heapPointer);
//    return GCL::to_string(*ptr);
//  }

//  template<>
//  inline std::string any::Manager_external<GCL::sqlWriter::columnRef>::S_toString(any const *anyp)
//  {
//    auto ptr = static_cast<GCL::sqlWriter::columnRef const *>(anyp->dataStorage.heapPointer);
//    return GCL::to_string(*ptr);
//  }

  inline sqlWriter::columnRef operator"" _cr(const char *st, std::size_t)
  {
    return sqlWriter::columnRef(st);
  }

}  // namespace GCL


#endif // GCL_CONTROL

#endif // GCL_SQLWRITER
