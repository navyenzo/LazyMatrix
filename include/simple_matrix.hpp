//-------------------------------------------------------------------
/**
 * @file simple_matrix.hpp
 * @brief Defines the SimpleMatrix class for basic 2D matrix storage.
 *
 * The SimpleMatrix class provides a basic implementation of a 2D matrix
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



#ifndef INCLUDE_SIMPLE_MATRIX_HPP_
#define INCLUDE_SIMPLE_MATRIX_HPP_



//-------------------------------------------------------------------
#include <vector>
#include <stdexcept>
#include <cstdint>

#include "base_matrix.hpp"
#include "shared_references.hpp"

// eigen library for fast/efficient matrix math
#include "Eigen/Eigen"

// dlib library for neural networks
#include "dlib/mlp.h"
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
 * @class SimpleMatrix
 * @brief A simple 2D matrix class using std::vector for storage.
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

class SimpleMatrix : public BaseMatrix<SimpleMatrix<DataType>,true>
{
public:

    // Type of value that is stored in the matrix
    using value_type = DataType;

    friend class MatrixFactory;
    friend class BaseMatrix<SimpleMatrix<DataType>,true>;

    /**
     * @brief Default constructor. Initializes a matrix with given rows and columns.
     * @param rows Number of rows in the matrix. Default is 0.
     * @param columns Number of columns in the matrix. Default is 0.
     * @param initial_value The initial value to fill the matrix. Default is 0.
     */
    SimpleMatrix(uintptr_t rows = 0, uintptr_t columns = 0, const DataType& initial_value = static_cast<DataType>(0));

    /**
     * @brief Default copy constructor (deep copy).
     * @param matrix The source matrix to deep copy.
     */
    SimpleMatrix(const SimpleMatrix<DataType>& matrix) = default;

    /**
     * @brief Constructor to create a matrix from a dlib matrix.
     * @tparam DataType2 Data type of the dlib matrix.
     * @tparam NR Number of rows in the dlib matrix.
     * @tparam NC Number of columns in the dlib matrix.
     * @tparam mem_manager Memory manager type of the dlib matrix.
     * @tparam layout Memory layout type of the dlib matrix.
     * @param dlib_matrix The dlib matrix to convert from.
     */
    template<typename DataType2, long NR, long NC, typename mem_manager, typename layout>
    SimpleMatrix(const dlib::matrix<DataType2, NR, NC, mem_manager, layout>& dlib_matrix);

    /**
     * @brief Constructor to create a matrix from an Eigen matrix.
     * @tparam DataType2 Data type of the Eigen matrix.
     * @param m The Eigen matrix to convert from.
     */
    template<typename DataType2>
    SimpleMatrix(const Eigen::MatrixBase<DataType2>& m);

    /**
     * @brief Construct a new Matrix object copying a matrix reference
     * 
     * @param matrix_expression The matrix to deep copy
     */
    template<typename ReferenceType, std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>
    SimpleMatrix(ReferenceType matrix_expression);

    /**
     * @brief Default Assignment operator (deep copy).
     * @param matrix The source matrix to deep copy.
     * @return Reference to this matrix after deep copying.
     */
    SimpleMatrix<DataType>& operator=(const SimpleMatrix<DataType>& matrix) = default;

    /**
     * @brief Assignment operator from a dlib matrix.
     * @tparam DataType2 Data type of the dlib matrix.
     * @tparam NR Number of rows in the dlib matrix.
     * @tparam NC Number of columns in the dlib matrix.
     * @tparam mem_manager Memory manager type of the dlib matrix.
     * @tparam layout Memory layout type of the dlib matrix.
     * @param dlib_matrix The dlib matrix to assign from.
     * @return Reference to this matrix after assignment.
     */
    template<typename DataType2, long NR, long NC, typename mem_manager, typename layout>
    SimpleMatrix<DataType>& operator=(const dlib::matrix<DataType2, NR, NC, mem_manager, layout>& dlib_matrix);

    /**
     * @brief Assignment operator from an eigen matrix expression.
     * @tparam DataType2 Data type of the Eigen matrix.
     * @param m The Eigen matrix to copy from.
     * @return Reference to this matrix object after assignment.
     */
    template<typename DataType2>
    SimpleMatrix<DataType>& operator=(const Eigen::MatrixBase<DataType2>& m);

    /**
     * @brief Assignement from a reference to a matrix expression.
     * 
     * @param matrix_expression The matrix to copy from
     */
    template<typename ReferenceType, std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>
    SimpleMatrix<DataType>& operator=(ReferenceType matrix_expression);

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

    // Functions used to handle row and column header names
    std::string get_row_header(int64_t row_index) { return headers_.get_row_header(row_index); }
    std::string get_column_header(int64_t column_index) { return headers_.get_column_header(column_index); }
    void set_row_header(int64_t row_index, const std::string& row_header) { headers_.set_row_header(row_index, row_header); }
    void set_column_header(int64_t column_index, const std::string& column_header) { headers_.set_column_header(column_index, column_header); }



