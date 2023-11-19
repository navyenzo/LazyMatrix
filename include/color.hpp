#ifndef INCLUDE_COLOR_HPP_
#define INCLUDE_COLOR_HPP_



#include <algorithm>
#include <complex>
#include <type_traits>



//-------------------------------------------------------------------
// File: color.h
// Description: Defines a series of color classes (Color1, Color2, Color3, Color4)
// for use in image handling, analyzing, and parsing.
//-------------------------------------------------------------------



namespace LazyMatrix
{



template<typename DataType>
struct is_type_a_color : std::false_type {};

template<typename DataType>
struct is_type_a_color1 : std::false_type {};

template<typename DataType>
struct is_type_a_color2 : std::false_type {};

template<typename DataType>
struct is_type_a_color3 : std::false_type {};

template<typename DataType>
struct is_type_a_color4 : std::false_type {};



//-------------------------------------------------------------------
template<typename DataType>
class Color1
{
public:

    Color1(DataType red = DataType(0), DataType green = DataType(0), DataType blue = DataType(0), DataType alpha = DataType(1))
    {
        set_red(alpha);
    }

    DataType get_red() const { return red_; }
    void set_red(DataType value) { red_ = this->clip_value(value); }

    DataType get_green() const { return DataType(0); }
    void set_green(DataType value) {}
    
    DataType get_blue() const { return DataType(0); }
    void set_blue(DataType value) {}
    
    DataType get_alpha() const { return DataType(0); }
    void set_alpha(DataType value) {}

    int get_number_of_channels()const { return 1; }



protected:

    static DataType clip_value(DataType value)
    {
        if constexpr (std::is_same_v<float, DataType> || std::is_same_v<double, DataType>)
        {
            return std::min(std::max(value, DataType(0)), DataType(1));
        }
        else
        {
            return value;
        }
    }



    DataType red_ = DataType(0);
};
//-------------------------------------------------------------------



template<typename DataType>
struct is_type_a_color<Color1<DataType>> : std::true_type {};

template<typename DataType>
struct is_type_a_color1<Color1<DataType>> : std::true_type {};



//-------------------------------------------------------------------
template<typename DataType>
class Color2 : public Color1<DataType>
{
public:

    Color2(DataType red = DataType(0), DataType green = DataType(0), DataType blue = DataType(0), DataType alpha = DataType(1))
           : Color1<DataType>(red, green, blue, alpha)
    {
        set_green(green);
    }

    DataType get_green() const { return green_; }
    void set_green(DataType value) { green_ = this->clip_value(value); }

    int get_number_of_channels()const { return 2; }

protected:

    DataType green_ = DataType(0);
};
//-------------------------------------------------------------------



template<typename DataType>
struct is_type_a_color<Color2<DataType>> : std::true_type {};

template<typename DataType>
struct is_type_a_color2<Color2<DataType>> : std::true_type {};



//-------------------------------------------------------------------
template<typename DataType>
class Color3 : public Color2<DataType>
{
public:

    Color3(DataType red = DataType(0), DataType green = DataType(0), DataType blue = DataType(0), DataType alpha = DataType(1))
           : Color2<DataType>(red, green, blue, alpha)
    {
        set_blue(blue);
    }

    DataType get_blue() const { return blue_; }
    void set_blue(DataType value) { blue_ = this->clip_value(value); }

    int get_number_of_channels()const { return 3; }

protected:

    DataType blue_ = DataType(0);
};
//-------------------------------------------------------------------



template<typename DataType>
struct is_type_a_color<Color3<DataType>> : std::true_type {};

template<typename DataType>
struct is_type_a_color3<Color3<DataType>> : std::true_type {};



//-------------------------------------------------------------------
template<typename DataType>
class Color4 : public Color3<DataType>
{
public:

    Color4(DataType red = DataType(0), DataType green = DataType(0), DataType blue = DataType(0), DataType alpha = DataType(1))
           : Color3<DataType>(red, green, blue, alpha)
    {
        set_alpha(alpha);
    }

    DataType get_alpha() const { return alpha_; }
    void set_alpha(DataType value) { alpha_ = this->clip_value(value); }

    int get_number_of_channels()const { return 4; }

protected:

