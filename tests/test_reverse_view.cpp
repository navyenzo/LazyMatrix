//-------------------------------------------------------------------
/**
 * @file test_reverse_view.cpp
 * @brief Test cases for reverse views of 2d-matrix-like data.
 * 
 * This file contains various test cases for creating reverse views
 * of 2d-matrix-like data. It tests reversing rows, reversing columns
 * as well as reversing both rows and columns.
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
 * @brief Test case for reversing rows of a matrix
 */
//-------------------------------------------------------------------
TEST_CASE("Reverse Rows of a Matrix", "[ReverseView]")
{
    // Create 2d matrices to make sure they all work
    auto matrix = LazyMatrix::generate_iota_matrix<int>(3,3,0,1);
    auto reverse_matrix = LazyMatrix::create_reversed_matrix_view(matrix, true, false);

    // Test that rows were successfully reversed
    REQUIRE(matrix(0,0) == reverse_matrix(2,0));
    REQUIRE(matrix(1,0) == reverse_matrix(1,0));
    REQUIRE(matrix(2,0) == reverse_matrix(0,0));
    REQUIRE(matrix(0,1) == reverse_matrix(2,1));
    REQUIRE(matrix(1,1) == reverse_matrix(1,1));
    REQUIRE(matrix(2,1) == reverse_matrix(0,1));
    REQUIRE(matrix(0,2) == reverse_matrix(2,2));
    REQUIRE(matrix(1,2) == reverse_matrix(1,2));
    REQUIRE(matrix(2,2) == reverse_matrix(0,2));
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test case for reversing columns of a matrix
 */
//-------------------------------------------------------------------
TEST_CASE("Reverse Columns of a Matrix", "[ReverseView]")
{
    // Create 2d matrices to make sure they all work
    auto matrix = LazyMatrix::generate_iota_matrix<int>(3,3,0,1);
    auto reverse_matrix = LazyMatrix::create_reversed_matrix_view(matrix, false, true);

    // Test that rows were successfully reversed
    REQUIRE(matrix(0,0) == reverse_matrix(0,2));
    REQUIRE(matrix(0,1) == reverse_matrix(0,1));
    REQUIRE(matrix(0,2) == reverse_matrix(0,0));
    REQUIRE(matrix(1,0) == reverse_matrix(1,2));
    REQUIRE(matrix(1,1) == reverse_matrix(1,1));
    REQUIRE(matrix(1,2) == reverse_matrix(1,0));
    REQUIRE(matrix(2,0) == reverse_matrix(2,2));
    REQUIRE(matrix(2,1) == reverse_matrix(2,1));
    REQUIRE(matrix(2,2) == reverse_matrix(2,0));
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test case for reversing both rows and columns of a matrix
 */
//-------------------------------------------------------------------
TEST_CASE("Reverse both Rows and Columns of a Matrix", "[ReverseView]")
{
    // Create 2d matrices to make sure they all work
    auto matrix = LazyMatrix::generate_iota_matrix<int>(3,3,0,1);
    auto reverse_matrix = LazyMatrix::create_reversed_matrix_view(matrix, true, true);

    // Test that rows were successfully reversed
    REQUIRE(matrix(0,0) == reverse_matrix(2,2));
    REQUIRE(matrix(1,1) == reverse_matrix(1,1));
    REQUIRE(matrix(2,2) == reverse_matrix(0,0));
    REQUIRE(matrix(0,0) == reverse_matrix(2,2));
    REQUIRE(matrix(1,1) == reverse_matrix(1,1));
    REQUIRE(matrix(2,2) == reverse_matrix(0,0));
    REQUIRE(matrix(0,0) == reverse_matrix(2,2));
    REQUIRE(matrix(1,1) == reverse_matrix(1,1));
    REQUIRE(matrix(2,2) == reverse_matrix(0,0));
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test case for row and colum header naming of reverse views
 */
//-------------------------------------------------------------------
TEST_CASE("Test row and column header naming for reverse view", "[ReverseView]")
{    
    // Create 2d matrices to make sure they all work
    auto matrix = LazyMatrix::generate_iota_matrix<int>(3,3,0,1);
    auto reversed_rows = LazyMatrix::create_reversed_matrix_view(matrix, true, false);
    auto reversed_columns = LazyMatrix::create_reversed_matrix_view(matrix, false, true);
    auto reversed_rows_and_columns = LazyMatrix::create_reversed_matrix_view(matrix, true, true);
    
    std::string renamed_row_name = "hello my row friend";
    std::string renamed_column_name = "hello my column friend";

    // Name a row and a column
    matrix.set_row_header(0, renamed_row_name);
    matrix.set_column_header(0, renamed_column_name);

    // Now let's make sure that the renamed row and column
    // show up as the correctly indexed row and column for
    // the reversed matrices
    REQUIRE(matrix.get_row_header(0) == renamed_row_name);
    REQUIRE(matrix.get_column_header(0) == renamed_column_name);
    REQUIRE(reversed_rows.get_row_header(reversed_rows.rows() - 1) == renamed_row_name);
    REQUIRE(reversed_columns.get_column_header(reversed_rows.columns() - 1) == renamed_column_name);
    REQUIRE(reversed_rows_and_columns.get_row_header(reversed_rows_and_columns.rows() - 1) == renamed_row_name);
    REQUIRE(reversed_rows_and_columns.get_column_header(reversed_rows_and_columns.columns() - 1) == renamed_column_name);
}
//-------------------------------------------------------------------