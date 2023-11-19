#ifndef INCLUDE_POLYMORPHIC_MATRIX_HPP_
#define INCLUDE_POLYMORPHIC_MATRIX_HPP_



//-------------------------------------------------------------------
#include <memory>

#include "base_matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// These classes allow a programmer to store complex combinations of
// lazymatrix components in a polymorphic container for applications
// that require it
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>

class PolymorphicMatrix
{
public:

    virtual int64_t rows()const = 0;
    virtual int64_t columns()const = 0;
    virtual int64_t size()const = 0;

    virtual DataType at(int64_t row, int64_t column)const = 0;
    virtual DataType& at(int64_t row, int64_t column) = 0;

    DataType at(int64_t index)const
    {
        return this->at(index / this->columns(), index % this->columns());
    }

    DataType& at(int64_t index)
    {
        return this->at(index / this->columns(), index % this->columns());
    }

    DataType operator()(int64_t row, int64_t column)const
    {
        return this->at(row, column);
    }

    DataType& operator()(int64_t row, int64_t column)
    {
        return this->at(row, column);
    }

    DataType operator()(int64_t index)const
    {
        return this->at(index / this->columns(), index % this->columns());
    }

    DataType& operator()(int64_t index)
    {
        return this->at(index / this->columns(), index % this->columns());
    }



    DataType circ_at(int64_t row, int64_t column)const
    {
        int64_t circ_row = (this->rows() + row % this->rows()) % this->rows();
        int64_t circ_column = (this->columns() + column % this->columns()) % this->columns();
        return this->at(circ_row, circ_column);
    }

    DataType& circ_at(int64_t row, int64_t column)
    {
        int64_t circ_row = (this->rows() + row % this->rows()) % this->rows();
        int64_t circ_column = (this->columns() + column % this->columns()) % this->columns();
        return this->at(circ_row, circ_column);
    }

    DataType circ_at(int64_t index)const
    {
        int64_t circ_index = (this->size() + index % this->size()) % this->size();
        return this->at(circ_index);
    }

    DataType& circ_at(int64_t index)
    {
        int64_t circ_index = (this->size() + index % this->size()) % this->size();
        return this->at(circ_index);
    }
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename MatrixType>

class PolymorphicMatrixWrapper : public PolymorphicMatrix<typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type>
{
public:

    // Type of value that is returned by the matrix
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;

    PolymorphicMatrixWrapper<MatrixType>(const MatrixType& matrix)
    : matrix_(matrix)
    {
    }



    int64_t rows()const override
    {
        return matrix_.rows();
    }

    int64_t columns()const override
    {
        return matrix_.columns();
    }

    int64_t size()const override
    {
        return matrix_.size();
    }

    value_type at(int64_t row, int64_t column)const override
    {
        return matrix_.at(row, column);
    }

    value_type& at(int64_t row, int64_t column) override
    {
        return zero_;
    }



private:

    const MatrixType& matrix_;
    value_type zero_ = value_type(0);
    
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename MatrixType>

class PolymorphicMatrixWrapperView : public PolymorphicMatrix<typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type>
{
public:

    // Type of value that is returned by the matrix
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;

    PolymorphicMatrixWrapperView<MatrixType>(MatrixType& matrix)
    : matrix_(matrix)
    {
    }



    int64_t rows()const override
    {
        return matrix_.rows();
    }

    int64_t columns()const override
    {
        return matrix_.columns();
    }

    int64_t size()const override
    {
        return matrix_.size();
    }

    value_type at(int64_t row, int64_t column)const override
    {
        return matrix_.at(row, column);
    }

    value_type& at(int64_t row, int64_t column) override
    {
        return matrix_.at(row, column);
    }



private:

    MatrixType& matrix_;
    
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< PolymorphicMatrixWrapper<MatrixType> > : std::true_type
{
};



template<typename MatrixType>

struct is_type_a_matrix< PolymorphicMatrixWrapperView<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Functions used to wrap a matrix expression in a matrix wrapper
//-------------------------------------------------------------------
template<typename MatrixType>

inline auto wrap_matrix(const MatrixType& matrix)
{
    return PolymorphicMatrixWrapper<MatrixType>(matrix);
}



template<typename MatrixType>

inline auto wrap_matrix_ptr(const MatrixType& matrix)
{
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;

    PolymorphicMatrix<value_type>* ptr = new PolymorphicMatrixWrapper<MatrixType>(matrix);

    return ptr;
}



template<typename MatrixType>

inline auto wrap_matrix_view(MatrixType& matrix)
{
    return PolymorphicMatrixWrapperView<MatrixType>(matrix);
}



template<typename MatrixType>

inline auto wrap_matrix_view_ptr(MatrixType& matrix)
{
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;

    PolymorphicMatrix<value_type>* ptr = new PolymorphicMatrixWrapperView<MatrixType>(matrix);

    return ptr;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_POLYMORPHIC_MATRIX_HPP_
