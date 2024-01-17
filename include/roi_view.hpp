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
 * @tparam MatrixType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

struct ROIView : public BaseMatrix< ROIView<MatrixType> >
{
    /**
     * @brief Constructs a ROIView object from a matrix expression and coordinates.
     *
     * @param expression The matrix expression from which to create the ROI view.
     * @param row1 The starting row index of the ROI.
     * @param column1 The starting column index of the ROI.
     * @param row2 The ending row index of the ROI.
     * @param column2 The ending column index of the ROI.
     */
    ROIView<MatrixType>(MatrixType& expression,
                        int64_t row1, int64_t column1, int64_t row2, int64_t column2)
    : expression_(expression),
      row1_(row1),
      column1_(column1),
      row2_(row2),
      column2_(column2)
    {
    }
    


    uintptr_t rows()const
    {
        return std::abs(row2_ - row1_) + 1;
    }

    uintptr_t columns()const
    {
        return std::abs(column2_ - column1_) + 1;
    }



    const MatrixType& get_expression()const
    {
        return expression_;
    }
    
    

    decltype(auto) at_(int64_t row, int64_t column)const
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

    decltype(auto) at_(int64_t row, int64_t column)
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



private:

    const MatrixType& expression_;
    
    int64_t row1_ = 0;
    int64_t column1_ = 0;
    int64_t row2_ = 0;
    int64_t column2_ = 0;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< ROIView<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Create a Region Of Interest (ROI) view of a given matrix expression
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

roi_view(MatrixType& m1, int64_t row1, int64_t column1, int64_t row2, int64_t column2)
{
    return ROIView(m1, row1, column1, row2, column2);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_ROI_VIEW_HPP_
