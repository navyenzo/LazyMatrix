//-------------------------------------------------------------------
/**
 * @file test_augmentation.cpp
 * @brief Test cases for augmenting matrices together either by rows or columns.
 * 
 * This file contains various test cases for when two matrices are
 * augmented together, either by augmenting rows (one on top of each other),
 * or by augmenting columns (one on the side of each other).
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
 * @brief Test case for augmenting two matrices by columns
 */
//-------------------------------------------------------------------
TEST_CASE("Augmenting matrices by columns and testing row and column header naming", "[AugmentColumnsView]")
{
    // Create 2d matrices to make sure they all work
    auto m1 = LazyMatrix::MatrixFactory::create_simple_matrix<double>(3,3,-1.1);
    auto m2 = LazyMatrix::MatrixFactory::create_simple_matrix<double>(3,3,5.5);

    // Augment the two matrices by columns
    auto augmented_matrix = LazyMatrix::augment_by_columns_view(m1,m2);

    // Test that the augmentation worked
    REQUIRE(augmented_matrix.rows() == std::max(m1.rows(), m2.rows()));
    REQUIRE(augmented_matrix.columns() == m1.columns() + m2.columns());
    REQUIRE(augmented_matrix(0,0) == -1.1);
    REQUIRE(augmented_matrix(0,4) == 5.5);
    
    std::string test_header_name = "hello my friend";

    // Try naming one two of the columns, one from the
    // left side expression and another from the right side
    augmented_matrix.set_column_header(1, test_header_name); //  Should rename second column of first matrix.
    augmented_matrix.set_column_header(4, test_header_name); // Should rename second column of second matrix.

    // Test retrieving the header names we set
    REQUIRE(m1.get_column_header(1) == test_header_name);
    REQUIRE(m2.get_column_header(1) == test_header_name);
    REQUIRE(augmented_matrix.get_column_header(1) == test_header_name);
    REQUIRE(augmented_matrix.get_column_header(4) == test_header_name);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test case for augmenting two matrices by rows
 */
//-------------------------------------------------------------------
TEST_CASE("Augmenting matrices by rows and testing row and column header naming", "[AugmentRowsView]")
{
    // Create 2d matrices to make sure they all work
    auto m1 = LazyMatrix::MatrixFactory::create_simple_matrix<double>(3,3,-1.1);
    auto m2 = LazyMatrix::MatrixFactory::create_simple_matrix<double>(3,3,5.5);

    // Augment the two matrices by columns
    auto augmented_matrix = LazyMatrix::augment_by_rows_view(m1,m2);

    // Test that the augmentation worked
    REQUIRE(augmented_matrix.rows() == m1.rows() + m2.rows());
    REQUIRE(augmented_matrix.columns() == std::max(m1.columns(), m2.columns()));
    REQUIRE(augmented_matrix(0,0) == -1.1);
    REQUIRE(augmented_matrix(4,0) == 5.5);
    
    std::string test_header_name = "hello my friend";

    // Try naming one two of the columns, one from the
    // left side expression and another from the right side
    augmented_matrix.set_row_header(1, test_header_name); //  Should rename second column of first matrix.
    augmented_matrix.set_row_header(4, test_header_name); // Should rename second column of second matrix.

    // Test retrieving the header names we set
    REQUIRE(m1.get_row_header(1) == test_header_name);
    REQUIRE(m2.get_row_header(1) == test_header_name);
    REQUIRE(augmented_matrix.get_row_header(1) == test_header_name);
    REQUIRE(augmented_matrix.get_row_header(4) == test_header_name);
}
//-------------------------------------------------------------------