//-------------------------------------------------------------------
/**
 * @file simple_matrix.hpp
 * @brief Defines the SimpleMatrix class for basic 2D matrix operations.
 *
 * The SimpleMatrix class provides a basic implementation of a 2D matrix
 * using a one-dimensional std::vector for storage. This class is a simpler
 * alternative for matrix operations, following the Curiously Recurring Template
 * Pattern (CRTP) for type safety and performance.
 *
 * @author Vincenzo Barbato
 * @contact GitHub Project: https://github.com/navyenzo/LazyMatrix.git
 *          LinkedIn: https://www.linkedin.com/in/vincenzobarbato/
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_SIMPLE_MATRIX_HPP_
#define INCLUDE_SIMPLE_MATRIX_HPP_



//-------------------------------------------------------------------
#include <vector>
#include <stdexcept>
#include <cstdint>

#include "base_matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyMatrix
{
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

class SimpleMatrix : public BaseMatrix<SimpleMatrix<DataType> >
{
public:

    /**
     * @brief Default constructor for creating a matrix of given dimensions.
     * @param rows The number of rows in the matrix.
     * @param columns The number of columns in the matrix.
     */
    SimpleMatrix(uintptr_t rows = 0, uintptr_t columns = 0, const DataType& initial_value = static_cast<DataType>(0))
    {
        this->resize(rows, columns, initial_value);
    }

    /**
     * @brief Copy constructor which makes a deep copy
     * @param matrix The matrix to copy
     */
    SimpleMatrix(const SimpleMatrix<DataType>& matrix)
    {
        this->resize(matrix.rows(), matrix.columns());

        for(int i = 0; i < matrix.rows(); ++i)
            for(int j = 0; j < matrix.columns(); ++j)
                (*this)(i,j) = matrix(i,j);
    }

    /**
     * @brief Constructor from a generic matrix expression
     * @param matrix The input matrix we will copy
     */
    template<typename MatrixType,
             std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

    SimpleMatrix(const MatrixType& matrix)
    {
        auto rows = matrix.rows();
        auto columns = matrix.columns();

        this->resize(matrix.rows(), matrix.columns());

        for(int i = 0; i < rows; ++i)
            for(int j = 0; j < columns; ++j)
                (*this)(i,j) = matrix(i,j);
    }

    /**
     * @brief Assignment operator from another simple matrix
     * @param matrix The matrix to copy
     */
    SimpleMatrix<DataType>& operator=(const SimpleMatrix<DataType>& matrix)
    {
        this->resize(matrix.rows(), matrix.columns());

        for(int i = 0; i < matrix.rows(); ++i)
            for(int j = 0; j < matrix.columns(); ++j)
                (*this)(i,j) = matrix(i,j);

        return (*this);
    }

    /**
     * @brief Assignment operator from a generic matrix expression
     * @param matrix The input matrix we will copy
     */
    template<typename MatrixType,
             std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

    SimpleMatrix<DataType>& operator=(const MatrixType& matrix)
    {
        auto rows = matrix.rows();
        auto columns = matrix.columns();

        this->resize(rows, columns);

        for(int i = 0; i < rows; ++i)
            for(int j = 0; j < columns; ++j)
                (*this)(i,j) = matrix(i,j);
        
        return (*this);
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
     * @param row Row index of the element.
     * @param column Column index of the element.
     * @return The element at the specified position.
     */
    const DataType& at_(int64_t row, int64_t column) const
    {
        return data_[(row * columns_) + column];
    }

    /**
     * @brief Accesses the element at the specified position (modifiable version).
     * @param row Row index of the element.
     * @param column Column index of the element.
     * @return A reference to the element at the specified position.
     */
    DataType& at_(int64_t row, int64_t column)
    {
        return data_[(row * columns_) + column];
    }

    /**
     * @brief Resizes the matrix to new dimensions and initializes to a specified value.
     * @param rows New number of rows.
     * @param columns New number of columns.
     * @param initial_value Value to initialize new elements (default = 0).
     */
    void resize(uintptr_t rows, uintptr_t columns, const DataType& initial_value = static_cast<DataType>(0))
    {
        // In case of failed memory allocation, we just
        // set the matrix size to zero
        try 
        {
            rows_ = rows;
            columns_ = columns;
            data_.resize(rows * columns, initial_value);
        }
        catch (const std::bad_alloc& e)
        {
            // Memory allocation failed, so we just
            // set the size to zero
            rows_ = 0;
            columns_ = 0;
            data_.clear();
        }
    }



private:

    uintptr_t rows_ = 0;                 ///< Number of rows in the matrix.
    uintptr_t columns_ = 0;              ///< Number of columns in the matrix.

    std::vector<DataType> data_;         ///< The flat array storing matrix elements.
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
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif // INCLUDE_SIMPLE_MATRIX_HPP_
