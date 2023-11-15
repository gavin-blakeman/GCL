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
//                      Copyright 2013-2023 Gavin Blakeman.
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
// CLASSES INCLUDED:    sqWriter
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

#include "include/SQLWriter.h"

  // Standard C++ library files

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include <utility>

  // Miscellaneous library header files

#include "boost/algorithm/string.hpp"
#include "boost/locale.hpp"
#include "fmt/chrono.h"
#include "fmt/format.h"
#include "fmt/std.h"

  // GCL library header Files

#include "include/common.h"
#include "include/error.h"
#include "include/GCLError.h"

namespace GCL
{

  std::string const TABLE("TABLE");
  std::string const COLUMN("COLUMN");
  std::string const END("END");

  /* QUERY as from statement (2022-09-16)
   * ====================================
   *
   * A sub-query may be used as a table for the FROM clause. This can also include an "AS" statement for aliases.
   *
   * The subquery type is already supported in WHERE clauses. This needs to be extended to the FROM clause.
   *
   * subqueries are represented by using a pointer_t.
   *
   * Two from's currently supported
   *    sqlWriter &from(std::string const &, std::string const & = "");
   *    sqlWriter &from(std::initializer_list<std::string>);
   *
   * from is currently stored in
   *  stringPairStorage fromFields;
   *
   *  typedef std::vector<stringPair> stringPairStorage;
   *  typedef std::pair<std::string, std::string> stringPair;
   *
   *  Define:
   *
   *  using from_t = std::variant<std::string, pointer_t>;
   *  using fromPair_t = std::pair<from_t, std::optional<std::string>>;
   *  using fromStorage_t = std::vector<fromPair_t>
   *
   *  sqlWriter &from(std::string const &, std::optional<std::string> = std::nullopt);
   *  sqlWriter &from(std::initializer_list<fromPair_t>);
   *  sqlWriter &from(pointer_t, std::optional<std::string> = std::nullopt);
   *
   */


  /* INSERT INTO SELECT
   * ==================
   *
   * Used to insert the values from one query into another query.
   *
   * The values for insertion are stored in
   *    valueStorage valueFields;
   *      of type
   *    using valueStorage = std::vector<parameterVector_t>;     // This is to allow multiple insertions in one statement.
   *    using parameterVector_t = std::vector<parameter_t>;
   *    using parameter = std::variant<...>
   *
   * IE, a vector of vectors containing parameters.
   *
   * An INSERT INTO SELECT can only have one value, the subquery.
   *
   * Change the valueStorage type as follows
   *  using pointer_t = std::unique_ptr<sqlWriter>; -> Already existing
   *
   * using valueType_t = std::variant<std::monotype, valueStorage, pointer_t>;
   *  This changes valueType_t to either a null, a vector of vectors or a pointer to a subQuery.
   *
   * Then use std::visit to generate the strings.
   */


  /* Where clauses
   * =============
   *
   * Where clauses can be complicated beasts:
   *  > There is the simple,      'WHERE ID = 3';
   *  > The more complicated      'WHERE (ID = 3) AND (Type = 4)
   *  > The IN style              'WHERE ID IN (2, 3)
   *  > The subquery style        'WHERE ID IN (subQuery)
   *  > BETWEEN                   'WHERE ID BETWEEN 2 AND 3
   *  > NOT BETWEEN
   *  > NOT IN
   *  and combinations of the above.
   *  The challenge is then to create a robust way of creating any version of the where clause.
   *
   *  Consider each of the following as a test
   *  > ID = 3                  string, operator, value
   *  > ID IN (2,3)             string, operator, value, valueSet
   *  > ID IN (subQuery)        string, operator, pointer
   *  > ID BETWEEN 2 AND 3      string, operator, valueSet
   *  > NOT IN (2, 3)           string, operator, value, valueSet
   *
   *  > Consider a singular WHERE clause => string, operator, (value, valueSet, pointer)
   *    This can be done with a single call to the where function that takes the necessary value. The following signatures
   *    will do.
   *      > where(std::string, operator_t, parameter)
   *      > where(std::string, operator_t, pointer)
   *      > where(std::string, operator_t, vector)
   *  > A chained WHERE clause => WHERE (test) AND (test) AND (test) AND ...
   *    This can be done with multiple calls to the where function, with the caveat that each call creates a new test
   *    in the list of tests. The 'AND' is then implied.
   *  > A complex WHERE clause (WHERE (test AND test) or (TEST IN (x,y)) ...
   *    This is best done by having a test clause builder. IE a function that returns a where clause.
   *
   *
   *  For the complex cases, (and the simple) cases, the challenge is to build a structure that can be read out in a way that
   *  creates the total test clause by a once-through operation.
   *  Consider a tree type object as follows:
   *
   *  variant-logical  = test_t, logicalOperator_t, test_t => (test AND test)
   *  variant-test     = std::string, operator_t, valueSet => (ID = 1)
   *
   *  For a complex, the tree then looks like a queue or vector, and is read from the front
   *
   *      0)  ("ID", eq, 1))
   *      1)  AND
   *      2)  ("Type", eq, 2)
   *
   *
   *
   *  Define the following:
   *  operator_t        eq(=), gt(>), lt(<), lte(<=), gte(>=), ne(!=), nse(<=>), in(IN), between(BETWEEN)
   *  logicalOperator_t AND, OR, NOT, XOR
   *  parameter         SCL::any
   *  parameterVector_t std::vector<parameter>
   *  pointer_t         std::unique_ptr<sqlQuery>
   *  parameterVariant  std::variant<parameter, parameterList, pointer>()
   *
   *  whereTest_t       std::tuple<std::string, operator_t, parameterVariant
   *  whereLogical_t    std::tuple<whereVariant_t, logicalOperator_t, whereVariant_t>
   *  whereVariant_t    std::variant<whereLogical_t, whereTest_t>
   *
   *  Main storage is sqlWriter is then the whereVariant.
   *
   *  Some helper functions are then needed.
   *  friend whereVariant_t &where_v(std::string, operator_t, parameter);
   *  friend whereVariant_t &where_v(std::string, operator_t, parameterVector_t);
   *  friend whereVariant_t &where_v(std::string, operator_t, pointer_t);
   *  friend whereVariant_t &where_v(whereVariant_t, logicalOperator_t, whereVariant_t);
   *
   *  sqlWriter &where(std::string, operator_t, parameter);
   *  sqlWriter &where(std::string, operator_t, sqlWriter &&);
   *  sqlWriter &where(std::string, operator_t, vector);
   *
   *
   *  Sample Code: 'WHERE (ID = 1 AND Type = 2) or (ID IN (3, 4))'
   *
   *  sqlWriter sqlQuery;
   *  whereVariant_t whereClause;
   *
   *  whereClause.
   *  sqlQuery.where( where_v(
   *                    where_v(
   *                      where_v("ID", eq, 1),
   *                      AND,
   *                      where_v("Type", eq, 2)),
   *                    OR,
   *                    where("ID", IN, {3, 4})));
   *
   *  Read out is done, (i think) by a std::visit of the variant.
   *  Now lets code this.
   *
   */

  struct parameter_to_string
  {
    bool preparingClause;
    std::string operator()(std::uint8_t const &p) { return std::to_string(p); }
    std::string operator()(std::uint16_t const &p) { return std::to_string(p); }
    std::string operator()(std::uint32_t const &p) { return std::to_string(p); }
    std::string operator()(std::uint64_t const &p) { return std::to_string(p); }
    std::string operator()(std::int8_t const &p) { return std::to_string(p); }
    std::string operator()(std::int16_t const &p) { return std::to_string(p); }
    std::string operator()(std::int32_t const &p) { return std::to_string(p); }
    std::string operator()(std::int64_t const &p) { return std::to_string(p); }
    std::string operator()(float const &p) { return std::to_string(p); }
    std::string operator()(double const &p) { return std::to_string(p); }
    std::string operator()(date_t const p)
    {
      if (preparingClause)
      {
        return fmt::format("{:%Y-%m-%d}", fmt::gmtime(p.date()));
      }
      else
      {
        return fmt::format("'{:%Y-%m-%d}'", fmt::gmtime(p.date()));
      };
    }
    std::string operator()(time_t const p)
    {
      if (preparingClause)
      {
        return fmt::format("{:%H:%M:%S}", fmt::gmtime(p.time()));
      }
      else
      {
        return fmt::format("'{:%H:%M:%S}'", fmt::gmtime(p.time()));
      };
    }
    std::string operator()(dateTime_t const p)
    {
      if (preparingClause)
      {
        return fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::gmtime(p.dateTime()));
      }
      else
      {
        return fmt::format("'{:%Y-%m-%d %H:%M:%S}'", fmt::gmtime(p.dateTime()));
      };
    }
    std::string operator()(decimal_t const p) { return p.str(0, std::ios::fixed); }

