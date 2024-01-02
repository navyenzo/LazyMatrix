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