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
template<typename MatrixType>

class BaseMatrix3D
{
public:

    // Accessors for matrix dimensions.
    uintptr_t pages()const { return underlying().pages(); }  
    uintptr_t rows()const { return underlying().rows(); }
    uintptr_t columns()const { return underlying().columns(); }
    uintptr_t size()const { return this->pages() * this->rows() * this->columns(); }

    // Accessors for matrix elements.
    decltype(auto) at(int64_t page, int64_t row, int64_t column)const { return this->const_at_(page, row, column); }
    decltype(auto) at(int64_t page, int64_t row, int64_t column) { return this->non_const_at_(page, row, column); }
    decltype(auto) at(int64_t index) const { return (*this)(index); }
    decltype(auto) at(int64_t index) { return (*this)(index); }

    // Operator overloads for element access.
    decltype(auto) operator()(int64_t page, int64_t row, int64_t column)const { return this->const_at_(page, row, column); }
    decltype(auto) operator()(int64_t page, int64_t row, int64_t column) { return this->non_const_at_(page, row, column); }

    decltype(auto) operator()(int64_t index)const
    {
        int64_t page = index / (this->rows() * this->columns());

        int64_t remainder = index % (this->rows() * this->columns());

        return (*this)(page, remainder / this->columns(), remainder % this->columns());
    }

    decltype(auto) operator()(int64_t index)
    {
        int64_t page = index / (this->rows() * this->columns());

        int64_t remainder = index % (this->rows() * this->columns());

        return (*this)(page, remainder / this->columns(), remainder % this->columns());
    }

    // Circular accessors for matrix elements.
    decltype(auto) circ_at(int64_t page, int64_t row, int64_t column)const
    {
        int64_t circ_page = (this->pages() + page % this->pages) % this->pages();
        int64_t circ_row = (this->rows() + row % this->rows()) % this->rows();
        int64_t circ_column = (this->columns() + column % this->columns()) % this->columns();
        return (*this)(circ_page, circ_row, circ_column);
    }
    
    decltype(auto) circ_at(int64_t page, int64_t row, int64_t column)
    {
        int64_t circ_page = (this->pages() + page % this->pages) % this->pages();
        int64_t circ_row = (this->rows() + row % this->rows()) % this->rows();
        int64_t circ_column = (this->columns() + column % this->columns()) % this->columns();
        return (*this)(circ_page, circ_row, circ_column);
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



private:

    // Private constructor to prevent direct instantiation.
    BaseMatrix3D<MatrixType>(){}
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

    // Implementation for element access, to be provided by derived classes.
    decltype(auto) const_at_(int64_t page, int64_t row, int64_t column)const
    {
        return underlying().const_at_(page, row, column);
    }

    decltype(auto) non_const_at_(int64_t page, int64_t row, int64_t column)
    {
        return underlying().non_const_at_(page, row, column);
    }
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix3d< BaseMatrix3D<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_BASE_MATRIX3D_HPP_
