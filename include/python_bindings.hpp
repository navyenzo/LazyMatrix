//-------------------------------------------------------------------
/**
 * @file python_bindings.hpp
 * @brief Python bindings for the LazyMatrix library.
 *
 * This file defines functions to bind matrix classes and their functionalities
 * to Python using pybind11. It includes bindings for view, const-access, and
 * storage constructors of matrix classes, along with their member functions.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_PYTHON_BINDINGS_HPP_
#define INCLUDE_PYTHON_BINDINGS_HPP_



//-------------------------------------------------------------------
#include <iostream>
#include <streambuf>
#include <iostream>
#include <sstream>

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

#include "shared_references.hpp"
#include "matrix_factory.hpp"
#include "output_operators.hpp"

namespace py = pybind11;
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------




//-------------------------------------------------------------------
/**
 * @brief Binds the MatrixFactory class and its static factory methods to a Python module.
 * 
 * This function is used to bind the MatrixFactory class from the LazyMatrix library to a Python module.
 * It exposes various static factory methods to Python, allowing for the creation of different types of matrices.
 * These include regular matrices, CSV matrices, image matrices, simple matrices, database matrices,
 * as well as their 3D counterparts.
 * 
 * @param m The Python module to which the class and methods are bound.
 */
//-------------------------------------------------------------------
inline void bind_factory_functions(pybind11::module_& m)
{
    using namespace py::literals; // For _a literal used in pybind11 function arguments

    // Wrappers for MatrixFactory::create_matrix function
    auto create_matrix_char = [](int64_t rows, int64_t columns)
    {
        return MatrixFactory::create_matrix<char>(rows, columns);
    };
    
    auto create_matrix_int = [](int64_t rows, int64_t columns)
    {
        return MatrixFactory::create_matrix<int>(rows, columns);
    };
    
    auto create_matrix_float = [](int64_t rows, int64_t columns)
    {
        return MatrixFactory::create_matrix<float>(rows, columns);
    };
    
    auto create_matrix_double = [](int64_t rows, int64_t columns)
    {
        return MatrixFactory::create_matrix<double>(rows, columns);
    };
    
    auto create_matrix_string = [](int64_t rows, int64_t columns)
    {
        return MatrixFactory::create_matrix<std::string>(rows, columns);
    };

    // Wrappers for MatrixFactory::create_matrix3d function
    auto create_matrix3d_char = [](int64_t pages, int64_t rows, int64_t columns)
    {
        return MatrixFactory::create_matrix3d<char>(pages, rows, columns);
    };

    auto create_matrix3d_int = [](int64_t pages, int64_t rows, int64_t columns)
    {
        return MatrixFactory::create_matrix3d<int>(pages, rows, columns);
    };

    auto create_matrix3d_float = [](int64_t pages, int64_t rows, int64_t columns)
    {
        return MatrixFactory::create_matrix3d<float>(pages, rows, columns);
    };

    auto create_matrix3d_double = [](int64_t pages, int64_t rows, int64_t columns)
    {
        return MatrixFactory::create_matrix3d<double>(pages, rows, columns);
    };

    auto create_matrix3d_string = [](int64_t pages, int64_t rows, int64_t columns)
    {
        return MatrixFactory::create_matrix3d<std::string>(pages, rows, columns);
    };

    // Wrappers for MatrixFactory::create_simple_matrix function
    auto create_simple_matrix_char = [](int64_t rows, int64_t columns)
    {
        return MatrixFactory::create_simple_matrix<char>(rows, columns);
    };
    
    auto create_simple_matrix_int = [](int64_t rows, int64_t columns)
    {
        return MatrixFactory::create_simple_matrix<int>(rows, columns);
    };
    
    auto create_simple_matrix_float = [](int64_t rows, int64_t columns)
    {
        return MatrixFactory::create_simple_matrix<float>(rows, columns);
    };
    
    auto create_simple_matrix_double = [](int64_t rows, int64_t columns)
    {
        return MatrixFactory::create_simple_matrix<double>(rows, columns);
    };
    
    auto create_simple_matrix_string = [](int64_t rows, int64_t columns)
    {
        return MatrixFactory::create_simple_matrix<std::string>(rows, columns);
    };

    // Wrappers for MatrixFactory::create_simple_matrix3d function
    auto create_simple_matrix3d_char = [](int64_t pages, int64_t rows, int64_t columns)
    {
        return MatrixFactory::create_simple_matrix3d<char>(pages, rows, columns);
    };

    auto create_simple_matrix3d_int = [](int64_t pages, int64_t rows, int64_t columns)
    {
        return MatrixFactory::create_simple_matrix3d<int>(pages, rows, columns);
    };

    auto create_simple_matrix3d_float = [](int64_t pages, int64_t rows, int64_t columns)
    {
        return MatrixFactory::create_simple_matrix3d<float>(pages, rows, columns);
    };

    auto create_simple_matrix3d_double = [](int64_t pages, int64_t rows, int64_t columns)
    {
        return MatrixFactory::create_simple_matrix3d<double>(pages, rows, columns);
    };

    auto create_simple_matrix3d_string = [](int64_t pages, int64_t rows, int64_t columns)
    {
        return MatrixFactory::create_simple_matrix3d<std::string>(pages, rows, columns);
    };

    // Binding the wrapper functions
    pybind11::class_<MatrixFactory>(m, "MatrixFactory")
        // create_matrix functions
        .def_static("create_matrix_char", create_matrix_char, "rows"_a, "columns"_a)
        .def_static("create_matrix_int", create_matrix_int, "rows"_a, "columns"_a)
        .def_static("create_matrix_float", create_matrix_float, "rows"_a, "columns"_a)
        .def_static("create_matrix_double", create_matrix_double, "rows"_a, "columns"_a)
        .def_static("create_matrix_string", create_matrix_string, "rows"_a, "columns"_a)
        // create_matrix3d functions
        .def_static("create_matrix3d_char", create_matrix3d_char, "pages"_a, "rows"_a, "columns"_a)
        .def_static("create_matrix3d_int", create_matrix3d_int, "pages"_a, "rows"_a, "columns"_a)
        .def_static("create_matrix3d_float", create_matrix3d_float, "pages"_a, "rows"_a, "columns"_a)
        .def_static("create_matrix3d_double", create_matrix3d_double, "pages"_a, "rows"_a, "columns"_a)
        .def_static("create_matrix3d_string", create_matrix3d_string, "pages"_a, "rows"_a, "columns"_a)
        // create_simple_matrix functions
        .def_static("create_simple_matrix_char", create_simple_matrix_char, "rows"_a, "columns"_a)
        .def_static("create_simple_matrix_int", create_simple_matrix_int, "rows"_a, "columns"_a)
        .def_static("create_simple_matrix_float", create_simple_matrix_float, "rows"_a, "columns"_a)
        .def_static("create_simple_matrix_double", create_simple_matrix_double, "rows"_a, "columns"_a)
        .def_static("create_simple_matrix_string", create_simple_matrix_string, "rows"_a, "columns"_a)
        // create_simple_matrix3d functions
        .def_static("create_simple_matrix3d_char", create_simple_matrix3d_char, "pages"_a, "rows"_a, "columns"_a)
        .def_static("create_simple_matrix3d_int", create_simple_matrix3d_int, "pages"_a, "rows"_a, "columns"_a)
        .def_static("create_simple_matrix3d_float", create_simple_matrix3d_float, "pages"_a, "rows"_a, "columns"_a)
        .def_static("create_simple_matrix3d_double", create_simple_matrix3d_double, "pages"_a, "rows"_a, "columns"_a)
        .def_static("create_simple_matrix3d_string", create_simple_matrix3d_string, "pages"_a, "rows"_a, "columns"_a);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Bind a SharedMatrixRef class to Python.
 *
 * This template function binds a SharedMatrixRef to a Python class, allowing the
 * Python side to interact with the C++ SharedMatrixRef objects. The binding includes the 
 * member functions of the SharedMatrixRef class. This function is specialized for types
 * recognized as matrix references.
 *
 * @tparam ReferenceType The shared matrix reference type.
 * @param m Reference to the Pybind11 module.
 * @param name The name of the class in the Python module.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

inline void bind_matrix_ref(py::module &m, const char* name)
{
    using ValueType = typename ReferenceType::value_type;

    py::class_<ReferenceType>(m, name)
        .def("rows", &ReferenceType::rows)
        .def("columns", &ReferenceType::columns)
        .def("size", &ReferenceType::size)
        .def("at", static_cast<ValueType (ReferenceType::*)(int64_t, int64_t) const>(&ReferenceType::at))
        .def("at", static_cast<ValueType& (ReferenceType::*)(int64_t, int64_t)>(&ReferenceType::at))
        .def("__call__", static_cast<ValueType (ReferenceType::*)(int64_t, int64_t) const>(&ReferenceType::operator()))
        .def("__call__", static_cast<ValueType& (ReferenceType::*)(int64_t, int64_t)>(&ReferenceType::operator()))
        .def("circ_at", static_cast<ValueType (ReferenceType::*)(int64_t, int64_t) const>(&ReferenceType::circ_at))
        .def("circ_at", static_cast<ValueType& (ReferenceType::*)(int64_t, int64_t)>(&ReferenceType::circ_at))
        .def("set_circ_at", &ReferenceType::set_circ_at)
        .def("resize", &ReferenceType::resize);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Bind a SharedMatrix3DRef class to Python.
 *
 * This template function binds a SharedMatrix3DRef to a Python class, allowing the
 * Python side to interact with the C++ SharedMatrix3DRef objects. The binding includes the 
 * member functions of the SharedMatrix3DRef class. This function is specialized for types
 * recognized as 3D matrix references.
 *
 * @tparam ReferenceType The shared 3D matrix reference type.
 * @param m Reference to the Pybind11 module.
 * @param name The name of the class in the Python module.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix3d_reference<ReferenceType>{}>* = nullptr>

inline void bind_matrix3d_ref(py::module &m, const char* name)
{
    using ValueType = typename ReferenceType::value_type;

    py::class_<ReferenceType>(m, name)
        .def("pages", &ReferenceType::pages)
        .def("rows", &ReferenceType::rows)
        .def("columns", &ReferenceType::columns)
        .def("size", &ReferenceType::size)
        .def("at", static_cast<ValueType (ReferenceType::*)(int64_t, int64_t, int64_t) const>(&ReferenceType::at))
        .def("at", static_cast<ValueType& (ReferenceType::*)(int64_t, int64_t, int64_t)>(&ReferenceType::at))
        .def("__call__", static_cast<ValueType (ReferenceType::*)(int64_t, int64_t, int64_t) const>(&ReferenceType::operator()))
        .def("__call__", static_cast<ValueType& (ReferenceType::*)(int64_t, int64_t, int64_t)>(&ReferenceType::operator()))
        .def("circ_at", static_cast<ValueType (ReferenceType::*)(int64_t, int64_t, int64_t) const>(&ReferenceType::circ_at))
        .def("circ_at", static_cast<ValueType& (ReferenceType::*)(int64_t, int64_t, int64_t)>(&ReferenceType::circ_at))
        .def("set_circ_at", &ReferenceType::set_circ_at)
        .def("resize", &ReferenceType::resize);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/*!
 * \class PythonStdOutErrStreamRedirect
 * \brief Captures and redirects C++ std::cout/std::cerr and Python's stdout/stderr streams.
 *
 * This class intercepts and redirects both C++'s std::cout and std::cerr streams as well as
 * Python's stdout and stderr streams. It captures output and error messages generated
 * by both C++ and Python code in a unified manner, storing them in a single sequential buffer.
 * This is useful in scenarios where C++ and Python are used together, such as in applications using pybind11.
 */
//-------------------------------------------------------------------
class PythonStdOutErrStreamRedirect 
{
public:

    /*!
     * \brief Constructor for PythonStdOutErrStreamRedirect.
     *
     * Initializes and redirects C++'s std::cout and std::cerr and Python's stdout and stderr streams
     * to a single internal stringstream buffer. This allows for capturing output generated by both C++ and Python scripts.
     */
    PythonStdOutErrStreamRedirect()
    {
        auto stringio = pybind11::module::import("io").attr("StringIO");
        auto sysm = pybind11::module::import("sys");

        // Shared buffer for all output
        shared_output_buffer_ = std::make_shared<std::stringstream>();

        // Redirect std::cout
        py_cout_buf_ = new PythonStdoutStreambuf(stringio(), shared_output_buffer_);
        old_cout_buf_ = std::cout.rdbuf(py_cout_buf_);

        // Redirect std::cerr
        py_cerr_buf_ = new PythonStdoutStreambuf(stringio(), shared_output_buffer_);
        old_cerr_buf_ = std::cerr.rdbuf(py_cerr_buf_);

        // Inject Python output capturer
        inject_python_output_capturer();
    }

    /*!
     * \brief Destructor for PythonStdOutErrStreamRedirect.
     *
     * Restores C++'s std::cout and std::cerr and Python's original stdout and stderr streams upon
     * destruction of the object.
     */
    ~PythonStdOutErrStreamRedirect()
    {
        // Restore C++ cout and cerr
        std::cout.rdbuf(old_cout_buf_);
        std::cerr.rdbuf(old_cerr_buf_);
        delete py_cout_buf_;
        delete py_cerr_buf_;

        // Restore Python stdout and stderr to their original state
        auto sysm = pybind11::module::import("sys");
        sysm.attr("stdout") = original_stdout_;
        sysm.attr("stderr") = original_stderr_;
    }

    /*!
     * \brief Retrieve the captured output as a string.
     * \return A std::string containing the captured output from both C++ and Python streams.
     */
    std::string get_captured_output() const
    {
        return shared_output_buffer_->str();
    }



private:

    /*!
     * \brief Injects a Python function to capture stdout and stderr.
     *
     * This method injects a Python function that redirects Python's stdout and stderr
     * to the shared C++ stringstream buffer.
     */
    void inject_python_output_capturer()
    {
        pybind11::exec(R"(
            import sys
            import io
            class CapturingStream(io.StringIO):
                def __init__(self, write_callback):
                    super().__init__()
                    self.write_callback = write_callback

                def write(self, txt):
                    self.write_callback(txt)

                def flush(self):
                    pass

            def capture_output(write_callback):
                sys.stdout = CapturingStream(write_callback)
                sys.stderr = CapturingStream(write_callback)
        )");

        auto capture_output = pybind11::module::import("__main__").attr("capture_output");
        capture_output(pybind11::cpp_function([this](const std::string& txt) {
            (*shared_output_buffer_) << txt;
        }));

        // Save the original Python stdout and stderr
        auto sysm = pybind11::module::import("sys");
        original_stdout_ = sysm.attr("stdout");
        original_stderr_ = sysm.attr("stderr");
    }

    class PythonStdoutStreambuf : public std::streambuf 
    {
    public:
        PythonStdoutStreambuf(pybind11::object stringio, std::shared_ptr<std::stringstream> shared_buffer)
        : stringio_(stringio), shared_buffer_(shared_buffer)
        {
            setp(&buffer_, &buffer_ + 1);
        }

    protected:
        virtual int_type overflow(int_type c) override
        {
            if (c != EOF) 
            {
                // Write to shared buffer
                (*shared_buffer_) << static_cast<char>(c);
                stringio_.attr("write")(pybind11::str(std::string(1, c)));

                // Immediately flush the buffers
                return this->sync();
            }
            return c;
        }

        virtual int sync() override
        {
            shared_buffer_->flush();
            stringio_.attr("flush")();
            return 0; // Return 0 on success
        }

    private:
        pybind11::object stringio_;
        char buffer_;
        std::shared_ptr<std::stringstream> shared_buffer_;
    };

    std::streambuf* old_cout_buf_;
    PythonStdoutStreambuf* py_cout_buf_;
    std::streambuf* old_cerr_buf_;
    PythonStdoutStreambuf* py_cerr_buf_;
    std::shared_ptr<std::stringstream> shared_output_buffer_;
    pybind11::object original_stdout_;
    pybind11::object original_stderr_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * \brief Creates a vector of named matrix references suitable for Python script execution.
 *
 * This overloaded function handles cases where at least one matrix is provided. It generates 
 * a vector containing pairs of matrix names and references. The names are automatically 
 * assigned based on the provided prefix followed by a sequential index.
 *
 * \tparam ReferenceType The type of the matrices, ensuring that only matrix types are used.
 * \tparam Matrices Variadic template to handle multiple matrix references.
 * \param prefix A string prefix for naming the matrices (e.g., "in" for input matrices).
 * \param first The first matrix reference.
 * \param rest The rest of the matrix references.
 * \return A vector of pairs of matrix names and matrix references.
 */
//-------------------------------------------------------------------
template<typename ReferenceType, typename... Matrices>
std::vector<std::pair<std::string, ReferenceType&>> create_named_matrix_vector(const std::string& prefix, ReferenceType& first, Matrices&... rest)
{
    std::vector<std::pair<std::string, ReferenceType&>> named_matrices;
    int index = 0;

    auto add_matrix = [&](ReferenceType& matrix)
    {
        named_matrices.emplace_back(prefix + std::to_string(index++), matrix);
    };

    add_matrix(first); // Add the first matrix
    (add_matrix(rest), ...); // Add the rest of the matrices

    return named_matrices;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * \brief Creates an empty vector of named matrix references.
 *
 * This overloaded function handles cases where no matrices are provided. It simply returns an empty vector.
 * This is useful when you need to provide an empty vector of matrix references to a function or process.
 *
 * \tparam ReferenceType The type of the matrices, ensuring that only matrix types are used.
 * \param prefix A string prefix for naming the matrices. This is not used but is kept for function signature consistency.
 * \return An empty vector of pairs of matrix names and matrix references.
 */
//-------------------------------------------------------------------
template<typename ReferenceType>
std::vector<std::pair<std::string, ReferenceType&>> create_named_matrix_vector(const std::string& prefix)
{
    return {}; // Return an empty vector
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * \brief Executes a Python script with given input and output matrices.
 *
 * \tparam ReferenceType The type of the matrices, which must be a matrix type.
 * \param input_matrices Vector of pairs of matrix names and references to matrix objects for input.
 * \param output_matrices Vector of pairs of matrix names and references to matrix objects for output.
 * \param script The Python script to execute.
 * \param module_name The Python module to import before executing the python script
 * \return The standard output captured from the Python script.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

std::string execute_python_script(const std::vector<ReferenceType>& input_matrices,
                                  const std::vector<ReferenceType>& output_matrices,
                                  const std::string& script,
                                  const std::string& module_name)
{
    std::stringstream python_script_output;

    try {
        // This captures the output of std::cout, std::cerr
        // and python out/err
        PythonStdOutErrStreamRedirect python_output_redirect;

        pybind11::dict locals;
        pybind11::module py_module = pybind11::module::import(module_name.c_str());

        // Automatically assign names to input matrices and expose them to Python
        for (size_t i = 0; i < input_matrices.size(); ++i) {
            locals[("in" + std::to_string(i)).c_str()] = pybind11::cast(input_matrices[i]);
        }

        // Automatically assign names to output matrices and expose them to Python
        for (size_t i = 0; i < output_matrices.size(); ++i) {
            locals[("out" + std::to_string(i)).c_str()] = pybind11::cast(output_matrices[i]);
        }

        // Execute the Python script
        pybind11::exec(script, py_module.attr("__dict__"), locals);

        // Capture the python script output
        python_script_output << python_output_redirect.get_captured_output();
    }
    catch (const std::exception& e)
    {
        python_script_output << "C++ Exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        python_script_output << "Unexpected Exception" << std::endl;
    }

    return python_script_output.str();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif // INCLUDE_PYTHON_BINDINGS_HPP_