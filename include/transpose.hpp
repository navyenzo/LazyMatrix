#ifndef INCLUDE_TRANSPOSE_HPP_
#define INCLUDE_TRANSPOSE_HPP_



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
// Take the transpose of the input matrix expression
//-------------------------------------------------------------------
template<typename MatrixType>

class Transpose : public BaseMatrix< Transpose<MatrixType> >
{
public:

    Transpose<MatrixType>(const MatrixType& expression)
    : expression_(expression)
    {
    }
    


    uintptr_t rows()const
    {
        return this->expression_.columns();
    }

    uintptr_t columns()const
    {
        return this->expression_.rows();
    }



    const MatrixType& get_expression()
    {
        return expression_;
    }



    decltype(auto) at_(const int64_t& row,const int64_t& column)const
    {
        return expression_(column, row);
    }



private:

    const MatrixType& expression_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< Transpose<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// transpose
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

transpose(const MatrixType& m)
{
    return Transpose<MatrixType>(m);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_TRANSPOSE_HPP_
