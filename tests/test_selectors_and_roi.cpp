//-------------------------------------------------------------------
/**
 * @file test_selectors_and_roi.cpp
 * @brief Tests for rows/columns selectors and Region Of Interest (ROI)
 *
 * This file includes tests for the selector views and ROIs, which are used to
 * select multiple rows or columns or a Region Of Interest from an input
 * matrix expression.
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
//-------------------------------------------------------------------



//-------------------------------------------------------------------
TEST_CASE("Rows and Columns Selection and Region Of Interest (ROI)", "[Rows_and_Columns_Selection_and_ROIs]")
{
    // Create a small matrix for testing with incrementing values
    auto matrix = LazyMatrix::generate_iota_matrix(10, 10, 0, 1);

    std::cout << "matrix =\n" << matrix << std::endl;

    auto selected_row = LazyMatrix::row(matrix, 0);
    auto selected_column = LazyMatrix::column(matrix, 0);
    auto selected_rows = LazyMatrix::rows(matrix, {0, 1, 2});
    auto selected_columns = LazyMatrix::columns(matrix, {0, 1, 2});

    std::cout << "selected_row =\n" << selected_row << std::endl;
    std::cout << "selected_column =\n" << selected_column << std::endl;
    std::cout << "selected_rows =\n" << selected_rows << std::endl;
    std::cout << "selected_columns =\n" << selected_columns << std::endl;

    // // Define padded size
    // const uintptr_t padded_rows = 4;
    // const uintptr_t padded_columns = 4;

    // SECTION("padded_matrix Test")
    // {
    //     // Apply padding
    //     auto padded_matrix = LazyMatrix::create_padded_matrix_view(small_matrix, padded_rows, padded_columns);

    //     // Check dimensions
    //     REQUIRE(padded_matrix.rows() == padded_rows);
    //     REQUIRE(padded_matrix.columns() == padded_columns);

    //     // Check original values
    //     REQUIRE(padded_matrix(0, 0) == 1);
    //     REQUIRE(padded_matrix(1, 1) == 4);

    //     // Check padding values
    //     REQUIRE(padded_matrix(2, 2) == 0);
    //     REQUIRE(padded_matrix(3, 3) == 0);
    // }
}
//-------------------------------------------------------------------