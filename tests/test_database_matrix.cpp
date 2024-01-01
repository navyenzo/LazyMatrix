//-------------------------------------------------------------------
/**
 * @file test_database_matrix.cpp
 * @brief Test cases for DatabaseMatrix class using Catch2 and SQLite.
 *
 * This test suite is designed to validate the functionality of the DatabaseMatrix class,
 * particularly its ability to interact with a SQLite database and perform basic operations
 * such as fetching the number of rows and columns and reading data using operator()(i, j).
 */
//-------------------------------------------------------------------



//-------------------------------------------------------------------
#include <iostream>
#include <catch2/catch_all.hpp>

#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Data/SQLite/SQLiteException.h>
#include <Poco/Data/Session.h>

#include "database_matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;
//-------------------------------------------------------------------



//-------------------------------------------------------------------
TEST_CASE("DatabaseMatrix operations", "[DatabaseMatrix]")
{
    Poco::Data::SQLite::Connector::registerConnector();

    try
    {
        Session session("SQLite", "memory");

        session << "DROP TABLE IF EXISTS test_table", now;
        session << "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT)", now;
        session << "INSERT INTO test_table (name) VALUES('Alice')", now;
        session << "INSERT INTO test_table (name) VALUES('Bob')", now;
        session << "INSERT INTO test_table (name) VALUES('Charlie')", now;

        LazyMatrix::DatabaseMatrix matrix(session, "test_table");

        SECTION("Test row count")
        {
            auto actual_row_count = matrix.rows();
            REQUIRE(actual_row_count == 3);
        }

        SECTION("Test column count")
        {
            REQUIRE(matrix.columns() == 2);
        }

        SECTION("Test data retrieval")
        {
            // Check first row
            auto name_row1 = matrix(0, 1).convert<std::string>();
            REQUIRE(name_row1 == "Alice");

            // Check second row
            auto name_row2 = matrix(1, 1).convert<std::string>();
            REQUIRE(name_row2 == "Bob");

            // Check third row
            auto name_row3 = matrix(2, 1).convert<std::string>();
            REQUIRE(name_row3 == "Charlie");
        }
    }
    catch (const Poco::Data::SQLite::SQLiteException& sqle)
    {
        FAIL("SQLite Error: " << sqle.what());
    }
    catch (const std::exception& e)
    {
        FAIL("General SQL Error: " << e.what());
    }

    Poco::Data::SQLite::Connector::unregisterConnector();
}
//-------------------------------------------------------------------