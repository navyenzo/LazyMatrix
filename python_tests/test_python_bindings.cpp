//-------------------------------------------------------------------
/**
 * \file test_lazy_matrix_python_bindings.cpp
 * \brief Test file for Python bindings of the LazyMatrix library.
 *
 * This file contains a test case for the Python bindings of the LazyMatrix library using pybind11.
 * It creates instances of LazyMatrix::SimpleMatrix and LazyMatrix::Matrix in C++, and then
 * exposes these instances to Python. Python code is executed to interact with these C++ objects,
 * demonstrating the integration between C++ and Python. The test also captures Python output and
 * redirects it to C++ for validation. The Catch2 framework is used for structuring the test case.
 * 
 * @author Vincenzo Barbato
 * @contact GitHub Project: https://github.com/navyenzo/LazyMatrix.git
 *          LinkedIn: https://www.linkedin.com/in/vincenzobarbato/
 */
//-------------------------------------------------------------------



//-------------------------------------------------------------------
#include <iostream>
#include <sstream>

#define CATCH_CONFIG_MAIN // This define is needed once only when using Catch2
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch_all.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

#include <lazy_matrix.hpp>
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
PYBIND11_EMBEDDED_MODULE(example, m)
{
    m.def("print_simple_matrix", &print_matrix<LazyMatrix::SimpleMatrix<double>>);
    m.def("print_matrix", &print_matrix<LazyMatrix::Matrix<double>>);

    LazyMatrix::bind_matrix_storage<LazyMatrix::SimpleMatrix<double>>(m, "SimpleMatrixDouble");
    LazyMatrix::bind_matrix_storage<LazyMatrix::Matrix<double>>(m, "MatrixDouble");
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
TEST_CASE("Python bindings for LazyMatrix", "[python_bindings]")
{
    pybind11::scoped_interpreter guard{}; // Start the Python interpreter

    std::string python_script_output;

    // Create matrices in C++
    LazyMatrix::Matrix<double> m1(3, 3, 5.0);
    LazyMatrix::Matrix<double> m2(3, 3, 10.0);
    auto named_input_matrices = LazyMatrix::create_named_matrix_vector("in", m1, m2);

    try
    {
        // Redirect both Python and C++ output to a string
        LazyMatrix::PythonStdOutErrStreamRedirect python_output_redirect;
        
        // Import the Pybind11 module containing the bindings
        pybind11::module example = pybind11::module::import("example");

        // Expose C++ matrices to Python using pointers
        pybind11::dict locals;
        for (const auto& [name, matrix] : named_input_matrices)
        {
            locals[name.c_str()] = pybind11::cast(&matrix);
        }

        // Execute Python code
        pybind11::exec(R"(
            from example import SimpleMatrixDouble, MatrixDouble, print_simple_matrix, print_matrix

            # Access the C++ matrices directly
            print("Printing C++ matrices in Python:")
            print_matrix(in0)
            print_matrix(in1)

            # Modify an element of in0
            in0.set_circ_at(5, 5, -17)

            # Modify an element of in1
            in1.set_circ_at(3, 3, -14)

            # Resize in0
            in0.resize(10,10,-11)

            # Print the modified matrix
            print("Modified in0:")
            print_matrix(in0)
        )", pybind11::globals(), locals);

        // Get the captured output
        python_script_output += python_output_redirect.get_captured_output();
    }
    catch (const std::exception& e)
    {
        // Handle any C++ exceptions here
        std::cerr << "C++ Exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        // Handle any other unexpected exceptions
        std::cerr << "Unexpected Exception" << std::endl;
    }

    std::cout << "\n\n\nPython Script Output:\n\n" << python_script_output << "\n\n\n";
}
//-------------------------------------------------------------------