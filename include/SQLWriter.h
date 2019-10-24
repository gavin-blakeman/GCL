//*********************************************************************************************************************************
//
// PROJECT:							General Class Library
// FILE:								SQLWriter
// SUBSYSTEM:						Database library
// LANGUAGE:						C++
// TARGET OS:						None - Standard C++
// LIBRARY DEPENDANCE:	boost.
// NAMESPACE:						GCL
// AUTHOR:							Gavin Blakeman.
// LICENSE:             GPLv2
//
//                      Copyright 2013-2019 Gavin Blakeman.
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
// HISTORY:             2015-09-22 GGB - AIRDAS 2015.09 release
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
/// This is placed in the GCL::sqlwriter namespace.

namespace GCL
{
  namespace sqlwriter
  {
    enum EOrderBy
    {
      ASC,            ///< Ascending
      DESC            ///< Descending
    };
    enum EDialect
    {
      MYSQL,          ///< MySQL Specific
      ORACLE,         ///< Oracle Specific
      MICROSOFT       ///< Microsoft specific
    };
    enum EJoin
    {
      JOIN_INNER,     ///< Inner Join
      JOIN_RIGHT,     ///< Outer Right Join
      JOIN_LEFT,      ///< Outer Left Join
      JOIN_FULL       ///< Full outer join
    };

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

    typedef SCL::CAny parameter;
    typedef std::pair<std::string, parameter> parameterPair;
    typedef std::pair<std::string, std::string> stringPair;
    typedef std::tuple<std::string, std::string, parameter> parameterTriple;
    typedef std::pair<std::string, EOrderBy> orderBy_t;

    typedef std::vector<parameter> parameterStorage;
    typedef std::vector<parameterPair> pairStorage;
    typedef std::vector<parameterTriple> tripleStorage;
    typedef std::vector<stringPair> stringPairStorage;
    typedef std::vector<orderBy_t> orderByStorage_t;

    typedef std::vector<parameterStorage> valueStorage;

    class bindValue : public std::string
    {
    public:
      bindValue(std::string const &st) : std::string(st) {}

    };

    class CSQLWriter
    {
    public:
      typedef std::pair<std::string, std::string> stringPair;
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
      };

      std::vector<std::string> selectFields;
      stringPairStorage fromFields;
      tripleStorage whereFields;
      std::string insertTable;
      valueStorage valueFields;
      orderByStorage_t orderByFields;
      joinStorage joinFields;
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

      std::string createOrderByClause() const;
      std::string createSelectClause() const;
      std::string createFromClause() const;
      std::string createJoinClause() const;
      std::string createWhereClause() const;
      std::string createSetClause() const;

    public:
      void setDialect(EDialect d) {dialect = d;}

      void resetQuery();
      void resetWhere();

      CSQLWriter &count(std::string const &);
      CSQLWriter &deleteFrom(std::string const &);
      CSQLWriter &distinct();
      CSQLWriter &from(std::string const &, std::string const & = "");
      CSQLWriter &from(std::initializer_list<std::string>);
      CSQLWriter &insertInto(std::string, std::initializer_list<std::string>);
      CSQLWriter &insertInto(std::string);
      CSQLWriter &join(std::initializer_list<parameterJoin>);
      CSQLWriter &limit(long);
      CSQLWriter &max(std::string const &, std::string const & = "");
      CSQLWriter &min(std::string const &, std::string const & = "");
      CSQLWriter &orderBy(std::initializer_list<std::pair<std::string, EOrderBy>>);
      CSQLWriter &select();
      CSQLWriter &select(std::initializer_list<std::string>);
      CSQLWriter &set(std::string const &, SCL::CAny const &);
      CSQLWriter &update(std::string const &);
      CSQLWriter &where(std::string const &, std::string const &, SCL::CAny const &);
      CSQLWriter &where(std::initializer_list<parameterTriple>);
      CSQLWriter &values(std::initializer_list<parameterStorage>);

      std::string string() const;

      virtual void readMapFile(boost::filesystem::path const &);

      virtual bool createTable(std::string const &tableName);
      virtual bool createColumn(std::string const &tableName, std::string const &columnName);
    };

  } // namespace database
}  // namespace GCL

#endif // GCL_CONTROL

#endif // GCL_SQLWRITER
