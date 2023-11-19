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
// Equivalent of Matlab's diff function Y = diff(X,1,dim)
// where 'dim' indicates the direction
//
// - dim -- direction of difference
//       - 1 -- rows differences -- Y(i,j) = X(i,j+1) - X(i,j)
//       - 2 -- column differences -- Y(i,j) = X(i+1,j) - X(i,j)
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
    


    int rows()const
    {
        if(difference_direction_ == 1)
            return expression_.rows();
        
        return std::max(0, expression_.rows() - 1);
    }

    int columns()const
    {
        if(difference_direction_ == 2)
            return expression_.columns();
            
        return std::max(0, expression_.columns() - 1);
    }



    decltype(auto) at(int row, int column)const
    {
        if(difference_direction_ == 1)
        {
            return expression_.at(row, column + 1) - expression_.at(row, column);
        }

        return expression_.at(row + 1, column) - expression_.at(row, column);
    }



    const MatrixType& get_expression()const
    {
        return expression_;
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
