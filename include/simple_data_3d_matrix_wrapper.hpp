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
 * @brief A wrapper class to provide a 3d-matrix-like interface for simple data types.
 *
 * This class is designed to wrap simple data types such as bool, float, int, etc.,
 * giving them a matrix-like interface. It presents these types as 1x1 matrices,
 * allowing them to be used interchangeably with more complex matrix types in
 * operations and functions expecting matrix-like objects.
 *
 * @tparam ScalarDataType The type of the simple data to be wrapped.
 */
//-------------------------------------------------------------------
template<typename ScalarDataType>
class SimpleData3DMatrixWrapper : public BaseMatrix3D<SimpleData3DMatrixWrapper<ScalarDataType>,true>
{
public:

    using value_type = ScalarDataType;

    friend class BaseMatrix3D<SimpleData3DMatrixWrapper<ScalarDataType>,true>;

    
    SimpleData3DMatrixWrapper(const ScalarDataType& value) : value_(value)
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

    // Functions used to handle page and row and column header names
    std::string get_page_header(int64_t page_index) { return page_header_; }
    std::string get_row_header(int64_t row_index) { return row_header_; }
    std::string get_column_header(int64_t column_index) { return column_header_; }
    void set_page_header(int64_t page_index, const std::string& page_header) { page_header_ = page_header; }
    void set_row_header(int64_t row_index, const std::string& row_header) { row_header_ = row_header; }
    void set_column_header(int64_t column_index, const std::string& column_header) { column_header_ = column_header; }



private: // Private functions

    /**
     * @brief Dummy "resize" function needed for the matrix interface, but
     *        here it doesn't do anything
     * 
     * @param pages
     * @param rows 
     * @param columns 
     * @return std::error_code 
     */
    std::error_code resize_(uintptr_t pages, uintptr_t rows, uintptr_t columns)
    {
        return std::error_code();
    }

    /**
     * @brief Accesses the element at the specified position.
     * @param page Page index.
     * @param row Row index.
     * @param column Column index.
     * @return A copy of the value of the element at the specified position.
     */
    value_type const_at_(int64_t page, int64_t row, int64_t column)const
    {
        return value_;
    }

    /**
     * @brief Accesses the element at the specified position.
     * @param page Page index.
     * @param row Row index.
     * @param column Column index.
     * @return A reference to the element at the specified position.
     */
    value_type& non_const_at_(int64_t page, int64_t row, int64_t column)
    {
        return value_;
    }



private:

    ScalarDataType value_;
    std::string page_header_ = "page: 0";
    std::string row_header_ = "row: 0";
    std::string column_header_ = "col: 0";
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename ScalarDataType>

struct is_type_a_matrix3d< SimpleData3DMatrixWrapper<ScalarDataType> > : std::true_type
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
 * @tparam ScalarDataType The type of the simple data to be wrapped.
 * @param value The value to be wrapped.
 * @return SharedMatrix3DRef of a scalar wrapped in a 3d matrix.
 */
//-------------------------------------------------------------------
template<typename ScalarDataType>

inline auto

wrap_scalar_in_matrix3d(const ScalarDataType& value)
{
    auto wrapped_scalar = std::make_shared<SimpleData3DMatrixWrapper<ScalarDataType>>(value);
    return SharedMatrix3DRef<SimpleData3DMatrixWrapper<ScalarDataType>>(wrapped_scalar);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_SIMPLE_DATA_3D_MATRIX_WRAPPER_HPP_
