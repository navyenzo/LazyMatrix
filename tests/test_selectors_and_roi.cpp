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
    auto matrix = LazyMatrix::generate_iota_matrix(3, 3, 0, 1);

    int64_t row_index = 0;
    int64_t column_index = 1;
    std::vector<int64_t> selected_vectors = {0, 2};

    SECTION("single row and column selection test")
    {
        auto selected_row = LazyMatrix::row(matrix, row_index);
        auto selected_column = LazyMatrix::column(matrix, column_index);

        // Testing single row selection
        for(int i = 0; i < matrix.columns(); ++i)
        {
            REQUIRE(matrix(row_index, i) == selected_row(0, i));
        }

        // Testing single column selection
        for(int i = 0; i < matrix.rows(); ++i)
        {
            REQUIRE(matrix(i, column_index) == selected_column(i,0));
        }
    }

    SECTION("multiple rows and column selection test")
    {
        auto selected_rows = LazyMatrix::rows(matrix, selected_vectors);
        auto selected_columns = LazyMatrix::columns(matrix, selected_vectors);

        // Testing multiple row selection
        for(int i = 0; i < selected_vectors.size(); ++i)
        {
            for(int j = 0; j < matrix.columns(); ++j)
            {
                REQUIRE(matrix(selected_vectors[i], j) == selected_rows(i, j));
            }
        }

        // Testing multiple column selection
        for(int i = 0; i < selected_vectors.size(); ++i)
        {
            for(int j = 0; j < matrix.rows(); ++j)
            {
                REQUIRE(matrix(j, selected_vectors[i]) == selected_columns(j, i));
            }
        }
    }

    SECTION("selecting multiple rows and columns at the same time test")
    {
        auto selected_rows_and_columns = LazyMatrix::rows_and_columns(matrix, selected_vectors, selected_vectors);

        for(int i = 0; i < selected_vectors.size(); ++i)
        {
            for(int j = 0; j < selected_vectors.size(); ++j)
            {
                REQUIRE(matrix(selected_vectors[i], selected_vectors[j]) == selected_rows_and_columns(i,j));
            }
        }
    }

    SECTION("Region Of Interest (ROI) selection test")
    {
        auto roi_matrix = LazyMatrix::roi(matrix, 1, 1, 2, 2);

        REQUIRE(matrix(1,1) == roi_matrix(0,0));
        REQUIRE(matrix(1,2) == roi_matrix(0,1));
        REQUIRE(matrix(2,1) == roi_matrix(1,0));
        REQUIRE(matrix(2,2) == roi_matrix(1,1));
    }
}
//-------------------------------------------------------------------