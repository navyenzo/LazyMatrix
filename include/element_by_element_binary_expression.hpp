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
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
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
 * @brief A structure to represent element-by-element binary operations between two matrices.
 *
 * This structure allows for operations like addition, subtraction, multiplication, etc., 
 * to be applied element-wise between two matrices. It uses a functional approach to define 
 * the operation that should be applied to corresponding elements of the matrices.
 *
 * @tparam ReferenceType1 Type of the first matrix (left operand).
 * @tparam ReferenceType2 Type of the second matrix (right operand).
 */
//-------------------------------------------------------------------
template<typename ReferenceType1,
         typename ReferenceType2,
         std::enable_if_t<is_matrix_reference<ReferenceType1>{}>* = nullptr,
         std::enable_if_t<is_matrix_reference<ReferenceType2>{}>* = nullptr>

class ElementByElementBinaryExpression : public BaseMatrix<ElementByElementBinaryExpression<ReferenceType1, ReferenceType2>,false>
{
public:

    // Type of value that is stored in the matrix
    using value_type = typename ReferenceType1::value_type;

    // The operation function type
    using operation_type = std::function<value_type(value_type, value_type)>;

    friend class BaseMatrix<ElementByElementBinaryExpression<ReferenceType1, ReferenceType2>,false>;

    /**
     * @brief Construct a new Element By Element Binary Expression<Reference Type 1,Reference Type 2> object
     * 
     * @param left_side_expression The input left side matrix expression.
     * @param right_side_expression The input right side matrix expression.
     * @param operation_function The function that is used to combine
     *                           values from both input matrices.
     */
    ElementByElementBinaryExpression(ReferenceType1 left_side_expression,
                                     ReferenceType2 right_side_expression,
                                     const operation_type& operation_function)
    {
        set_left_side_expression(left_side_expression);
        set_right_side_expression(right_side_expression);
        set_operation_function(operation_function);
    }

    /**
     * @brief Sets the reference to the left side matrix expression
     * @param left_side_expression Reference to the left side matrix.
     */
    void set_left_side_expression(ReferenceType1 left_side_expression)
    {
        left_side_expression_ = left_side_expression;
    }

    /**
     * @brief Sets the reference to the right side matrix expression
     * @param right_side_expression Reference to the right side matrix.
     */
    void set_right_side_expression(ReferenceType2 right_side_expression)
    {
        right_side_expression_ = right_side_expression;
    }

    /**
     * @brief Set the operation function object
     * 
     * @param operation_function The function used to calculate the output
     */
    void set_operation_function(const operation_type& operation_function)
    {
        operation_function_ = operation_function;
    }

    /**
     * @brief Returns the number of rows in the resulting matrix.
     *        - This function assumes both expressions are the same size.
     * @return The number of rows of the resulting matrix.
     */
    uintptr_t rows()const
    {
        return this->left_side_expression_.rows();
    }

    /**
     * @brief Returns the number of columns in the resulting matrix.
     *        - This function assumes both expressions are the same size.
     * @return The number of columns of the resulting matrix.
     */
    uintptr_t columns()const
    {
        return this->left_side_expression_.columns();
    }



private: // Private functions

    /**
     * @brief Dummy "resize" function needed for the matrix interface, but
     *        here it doesn't do anything
     * 
     * @param rows 
     * @param columns 
     * @return std::error_code 
     */
    std::error_code resize_(uintptr_t rows, uintptr_t columns)
    {
        return std::error_code();
    }

    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A copy of the value of the element at the specified position.
     */
    value_type const_at_(int row, int column)const
    {
        return static_cast<value_type>(operation_function_(this->left_side_expression_.at(row,column),
                                                           this->right_side_expression_.at(row,column)));
    }



private: // Private variables

