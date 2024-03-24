//-------------------------------------------------------------------
/**
 * @file base_matrix3d.hpp
 * @brief Defines the BaseMatrix3D class as a foundation for 3D matrix operations in the LazyMatrix library.
 *
 * The BaseMatrix3D class serves as a base class for various 3D matrix types, providing common operations 
 * and properties used across different 3D matrix implementations. It employs the Curiously Recurring 
 * Template Pattern (CRTP) for type-safe operations.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_BASE_MATRIX3D_HPP_
#define INCLUDE_BASE_MATRIX3D_HPP_



//-------------------------------------------------------------------
#include <iostream>
#include <cstdint>
#include "page_and_row_and_column_headers.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Trait to determine if a type is a 3D matrix expression.
//-------------------------------------------------------------------
template<typename MatrixType>
struct is_type_a_matrix3d : std::false_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Base class for 3D matrix expressions.
 *
 * @tparam MatrixType The derived matrix type following the CRTP pattern.
 */
//-------------------------------------------------------------------
template<typename MatrixType,
         bool HasNonConstAccess>

class BaseMatrix3D
{
public:

    // Accessors for matrix dimensions.
    uintptr_t pages()const
    {
        return underlying().pages();
    }
    
    uintptr_t rows()const
    {
        return underlying().rows();
    }
    
    uintptr_t columns()const
    {
        return underlying().columns();
    }
    
    uintptr_t size()const
    {
        return this->pages() * this->rows() * this->columns();
    }

    // Operator overloads for element access.
    decltype(auto) operator()(int64_t page, int64_t row, int64_t column)const
    {
        return underlying().const_at_(page, row, column);
    }
    
    decltype(auto) operator()(int64_t page, int64_t row, int64_t column)
    {
        return underlying().non_const_at_(page, row, column);
    }
    
    decltype(auto) operator()(int64_t index)const
    {
        int64_t page = index / (this->rows() * this->columns());

        int64_t remainder = index % (this->rows() * this->columns());

        return underlying().const_at_(page, remainder / this->columns(), remainder % this->columns());
    }
    
    decltype(auto) operator()(int64_t index)
    {
        int64_t page = index / (this->rows() * this->columns());

        int64_t remainder = index % (this->rows() * this->columns());

        return underlying().const_at_(page, remainder / this->columns(), remainder % this->columns());
    }

    // Accessors for matrix elements.
    decltype(auto) at(int64_t page, int64_t row, int64_t column)const
    {
        return underlying().const_at_(page, row, column);
    }
    
    decltype(auto) at(int64_t page, int64_t row, int64_t column)
    {
        return underlying().non_const_at_(page, row, column);
    }
    
    decltype(auto) at(int64_t index)const
    {
        int64_t page = index / (this->rows() * this->columns());

        int64_t remainder = index % (this->rows() * this->columns());

        return underlying().const_at_(page, remainder / this->columns(), remainder % this->columns());
    }
    
    decltype(auto) at(int64_t index)
    {
        int64_t page = index / (this->rows() * this->columns());

        int64_t remainder = index % (this->rows() * this->columns());

        return underlying().const_at_(page, remainder / this->columns(), remainder % this->columns());
    }

    // Circular accessors for matrix elements.
    decltype(auto) circ_at(int64_t page, int64_t row, int64_t column)const
    {
        int64_t circ_page = (this->pages() + page % this->pages()) % this->pages();
        int64_t circ_row = (this->rows() + row % this->rows()) % this->rows();
        int64_t circ_column = (this->columns() + column % this->columns()) % this->columns();
        return underlying().const_at_(circ_page, circ_row, circ_column);
    }

    decltype(auto) circ_at(int64_t page, int64_t row, int64_t column)
    {
        int64_t circ_page = (this->pages() + page % this->pages()) % this->pages();
        int64_t circ_row = (this->rows() + row % this->rows()) % this->rows();
        int64_t circ_column = (this->columns() + column % this->columns()) % this->columns();
        return underlying().non_const_at_(circ_page, circ_row, circ_column);
    }

    decltype(auto) circ_at(int64_t index)const
    {
        int64_t circ_index = (this->size() + index % this->size()) % this->size();
        return (*this)(circ_index);
    }

    decltype(auto) circ_at(int64_t index)
    {
        int64_t circ_index = (this->size() + index % this->size()) % this->size();
        return (*this)(circ_index);
    }
    
    // Function used to resize the underlying matrix
    std::error_code resize(uintptr_t pages,
                           uintptr_t rows,
                           uintptr_t columns)
    {
        return underlying().resize_(pages, rows, columns);
    }

    // Setter methods defined here to help define python/c++ interface
    template<typename ValueType>
    void set_at(int64_t page, int64_t row, int64_t column, ValueType value)
    {
        this->at(page, row, column) = value;
    }

