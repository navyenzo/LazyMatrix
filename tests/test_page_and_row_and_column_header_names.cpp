//-------------------------------------------------------------------
/**
 * @file test_page_and_row_and_column_header_names.cpp
 * @brief Test cases for handling page, row and column header names for 2d and 3d-matrix-like data.
 *
 * This file contains various test cases for handling the header names
 * of pages, rows and columns of 2d-matrix-like and 3d-matrix-like data.
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
 * @brief Test case for 2d-matrix-like data row and column header names
 *
 * Tests applying header names for rows and columns to 2d-matrix-like
 * data and retrieving those names as well as trying to retrieve names
 * for rows and columns that have not been named (should return default names).
 */
//-------------------------------------------------------------------
TEST_CASE("2D-Matrix-Like row and column header naming", "[RowAndColumnNames]")
{
    // Create 2d matrices to make sure they all work
    auto m1 = LazyMatrix::MatrixFactory::create_matrix<int>(3,3,0);
    auto m2 = LazyMatrix::MatrixFactory::create_image_matrix<int>(3,3,0);
    auto m3 = LazyMatrix::MatrixFactory::create_simple_matrix<int>(3,3,0);

    std::string test_row_header_name = "hello row 1";
    std::string test_column_header_name = "hello column 1";

    // Try naming one of the rows and one of the columns
    m1.set_row_header(1, test_row_header_name);
    m1.set_column_header(1, test_column_header_name);
    m2.set_row_header(1, test_row_header_name);
    m2.set_column_header(1, test_column_header_name);
    m3.set_row_header(1, test_row_header_name);
    m3.set_column_header(1, test_column_header_name);

    // Test retrieving the header names we set
    REQUIRE(m1.get_row_header(1) == test_row_header_name);
    REQUIRE(m1.get_column_header(1) == test_column_header_name);
    REQUIRE(m2.get_row_header(1) == test_row_header_name);
    REQUIRE(m2.get_column_header(1) == test_column_header_name);
    REQUIRE(m3.get_row_header(1) == test_row_header_name);
    REQUIRE(m3.get_column_header(1) == test_column_header_name);
}
//-------------------------------------------------------------------