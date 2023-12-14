//-------------------------------------------------------------------
/**
 * @file padding_view.hpp
 * @brief Provides functionality for padding matrix expressions.
 *
 * This file contains the PaddedMatrix template class, which is used to pad a matrix expression 
 * with a constant value. This is useful for algorithms that require matrices with specific 
 * dimensions or for handling boundary conditions. The file also includes utility functions to 
 * facilitate the creation of padded matrices.
 *
 * @author Vincenzo Barbato
 * @contact GitHub Project: https://github.com/navyenzo/LazyMatrix.git
 *          LinkedIn: https://www.linkedin.com/in/vincenzobarbato/
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_PADDING_VIEW_HPP_
#define INCLUDE_PADDING_VIEW_HPP_



//-------------------------------------------------------------------
#include <algorithm>
#include <cstdint>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class PaddedMatrix
 * @brief Pads matrix expression with a constant value
 *
 * @tparam MatrixType Type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

struct PaddedMatrixView : public BaseMatrix< PaddedMatrixView<MatrixType> >
{
    // Type of value that is stored in the expression
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;



    PaddedMatrixView<MatrixType>(MatrixType& expression, uintptr_t padded_rows, uintptr_t padded_columns, const value_type& constant_value_for_padding = static_cast<value_type>(0))
    : expression_(expression),
      padded_rows_(padded_rows),
      padded_columns_(padded_columns),
      constant_value_for_padding_(constant_value_for_padding)
    {
    }



    uintptr_t rows()const
    {
        return padded_rows_;
    }

    uintptr_t columns()const
    {
        return padded_columns_;
    }



    const MatrixType& get_expression_()const
    {
        return expression_;
    }

    const value_type& get_constant_value_for_padding()const
    {
        return constant_value_for_padding_;
    }

    void set_constant_value(const value_type& constant_value_for_padding)
    {
        constant_value_for_padding_ = constant_value_for_padding;
    }
    


   const value_type& at_(int64_t row, int64_t column)const
    {
        if(row < 0 || row >= expression_.rows() || column < 0 || column >= expression_.columns())
            return constant_value_for_padding_;

        return expression_(row, column);
    }

    value_type& at_(int64_t row, int64_t column)
    {
        if(row < 0 || row >= expression_.rows() || column < 0 || column >= expression_.columns())
            return constant_value_for_padding_;

        return expression_(row, column);
    }



private:

    MatrixType& expression_;

    const uintptr_t padded_rows_;
    const uintptr_t padded_columns_;
    value_type constant_value_for_padding_ = static_cast<value_type>(0);
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< PaddedMatrixView<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Pad a matrix and be able to edit original source matrix
//-------------------------------------------------------------------
template<typename MatrixType,
         typename DataType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

pad_matrix_view(MatrixType& m,
                uintptr_t padded_rows,
                uintptr_t padded_columns,
                const DataType& constant_value_for_padding)
{
    return PaddedMatrixView<MatrixType>(m, padded_rows, padded_columns, constant_value_for_padding);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_PADDING_VIEW_HPP_
