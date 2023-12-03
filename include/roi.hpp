//-------------------------------------------------------------------
/**
 * @file roi.hpp
 * @brief Provides functionality to select a Region Of Interest (ROI) in matrices.
 *
 * This file contains the ROI template class, allowing the selection of a specific
 * sub-region within a matrix. The ROI class is useful for focusing on a particular
 * area of a matrix for further processing or analysis.
 *
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_ROI_HPP_
#define INCLUDE_ROI_HPP_



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
 * @class ROI
 * @brief Class to create a view representing a Region Of Interest within a matrix.
 *
 * @tparam MatrixType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

struct ROI : public BaseMatrix< ROI<MatrixType> >
{
    /**
     * @brief Constructs a ROI object from a matrix expression and coordinates.
     *
     * @param expression The matrix expression from which to create the ROI.
     * @param row1 The starting row index of the ROI.
     * @param column1 The starting column index of the ROI.
     * @param row2 The ending row index of the ROI.
     * @param column2 The ending column index of the ROI.
     */
    ROI<MatrixType>(const MatrixType& expression,
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



private:

    const MatrixType& expression_;
    
    int row1_ = 0;
    int column1_ = 0;
    int row2_ = 0;
    int column2_ = 0;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< ROI<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Create a Region Of Interest (ROI) of a given matrix expression
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

roi(const MatrixType& m1, int row1, int column1, int row2, int column2)
{
    return ROI(m1, row1, column1, row2, column2);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_AUGMENT_ROWS_HPP_