    ReferenceType1 left_side_expression_;
    ReferenceType2 right_side_expression_;
    operation_type operation_function_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename ReferenceType1,
         typename ReferenceType2>

struct is_type_a_matrix< ElementByElementBinaryExpression<ReferenceType1, ReferenceType2> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Addition.
 * @tparam ReferenceType1 Type of the left side matrix.
 * @tparam ReferenceType2 Type of the right side matrix.
 * @param m1 Shared reference to the left side matrix.
 * @param m2 Shared reference to the right side matrix.
 * @return Returns a SharedMatrixRef to the addition of both
 *         input matrix expressions.
 */
//-------------------------------------------------------------------
template<typename ReferenceType1,
         typename ReferenceType2,
         std::enable_if_t<is_matrix_reference<ReferenceType1>{}>* = nullptr,
         std::enable_if_t<is_matrix_reference<ReferenceType2>{}>* = nullptr>

inline auto

operator+(ReferenceType1 m1,
          ReferenceType2 m2)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<ReferenceType1>()(0,0))>::type>::type;

    auto view = std::make_shared<ElementByElementBinaryExpression<ReferenceType1,ReferenceType2>>(m1, m2, std::plus<value_type>());

    return ConstSharedMatrixRef<ElementByElementBinaryExpression<ReferenceType1,ReferenceType2>>(view);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Subtraction.
 * @tparam ReferenceType1 Type of the left side matrix.
 * @tparam ReferenceType2 Type of the right side matrix.
 * @param m1 Shared reference to the left side matrix.
 * @param m2 Shared reference to the right side matrix.
 * @return Returns a SharedMatrixRef to the subtraction of both
 *         input matrix expressions.
 */
//-------------------------------------------------------------------
template<typename ReferenceType1,
         typename ReferenceType2,
         std::enable_if_t<is_matrix_reference<ReferenceType1>{}>* = nullptr,
         std::enable_if_t<is_matrix_reference<ReferenceType2>{}>* = nullptr>

inline auto

operator-(ReferenceType1 m1,
          ReferenceType2 m2)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<ReferenceType1>()(0,0))>::type>::type;

    auto view = std::make_shared<ElementByElementBinaryExpression<ReferenceType1,ReferenceType2>>(m1, m2, std::minus<value_type>());

    return ConstSharedMatrixRef<ElementByElementBinaryExpression<ReferenceType1,ReferenceType2>>(view);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Modulus (Element by element operator%.
 * @tparam ReferenceType1 Type of the left side matrix.
 * @tparam ReferenceType2 Type of the right side matrix.
 * @param m1 Shared reference to the left side matrix.
 * @param m2 Shared reference to the right side matrix.
 * @return Returns a SharedMatrixRef to the modulus of both
 *         input matrix expressions (m1 % m2).
 */
//-------------------------------------------------------------------
template<typename ReferenceType1,
         typename ReferenceType2,
         std::enable_if_t<is_matrix_reference<ReferenceType1>{}>* = nullptr,
         std::enable_if_t<is_matrix_reference<ReferenceType2>{}>* = nullptr>

inline auto

operator%(ReferenceType1 m1,
          ReferenceType2 m2)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<ReferenceType1>()(0,0))>::type>::type;

    auto view = std::make_shared<ElementByElementBinaryExpression<ReferenceType1,ReferenceType2>>(m1, m2, std::fmod<value_type>());

    return ConstSharedMatrixRef<ElementByElementBinaryExpression<ReferenceType1,ReferenceType2>>(view);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Element by element multiplication (m1 .* m2)
 * @tparam ReferenceType1 Type of the left side matrix.
 * @tparam ReferenceType2 Type of the right side matrix.
 * @param m1 Shared reference to the left side matrix.
 * @param m2 Shared reference to the right side matrix.
 * @return Returns a SharedMatrixRef to the element by element
 *         multiplication of both matrix expressions (m1 .* m2).
 */
//-------------------------------------------------------------------
template<typename ReferenceType1,
         typename ReferenceType2,
         std::enable_if_t<is_matrix_reference<ReferenceType1>{}>* = nullptr,
         std::enable_if_t<is_matrix_reference<ReferenceType2>{}>* = nullptr>

inline auto

elem_by_elem_multiply(ReferenceType1 m1,
                      ReferenceType2 m2)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<ReferenceType1>()(0,0))>::type>::type;

    auto view = std::make_shared<ElementByElementBinaryExpression<ReferenceType1,ReferenceType2>>(m1, m2, std::multiplies<value_type>());

    return ConstSharedMatrixRef<ElementByElementBinaryExpression<ReferenceType1,ReferenceType2>>(view);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Element by element division (m1 ./ m2)
 * @tparam ReferenceType1 Type of the left side matrix.
 * @tparam ReferenceType2 Type of the right side matrix.
 * @param m1 Shared reference to the left side matrix.
 * @param m2 Shared reference to the right side matrix.
 * @return Returns a SharedMatrixRef to the element by element
 *         division of both matrix expressions (m1 ./ m2).
 */
