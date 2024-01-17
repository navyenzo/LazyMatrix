//-------------------------------------------------------------------
#include <iostream>
#include <catch2/catch_all.hpp>
#include "lazy_matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
TEST_CASE("Wrapping a matrix with polymorphic wrapper with non-const access", "[PolymorphicWrapper]")
{
    // Create a matrix
    LazyMatrix::Matrix<int> mat(2, 2);

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
    LazyMatrix::SimpleMatrix3D<int> mat(2, 2, 2);

    // Populate the matrix
    mat(0,0,0) = 1; mat(0,0,1) = 2;
    mat(0,1,0) = 3; mat(0,1,1) = 4;
    mat(1,0,0) = 5; mat(1,0,1) = 6;
    mat(1,1,0) = 7; mat(1,1,1) = 8;

    std::cout << "\n\n\n3d matrix =\n" << mat << "\n\n\n";

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