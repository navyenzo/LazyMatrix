//-------------------------------------------------------------------
/**
 * @file diff.hpp
 * @brief Functionality for first-order difference calculation in matrices.
 *
 * This file is part of the LazyMatrix library. It provides the Difference class
 * which is used for computing first-order differences in matrices. This functionality
 * mimics Matlab's diff function, offering the ability to compute differences along
 * specified dimensions (rows or columns). It's useful in various fields such as signal
 * processing, numerical analysis, and data analysis where differences between consecutive
 * elements are of interest.
 *
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_DIFF_HPP_
#define INCLUDE_DIFF_HPP_



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
// Given a matrix return the first-order differences matrix
/**
 * @class Difference
 * @brief Computes first-order differences of a matrix.
 *
 * The Difference class is designed to calculate the first-order differences of a given matrix.
 * It supports computing differences either row-wise or column-wise. The class takes in a matrix
 * and the desired direction (rows or columns) for calculating differences. This class extends
 * the functionality of the BaseMatrix class and is templated to work with various matrix types.
 *
 * @tparam MatrixType The type of the input matrix for which differences are to be calculated.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

struct Difference : public BaseMatrix< Difference<MatrixType> >
{
    Difference<MatrixType>(const MatrixType& expression,
                           int difference_direction)
    : expression_(expression),
      difference_direction_(std::max(2, std::min(1, difference_direction)))
    {
    }
    


    uintptr_t rows()const
    {
        if(difference_direction_ == 1)
            return expression_.rows();
        
        if(expression_.rows() > 0)
            return expression_.rows() - 1;
        
        return 0;
    }

    uintptr_t columns()const
    {
        if(difference_direction_ == 2)
            return expression_.columns();
        
        if(expression_.columns() > 0)
            return expression_.columns() - 1;
        
        return 0;
    }



    const MatrixType& get_expression()const
    {
        return expression_;
    }
    
    

    decltype(auto) at_(int64_t row, int64_t column)const
    {
        if(difference_direction_ == 1)
        {
            return expression_.at(row, column + 1) - expression_.at(row, column);
        }

        return expression_.at(row + 1, column) - expression_.at(row, column);
    }



private:

    const MatrixType& expression_;
    const int difference_direction_ = 1;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< Difference<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Short-hand form function for diff
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

diff(const MatrixType& m1, int difference_direction)
{
    return Difference(m1, difference_direction);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_DIFF_HPP_
