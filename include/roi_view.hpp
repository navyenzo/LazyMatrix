//-------------------------------------------------------------------
/**
 * @file roi_view.hpp
 * @brief Provides functionality to create a view representing a Region Of Interest (ROI) in matrices.
 *
 * This file contains the ROIView template class, which enables the creation of a modifiable view
 * focusing on a specific sub-region within a matrix. This class is particularly useful for
 * operations that require modifying or analyzing a particular area of a matrix without affecting
 * the rest of the matrix.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_ROI_VIEW_HPP_
#define INCLUDE_ROI_VIEW_HPP_



//-------------------------------------------------------------------
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
 * @class ROIView
 * @brief Class to create a view representing a modifiable Region Of Interest within a matrix.
 *
 * @tparam ReferenceType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

class ROIView : public BaseMatrix< ROIView<ReferenceType> >
{
public:

    // Type of value that is stored in the expression
    using value_type = typename ReferenceType::value_type;

    friend class BaseMatrix< ROIView<ReferenceType> >;

    /**
     * @brief Constructs a ROIView object from a matrix expression and coordinates.
     *
     * @param expression The matrix expression from which to create the ROI view.
     * @param row1 The starting row index of the ROI.
     * @param column1 The starting column index of the ROI.
     * @param row2 The ending row index of the ROI.
     * @param column2 The ending column index of the ROI.
     */
    ROIView(ReferenceType& expression,
            int64_t row1, int64_t column1, int64_t row2, int64_t column2)
    {
        set_expression(expression);
        set_roi_bounds(row1, column1, row2, column2);
    }

    /**
     * @brief Sets the reference to the matrix expression
     * @param left_side_expression Reference to the matrix.
     */
    void set_expression(ReferenceType expression)
    {
        expression_ = expression;
    }

    /**
     * @brief Set the roi bounds object
     * 
     * @param row1 
     * @param column1 
     * @param row2 
     * @param column2 
     */
    void set_roi_bounds(int64_t row1, int64_t column1, int64_t row2, int64_t column2)
    {
        row1_ = row1;
        column1_ = column1;
        row2_ = row2;
        column2_ = column2;
    }

    /**
     * @brief Return the number of rows of the resulting matrix
     */
    uintptr_t rows()const
    {
        return std::abs(row2_ - row1_) + 1;
    }

    /**
     * @brief Return the number of columns of the resulting matrix
     */
    uintptr_t columns()const
    {
        return std::abs(column2_ - column1_) + 1;
    }



private: // Private functions

    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A copy of the value of the element at the specified position.
     */
    value_type const_at_(int64_t row, int64_t column)const
    {
        int64_t actual_row = row1_;
        int64_t actual_column = column1_;

        if(rows() > 1)
        {
            if(row2_ > row1_)
                actual_row += row;
            else
                actual_row -= row;
        }

        if(columns() > 1)
        {
            if(column2_ > column1_)
                actual_column += column;
            else
                actual_column -= column;
        }

        return expression_.circ_at(actual_row, actual_column);
    }

    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A reference to the element at the specified position.
     */
    value_type& non_const_at_(int64_t row, int64_t column)
    {
        int64_t actual_row = row1_;
        int64_t actual_column = column1_;

        if(rows() > 1)
        {
            if(row2_ > row1_)
                actual_row += row;
            else
                actual_row -= row;
        }

        if(columns() > 1)
        {
            if(column2_ > column1_)
                actual_column += column;
            else
                actual_column -= column;
        }

        return expression_.circ_at(actual_row, actual_column);
    }



private: // private variables

    ReferenceType expression_;
    
    int64_t row1_ = 0;
    int64_t column1_ = 0;
    int64_t row2_ = 0;
    int64_t column2_ = 0;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename ReferenceType>

struct is_type_a_matrix< ROIView<ReferenceType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Creates a ROI of an input matrix (Region Of Interest).
 * @tparam ReferenceType Type of the input matrix expression.
 * @param m Shared reference to the input matrix expression
 * @param row1 first row of the ROI.
 * @param column1 first column of the ROI.
 * @param row2 second row of the ROI.
 * @param column2 second column of the ROI.
 * @return A SharedMatrixRef to the ROI matrix object.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

inline auto

create_roi_view(ReferenceType m,
                          int64_t row1, int64_t column1, int64_t row2, int64_t column2)
{
    auto view = std::make_shared<ROIView<ReferenceType>>(m, row1, column1, row2, column2);
    return SharedMatrixRef<ROIView<ReferenceType>>(view);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_ROI_VIEW_HPP_
