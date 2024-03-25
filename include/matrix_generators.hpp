//-------------------------------------------------------------------
/**
 * @file matrix_generators.hpp
 * @brief Provides matrix generation functionality for the LazyMatrix library.
 *
 * This file includes templates for generating various types of matrices like Iota, RandomMatrix,
 * and SineWaveMatrix. These generators are useful for creating specific patterns or random matrices
 * for testing, simulation, and other purposes in computational mathematics.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_MATRIX_GENERATORS_HPP_
#define INCLUDE_MATRIX_GENERATORS_HPP_



//-------------------------------------------------------------------
#include <cstdint>
#include <chrono>
#include <ctime>

#include "base_matrix.hpp"
#include "numerical_constants.hpp"
#include "shared_references.hpp"

// eigen library for fast/efficient matrix math
#include "Eigen/Eigen"

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
 * @class Iota
 * @brief Class to generate matrices with incrementally increasing values.
 *
 * @tparam DataType The data type of the matrix elements.
 */
//-------------------------------------------------------------------
template<typename DataType>

class IotaMatrix : public BaseMatrix<IotaMatrix<DataType>,false>
{
public:

    using value_type = DataType;

    friend class BaseMatrix<IotaMatrix<DataType>,false>;

    /**
     * @brief Construct a new Iota Matrix object
     * 
     * @param rows 
     * @param columns 
     * @param starting_value 
     * @param step 
     */
    IotaMatrix(int rows, int columns, DataType starting_value = static_cast<DataType>(0), DataType step = static_cast<DataType>(1))
    : rows_(rows),
      columns_(columns),
      starting_value_(starting_value),
      step_(step)
    {
    }

    /**
     * @brief Return the number of rows
     */
    uintptr_t rows()const
    {
        return rows_;
    }

    /**
     * @brief Return the number of columns
     */
    uintptr_t columns()const
    {
        return columns_;
    }

    // Functions used to handle row and column header names
    std::string get_row_header(int64_t row_index) const { return this->headers_.get_row_header(row_index); }
    std::string get_column_header(int64_t column_index) const { return this->headers_.get_column_header(column_index); }
    void set_row_header(int64_t row_index, const std::string& row_header) const { this->headers_.set_row_header(row_index, row_header); }
    void set_column_header(int64_t column_index, const std::string& column_header) const { this->headers_.set_column_header(column_index, column_header); }



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
     * @brief Return the generated value at the (row,column) position
     * @param row 
     * @param column 
     * @return DataType 
     */
    DataType const_at_(int64_t row, int64_t column)const
    {
        return static_cast<DataType>(row * this->columns() + column) * step_ + starting_value_;
    }



private: // Private variables

    uintptr_t rows_ = 0;
    uintptr_t columns_ = 0;

    DataType starting_value_ = static_cast<DataType>(0);
    DataType step_ = static_cast<DataType>(1);
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename DataType>

struct is_type_a_matrix< IotaMatrix<DataType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class RandomMatrix
 * @brief Class to generate matrices with random values.
 *
 * @tparam DataType The data type of the matrix elements.
 */
//-------------------------------------------------------------------
template<typename DataType>

class RandomMatrix : public BaseMatrix<RandomMatrix<DataType>,false>
{
public:

    using value_type = DataType;

    friend class BaseMatrix<RandomMatrix<DataType>,false>;

    /**
     * @brief Construct a new Random Matrix object
     * 
     * @param rows 
     * @param columns 
     * @param min_value 
     * @param max_value 
     * @param steps 
     */
    RandomMatrix(int rows, int columns, DataType min_value, DataType max_value, int64_t steps = 4294967296)
    : rows_(rows),
      columns_(columns),
      min_value_(min_value),
      max_value_(max_value),
      steps_(steps)
    {
    }

    /**
     * @brief Return the number of rows
     */
    uintptr_t rows()const
    {
        return rows_;
    }

    /**
     * @brief Return the number of columns
     */
    uintptr_t columns()const
    {
        return columns_;
    }

    // Functions used to handle row and column header names
    std::string get_row_header(int64_t row_index) const { return this->headers_.get_row_header(row_index); }
    std::string get_column_header(int64_t column_index) const { return this->headers_.get_column_header(column_index); }
    void set_row_header(int64_t row_index, const std::string& row_header) const { this->headers_.set_row_header(row_index, row_header); }
    void set_column_header(int64_t column_index, const std::string& column_header) const { this->headers_.set_column_header(column_index, column_header); }



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
     * @brief Return the generated value at the (row,column) position
     * @param row 
     * @param column 
     * @return DataType 
     */
    DataType const_at_(int64_t row, int64_t column)const
    {
        static XoshiroCpp::Xoshiro256PlusPlus rng(time(NULL));

        return min_value_ + (double(rng() % (steps_ + 1)) / double(steps_)) * double(max_value_ - min_value_);
    }



private: // Private variables

    uintptr_t rows_ = 0;
    uintptr_t columns_ = 0;

    DataType min_value_ = 0;
    DataType max_value_ = 1;
    int64_t steps_ = 4294967296;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename DataType>

struct is_type_a_matrix< RandomMatrix<DataType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class SineWaveMatrix
 * @brief Class to generate matrices representing a sine wave.
 *
 * @tparam DataType The data type of the matrix elements.
 */
//-------------------------------------------------------------------
template<typename DataType>

class SineWaveMatrix : public BaseMatrix<SineWaveMatrix<DataType>,false>
{
public:

    using value_type = DataType;

    friend class BaseMatrix<SineWaveMatrix<DataType>,false>;

