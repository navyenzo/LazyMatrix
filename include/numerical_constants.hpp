//-------------------------------------------------------------------
/**
 * @file numerical_constants.hpp
 * @brief Defines important numerical constants for the LazyMatrix library.
 *
 * This header file defines several fundamental mathematical constants that are 
 * commonly used in various matrix operations and calculations within the 
 * LazyMatrix library.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_NUMERICAL_CONSTANTS_HPP_
#define INCLUDE_NUMERICAL_CONSTANTS_HPP_



//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Mathematical constant Pi (π).
 *
 * Pi (π) is the ratio of a circle's circumference to its diameter.
 * It's a fundamental constant in mathematics, often used in trigonometry,
 * geometry, and various calculations involving circles and periodic functions.
 */
//-------------------------------------------------------------------
const static double PI = 3.14159265358979323846;
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Mathematical constant e (Euler's number).
 *
 * Euler's number (e) is the base of the natural logarithm. It is an
 * irrational number that is crucial in calculus, particularly in exponential
 * and logarithmic functions, and it frequently appears in various mathematical
 * and physical contexts.
 */
//-------------------------------------------------------------------
const static double e = 2.71828182845904523536;
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief A template struct for holding a dummy value.
 *
 * This struct provides a static dummy value of type `T`. It is used as a fallback
 * or placeholder value. The dummy value is default-initialized.
 *
 * @tparam T The data type for which the dummy value is needed.
 */
//-------------------------------------------------------------------
template<typename T>
struct DummyValueHolder
{
    // Define a static dummy value, default-initialized
    inline static T zero{};
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_NUMERICAL_CONSTANTS_HPP_

