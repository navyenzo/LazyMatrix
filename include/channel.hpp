#ifndef INCLUDE_CHANNEL_HPP_
#define INCLUDE_CHANNEL_HPP_



//-------------------------------------------------------------------
#include <complex>

#include "color.hpp"
#include "base_matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to return the value corresponding to a speicified
// channel
//-------------------------------------------------------------------
template<typename DataType>
inline const DataType& get_channel_value_const(const DataType& value, int channel_index)
{
    return value;
}



template<typename DataType>

inline DataType& get_channel_value(DataType& value, int channel_index)
{
    return value;
}



template<typename DataType>

inline const DataType& get_channel_value_const(const std::complex<DataType>& value, int channel_index)
{
    switch(channel_index % 2)
    {
        default:
        case 0:
            return reinterpret_cast<const DataType*>(&value)[0];
        
        case 1:
            return reinterpret_cast<const DataType*>(&value)[1];
    }
}



template<typename DataType>

inline DataType& get_channel_value(std::complex<DataType>& value, int channel_index)
{
    switch(channel_index % 2)
    {
        default:
        case 0:
            return reinterpret_cast<DataType*>(&value)[0];
        
        case 1:
            return reinterpret_cast<DataType*>(&value)[1];
    }
}



template<typename DataType>

inline const DataType& get_channel_value_const(const Color3<DataType>& value, int channel_index)
{
    switch(channel_index % 3)
    {
        default:
        case 0:
            return value.red();
        
        case 1:
            return value.green();
        
        case 2:
            return value.blue();
    }
}



template<typename DataType>

inline DataType& get_channel_value(Color3<DataType>& value, int channel_index)
{
    switch(channel_index % 3)
    {
        default:
        case 0:
            return value.red();
        
        case 1:
            return value.green();
        
        case 2:
            return value.blue();
    }
}



template<typename DataType>

inline const DataType& get_channel_value_const(const Color4<DataType>& value, int channel_index)
{
    switch(channel_index % 4)
    {
        default:
        case 0:
            return value.red();
        
        case 1:
            return value.green();
        
        case 2:
            return value.blue();

        case 3:
            return value.alpha();
    }
}



template<typename DataType>

inline DataType& get_channel_value(Color4<DataType>& value, int channel_index)
{
    switch(channel_index % 4)
    {
        default:
        case 0:
            return value.red();
        
        case 1:
            return value.green();
        
        case 2:
            return value.blue();

        case 3:
            return value.alpha();
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to convert a color to grayscale
//-------------------------------------------------------------------
template<typename DataType>

inline DataType to_grayscale(const DataType& value)
{
    return value;
}



template<typename DataType>

inline DataType to_grayscale(const Color3<DataType>& value)
{
    return value.to_grayscale();
}



template<typename DataType>

inline DataType to_grayscale(const Color4<DataType>& value)
{
    return value.to_grayscale();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Grab a channel from a multi-channel matrix
//-------------------------------------------------------------------
template<typename MatrixType>

class Channel : public BaseMatrix< Channel<MatrixType> >
{
public:

    // Type of value that is stored in left side expression
    using value_type = typename std::remove_reference<decltype(get_channel_value_const(std::declval<MatrixType>()(0,0), 0))>::type;

    Channel<MatrixType>(const MatrixType& expression, int channel_index)
    : expression_(expression),
      channel_index_(channel_index)
    {
    }
    


    int rows()const
    {
        return this->expression_.rows();
    }

    int columns()const
    {
        return this->expression_.columns();
    }



    value_type at(const int64_t& row,const int64_t& column)const
    {
        return get_channel_value_const(expression_.at(row, column), channel_index_);
    }



    const MatrixType& get_expression()
    {
        return expression_;
    }



private:

    const MatrixType& expression_;
    int channel_index_ = 0;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< Channel<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Grab a channel from a multi-channel matrix/image
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

get_channel(const MatrixType& m, int channel_index)
{
    return Channel<MatrixType>(m, channel_index);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Convert a color matrix to grayscale
//-------------------------------------------------------------------
template<typename MatrixType>

class Grayscale : public BaseMatrix< Grayscale<MatrixType> >
{
public:

    Grayscale<MatrixType>(const MatrixType& expression)
    : expression_(expression)
    {
    }
    


    int rows()const
    {
        return this->expression_.columns();
    }

    int columns()const
    {
        return this->expression_.rows();
    }



    decltype(auto) at(const int64_t& row,const int64_t& column)const
    {
        return to_grayscale(expression_.at(row, column));
    }



    const MatrixType& get_expression()
    {
        return expression_;
    }



private:

    const MatrixType& expression_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< Grayscale<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Grab a channel from a multi-channel matrix/image
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

get_grayscale(const MatrixType& m)
{
    return Grayscale<MatrixType>(m);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_CHANNEL_HPP_