//-------------------------------------------------------------------
template<typename ReferenceType1,
         typename ReferenceType2,
         std::enable_if_t<is_matrix_reference<ReferenceType1>{}>* = nullptr,
         std::enable_if_t<is_matrix_reference<ReferenceType2>{}>* = nullptr>

inline auto

elem_by_elem_divide(ReferenceType1 m1,
                    ReferenceType2 m2)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<ReferenceType1>()(0,0))>::type>::type;

    auto view = std::make_shared<ElementByElementBinaryExpression<ReferenceType1,ReferenceType2>>(m1, m2, std::divides<value_type>());

    return ConstSharedMatrixRef<ElementByElementBinaryExpression<ReferenceType1,ReferenceType2>>(view);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Element by element power (m1 .^ m2)
 * @tparam ReferenceType1 Type of the left side matrix.
 * @tparam ReferenceType2 Type of the right side matrix.
 * @param m1 Shared reference to the left side matrix.
 * @param m2 Shared reference to the right side matrix.
 * @return Returns a SharedMatrixRef to the element by element
 *         power of both matrix expressions (m1 .^ m2).
 */
//-------------------------------------------------------------------
template<typename ReferenceType1,
         typename ReferenceType2,
         std::enable_if_t<is_matrix_reference<ReferenceType1>{}>* = nullptr,
         std::enable_if_t<is_matrix_reference<ReferenceType2>{}>* = nullptr>

inline auto

elem_by_elem_pow(ReferenceType1 m1,
                 ReferenceType2 m2)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<ReferenceType1>()(0,0))>::type>::type;

    auto view = std::make_shared<ElementByElementBinaryExpression<ReferenceType1,ReferenceType2>>(m1, m2, [](value_type a, value_type b) { return std::pow(a,b); });

    return ConstSharedMatrixRef<ElementByElementBinaryExpression<ReferenceType1,ReferenceType2>>(view);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Element by element minimums
 * @tparam ReferenceType1 Type of the left side matrix.
 * @tparam ReferenceType2 Type of the right side matrix.
 * @param m1 Shared reference to the left side matrix.
 * @param m2 Shared reference to the right side matrix.
 * @return Returns a SharedMatrixRef to the element by element
 *         minimums
 */
//-------------------------------------------------------------------
template<typename ReferenceType1,
         typename ReferenceType2,
         std::enable_if_t<is_matrix_reference<ReferenceType1>{}>* = nullptr,
         std::enable_if_t<is_matrix_reference<ReferenceType2>{}>* = nullptr>

inline auto

min(ReferenceType1 m1,
    ReferenceType2 m2)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<ReferenceType1>()(0,0))>::type>::type;

    auto view = std::make_shared<ElementByElementBinaryExpression<ReferenceType1,ReferenceType2>>(m1, m2, [](value_type a, value_type b) { return std::min(a,b); });

    return ConstSharedMatrixRef<ElementByElementBinaryExpression<ReferenceType1,ReferenceType2>>(view);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Element by element maximumns
 * @tparam ReferenceType1 Type of the left side matrix.
 * @tparam ReferenceType2 Type of the right side matrix.
 * @param m1 Shared reference to the left side matrix.
 * @param m2 Shared reference to the right side matrix.
 * @return Returns a SharedMatrixRef to the element by element
 *         maximumns
 */
//-------------------------------------------------------------------
template<typename ReferenceType1,
         typename ReferenceType2,
         std::enable_if_t<is_matrix_reference<ReferenceType1>{}>* = nullptr,
         std::enable_if_t<is_matrix_reference<ReferenceType2>{}>* = nullptr>

inline auto

max(ReferenceType1 m1,
    ReferenceType2 m2)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<ReferenceType1>()(0,0))>::type>::type;

    auto view = std::make_shared<ElementByElementBinaryExpression<ReferenceType1,ReferenceType2>>(m1, m2, [](value_type a, value_type b) { return std::max(a,b); });

    return ConstSharedMatrixRef<ElementByElementBinaryExpression<ReferenceType1,ReferenceType2>>(view);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_ELEMENT_BY_ELEMENT_BINARY_EXPRESSION_HPP_
