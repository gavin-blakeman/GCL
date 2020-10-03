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
//                      Copyright 2013-2020 Gavin Blakeman.
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
// HISTORY:             2020-04-25 GGB - Added offset functionality.
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
#include <initializer_list>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

  // Miscellaneous Library header files

#include "boost/filesystem.hpp"
#include <SCL>

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
      POSTGRE,        ///< PostGre database
    };
    enum EJoin
    {
      JOIN_INNER,     ///< Inner Join
      JOIN_RIGHT,     ///< Outer Right Join
      JOIN_LEFT,      ///< Outer Left Join
      JOIN_FULL       ///< Full outer join
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

    using parameter = SCL::variant_t;
    typedef std::pair<std::string, parameter> parameterPair;
    typedef std::pair<std::string, std::string> stringPair;
    typedef std::tuple<std::string, std::string, parameter> parameterTriple;
    typedef std::pair<std::string, EOrderBy> orderBy_t;

    typedef std::vector<parameter> parameterStorage;
    typedef std::vector<parameterPair> pairStorage;
    typedef std::vector<parameterTriple> tripleStorage;
    typedef std::vector<stringPair> stringPairStorage;
    typedef std::vector<orderBy_t> orderByStorage_t;

    typedef std::vector<parameterStorage> valueStorage;     // This is to allow multiple insertions in one statement.
    typedef std::tuple<std::string, std::string, EJoin, std::string, std::string> parameterJoin;

    typedef std::vector<parameterJoin> joinStorage;

  private:
    EDialect dialect = MYSQL;

    TDatabaseMap databaseMap;
    enum EQueryType
    {
      qt_none,
      qt_select,
      qt_insert,
      qt_delete,
      qt_update,
      qt_upsert,
    };

    std::vector<std::string> selectFields;
    stringPairStorage fromFields;
    tripleStorage whereFields;
    std::string insertTable;
    valueStorage valueFields;
    orderByStorage_t orderByFields;
    joinStorage joinFields;
    std::optional<std::uint64_t> offsetValue;
    std::optional<std::uint64_t> limitValue;
    std::optional<std::string> countValue;
    bool distinct_ = false;
    stringPairStorage minFields;
    stringPairStorage maxFields;
    std::string updateTable;
    pairStorage setFields;
    std::string deleteTable;

    EQueryType queryType;
    std::string currentTable;

    bool verifyOperator(std::string const &) const;

  protected:
    void setTableMap(std::string const &, std::string const &);
    void setColumnMap(std::string const &, std::string const &, std::string const &);

    std::string getColumnMap(std::string const &) const;
    std::string getTableMap(std::string const &) const;

    std::string createSelectQuery() const;
    std::string createInsertQuery() const;
    std::string createUpdateQuery() const;
    std::string createDeleteQuery() const;
    std::string createUpsertQuery() const;

    std::string createOrderByClause() const;
    std::string createSelectClause() const;
    std::string createFromClause() const;
    std::string createJoinClause() const;
    std::string createWhereClause() const;
    std::string createSetClause() const;
    std::string createLimitClause() const;

  public:
    void setDialect(EDialect d) {dialect = d;}

    void resetQuery();
    void resetWhere();

    sqlWriter &count(std::string const &);
    sqlWriter &deleteFrom(std::string const &);
    sqlWriter &distinct();
    sqlWriter &from(std::string const &, std::string const & = "");
    sqlWriter &from(std::initializer_list<std::string>);
    sqlWriter &insertInto(std::string, std::initializer_list<std::string>);
    sqlWriter &insertInto(std::string);
    sqlWriter &join(std::initializer_list<parameterJoin>);
    sqlWriter &limit(std::uint64_t);
    sqlWriter &max(std::string const &, std::string const & = "");
    sqlWriter &min(std::string const &, std::string const & = "");
    sqlWriter &offset(std::uint64_t);
    sqlWriter &orderBy(std::initializer_list<std::pair<std::string, EOrderBy>>);
    sqlWriter &select();
    sqlWriter &select(std::initializer_list<std::string>);
    sqlWriter &select(std::string const &, std::initializer_list<std::string>);
    sqlWriter &set(std::string const &, parameter const &);
    sqlWriter &set(std::initializer_list<parameterPair>);
    sqlWriter &update(std::string const &);
    sqlWriter &upsert(std::string const &);

    /// @brief Adds a single where clause to the where list.
    /// @param[in] columnName: The columnName to add
    /// @param[in] operatorString: The operatorString to add
    /// @param[in] value: The value to add.
    /// @returns (*this)
    /// @throws None.
    /// @version 2020-09-09/GGB - Changed to a templated forwarding reference.
    /// @version 2017-08-21/GGB - Function created.

    template<typename T>
    sqlWriter &where(std::string const &columnName, std::string const &operatorString, T &&value)
    {
      whereFields.emplace_back(columnName, operatorString, std::forward<T>(value));

      return (*this);
    }

    sqlWriter &where(std::initializer_list<parameterTriple>);
    sqlWriter &values(std::initializer_list<parameterStorage>);

    std::string string() const;

    virtual void readMapFile(boost::filesystem::path const &);

    virtual bool createTable(std::string const &tableName);
    virtual bool createColumn(std::string const &tableName, std::string const &columnName);
  };

  std::string to_string(GCL::sqlWriter::bindValue const &);

}  // namespace GCL

namespace SCL
{
  template<>
  inline std::string variant_t::Manager_external<GCL::sqlWriter::bindValue>::S_toString(variant_t const *anyp)
  {
    auto ptr = static_cast<GCL::sqlWriter::bindValue const *>(anyp->dataStorage.heapPointer);
    return GCL::to_string(*ptr);
  }
}


#endif // GCL_CONTROL

#endif // GCL_SQLWRITER
