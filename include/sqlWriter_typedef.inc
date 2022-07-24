public:
  using parameter = GCL::any;
  using logicalOperatorMap_t = std::map<logicalOperator_t, std::string>;
  using operatorMap_t = std::map<operator_t, std::string>;

  using pointer_t = std::unique_ptr<sqlWriter>;

  using groupByStorage = std::vector<parameter>;
  using parameterVector_t = std::vector<parameter>;
  typedef std::pair<std::string, parameter> parameterPair;
  typedef std::pair<std::string, std::string> stringPair;
  using parameterTriple = std::tuple<std::string, std::string, parameter>;
  typedef std::pair<std::string, EOrderBy> orderBy_t;

  using parameterStorage = std::vector<parameter>;
  typedef std::vector<parameterPair> pairStorage;
  using tripleStorage = std::vector<parameterTriple>;
  typedef std::vector<stringPair> stringPairStorage;
  typedef std::vector<orderBy_t> orderByStorage_t;

  using valueStorage = std::vector<parameterVector_t>;     // This is to allow multiple insertions in one statement.
  using valueType_t = std::variant<std::monostate, valueStorage, pointer_t>;

  typedef std::tuple<std::string, std::string, EJoin, std::string, std::string> parameterJoin;

  typedef std::vector<parameterJoin> joinStorage;

    // Where Types

  using parameterVariant_t = std::variant<parameter, parameterVector_t, pointer_t>;
  using whereTest_t = std::tuple<std::string, operator_t, parameterVariant_t>;

  struct whereVariant_t;
  using whereLogical_t = std::tuple<std::unique_ptr<whereVariant_t>, logicalOperator_t, std::unique_ptr<whereVariant_t>>;

  struct whereVariant_t
  {
  public:
    std::variant<std::monostate, whereTest_t, whereLogical_t> base;
  };

