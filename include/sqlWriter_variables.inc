private:
  enum EQueryType
  {
    qt_none,
    qt_select,
    qt_insert,
    qt_delete,
    qt_update,
    qt_upsert,
    qt_call,
  };

    // Group all storage here. This makes it easier to check/update copy constructors and move constructors.
    // When adding to this, update the move and copy constructors.

  static operatorMap_t operatorMap;
  static logicalOperatorMap_t logicalOperatorMap;
  EDialect dialect = MYSQL;
  TDatabaseMap databaseMap;
  std::vector<std::string> selectFields;
  std::vector<std::string> returningFields_;
  stringPairStorage fromFields;
  std::string insertTable;
  valueStorage valueFields;
  groupByStorage groupByFields_;
  orderByStorage_t orderByFields;
  joinStorage joinFields;
  std::optional<std::uint64_t> offsetValue;
  std::optional<std::uint64_t> limitValue;
  std::optional<std::pair<std::string, std::string>> countValue;
  bool distinct_ = false;
  stringPairStorage minFields;
  stringPairStorage maxFields;
  std::string updateTable;
  pairStorage setFields;
  std::string deleteTable;
  std::string procedureName_;
  parameterStorage procedureParameters_;
  EQueryType queryType;
  std::string currentTable;
  whereVariant_t whereClause_;

  bool forUpdate_ = false;
  bool forShare_ = false;
