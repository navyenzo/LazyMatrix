//-------------------------------------------------------------------
/**
 * @file test_polymorphic_wrappers.cpp
 * @brief Tests for polymorphic wrappers in the LazyMatrix library.
 *
 * This file includes tests for the polymorphic wrappers, ensuring that
 * they correctly abstract and manage various matrix operations and
 * types consistently and effectively.
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
TEST_CASE("Wrapping a matrix with polymorphic wrapper with non-const access", "[PolymorphicWrapper]")
{
    // Create a matrix
    auto mat = LazyMatrix::MatrixFactory::create_simple_matrix<int>(2,2,0);

    // Populate the matrix
    mat(0,0) = 1; mat(0,1) = 2;
    mat(1,0) = 3; mat(1,1) = 4;

    // Wrap the matrix with a polymorphic wrapper
    auto wrapper = LazyMatrix::wrap_matrix(mat);

    // Test wrapped matrix
    REQUIRE(wrapper->rows() == mat.rows()); // Rows
    REQUIRE(wrapper->columns() == mat.columns()); // Columns
    REQUIRE(wrapper->size() == mat.size()); // Size

    // Test const access
    REQUIRE(wrapper->at(0,0) == mat(0,0));
    REQUIRE(wrapper->at(0,1) == mat(0,1));
    REQUIRE(wrapper->at(1,0) == mat(1,0));
    REQUIRE(wrapper->at(1,1) == mat(1,1));
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
TEST_CASE("Wrapping a 3d matrix with polymorphic wrapper with non-const access", "[PolymorphicWrapper3D]")
{
    // Create a matrix
    auto mat = LazyMatrix::MatrixFactory::create_simple_matrix3d<int>(2,2,2,0);

    // Populate the matrix
    mat(0,0,0) = 1; mat(0,0,1) = 2;
    mat(0,1,0) = 3; mat(0,1,1) = 4;
    mat(1,0,0) = 5; mat(1,0,1) = 6;
    mat(1,1,0) = 7; mat(1,1,1) = 8;

    // Wrap the matrix with a polymorphic wrapper
    auto wrapper = LazyMatrix::wrap_matrix3d(mat);

    // Test wrapped matrix
    REQUIRE(wrapper->pages() == mat.pages()); // Rows
    REQUIRE(wrapper->rows() == mat.rows()); // Rows
    REQUIRE(wrapper->columns() == mat.columns()); // Columns
    REQUIRE(wrapper->size() == mat.size()); // Size

    // Test const access
    REQUIRE(wrapper->at(0,0,0) == mat(0,0,0));
    REQUIRE(wrapper->at(0,0,1) == mat(0,0,1));
    REQUIRE(wrapper->at(0,1,0) == mat(0,1,0));
    REQUIRE(wrapper->at(0,1,1) == mat(0,1,1));
    REQUIRE(wrapper->at(1,0,0) == mat(1,0,0));
    REQUIRE(wrapper->at(1,0,1) == mat(1,0,1));
    REQUIRE(wrapper->at(1,1,0) == mat(1,1,0));
    REQUIRE(wrapper->at(1,1,1) == mat(1,1,1));
}
//-------------------------------------------------------------------