    std::string operator()(std::string const &s)
    {
      using namespace std::string_literals;
      return (preparingClause ? ""s : "'"s) + s + (preparingClause ? ""s : "'"s);
    }
    std::string operator()(sqlWriter::bindValue_t const &bvt) { return bvt.to_string(); }
  };

  struct parameter_to_type
  {
    sqlWriter::parameterType_t operator()(std::uint8_t const &) { return sqlWriter::PT_U8; }
    sqlWriter::parameterType_t operator()(std::uint16_t const &) { return sqlWriter::PT_U16; }
    sqlWriter::parameterType_t operator()(std::uint32_t const &) { return sqlWriter::PT_U32; }
    sqlWriter::parameterType_t operator()(std::uint64_t const &) { return sqlWriter::PT_U64; }
    sqlWriter::parameterType_t operator()(std::int8_t const &) { return sqlWriter::PT_I8; }
    sqlWriter::parameterType_t operator()(std::int16_t const &) { return sqlWriter::PT_I16; }
    sqlWriter::parameterType_t operator()(std::int32_t const &) { return sqlWriter::PT_I32; }
    sqlWriter::parameterType_t operator()(std::int64_t const &) { return sqlWriter::PT_I64; }
    sqlWriter::parameterType_t operator()(float const &) { return sqlWriter::PT_FLOAT; }
    sqlWriter::parameterType_t operator()(double const &) { return sqlWriter::PT_DOUBLE; }
    sqlWriter::parameterType_t operator()(date_t const ) { return sqlWriter::PT_DATE; }
    sqlWriter::parameterType_t operator()(time_t const ) { return sqlWriter::PT_TIME; }
    sqlWriter::parameterType_t operator()(dateTime_t const ) { return sqlWriter::PT_DATETIME; }
    sqlWriter::parameterType_t operator()(decimal_t const ) { return sqlWriter::PT_DECIMAL; }

    sqlWriter::parameterType_t operator()(std::string const &) { return sqlWriter::PT_STRING; }
    sqlWriter::parameterType_t operator()(sqlWriter::bindValue_t const &) { CODE_ERROR(); }
  };

  struct selectExpression_to_string
  {
    std::string operator()(std::uint8_t const &p) { return std::to_string(p); }
    std::string operator()(std::uint16_t const &p) { return std::to_string(p); }
    std::string operator()(std::uint32_t const &p) { return std::to_string(p); }
    std::string operator()(std::uint64_t const &p) { return std::to_string(p); }
    std::string operator()(std::int8_t const &p) { return std::to_string(p); }
    std::string operator()(std::int16_t const &p) { return std::to_string(p); }
    std::string operator()(std::int32_t const &p) { return std::to_string(p); }
    std::string operator()(std::int64_t const &p) { return std::to_string(p); }
    std::string operator()(float const &p) { return std::to_string(p); }
    std::string operator()(double const &p) { return std::to_string(p); }
    std::string operator()(date_t const p) { return fmt::format("'{:%Y-%m-%d}'", fmt::gmtime(p.date())); }
    std::string operator()(time_t const p) { return fmt::format("'{:%H:%M:%S}'", fmt::gmtime(p.time())); }
    std::string operator()(dateTime_t const p) { return fmt::format("'{:%Y-%m-%d %H:%M:%S}'", fmt::gmtime(p.dateTime())); }
    std::string operator()(decimal_t const p) { return p.str(0, std::ios::fixed);; }
    std::string operator()(std::string const &s) { return s; }
  };

  struct groupBy_to_string
  {
    std::string operator()(std::string const &s) { return s; }
    std::string operator()(std::size_t const &cn) { return std::to_string(cn); }
  };

  // helper type for the visitor
  template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
  // explicit deduction guide (not needed as of C++20)
  template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

  std::string sqlWriter::to_string(parameter_t const &p) const
  {
    parameter_to_string v{preparingStatement};
    return std::visit(v, p);
  }

  sqlWriter::parameterType_t sqlWriter::parameterType(parameter_t const &p) const
  {
    return std::visit(parameter_to_type(), p);
  }

  std::string sqlWriter::to_string(groupBy_t const &p) const
  {
    return std::visit(groupBy_to_string(), p);
  }

  std::string sqlWriter::to_string(selectExpression_t const &p) const
  {
    return std::visit(selectExpression_to_string(), p);
  }

  /// @brief      Converts a whereTest_t to a string.
  /// @param[in]  w: The whereTest_t to convert.
  /// @throws
  /// @version    2022-06-07/GGB - Function created.

  std::string sqlWriter::to_string(whereTest_t const &w) const
  {

    std::string returnValue = "(";

    returnValue += std::get<0>(w) + " ";
    returnValue += operatorMap[std::get<1>(w)] + " ";

    switch(std::get<1>(w))
    {
      case eq:
      case gt:
      case lt:
      case gte:
      case lte:
      case neq:
      case nse:
      {
        if (std::holds_alternative<parameter_t>(std::get<2>(w)))
        {
          returnValue += to_string(std::get<parameter_t>(std::get<2>(w)));
        }
        else
        {
          RUNTIME_ERROR("Only a single parameter is allowed for " + sqlWriter::operatorMap[std::get<1>(w)] + ".");
        }
        break;
      }
      case in:
      case nin:
      {
        returnValue += "(";
        std::visit(overloaded
                   {
                     [&](parameter_t const &p) { returnValue += to_string(p) + ")"; },
                     [&](parameterVector_t const &pv)
                     {
                       bool first = true;

                       for (auto const &p : pv)
                       {
                         if (first)
                         {
                           first = false;
                         }
                         else
                         {
                           returnValue += ", ";
                         }
                         returnValue += to_string(p);
                       }
                       returnValue += ")";
                     },
                     [&](parameterSet_t const &pv)
                     {
                       bool first = true;

                       for (auto const &p : pv)
                       {
                         if (first)
                         {
                           first = false;
                         }
                         else
                         {
                           returnValue += ", ";
                         }
                         returnValue += to_string(p);
                       }
                       returnValue += ")";
                     },
                     [&](pointer_t const &pt)
                     {
                       returnValue += static_cast<std::string>(*pt) + ")";
                     },
                   }, std::get<2>(w));
        break;
      };
      case between:
      {
        if (std::holds_alternative<parameterVector_t>(std::get<2>(w)))
        {
          if (std::get<parameterVector_t>(std::get<2>(w)).size() != 2)
          {
            RUNTIME_ERROR("Two parameters are required for 'BETWEEN'.");
          }
          else
          {
            returnValue += to_string(std::get<parameterVector_t>(std::get<2>(w))[0]);
            returnValue += " AND ";
            returnValue += to_string(std::get<parameterVector_t>(std::get<2>(w))[1]);
          }
        }
        break;
      }
      default:
      {
        CODE_ERROR();
        break;
      }
    }
    returnValue += ")";
    return returnValue;
  }

  /// @brief      Converts a whereTest_t to a string.
  /// @param[in]  w: The whereTest_t to convert.
  /// @throws
  /// @version    2022-06-07/GGB - Function created.

  std::string sqlWriter::to_string(whereLogical_t const &wl) const
  {
    std::string returnValue = " (";

    returnValue += to_string(*std::get<0>(wl));
    returnValue += " " + logicalOperatorMap[std::get<1>(wl)] + " ";
    returnValue += to_string(*std::get<2>(wl)) + ")";

    return returnValue;
  }

  //  class whereVariant_t : public std::variant<whereTest_t, whereLogical_t, nullptr_t>
  std::string sqlWriter::to_string(whereVariant_t const &wv) const
  {
    std::string returnValue;

    std::visit(overloaded
               {
                 [&](std::monostate const &) { CODE_ERROR(); },
                 [&](whereTest_t const &wt) { returnValue = to_string(wt); },
                 [&](whereLogical_t const &wl) { returnValue = to_string(wl); },
               }, wv.base);

    return returnValue;
  }

  /// @brief      Creates the string for the values for an INSERT query.
  /// @param[in]  values: The values to convert to a string.
  /// @version    2022-07-23/GGB - Function created.

  std::string sqlWriter::to_string(valueType_t const &values) const
  {
    std::string returnValue;

    std::visit(overloaded
               {
                 [&](std::monostate const &) { CODE_ERROR(); },
                 [&](valueStorage_t const &vs) { returnValue = to_string(vs); },
                 [&](pointer_t const &pt) { returnValue = "( " + static_cast<std::string>(*pt) + ") "; },
               }, values);

    return returnValue;
  }

  /// @brief      Creates the string for the values for an INSERT query.
  /// @param[in]  values: The values to convert to a string.
  /// @version    2022-07-23/GGB - Function created.

  std::string sqlWriter::to_string(valueStorage_t const &valueFields) const
  {
    bool firstValue = true;
    bool firstRow = true;

    std::string returnValue = "VALUES ";

    for (auto &outerElement : valueFields)
    {
      if (firstRow)
      {
        firstRow = false;
      }
      else
      {
        returnValue += ", ";
      }

      firstValue = true;
      returnValue += "(";

      for (auto & innerElement : outerElement)
      {
        if (firstValue)
        {
          firstValue = false;
        }
        else
        {
          returnValue += ", ";
        };

        returnValue += to_string(innerElement);
      };

      returnValue += ")";
    };

    return returnValue;
  }


  //******************************************************************************************************************************
  //
  // CSQLWriter
  //
  //******************************************************************************************************************************

  sqlWriter::operatorMap_t sqlWriter::operatorMap =
  {
    {eq, "="},
    {gt, ">"},
    {lt, "<"},
    {gte, ">="},
    {lte, "<="},
    {neq, "!="},
    {nse, "<=>"},
    {in, "IN"},
    {between, "BETWEEN"},
    {nin, "NOT IN"},
  };

  sqlWriter::logicalOperatorMap_t sqlWriter::logicalOperatorMap =
  {
    {AND, "AND"},
    {OR, "OR"},
    {XOR, "XOR"},
    {NOT, "NOT"}
  };



  /// @brief      Function to create a call procedure.
  /// @param[in]  procedureName: The name of the procedure to call.
  /// @param[in]  parameters: The parameters for the procedure call.
  /// @returns    (*this)
  /// @throws     None
  /// @version    2021-04-13/GGB - Function created.

  sqlWriter &sqlWriter::call(std::string const &procedureName, std::initializer_list<parameter_t> parameters)
  {
    resetQuery();
    queryType = qt_call;

    procedureName_ = procedureName;

    for (auto param : parameters)
    {
      procedureParameters_.push_back(param);
    };

    return (*this);
  }

  /// @brief      Returns the number of columns affected.
  /// @details    The number of columns returned is dependant on the type of query.
  /// @returns    The number of columns. This is dependent on the type of query.
  /// @throws
  /// @version    2023-09-23/GGB - Function created.

  std::size_t sqlWriter::columnCount() const
  {
    std::size_t rv;

    switch (queryType)
    {
      case qt_insert:
      {
        rv = selectFields.size();
        break;
      }
      default:
      {
        CODE_ERROR();
        break;
      }
    }

    return rv;
  }

  /// @brief      Returns the type of the column. (Only insert, update and upsert queries)
  /// @param[in]  columnNo: The number of the column.
  /// @returns    The type of the column.
  /// @throws
  /// @version    2023-09-23/GGB - Function created.

  sqlWriter::parameterType_t sqlWriter::columnType(std::size_t columnNo) const
  {
    parameterType_t rv = PT_NONE;

    switch (queryType)
    {
      case qt_insert:
      {
        if (std::holds_alternative<valueStorage_t>(insertValue))
        {
          valueStorage_t const valueStorage = std::get<valueStorage_t>(insertValue);
          parameterVector_t const &parameters = valueStorage.back();
          rv = parameterType(parameters[columnNo]);
        };
        break;
      }
      default:
      {
        CODE_ERROR();
        break;
      }
    }

    return rv;
  }

  /// @brief      Function to capture the count expression
  /// @param[in]  countExpression: The count expression to capture.
  /// @param[in]  countAs: The name for the count expression.
  /// @returns    (*this)
  /// @throws     None
  /// @version    2022-04-06/GGB - Added second parameter to allow for specifying count AS ...
  /// @version    2017-08-12/GGB - Function created.

  sqlWriter &sqlWriter::count(std::string const &countExpression, std::string const &countAs)
  {
    queryType = qt_select;
    countValue = std::make_pair(countExpression, countAs);

    return (*this);
  }

  /// @brief      Creates the string for the call including parameters.
  /// @returns    a string representation of stored procedure call.
  /// @version    2021-04-13/GGB - Function created.

  std::string sqlWriter::createCall() const
  {
    bool commaRequired = false;
    std::string returnValue = "CALL ";

    returnValue += procedureName_ + "(";

    for (auto const &param: procedureParameters_)
    {
      if (commaRequired)
      {
        returnValue += ", ";
      }
      else
      {
        commaRequired = true;
      };
      returnValue += "'" + to_string(param) + "'";
    }

    returnValue += ")";

    return returnValue;
  }

  /// @brief      Creates the test for the specified delete query.
  /// @returns    a string representation of the delete query.
  /// @version    2018-05-12/GGB - Function created.

  std::string sqlWriter::createDeleteQuery() const
  {
    std::string returnValue = "DELETE FROM ";

    returnValue += getColumnMappedName(deleteTable);

    returnValue += createWhereClause();

    return returnValue;
  }

  /// @brief      Creates the string for an insert query.
  /// @param[in]  preparedQuery: true if the quewry should be a prepared query.
  /// @returns    The Insert Query as a string.
  /// @throws     None.
  /// @version    2023-09-23/GGB - Added the parameter preparedQuery to support preparedQueries.
  /// @version    2022-07-23/GGB - Changed the value storage to valueType_t and useing std::variant
  /// @version    2022-05-01/GGB - Added support for the 'RETURNING' function.
  /// @version    2015-03-31/GGB - Function created.

  std::string sqlWriter::createInsertQuery(bool preparedQuery) const
  {
    std::string returnValue = "INSERT INTO " + insertTable + " (";
    bool firstValue = true;

      // Output the column names.

    for (auto &element : selectFields)
    {
      std::string columnName;

      if (firstValue)
      {
        firstValue = false;
      }
      else
      {
        returnValue += ", ";
      };
      columnName = to_string(element);

      returnValue += columnName;
    };

    returnValue += ") ";

    if (preparedQuery)
    {
      std::size_t placeHolderCount = selectFields.size();

      if (placeHolderCount >= 1)
      {
        returnValue += " VALUES (";
        firstValue = true;

        while (placeHolderCount)
        {
          if (firstValue)
          {
            firstValue = false;
            returnValue += " ?";
          }
          else
          {
            returnValue += ", ?";
          }
          placeHolderCount--;
        }
        returnValue += ") ";
      }
    }
    else
    {
      returnValue += to_string(insertValue);      /// Creates the VALUES clause.
    };

    if (!returningFields_.empty())
    {
      returnValue += " RETURNING ";
      firstValue = true;

      for (std::string const &v : returningFields_)
      {
        if (firstValue)
        {
          firstValue = false;
        }
        else
        {
          returnValue += ", ";
        }
        returnValue += v;
      };
    }

    return returnValue;
  }

  /// @brief    Creates the limit clause
  /// @throws   None.
  /// @version  2022-05-10/GGB - Bug Fix (#226)
  /// @version  2020-04-25/GGB - Function created.

  std::string sqlWriter::createLimitClause() const
  {
    std::string returnValue;

    switch (dialect)
    {
      case MYSQL:
      {
        // Offset does not have to be present. However if offset is present, then limit must also be present.

        if (offsetValue)
        {
          returnValue = " LIMIT " + std::to_string(*offsetValue) + ", "
                        + std::to_string(limitValue ? *limitValue : std::numeric_limits<std::uint64_t>::max()) + " ";
        }
        else
        {
          if (limitValue)
          {
            returnValue = " LIMIT " + std::to_string(*limitValue) + " ";
          };
        };
        break;
      };
      case POSTGRE:
      {
        break;
      }
      default:
      {
        RUNTIME_ERROR(boost::locale::translate("Unknown dialect"), E_SQLWRITER_UNKNOWNDIALECT, LIBRARYNAME);
      }
    }

    return returnValue;
  }

  /// @brief      Creates the set clause.
  /// @param[in]  preparedQuery: Create a prepared query.
  /// @throws     GCL::CRuntimeAssert
  /// @version    2023-09-26/GGB - Added support for prepared queries.
  /// @version    2017-08-21/GGB - Function created.

  std::string sqlWriter::createSetClause(bool preparedQuery) const
  {
    RUNTIME_ASSERT(!setFields.empty(), boost::locale::translate("No Set fields defined for update query."));

    std::string returnValue = "SET ";


    bool firstValue = true;

    for (auto element : setFields)
    {
      if (firstValue)
      {
        firstValue = false;
      }
      else
      {
        returnValue += ", ";
      };
      returnValue += element.first + " = ";

      returnValue += preparedQuery ? "?" : to_string(element.second);
    };

    return returnValue;
  }

  /// @brief      Converts the query to a string. Specifically for an update query.
  /// @param[in]  preparedQuery: Create a prepared query.
  /// @throws
  /// @version    2023-09-26/GGB - Added support for prepared queries.
  /// @version    2017-08-21/GGB - Function created.

  std::string sqlWriter::createUpdateQuery(bool preparedQuery) const
  {
    std::string returnValue = "UPDATE " + updateTable + " ";

    returnValue += createSetClause(preparedQuery);

    returnValue += createWhereClause();

    return returnValue;
  }

  /// @brief      Converts the upsert query to a string.
  /// @param[in]  preparedQuery: true = preparing a query.
  /// @throws     GCL::CRuntimeError
  /// @version    2023-09-26/GGB - Added support for prepared queries
  /// @version    2021-11-18/GGB - Updated to use std::variant with the where fields.
  /// @version    2019-12-08/GGB - Function created.

  std::string sqlWriter::createUpsertQuery(bool preparedQuery) const
  {
    // Notes:
    //  1. The where() clauses should be populated. These need to be converted to insert clauses for the insert function.
    //  2. The set clause needs to be included in the insert clauses.

    RUNTIME_ASSERT(dialect == MYSQL, boost::locale::translate("Upsert only implemented for MYSQL."));

    std::string returnValue;
    std::string fieldNames;
    std::string fieldValues;
    bool firstValue = true;

    switch(dialect)
    {
      case MYSQL:
      {

          // Add the set clause.

        for (auto const &element : setFields)
        {
          if (firstValue)
          {
            firstValue = false;
          }
          else
          {
            fieldNames += ", ";
            fieldValues += ", ";
          };

          fieldNames += getColumnMappedName(element.first);
          fieldValues += preparedQuery ? "?" : to_string(element.second);
        };

          // Before we can create the insert query, we need to

        returnValue = "INSERT INTO " + insertTable + "(";
        returnValue += fieldNames + ") VALUES (";
        returnValue += fieldValues + ") ";
        returnValue += "ON DUPLICATE KEY UPDATE ";

        firstValue = true;
        fieldValues.clear();

        for (auto const &element : setFields)
        {
          if (firstValue)
          {
            firstValue = false;
          }
          else
          {
            fieldValues += ", ";
          };
          fieldValues+= getColumnMappedName(element.first) + " = ";

          fieldValues += preparedQuery ? "?" : to_string(element.second);
        };

        returnValue += fieldValues;

        break;
      }
      default:
      {
        CODE_ERROR();
      }
    };

    return returnValue;
  }

  /// @brief      Converts the where clause to a string for creating the SQL string.
  /// @returns    The where clause.
  /// @throws
  /// @version    2022-06-02/GGB - Added support for 'IN'
  /// @version    2021-11-18/GGB - Updated to use std::variant with the where fields.
  /// @version    2015-05-24/GGB - Function created.

  std::string sqlWriter::createWhereClause() const
  {
    logger::TRACE_ENTER();
    std::string returnValue = "";

    if (!std::holds_alternative<std::monostate>(whereClause_.base))
    {
      returnValue += " WHERE " + to_string(whereClause_);
    }

    logger::TRACE_EXIT();
    return returnValue;
  }

  /// @brief Adds the distinct keyword to a select query.
  /// @returns (*this)
  /// @throws None.
  /// @version 2017-08-19/GGB - Function created.

  sqlWriter &sqlWriter::distinct()
  {
    distinct_ = true;
    return (*this);
  }

  /// @brief      Adds the table name to the from clause. @c from("tbl_name", "n")
  /// @param[in]  fromString: The table name to add to the from clause.
  /// @param[in]  alias: The alias to use for the table name.
  /// @returns    (*this)
  /// @throws     None.
  /// @version    2022-09-16/GGGB - Updated to support sub-queries
  /// @version    2017-08-20/GGB - Function created.

  sqlWriter &sqlWriter::from(std::string const &fromString, std::optional<std::string> alias)
  {
    fromFields.emplace_back(fromString, alias);

    return (*this);
  }

  /// @brief      Adds the table name to the from clause. @c from("tbl_name", "n")
  /// @param[in]  fromString: The table name to add to the from clause.
  /// @param[in]  alias: The alias to use for the table name.
  /// @returns    (*this)
  /// @throws     None.
  /// @version    2022-09-16/GGB - Function created.

  sqlWriter &sqlWriter::from(pointer_t subQuery, std::optional<std::string> alias)
  {
    fromFields.emplace_back(std::move(subQuery), alias);

    return *this;
  }

  /// @brief        Searches the databaseMap and determines the mapped column names.
  /// @param[in]    columnName: The columnName to map.
  /// @returns      The mapped column name,
  /// @throws       None.
  /// @version
  /// @todo         Implement this function. (Bug# 0000193)

  std::string sqlWriter::getColumnMappedName(std::string const &columnName) const
  {
    std::string returnValue = columnName;

    return returnValue;
  }

  /// @brief      Adds a max() function to the query.
  /// @param[in]  column: The name of the column to take the max of.
  /// @param[in]  as: The column name to assign to the max() function.
  /// @returns    (*this)
  /// @throws     None.
  /// @version    2017-08-20/GGB - Function created.

  sqlWriter &sqlWriter::max(std::string const &column, std::string const &as)
  {
    queryType = qt_select;
    maxFields.emplace_back(column, as);
    return (*this);
  }

  /// @brief      Adds a min() function to the query.
  /// @param[in]  column: The name of the column to take the max of.
  /// @param[in]  as: The column name to assign to the min() function.
  /// @returns    (*this)
  /// @throws     None.
  /// @version    2017-08-20/GGB - Function created.

  sqlWriter &sqlWriter::min(std::string const &column, std::string const &as)
  {
    queryType = qt_select;
    minFields.emplace_back(column, as);
    return (*this);
  }

  /// @brief      Function called to add columnData into a table.
  /// @param[in]  tableName: The table name to associate with the column.
  /// @param[in]  columnName: The column name to create.
  /// @returns    true - column added.
  /// @returns    false - column no added.
  /// @throws     None.
  /// @version    2013-01-26/GGB - Function created.

  bool sqlWriter::createColumn(std::string const &tableName, std::string const &columnName)
  {
    TDatabaseMap::iterator iter;

    if ( (iter = databaseMap.find(tableName)) == databaseMap.end() )
    {
      return false;
    }
    else if ( (*iter).second.columnData.find(columnName) != (*iter).second.columnData.end() )
    {
      return false;
    }
    else
    {
      SColumnData columnData;
      columnData.columnName.first = columnName;

      (*iter).second.columnData[columnName] = columnData;
      return true;
    };
  }

  /// @brief Function to add a table to the database map.
  /// @param[in] tableName: The table name.
  /// @returns true =
  /// @note The table is not yet mapped at this point.
  /// @version 2013-01-26/GGB - Function created.

  bool sqlWriter::createTable(std::string const &tableName)
  {
    // Check if the table already exists in the database.

    if ( (databaseMap.find(tableName) != databaseMap.end()) )
    {
      return false;
    }
    else
    {
      STableData newTable;
      newTable.tableName.first = tableName;

      databaseMap[tableName] = newTable;
      return true;
    };
  }

  /// @brief Sets the Mapping of a specified column.
  /// @param[in] tableName: The table name having the column.
  /// @param[in] columnName: The column name to map.
  /// @param[in] columnMap: The columnName to use when referring to the columnName.
  /// @throws None.
  /// @version 2013-01-26/GGB - Function created.

  void sqlWriter::setColumnMap(std::string const &tableName, std::string const &columnName, std::string const &columnMap)
  {
    if ( databaseMap.find(tableName) != databaseMap.end() )
    {
      if ( databaseMap[tableName].columnData.find(columnName) != databaseMap[tableName].columnData.end() )
      {
        databaseMap[tableName].columnData[columnName].columnName.first = columnName;
        databaseMap[tableName].columnData[columnName].columnName.second = columnMap;
      };
    };
  }

  /// @brief Sets the Mapping of the table.
  /// @param[in] tableName: The name of the table.
  /// @param[in] tableMap: The string to map to the tableName.
  /// @version 2013-01-26/GGB - Function created.

  void sqlWriter::setTableMap(std::string const &tableName, std::string const &tableMap)
  {
    if ( databaseMap.find(tableName) != databaseMap.end() )
    {
      databaseMap[tableName].tableName.first = tableName;
      databaseMap[tableName].tableName.second = tableMap;
    };
  }

  /// @brief Output the "FROM" clause as a string.
  /// @returns A string representation of the "FROM" clause.
  /// @version 2022-09-16/GGB - Added functionality for sub-queries.
  /// @version 2016-05-08/GGB: Added support for table alisases and table maps.
  /// @version 2015-04-12/GGB: Function created.

  std::string sqlWriter::createFromClause() const
  {
    std::string returnValue = " FROM ";
    std::string tableName;
    bool first = true;
    std::vector<std::string>::const_iterator iterator;

    first = true;
    for (auto const &element : fromFields)
    {
      if (first)
      {
        first = false;
      }
      else
      {
        returnValue += ", ";
      };

      std::visit(overloaded
                 {
                   [&](std::string const &p)
                   {
                     returnValue += p;
                   },
                   [&](pointer_t const &pt)
                   {
                     returnValue += "(" + static_cast<std::string>(*pt) + ")";
                   },
                 }, std::get<0>(element));

      if (std::get<1>(element))
      {
        returnValue += " AS " + *std::get<1>(element);
      };
    }

    return returnValue;
  }

  /// @brief Creates the 'GROUP BY' clause if required.
  /// @returns A string containing the 'GROUP BY' clause.
  /// @version 2022-04-07/GGB - Function created.

  std::string sqlWriter::createGroupByClause() const
  {
    std::string returnValue = " GROUP BY ";
    bool first = true;

    for (auto column : groupByFields_)
    {
      if (first)
      {
        first = false;
      }
      else
      {
        returnValue += ", ";
      }
      returnValue += to_string(column);
    }

    return returnValue;
  }

  /// @brief Creates the "JOIN" clause
  /// @returns A string containing the join clause (begins with the JOIN keyword.
  /// @version 2017-07-29/GGB - Function created.

  std::string sqlWriter::createJoinClause() const
  {
    std::string returnValue;

    for (auto element : joinFields)
    {
      switch (std::get<2>(element))
      {
        case JOIN_LEFT:
        {
          returnValue += + " LEFT JOIN ";
          break;
        };
        case JOIN_RIGHT:
        {
          returnValue += + " RIGHT JOIN ";
          break;
        };
        case JOIN_INNER:
        {
          returnValue += " INNER JOIN ";
          break;
        }
        case JOIN_FULL:
        {
          returnValue += "FULL JOIN ";
          break;
        }
        default:
        {
          CODE_ERROR();
          break;
        }

      };
      returnValue += std::get<3>(element) + " ON ";
      returnValue += std::get<0>(element) + "." + std::get<1>(element) + "=" + std::get<3>(element) + "." + std::get<4>(element);
    }

    return returnValue;
  }

  /// @brief Creates the "ORDER BY" clause.
  /// @returns A string representation of the "Order By" clause.
  /// @throws None.
  /// @version 2015-04-12/GGB - Function created.

  std::string sqlWriter::createOrderByClause() const
  {
    orderByStorage_t::const_iterator iterator;
    bool first = true;

    std::string returnValue = " ORDER BY ";

    for (iterator = orderByFields.begin(); iterator != orderByFields.end(); iterator++)
    {
      if (first)
      {
        first = false;
      }
      else
      {
        returnValue += ", ";
      };
      returnValue += getColumnMappedName((*iterator).first);
      returnValue += " ";

      if ((*iterator).second == ASC)
      {
        returnValue += "ASC ";
      }
      else if ((*iterator).second == DESC)
      {
        returnValue += "DESC ";
      };
    };

    return returnValue;
  }

  /// @brief    Function to create the select clause.
  /// @returns  A string representation of the select clause.
  /// @note     This function also performs the mapping to the correct table.columnNames. Additionally, if only the columnName is
  ///           given the function will also search the correct tableName or tableAlias and add that to the term.
  /// @version  2022-07-23/GGB - Added support for constants as select cases.
  /// @version  2022-04-06/GGB - Added support for 'COUNT() AS '
  /// @version  2017-08-20/GGB - Added support for min() and max()
  /// @version  2017-08-19/GGB - Added support for DISTINCT
  /// @version  2017-08-12/GGB - Added code to support COUNT() clauses.
  /// @version  2015-04-12/GGB - Function created.

  std::string sqlWriter::createSelectClause() const
  {
    std::string returnValue = "SELECT ";
    bool first = true;
    std::vector<std::string>::const_iterator iterator;
    std::string columnName;

    if ( (dialect == MICROSOFT) && (limitValue))
    {
      returnValue += "TOP " + std::to_string(*limitValue) + " ";
    };

    if (distinct_)
    {
      returnValue += "DISTINCT ";
    };

    for (auto const &field: selectFields)
    {
      if (first)
      {
        first = false;
      }
      else
      {
        returnValue += ", ";
      };
      columnName = to_string(field);

      returnValue += columnName;
    };

    if (countValue)
    {
      if (first)
      {
        first = false;
      }
      else
      {
        returnValue += ", ";
      };

      if ((*countValue).first == "*")
      {
        returnValue += "COUNT(*) ";
      }
      else
      {
        returnValue += "COUNT(" + (*countValue).first + ") ";
      };

      if ((*countValue).second.size() != 0)
      {
        returnValue += " AS " + (*countValue).second;
      };
    }

    if (!maxFields.empty())
    {
      for (auto element : maxFields)
      {
        if (first)
        {
          first = false;
        }
        else
        {
          returnValue += ", ";
        };

        returnValue += "MAX(" + element.first + ")";
        if (!element.second.empty())
        {
          returnValue += " AS " + element.second;
        };
      }
    };

    if (!minFields.empty())
    {
      for (auto element : minFields)
      {
        if (first)
        {
          first = false;
        }
        else
        {
          returnValue += ", ";
        };

        returnValue += "MIN(" + element.first + ")";
        if (!element.second.empty())
        {
          returnValue += " AS " + element.second;
        };
      }
    };

    return returnValue;
  }

  /// @brief      Produces the string for a SELECT query.
  /// @returns    A string containing the select clause.
  /// @throws     None.
  /// @version    2021-04-11/GGB - Added support for "FOR SHARE" and "FOR UPDATE"
  /// @version    2020-04-25/GGB - Added support for the LIMIT and OFFSET clauses.
  /// @version    2017-08-12/GGB - Added check for countValue in selectClause if statement.
  /// @version    2015-03-30/GGB - Function created.

  std::string sqlWriter::createSelectQuery() const
  {
    std::string returnValue;

    if ( !selectFields.empty() || countValue ||
         !maxFields.empty() || !minFields.empty() )
    {
      returnValue += createSelectClause();
    }
    else
    {
      RUNTIME_ERROR(boost::locale::translate("MAPPED SQL WRITER: No Select fields in select clause."), E_SQLWRITER_NOSELECTFIELDS,
                    LIBRARYNAME);
    };

    if (!fromFields.empty())
    {
      returnValue += createFromClause();
    }
    else
    {
      RUNTIME_ERROR(boost::locale::translate("MAPPED SQL WRITER: No from fields in select clause."), E_SQLWRITER_NOFROMFIELD,
                    LIBRARYNAME);
    };

    if (!joinFields.empty())
    {
      returnValue += createJoinClause();
    }

    returnValue += createWhereClause();

    if (!groupByFields_.empty())
    {
      returnValue += createGroupByClause();
    };

    if (!orderByFields.empty())
    {
      returnValue += createOrderByClause();
    };

    returnValue += createLimitClause();

    if (forShare_)
    {
      returnValue += " FOR SHARE";
    }
    else if (forUpdate_)
    {
      returnValue += " FOR UPDATE";
    };

    return returnValue;
  }

  /// @brief Constructor for the from clause.
  /// @param[in] fields: The fields to add to the from clause.
  /// @throws None.
  /// @version 2017-08-20/GGB - Added support for aliases.
  /// @version 2015-03-30/GGB - Function created.

  sqlWriter &sqlWriter::from(std::initializer_list<std::string> fields)
  {
    for (auto elem : fields)
    {
      fromFields.emplace_back(elem, "");
    }

    return (*this);
  }

  /// @brief Set the query type to a 'DELETE' query.
  /// @param[in] tableName: The name of the table to execute the delete query on.
  /// @returns *this
  /// @version 2019-12-08/GGB - If the query is restarted without resetQuery() being called, then resetQuery() will be called.
  /// @version 2018-05-12/GGB - Added parameter for the deletion table name.
  /// @version 2015-03-30/GGB - Function created.

  sqlWriter &sqlWriter::deleteFrom(std::string const &tableName)
  {
    if (queryType != qt_none)
    {
      resetQuery();
    };

    queryType = qt_delete;

    deleteTable = tableName;

    return *this;
  }

  /// @brief      Add a single group by column by name.
  /// @param[in]  column: The column name.
  /// @returns    *this;
  /// @version    2022-04-07/GGB - Function created.

  sqlWriter &sqlWriter::groupBy(std::string const &column)
  {
    groupByFields_.push_back(column);

    return *this;
  }

  /// @brief  Add a single group by column by number.
  /// @param[in] column: The column name.
  /// @returns *this;
  /// @version 2022-04-07/GGB - Function created.

  sqlWriter &sqlWriter::groupBy(columnNumber_t column)
  {
    groupByFields_.push_back(column);

    return *this;
  }

  /// @brief  Add a group of group by columns.
  /// @param[in] column: The column name.
  /// @returns *this;
  /// @version 2022-04-07/GGB - Function created.

  sqlWriter &sqlWriter::groupBy(std::initializer_list<std::string> columns)
  {
    for (auto column: columns)
    {
      groupByFields_.push_back(column);
    }

    return *this;
  }

  /// @brief  Add a group of group by columns.
  /// @param[in] column: The column name.
  /// @returns *this;
  /// @version 2022-04-07/GGB - Function created.

  sqlWriter &sqlWriter::groupBy(std::initializer_list<columnNumber_t> columns)
  {
    for (auto column: columns)
    {
      groupByFields_.push_back(column);
    }

    return *this;
  }

  /// @brief      Set the query type to a 'INSERT' query.
  /// @param[in]  tableName: The table name to insert into.
  /// @returns    *this
  /// @version    2019-12-08/GGB - If the query is restarted without resetQuery() being called, then resetQuery() will be called.
  /// @version    2018-08-19/GGB - Function created.

  sqlWriter &sqlWriter::insertInto(std::string tableName)
  {
    if (queryType != qt_none)
    {
      resetQuery();
    };

    queryType = qt_insert;

    insertTable = tableName;

    return *this;
  }

  /// @brief      Set the query type to a 'INSERT' query.
  /// @param[in]  tableName: The table name to insert into.
  /// @param[in]  fields: The fields and field values.
  /// @returns    *this
  /// @version    2019-12-08/GGB - If the query is restarted without resetQuery() being called, then resetQuery() will be called.
  /// @version    2017-07-26/GGB - Added support for fields.
  /// @version    2015-03-30/GGB - Function created.

  sqlWriter &sqlWriter::insertInto(std::string tableName, std::initializer_list<std::string> fields)
  {
    if (queryType != qt_none)
    {
      resetQuery();
    };

    queryType = qt_insert;

    insertTable = tableName;

    for (auto elem : fields)
    {
      selectFields.push_back(elem);
    };

    return *this;
  }

  /// @brief      Adds join statements to the query.
  /// @param[in]  fields: The join statements to add.
  /// @returns    (*this)
  /// @throws     None.
  /// @version    2016-05-08/GGB - Function created.

  sqlWriter &sqlWriter::join(std::initializer_list<parameterJoin> fields)
  {
    for (auto elem: fields)
    {
      joinFields.push_back(elem);
    }

    return *this;
  }

  /// @brief  Sets the limit value. This will be interpreted based on the SQL dialect in use.
  /// @param[in] limit: The maximum number of records to return.
  /// @version 2020-04-25/GGB - Changed parameter from std::size_t to std::uint64_t.
  /// @version 2019-12-14/GGB - Changed parameter from long to std::size_t
  /// @version 2015-04-12/GGB - Function created.

  sqlWriter &sqlWriter::limit(std::uint64_t limit)
  {
    limitValue = limit;
    return (*this);
  }

  /// @brief  Sets the offset value. This will be interpreted based on the SQL dialect in use.
  /// @param[in] offset: The offset of the first record to return.
  /// @version 2020-04-25/GGB - Function created.

  sqlWriter &sqlWriter::offset(std::uint64_t offset)
  {
    offsetValue = offset;
    return (*this);
  }

  /// @brief      Add a single orderBy pair to the list.
  /// @param[in]  field: the field to order on.
  /// @param[in]  order: The sort order
  /// @returns    *this
  /// @throws     None.
  /// @version    202-05-25/GGB - Function created.

  sqlWriter &sqlWriter::orderBy(std::string field, EOrderBy order)
  {
    orderByFields.emplace_back(field, order);

    return *this;
  }


  /// @brief Copy the orderBy pairs to the list.
  /// @param[in] fields: The orderBy Pairs to copy
  /// @returns *this
  /// @throws None.
  /// @version 2019-10-24/GGB - Changed the orderBy type and storage.
  /// @version 2015-04-12/GGB - Function created.

  sqlWriter &sqlWriter::orderBy(std::initializer_list<std::pair<std::string, EOrderBy>> fields)
  {
    for (auto elem : fields)
    {
      orderByFields.push_back(elem);
    };

    return *this;
  }

  /// @brief      Prepares a query. For insert queries, values are replaced with the relevant character.
  /// @throws     GCL::CRuntimeError
  /// @version    2023-09-23/GGB - Function created.

  std::string sqlWriter::preparedQuery() const
  {
    std::string returnValue;

    switch (queryType)
    {
      case qt_select:
      {
        returnValue = createSelectQuery();
        break;
      };
      case qt_insert:
      {
        returnValue = createInsertQuery(true);
        break;
      };
      case qt_update:
      {
        returnValue = createUpdateQuery(true);
        break;
      }
      case qt_delete:
      {
        returnValue = createDeleteQuery();
        break;
      }
      case qt_upsert:
      {
        returnValue = createUpsertQuery(true);
        break;
      }
      case qt_call:
      {
        returnValue = createCall();
        break;
      }
      default:
      {
        CODE_ERROR();
        break;
      };
    }

    return returnValue;



  }

  /// @brief      Function to load a map file and store all the aliases.
  /// @param[in]  ifn: The file to read the database mapping from.
  /// @throws     GCL::CRuntimeError
  /// @version    2022-04-11/GGB - Converted to std::filesystem.
  /// @version    2013-01-26/GGB - Function created.

  void sqlWriter::readMapFile(std::filesystem::path const &ifn)
  {
    std::ifstream ifs;
    std::string textLine;
    std::string szCommand;
    int lineNumber = 1;
    std::string currentTable;
    size_t spacePosn, token1S, token1E, token2S, token2E, equalPosn;
    std::string szToken1, szToken2;

    ifs.open(ifn);

    if (!ifs)
    {
      RUNTIME_ERROR("Could not open SQL map file:" + ifn.native());
    }
    else
    {
      while (ifs.good())
      {
        std::getline(ifs, textLine);
        if ( (textLine.size() > 1) && (textLine[0] != ';') )
        {
          spacePosn = textLine.find_first_of(' ', 0);
          szCommand = textLine.substr(0, spacePosn);
          boost::trim(szCommand);
          equalPosn = textLine.find_first_of('=', 0);
          token1S = textLine.find_first_of('[', 0);
          token1E = textLine.find_first_of(']', 0);
          token2S = textLine.find_first_of('[', equalPosn);
          token2E = textLine.find_first_of(']', equalPosn);

          if (token1S < token1E)
          {
            szToken1 = textLine.substr(token1S + 1, token1E - token1S - 1);
          }
          else if (szCommand != END)
          {
            RUNTIME_ERROR("Error in SQL map file:" + ifn.native() + "Syntax error on line: " +
                          std::to_string(lineNumber) + " - Needs at least one token.");
          };

          if (token2S < token2E)
          {
            szToken2 = textLine.substr(token2S + 1, token2E - token2S - 1);
          }
          else
          {
            szToken2.clear();
          };

          if (szCommand == COLUMN)
          {
            if (currentTable.empty())
            {
              std::clog << "Error in SQL map file: " << ifn << std::endl;
              std::clog << "Syntax command on line: " << lineNumber << " - COLUMN directive found, but no TABLE directive in force." << std::endl;
              RUNTIME_ERROR(boost::locale::translate("MAPPED SQL WRITER: Syntax Error."), E_SQLWRITER_SYNTAXERROR, LIBRARYNAME);
            }
            else if (szToken1.empty())
            {
              std::clog << "Error in SQL map file: " << ifn << std::endl;
              std::clog << "Syntax command on line: " << lineNumber << " - COLUMN directive found, column name." << std::endl;
              RUNTIME_ERROR(boost::locale::translate("MAPPED SQL WRITER: Syntax Error."), E_SQLWRITER_SYNTAXERROR, LIBRARYNAME);
            }
            else if ( (*databaseMap.find(currentTable)).second.columnData.find(szToken1) ==
                      (*databaseMap.find(currentTable)).second.columnData.end())
            {
              RUNTIME_ERROR("Error in SQL map file: " + ifn.native() +
                            "Syntax command on line: " + std::to_string(lineNumber) + " - Invalid column name.");
            }
            else
            {
              if (!szToken2.empty())
              {
                setColumnMap(currentTable, szToken1, szToken2);
              };
            };
          }
          else if (szCommand == TABLE)
          {
            if (!currentTable.empty())
            {
              std::clog << "Error in SQL map file: " << ifn << std::endl;
              std::clog << "Syntax command on line: " << lineNumber << " - TABLE directive found, but a TABLE directive is already specified." << std::endl;
              RUNTIME_ERROR(boost::locale::translate("MAPPED SQL WRITER: Syntax Error."), E_SQLWRITER_SYNTAXERROR, LIBRARYNAME);
            }
            else if (szToken1.empty() )
            {
              std::clog << "Error in SQL map file: " << ifn << std::endl;
              std::clog << "Syntax command on line: " << lineNumber << " - TABLE directive found, but no table name." << std::endl;
              RUNTIME_ERROR(boost::locale::translate("MAPPED SQL WRITER: Syntax Error."), E_SQLWRITER_SYNTAXERROR, LIBRARYNAME);
            }
            else if (databaseMap.find(szToken1) == databaseMap.end())
            {
              std::clog << "Error in SQL map file: " << ifn << std::endl;
              std::clog << "Error on line: " << lineNumber << " - Invalid Table name." << std::endl;
              RUNTIME_ERROR(boost::locale::translate("MAPPED SQL WRITER: Invalid Table Name."), E_SQLWRITER_INVALIDTABLENAME,
                            LIBRARYNAME);
            }
            else
            {
              currentTable = szToken1;
              if (!szToken2.empty())
              {
                setTableMap(currentTable, szToken2);  // Add the alias into the record.
              };
            };
          }
          else if (szCommand == END)
          {
            if ( (token1S == textLine.npos) &&
                 (token1E == textLine.npos) &&
                 (token2S == textLine.npos) &&
                 (token2E == textLine.npos) &&
                 (equalPosn == textLine.npos) )
            {
              currentTable.clear();
            }
            else
            {
              std::clog << "Error in SQL map file: " << ifn << std::endl;
              std::clog << "Syntax command on line: " << lineNumber << std::endl;
              RUNTIME_ERROR(boost::locale::translate("MAPPED SQL WRITER: Syntax Error."), E_SQLWRITER_SYNTAXERROR, LIBRARYNAME);
            };
          }
          else
          {
            std::clog << "Error in SQL map file: " << ifn << std::endl;
            std::clog << "Invalid command on line: " << lineNumber << std::endl;
            RUNTIME_ERROR(boost::locale::translate("MAPPED SQL WRITER: Invalid Command."), E_SQLWRITER_INVALIDCOMMAND, LIBRARYNAME);
          };
        };

        lineNumber++;
      };
      ifs.close();
    };
  }

  /// @brief Resets all the fields for the query.
  /// @throws None.
  /// @version 2022-06-02/GGB - Added support for 'IN'
  /// @version 2022-05-01/GGB - Added support for 'RETURNING'
  /// @version 2021-04-13/GGB - Added procedure call support.
  /// @version 2021-04-11/GGB - Added "FOR SHARE" and "FOR UPDATE" functionality.
  /// @version 2020-04-25/GGB - Added offsetValue to support offsets.
  /// @version 2019-12-08/GGB - Update queryType = qt_none when the query is reset.
  /// @version 2017-08-19/GGB - Added support for distinct.
  /// @version 2017-08-12/GGB - Added support for count expressions.
  /// @version 2015-05-19/GGB - Function created.

  void sqlWriter::resetQuery()
  {
    selectFields.clear();
    fromFields.clear();
    insertTable.clear();
    orderByFields.clear();
    joinFields.clear();
    limitValue.reset();
    offsetValue.reset();
    countValue.reset();
    distinct_ = false;
    maxFields.clear();
    minFields.clear();
    updateTable.clear();
    setFields.clear();
    queryType = qt_none;
    forShare_ = false;
    forUpdate_ = false;
    procedureName_.clear();
    groupByFields_.clear();
    returningFields_.clear();

    resetValues();
    resetWhere();
  }

  /// @brief    Resets the value clause of a query.
  /// @throws   None.
  /// @version  2022-07-23/GGB - Changed to use std::variant to store data.
  /// @version  2022-05-12/GGB - Function created.

  void sqlWriter::resetValues()
  {
    insertValue = valueType_t{};
  }

  /// @brief Resets the where clause of a query.
  /// @throws None.
  /// @version 2017-08-19/GGB - Function created.

  void sqlWriter::resetWhere()
  {
    whereClause_ = whereVariant_t{};
  }

  /// @brief      Saves the returning fields.
  /// @returns    A reference to (*this)
  /// @version    2022-05-01/GGB - Function created.

  sqlWriter &sqlWriter::returning(std::string const &field)
  {
    returningFields_.emplace_back(field);

    return *this;
  }

  /// @brief      Saves the returning fields.
  /// @returns    A reference to (*this)
  /// @version    2022-05-01/GGB - Function created.

  sqlWriter &sqlWriter::returning(std::initializer_list<std::string> fields)
  {
    for (auto elem : fields)
    {
      returningFields_.push_back(elem);
    };

    return *this;
  }

  /// @brief    Returns the number of inserion rows for an insert query.
  /// @returns  The number of rows.
  /// @version  2023-09-23/GGB - Function created.

  std::size_t sqlWriter::rowCount() const
  {
    std::size_t rv = 0;

    switch (queryType)
    {
      case qt_insert:
      {
        if (std::holds_alternative<valueStorage_t>(insertValue))
        {
          valueStorage_t const valueStorage = std::get<valueStorage_t>(insertValue);
          rv = valueStorage.size();
        }
        else
        {
          CODE_ERROR();
        }
        break;

        rv = selectFields.size();
        break;
      }
      default:
      {
        CODE_ERROR();
        break;
      }
    }

    return rv;
  }

  /// @brief      Specify the selection fields for the select query.
  /// @param[in]  fields: The fields to add to the select clause.
  /// @returns    A reference to (*this)
  /// @version    2020-10-02/GGB - Call to select() to reset the query and set the query type.
  /// @version    2019-12-08/GGB - If the query is restarted without resetQuery() being called, then resetQuery() will be called.
  /// @version    2014-12-19/GGB - Function created.

  sqlWriter &sqlWriter::select(std::initializer_list<selectExpression_t> fields)
  {
    if (queryType != qt_none)
    {
      resetQuery();
    };

    queryType = qt_select;

    for (auto elem : fields)
    {
      selectFields.push_back(elem);
    };

    return *this;
  }

  /// @brief      Processes a single set clause.
  /// @param[in]  columnName: The columnName to set
  /// @param[in]  value: The value to set.
  /// @returns    (*this)
  /// @throws     None.
  /// @version  2017-08-21/GGB - Function created.

  sqlWriter &sqlWriter::set(std::string const &columnName, parameter_t const &value)
  {
    setFields.emplace_back(columnName, value);

    return (*this);
  }

  /// @brief      Processes a list of set clauses
  /// @param[in]  fields: The initialiser list of set clauses.
  /// @returns    (*this)
  /// @throws
  /// @version    2020-03-24/GGB - Function created.

  sqlWriter &sqlWriter::set(std::initializer_list<parameterPair> fields)
  {
    for (auto elem : fields)
    {
      setFields.push_back(elem);
    };

    return *this;
  }

  /// @brief      Converts the query into an SQL query string.
  /// @returns    The SQL query as a string.
  /// @throws     None.
  /// @version    2021-04-13/GGB - Added stored procedure calls.
  /// @version    2019-12-08/GGB - Added UPSERT query.
  /// @version    2017-08-12/GGB - Function created.

  std::string sqlWriter::string() const
  {
    std::string returnValue;

    switch (queryType)
    {
      case qt_select:
      {
        returnValue = createSelectQuery();
        break;
      };
      case qt_insert:
      {
        returnValue = createInsertQuery();
        break;
      };
      case qt_update:
      {
        returnValue = createUpdateQuery();
        break;
      }
      case qt_delete:
      {
        returnValue = createDeleteQuery();
        break;
      }
      case qt_upsert:
      {
        returnValue = createUpsertQuery();
        break;
      }
      case qt_call:
      {
        returnValue = createCall();
        break;
      }
      default:
      {
        CODE_ERROR();
        break;
      };
    }

    return returnValue;
  }

  /// @brief  Return the mapped table name.
  /// @todo Implement the getTableMap function. (Bug# 0000194)

  std::string sqlWriter::getTableMappedName(std::string const &search) const
  {
    std::string returnValue = search;

    return returnValue;
  }

  std::string sqlWriter::sum(std::string const &col)
  {
    return "SUM(" + col + ")";
  }

  sqlWriter &sqlWriter::selfJoin(std::string const &, std::string const &)
  {

    return *this;
  }

  /// @brief Sets the table name for an update query.
  /// @param[in] tableName: The tableName to update.
  /// @returns (*this)
  /// @throws None.
  /// @version 2019-12-08/GGB - If the query is restarted without resetQuery() being called, then resetQuery() will be called.
  /// @version 2017-08-21/GGB - Function created.

  sqlWriter &sqlWriter::update(std::string const &tableName)
  {
    if (queryType != qt_none)
    {
      resetQuery();
    };

    queryType = qt_update;

    updateTable = tableName;

    return (*this);
  }

  /// @brief      Sets the table name for an upsert query.
  /// @param[in]  tableName: The tableName to upsert.
  /// @returns  (*this)
  /// @throws None.
  /// @details @code upsert("tbl_scheduleModification").set("Value", true).where({"ModificationType", "=", 1} @endcode
  ///          translates to
  ///          @code INSERT INTO tbl_scheduleModification(ModificationType, Value) VALUES (1, true) ON DUPLICATE UPDATE
  ///                Value = true @endcode
  /// @version 2019-12-08/GGB - Function created.

  sqlWriter &sqlWriter::upsert(std::string const &tableName)
  {
    if (queryType != qt_none)
    {
      resetQuery();
    };

    queryType = qt_upsert;

    insertTable = tableName;

    return (*this);
  }

  /// @brief        Stores the value fields for the query.
  /// @param[in]    fields: The parameter values to include in the query.
  /// @returns      (*this)
  /// @version      2022-07-23/GGB - Extended to allow INSERT INTO SELECT
  /// @version      2017-07-26/GGB - Changed code to use parameter rather than parameter pair.
  /// @version      2015-03-31/GGB - Function created.

  sqlWriter &sqlWriter::values(std::initializer_list<parameterStorage> fields)
  {
    valueStorage_t valueFields;

    for (const auto &f : fields)
    {
      valueFields.emplace_back(f);
    };

    insertValue = std::move(valueFields);

    return *this;
  }

  /// @brief        Stores values passed in a standard vector.
  /// @param[in]    insertRows: The rows of values to insert.
  /// @returns      (*this)
  /// @version      2022-07-23/GGB - Extended to allow INSERT INTO SELECT
  /// @version      2022-04-29/GGB - Function created.

  sqlWriter &sqlWriter::values(valueStorage_t &&insertRows)
  {
    insertValue = std::move(insertRows);

    return *this;
  }

  /// @brief        Returns a reference to the values for the query.
  /// @returns      A const reference to the values.
  /// @throws       None.
  /// @version      2023-03-24/GGB - Function created.

  sqlWriter::valueType_t const &sqlWriter::values() const noexcept
  {
    return insertValue;
  }

  /// @brief        Stores the subquery for a INSERT INTO SELECT query.
  /// @param[in]    subQuery: The rows of values to insert.
  /// @returns      (*this)
  /// @version      2022-07-23/GGB - Function created.

  sqlWriter &sqlWriter::values(pointer_t subQuery)
  {
    insertValue = std::move(subQuery);

    return *this;
  }

  /// @brief      Verifies if a string constitutes a valid SQL operator.
  /// @param[in]  oper: The string to test.
  /// @returns    true - The string is a valid operator.
  /// @returns    false - The string is not a valid operator.
  /// @note       Valid operators are: "=", "<>", "!=", ">", "<", ">=", "<=", "BETWEEN", "LIKE", "IN"
  /// @throws     None.
  /// @version

  bool sqlWriter::verifyOperator(std::string const &oper) const
  {
    bool returnValue = false;

    returnValue = ( (oper == "=") || (oper == "<>") || (oper == "!=") || (oper == ">") || (oper == "<") ||
                    (oper == ">=") || (oper == "<=") || (oper == "BETWEEN") || (oper == "LIKE") || (oper == "IN"));

    return returnValue;
  }

  /// @brief Assigns a whereVaraint to the where clause.
  /// @param[in] w: whereVariant.
  /// @returns *this
  /// @version  2022-06-09/GGB - Function created

  sqlWriter &sqlWriter::where(whereVariant_t &&w)
  {
    whereClause_ = std::move(w);

    return *this;
  }

  /// @brief      to_string function for a columnRef.
  /// @param[in]  bv: The value to convert to a std::string.
  /// @version    2022-09-19/GGB - Function created.

  std::string to_string(GCL::sqlWriter::columnRef const &cr)
  {
    return cr.to_string();
  }

    /*
     * Friend functions.
     */


  sqlWriter::whereVariant_t where_v(std::string col, operator_t op, sqlWriter::parameter_t param)
  {
    return {std::make_tuple(col, op, param)};
  }

  sqlWriter::whereVariant_t where_v(std::string col, operator_t op, sqlWriter::parameterVector_t param)
  {
    return {std::make_tuple(col, op, param)};
  }

  /// @brief      Parameter variant to handle a parameterSet.
  /// @param[in]  col: The column name.
  /// @param[in]  op: The operator to apply. (Should be IN or NOT IN)
  /// @param[in]  param: The set containing the parameters.
  /// @returns    The where variant.
  /// @version    2023-10-19/GGB - Function created.

  sqlWriter::whereVariant_t where_v(std::string col, operator_t op, sqlWriter::parameterSet_t param)
  {
    RUNTIME_ASSERT((op == in) || (op == nin), "Only IN and NIN operators allowed with parameter sets.");

    return {std::make_tuple(col, op, param)};
  }

  sqlWriter::whereVariant_t where_v(std::string col, operator_t op, sqlWriter::pointer_t param)
  {
    return {std::make_tuple(col, op, std::move(param))};
  }

  sqlWriter::whereVariant_t where_v(sqlWriter::whereVariant_t lhs, logicalOperator_t op, sqlWriter::whereVariant_t rhs)
  {
    sqlWriter::whereLogical_t wl = std::make_tuple(std::make_unique<sqlWriter::whereVariant_t>(std::move(lhs)),
                                                   op,
                                                   std::make_unique<sqlWriter::whereVariant_t>(std::move(rhs)));


    return {std::move(wl)};
  }

  sqlWriter::whereVariant_t where_v(std::string col, operator_t op, sqlWriter &&rhs)
  {
    return {std::make_tuple(col, op, std::make_unique<sqlWriter>(std::move(rhs)))};
  }


}  // namespace GCL

