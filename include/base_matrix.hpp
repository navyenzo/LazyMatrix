//-------------------------------------------------------------------
/**
 * @file base_matrix.hpp
 * @brief Defines the BaseMatrix class as a foundation for matrix operations in the LazyMatrix library.
 *
 * The BaseMatrix class serves as a base class for various matrix types, providing common operations 
 * and properties used across different matrix implementations. It employs the Curiously Recurring 
 * Template Pattern (CRTP) for type-safe operations.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_BASE_MATRIX_HPP_
#define INCLUDE_BASE_MATRIX_HPP_



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
/**
 * @brief Trait to check if a type is a matrix type
 *
 * This trait is used to determine if a given type is a matrix type
 * which is used to define functions that require arguments to be
 * matrices
 * 
 * @tparam T The reference type (templated with MatrixType) to check.
 */
//-------------------------------------------------------------------
template<typename MatrixType>
struct is_type_a_matrix : std::false_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Base class for matrix expressions.
 *
 * @tparam MatrixType The derived matrix type following the CRTP pattern.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

class BaseMatrix
{
public:
    
    // Accessors for matrix dimensions.
    uintptr_t rows()const { return underlying().rows(); }
    uintptr_t columns()const { return underlying().columns(); }
    uintptr_t size()const { return this->rows() * this->columns(); }

    // Accessors for matrix elements.
    decltype(auto) at(int64_t row, int64_t column)const { return (*this)(row, column); }
    decltype(auto) at(int64_t row, int64_t column) { return (*this)(row, column); }
    decltype(auto) at(int64_t index)const { return (*this)(index / this->columns(), index % this->columns()); }
    decltype(auto) at(int64_t index) { return (*this)(index / this->columns(), index % this->columns()); }

    // Operator overloads for element access.
    decltype(auto) operator()(int64_t row, int64_t column)const { return this->const_at_(row, column); }
    decltype(auto) operator()(int64_t row, int64_t column) { return this->non_const_at_(row, column); }
    decltype(auto) operator()(int64_t index)const { return (*this)(index / this->columns(), index % this->columns()); }
    decltype(auto) operator()(int64_t index) { return (*this)(index / this->columns(), index % this->columns()); }

    // Circular accessors for matrix elements.
    decltype(auto) circ_at(int64_t row, int64_t column)const
    {
        int64_t circ_row = ( int64_t(this->rows()) + row % int64_t(this->rows()) ) % int64_t(this->rows());
        int64_t circ_column = ( int64_t(this->columns()) + column % int64_t(this->columns()) ) % int64_t(this->columns());
        return (*this)(circ_row, circ_column);
    }

    decltype(auto) circ_at(int64_t row, int64_t column)
    {
        int64_t circ_row = ( int64_t(this->rows()) + row % int64_t(this->rows()) ) % int64_t(this->rows());
        int64_t circ_column = ( int64_t(this->columns()) + column % int64_t(this->columns()) ) % int64_t(this->columns());
        return (*this)(circ_row, circ_column);
    }

    decltype(auto) circ_at(int64_t index)const
    {
        int64_t circ_index = ( int64_t(this->size()) + index % int64_t(this->size()) ) % int64_t(this->size());
        return (*this)(circ_index);
    }

    decltype(auto) circ_at(int64_t index)
    {
        int64_t circ_index = ( int64_t(this->size()) + index % int64_t(this->size()) ) % int64_t(this->size());
        return (*this)(circ_index);
    }



    // Setter methods defined here to help define python/c++ interface
    template<typename ValueType>
    void set_circ_at(int64_t row, int64_t column, const ValueType& value) { this->circ_at(row, column) = value; }



private:

    // Private default constructor to prevent direct instantiation.
    BaseMatrix<MatrixType>(){}
    friend MatrixType;

    // Accessors for the derived type.
    const MatrixType& underlying()const { return static_cast<const MatrixType&>(*this); }
    MatrixType& underlying() { return static_cast<MatrixType&>(*this); }

    // Implementation for element access, to be provided by derived classes.
    decltype(auto) const_at_(int64_t row, int64_t column)const { return underlying().const_at_(row, column); }
    decltype(auto) non_const_at_(int64_t row, int64_t column) { return underlying().non_const_at_(row, column); }
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
 // Trait specialization to recognize BaseMatrix types as matrix expressions.
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< BaseMatrix<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_BASE_MATRIX_HPP_
