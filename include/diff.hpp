//-------------------------------------------------------------------
/**
 * @file diff.hpp
 * @brief Functionality for first-order difference calculation in matrices.
 *
 * This file is part of the LazyMatrix library. It provides the Diff class
 * which is used for computing first-order differences in matrices. This functionality
 * mimics Matlab's diff function, offering the ability to compute differences along
 * specified dimensions (rows or columns). It's useful in various fields such as signal
 * processing, numerical analysis, and data analysis where differences between consecutive
 * elements are of interest.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_DIFF_HPP_
#define INCLUDE_DIFF_HPP_



//-------------------------------------------------------------------
#include "base_matrix.hpp",
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
 * @brief DiffDirection enum used to know whether to diff rows or columns.
 */
//-------------------------------------------------------------------
enum class DiffDirection : int
{
    RowDiff = 0,
    ColumnDiff = 1
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Given a matrix return the first-order differences matrix
/**
 * @class Diff
 * @brief Computes first-order differences of a matrix.
 *
 * The Diff class is designed to calculate the first-order differences of a given matrix.
 * It supports computing differences either row-wise or column-wise. The class takes in a matrix
 * and the desired direction (rows or columns) for calculating differences. This class extends
 * the functionality of the BaseMatrix class and is templated to work with various matrix types.
 *
 * @tparam ReferenceType The type of the input matrix for which differences are to be calculated.
 */
//-------------------------------------------------------------------
template<typename ReferenceType>

struct Diff : public BaseMatrix< Diff<ReferenceType> >
{
    // Type of value that is stored in the matrix
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<ReferenceType>()(0,0))>::type>::type;

    /**
     * @brief Construct a new Difference<ReferenceType> object
     * 
     * @param expression THe input matrix expression to take the diff of
     * @param diff_direction row or column diff
     */
    Diff<ReferenceType>(ReferenceType expression,
                        DiffDirection diff_direction)
    {
        set_diff_direction(diff_direction);
        set_expression(expression);
    }
    
    /**
     * @brief Sets the reference to the input matrix expression
     * @param expression Reference to the input matrix.
     */
    void set_expression(ReferenceType expression)
    {
        expression_ = expression;
    }
    
    /**
     * @brief Sets the diff direction (either diff of rows or columns)
     * @param diff_direction The direction (either rows or columns).
     */
    void set_diff_direction(DiffDirection diff_direction)
    {
        diff_direction_ = diff_direction;
    }

    /**
     * @brief Returns the number of rows of the resulting matrix.
     */
    uintptr_t rows()const
    {
        switch(diff_direction_)
        {
            default:
            case DiffDirection::RowDiff:
                return (expression_.rows() > 0) ? (expression_.rows() - 1) : 0;
            
            case DiffDirection::ColumnDiff:
                return expression_.rows();
        }
    }

    /**
     * @brief Returns the number of columns of the resulting matrix.
     */
    uintptr_t columns()const
    {
        switch(diff_direction_)
        {
            default:
            case DiffDirection::RowDiff:
                return expression_.columns();
            
            case DiffDirection::ColumnDiff:
                return (expression_.columns() > 0) ? (expression_.columns() - 1) : 0;
        }
    }
    
    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A copy of the value of the element at the specified position.
     */
    value_type at_(int64_t row, int64_t column)const
    {
        switch(diff_direction_)
        {
            default:
            case DiffDirection::RowDiff:
                return expression_.at(row, column + 1) - expression_.at(row, column);
            
            case DiffDirection::ColumnDiff:
                return expression_.at(row + 1, column) - expression_.at(row, column);
        }
    }



private:

    ReferenceType expression_;
    DiffDirection diff_direction_ = DiffDirection::RowDiff;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename ReferenceType>

struct is_type_a_matrix< Diff<ReferenceType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Creates a diff matrix from an input matrix expression by taking
 *        diff of either rows or columns as specified by the user.
 * @tparam ReferenceType Type of the input matrix.
 * @param m Shared reference to the input matrix.
 * @return A ConstSharedMatrixRef to the Difference
 *         AugmentColumnsView combining both matrices.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

inline auto

diff(ReferenceType m, DiffDirection diff_direction)
{
    auto view = std::make_shared<Diff<ReferenceType>>(m, diff_direction);
    return ConstSharedMatrixRef<Diff<ReferenceType>>(view);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_DIFF_HPP_
