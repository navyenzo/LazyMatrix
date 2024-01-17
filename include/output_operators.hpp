//-------------------------------------------------------------------
/**
 * @brief Output stream operators for various pixel and matrix types.
 *
 * This file contains a series of overloaded operator<< functions that enable
 * the output of different pixel types and matrix expressions to standard
 * output streams. It supports various pixel types from the dlib library and
 * custom matrix types defined in the application.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_OUTPUT_OPERATORS_HPP_
#define INCLUDE_OUTPUT_OPERATORS_HPP_



//-------------------------------------------------------------------
#include <iostream>

#include <Poco/Dynamic/Var.h>

#include "base_matrix.hpp"
#include "base_matrix3d.hpp"
#include "image_matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define the output operators within the global namespace
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Outputs a pixel value to a stream
 * 
 * @param out The output stream.
 * @param pixel The pixel to output.
 * @return std::ostream& The output stream.
 */
//-------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& out, const dlib::rgb_pixel& pixel)
{
    if constexpr (std::is_same_v<decltype(pixel.red), unsigned char>)
    {
        return out << "(" << static_cast<int>(pixel.red) << ", "
                   << static_cast<int>(pixel.green) << ", "
                   << static_cast<int>(pixel.blue) << ")";
    }
    else
    {
        return out << "(" << pixel.red << ", "
                   << pixel.green << ", "
                   << pixel.blue << ")";
    }
}

inline std::ostream& operator<<(std::ostream& out, const dlib::bgr_pixel& pixel)
{
    if constexpr (std::is_same_v<decltype(pixel.red), unsigned char>)
    {
        return out << "(" << static_cast<int>(pixel.blue) << ", "
                   << static_cast<int>(pixel.green) << ", "
                   << static_cast<int>(pixel.red) << ")";
    }
    else
    {
        return out << "(" << pixel.blue << ", "
                   << pixel.green << ", "
                   << pixel.red << ")";
    }
}

inline std::ostream& operator<<(std::ostream& out, const dlib::rgb_alpha_pixel& pixel)
{
    if constexpr (std::is_same_v<decltype(pixel.red), unsigned char>)
    {
        return out << "(" << static_cast<int>(pixel.red) << ", "
                   << static_cast<int>(pixel.green) << ", "
                   << static_cast<int>(pixel.blue) << ", "
                   << static_cast<int>(pixel.alpha) << ")";
    }
    else
    {
        return out << "(" << pixel.red << ", "
                   << pixel.green << ", "
                   << pixel.blue << ", "
                   << pixel.alpha << ")";
    }
}

inline std::ostream& operator<<(std::ostream& out, const dlib::bgr_alpha_pixel& pixel)
{
    if constexpr (std::is_same_v<decltype(pixel.red), unsigned char>)
    {
        return out << "(" << static_cast<int>(pixel.blue) << ", "
                   << static_cast<int>(pixel.green) << ", "
                   << static_cast<int>(pixel.red) << ", "
                   << static_cast<int>(pixel.alpha) << ")";
    }
    else
    {
        return out << "(" << pixel.blue << ", "
                   << pixel.green << ", "
                   << pixel.red << ", "
                   << pixel.alpha << ")";
    }
}

inline std::ostream& operator<<(std::ostream& out, const dlib::hsi_pixel& pixel)
{
    if constexpr (std::is_same_v<decltype(pixel.h), unsigned char>)
    {
        return out << "(" << static_cast<int>(pixel.h) << ", " 
                   << static_cast<int>(pixel.s) << ", " 
                   << static_cast<int>(pixel.i) << ")";
    }
    else
    {
        return out << "(" << pixel.h << ", " 
                   << pixel.s << ", " 
                   << pixel.i << ")";
    }
}

inline std::ostream& operator<<(std::ostream& out, const dlib::hsv_pixel& pixel)
{
    if constexpr (std::is_same_v<decltype(pixel.h), unsigned char>)
    {
        return out << "(" << static_cast<int>(pixel.h) << ", "
                   << static_cast<int>(pixel.s) << ", "
                   << static_cast<int>(pixel.v) << ")";
    }
    else
    {
        return out << "(" << pixel.h << ", "
                   << pixel.s << ", "
                   << pixel.v << ")";
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Utility function used to print a 2d matrix expression to an output stream
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<LazyMatrix::is_type_a_matrix<MatrixType>{}>* = nullptr>

inline std::ostream& operator<<(std::ostream& os, const MatrixType& expression)
{
    // Type of value that is stored in the expression
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type>::type;

    os << "(" << expression.rows() << "x" << expression.columns() << ")\n";

    for(int i = 0; i < expression.rows(); ++i)
    {
        for(int j = 0; j < expression.columns() - 1; ++j)
        {
            if constexpr (std::is_same_v<value_type, Poco::Dynamic::Var>)
            {
                os << expression(i,j).toString() << ",";
            }
            else
            {
                os << expression(i,j) << ",";
            }
        }

        if(expression.columns() > 0)
        {
            if constexpr (std::is_same_v<value_type, Poco::Dynamic::Var>)
            {
                os << expression(i,expression.columns() - 1).toString() << "\n";
            }
            else
            {
                os << expression(i,expression.columns() - 1) << "\n";
            }
        }
    }

    return os;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Utility function used to print a 3d expression to an output stream
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<LazyMatrix::is_type_a_matrix3d<MatrixType>{}>* = nullptr>
         
inline std::ostream& operator<<(std::ostream& os, const MatrixType& expression)
{
    // Type of value that is stored in the expression
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0,0))>::type>::type;

    os << "(" << expression.pages() << "x" << expression.rows() << "x" << expression.columns() << ")\n";

    for(int k = 0; k < expression.pages(); ++k)
    {
        os << "(" << expression.rows() << "x" << expression.columns() << ")\n";

        for(int i = 0; i < expression.rows(); ++i)
        {
            for(int j = 0; j < expression.columns() - 1; ++j)
            {
                if constexpr (std::is_same_v<value_type, Poco::Dynamic::Var>)
                {
                    os << expression(k,i,j).toString() << ",";
                }
                else
                {
                    os << expression(k,i,j) << ",";
                }
            }

            if(expression.columns() > 0)
            {
                if constexpr (std::is_same_v<value_type, Poco::Dynamic::Var>)
                {
                    os << expression(k,i,expression.columns() - 1).toString() << "\n";
                }
                else
                {
                    os << expression(k,i,expression.columns() - 1) << "\n";
                }
            }
        }
    }

    return os;
}
//-------------------------------------------------------------------



#endif  // INCLUDE_OUTPUT_OPERATORS_HPP_
