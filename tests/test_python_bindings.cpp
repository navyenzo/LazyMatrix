//-------------------------------------------------------------------
/**
 * @file test_python_bindings.cpp
 * @brief Test case for Python bindings of the LazyMatrix library.
 *
 * This test case is designed to verify the Python bindings for the LazyMatrix library,
 * specifically for the SimpleMatrix<double> class. It embeds the Python interpreter using
 * pybind11, exposes the SimpleMatrix<double> class to Python, and runs a Python script
 * to create an instance of this class and print its contents. The test is useful for
 * ensuring that the Python bindings are correctly set up and functional.
 *
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 * @see https://github.com/navyenzo/LazyMatrix.git
 * @namespace LazyMatrix
 */
//-------------------------------------------------------------------



//-------------------------------------------------------------------
#include <catch2/catch_all.hpp>
#include <pybind11/embed.h>
#include "lazy_matrix.hpp"
#include <iostream>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace py = pybind11;
namespace lm = LazyMatrix;
//-------------------------------------------------------------------



//-------------------------------------------------------------------
TEST_CASE("Python Script Reads SimpleMatrix<double>", "[python_embedding]")
{
    std::cout << "[Test] Starting Python interpreter." << std::endl;
    py::scoped_interpreter guard{};  // Start the Python interpreter

    std::cout << "[Test] Creating a SimpleMatrix<double> instance in C++." << std::endl;
    lm::SimpleMatrix<double> my_matrix(3, 3, 5.0);  // Create and initialize the matrix

    SECTION("Run Python Script") {
        // Attempt to import the Python module
        try {
            std::cout << "[Test] Importing Python module 'lazymatrix_test'." << std::endl;
            py::module_ lazymatrix_module = py::module_::import("lazymatrix_test");
            std::cout << "[Test] Module 'lazymatrix_test' imported successfully." << std::endl;

            // Expose SimpleMatrix<double> to Python
            std::cout << "[Test] Binding SimpleMatrix<double> to Python." << std::endl;
            lm::bind_matrix<lm::SimpleMatrix<double>>(lazymatrix_module, "SimpleMatrixDouble");
            std::cout << "[Test] Binding completed." << std::endl;

            // Run the embedded Python script
            std::cout << "[Test] Running embedded Python script." << std::endl;
            py::exec(R"(
                import lazymatrix_test as lm
                mat = lm.SimpleMatrixDouble(3, 3, 5.0)
                print("Matrix contents from Python:")
                for i in range(3):
                    for j in range(3):
                        print(mat.at(i, j), end=' ')
                    print()
            )");
            std::cout << "[Test] Python script executed successfully." << std::endl;
        } catch (const py::error_already_set& e) {
            std::cerr << "[Error] Python error: " << e.what() << std::endl;
            throw;
        }
    }
}
//-------------------------------------------------------------------