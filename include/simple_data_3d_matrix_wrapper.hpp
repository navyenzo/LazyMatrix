//-------------------------------------------------------------------
/**
 * @file simple_data_3d_matrix_wrapper.hpp
 * @brief Provides a wrapper to treat simple data types as 3D matrix-like objects.
 *
 * This file contains the SimpleData3DMatrixWrapper template class, which extends the concept of
 * wrapping simple data types (like int, float, etc.) to provide a 3D matrix-like interface. It
 * treats these types as 1x1 matrices, allowing them to be integrated into operations and functions
 * that expect 3D matrix-like objects. The file also includes utility functions for easy use of this
 * wrapper in polymorphic environments.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_SIMPLE_DATA_3D_MATRIX_WRAPPER_HPP_
#define INCLUDE_SIMPLE_DATA_3D_MATRIX_WRAPPER_HPP_



//-------------------------------------------------------------------
#include "base_matrix3d.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief A wrapper class to provide a 3d-matrix-like interface for simple data types.
 *
 * This class is designed to wrap simple data types such as bool, float, int, etc.,
 * giving them a matrix-like interface. It presents these types as 1x1 matrices,
 * allowing them to be used interchangeably with more complex matrix types in
 * operations and functions expecting matrix-like objects.
 *
 * @tparam SimpleDataType The type of the simple data to be wrapped.
 */
//-------------------------------------------------------------------
template<typename SimpleDataType>
class SimpleData3DMatrixWrapper : public BaseMatrix3D<SimpleData3DMatrixWrapper<SimpleDataType> >
{
public:

    friend class BaseMatrix3D<SimpleData3DMatrixWrapper<SimpleDataType> >;

    
    SimpleData3DMatrixWrapper(const SimpleDataType& value) : value_(value)
    {
    }

    /**
     * @brief Get the number of pages in the 3d-matrix, always 1 for simple data.
     * 
     * @return Number of pages (1).
     */
    uintptr_t pages() const
    {
        return 1;
    }

    /**
     * @brief Get the number of rows in the 3d-matrix, always 1 for simple data.
     * 
     * @return Number of rows (1).
     */
    uintptr_t rows() const
    {
        return 1;
    }

    /**
     * @brief Get the number of columns in the 3d-matrix, always 1 for simple data.
     * 
     * @return Number of columns (1).
     */
    uintptr_t columns() const
    {
        return 1;
    }
    
    

    /**
     * @brief Const access of the wrapped value, ignoring page, row, and column indices.
     * 
     * This function returns the same wrapped value regardless of the specified indices.
     * 
     * @param page Page index (ignored).
     * @param row Row index (ignored).
     * @param column Column index (ignored).
     * @return The wrapped value.
     */
    SimpleDataType at_(int64_t page, int64_t row, int64_t column)const
    {
        return value_; // Since it's a single value, ignore page, row and column indices
    }

    /**
     * @brief Non-const access of the wrapped value, ignoring page, row, and column indices.
     * 
     * This function returns the same wrapped value regardless of the specified indices.
     * 
     * @param page Page index (ignored).
     * @param row Row index (ignored).
     * @param column Column index (ignored).
     * @return The wrapped value.
     */
    SimpleDataType at_(int64_t page, int64_t row, int64_t column)
    {
        return value_; // Since it's a single value, ignore page, row and column indices
    }



private:

    SimpleDataType value_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename SimpleDataType>

struct is_type_a_matrix3d< SimpleData3DMatrixWrapper<SimpleDataType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Creates a 3d-matrix-like wrapper for a simple data value.
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

SimpleData3DMatrixWrapper<SimpleDataType> wrap_in_matrix(const SimpleDataType& value)
{
    return SimpleData3DMatrixWrapper<SimpleDataType>(value);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_SIMPLE_DATA_3D_MATRIX_WRAPPER_HPP_
