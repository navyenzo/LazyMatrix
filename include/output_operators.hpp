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
#include "polymorphic_matrix.hpp"
#include "polymorphic_matrix3d.hpp"
#include "shared_references.hpp"
#include "eigen_wrappers.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define the output operators within the global namespace
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Outputs a dlib::rgb_pixel value to a stream
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
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Outputs a dlib::bgr_pixel value to a stream
 * 
 * @param out The output stream.
 * @param pixel The pixel to output.
 * @return std::ostream& The output stream.
 */
//-------------------------------------------------------------------
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
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Outputs a dlib::rgb_alpha_pixel value to a stream
 * 
 * @param out The output stream.
 * @param pixel The pixel to output.
 * @return std::ostream& The output stream.
 */
//-------------------------------------------------------------------
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
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Outputs a dlib::bgr_alpha_pixel value to a stream
 * 
 * @param out The output stream.
 * @param pixel The pixel to output.
 * @return std::ostream& The output stream.
 */
//-------------------------------------------------------------------
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
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Outputs a dlib::hsi_pixel value to a stream
 * 
 * @param out The output stream.
 * @param pixel The pixel to output.
 * @return std::ostream& The output stream.
 */
//-------------------------------------------------------------------
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
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Outputs a dlib::hsv_pixel value to a stream
 * 
 * @param out The output stream.
 * @param pixel The pixel to output.
 * @return std::ostream& The output stream.
 */
//-------------------------------------------------------------------
// inline std::ostream& operator<<(std::ostream& out, const dlib::hsv_pixel& pixel)
// {
//     if constexpr (std::is_same_v<decltype(pixel.h), unsigned char>)
//     {
//         return out << "(" << static_cast<int>(pixel.h) << ", "
//                    << static_cast<int>(pixel.s) << ", "
//                    << static_cast<int>(pixel.v) << ")";
//     }
//     else
//     {
//         return out << "(" << pixel.h << ", "
//                    << pixel.s << ", "
//                    << pixel.v << ")";
//     }
// }
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Prints a 2d matrix expression to an output stream.
 * 
 * @param os The output stream we're printing to.
 * @param expression The 2d matrix we're printing.
 * @return std::ostream& The output stream.
 */
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
/**
 * @brief Prints a 3d matrix expression to an output stream.
 * 
 * @param os The output stream we're printing to.
 * @param expression The 3d matrix we're printing.
 * @return std::ostream& The output stream.
 */
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



//-------------------------------------------------------------------
/**
 * @brief Prints a 2d matrix expression reference to an output stream.
 * 
 * @param os The output stream we're printing to.
 * @param expression The 2d matrix reference we're printing.
 * @return std::ostream& The output stream.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<LazyMatrix::is_matrix_reference<ReferenceType>{}>* = nullptr>

inline std::ostream& operator<<(std::ostream& os, ReferenceType expression)
{
    os << *expression;
    return os;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Prints a 3d matrix expression reference to an output stream.
 * 
 * @param os The output stream we're printing to.
 * @param expression The 3d matrix reference we're printing.
 * @return std::ostream& The output stream.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<LazyMatrix::is_matrix3d_reference<ReferenceType>{}>* = nullptr>

inline std::ostream& operator<<(std::ostream& os, ReferenceType expression)
{
    os << *expression;
    return os;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Prints an eigen wrapped 2d matrix expression reference to an output stream.
 * 
 * @param os The output stream we're printing to.
 * @param expression The eigen wrapped 2d matrix reference we're printing.
 * @return std::ostream& The output stream.
 */
//-------------------------------------------------------------------
template<typename ReferenceType>

inline std::ostream& operator<<(std::ostream& os, const LazyMatrix::EigenWrapper<ReferenceType>& expression)
{
    os << expression.get_matrix();
    return os;
}



template<typename ReferenceType>

inline std::ostream& operator<<(std::ostream& os, const LazyMatrix::ConstEigenWrapper<ReferenceType>& expression)
{
    os << expression.get_matrix();
    return os;
}
//-------------------------------------------------------------------



#endif  // INCLUDE_OUTPUT_OPERATORS_HPP_
