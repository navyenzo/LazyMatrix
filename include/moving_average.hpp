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
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_MOVING_AVERAGE_HPP_
#define INCLUDE_MOVING_AVERAGE_HPP_



//-------------------------------------------------------------------
#include "base_matrix.hpp"
#include "shared_references.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief MovingAverageDirection enum used to know whether to take a
 *        moving average of rows or columns.
 */
//-------------------------------------------------------------------
enum class MovingAverageDirection : int
{
    RowAverage = 0,
    ColumnAverage = 1
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class SimpleMovingAverageOfRows
 * @brief Class to compute the simple moving average of rows in a matrix.
 *
 * @tparam ReferenceType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

struct SimpleMovingAverage : public BaseMatrix< SimpleMovingAverage<ReferenceType> >
{
    // Type of value that is stored in left side expression
    using value_type = typename ReferenceType::value_type;

    /**
     * @brief Construct a new Simple Moving Average Of Rows< Reference Type> object
     * 
     * @param expression The input matrix expression
     * @param number_of_data_points_to_average The number of data points to average
     */
    SimpleMovingAverage<ReferenceType>(ReferenceType expression,
                                             int64_t number_of_data_points_to_average,
                                             MovingAverageDirection moving_average_direction)
    {
        set_expression(expression);
        set_number_of_data_points_to_average(number_of_data_points_to_average);
        set_moving_average_direction(moving_average_direction);
    }

    /**
     * @brief Sets the reference to the input matrix expression.
     * @param left_side_expression Reference to the input matrix.
     */
    void set_expression(ReferenceType expression)
    {
        expression_ = expression;
    }

    /**
     * @brief Set the number of data points to average.
     */
    void set_number_of_data_points_to_average(int64_t number_of_data_points_to_average)
    {
        number_of_data_points_to_average_ = std::max(int64_t(1), std::abs(number_of_data_points_to_average));
    }

    /**
     * @brief Set the direction of moving average (rows or columns).
     */
    void set_moving_average_direction(MovingAverageDirection moving_average_direction)
    {
        moving_average_direction_ = moving_average_direction;
    }

    /**
     * @brief Returns the number of rows in the resulting matrix.
     */
    uintptr_t rows()const
    {
        return expression_.rows();
    }

    /**
     * @brief Returns the number of columns in the resulting matrix.
     */
    uintptr_t columns()const
    {
        return expression_.columns();
    }

    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A copy of the value of the element at the specified position.
     */
    value_type at_(int64_t row, int64_t column)const
    {
        if(moving_average_direction_ == MovingAverageDirection::RowAverage)
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
        else
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
    }



private:

    ReferenceType expression_;
    int64_t number_of_data_points_to_average_ = 1;
    MovingAverageDirection moving_average_direction_ = MovingAverageDirection::RowAverage;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename ReferenceType>

struct is_type_a_matrix< SimpleMovingAverage<ReferenceType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Creates a matrix whose elements are the moving average of
 *        either the rows or columns of the input matrix expression.
 * @tparam ReferenceType Type of the input matrix.
 * @param m Shared reference to the input matrix.
 * @return A ConstSharedMatrixRef to the SimpleMovingAverage matrix object.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

inline auto

simple_moving_average(ReferenceType m,
                      int64_t number_of_data_points_to_average,
                      MovingAverageDirection moving_average_direction)
{
    auto view = std::make_shared<SimpleMovingAverage<ReferenceType>>(m, number_of_data_points_to_average, moving_average_direction);
    return ConstSharedMatrixRef<SimpleMovingAverage<ReferenceType>>(view);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_MOVING_AVERAGE_HPP_
