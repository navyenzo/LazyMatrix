//-------------------------------------------------------------------
/**
 * @file simple_matrix3d.hpp
 * @brief Defines the SimpleMatrix3D class for basic 3D matrix operations.
 *
 * The SimpleMatrix3D class provides a basic implementation of a 3D matrix
 * using a one-dimensional std::vector for storage. This class is a simpler
 * alternative for matrix operations, following the Curiously Recurring Template
 * Pattern (CRTP) for type safety and performance.
 *
 * @author Vincenzo Barbato
 * @contact GitHub Project: https://github.com/navyenzo/LazyMatrix.git
 *          LinkedIn: https://www.linkedin.com/in/vincenzobarbato/
 */
//-------------------------------------------------------------------



#ifndef SIMPLE_MATRIX3D_HPP
#define SIMPLE_MATRIX3D_HPP



//-------------------------------------------------------------------
#include <vector>
#include <stdexcept>
#include <cstdint>

#include "base_matrix3d.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class SimpleMatrix3D
 * @brief A simple 3D matrix class using std::vector for storage.
 *
 * SimpleMatrix is designed for basic matrix operations. It stores matrix
 * elements in a contiguous block of memory for efficient access and manipulation.
 * This class inherits from BaseMatrix and implements the required interface
 * for matrix operations.
 *
 * @tparam DataType The data type of the matrix elements.
 */
//-------------------------------------------------------------------
template<typename DataType>

class SimpleMatrix3D : public BaseMatrix3D<SimpleMatrix3D<DataType> >
{
public:

    /**
     * @brief Constructor for creating a matrix of given dimensions.
     * @param pages The number of pages in the matrix.
     * @param rows The number of rows in the matrix.
     * @param columns The number of columns in the matrix.
     */
    SimpleMatrix3D(uintptr_t pages, uintptr_t rows, uintptr_t columns, const DataType& initial_value = static_cast<DataType>(0))
    {
        this->resize(pages, rows, columns, initial_value);
    }

    /**
     * @brief Gets the number of pages in the matrix.
     * @return Number of pages.
     */
    uintptr_t pages() const
    {
        return pages_;
    }

    /**
     * @brief Gets the number of rows in the matrix.
     * @return Number of rows.
     */
    uintptr_t rows() const
    {
        return rows_;
    }

    /**
     * @brief Gets the number of columns in the matrix.
     * @return Number of columns.
     */
    uintptr_t columns() const
    {
        return columns_;
    }

    /**
     * @brief Accesses the element at the specified position (const version).
     * @param page Page index of the element.
     * @param row Row index of the element.
     * @param column Column index of the element.
     * @return The element at the specified position.
     */
    const DataType& at_(int64_t page, int64_t row, int64_t column) const
    {
        return data_[page*rows()*columns() + row*columns() + column];
    }

    /**
     * @brief Accesses the element at the specified position (modifiable version).
     * @param page Page index of the element.
     * @param row Row index of the element.
     * @param column Column index of the element.
     * @return A reference to the element at the specified position.
     */
    DataType& at_(int64_t page, int64_t row, int64_t column)
    {
        return data_[page*rows()*columns() + row*columns() + column];
    }

    /**
     * @brief Resizes the matrix to new dimensions and initializes to a specified value.
     * @param pages New number of pages.
     * @param rows New number of rows.
     * @param columns New number of columns.
     * @param initial_value Value to initialize new elements (default = 0).
     */
    void resize(uintptr_t pages, uintptr_t rows, uintptr_t columns, const DataType& initial_value = static_cast<DataType>(0))
    {
        // In case of failed memory allocation, we just
        // set the matrix size to zero
        try 
        {
            pages_ = pages;
            rows_ = rows;
            columns_ = columns;
            data_.resize(pages * rows * columns, initial_value);
        }
        catch (const std::bad_alloc& e)
        {
            // Memory allocation failed, so we just
            // set the size to zero
            pages_ = 0;
            rows_ = 0;
            columns_ = 0;
            data_.clear();
        }
    }



private:

    uintptr_t pages_ = 0;                ///< Number of pages in the matrix.
    uintptr_t rows_ = 0;                 ///< Number of rows in the matrix.
    uintptr_t columns_ = 0;              ///< Number of columns in the matrix.

    std::vector<DataType> data_;         ///< The flat array storing matrix elements.
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename DataType>

struct is_type_a_matrix3d< SimpleMatrix3D<DataType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif // SIMPLE_MATRIX3D_HPP
