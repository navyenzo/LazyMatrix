//-------------------------------------------------------------------
/**
 * @file shuffle.hpp
 * @brief Provides functionality to shuffle rows and/or columns of matrices.
 *
 * This file contains the template for Shuffle, which allows users to shuffle rows, columns, or both
 * of a given matrix expression. The shuffling is done in a random manner using the Xoshiro256PlusPlus
 * random number generator from the xoshiro-cpp library. This is particularly useful in data processing
 * and machine learning applications where random sampling or permutation is required.
 *
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_SHUFFLE_VIEW_HPP_
#define INCLUDE_SHUFFLE_VIEW_HPP_



//-------------------------------------------------------------------
#include <vector>
#include <numeric>
#include <algorithm>

#include "base_matrix.hpp"

// xoshiro-cpp library for random number generators
#include "XoshiroCpp.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class ShuffleView
 * @brief Class for shuffling rows and/or columns of a matrix expression.
 *
 * @tparam MatrixType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

class ShuffleView : public BaseMatrix< ShuffleView<MatrixType> >
{
public:

    ShuffleView<MatrixType>(MatrixType& expression, bool should_rows_be_shuffled, bool should_columns_be_shuffled)
    : expression_(expression),
      should_rows_be_shuffled_(should_rows_be_shuffled),
      should_columns_be_shuffled_(should_columns_be_shuffled)
    {
        row_indeces_.resize(expression.rows());
        std::iota(row_indeces_.begin(), row_indeces_.end(), 0);

        column_indeces_ = row_indeces_;

        static XoshiroCpp::Xoshiro256PlusPlus rng(time(NULL));

        if(should_rows_be_shuffled_)
            std::shuffle(row_indeces_.begin(), row_indeces_.end(), rng);

        if(should_columns_be_shuffled_)
            std::shuffle(column_indeces_.begin(), column_indeces_.end(), rng);
    }
    


    uintptr_t rows()const
    {
        return this->expression_.rows();
    }

    uintptr_t columns()const
    {
        return this->expression_.columns();
    }



    const MatrixType& get_expression()
    {
        return expression_;
    }



    decltype(auto) at_(int64_t row, int64_t column)const
    {
        return expression_.circ_at(row_indeces_[row], column_indeces_[column]);
    }

    decltype(auto) at_(int64_t row, int64_t column)
    {
        return expression_.circ_at(row_indeces_[row], column_indeces_[column]);
    }



private:

    MatrixType& expression_;
    
    bool should_rows_be_shuffled_ = true;
    bool should_columns_be_shuffled_ = true;

    std::vector<int64_t> row_indeces_;
    std::vector<int64_t> column_indeces_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< ShuffleView<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// reverse function
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

shuffle_view(MatrixType& m, bool should_rows_be_reversed, bool should_columns_be_reversed)
{
    return ShuffleView<MatrixType>(m, should_rows_be_reversed, should_columns_be_reversed);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_SHUFFLE_HPP_
