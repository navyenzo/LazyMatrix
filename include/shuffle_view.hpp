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
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_SHUFFLE_VIEW_HPP_
#define INCLUDE_SHUFFLE_VIEW_HPP_



//-------------------------------------------------------------------
#include <vector>
#include <numeric>
#include <algorithm>

#include "base_matrix.hpp"
#include "shared_references.hpp"

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
 * @class ShuffledView
 * @brief Class for shuffling rows and/or columns of an input matrix expression.
 *
 * @tparam ReferenceType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

class ShuffledView : public BaseMatrix< ShuffledView<ReferenceType> >
{
public:

    // Type of value that is stored in the expression
    using value_type = typename ReferenceType::value_type;

    friend class BaseMatrix< ShuffledView<ReferenceType> >;

    /**
     * @brief Construct a new Shuffle object
     * 
     * @param expression The input matrix expression
     * @param should_rows_be_shuffled Boolean to know whether to shuffle rows
     * @param should_columns_be_shuffled Boolean to know whether to shuffle columns
     */
    ShuffledView(ReferenceType& expression, bool should_rows_be_shuffled, bool should_columns_be_shuffled)
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

    /**
     * @brief Return the number of rows of the resulting matrix.
     */
    uintptr_t rows()const
    {
        return this->expression_.rows();
    }

    /**
     * @brief Return the number of columns of the resulting matrix.
     */
    uintptr_t columns()const
    {
        return this->expression_.columns();
    }



private: // Private functions

    /**
     * @brief Dummy "resize" function needed for the matrix interface, but
     *        here it doesn't do anything
     * 
     * @param rows 
     * @param columns 
     * @return std::error_code 
     */
    std::error_code resize_(uintptr_t rows, uintptr_t columns)
    {
        return std::error_code();
    }

    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A copy of the value of the element at the specified position.
     */
    value_type const_at_(int64_t row, int64_t column)const
    {
        return expression_.circ_at(row_indeces_[row], column_indeces_[column]);
    }

    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A reference to the element at the specified position.
     */
    value_type& non_const_at_(int64_t row, int64_t column)
    {
        return expression_.circ_at(row_indeces_[row], column_indeces_[column]);
    }



private: // Private variables

    ReferenceType expression_;
    
    bool should_rows_be_shuffled_ = true;
    bool should_columns_be_shuffled_ = true;

    std::vector<int64_t> row_indeces_;
    std::vector<int64_t> column_indeces_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename ReferenceType>

struct is_type_a_matrix< ShuffledView<ReferenceType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// helper function
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Creates a shuffled view of a matrix (rows, columns or both).
 * @tparam ReferenceType Type of the input matrix expression.
 * @param m Shared reference to the input matrix expression
 * @param should_rows_be_shuffled Whether it should shuffle rows.
 * @param should_columns_be_shuffled Whether it should shuffle columns.
 * @return A SharedMatrixRef to the shuffled view of the input matrix expression.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

inline auto

create_shuffled_matrix_view(ReferenceType m,
                            bool should_rows_be_shuffled,
                            bool should_columns_be_shuffled)
{
    auto view = std::make_shared<ShuffledView<ReferenceType>>(m, should_rows_be_shuffled, should_columns_be_shuffled);
    return SharedMatrixRef<ShuffledView<ReferenceType>>(view);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_SHUFFLE_HPP_
