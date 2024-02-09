//-------------------------------------------------------------------
/**
 * \file test_python_bindings.cpp
 * \brief Test file for Python bindings of the LazyMatrix library.
 *
 * This file contains a test case for the Python bindings of the LazyMatrix library using pybind11.
 * It creates instances of various matrix types in C++, and then exposes these instances to Python.
 * Python code is executed to interact with these C++ objects, demonstrating the integration between C++ and Python.
 * The test also captures Python output and redirects it to C++ for validation. The Catch2 framework is used for structuring the test case.
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
#include <sstream>

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch_all.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>

#include <lazy_matrix.hpp>

namespace py = pybind11;
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Template function to bind the output operator
//-------------------------------------------------------------------
template<typename MatrixType>
void print_matrix(const MatrixType& matrix)
{
    std::cout << matrix << std::endl;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
PYBIND11_EMBEDDED_MODULE(lazymatrix, m)
{
    m.def("print_matrix_double", &print_matrix<LazyMatrix::SharedMatrixRef<LazyMatrix::Data<double>>>);
    m.def("print_matrix_float", &print_matrix<LazyMatrix::SharedMatrixRef<LazyMatrix::Data<float>>>);

    LazyMatrix::bind_factory_functions(m);
    LazyMatrix::bind_matrix_ref<LazyMatrix::SharedMatrixRef<LazyMatrix::Data<double>>>(m, "MatrixDouble");
    LazyMatrix::bind_matrix_ref<LazyMatrix::SharedMatrixRef<LazyMatrix::Data<float>>>(m, "MatrixFloat");
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
TEST_CASE("Python bindings for LazyMatrix", "[python_bindings]")
{
    // Initialize the Python interpreter.
    py::scoped_interpreter guard{};

    try
    {
        // Define the Python script to be executed
        std::string python_script =
            "import lazymatrix\n"
            "print('Creating matrices using factory functions:\\n')\n"
            "matrix1 = lazymatrix.MatrixFactory.create_matrix_double(3, 3)\n"
            "matrix1.set_circ_at(0,0,-2)\n"
            "matrix2 = lazymatrix.MatrixFactory.create_matrix_float(2, 4)\n"
            "print('Matrix 1 (double):')\n"
            "lazymatrix.print_matrix_double(matrix1)\n"
            "print('Matrix 2 (float):')\n"
            "lazymatrix.print_matrix_float(matrix2)\n";

        // Redirect Python's stdout and stderr to C++
        //LazyMatrix::PythonStdOutErrStreamRedirect output_redirect;

        // Execute the Python script
        py::exec(python_script);

        // Output the script execution result
        //std::cout << "\nPython Script Output:\n" << output_redirect.get_captured_output() << "\n";
    }
    catch (const py::error_already_set& e)
    {
        // Handle exceptions thrown by the Python interpreter
        std::cerr << "Python error: " << e.what() << std::endl;
    }
    catch (const std::exception& e)
    {
        // Handle other C++ exceptions
        std::cerr << "C++ exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        // Catch-all for other exceptions
        std::cerr << "An unknown exception occurred." << std::endl;
    }
}
//-------------------------------------------------------------------