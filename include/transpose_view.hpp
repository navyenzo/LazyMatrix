#ifndef INCLUDE_TRANSPOSE_VIEW_HPP_
#define INCLUDE_TRANSPOSE_VIEW_HPP_



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
// Take the transpose view of the input matrix expression
// - The view allows to also edit the original data
//-------------------------------------------------------------------
template<typename MatrixType>

class TransposeView : public BaseMatrix< TransposeView<MatrixType> >
{
public:

    TransposeView<MatrixType>(MatrixType& expression)
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



    decltype(auto) at_(int64_t row, int64_t column)const
    {
        return expression_(column, row);
    }

    decltype(auto) at_(int64_t row, int64_t column)
    {
        return expression_(column, row);
    }



    const MatrixType& get_expression()
    {
        return expression_;
    }



private:

    MatrixType& expression_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< TransposeView<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// transpose
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

transpose_view(MatrixType& m)
{
    return TransposeView<MatrixType>(m);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_TRANSPOSE_VIEW_HPP_
