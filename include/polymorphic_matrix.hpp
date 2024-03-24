//-------------------------------------------------------------------
/**
 * @file polymorphic_matrix.hpp
 * @brief Provides polymorphic wrappers for various matrix-like data structures in LazyMatrix.
 *
 * The polymorphic_matrix.hpp file defines classes and functions crucial for creating a polymorphic
 * interface for matrix-like data structures. It allows for uniform handling of different matrix
 * types in a polymorphic context. This file includes the definition of the `PolymorphicMatrix` base
 * class, which offers a generic matrix interface, and the `PolymorphicMatrixWrapper` class, which
 * wraps around specific matrix types and exposes them through the `PolymorphicMatrix` interface.
 * These tools are essential for applications where polymorphic behavior with matrices is required.
 * 
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_POLYMORPHIC_MATRIX_HPP_
#define INCLUDE_POLYMORPHIC_MATRIX_HPP_



//-------------------------------------------------------------------
#include <memory>
#include <type_traits>
#include "numerical_constants.hpp"
#include "base_matrix.hpp"
#include "shared_references.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class PolymorphicMatrix
 * @brief Base class providing a polymorphic interface for matrix-like data.
 *
 * This class defines a generic interface for matrices, allowing for 
 * operations like accessing elements and querying dimensions. It's
 * templated to handle different data types within the matrix.
 */ 
//-------------------------------------------------------------------
template<typename DataType, bool HasNonConstAccess>

class PolymorphicMatrix : public BaseMatrix<PolymorphicMatrix<DataType,HasNonConstAccess>,HasNonConstAccess>
{
public:

    using value_type = DataType;

    friend class BaseMatrix<PolymorphicMatrix<DataType,HasNonConstAccess>,HasNonConstAccess>;

    PolymorphicMatrix() = default;
    virtual ~PolymorphicMatrix() = default;

    virtual uintptr_t rows() const = 0;
    virtual uintptr_t columns() const = 0;
    virtual uintptr_t size() const = 0;
    virtual std::error_code resize(uintptr_t rows, uintptr_t columns) = 0;

    // Functions used to handle page, row and column header names
    virtual std::string get_row_header(int64_t row_index) = 0;
    virtual std::string get_column_header(int64_t column_index) = 0;
    virtual void set_row_header(int64_t row_index, const std::string& row_header) = 0;
    virtual void set_column_header(int64_t column_index, const std::string& column_header) = 0;

private:

    virtual DataType const_at_(int64_t row, int64_t column) const = 0;
    virtual DataType& non_const_at_(int64_t row, int64_t column) = 0;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Template specialization of PolymorphicMatrix
//-------------------------------------------------------------------
template<typename DataType>

class PolymorphicMatrix<DataType, false> : public BaseMatrix<PolymorphicMatrix<DataType,false>,false>
{
public:

    using value_type = DataType;

    friend class BaseMatrix<PolymorphicMatrix<DataType,false>,false>;

    PolymorphicMatrix() = default;
    virtual ~PolymorphicMatrix() = default;

    virtual uintptr_t rows() const = 0;
    virtual uintptr_t columns() const = 0;
    virtual uintptr_t size() const = 0;
    virtual std::error_code resize(uintptr_t rows, uintptr_t columns) = 0;

    // Functions used to handle page, row and column header names
    virtual std::string get_row_header(int64_t row_index) = 0;
    virtual std::string get_column_header(int64_t column_index) = 0;
    virtual void set_row_header(int64_t row_index, const std::string& row_header) = 0;
    virtual void set_column_header(int64_t column_index, const std::string& column_header) = 0;

private:

