#ifndef INCLUDE_INTERVAL_HPP_
#define INCLUDE_INTERVAL_HPP_



//-------------------------------------------------------------------
#include <initializer_list>
#include <algorithm>
#include <array>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Defines an interval: [min,max] together with the arithmetic
// operators
//-------------------------------------------------------------------
template<typename DataType>
class Interval
{
public:

    Interval<DataType>()
    {
    }

    Interval<DataType>(const DataType& value)
    {
        min_ = value;
        max_ = value;
    }

    Interval<DataType>(const DataType& min, const DataType& max)
    {
        if(min <= max)
        {
            min_ = min;
            max_ = max;
        }
        else
        {
            min_ = max;
            max_ = min;
        }
    }



    Interval<DataType> get_inverse()const
    {
        return Interval<DataType>(static_cast<DataType>(1) / max_,
                                  static_cast<DataType>(1) / min_);
    }
    
    
    
    Interval<DataType> operator-()const
    {
        return Interval<DataType>(-max_, -min_);
    }
    
    Interval<DataType> operator+(const Interval<DataType>& interval)const
    {
        return Interval<DataType>(min_ + interval.min(), max_ + interval.max());
    }

    Interval<DataType> operator-(const Interval<DataType>& interval)const
    {
        return Interval<DataType>(min_ - interval.max(), max_ - interval.min());
    }

    Interval<DataType> operator*(const Interval<DataType>& interval)const
    {
        std::array<DataType,4>values = {min_ * interval.min(), min_ * interval.max(), max_ * interval.min(), max_ * interval.max()};

        return Interval<DataType>(*std::min_element(values.cbegin(), values.cend()),
                                  *std::max_element(values.cbegin(), values.cend()));
    }
    
    Interval<DataType> operator/(const Interval<DataType>& interval)const
    {
        return (*this) * interval.get_inverse();
    }
    
    

    const DataType& min()const
    {
        return min_;
    }

    const DataType& max()const
    {
        return max_;
    }

    void set_min(const DataType& min)
    {
        min_ = min;

        if(min_ > max_)
            std::swap(min_, max_);
    }

    void set_max(const DataType& max)
    {
        max_ - max;

        if(max_ < min_)
            std::swap(min_, max_);
    }
    
    
    
    const Interval<DataType>& operator()(int row, int column)const
    {
        return (*this);
    }

    Interval<DataType>& operator()(int row, int column)
    {
        return (*this);
    }



private:

    DataType min_ = static_cast<DataType>(0);
    DataType max_ = static_cast<DataType>(0);
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Print to an output stream
//-------------------------------------------------------------------
template<typename DataType>

inline std::ostream&

operator<<(std::ostream& os, const Interval<DataType>& interval)
{
    os << "[" << interval.min() << "," << interval.max() << "]";

    return os;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_INTERVAL_HPP_
