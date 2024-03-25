//-------------------------------------------------------------------
/**
 * @file test_roi.cpp
 * @brief Test cases for Region Of Interest views of 2d-matrix-like data.
 * 
 * This file contains various test cases for applying a Region Of Interest (ROI)
 * to a 2d-matrix-like data.
 * 
 * The ROI is specified by supplying (r1,c1) and (r2,c2) coordinates of a
 * matrix which stand for row1,column1 and row2,column2 indeces.
 * 
 * The indeces are inclusive and they are circular, which means they can
 * be negative also.
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
 * @brief Test case for applying a ROI to a 2d-matrix-like data.
 */
//-------------------------------------------------------------------
TEST_CASE("ROI matrix test with circular indexing", "[ROIView]")
{
    // Create 2d matrices to make sure they all work
    auto matrix = LazyMatrix::generate_iota_matrix(5, 5, 0, 1);

    // Apply a ROI to the matrix
    auto roi_view1 = LazyMatrix::roi(matrix, 1, 1, 3, 3);

    // Apply a ROI in reverse order
    auto roi_view2 = LazyMatrix::roi(matrix, 3, 3, 1, 1);

    // Apply a ROI using circular indexing and generating a bigger
    // matrix than the original one
    auto roi_view3 = LazyMatrix::roi(matrix, 0, 0, 2*matrix.rows() - 1, 2*matrix.columns() - 1);
    
    // Let's check ROIs are correctly applied

    // roi_view1
    REQUIRE(roi_view1.rows() == 3);
    REQUIRE(roi_view1.columns() == 3);
    REQUIRE(matrix(1,1) == roi_view1(0,0));
    REQUIRE(matrix(1,2) == roi_view1(0,1));
    REQUIRE(matrix(1,3) == roi_view1(0,2));
    REQUIRE(matrix(2,1) == roi_view1(1,0));
    REQUIRE(matrix(2,2) == roi_view1(1,1));
    REQUIRE(matrix(2,3) == roi_view1(1,2));
    REQUIRE(matrix(3,1) == roi_view1(2,0));
    REQUIRE(matrix(3,2) == roi_view1(2,1));
    REQUIRE(matrix(3,3) == roi_view1(2,2));

    // roi_view2
    REQUIRE(roi_view2.rows() == 3);
    REQUIRE(roi_view2.columns() == 3);
    REQUIRE(matrix(1,1) == roi_view2(2,2));
    REQUIRE(matrix(1,2) == roi_view2(2,1));
    REQUIRE(matrix(1,3) == roi_view2(2,0));
    REQUIRE(matrix(2,1) == roi_view2(1,2));
    REQUIRE(matrix(2,2) == roi_view2(1,1));
    REQUIRE(matrix(2,3) == roi_view2(1,0));
    REQUIRE(matrix(3,1) == roi_view2(0,2));
    REQUIRE(matrix(3,2) == roi_view2(0,1));
    REQUIRE(matrix(3,3) == roi_view2(0,0));

    // roi_view3
    REQUIRE(roi_view3.rows() == matrix.rows() * 2);
    REQUIRE(roi_view3.columns() == matrix.columns() * 2);
    REQUIRE(matrix(0,0) == roi_view3(0,0));
    REQUIRE(matrix(0,1) == roi_view3(0,1));
    REQUIRE(matrix(0,2) == roi_view3(0,2));
    REQUIRE(matrix(0,3) == roi_view3(0,3));
    REQUIRE(matrix(0,4) == roi_view3(0,4));
    REQUIRE(matrix(1,0) == roi_view3(1,0));
    REQUIRE(matrix(1,1) == roi_view3(1,1));
    REQUIRE(matrix(1,2) == roi_view3(1,2));
    REQUIRE(matrix(1,3) == roi_view3(1,3));
    REQUIRE(matrix(1,4) == roi_view3(1,4));
    REQUIRE(matrix(2,0) == roi_view3(2,0));
    REQUIRE(matrix(2,1) == roi_view3(2,1));
    REQUIRE(matrix(2,2) == roi_view3(2,2));
    REQUIRE(matrix(2,3) == roi_view3(2,3));
    REQUIRE(matrix(2,4) == roi_view3(2,4));
    REQUIRE(matrix(3,0) == roi_view3(3,0));
    REQUIRE(matrix(3,1) == roi_view3(3,1));
    REQUIRE(matrix(3,2) == roi_view3(3,2));
    REQUIRE(matrix(3,3) == roi_view3(3,3));
    REQUIRE(matrix(3,4) == roi_view3(3,4));
    REQUIRE(matrix(4,0) == roi_view3(4,0));
    REQUIRE(matrix(4,1) == roi_view3(4,1));
    REQUIRE(matrix(4,2) == roi_view3(4,2));
    REQUIRE(matrix(4,3) == roi_view3(4,3));
    REQUIRE(matrix(4,4) == roi_view3(4,4));
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test case for applying a ROI to a 2d-matrix-like data.
 */
//-------------------------------------------------------------------
TEST_CASE("ROI view row and column renaming test", "[ROIView]")
{
    // Create 2d matrices to make sure they all work
    auto matrix = LazyMatrix::generate_iota_matrix(5, 5, 0, 1);

    // Apply a ROI to the matrix
    auto roi_view = LazyMatrix::roi(matrix, 1, 1, 3, 3);

    // Row and Column custom names
    std::string custom_row_name = "hello my row friend";
    std::string custom_column_name = "hello my column friend";

    // Set the name of a row and a column of the original matrix
    // Here we set row and column 2 of the original matrix, which
    // corresponds to row and column 1 of ROI view matrix
    matrix.set_row_header(2, custom_row_name);
    matrix.set_column_header(2, custom_column_name);

    REQUIRE(matrix.get_row_header(2) == roi_view.get_row_header(1));
    REQUIRE(matrix.get_column_header(2) == roi_view.get_column_header(1));
}
//-------------------------------------------------------------------