private: // Private functions

    /**
     * Resizes the matrix to new dimensions, initializing new elements to a specified value.
     * @param rows The new number of rows.
     * @param columns The new number of columns.
     * @param initial_value The value to initialize new elements to. Default is static_cast<DataType>(0).
     */
    std::error_code resize_(uintptr_t rows, uintptr_t columns, const DataType& initial_value = static_cast<DataType>(0))
    {
        // In case of failed memory allocation, we just
        // set the matrix size to zero
        try 
        {
            rows_ = rows;
            columns_ = columns;
            data_.resize(rows * columns, initial_value);
            return std::error_code();
        }
        catch (const std::bad_alloc& e)
        {
            // Memory allocation failed, so we just
            // set the size to zero
            rows_ = 0;
            columns_ = 0;
            data_.clear();
            return std::make_error_code(std::errc::not_enough_memory);
        }
    }

    /**
     * Accesses the element at the specified position (const version).
     * @param row The row index of the element.
     * @param column The column index of the element.
     * @return The element at the specified position.
     */
    const DataType& const_at_(int64_t row, int64_t column) const
    {
        return data_[(row * columns_) + column];
    }

    /**
     * Accesses the element at the specified position (modifiable version).
     * @param row The row index of the element.
     * @param column The column index of the element.
     * @return A reference to the element at the specified position.
     */
    DataType& non_const_at_(int64_t row, int64_t column)
    {
        return data_[(row * columns_) + column];
    }



private: // Private variables

    uintptr_t rows_ = 0;                 ///< The number of rows in the matrix.
    uintptr_t columns_ = 0;              ///< The number of columns in the matrix.
    std::vector<DataType> data_;         ///< The flat array storing matrix elements.

    // Row and Column Headers
    RowAndColumnNames headers_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< SimpleMatrix<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Default constructor from rows, columns and initial value
//-------------------------------------------------------------------
template<typename DataType>

inline SimpleMatrix<DataType>::SimpleMatrix(uintptr_t rows, uintptr_t columns, const DataType& initial_value)
{
    this->resize_(rows, columns, initial_value);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Constructor from a dlib matrix
//-------------------------------------------------------------------
template<typename DataType>
template<typename DataType2, long NR, long NC, typename mem_manager, typename layout>

inline SimpleMatrix<DataType>::SimpleMatrix(const dlib::matrix<DataType2, NR, NC, mem_manager, layout>& dlib_matrix)
{
    this->resize_(dlib_matrix.nr(), dlib_matrix.nc());

    for(int64_t i = 0; i < this->rows(); ++i)
    {
        for(int64_t j = 0; j < this->columns(); ++j)
        {
            (*this)(i,j) = dlib_matrix(i,j);
        }
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Constructor from an eigen matrix
//-------------------------------------------------------------------
template<typename DataType>
template<typename DataType2>

inline SimpleMatrix<DataType>::SimpleMatrix(const Eigen::MatrixBase<DataType2>& m)
{
    uintptr_t rows = m.rows();
    uintptr_t columns = m.columns();

    this->resize_(rows,columns);

    for(int64_t i = 0; i < rows; ++i)
    {
        for(int64_t j = 0; j < columns; ++j)
        {
            (*this)(i,j) = m(i,j);
        }
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Constructor from a matrix expression reference
//-------------------------------------------------------------------
template<typename DataType>
template<typename ReferenceType, std::enable_if_t<is_matrix_reference<ReferenceType>{}>*>

inline SimpleMatrix<DataType>::SimpleMatrix(ReferenceType matrix_expression)
{
    uintptr_t rows = matrix_expression.rows();
    uintptr_t columns = matrix_expression.columns();

    this->resize_(rows,columns);

    for(int64_t i = 0; i < rows; ++i)
    {
        for(int64_t j = 0; j < columns; ++j)
        {
            (*this)(i,j) = matrix_expression(i,j);
        }
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Assignment from a dlib matrix
//-------------------------------------------------------------------
template<typename DataType>
template<typename DataType2, long NR, long NC, typename mem_manager, typename layout>

inline SimpleMatrix<DataType>& SimpleMatrix<DataType>::operator=(const dlib::matrix<DataType2, NR, NC, mem_manager, layout>& dlib_matrix)
{
    this->resize_(dlib_matrix.nr(), dlib_matrix.nc());

    for(int64_t i = 0; i < this->rows(); ++i)
    {
        for(int64_t j = 0; j < this->columns(); ++j)
        {
            (*this)(i,j) = dlib_matrix(i,j);
        }
    }

    return (*this);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Assignment from an eigen matrix
//-------------------------------------------------------------------
template<typename DataType>
template<typename DataType2>

inline SimpleMatrix<DataType>& SimpleMatrix<DataType>::operator=(const Eigen::MatrixBase<DataType2>& m)
{
    this->resize_(m.rows(),m.cols());

    for(int64_t i = 0; i < m.rows(); ++i)
    {
        for(int64_t j = 0; j < m.cols(); ++j)
        {
            (*this)(i,j) = m(i,j);
        }
    }

    return (*this);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Assignment from a reference to a matrix expression
//-------------------------------------------------------------------
template<typename DataType>
template<typename ReferenceType, std::enable_if_t<is_matrix_reference<ReferenceType>{}>*>

inline SimpleMatrix<DataType>& SimpleMatrix<DataType>::operator=(ReferenceType matrix_expression)
{
    uintptr_t rows = matrix_expression.rows();
    uintptr_t columns = matrix_expression.columns();

    std::error_code error = this->resize_(rows, columns);

    if(error)
        return (*this);

    for(int64_t i = 0; i < rows; ++i)
        for(int64_t j = 0; j < columns; ++j)
            (*this)(i,j) = matrix_expression(i,j);

    return (*this);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif // INCLUDE_SIMPLE_MATRIX_HPP_
