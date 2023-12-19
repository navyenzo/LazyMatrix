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
 * @contact GitHub Project: https://github.com/navyenzo/LazyMatrix.git
 *          LinkedIn: https://www.linkedin.com/in/vincenzobarbato/
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

#include "base_matrix.hpp"
#include "output_operators.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Bind matrix class for view access in Python.
 * @tparam MatrixType The matrix type to bind.
 * @param m The pybind11 module.
 * @param name The name of the matrix in Python.
 */
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

void bind_matrix_view(pybind11::module &m, const char* name)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type>::type;

    pybind11::class_<MatrixType>(m, name)
        .def("rows", &MatrixType::rows)
        .def("columns", &MatrixType::columns)
        .def("size", &MatrixType::size)
        .def("at", (const value_type& (MatrixType::*)(int64_t, int64_t) const) &MatrixType::at)
        .def("at", (const value_type& (MatrixType::*)(int64_t) const) &MatrixType::at)
        .def("circ_at", (const value_type& (MatrixType::*)(int64_t, int64_t) const) &MatrixType::circ_at)
        .def("circ_at", (const value_type& (MatrixType::*)(int64_t) const) &MatrixType::circ_at)
        .def("__call__", (const value_type& (MatrixType::*)(int64_t, int64_t) const) &MatrixType::operator())
        .def("__call__", (const value_type& (MatrixType::*)(int64_t) const) &MatrixType::operator());
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Bind matrix class for const access in Python.
 * @tparam MatrixType The matrix type to bind.
 * @param m The pybind11 module.
 * @param name The name of the matrix in Python.
 */
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline void bind_matrix(pybind11::module &m, const char* name)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type>::type;

    pybind11::class_<MatrixType>(m, name)
        .def("rows", &MatrixType::rows)
        .def("columns", &MatrixType::columns)
        .def("size", &MatrixType::size)
        .def("at", (const value_type& (MatrixType::*)(int64_t, int64_t) const) &MatrixType::at)
        .def("at", (const value_type& (MatrixType::*)(int64_t) const) &MatrixType::at)
        .def("circ_at", (const value_type& (MatrixType::*)(int64_t, int64_t) const) &MatrixType::circ_at)
        .def("circ_at", (const value_type& (MatrixType::*)(int64_t) const) &MatrixType::circ_at)
        .def("__call__", (const value_type& (MatrixType::*)(int64_t, int64_t) const) &MatrixType::operator())
        .def("__call__", (const value_type& (MatrixType::*)(int64_t) const) &MatrixType::operator())
        .def("set_circ_at", (void (MatrixType::*)(int64_t, int64_t, const value_type&)) &MatrixType::set_circ_at);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Bind matrix class with storage constructor in Python.
 * @tparam MatrixType The matrix type to bind.
 * @param m The pybind11 module.
 * @param name The name of the matrix in Python.
 */
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline void bind_matrix_storage(pybind11::module &m, const char* name)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type>::type;

    pybind11::class_<MatrixType>(m, name)
        .def(pybind11::init<uintptr_t, uintptr_t, const value_type&>())
        .def("resize", (void (MatrixType::*)(int64_t, int64_t, const value_type&)) &MatrixType::resize)
        .def("rows", &MatrixType::rows)
        .def("columns", &MatrixType::columns)
        .def("size", &MatrixType::size)
        .def("at", (const value_type& (MatrixType::*)(int64_t, int64_t) const) &MatrixType::at)
        .def("at", (const value_type& (MatrixType::*)(int64_t) const) &MatrixType::at)
        .def("circ_at", (const value_type& (MatrixType::*)(int64_t, int64_t) const) &MatrixType::circ_at)
        .def("circ_at", (const value_type& (MatrixType::*)(int64_t) const) &MatrixType::circ_at)
        .def("__call__", (const value_type& (MatrixType::*)(int64_t, int64_t) const) &MatrixType::operator())
        .def("__call__", (const value_type& (MatrixType::*)(int64_t) const) &MatrixType::operator())
        .def("set_circ_at", (void (MatrixType::*)(int64_t, int64_t, const value_type&)) &MatrixType::set_circ_at);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/*!
 * \class PythonStdOutErrStreamRedirect
 * \brief Captures and redirects C++ std::cout/std::cerr and Python's stdout/stderr streams.
 *
 * This class is designed to intercept and redirect both C++'s std::cout and std::cerr streams as well as
 * Python's stdout and stderr streams. It enables the capture of output and error messages generated
 * by both C++ and Python code in a unified manner. This is useful in scenarios where C++ and Python
 * are used together, such as in applications using pybind11.
 */
