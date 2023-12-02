#include <iostream>
#include <catch2/catch_all.hpp>
#include "lazy_matrix.hpp"



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

    // Text const access
    REQUIRE(wrapper->at(0,0) == mat(0,0));
    REQUIRE(wrapper->at(0,1) == mat(0,1));
    REQUIRE(wrapper->at(1,0) == mat(1,0));
    REQUIRE(wrapper->at(1,1) == mat(1,1));
}
