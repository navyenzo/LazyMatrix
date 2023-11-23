#ifndef INCLUDE_BASE_MATRIX_HPP_
#define INCLUDE_BASE_MATRIX_HPP_



//-------------------------------------------------------------------
#include <iostream>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>
struct is_type_a_matrix : std::false_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Used as the base expression class from which to derive from
//-------------------------------------------------------------------
template<typename MatrixType>

class BaseMatrix
{
public:
    
    int64_t rows()const
    {
        return underlying().rows();
    }

    int64_t columns()const
    {
        return underlying().columns();
    }

    int64_t size()const
    {
        return this->rows() * this->columns();
    }

    
    
    decltype(auto) at(int64_t row, int64_t column)const
    {
        return underlying().at(row, column);
    }
    
    decltype(auto) at(int64_t row, int64_t column)
    {
        return underlying().at(row, column);
    }

    decltype(auto) at(int64_t index)const
    {
        return this->at(index / this->columns(), index % this->columns());
    }

    decltype(auto) at(int64_t index)
    {
        return this->at(index / this->columns(), index % this->columns());
    }


    
    decltype(auto) operator()(int64_t row, int64_t column)const
    {
        return this->at(row, column);
    }
    
    decltype(auto) operator()(int64_t row, int64_t column)
    {
        return this->at(row, column);
    }

    decltype(auto) operator()(int64_t index)const
    {
        return this->at(index / this->columns(), index % this->columns());
    }

    decltype(auto) operator()(int64_t index)
    {
        return this->at(index / this->columns(), index % this->columns());
    }



    decltype(auto) circ_at(int64_t row, int64_t column)const
    {
        int64_t circ_row = (this->rows() + row % this->rows()) % this->rows();
        int64_t circ_column = (this->columns() + column % this->columns()) % this->columns();
        return this->at(circ_row, circ_column);
    }

    decltype(auto) circ_at(int64_t row, int64_t column)
    {
        int64_t circ_row = (this->rows() + row % this->rows()) % this->rows();
        int64_t circ_column = (this->columns() + column % this->columns()) % this->columns();
        return this->at(circ_row, circ_column);
    }

    decltype(auto) circ_at(int64_t index)const
    {
        int64_t circ_index = (this->size() + index % this->size()) % this->size();
        return this->at(circ_index);
    }

    decltype(auto) circ_at(int64_t index)
    {
        int64_t circ_index = (this->size() + index % this->size()) % this->size();
        return this->at(circ_index);
    }



private:

    BaseMatrix<MatrixType>(){}
    friend MatrixType;

    MatrixType& underlying()
    {
        return static_cast<MatrixType&>(*this);
    }

    const MatrixType& underlying()const
    {
        return static_cast<const MatrixType&>(*this);
    }
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< BaseMatrix<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_BASE_MATRIX_HPP_
