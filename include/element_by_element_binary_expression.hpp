//-------------------------------------------------------------------
/**
 * @file element_by_element_binary_expression.hpp
 * @brief Provides element-by-element binary operations for matrices.
 *
 * This file is part of the LazyMatrix library. It includes a template structure and functions to perform 
 * element-by-element binary operations such as addition, subtraction, multiplication, etc., on matrices. 
 * These operations are fundamental for matrix-based calculations and analyses.
 *
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_ELEMENT_BY_ELEMENT_BINARY_EXPRESSION_HPP_
#define INCLUDE_ELEMENT_BY_ELEMENT_BINARY_EXPRESSION_HPP_



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
 * @brief A structure to represent element-by-element binary operations between two matrices.
 *
 * This structure allows for operations like addition, subtraction, multiplication, etc., 
 * to be applied element-wise between two matrices. It uses a functional approach to define 
 * the operation that should be applied to corresponding elements of the matrices.
 *
 * @tparam MatrixType1 Type of the first matrix (left operand).
 * @tparam MatrixType2 Type of the second matrix (right operand).
 */
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2>

struct ElementByElementBinaryExpression : public BaseMatrix< ElementByElementBinaryExpression<MatrixType1, MatrixType2> >
{
    // Type of value that is stored in left side expression
    // - We use that type to return the same type for the
    //   resulting binary operation (i.e. m1 + m2 returns type of m1(0,0))
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType1>()(0,0))>::type;

    // The operation function type
    using operation_type = std::function<value_type(value_type, value_type)>;



    ElementByElementBinaryExpression<MatrixType1, MatrixType2>(const MatrixType1& left_side_expression,
                                                               const MatrixType2& right_side_expression,
                                                               const operation_type& operation_function)
    : left_side_expression_(left_side_expression),
      right_side_expression_(right_side_expression),
      operation_function_(operation_function)
    {
    }



    // This operation assumes both expressions are the
    // same size
    uintptr_t rows()const
    {
        return this->left_side_expression_.rows();
    }

    uintptr_t columns()const
    {
        return this->left_side_expression_.columns();
    }



    const MatrixType1& get_left_side_expression()
    {
        return left_side_expression_;
    }

    const MatrixType2& get_right_side_expression()
    {
        return right_side_expression_;
    }
    
    

    decltype(auto) at_(int row, int column)const
    {
        return static_cast<value_type>(operation_function_(this->left_side_expression_.at(row,column),
                                                           this->right_side_expression_.at(row,column)));
    }



private:

    const MatrixType1& left_side_expression_;
    const MatrixType2& right_side_expression_;
    operation_type operation_function_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2>

struct is_type_a_matrix< ElementByElementBinaryExpression<MatrixType1, MatrixType2> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Addition -- operator+
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>

inline auto

operator+(const MatrixType1& m1,
          const MatrixType2& m2)
{
    using value_type = typename std::remove_reference<decltype(m1(0,0))>::type;

    return ElementByElementBinaryExpression<MatrixType1,MatrixType2>(m1,m2,std::plus<value_type>());
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Subtraction -- operator-
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>

inline auto

operator-(const MatrixType1& m1,
          const MatrixType2& m2)
{
    using value_type = typename std::remove_reference<decltype(m1(0,0))>::type;

    return ElementByElementBinaryExpression<MatrixType1,MatrixType2>(m1,m2,std::minus<value_type>());
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// element by element m1 modulus m2 -- operator%
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>

inline auto

operator%(const MatrixType1& m1,
          const MatrixType2& m2)
{
    using value_type = typename std::remove_reference<decltype(m1(0,0))>::type;

    return ElementByElementBinaryExpression<MatrixType1,MatrixType2>(m1,m2,  [](value_type a, value_type b) { return std::fmod(a,b); });
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Element by element multiplication
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>

inline auto

elem_by_elem_multiply(const MatrixType1& m1,
                      const MatrixType2& m2)
{
    using value_type = typename std::remove_reference<decltype(m1(0,0))>::type;

    return ElementByElementBinaryExpression<MatrixType1,MatrixType2>(m1,m2,std::multiplies<value_type>());
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Element by element division
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>

inline auto

elem_by_elem_divide(const MatrixType1& m1,
                    const MatrixType2& m2)
{
    using value_type = typename std::remove_reference<decltype(m1(0,0))>::type;

    return ElementByElementBinaryExpression<MatrixType1,MatrixType2>(m1,m2,std::divides<value_type>());
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// element by element pow (m1 is the base, m2 is the exponent)
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>

inline auto

elem_by_elem_pow(const MatrixType1& base,
                 const MatrixType2& exponent)
{
    using value_type = typename std::remove_reference<decltype(base(0,0))>::type;

    return ElementByElementBinaryExpression<MatrixType1,MatrixType2>(base, exponent,  [](value_type a, value_type b) { return std::pow(a,b); });
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Carries out an element by element min (i.e. min(m1,m2))
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>

inline auto

min(const MatrixType1& m1,
    const MatrixType2& m2)
{
    using value_type = typename std::remove_reference<decltype(m1(0,0))>::type;

    return ElementByElementBinaryExpression<MatrixType1,MatrixType2>(m1, m2,  [](value_type a, value_type b) { return std::min(a,b); });
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Carries out an element by element max (i.e. max(m1,m2))
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>

inline auto

max(const MatrixType1& m1,
    const MatrixType2& m2)
{
    using value_type = typename std::remove_reference<decltype(m1(0,0))>::type;

    return ElementByElementBinaryExpression<MatrixType1,MatrixType2>(m1, m2, [](value_type a, value_type b) { return std::max(a,b); });
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_ELEMENT_BY_ELEMENT_BINARY_EXPRESSION_HPP_
