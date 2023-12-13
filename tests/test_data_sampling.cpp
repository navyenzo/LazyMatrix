#include <catch2/catch_all.hpp>
#include "lazy_matrix.hpp"



TEST_CASE("Matrix Simple Data Sampling", "[MatrixSampling]")
{
    // Create a source matrix with sequential values
    int rows = 1000, columns = 5;
    LazyMatrix::Matrix<int> source(rows, columns, 0);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            source(i, j) = i * columns + j; // Fill with sequential values
        }
    }

    // Case 1: Sample rows
    LazyMatrix::Matrix<int> destRowSampled(100, columns, 0);
    simple_data_sampling(source, destRowSampled, 0, 999, true);

    // Verify row sampling
    int rowInterval = rows / destRowSampled.rows();
    for (int i = 0; i < destRowSampled.rows(); ++i) {
        int sourceRowIndex = i * rowInterval;
        for (int j = 0; j < destRowSampled.columns(); ++j) {
            REQUIRE(destRowSampled(i, j) == source(sourceRowIndex, j));
        }
    }

    // Case 2: Sample columns
    LazyMatrix::Matrix<int> destcolumnsampled(rows, 3, 0);
    simple_data_sampling(source, destcolumnsampled, 0, columns - 1, false);

    // Verify column sampling
    int colInterval = columns / destcolumnsampled.columns();
    for (int j = 0; j < destcolumnsampled.columns(); ++j) {
        int sourceColIndex = j * colInterval;
        for (int i = 0; i < destcolumnsampled.rows(); ++i) {
            REQUIRE(destcolumnsampled(i, j) == source(i, sourceColIndex));
        }
    }
}
