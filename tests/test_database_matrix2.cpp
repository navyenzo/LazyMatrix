//-------------------------------------------------------------------
/**
 * @file test_database_matrix2.cpp
 * @brief Test cases for DatabaseMatrix class using Catch2 and SQLite.
 *
 * This test suite is designed to validate the functionality of the DatabaseMatrix2 class,
 * particularly its ability to interact with a SQLite database and perform basic operations
 * such as fetching the number of rows and columns and reading data using operator()(i, j).
 * 
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 * @see https://github.com/navyenzo/LazyMatrix.git
 */
//-------------------------------------------------------------------



//-------------------------------------------------------------------
#include <iostream>
#include <catch2/catch_all.hpp>

#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>

#include "lazy_matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
TEST_CASE("DatabaseMatrix2 operations", "[DatabaseMatrix2]")
{
    // Register the SQLite3 connector
    soci::register_factory_sqlite3();

    try
    {
        // Set up a database connection using SOCI
        soci::session session(soci::sqlite3, "your_database.db");

        // Execute the SQL commands to create a table and insert data
        session << "DROP TABLE IF EXISTS test_table";
        session << "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT)";
        session << "INSERT INTO test_table (name) VALUES('Alice')";
        session << "INSERT INTO test_table (name) VALUES('Bob')";
        session << "INSERT INTO test_table (name) VALUES('Charlie')";

        LazyMatrix::DatabaseMatrix2 matrix(session, "test_table");

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
            std::string name_row1;
            if(auto str_ptr = std::get_if<std::string>(&matrix(0,1)))
            {
                name_row1 = *str_ptr;
            }
            REQUIRE(name_row1 == "Alice");

            // Check second row
            std::string name_row2;
            if(auto str_ptr = std::get_if<std::string>(&matrix(1,1)))
            {
                name_row2 = *str_ptr;
            }
            REQUIRE(name_row2 == "Bob");

            // Check third row
            std::string name_row3;
            if(auto str_ptr = std::get_if<std::string>(&matrix(1,1)))
            {
                name_row3 = *str_ptr;
            }
            REQUIRE(name_row3 == "Charlie");
        }
    }
    catch (const soci::soci_error& e)
    {
        FAIL("SOCI SQLite Error: " << e.what());
    }
    catch (const std::exception& e)
    {
        FAIL("General SQL Error: " << e.what());
    }
}
//-------------------------------------------------------------------