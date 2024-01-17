//-------------------------------------------------------------------
/**
 * @file convert_numbers.hpp
 * @brief Provides functionality to convert strings to numbers and evaluate mathematical expressions.
 *
 * This header file contains templates and classes for converting strings to numeric types and for 
 * evaluating mathematical expressions represented as strings. It is a part of the LazyMatrix library 
 * which focuses on matrix operations and related utilities.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_CONVERT_NUMBERS_HPP_
#define INCLUDE_CONVERT_NUMBERS_HPP_



//-------------------------------------------------------------------
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Converts a substring of a string to a number.
 *
 * @tparam NumberType The numeric type to which the string will be converted.
 * @param resulting_converted_number The converted numeric result.
 * @param string_begin Pointer to the beginning of the string to be converted.
 * @param position_where_to_begin_converting Index in the string to start conversion.
 * @param position_where_to_end_converting Index in the string to end conversion.
 * @param decimal_point_delimiter Character used to denote the decimal point.
 * @return int Position in the string where the conversion ended.
 *
 * Converts a portion of a string into a numeric value of type NumberType, starting and ending 
 * at specified positions. Handles negative numbers and scientific notation.
 */
//-------------------------------------------------------------------
template<typename NumberType>

inline int from_string(NumberType& resulting_converted_number,
                       const char* string_begin,
                       int position_where_to_begin_converting,
                       int position_where_to_end_converting,
                       const char decimal_point_delimiter)
{
    resulting_converted_number = static_cast<NumberType>(0);

    if((position_where_to_begin_converting >= position_where_to_end_converting) || !string_begin)
    {
        // In this case there's nothing
        // to convert so we just return
        return position_where_to_begin_converting;
    }

    int current_position = position_where_to_begin_converting;

    bool is_number_negative = false;

    bool has_decimal_point_been_encountered_already = false;
    NumberType decimal_point_multiplier = static_cast<NumberType>(1);

    char current_character = string_begin[current_position];

    // The first step is to check the first digit
    // for special characters
    if(current_character == '-')
    {
        is_number_negative = true;
        ++current_position;
    }
    else if(current_character == '+')
    {
        ++current_position;
    }
    else if(current_character == decimal_point_delimiter)
    {
        has_decimal_point_been_encountered_already = true;
        ++current_position;
    }
    else if(current_character == 'e' || current_character == 'E')
    {
        // In this case the first character of the
        // string is an 'e' or 'E' which means that
        // the number we're converting is an exponent.
        // Since it is the first character, the number
        // will be 10^exponent.
        // Therefore, we find the exponent and then raise
        // 10 to its power
        ++current_position;
        NumberType exponent;
        current_position = from_string(exponent, string_begin, current_position, position_where_to_end_converting, decimal_point_delimiter);

        resulting_converted_number = std::pow(static_cast<NumberType>(10), exponent);

        return current_position;
    }

    // Now we loop through the remaining elements
    // of the string and calculate the number
    // accordingly.

    // NOTE:    Remember that we are looping from left to
    //          right, so from the highest value digit to
    //          the lowest value digit (numerically speaking)

    while(current_position < position_where_to_end_converting)
    {
        current_character = string_begin[current_position];

        if(current_character >= '0' && current_character <= '9')
        {
            if(has_decimal_point_been_encountered_already)
            {
                // This means that the digit is after the decimal
                // point, so we add the current digit divided by
                // its place after the decimal point
                decimal_point_multiplier *= static_cast<NumberType>(10);
                resulting_converted_number += (static_cast<NumberType>(current_character - '0') / decimal_point_multiplier);
            }
            else
            {
                // This means that the digit is before the decimal
                // point, so we multiply the current number by 10
                // and add the new digit
                resulting_converted_number = resulting_converted_number * static_cast<NumberType>(10) + static_cast<NumberType>(current_character - '0');
            }
        }
        else if(current_character == decimal_point_delimiter && !has_decimal_point_been_encountered_already)
        {
            // This means that we have just found the decimal
            // point so we just store the fact that we found
            // it in the boolean flag
            has_decimal_point_been_encountered_already = true;
        }
        else if( current_character == 'e' || current_character == 'E')
        {
            // In this case, it means that we're about to multiply
            // the current number by 10^exponent.
            // Thus, we find the value of the exponent and
            // then multiply

            // We calculate the exponent by recursively calling
            // this function
            NumberType exponent;
            int new_position = from_string(exponent, string_begin, current_position + 1, position_where_to_end_converting, decimal_point_delimiter);

            if((current_position - position_where_to_begin_converting == 1) &&
               (string_begin[current_position - 1] == '-' ||
                string_begin[current_position - 1] == '+' ||
                string_begin[current_position - 1] == '.'))
            {
                resulting_converted_number = std::pow(static_cast<NumberType>(10), exponent);
            }
            else
                resulting_converted_number = resulting_converted_number * std::pow(static_cast<NumberType>(10), exponent);

            current_position = new_position;

            break;
        }
        else
        {
            // In this case, we found a non-valid character, so
            // we are done
            break;
        }

        // Advance to the next position in the string
        ++current_position;
    }

    // Check if the number is negative
    if(is_number_negative)
        resulting_converted_number *= static_cast<NumberType>(-1);

    // Now we return the current position in the string
    // where we ended the conversion
    return current_position;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief A class to evaluate mathematical expressions.
 *
 * @tparam T The numeric type used for calculations.
 */
//-------------------------------------------------------------------
template<typename T>
class ExpressionEvaluator
{
public:

    /**
     * @brief Constructs an ExpressionEvaluator with the given expression.
     *
     * @param expression The mathematical expression to evaluate.
     */
    explicit ExpressionEvaluator(const std::string& expression)
        : expr(expression), it(expr.begin()), end(expr.end())
    {
    }

    /**
     * @brief Evaluates the expression and returns the result.
     *
     * @return The result of evaluating the expression.
     */
    T evaluate()
    {
        next_token();
        return parse_expression();
    }



private:

    enum TokenType
    {
        NUMBER, PLUS, MINUS, MULTIPLY, DIVIDE, LEFT_PAREN, RIGHT_PAREN,
        SIN, COS, TAN, COT, CSC, SEC, EXP, LN, EXPONENT, END
    };

    struct Token
    {
        TokenType type;
        T value;
    };

    std::string expr;
    std::string::const_iterator it;
    std::string::const_iterator end;
    Token token;

    void next_token()
    {
        while (it != end && std::isspace(*it)) ++it;  // Skip whitespace

        if (it == end)
        {
            token.type = END;
            return;
        }

        if (std::isdigit(*it))
        {
            std::string number_str;
            while (it != end && (std::isdigit(*it) || *it == '.')) number_str.push_back(*it++);
            std::istringstream iss(number_str);
            iss >> token.value;
            token.type = NUMBER;
            return;
        }

        switch (*it)
        {
            case '+': token.type = PLUS; break;
            case '-': token.type = MINUS; break;
            case '*': token.type = MULTIPLY; break;
            case '/': token.type = DIVIDE; break;
            case '(': token.type = LEFT_PAREN; break;
            case ')': token.type = RIGHT_PAREN; break;
            case '^': token.type = EXPONENT; break;
            default: break;
        }

        if (is_function_name("sin", it, end)) token.type = SIN;
        else if (is_function_name("cos", it, end)) token.type = COS;
        else if (is_function_name("tan", it, end)) token.type = TAN;
        else if (is_function_name("cot", it, end)) token.type = COT;
        else if (is_function_name("csc", it, end)) token.type = CSC;
        else if (is_function_name("sec", it, end)) token.type = SEC;
        else if (is_function_name("exp", it, end)) token.type = EXP;
        else if (is_function_name("ln", it, end)) token.type = LN;

        ++it;
    }

    T parse_expression()
    {
        T result = parse_term();

        while (token.type == PLUS || token.type == MINUS)
        {
            TokenType op = token.type;
            next_token();
            T value = parse_term();
            if (op == PLUS)
                result += value;
            else
                result -= value;
        }

        return result;
    }

    T parse_term()
    {
        T result = parse_factor();

        while (token.type == MULTIPLY || token.type == DIVIDE)
        {
            TokenType op = token.type;
            next_token();
            T value = parse_factor();
            if (op == MULTIPLY)
                result *= value;
            else
                result /= value;
        }

        return result;
    }

    T parse_factor()
    {
        T result = parse_value();

        while (token.type == EXPONENT)
        {
            next_token();
            T exponent = parse_value();
            result = std::pow(result, exponent);
        }

        return result;
    }

    T parse_value()
    {
        T result;

        if (token.type == NUMBER)
        {
            result = token.value;
            next_token();
        }
        else if (token.type == LEFT_PAREN)
        {
            next_token();
            result = parse_expression();
            if (token.type != RIGHT_PAREN)
            {
                std::cerr << "Error: Expected right parenthesis." << std::endl;
                return T();
            }
            next_token();
        }
        else if (token.type == SIN || token.type == COS || token.type == TAN ||
                 token.type == COT || token.type == CSC || token.type == SEC ||
                 token.type == EXP || token.type == LN)
        {
            TokenType func_type = token.type;
            next_token();
            T argument = parse_value();
            switch (func_type)
            {
                case SIN: result = sin(argument); break;
                case COS: result = cos(argument); break;
                case TAN: result = tan(argument); break;
                case COT: result = T(1) / tan(argument); break;
                case CSC: result = T(1) / sin(argument); break;
                case SEC: result = T(1) / cos(argument); break;
                case EXP: result = exp(argument); break;
                case LN: result = log(argument); break;
                default: break;
            }
        }
        else
        {
            std::cerr << "Error: Expected number, parenthesis, or function." << std::endl;
            return T();
        }

        return result;
    }

    bool is_function_name(
        const char* name,
        std::string::const_iterator& it,
        const std::string::const_iterator& end
    )
    {
        auto name_end = name + std::strlen(name);
        if (std::distance(it, end) >= std::distance(name, name_end) &&
            std::equal(it, it + std::distance(name, name_end), name))
        {
            it += std::distance(name, name_end);
            return true;
        }
        return false;
    }
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_CONVERT_NUMBERS_HPP_
