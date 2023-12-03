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
 * @link https://www.linkedin.com/in/vincenzobarbato/
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_SIMPLE_DATA_2D_MATRIX_WRAPPER_HPP_
#define INCLUDE_SIMPLE_DATA_2D_MATRIX_WRAPPER_HPP_



//-------------------------------------------------------------------
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
 * @brief A wrapper class for simple data types to provide a 2D matrix-like interface.
 *
 * This class allows simple data types (like int, float, etc.) to be treated as 1x1 matrices.
 * It's particularly useful in a polymorphic context where different data types including
 * matrices and simple data types are handled uniformly.
 *
 * @tparam SimpleDataType The data type of the value to be wrapped.
 */
//-------------------------------------------------------------------
template<typename SimpleDataType>
class SimpleData2DMatrixWrapper : public BaseMatrix<SimpleData2DMatrixWrapper<SimpleDataType> >
{
public:

    /**
     * @brief Construct a new Simple Data 2D Matrix Wrapper object.
     * 
     * @param value The value to be wrapped in the matrix.
     */
    explicit SimpleData2DMatrixWrapper(const SimpleDataType& value) : value_(value)
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
    
    
    
    /**
     * @brief Const access of the wrapped value, ignoring row and column indices.
     * 
     * @param row Row index (ignored).
     * @param column Column index (ignored).
     * @return The wrapped value.
     */
    SimpleDataType at_(int64_t row, int64_t column) const
    {
        return value_; // Since it's a single value, ignore row and column indices
    }

    /**
     * @brief Non-const access of the wrapped value, ignoring row and column indices.
     * 
     * @param row Row index (ignored).
     * @param column Column index (ignored).
     * @return The wrapped value.
     */
    SimpleDataType at_(int64_t row, int64_t column)
    {
        return value_; // Since it's a single value, ignore row and column indices
    }



private:

    SimpleDataType value_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename SimpleDataType>

struct is_type_a_matrix< SimpleData2DMatrixWrapper<SimpleDataType> > : std::true_type
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
 * @tparam SimpleDataType The type of the simple data to be wrapped.
 * @param value The value to be wrapped.
 * @return SimpleData2DMatrixWrapper<SimpleDataType> The wrapped matrix-like object.
 */
//-------------------------------------------------------------------
template<typename SimpleDataType>

SimpleData2DMatrixWrapper<SimpleDataType> wrap_in_matrix(const SimpleDataType& value)
{
    return SimpleData2DMatrixWrapper<SimpleDataType>(value);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_SIMPLE_DATA_2D_MATRIX_WRAPPER_HPP_
