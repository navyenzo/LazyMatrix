//-------------------------------------------------------------------
/**
 * @file python_bindings.hpp
 * @brief Provides Python bindings for the LazyMatrix library using pybind11.
 *
 * This file contains the template function `bind_matrix` which is used to create
 * Python bindings for different matrix types defined in the LazyMatrix library.
 * It uses pybind11 to expose the functionality of matrices to Python, allowing
 * for seamless integration of LazyMatrix with Python scripts. The bindings enable
 * Python users to interact with LazyMatrix matrices as if they were native Python
 * objects, using common matrix operations such as accessing the number of rows
 * and columns, and element access.
 *
 * The `bind_matrix` function is templated to work with any matrix type that
 * conforms to the interface of BaseMatrix, making it highly flexible and
 * adaptable to different matrix implementations within the LazyMatrix library.
 *
 * Usage:
 * In C++, include this header in the file where you define your pybind11 module,
 * and use the `bind_matrix` function to add bindings for your specific matrix types.
 *
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 * @see https://github.com/navyenzo/LazyMatrix.git
 * @namespace LazyMatrix
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_PYTHON_BINDINGS_HPP_
#define INCLUDE_PYTHON_BINDINGS_HPP_



//-------------------------------------------------------------------
#include <pybind11/pybind11.h>
#include "base_matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Binds a given matrix type to Python using pybind11.
 *
 * This template function creates bindings for a matrix type derived from BaseMatrix,
 * enabling its use in Python scripts. It exposes common matrix functionalities
 * such as accessing the number of rows and columns, and element access through
 * the operator() method. The function is templated to work with any matrix type
 * that conforms to the interface of BaseMatrix.
 *>("module_name", "Module description");
 *   bind_matrix<YourMatrixType>(m, "YourMatrixTypeName");
 * @endcode
 *
 * Usage in Python:
 * @code
 *   import module_name
 *   mat = module_name.YourMatrixTypeName(...)
 *   print(mat.rows(), mat.columns())
 *   element = mat(i, j)
 * @endcode
 *
 * @tparam MatrixType The matrix type to bind. Should be derived from BaseMatrix.
 * @param m The pybind11 module to which the bindings will be added.
 * @param name The name of the matrix type in Python.
 */
//-------------------------------------------------------------------
template<typename MatrixType>
void bind_matrix(pybind11::module &m, const char* name)
{
     using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type>::type;
     
    // Creating a Python class in the module for the MatrixType
    pybind11::class_<MatrixType, std::shared_ptr<MatrixType>>(m, name)
        // Binding the 'rows' method
        .def("rows", &MatrixType::rows)
        // Binding the 'columns' method
        .def("columns", &MatrixType::columns)
        // Binding the 'size' method
        .def("size", &MatrixType::size)
        // Binding the 'at' method for accessing elements (const version)
        .def("at", 
                static_cast<const value_type& (MatrixType::*)(int64_t, int64_t) const>(&MatrixType::at),
                pybind11::arg("row"), pybind11::arg("column"))
        // Binding the 'at' method for accessing elements (non-const version)
        .def("at", 
                static_cast<value_type& (MatrixType::*)(int64_t, int64_t)>(&MatrixType::at),
                pybind11::arg("row"), pybind11::arg("column"))
        // Binding the 'at' method for single index access (const version)
        .def("at_index", 
                static_cast<const value_type& (MatrixType::*)(int64_t) const>(&MatrixType::at),
                pybind11::arg("index"))
        // Binding the 'at' method for single index access (non-const version)
        .def("at_index", 
                static_cast<value_type& (MatrixType::*)(int64_t)>(&MatrixType::at),
                pybind11::arg("index"))
        // Binding the 'operator()' for element access (const version)
        .def("__call__", 
                static_cast<const value_type& (MatrixType::*)(int64_t, int64_t) const>(&MatrixType::operator()),
                pybind11::arg("row"), pybind11::arg("column"))
        // Binding the 'operator()' for element access (non-const version)
        .def("__call__", 
                static_cast<value_type& (MatrixType::*)(int64_t, int64_t)>(&MatrixType::operator()),
                pybind11::arg("row"), pybind11::arg("column"))
        // Binding the 'circ_at' method for circular access (const version)
        .def("circ_at", 
                static_cast<const value_type& (MatrixType::*)(int64_t, int64_t) const>(&MatrixType::circ_at),
                pybind11::arg("row"), pybind11::arg("column"))
        // Binding the 'circ_at' method for circular access (non-const version)
        .def("circ_at", 
                static_cast<value_type& (MatrixType::*)(int64_t, int64_t)>(&MatrixType::circ_at),
                pybind11::arg("row"), pybind11::arg("column"))
        // Binding the 'circ_at' method for single index circular access (const version)
        .def("circ_at_index", 
                static_cast<const value_type& (MatrixType::*)(int64_t) const>(&MatrixType::circ_at),
                pybind11::arg("index"))
        // Binding the 'circ_at' method for single index circular access (non-const version)
        .def("circ_at_index", 
                static_cast<value_type& (MatrixType::*)(int64_t)>(&MatrixType::circ_at),
                pybind11::arg("index"))
        // Add other common methods or operators as needed
        ;
}
//-------------------------------------------------------------------
// Usage example:
// bind_matrix<SomeMatrixType>(m, "SomeMatrixTypeName");







//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif // INCLUDE_PYTHON_BINDINGS_HPP_