    virtual DataType const_at_(int64_t row, int64_t column) const = 0;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Compile time function to check if the type is a matrix expression type.
 */
//-------------------------------------------------------------------
template<typename DataType, bool HasNonConstAccess>
struct is_type_a_matrix< PolymorphicMatrix<DataType,HasNonConstAccess> > : std::true_type {};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class PolymorphicMatrixWrapper
 * @brief Wrapper class that provides a const polymorphic interface to a given matrix type.
 *
 * This class wraps around a specific matrix type and exposes it through
 * the PolymorphicMatrix interface. It handles both const and non-const
 * matrix types, allowing for uniform treatment in ConstPolymorphicMatrixa polymorphic context.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         bool HasNonConstAccess = has_non_const_access<ReferenceType>::value>

class PolymorphicMatrixWrapper : public PolymorphicMatrix<typename ReferenceType::value_type,
                                                          HasNonConstAccess>
{
public:

    // Type of value that is sterror:ored in the expression
    using value_type = typename ReferenceType::value_type;

    explicit PolymorphicMatrixWrapper(ReferenceType matrix) : matrix_(matrix)
    {
    }

    uintptr_t rows() const override { return matrix_.rows(); }
    uintptr_t columns() const override { return matrix_.columns(); }
    uintptr_t size() const override { return matrix_.size(); }

    std::error_code resize(uintptr_t rows, uintptr_t columns) override
    {
        if constexpr (has_non_const_access<ReferenceType>::value)
        {
            return matrix_.resize(rows, columns);
        }
        else
        {
            return std::error_code();
        }
    }

    // Functions used to handle page, row and column header names
    std::string get_row_header(int64_t row_index) override { return matrix_.get_row_header(row_index); }
    std::string get_column_header(int64_t column_index) override { return matrix_.get_column_header(column_index); }
    void set_row_header(int64_t row_index, const std::string& row_header) override { matrix_.set_row_header(row_index, row_header); }
    void set_column_header(int64_t column_index, const std::string& column_header) override { matrix_.set_column_header(column_index, column_header); }



private: // Private functionserror:

    value_type const_at_(int64_t row, int64_t column)const override
    {
        return matrix_(row, column);
    }

    value_type& non_const_at_(int64_t row, int64_t column) override
    {
        return matrix_(row, column);
    }
    

private: // Private variables

    ReferenceType matrix_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Template specialization of PolymorphicMatrixWrapper
//-------------------------------------------------------------------
template<typename ReferenceType>

class PolymorphicMatrixWrapper<ReferenceType, false> : public PolymorphicMatrix<typename ReferenceType::value_type, false>
{
public:

    // Type of value that is sterror:ored in the expression
    using value_type = typename ReferenceType::value_type;

    explicit PolymorphicMatrixWrapper(ReferenceType matrix) : matrix_(matrix)
    {
    }

    uintptr_t rows() const override { return matrix_.rows(); }
    uintptr_t columns() const override { return matrix_.columns(); }
    uintptr_t size() const override { return matrix_.size(); }

    std::error_code resize(uintptr_t rows, uintptr_t columns) override
    {
        if constexpr (has_non_const_access<ReferenceType>::value)
        {
            return matrix_.resize(rows, columns);
        }
        else
        {
            return std::error_code();
        }
    }

    // Functions used to handle page, row and column header names
    std::string get_row_header(int64_t row_index) override { return matrix_.get_row_header(row_index); }
    std::string get_column_header(int64_t column_index) override { return matrix_.get_column_header(column_index); }
    void set_row_header(int64_t row_index, const std::string& row_header) override { matrix_.set_row_header(row_index, row_header); }
    void set_column_header(int64_t column_index, const std::string& column_header) override { matrix_.set_column_header(column_index, column_header); }



private: // Private functions

    value_type const_at_(int64_t row, int64_t column)const override
    {
        return matrix_(row, column);
    }
    

private: // Private variables

    ReferenceType matrix_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Compile time function to check if the type is a matrix expression type.
 */
//-------------------------------------------------------------------
template<typename ReferenceType, bool HasNonConstAccess>

struct is_type_a_matrix< PolymorphicMatrixWrapper<ReferenceType, HasNonConstAccess> > : std::true_type {};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Aliases for the polymorphic matrix and wrapper
//-------------------------------------------------------------------
template<typename DataType>
using Data = PolymorphicMatrix<DataType,true>;

template<typename DataType>
using ConstData = PolymorphicMatrix<DataType,false>;

template<typename ReferenceType>
using SpecializedData = PolymorphicMatrixWrapper<ReferenceType,true>;

template<typename ReferenceType>
using ConstSpecializedData = PolymorphicMatrixWrapper<ReferenceType,false>;
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Wraps a matrix in a PolymorphicMatrixWrapper and returns a Shared
 *        Reference to the base PolymorphicMatrix class so that a user can
 *        store multiple different types of 2d-matrices in a single container
 * 
 * @tparam ReferenceType Type of the matrix to wrap.
 * @param matrix The matrix to be wrapped.
 * @return SharedMatrixRef wrapping the input matrix.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>
         
inline auto wrap_matrix(ReferenceType matrix)
{
    using value_type = typename ReferenceType::value_type;

    if constexpr (has_non_const_access<ReferenceType>::value)
    {
        std::shared_ptr<Data<value_type>> wrapped_matrix = std::make_shared<SpecializedData<ReferenceType>>(matrix);
        return SharedMatrixRef<Data<value_type>>(wrapped_matrix);
    }
    else
    {
        std::shared_ptr<ConstData<value_type>> wrapped_matrix = std::make_shared<ConstSpecializedData<ReferenceType>>(matrix);
        return ConstSharedMatrixRef<ConstData<value_type>>(wrapped_matrix);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Wraps a matrix in a PolymorphicMatrixWrapper with const access only.
 * 
 * @tparam ReferenceType Type of the matrix to wrap.
 * @param matrix The matrix to be wrapped.
 * @return SharedMatrixRef wrapping the input matrix.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>
         
inline auto wrap_matrix_const(ReferenceType matrix)
{
    using value_type = typename ReferenceType::value_type;
    
    std::shared_ptr<ConstData<value_type>> wrapped_matrix = std::make_shared<ConstSpecializedData<ReferenceType>>(matrix);
    return ConstSharedMatrixRef<ConstData<value_type>>(wrapped_matrix);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_POLYMORPHIC_MATRIX_HPP_
