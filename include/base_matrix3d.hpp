#ifndef INCLUDE_BASE_MATRIX3D_HPP_
#define INCLUDE_BASE_MATRIX3D_HPP_



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
struct is_type_a_matrix3d : std::false_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Used as the base expression class from which to derive from
//-------------------------------------------------------------------
template<typename MatrixType>

class BaseMatrix3D
{
public:  

    int pages()const
    {
        return underlying().pages();
    }  
    
    int rows()const
    {
        return underlying().rows();
    }

    int columns()const
    {
        return underlying().columns();
    }

    int size()const
    {
        return this->rows() * this->columns() * this->pages();
    }

    
    
    decltype(auto) at(int pages, int row, int column)const
    {
        return underlying()(pages, row, column);
    }

    decltype(auto) at(int pages, int row, int column)
    {
        return underlying()(pages, row, column);
    }
    
    decltype(auto) operator()(int pages, int row, int column)const
    {
        return this->at(pages, row, column);
    }
    
    decltype(auto) operator()(int pages, int row, int column)
    {
        return this->at(pages, row, column);
    }

    decltype(auto) operator()(int index)const
    {
        int page = index / (this->rows() * this->columns());

        int remainder = index % (this->rows() * this->columns());

        return this->at(page, remainder / this->columns(), remainder % this->columns());
    }



    decltype(auto) circ_at(int page, int row, int column)const
    {
        int circ_page = (this->pages() + page % this->pages) % this->pages();
        int circ_row = (this->rows() + row % this->rows()) % this->rows();
        int circ_column = (this->columns() + column % this->columns()) % this->columns();
        return (*this)(circ_page, circ_row, circ_column);
    }

    decltype(auto) circ_at(int index)const
    {
        int64_t circ_index = (this->size() + index % this->size()) % this->size();
        return (*this)(circ_index);
    }



private:

    BaseMatrix3D<MatrixType>(){}
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

struct is_type_a_matrix3d< BaseMatrix3D<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_BASE_MATRIX3D_HPP_
