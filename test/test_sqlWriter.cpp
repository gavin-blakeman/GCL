﻿#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define TEST  // Used to exclude error reporting and avoid having to link GCL + others.

#include <vector>

#include "include/SQLWriter.h"

BOOST_AUTO_TEST_SUITE(sqlWriter_test)

BOOST_AUTO_TEST_CASE(hasBindValues_select, *boost::unit_test::tolerance(0.001))
{
  using namespace GCL;
  sqlWriter sqlQuery1, sqlQuery2, sqlQuery3, sqlQuery4;
  std::uint32_t ID;

  sqlQuery1.select({"ID"}).from("TBL_TEST").where("COL_TEXT", eq, ID);
  BOOST_REQUIRE_NO_THROW(sqlQuery1.hasBindValues());
  BOOST_TEST(sqlQuery1.hasBindValues()  == false);

  sqlQuery2.select({"ID"}).from("TBL_TEST").where("COL_TEXT", eq, sqlWriter::bindValue_t("test"));
  BOOST_REQUIRE_NO_THROW(sqlQuery2.hasBindValues());
  BOOST_TEST(sqlQuery2.hasBindValues()  == true);

  sqlQuery3.select({"ID"}).from("TBL_TEST").where(where_v(where_v("COL_TEXT", eq, sqlWriter::bindValue_t("test")),
                                                          AND,
                                                          where_v("COL_TEXT", eq, "tesxt")));
  BOOST_REQUIRE_NO_THROW(sqlQuery3.hasBindValues());
  BOOST_TEST(sqlQuery3.hasBindValues()  == true);

  sqlQuery4.select({"ID"}).from("TBL_TEST").where(where_v(where_v("COL_TEXT", eq, "test"),
                                                          AND,
                                                          where_v("COL_TEXT", eq, "tesxt")));
  BOOST_REQUIRE_NO_THROW(sqlQuery4.hasBindValues());
  BOOST_TEST(sqlQuery4.hasBindValues()  == false);
}

BOOST_AUTO_TEST_CASE(hasBindValues_delete, *boost::unit_test::tolerance(0.001))
{
  using namespace GCL;
  sqlWriter sqlQuery1, sqlQuery2, sqlQuery3, sqlQuery4;
  std::uint32_t ID;

  sqlQuery1.deleteFrom({"ID"}).where("COL_TEXT", eq, ID);
  BOOST_REQUIRE_NO_THROW(sqlQuery1.hasBindValues());
  BOOST_TEST(sqlQuery1.hasBindValues()  == false);

  sqlQuery2.deleteFrom({"ID"}).where("COL_TEXT", eq, sqlWriter::bindValue_t("test"));
  BOOST_REQUIRE_NO_THROW(sqlQuery2.hasBindValues());
  BOOST_TEST(sqlQuery2.hasBindValues()  == true);

  sqlQuery3.deleteFrom({"ID"}).where(where_v(where_v("COL_TEXT", eq, sqlWriter::bindValue_t("test")),
                                                          AND,
                                                          where_v("COL_TEXT", eq, "tesxt")));
  BOOST_REQUIRE_NO_THROW(sqlQuery3.hasBindValues());
  BOOST_TEST(sqlQuery3.hasBindValues()  == true);

  sqlQuery4.deleteFrom({"ID"}).where(where_v(where_v("COL_TEXT", eq, "test"),
                                                          AND,
                                                          where_v("COL_TEXT", eq, "tesxt")));
  BOOST_REQUIRE_NO_THROW(sqlQuery4.hasBindValues());
  BOOST_TEST(sqlQuery4.hasBindValues()  == false);
}

BOOST_AUTO_TEST_CASE(hasBindValues_update)
{
  using namespace GCL;
  sqlWriter sqlQuery1, sqlQuery2, sqlQuery3;
  std::uint32_t ID;
  std::string test;

  sqlQuery1.update("TBL").set({
    {"COL1", ID},
    {"COL2", ID},
    {"COL3", ID},
    {"COL4", ID},
  }).where("Test", eq, ID);
  BOOST_REQUIRE_NO_THROW(sqlQuery1.hasBindValues());
  BOOST_TEST(sqlQuery1.hasBindValues()  == false);

  sqlQuery2.update("TBL").set({
    {"COL1", sqlWriter::bindValue_t(test)},
    {"COL2", test},
    {"COL3", ID},
    {"COL4", ID},
  }).where("Test", eq, ID);
  BOOST_REQUIRE_NO_THROW(sqlQuery2.hasBindValues());
  BOOST_TEST(sqlQuery2.hasBindValues()  == true);

  sqlQuery3.update("TBL").set({
    {"COL1", "Test"},
    {"COL2", test},
    {"COL3", ID},
    {"COL4", ID},
  }).where("Test", eq, sqlWriter::bindValue_t(test));
  BOOST_REQUIRE_NO_THROW(sqlQuery3.hasBindValues());
  BOOST_TEST(sqlQuery3.hasBindValues()  == true);
}

