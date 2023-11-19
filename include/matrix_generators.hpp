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
// Used to generate values on the fly (deterministically or randomly)
//-------------------------------------------------------------------
template<typename GeneratorType>

class MatrixGenerator : public BaseMatrix< MatrixGenerator<GeneratorType> >
{
public:

    MatrixGenerator<GeneratorType>(int rows, int columns)
    : BaseMatrix< MatrixGenerator<GeneratorType> >(),
      rows_(rows),
      columns_(columns)
    {
    }

    int64_t rows()const
    {
        return rows_;
    }

    int64_t columns()const
    {
        return columns_;
    }

    decltype(auto) at(int row, int column)const
    {
        return static_cast<const GeneratorType&>(*this).at(row, column);
    }

private:

    int64_t rows_ = 0;
    int64_t columns_ = 0;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename GeneratorType>

struct is_type_a_matrix< MatrixGenerator<GeneratorType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Iota matrix generator
//-------------------------------------------------------------------
template<typename DataType>

class Iota : public MatrixGenerator< Iota<DataType> >
{
public:

    Iota<DataType>(int rows, int columns, DataType starting_value = static_cast<DataType>(0), DataType step = static_cast<DataType>(1))
    : MatrixGenerator< Iota<DataType> >(rows, columns),
      starting_value_(starting_value),
      step_(step)
    {
    }

    DataType at(int row, int column)const
    {
        return static_cast<DataType>(row * this->columns() + column) * step_ + starting_value_;
    }

private:

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
// Random matrix generator for integer matrices
//-------------------------------------------------------------------
template<typename DataType>

class RandomMatrix : public MatrixGenerator< RandomMatrix<DataType> >
{
public:

    RandomMatrix<DataType>(int rows, int columns, DataType min_value, DataType max_value, int64_t steps = 4294967296)
    : MatrixGenerator< RandomMatrix<DataType> >(rows, columns),
      min_value_(min_value),
      max_value_(max_value),
      steps_(steps)
    {
    }

    DataType at(int row, int column)const
    {
        static XoshiroCpp::Xoshiro256PlusPlus rng(time(NULL));

        return min_value_ + (double(rng() % (steps_ + 1)) / double(steps_)) * double(max_value_ - min_value_);
    }

private:

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
// Sine wave matrix generator
//-------------------------------------------------------------------
template<typename DataType>

class SineWaveMatrix : public MatrixGenerator< SineWaveMatrix<DataType> >
{
public:

    SineWaveMatrix<DataType>(int number_of_data_points,
                             DataType amplitude = 1,
                             DataType frequency = 1,
                             DataType phase_offset_in_radians = 0,
                             DataType y_offset = 0,
                             DataType delta_time = 0.1,
                             DataType initial_time = 0)
    : MatrixGenerator< SineWaveMatrix<DataType> >(number_of_data_points, 1),
      amplitude_(amplitude),
      frequency_(frequency),
      phase_offset_in_radians_(phase_offset_in_radians),
      y_offset_(y_offset),
      delta_time_(delta_time),
      initial_time_(initial_time)
    {
    }

    DataType at(int row, int column)const
    {
        DataType time = initial_time_ + DataType(row * this->columns() + column) * delta_time_;

        DataType value = amplitude_ * std::sin(DataType(2) * LazyMatrix::PI * frequency_ * time + phase_offset_in_radians_) + y_offset_;

        return value;
    }

private:

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
