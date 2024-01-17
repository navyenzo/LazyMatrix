//-------------------------------------------------------------------
/**
 * @file test_database_matrix.cpp
 * @brief Test cases for DatabaseMatrix class using Catch2 and SQLite.
 *
 * This test suite is designed to validate the functionality of the DatabaseMatrix class,
 * particularly its ability to interact with a SQLite database and perform basic operations
 * such as fetching the number of rows and columns and reading data using operator()(i, j).
 * 
 * 
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



//-------------------------------------------------------------------
#include <iostream>
#include <catch2/catch_all.hpp>

#include "lazy_matrix.hpp"

// Includes used to test DatabaseMatrix
#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Data/SQLite/SQLiteException.h>
#include <Poco/Data/Session.h>
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
        
        LazyMatrix::SafeName safe_table_name("test_table");
        LazyMatrix::DatabaseMatrix matrix(session, safe_table_name);

        SECTION("Test row count and column count")
        {
            REQUIRE(matrix.rows() == 3);
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
        
        SECTION("Test with condition")
        {
            matrix.set_condition("name LIKE 'B%'");
            
            REQUIRE(matrix.rows() == 1); // Expecting only one row where name starts with 'B'
            
            auto name = matrix(0, 1).convert<std::string>();
            REQUIRE(name == "Bob"); // Checking if the correct row is fetched
        }
        
        SECTION("Test with different condition")
        {
            matrix.set_condition("id > 1");
            
            REQUIRE(matrix.rows() == 2); // Expecting two rows with id > 1
            
            auto name_row1 = matrix(0, 1).convert<std::string>();
            REQUIRE(name_row1 == "Bob");

            auto name_row2 = matrix(1, 1).convert<std::string>();
            REQUIRE(name_row2 == "Charlie");
        }
        
        SECTION("Test with row sorting")
        {
            LazyMatrix::SafeRowSortingMethod safe_row_sorting_method("id", "DESC");
            matrix.set_row_sorting_method(safe_row_sorting_method);

            auto name_row1 = matrix(0, 1).convert<std::string>();
            REQUIRE(name_row1 == "Charlie"); // First row should now be Charlie

            auto name_row2 = matrix(1, 1).convert<std::string>();
            REQUIRE(name_row2 == "Bob"); // Second row should now be Bob

            auto name_row3 = matrix(2, 1).convert<std::string>();
            REQUIRE(name_row3 == "Alice"); // Third row should now be Alice
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