    // Setter methods defined here to help define python/c++ interface
    template<typename ValueType>
    void set_circ_at(int64_t page, int64_t row, int64_t column, ValueType value)
    {
        this->circ_at(page, row, column) = value;
    }

    // Set all matrix values to a constant defined here to help define pythong/c++ interface
    template<typename ValueType>
    void set_all_values_to_a_constant(ValueType value)
    {
        int64_t matrix_size = int64_t(this->size());
        
        for(int64_t i = 0; i < matrix_size; ++i)
        {
            this->at(i) = value;
        }
    }

    // Functions used to handle row and column header names
    std::string get_page_header(int64_t page_index) const { return underlying().get_page_header(page_index); }
    std::string get_row_header(int64_t row_index) const { return underlying().get_row_header(row_index); }
    std::string get_column_header(int64_t column_index) const { return underlying().get_column_header(column_index); }
    void set_page_header(int64_t page_index, const std::string& page_header) { underlying().set_page_header(page_index, page_header); }
    void set_row_header(int64_t row_index, const std::string& row_header) { underlying().set_row_header(row_index, row_header); }
    void set_column_header(int64_t column_index, const std::string& column_header) { underlying().set_column_header(column_index, column_header); }



private:

    // Private constructor to prevent direct instantiation.
    BaseMatrix3D<MatrixType,HasNonConstAccess>(){}
    friend MatrixType;

    // Access to the derived class instance.
    MatrixType& underlying()
    {
        return static_cast<MatrixType&>(*this);
    }

    const MatrixType& underlying()const
    {
        return static_cast<const MatrixType&>(*this);
    }
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Specializatiion for when there's only const-access
 */
//-------------------------------------------------------------------
template<typename MatrixType>

class BaseMatrix3D<MatrixType,false>
{
public:

    // Accessors for matrix dimensions.
    uintptr_t pages()const
    {
        return underlying().pages();
    }
    
    uintptr_t rows()const
    {
        return underlying().rows();
    }
    
    uintptr_t columns()const
    {
        return underlying().columns();
    }
    
    uintptr_t size()const
    {
        return this->pages() * this->rows() * this->columns();
    }

    // Operator overloads for element access.
    decltype(auto) operator()(int64_t page, int64_t row, int64_t column)const
    {
        return underlying().const_at_(page, row, column);
    }
    
    decltype(auto) operator()(int64_t index)const
    {
        int64_t page = index / (this->rows() * this->columns());

        int64_t remainder = index % (this->rows() * this->columns());

        return underlying().const_at_(page, remainder / this->columns(), remainder % this->columns());
    }

    // Accessors for matrix elements.
    decltype(auto) at(int64_t page, int64_t row, int64_t column)const
    {
        return underlying().const_at_(page, row, column);
    }
    
    decltype(auto) at(int64_t index)const
    {
        int64_t page = index / (this->rows() * this->columns());

        int64_t remainder = index % (this->rows() * this->columns());

        return underlying().const_at_(page, remainder / this->columns(), remainder % this->columns());
    }

    // Circular accessors for matrix elements.
    decltype(auto) circ_at(int64_t page, int64_t row, int64_t column)const
    {
        int64_t circ_page = (this->pages() + page % this->pages()) % this->pages();
        int64_t circ_row = (this->rows() + row % this->rows()) % this->rows();
        int64_t circ_column = (this->columns() + column % this->columns()) % this->columns();
        return underlying().const_at_(circ_page, circ_row, circ_column);
    }

    decltype(auto) circ_at(int64_t index)const
    {
        int64_t circ_index = (this->size() + index % this->size()) % this->size();
        return (*this)(circ_index);
    }

    // Functions used to handle row and column header names
    std::string get_page_header(int64_t page_index) const { return underlying().get_page_header(page_index); }
    std::string get_row_header(int64_t row_index) const { return underlying().get_row_header(row_index); }
    std::string get_column_header(int64_t column_index) const { return underlying().get_column_header(column_index); }
    void set_page_header(int64_t page_index, const std::string& page_header) const { underlying().set_page_header(page_index, page_header); }
    void set_row_header(int64_t row_index, const std::string& row_header) const { underlying().set_row_header(row_index, row_header); }
    void set_column_header(int64_t column_index, const std::string& column_header) const { underlying().set_column_header(column_index, column_header); }



private:

    // Private constructor to prevent direct instantiation.
    BaseMatrix3D<MatrixType,false>(){}
    friend MatrixType;

    const MatrixType& underlying()const
    {
        return static_cast<const MatrixType&>(*this);
    }
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType,
         bool HasNonConstAccess>

struct is_type_a_matrix3d<BaseMatrix3D<MatrixType,HasNonConstAccess>> : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_BASE_MATRIX3D_HPP_