    DataType alpha_ = DataType(1);
};
//-------------------------------------------------------------------



template<typename DataType>
struct is_type_a_color<Color4<DataType>> : std::true_type {};

template<typename DataType>
struct is_type_a_color4<Color4<DataType>> : std::true_type {};



//-------------------------------------------------------------------
// Operator overloads
//-------------------------------------------------------------------
template<typename ColorType1,
         typename ColorType2,
         std::enable_if_t<is_type_a_color<ColorType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_color<ColorType2>{}>* = nullptr>

inline auto

operator+(const ColorType1& c1,
          const ColorType2& c2)
{
    ColorType1 result;
    result.set_red(c1.get_red() + c2.get_red());
    result.set_green(c1.get_green() + c2.get_green());
    result.set_blue(c1.get_blue() + c2.get_blue());
    result.set_alpha(c1.get_alpha() + c2.get_alpha());

    return result;
}



template<typename ColorType1,
         typename ColorType2,
         std::enable_if_t<is_type_a_color<ColorType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_color<ColorType2>{}>* = nullptr>

inline auto

operator-(const ColorType1& c1,
          const ColorType2& c2)
{
    ColorType1 result;
    result.set_red(c1.get_red() - c2.get_red());
    result.set_green(c1.get_green() - c2.get_green());
    result.set_blue(c1.get_blue() - c2.get_blue());
    result.set_alpha(c1.get_alpha() - c2.get_alpha());

    return result;
}



template<typename ColorType1,
         typename ColorType2,
         std::enable_if_t<is_type_a_color<ColorType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_color<ColorType2>{}>* = nullptr>

inline auto

operator*(const ColorType1& c1,
          const ColorType2& c2)
{
    ColorType1 result;
    result.set_red(c1.get_red() * c2.get_red());
    result.set_green(c1.get_green() * c2.get_green());
    result.set_blue(c1.get_blue() * c2.get_blue());
    result.set_alpha(c1.get_alpha() * c2.get_alpha());

    return result;
}



template<typename ColorType1,
         typename ColorType2,
         std::enable_if_t<is_type_a_color<ColorType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_color<ColorType2>{}>* = nullptr>

inline auto

operator/(const ColorType1& c1,
          const ColorType2& c2)
{
    ColorType1 result;
    result.set_red(c1.get_red() / c2.get_red());
    result.set_green(c1.get_green() / c2.get_green());
    result.set_blue(c1.get_blue() / c2.get_blue());
    result.set_alpha(c1.get_alpha() / c2.get_alpha());

    return result;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Overloaded ostream operator
//-------------------------------------------------------------------
template<typename ColorType,
         std::enable_if_t<is_type_a_color<ColorType>{}>* = nullptr>

inline std::ostream&

operator<<(std::ostream& os, const ColorType& color)
{
    os << "(" << color.red() << "," << color.green() << "," << color.blue() << "," << color.alpha() << ")";

    return os;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function which returns the number of channels for a color
//-------------------------------------------------------------------
// Primary template: assumes T is not a complex type
template<typename T>
struct is_complex : std::false_type {};

// Specialization for std::complex
template<typename T>
struct is_complex<std::complex<T> > : std::true_type {};

// Base template for ChannelTraits
template<typename T>
struct ChannelTraits
{
    static constexpr int value = 1; // Default for non-specialized types
};

// Specialization for std::complex
template<typename T>
struct ChannelTraits<std::complex<T>>
{
    static constexpr int value = 2;
};

// Specialization for Color types
template<typename DataType>
struct ChannelTraits<Color1<DataType>>
{
    static constexpr int value = 1;
};

template<typename DataType>
struct ChannelTraits<Color2<DataType>>
{
    static constexpr int value = 2;
};

// And so on for Color3 and Color4...

// Usage function
template<typename T>
inline int get_number_of_channels()
{
    return ChannelTraits<T>::value;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to convert color to grayscale
//-------------------------------------------------------------------
template<typename ColorType,
         std::enable_if_t<is_type_a_color<ColorType>{}>* = nullptr>

inline auto convert_to_grayscale(const ColorType& color)
{
    if constexpr (is_type_a_color1<ColorType>::value)
    {
        return color.get_red();
    }
    else
    {
        return (0.299 * color.get_red() + 0.587 * color.get_green() + 0.114 * color.get_blue());
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_COLOR_HPP_
