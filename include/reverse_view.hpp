#ifndef INCLUDE_REVERSE_VIEW_HPP_
#define INCLUDE_REVERSE_VIEW_HPP_



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
// Reverse rows and columns of an input matrix
//-------------------------------------------------------------------
template<typename MatrixType>

struct ReverseView : public BaseMatrix< ReverseView<MatrixType> >
{
    ReverseView<MatrixType>(MatrixType& expression, bool should_rows_be_reversed, bool should_columns_be_reversed)
    : expression_(expression),
      should_rows_be_reversed_(should_rows_be_reversed),
      should_columns_be_reversed_(should_columns_be_reversed)
    {
    }
    


    int rows()const
    {
        return this->expression_.rows();
    }

    int columns()const
    {
        return this->expression_.columns();
    }



    decltype(auto) at(const int64_t& row,const int64_t& column)const
    {
        int64_t actual_row = row;
        int64_t actual_column = column;

        if(should_rows_be_reversed_)
            actual_row = this->rows() - row - 1;
        
        if(should_columns_be_reversed_)
            actual_column = this->columns() - column - 1;

        return expression_(actual_row, actual_column);
    }

    decltype(auto) at(const int64_t& row,const int64_t& column)
    {
        int64_t actual_row = row;
        int64_t actual_column = column;

        if(should_rows_be_reversed_)
            actual_row = this->rows() - row - 1;
        
        if(should_columns_be_reversed_)
            actual_column = this->columns() - column - 1;

        return expression_(actual_row, actual_column);
    }



    const MatrixType& get_expression()
    {
        return expression_;
    }



private:

    MatrixType& expression_;
    bool should_rows_be_reversed_ = true;
    bool should_columns_be_reversed_ = true;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< ReverseView<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// reverse function
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

reverse_view(MatrixType& m, bool should_rows_be_reversed, bool should_columns_be_reversed)
{
    return ReverseView<MatrixType>(m, should_rows_be_reversed, should_columns_be_reversed);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_REVERSE_VIEW_HPP_