BOOST_AUTO_TEST_CASE(hasBindValues_insert)
{
  using namespace GCL;
  sqlWriter sqlQuery1, sqlQuery2;
  std::uint32_t ID;
  std::string test;

  sqlQuery1.insertInto("TBL", {"COL1", "COL2", "COL3", "COL4", "COL5", "COL6"})
                .values({ {ID, ID, ID, ID, ID,ID}});
  BOOST_REQUIRE_NO_THROW(sqlQuery1.hasBindValues());
  BOOST_TEST(sqlQuery1.hasBindValues()  == false);

  sqlQuery2.insertInto("TBL", {"COL1", "COL2", "COL3", "COL4", "COL5", "COL6"})
                  .values({ {ID, sqlWriter::bindValue_t(test), ID, ID, "Text", ID}});
  BOOST_REQUIRE_NO_THROW(sqlQuery2.hasBindValues());
  BOOST_TEST(sqlQuery2.hasBindValues()  == true);
}

BOOST_AUTO_TEST_CASE(shouldParameterise_select)
{
  using namespace GCL;
  sqlWriter sqlQuery1, sqlQuery2, sqlQuery3;
  std::uint32_t ID;

  sqlQuery1.select({"ID"}).from("TBL_TEST").where("COL_TEXT", eq, ID);
  BOOST_REQUIRE_NO_THROW(sqlQuery1.shouldParameterise());
  BOOST_TEST(sqlQuery1.shouldParameterise()  == false);

  sqlQuery2.select({"ID"}).from("TBL_TEST").where("COL_TEXT", eq, "test");
  BOOST_REQUIRE_NO_THROW(sqlQuery2.shouldParameterise());
  BOOST_TEST(sqlQuery2.shouldParameterise()  == true);

  sqlQuery3.select({"ID"}).from("TBL_TEST").where(where_v(where_v("COL_TEXT", eq, sqlWriter::bindValue_t("test")),
                                                          AND,
                                                          where_v("COL_TEXT", eq, "text")));
  BOOST_REQUIRE_NO_THROW(sqlQuery3.shouldParameterise());
  BOOST_TEST(sqlQuery3.shouldParameterise()  == true);
}

BOOST_AUTO_TEST_CASE(shouldParameterise_delete)
{
  using namespace GCL;
  sqlWriter sqlQuery1, sqlQuery2, sqlQuery3;
  std::uint32_t ID;
  std::string test;

  sqlQuery1.deleteFrom({"ID"}).where("COL_TEXT", eq, ID);
  BOOST_REQUIRE_NO_THROW(sqlQuery1.shouldParameterise());
  BOOST_TEST(sqlQuery1.shouldParameterise()  == false);

  sqlQuery2.deleteFrom({"ID"}).where("COL_TEXT", eq, "test");
  BOOST_REQUIRE_NO_THROW(sqlQuery2.shouldParameterise());
  BOOST_TEST(sqlQuery2.shouldParameterise()  == true);

  sqlQuery3.deleteFrom({"ID"}).where(where_v(where_v("COL_TEXT", eq, sqlWriter::bindValue_t("test")),
                                                          AND,
                                                          where_v("COL_TEXT", eq, "text")));
  BOOST_REQUIRE_NO_THROW(sqlQuery3.shouldParameterise());
  BOOST_TEST(sqlQuery3.shouldParameterise()  == true);
}

BOOST_AUTO_TEST_CASE(shouldParameterise_update)
{
  using namespace GCL;
  sqlWriter sqlQuery1, sqlQuery2, sqlQuery3;
  std::uint32_t ID;
  std::string test;

  sqlQuery1.update("TBL").set({
    {"COL1", ID},
    {"COL2", ID},
    {"COL3", ID},
    {"COL4", ID},
  }).where("Test", eq, ID);
  BOOST_REQUIRE_NO_THROW(sqlQuery1.shouldParameterise());
  BOOST_TEST(sqlQuery1.shouldParameterise()  == false);

  sqlQuery2.update("TBL").set({
    {"COL1", "Test"},
    {"COL2", test},
    {"COL3", ID},
    {"COL4", ID},
  }).where("Test", eq, ID);
  BOOST_REQUIRE_NO_THROW(sqlQuery2.shouldParameterise());
  BOOST_TEST(sqlQuery2.shouldParameterise()  == true);

  sqlQuery3.update("TBL").set({
    {"COL1", "Test"},
    {"COL2", test},
    {"COL3", ID},
    {"COL4", ID},
  }).where("Test", eq, "Test");

  BOOST_REQUIRE_NO_THROW(sqlQuery3.shouldParameterise());
  BOOST_TEST(sqlQuery3.shouldParameterise()  == true);
}

