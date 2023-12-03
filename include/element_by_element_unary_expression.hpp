//-------------------------------------------------------------------
/**
 * @file element_by_element_unary_expression.hpp
 * @brief Provides element-by-element unary operations for matrices.
 *
 * This file is part of the LazyMatrix library. It includes a template structure and functions to perform 
 * element-by-element unary operations such as negation, sign, absolute value, square root, and exponential functions on matrices. 
 * These operations are essential for various matrix-based calculations and transformations.
 *
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_ELEMENT_BY_ELEMENT_UNARY_EXPRESSION_HPP_
#define INCLUDE_ELEMENT_BY_ELEMENT_UNARY_EXPRESSION_HPP_



//-------------------------------------------------------------------
#include <algorithm>
#include <functional>
#include <cmath>
#include <complex>
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
 * @brief A structure to represent element-by-element unary operations on a matrix.
 *
 * This structure allows for operations like negation, absolute value, square root, etc., 
 * to be applied element-wise on a matrix. It uses a functional approach to define 
 * the operation that should be applied to each element of the matrix.
 *
 * @tparam MatrixType Type of the matrix.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

struct ElementByElementUnaryExpression : public BaseMatrix< ElementByElementUnaryExpression<MatrixType> >
{
    // Type of value that is stored in expression
    // - We use that type to return the same type for the
    //   resulting unary operation (i.e. abs(m1(0,0)) returns type of m1(0,0))
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;

    // The operation function type
    using operation_type = std::function<value_type(value_type)>;



    ElementByElementUnaryExpression<MatrixType>(const MatrixType& expression,
                                                const operation_type& operation_function)
    : expression_(expression),
      operation_function_(operation_function)
    {
    }



    // This operation assumes both expressions are the
    // same size
    uintptr_t rows()const
    {
        return this->expression_.rows();
    }

    uintptr_t columns()const
    {
        return this->expression_.columns();
    }



    const MatrixType& get_expression()
    {
        return expression_;
    }
    
    

    decltype(auto) at_(int row, int column)const
    {
        return static_cast<value_type>(operation_function_(this->expression_.at(row,column)));
    }



private:

    const MatrixType& expression_;
    operation_type operation_function_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< ElementByElementUnaryExpression<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Subtraction -- operator-
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

operator-(const MatrixType& m)
{
    using value_type = typename std::remove_reference<decltype(m(0,0))>::type;

    return ElementByElementUnaryExpression<MatrixType>(m,std::negate<value_type>());
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Sign -- return the sign of every element (1 or -1)
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

sign(const MatrixType& m)
{
    using value_type = typename std::remove_reference<decltype(m(0,0))>::type;

    return ElementByElementUnaryExpression<MatrixType>(m,
                                                       [](value_type number)
                                                       {
                                                            value_type zero = static_cast<value_type>(0);
                                                            return static_cast<decltype(number)>( (zero < number) - (number < zero) );
                                                       }
                                                      );
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Absolute value -- return the absolute value of every element
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

abs(const MatrixType& m)
{
    using value_type = typename std::remove_reference<decltype(m(0,0))>::type;

    return ElementByElementUnaryExpression<MatrixType>(m, [](const value_type& number){ return std::abs(number); });
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Square root -- return the positive square root value of every element
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

sqrt(const MatrixType& m)
{
    using value_type = typename std::remove_reference<decltype(m(0,0))>::type;

    return ElementByElementUnaryExpression<MatrixType>(m, [](const value_type& number){ return std::sqrt(number); });
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// e^x -- For every element x, we make the new element e^x
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

exp(const MatrixType& m)
{
    using value_type = typename std::remove_reference<decltype(m(0,0))>::type;

    return ElementByElementUnaryExpression<MatrixType>(m, [](const value_type& number){ return std::exp(number); });
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// 2^x -- For every element x, we make the new element 2^x
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

exp2(const MatrixType& m)
{
    using value_type = typename std::remove_reference<decltype(m(0,0))>::type;

    return ElementByElementUnaryExpression<MatrixType>(m, [](const value_type& number){ return std::exp2(number); });
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_ELEMENT_BY_ELEMENT_UNARY_EXPRESSION_HPP_