//-------------------------------------------------------------------
class PythonStdOutErrStreamRedirect 
{
public:
    /*!
     * \brief Constructor for PythonStdOutErrStreamRedirect.
     *
     * Initializes and redirects C++'s std::cout and std::cerr and Python's stdout and stderr streams
     * to internal StringIO buffers. This allows for capturing output generated by both C++ and Python scripts.
     */
    PythonStdOutErrStreamRedirect()
    {
        auto stringio = pybind11::module::import("io").attr("StringIO");

        // Redirect std::cout
        py_cout_buf_ = new PythonStdoutStreambuf(stringio());
        old_cout_buf_ = std::cout.rdbuf(py_cout_buf_);

        // Redirect std::cerr
        py_cerr_buf_ = new PythonStdoutStreambuf(stringio());
        old_cerr_buf_ = std::cerr.rdbuf(py_cerr_buf_);

        // Redirect Python stdout and stderr
        auto sysm = pybind11::module::import("sys");
        stdout_ = sysm.attr("stdout");
        stderr_ = sysm.attr("stderr");
        stdout_buffer_ = stringio();
        stderr_buffer_ = stringio();
        sysm.attr("stdout") = stdout_buffer_;
        sysm.attr("stderr") = stderr_buffer_;
    }

    /*!
     * \brief Destructor for PythonStdOutErrStreamRedirect.
     *
     * Restores C++'s std::cout and std::cerr and Python's original stdout and stderr streams upon
     * destruction of the object.
     */
    ~PythonStdOutErrStreamRedirect()
    {
        // Restore std::cout
        std::cout.rdbuf(old_cout_buf_);
        delete py_cout_buf_;

        // Restore std::cerr
        std::cerr.rdbuf(old_cerr_buf_);
        delete py_cerr_buf_;

        // Restore Python stdout and stderr
        auto sysm = pybind11::module::import("sys");
        sysm.attr("stdout") = stdout_;
        sysm.attr("stderr") = stderr_;
    }

    /*!
     * \brief Retrieves the captured Python stdout as a string.
     *
     * \return A std::string containing the captured standard output from Python.
     */
    std::string std_out_string()
    {
        stdout_buffer_.attr("seek")(0);
        return pybind11::str(stdout_buffer_.attr("read")());
    }

    /*!
     * \brief Retrieves the captured Python stderr as a string.
     *
     * \return A std::string containing the captured standard error output from Python.
     */
    std::string std_err_string()
    {
        stderr_buffer_.attr("seek")(0);
        return pybind11::str(stderr_buffer_.attr("read")());
    }

    /*!
     * \brief Retrieves the captured C++ std::cout as a string.
     *
     * \return A std::string containing the captured standard output from C++ std::cout.
     */
    std::string c_out_string()
    {
        return py_cout_buf_->get_string();
    }

    /*!
     * \brief Retrieves the captured C++ std::cerr as a string.
     *
     * \return A std::string containing the captured standard error output from C++ std::cerr.
     */
    std::string c_err_string()
    {
        return py_cerr_buf_->get_string();
    }



private:

    class PythonStdoutStreambuf : public std::streambuf 
    {
    public:
        PythonStdoutStreambuf(pybind11::object stringio)
        : stringio_(stringio) 
        {
            setp(&buffer_, &buffer_ + 1);
        }

        std::string get_string()
        {
            stringio_.attr("seek")(0);
            return pybind11::str(stringio_.attr("read")());
        }

    
    
