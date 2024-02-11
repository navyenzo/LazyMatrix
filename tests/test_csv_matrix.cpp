//-------------------------------------------------------------------
/**
 * @file test_csv_matrix.cpp
 * @brief Test cases for CSVMatrix class using Catch2 and Doxygen comments.
 *
 * This file contains various test cases for the CSVMatrix class, ensuring proper functionality
 * for different data types and scenarios, including loading of CSV files, handling of data types
 * like double and std::string, and error handling.
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
// Utility function to create a CSV file for testing
void createTestCSVFile(const std::string& filename, const std::string& content)
{
    std::ofstream file(filename);
    file << content;
    file.close();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test case for CSVMatrix with double data type.
 *
 * Tests loading of CSV data into a matrix of doubles and verifies
 * correct data retrieval.
 */
//-------------------------------------------------------------------
TEST_CASE("CSVMatrix with double", "[CSVMatrix]")
{
    std::string filename = "test_double.csv";
    createTestCSVFile(filename, "1.1,2.2,3.3\n4.4,5.5,6.6");

    LazyMatrix::CSVMatrix<double> matrix;
    matrix.load(filename, false, false);


    std::cout << "matrix =\n" << matrix << "\n\n\n";

    REQUIRE(matrix(0, 0) == Catch::Approx(1.1));
    REQUIRE(matrix(0, 1) == Catch::Approx(2.2));
    REQUIRE(matrix(1, 2) == Catch::Approx(6.6));

    std::remove(filename.c_str());
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test case for CSVMatrix with std::string data type.
 *
 * Tests loading of CSV data into a matrix of std::string and verifies
 * correct data retrieval, including handling of headers and row names.
 */
//-------------------------------------------------------------------
TEST_CASE("CSVMatrix with std::string", "[CSVMatrix]")
{
    std::string filename = "test_string.csv";
    createTestCSVFile(filename, "header1,header2\nrow1,data1\nrow2,data2");

    LazyMatrix::CSVMatrix<std::string> matrix;
    matrix.load(filename, true, true);

    REQUIRE(matrix(0, 0) == "data1");
    REQUIRE(matrix(1, 0) == "data2");
    REQUIRE(matrix.get_column_header(0) == "header2");
    REQUIRE(matrix.get_row_header(0) == "row1");

    std::remove(filename.c_str());
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test case for initial state of CSVMatrix and load function.
 *
 * Tests the initial state of the CSVMatrix object and its behavior when
 * loading a CSV file, including error handling for failed load operations.
 */
//-------------------------------------------------------------------
TEST_CASE("CSVMatrix initial state and load function", "[CSVMatrix]")
{
    LazyMatrix::CSVMatrix<double> matrix;

    // Initial state checks
    REQUIRE(matrix.rows() == 0);
    REQUIRE(matrix.columns() == 0);

    // Load function success test
    std::string validFilename = "valid_test.csv";
    createTestCSVFile(validFilename, "1.0,2.0\n3.0,4.0");
    std::error_code loadError = matrix.load(validFilename, false, false);
    REQUIRE_FALSE(loadError); // No error should be present
    REQUIRE(matrix.rows() > 0);
    REQUIRE(matrix.columns() > 0);
    std::remove(validFilename.c_str());

    // Load function failure test
    std::string invalidFilename = "non_existent.csv";
    loadError = matrix.load(invalidFilename, false, false);
    REQUIRE(loadError); // Error should be present
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test case for circular access with circ_at(i, j).
 *
 * Tests circular access of CSVMatrix using circ_at(i, j) method with
 * negative indices and indices greater than the matrix dimensions.
 */
//-------------------------------------------------------------------
TEST_CASE("CSVMatrix circular access with circ_at(i, j)", "[CSVMatrix]") {
    std::string filename = "test_circular.csv";
    createTestCSVFile(filename, "1,2,3\n4,5,6\n7,8,9");

    LazyMatrix::CSVMatrix<int> matrix;
    matrix.load(filename, false, false);

    // Test circular access

    REQUIRE(matrix.circ_at(-1, -1) == 9);
    REQUIRE(matrix.circ_at(3, 3) == 1); 
    REQUIRE(matrix.circ_at(4, 2) == 6);

    std::remove(filename.c_str());
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test case for 1D circular access with circ_at(index).
 *
 * Tests circular access of CSVMatrix treating it as a 1D array using
 * circ_at(index) method with negative indices and indices greater than
 * the total number of elements.
 */
//-------------------------------------------------------------------
TEST_CASE("CSVMatrix 1D circular access with circ_at(index)", "[CSVMatrix]") {
    std::string filename = "test_1d_circular.csv";
    createTestCSVFile(filename, "a,b,c\nd,e,f\ng,h,i");

    LazyMatrix::CSVMatrix<std::string> matrix;
    matrix.load(filename, false, false);

    // Test 1D circular access
    REQUIRE(matrix.circ_at(-1) == "i");
    REQUIRE(matrix.circ_at(9) == "a");
    REQUIRE(matrix.circ_at(11) == "c");

    std::remove(filename.c_str());
}
//-------------------------------------------------------------------