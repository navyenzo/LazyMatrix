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
 * @link https://www.linkedin.com/in/vincenzobarbato/
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_MATRIX_GENERATORS_HPP_
#define INCLUDE_MATRIX_GENERATORS_HPP_



//-------------------------------------------------------------------
#include <time.h>
#include <cstdint>

#include "base_matrix.hpp"
#include "numerical_constants.hpp"

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

class Iota : public BaseMatrix< Iota<DataType> >
{
public:

    Iota<DataType>(int rows, int columns, DataType starting_value = static_cast<DataType>(0), DataType step = static_cast<DataType>(1))
    : BaseMatrix< Iota<DataType> >(),
      rows_(rows),
      columns_(columns),
      starting_value_(starting_value),
      step_(step)
    {
    }

    uintptr_t rows()const
    {
        return rows_;
    }

    uintptr_t columns()const
    {
        return columns_;
    }
    


    DataType at_(int64_t row, int64_t column)const
    {
        return static_cast<DataType>(row * this->columns() + column) * step_ + starting_value_;
    }



private:

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

struct is_type_a_matrix< Iota<DataType> > : std::true_type
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

class RandomMatrix : public BaseMatrix< RandomMatrix<DataType> >
{
public:

    RandomMatrix<DataType>(int rows, int columns, DataType min_value, DataType max_value, int64_t steps = 4294967296)
    : BaseMatrix< RandomMatrix<DataType> >(),
      rows_(rows),
      columns_(columns),
      min_value_(min_value),
      max_value_(max_value),
      steps_(steps)
    {
    }

    uintptr_t rows()const
    {
        return rows_;
    }

    uintptr_t columns()const
    {
        return columns_;
    }
    


    DataType at_(int64_t row, int64_t column)const
    {
        static XoshiroCpp::Xoshiro256PlusPlus rng(time(NULL));

        return min_value_ + (double(rng() % (steps_ + 1)) / double(steps_)) * double(max_value_ - min_value_);
    }



private:

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

class SineWaveMatrix : public BaseMatrix< SineWaveMatrix<DataType> >
{
public:

    SineWaveMatrix<DataType>(int number_of_data_points,
                             DataType amplitude = 1,
                             DataType frequency = 1,
                             DataType phase_offset_in_radians = 0,
                             DataType y_offset = 0,
                             DataType delta_time = 0.1,
                             DataType initial_time = 0)
    : BaseMatrix< SineWaveMatrix<DataType> >(),
      rows_(number_of_data_points),
      columns_(1),
      amplitude_(amplitude),
      frequency_(frequency),
      phase_offset_in_radians_(phase_offset_in_radians),
      y_offset_(y_offset),
      delta_time_(delta_time),
      initial_time_(initial_time)
    {
    }

    uintptr_t rows()const
    {
        return rows_;
    }

    uintptr_t columns()const
    {
        return columns_;
    }
    


    DataType at_(int64_t row, int64_t column)const
    {
        DataType time = initial_time_ + DataType(row * this->columns() + column) * delta_time_;

        DataType value = amplitude_ * std::sin(DataType(2) * LazyMatrix::PI * frequency_ * time + phase_offset_in_radians_) + y_offset_;

        return value;
    }



private:

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
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_MATRIX_GENERATORS_HPP_