BOOST_AUTO_TEST_CASE(shouldParameterise_insert)
{
  using namespace GCL;
  sqlWriter sqlQuery1, sqlQuery2, sqlQuery3;
  std::uint32_t ID;
  std::string test;

  sqlQuery1.insertInto("TBL", {"COL1", "COL2", "COL3", "COL4", "COL5", "COL6"})
          .values({ {ID, ID, ID, ID, ID,ID}});
  BOOST_REQUIRE_NO_THROW(sqlQuery1.shouldParameterise());
  BOOST_TEST(sqlQuery1.shouldParameterise()  == false);

  sqlQuery2.insertInto("TBL", {"COL1", "COL2", "COL3", "COL4", "COL5", "COL6"})
            .values({ {ID, ID, ID, ID, "Text", ID}});
  BOOST_REQUIRE_NO_THROW(sqlQuery2.shouldParameterise());
  BOOST_TEST(sqlQuery2.shouldParameterise()  == true);

  sqlQuery3.insertInto("TBL", {"COL1", "COL2", "COL3", "COL4", "COL5", "COL6"})
              .values({ {ID, test, ID, ID, "Text", ID}});
  BOOST_REQUIRE_NO_THROW(sqlQuery3.shouldParameterise());
  BOOST_TEST(sqlQuery3.shouldParameterise()  == true);
}

BOOST_AUTO_TEST_CASE(bindValues_insert)
{

  using namespace GCL;
  sqlWriter sqlQuery1, sqlQuery2, sqlQuery3;
  std::uint32_t ID = 1;
  std::string test;

  std::list<sqlWriter::bindParameter_t> rv;

  BOOST_TEST_INFO("Testing retrieval of bindValues from insert query.");

  sqlQuery1.insertInto("TBL", {"COL1", "COL2", "COL3", "COL4", "COL5", "COL6"})
          .values({ {ID++, ID++, ID++, ID++, ID++, ID } });

  BOOST_REQUIRE_NO_THROW(sqlQuery1.bindValues(rv));
  ID = 1;
  std::vector<uint32_t> tv = { ID++, ID++, ID++, ID++, ID++, ID };
  std::vector<uint32_t> rtv;

  // The RV is a vector of parameterValues_t, in itself a vector of parameters.

  for (auto &column: rv)
  {
    sqlWriter::parameterValues_t tempVal = std::get<sqlWriter::parameterValues_t>(column);
    for (auto &value: tempVal)
    {
      rtv.push_back(std::get<std::uint32_t>(value.get()));
    };
  }
  BOOST_TEST(tv == rtv);
}

BOOST_AUTO_TEST_CASE(bindValues_update)
{
  using namespace GCL;
  sqlWriter sqlQuery1;
  std::uint32_t ID = 1;
  std::string test;

  std::vector<std::string> tv = { "STR1", "STR2", "STR3", "STR4", "STR5", "STR6"};
  auto i = tv.begin();

  std::list<sqlWriter::bindParameter_t> rv;

  BOOST_TEST_INFO("Testing retrieval of bindValues from update query.");

  sqlQuery1.update("TBL").set({ {"COL1", *i++}, {"COL2", *i++}, {"COL3", *i++}, {"COL4", *i++}, {"COL5", *i++}, {"COL6", *i}});

  BOOST_REQUIRE_NO_THROW(sqlQuery1.bindValues(rv));
  std::vector<std::string> rtv;
  for (auto &val: rv)
  {
    rtv.push_back(std::get<std::string>(std::get<sqlWriter::parameter_ref>(val).get()));
  }
  BOOST_TEST(tv == rtv);
}

BOOST_AUTO_TEST_CASE(bindValues_select)
{
  using namespace GCL;
  sqlWriter sqlQuery1;
  std::uint32_t ID = 1;
  std::string test;
  std::list<sqlWriter::bindParameter_t> rv;

  BOOST_TEST_INFO("Testing retrieval of bindValues from update query.");

  sqlQuery1.select({"COL"}).from("TBL1").where("COL1", eq, "STR1");
  BOOST_REQUIRE_NO_THROW(sqlQuery1.bindValues(rv));
  BOOST_TEST("STR1" == std::get<std::string>(std::get<sqlWriter::parameter_ref>(rv.back()).get()));
}

BOOST_AUTO_TEST_CASE(tableName_test)
{
  using namespace GCL;

  sqlWriter::tableName_t table1("t");
  BOOST_TEST(table1.tableDefinition() == "t");
  BOOST_TEST(table1.table() == "t");

  sqlWriter::tableName_t table2("t", "a");
  BOOST_TEST(table2.tableDefinition() == "t AS a");
  BOOST_TEST(table2.table() == "a");
}

BOOST_AUTO_TEST_CASE(select_table_as)
{
  using namespace GCL;
  sqlWriter sqlQuery;
  sqlQuery.select({"*"}).from("table");

  BOOST_TEST(sqlQuery.string() == "SELECT * FROM table");
}

BOOST_AUTO_TEST_SUITE_END()