    /**
     * @brief Construct a new Sine Wave Matrix object
     * 
     * @param number_of_data_points 
     * @param amplitude 
     * @param frequency 
     * @param phase_offset_in_radians 
     * @param y_offset 
     * @param delta_time 
     * @param initial_time 
     */
    SineWaveMatrix(int number_of_data_points,
                   DataType amplitude = 1,
                   DataType frequency = 1,
                   DataType phase_offset_in_radians = 0,
                   DataType y_offset = 0,
                   DataType delta_time = 0.1,
                   DataType initial_time = 0)
    : rows_(number_of_data_points),
      columns_(1),
      amplitude_(amplitude),
      frequency_(frequency),
      phase_offset_in_radians_(phase_offset_in_radians),
      y_offset_(y_offset),
      delta_time_(delta_time),
      initial_time_(initial_time)
    {
    }

    /**
     * @brief Return the number of rows
     */
    uintptr_t rows()const
    {
        return rows_;
    }

    /**
     * @brief Return the number of columns
     */
    uintptr_t columns()const
    {
        return columns_;
    }

    // Functions used to handle row and column header names
    std::string get_row_header(int64_t row_index) const { return this->headers_.get_row_header(row_index); }
    std::string get_column_header(int64_t column_index) const { return this->headers_.get_column_header(column_index); }
    void set_row_header(int64_t row_index, const std::string& row_header) const { this->headers_.set_row_header(row_index, row_header); }
    void set_column_header(int64_t column_index, const std::string& column_header) const { this->headers_.set_column_header(column_index, column_header); }



private: // Private function

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
     * @brief Return the generated value at the (row,column) position
     * @param row 
     * @param column 
     * @return DataType 
     */
    DataType const_at_(int64_t row, int64_t column)const
    {
        DataType time = initial_time_ + DataType(row * this->columns() + column) * delta_time_;

        DataType value = amplitude_ * std::sin(DataType(2) * LazyMatrix::PI * frequency_ * time + phase_offset_in_radians_) + y_offset_;

        return value;
    }



private: // Private variables

    uintptr_t rows_ = 0;
    uintptr_t columns_ = 0;

    DataType amplitude_ = 1;
    DataType frequency_ = 1;
    DataType phase_offset_in_radians_ = 0;
    DataType y_offset_ = 0;
    DataType delta_time_ = 0.1;
    DataType initial_time_ = 0;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename DataType>

struct is_type_a_matrix< SineWaveMatrix<DataType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Generates an Iota matrix object.
 * @tparam DataType The type of value inside the generated matrix.
 * @param rows The number of rows the generated matrix is supposed to have.
 * @param columns The number of columns the generated matrix is supposed to have.
 * @param starting_value The starting value for the iota formula.
 * @param step The change between two consecutive values in the generated matrix.
 * @return A SharedMatrixRef to the IotaMatrix object.
 */
//-------------------------------------------------------------------
template<typename DataType>

inline auto

generate_iota_matrix(int rows,
                     int columns,
                     DataType starting_value = static_cast<DataType>(0),
                     DataType step = static_cast<DataType>(1))
{
    auto view = std::make_shared<IotaMatrix<DataType>>(rows, columns, starting_value, step);
    return ConstSharedMatrixRef<IotaMatrix<DataType>>(view);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Generates a matrix whose values are random. Its values change
 *        every time the user asks for them. For example asking for value
 *        m(3,4) will return a different value every time.
 * @tparam DataType The type of value inside the generated matrix.
 * @param rows The number of rows the generated matrix is supposed to have.
 * @param columns The number of columns the generated matrix is supposed to have.
 * @param min_value The minimum bound of the generated random values.
 * @param max_value The maximum bound of the generated random values.
 * @param steps The value used in the generation of the random values.
 * @return A SharedMatrixRef to the RandomMatrix object.
 */
//-------------------------------------------------------------------
template<typename DataType>

inline auto

generate_random_matrix(int rows,
                       int columns,
                       DataType min_value,
                       DataType max_value,
                       int64_t steps = 4294967296)
{
    auto view = std::make_shared<RandomMatrix<DataType>>(rows, columns, min_value, max_value, steps);
    return ConstSharedMatrixRef<RandomMatrix<DataType>>(view);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Generates a single column vector whose values form a sine wave.
 * @tparam DataType The type of value inside the generated matrix.
 * @param number_of_data_points The number of rows (number of samples).
 * @param amplitude The amplitude of the sine wave.
 * @param frequency The frequency of the sine wave.
 * @param phase_offset_in_radians The phase offset of the sine wave (pi/2 = cosine wave).
 * @param y_offset The y-offset (move wave up and down).
 * @param delta_time The sampling period.
 * @param initial_time Initial time used to generate sine wave.
 * @return A SharedMatrixRef to the SineWaveMatrix object.
 */
//-------------------------------------------------------------------
template<typename DataType>

inline auto

generate_sine_wave_matrix(int number_of_data_points,
                          DataType amplitude = 1,
                          DataType frequency = 1,
                          DataType phase_offset_in_radians = 0,
                          DataType y_offset = 0,
                          DataType delta_time = 0.1,
                          DataType initial_time = 0)
{
    auto view = std::make_shared<SineWaveMatrix<DataType>>(number_of_data_points,
                                                           amplitude,
                                                           frequency,
                                                           phase_offset_in_radians,
                                                           y_offset,
                                                           delta_time,
                                                           initial_time);
                                                          
    return ConstSharedMatrixRef<SineWaveMatrix<DataType>>(view);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_MATRIX_GENERATORS_HPP_
