﻿public:

  /* The parameter_t variant contains all the possible SQL database types. Conversion of other types to the database
     types needs to be handled by the calling code.
   */

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
  using logicalOperatorMap_t = std::map<logicalOperator_t, std::string>;
  using operatorMap_t = std::map<operator_t, std::string>;

  using selectExpression_t = std::variant<std::uint8_t,
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
                                          decimal_t>;
  using selectExpressionStorage_t = std::vector<selectExpression_t>;

  using pointer_t = std::unique_ptr<sqlWriter>;
  using groupBy_t = std::variant<std::size_t, std::string>;

  using groupByStorage = std::vector<groupBy_t>;
  using parameterVector_t = std::vector<parameter_t>;
  typedef std::pair<std::string, parameter_t> parameterPair;
  typedef std::pair<std::string, std::string> stringPair;
  using parameterTriple = std::tuple<std::string, std::string, parameter_t>;
  typedef std::pair<std::string, EOrderBy> orderBy_t;

  using parameterStorage = std::vector<parameter_t>;
  typedef std::vector<parameterPair> pairStorage;
  using tripleStorage = std::vector<parameterTriple>;
  typedef std::vector<stringPair> stringPairStorage;
  typedef std::vector<orderBy_t> orderByStorage_t;

  using valueStorage = std::vector<parameterVector_t>;     // This is to allow multiple insertions in one statement.
  using valueType_t = std::variant<std::monostate, valueStorage, pointer_t>;

  typedef std::tuple<std::string, std::string, EJoin, std::string, std::string> parameterJoin;

  typedef std::vector<parameterJoin> joinStorage;

    // Where Types

  using parameterVariant_t = std::variant<parameter_t, parameterVector_t, pointer_t>;
  using whereTest_t = std::tuple<std::string, operator_t, parameterVariant_t>;

  struct whereVariant_t;
  using whereLogical_t = std::tuple<std::unique_ptr<whereVariant_t>, logicalOperator_t, std::unique_ptr<whereVariant_t>>;

  struct whereVariant_t
  {
  public:
    std::variant<std::monostate, whereTest_t, whereLogical_t> base;
  };

    // From Types

  using from_t = std::variant<std::string, pointer_t>;
  using fromPair_t = std::pair<from_t, std::optional<std::string>>;
  using fromStorage_t = std::vector<fromPair_t>;
