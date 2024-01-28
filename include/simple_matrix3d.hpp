//-------------------------------------------------------------------
/**
 * @file simple_matrix3d.hpp
 * @brief Defines the SimpleMatrix3D class for basic 3D matrix storage.
 *
 * The SimpleMatrix3D class provides a basic implementation of a 3D matrix
 * using a one-dimensional std::vector for storage. This class is a simpler
 * alternative for matrix operations, following the Curiously Recurring Template
 * Pattern (CRTP) for type safety and performance.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_SIMPLE_MATRIX3D_HPP_
#define INCLUDE_SIMPLE_MATRIX3D_HPP_



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
 * @brief Forward declation of the MatrixFactory class which is used
 *        to create SharedMatrixRef references of matrices.
 */
//-------------------------------------------------------------------
class MatrixFactory;
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class SimpleMatrix3D
 * @brief A simple 3D matrix class using std::vector for storage.
 *
 * SimpleMatrix3D is designed for basic 3d-matrix operations. It stores 3d-matrix
 * elements in a contiguous block of memory for efficient access and manipulation.
 * This class inherits from BaseMatrix3D and implements the required interface
 * for 3d-matrix operations.
 *
 * @tparam DataType The data type of the 3d-matrix elements.
 */
//-------------------------------------------------------------------
template<typename DataType>

class SimpleMatrix3D : public BaseMatrix3D<SimpleMatrix3D<DataType>,true>
{
public:

    // Type of value that is stored in the matrix
    using value_type = DataType;

    friend class MatrixFactory;
    friend class BaseMatrix3D<SimpleMatrix3D<DataType>,true>;

    /**
     * Default constructor. Creates a matrix of specified dimensions with all elements initialized to a default value.
     * @param pages The number of pages in the matrix. Default is 0.
     * @param rows The number of rows in the matrix. Default is 0.
     * @param columns The number of columns in the matrix. Default is 0.
     * @param initial_value The initial value for each element of the matrix. Default is static_cast<DataType>(0).
     */
    SimpleMatrix3D(uintptr_t pages = 0, uintptr_t rows = 0, uintptr_t columns = 0, const DataType& initial_value = static_cast<DataType>(0))
    {
        this->resize_(pages, rows, columns, initial_value);
    }

    /**
     * Constructor from a generic matrix expression.
     * @param matrix The matrix expression from which to initialize this matrix.
     */
    template<typename MatrixType,
             std::enable_if_t<is_type_a_matrix3d<MatrixType>{}>* = nullptr>

    SimpleMatrix3D(const MatrixType& matrix)
    {
        auto pages = matrix.pages();
        auto rows = matrix.rows();
        auto columns = matrix.columns();

        this->resize_(pages, rows, columns);

        for(int k = 0; k < pages; ++k)
            for(int i = 0; i < rows; ++i)
                for(int j = 0; j < columns; ++j)
                    (*this)(k,i,j) = matrix(k,i,j);
    }

    /**
     * Assignment operator from a generic matrix expression.
     * @param matrix The matrix expression from which to assign this matrix.
     * @return A reference to this matrix.
     */
    template<typename MatrixType,
             std::enable_if_t<is_type_a_matrix3d<MatrixType>{}>* = nullptr>

    SimpleMatrix3D<DataType>& operator=(const MatrixType& matrix)
    {
        auto pages = matrix.pages();
        auto rows = matrix.rows();
        auto columns = matrix.columns();

        this->resize_(pages,rows, columns);

        for(int k = 0; k < pages; ++k)
            for(int i = 0; i < rows; ++i)
                for(int j = 0; j < columns; ++j)
                    (*this)(k,i,j) = matrix(k,i,j);
        
        return (*this);
    }

    /**
     * Gets the number of rows in the matrix.
     * @return The number of rows.
     */
    uintptr_t pages() const
    {
        return pages_;
    }

    /**
     * Gets the number of rows in the matrix.
     * @return The number of rows.
     */
    uintptr_t rows() const
    {
        return rows_;
    }

    /**
     * Gets the number of columns in the matrix.
     * @return The number of columns.
     */
    uintptr_t columns() const
    {
        return columns_;
    }



private: // Private functions

    /**
     * Resizes the matrix to new dimensions, initializing new elements to a specified value.
     * @param pages The new number of pages.
     * @param rows The new number of rows.
     * @param columns The new number of columns.
     * @param initial_value The value to initialize new elements to. Default is static_cast<DataType>(0).
     */
    std::error_code resize_(uintptr_t pages, uintptr_t rows, uintptr_t columns, const DataType& initial_value = static_cast<DataType>(0))
    {
        // In case of failed memory allocation, we just
        // set the matrix size to zero
        try 
        {
            pages_ = pages;
            rows_ = rows;
            columns_ = columns;
            data_.resize(pages * rows * columns, initial_value);
            return std::error_code();
        }
        catch (const std::bad_alloc& e)
        {
            // Memory allocation failed, so we just
            // set the size to zero
            pages_ = 0;
            rows_ = 0;
            columns_ = 0;
            data_.clear();
            return std::make_error_code(std::errc::not_enough_memory);
        }
    }

    /**
     * Accesses the element at the specified position (const version).
     * @param page The page index of the element.
     * @param row The row index of the element.
     * @param column The column index of the element.
     * @return The element at the specified position.
     */
    const DataType& const_at_(int64_t page, int64_t row, int64_t column) const
    {
        return data_[(page * rows_ * columns_) + (row * columns_) + column];
    }

    /**
     * Accesses the element at the specified position (modifiable version).
     * @param page The page index of the element.
     * @param row The row index of the element.
     * @param column The column index of the element.
     * @return A reference to the element at the specified position.
     */
    DataType& non_const_at_(int64_t page, int64_t row, int64_t column)
    {
        return data_[(page * rows_ * columns_) + (row * columns_) + column];
    }



private: // Private variables

    uintptr_t pages_ = 0;           ///< The number pf pages in the 3d-matrix.
    uintptr_t rows_ = 0;            ///< The number of rows in the matrix.
    uintptr_t columns_ = 0;         ///< The number of columns in the matrix.
    std::vector<DataType> data_;    ///< The flat array storing matrix elements.
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix3d< SimpleMatrix3D<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif // INCLUDE_SIMPLE_MATRIX3D_HPP_
