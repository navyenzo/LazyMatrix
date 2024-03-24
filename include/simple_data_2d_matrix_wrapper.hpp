//-------------------------------------------------------------------
/**
 * @file simple_data_2d_matrix_wrapper.hpp
 * @brief Provides a wrapper to treat simple data types as 2D matrix-like objects.
 *
 * This file contains the SimpleData2DMatrixWrapper template class, which wraps simple data types
 * (like int, float, etc.) to be treated as 1x1 matrices. This is particularly useful in polymorphic
 * contexts where both simple types and matrices need to be handled uniformly. The file also includes
 * utility functions to facilitate the use of this wrapper.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_SIMPLE_DATA_2D_MATRIX_WRAPPER_HPP_
#define INCLUDE_SIMPLE_DATA_2D_MATRIX_WRAPPER_HPP_



//-------------------------------------------------------------------
#include "base_matrix.hpp"
#include "shared_references.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief A wrapper class for simple data types to provide a 2D matrix-like interface.
 *
 * This class allows simple data types (like int, float, etc.) to be treated as 1x1 matrices.
 * It's particularly useful in a polymorphic context where different data types including
 * matrices and simple data types are handled uniformly.
 *
 * @tparam ScalarDataType The data type of the value to be wrapped.
 */
//-------------------------------------------------------------------
template<typename ScalarDataType>
class SimpleData2DMatrixWrapper : public BaseMatrix<SimpleData2DMatrixWrapper<ScalarDataType>,true>
{
public:

    using value_type = ScalarDataType;

    friend class BaseMatrix<SimpleData2DMatrixWrapper<ScalarDataType>,true>;

    /**
     * @brief Construct a new Simple Data 2D Matrix Wrapper object.
     * 
     * @param value The value to be wrapped in the matrix.
     */
    explicit SimpleData2DMatrixWrapper(const ScalarDataType& value) : value_(value)
    {
    }

    /**
     * @brief Get the number of rows in the matrix, always 1 for simple data.
     * 
     * @return Number of rows (1).
     */
    uintptr_t rows() const
    {
        return 1;
    }

    /**
     * @brief Get the number of columns in the matrix, always 1 for simple data.
     * 
     * @return Number of columns (1).
     */
    uintptr_t columns() const
    {
        return 1;
    }

    // Functions used to handle row and column header names
    std::string get_row_header(int64_t row_index) const { return row_header_; }
    std::string get_column_header(int64_t column_index) const { return column_header_; }
    void set_row_header(int64_t row_index, const std::string& row_header) const { row_header_ = row_header; }
    void set_column_header(int64_t column_index, const std::string& column_header) const { column_header_ = column_header; }



private: // Private functions

    /**
     * @brief Dummy "resize" function needed for the matrix interface, but
     *        here it doesn't do anything
     * 
     * @param rows 
     * @param columns 
     * @return std::error_code 
     */
    std::error_code resize_(uintptr_t rows, uintptr_t columns)
    {
        return std::error_code();
    }

    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A copy of the value of the element at the specified position.
     */
    value_type const_at_(int64_t row, int64_t column)const
    {
        return value_;
    }

    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A reference to the element at the specified position.
     */
    value_type& non_const_at_(int64_t row, int64_t column)
    {
        return value_;
    }



private:

    ScalarDataType value_;
    mutable std::string row_header_ = "row: 0";
    mutable std::string column_header_ = "col: 0";
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename ScalarDataType>

struct is_type_a_matrix< SimpleData2DMatrixWrapper<ScalarDataType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Creates a 2d-matrix-like wrapper for a simple data value.
 *
 * This function takes a simple data type and wraps it in a `SimpleData2DMatrixWrapper`,
 * allowing it to be used in contexts that expect a matrix-like object.
 *
 * @tparam ScalarDataType The type of the simple data to be wrapped.
 * @param value The value to be wrapped.
 * @return SimpleData2DMatrixWrapper<ScalarDataType> The wrapped matrix-like object.
 */
//-------------------------------------------------------------------
template<typename ScalarDataType>

inline auto

wrap_scalar_in_matrix(const ScalarDataType& value)
{
    auto wrapped_scalar = std::make_shared<SimpleData2DMatrixWrapper<ScalarDataType>>(value);
    return SharedMatrixRef<SimpleData2DMatrixWrapper<ScalarDataType>>(wrapped_scalar);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_SIMPLE_DATA_2D_MATRIX_WRAPPER_HPP_
