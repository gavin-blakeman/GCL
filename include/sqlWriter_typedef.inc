public:

  /* The parameter_t variant contains all the possible SQL database types. Conversion of other types to the database
     types needs to be handled by the calling code.
   */

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
  using selectExpressionStorage_t = std::list<selectExpression_t>;

  using pointer_t = std::unique_ptr<sqlWriter>;
  using groupBy_t = std::variant<std::size_t, std::string>;

  using groupByStorage = std::vector<groupBy_t>;
  using parameterVector_t = std::vector<parameter_t>;
  using parameterSet_t = std::set<parameter_t>;
  using parameterPair = std::pair<std::string, parameter_t>;
  using stringPair = std::pair<std::string, std::string>;
  using parameterTriple = std::tuple<std::string, std::string, parameter_t>;
  using orderBy_t = std::pair<std::string, EOrderBy>;

  using parameterStorage = std::vector<parameter_t>;
  using pairStorage = std::vector<parameterPair>;
  using tripleStorage = std::vector<parameterTriple>;
  using stringPairStorage = std::vector<stringPair>;
  using orderByStorage_t = std::vector<orderBy_t>;

  using valueStorage_t = std::vector<parameterVector_t>;     // This is to allow multiple insertions in one statement.
  using valueType_t = std::variant<std::monostate, valueStorage_t, pointer_t>;

  typedef std::tuple<std::string, std::string, EJoin, std::string, std::string> parameterJoin;

  typedef std::vector<parameterJoin> joinStorage;

    // Where clause types

  using parameterVariant_t = std::variant<parameter_t, parameterVector_t, parameterSet_t, pointer_t>;
  using whereTest_t = std::tuple<std::string, operator_t, parameterVariant_t>;

  struct whereVariant_t;
  using whereLogical_t = std::tuple<std::unique_ptr<whereVariant_t>, logicalOperator_t, std::unique_ptr<whereVariant_t>>;

  struct whereVariant_t
  {
  public:
    whereVariant_t() = default;
    whereVariant_t(whereVariant_t const &) = default;
    whereVariant_t(whereVariant_t &&) = default;
    whereVariant_t(whereTest_t &&wt) : base(std::move(wt)) {}
    whereVariant_t(whereLogical_t &&wl) : base(std::move(wl)) {}
    whereVariant_t(std::string const &s, operator_t o, parameterVariant_t &&p)
    {
      whereTest_t wt = std::make_tuple(s, o, std::move(p));
      base = std::move(wt);
    }

    whereVariant_t &operator=(whereVariant_t const &) = default;
    whereVariant_t &operator=(whereVariant_t &&) = default;

    std::variant<std::monostate, whereTest_t, whereLogical_t> base;
  };

    // From Types

  using from_t = std::variant<std::string, pointer_t>;                ///!< Field/SubQuery
  using fromPair_t = std::pair<from_t, std::optional<std::string>>;   ///!< Field:
  using fromStorage_t = std::vector<fromPair_t>;

  // Bind paramters

  using parameter_ref = std::reference_wrapper<parameter_t>;
  using bindParameter_t = std::variant<std::monostate,
                                       parameter_ref,
                                       std::reference_wrapper<parameterVariant_t>>;
