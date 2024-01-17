//-------------------------------------------------------------------
/**
 * @file transpose_view.hpp
 * @brief Provides the TransposeView class for transposing matrix expressions in LazyMatrix.
 *
 * This file defines the TransposeView class, which creates a view of a matrix expression
 * where the rows and columns are swapped (transposed). The view is not a copy of the data;
 * it references the original matrix, allowing changes to be reflected in the original matrix.
 * This class is particularly useful for operations that require a transpose without the cost
 * of physically rearranging the data.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



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

/**
 * @class TransposeView
 * @brief A view class that provides a transposed view of a matrix.
 *
 * TransposeView presents a transposed interface to the underlying matrix expression. It swaps
 * rows and columns, allowing for operations as if the matrix were transposed. This class does
 * not create a copy of the data, but rather provides a different view of the same data. Thus,
 * modifications through this view will affect the original matrix.
 *
 * @tparam MatrixType The type of the underlying matrix expression.
 */
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
/**
 * @brief Helper function to create a TransposeView from a matrix expression.
 *
 * This function provides an easy way to create a TransposeView of a given matrix.
 * It checks at compile time if the provided object is a matrix expression and returns
 * a TransposeView object.
 *
 * @tparam MatrixType The type of the matrix expression.
 * @param m Reference to the matrix expression.
 * @return TransposeView<MatrixType> A transposed view of the matrix expression.
 */
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
