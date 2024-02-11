//-------------------------------------------------------------------
/**
 * @file test_string_to_number_converions.cpp
 * @brief Test the string conversion function for various cases and failures.
 * 
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



//-------------------------------------------------------------------
#include <catch2/catch_all.hpp>
#include "lazy_matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test case for CSVMatrix with double data type.
 *
 * Tests loading of CSV data into a matrix of doubles and verifies
 * correct data retrieval.
 */
//-------------------------------------------------------------------
TEST_CASE("interpret_string function tests", "[interpret_string]")
{
    SECTION("Interprets numbers correctly")
    {
        auto [result,type] = LazyMatrix::interpret_string("42");
        REQUIRE(type == LazyMatrix::ResultType::Number);
        REQUIRE(result.convert<double>() == 42);
    }

    SECTION("Interprets date/time correctly")
    {
        auto [result,type] = LazyMatrix::interpret_string("2022-01-01T12:00:00");
        REQUIRE(type == LazyMatrix::ResultType::DateTime);
    }

    SECTION("Interprets vectors correctly")
    {
        auto [result,type] = LazyMatrix::interpret_string("{1,2,3,4,5}");
        REQUIRE(type == LazyMatrix::ResultType::Vector);
        auto vec = result.extract<std::vector<double>>();
        REQUIRE(vec.size() == 5);
        REQUIRE(vec[0] == 1);
        REQUIRE(vec[4] == 5);
    }

    SECTION("Interprets vectors correctly with special cases")
    {
        auto [result,type] = LazyMatrix::interpret_string("{,1,2,3,4,,5}");
        REQUIRE(type == LazyMatrix::ResultType::Vector);
        auto vec = result.extract<std::vector<double>>();
        REQUIRE(vec.size() == 7);
        REQUIRE(vec[0] == 0);
        REQUIRE(vec[1] == 1);
        REQUIRE(vec[2] == 2);
        REQUIRE(vec[3] == 3);
        REQUIRE(vec[4] == 4);
        REQUIRE(vec[5] == 0);
        REQUIRE(vec[6] == 5);
    }

    SECTION("Interprets plain strings correctly")
    {
        auto [result,type] = LazyMatrix::interpret_string("Hello, World!");
        REQUIRE(type == LazyMatrix::ResultType::String);
        REQUIRE(result.convert<std::string>() == "Hello, World!");
    }

    // Additional tests can be added here to cover more cases, such as specific expression evaluations,
    // handling of invalid date formats, etc.
}
//-------------------------------------------------------------------