    protected:

        virtual int_type overflow(int_type c) override
        {
            if (c != EOF) 
            {
                stringio_.attr("write")(pybind11::str(std::string(1, c)));
                return c;
            }
            return sync() ? traits_type::eof() : traits_type::not_eof(c);
        }

        virtual int sync() override
        {
            stringio_.attr("flush")();
            return 0; // Return 0 on success
        }

    private:
        pybind11::object stringio_;
        char buffer_;
    };

    std::streambuf* old_cout_buf_;
    PythonStdoutStreambuf* py_cout_buf_;
    std::streambuf* old_cerr_buf_;
    PythonStdoutStreambuf* py_cerr_buf_;
    pybind11::object stdout_;
    pybind11::object stderr_;
    pybind11::object stdout_buffer_;
    pybind11::object stderr_buffer_;
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
 * \tparam MatrixType The type of the matrices, ensuring that only matrix types are used.
 * \tparam Matrices Variadic template to handle multiple matrix references.
 * \param prefix A string prefix for naming the matrices (e.g., "in" for input matrices).
 * \param first The first matrix reference.
 * \param rest The rest of the matrix references.
 * \return A vector of pairs of matrix names and matrix references.
 */
//-------------------------------------------------------------------
template<typename MatrixType, typename... Matrices>
std::vector<std::pair<std::string, MatrixType&>> create_named_matrix_vector(const std::string& prefix, MatrixType& first, Matrices&... rest)
{
    std::vector<std::pair<std::string, MatrixType&>> named_matrices;
    int index = 0;

    auto add_matrix = [&](MatrixType& matrix)
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
 * \tparam MatrixType The type of the matrices, ensuring that only matrix types are used.
 * \param prefix A string prefix for naming the matrices. This is not used but is kept for function signature consistency.
 * \return An empty vector of pairs of matrix names and matrix references.
 */
//-------------------------------------------------------------------
template<typename MatrixType>
std::vector<std::pair<std::string, MatrixType&>> create_named_matrix_vector(const std::string& prefix)
{
    return {}; // Return an empty vector
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * \brief Executes a Python script with given input and output matrices.
 *
 * \tparam MatrixType The type of the matrices, which must be a matrix type.
 * \param input_matrices Vector of pairs of matrix names and references to matrix objects for input.
 * \param output_matrices Vector of pairs of matrix names and references to matrix objects for output.
 * \param script The Python script to execute.
 * \param module_name The Python module to import before executing the python script
 * \return The standard output captured from the Python script.
 */
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline std::string execute_python_script(const std::vector<std::pair<std::string, MatrixType&>>& input_matrices,
                                  std::vector<std::pair<std::string, MatrixType&>>& output_matrices,
                                  const std::string& script,
                                  const std::string& module_name)
{
    std::stringstream python_script_output;

    std::cout << "checking input matrices:\n\n";

    int i = 0;

    for(const auto& m : input_matrices)
    {
        std::cout << "m" << i << ":\nname: " << m.first << "\nsize: (" << m.second.rows() << "," << m.second.columns() << ")\n\n\n" << std::endl;
        ++i;
    }

    std::cout << "\n\n\nModule name = " << module_name << "\n\n" << std::endl;

    try
    {
        LazyMatrix::PythonStdOutErrStreamRedirect python_output_redirect;
        pybind11::scoped_interpreter guard{}; // Start the Python interpreter

        // Import the Pybind11 module containing the bindings
        pybind11::module py_module = pybind11::module::import(module_name.c_str());

        pybind11::dict locals;

        // Expose input matrices to Python
        for (const auto& [name, matrix] : input_matrices)
        {
            locals[name.c_str()] = pybind11::cast(&matrix);
        }

        // Expose output matrices to Python
        for (auto& [name, matrix] : output_matrices)
        {
            locals[name.c_str()] = pybind11::cast(&matrix);
        }

        // Execute the Python script
        pybind11::exec(script, pybind11::globals(), locals);

        // Capture the python script output
        python_script_output << python_output_redirect.std_out_string();
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