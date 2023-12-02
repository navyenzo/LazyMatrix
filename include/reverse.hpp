#ifndef INCLUDE_REVERSE_HPP_
#define INCLUDE_REVERSE_HPP_



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

struct Reverse : public BaseMatrix< Reverse<MatrixType> >
{
    Reverse<MatrixType>(const MatrixType& expression, bool should_rows_be_reversed, bool should_columns_be_reversed)
    : expression_(expression),
      should_rows_be_reversed_(should_rows_be_reversed),
      should_columns_be_reversed_(should_columns_be_reversed)
    {
    }
    


    uintptr_t rows()const
    {
        return this->expression_.rows();
    }

    uintptr_t columns()const
    {
        return this->expression_.columns();
    }



    decltype(auto) at(int64_t row, int64_t column)const
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

    const MatrixType& expression_;
    bool should_rows_be_reversed_ = true;
    bool should_columns_be_reversed_ = true;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< Reverse<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// reverse function
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

reverse(const MatrixType& m, bool should_rows_be_reversed, bool should_columns_be_reversed)
{
    return Reverse<MatrixType>(m, should_rows_be_reversed, should_columns_be_reversed);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_REVERSE_HPP_
