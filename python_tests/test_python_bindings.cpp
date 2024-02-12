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
PYBIND11_EMBEDDED_MODULE(lazymatrix, m)
{
    LazyMatrix::bind_factory_functions_and_matrix_types(m);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
TEST_CASE("Python bindings for LazyMatrix", "[python_bindings]")
{
    std::stringstream program_output;

    // Initialize the Python interpreter.
    py::scoped_interpreter guard{};

    // Example input and output matrices setup
    auto m_iota = LazyMatrix::generate_iota_matrix<double>(2,2,0,1);
    auto m1 = LazyMatrix::wrap_matrix_const(LazyMatrix::MatrixFactory::create_simple_matrix<Poco::Dynamic::Var>(2,2));
    auto m2 = LazyMatrix::wrap_matrix(LazyMatrix::MatrixFactory::create_simple_matrix<Poco::Dynamic::Var>(2,2));

    try
    {
        // This captures the output of std::cout, std::cerr
        // and python out/err
        LazyMatrix::PythonStdOutErrStreamRedirect python_output_redirect;

        pybind11::dict locals;
        pybind11::module py_module = pybind11::module::import("lazymatrix");

        locals["in0"] = m1;
        locals["out0"] = m2;


        std::string python_script =
            "import lazymatrix\n"
            "print('in0:', flush=True)\n"
            "print(in0)\n"
            "print('out0:', flush=True)\n"
            "print(out0)\n"
            "print('about to change values of m2', flush=True)\n"
            "out0.set_at(0,0,-5)\n"
            "out0.set_at(0,1,-6)\n"
            "out0.set_at(1,0,-7)\n"
            "out0.set_at(1,1,-8)\n"
            "print('out0 (after modification):', flush=True)\n"
            "print(out0)\n";

        // Execute the Python script
        pybind11::exec(python_script, py_module.attr("__dict__"), locals);

        // Capture the python script output
        program_output << python_output_redirect.get_captured_output();
    }
    catch (const py::error_already_set& e)
    {
        // Handle exceptions thrown by the Python interpreter
        program_output << "Python error: " << e.what() << std::endl;
    }
    catch (const std::exception& e)
    {
        // Handle other C++ exceptions
        std::cerr << "C++ exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        // Catch-all for other exceptions
        program_output << "An unknown exception occurred." << std::endl;
    }

    // Tests
    REQUIRE(m2(0,0) == -5);
    REQUIRE(m2(0,1) == -6);
    REQUIRE(m2(1,0) == -7);
    REQUIRE(m2(1,1) == -8);
}
//-------------------------------------------------------------------