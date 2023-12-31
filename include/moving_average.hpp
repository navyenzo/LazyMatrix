//-------------------------------------------------------------------
/**
 * @file moving_average.hpp
 * @brief Provides functionality for computing simple moving averages of matrix rows or columns.
 *
 * This file contains templates for SimpleMovingAverageOfRows and SimpleMovingAverageOfColumns,
 * which enable users to compute the simple moving average across rows or columns of a given
 * matrix expression. These functionalities are useful in statistical analysis and signal processing,
 * where moving averages are commonly used to smooth data or identify trends.
 *
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_MOVING_AVERAGE_HPP_
#define INCLUDE_MOVING_AVERAGE_HPP_



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
 * @class SimpleMovingAverageOfRows
 * @brief Class to compute the simple moving average of rows in a matrix.
 *
 * @tparam MatrixType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

struct SimpleMovingAverageOfRows : public BaseMatrix< SimpleMovingAverageOfRows<MatrixType> >
{
    // Type of value that is stored in left side expression
    using value_type = typename std::remove_reference<decltype(std::declval< BaseMatrix<MatrixType> >()(0,0))>::type;



    SimpleMovingAverageOfRows<MatrixType>(const MatrixType& expression,
                                          int64_t number_of_data_points_to_average)
    : expression_(expression),
      number_of_data_points_to_average_(std::max(int64_t(1), std::abs(number_of_data_points_to_average)))
    {
    }
    


    uintptr_t rows()const
    {
        return expression_.rows();
    }

    uintptr_t columns()const
    {
        return expression_.columns();
    }



    const MatrixType& get_expression()const
    {
        return expression_;
    }



    value_type at_(int64_t row, int64_t column)const
    {
        int64_t first_column = std::max(int64_t(0), column - number_of_data_points_to_average_ + 1);

        auto avg = expression_(row, first_column);

        for(int64_t i = first_column + 1; i <= column; ++ i)
        {
            avg += expression_(row, i);
        }

        avg /= static_cast<value_type>(column + 1 - first_column);

        return avg;
    }



private:

    const MatrixType& expression_;
    
    int64_t number_of_data_points_to_average_ = 1;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< SimpleMovingAverageOfRows<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class SimpleMovingAverageOfColumns
 * @brief Class to compute the simple moving average of columns in a matrix.
 *
 * @tparam MatrixType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

struct SimpleMovingAverageOfColumns : public BaseMatrix< SimpleMovingAverageOfColumns<MatrixType> >
{
    // Type of value that is stored in left side expression
    using value_type = typename std::remove_reference<decltype(std::declval< BaseMatrix<MatrixType> >()(0,0))>::type;



    SimpleMovingAverageOfColumns<MatrixType>(const MatrixType& expression,
                                             int64_t number_of_data_points_to_average)
    : expression_(expression),
      number_of_data_points_to_average_(std::max(int64_t(1), std::abs(number_of_data_points_to_average)))
    {
    }
    


    uintptr_t rows()const
    {
        return expression_.rows();
    }

    uintptr_t columns()const
    {
        return expression_.columns();
    }



    const MatrixType& get_expression()const
    {
        return expression_;
    }



    value_type at_(int64_t row, int64_t column)const
    {
        int64_t first_row = std::max(int64_t(0), row - number_of_data_points_to_average_ + 1);

        auto avg = expression_(first_row, column);

        for(int64_t i = first_row + 1; i <= row; ++ i)
        {
            avg += expression_(i, column);
        }

        avg /= static_cast<value_type>(row + 1 - first_row);

        return avg;
    }



private:

    const MatrixType& expression_;
    
    int64_t number_of_data_points_to_average_ = 1;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< SimpleMovingAverageOfColumns<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Simple moving average of rows
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

simple_moving_average_of_rows(const MatrixType& m1, int64_t number_of_data_points_to_average)
{
    return SimpleMovingAverageOfRows<MatrixType>(m1, number_of_data_points_to_average);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Simple moving average of columns
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

simple_moving_average_of_columns(const MatrixType& m1, int64_t number_of_data_points_to_average)
{
    return SimpleMovingAverageOfColumns<MatrixType>(m1, number_of_data_points_to_average);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_MOVING_AVERAGE_HPP_
