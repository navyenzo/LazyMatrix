//-------------------------------------------------------------------
// eigen_wrappers.hpp
// Provides wrappers for integrating LazyMatrix references with Eigen.
//
// Author: Vincenzo Barbato
// GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
// LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
//-------------------------------------------------------------------

#ifndef INCLUDE_EIGEN_WRAPPERS_HPP_
#define INCLUDE_EIGEN_WRAPPERS_HPP_

#include <Eigen/Eigen>
#include "shared_references.hpp"

namespace LazyMatrix
{
//-------------------------------------------------------------------
/**
 * @class EigenMatrixAdapter
 * @brief A wrapper class to integrate SharedMatrixRef with Eigen.
 *
 * Adapts SharedMatrixRef for use with Eigen's operations and algorithms.
 *
 * @tparam MatrixType The matrix type wrapped by SharedMatrixRef.
 */
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

class EigenMatrixAdapter : public Eigen::MatrixBase<EigenMatrixAdapter<MatrixType>>
{
public:

    using Scalar = typename MatrixType::value_type;
    using Index = Eigen::Index;

    static constexpr int RowsAtCompileTime = Eigen::Dynamic;
    static constexpr int ColsAtCompileTime = Eigen::Dynamic;
    static constexpr int MaxRowsAtCompileTime = Eigen::Dynamic;
    static constexpr int MaxColsAtCompileTime = Eigen::Dynamic;
    
    using Nested = EigenMatrixAdapter<MatrixType>;
    using NestedExpression = EigenMatrixAdapter<MatrixType>;
    
    enum { CoeffReadCost = Eigen::NumTraits<Scalar>::ReadCost, Flags = Eigen::RowMajor };
    
    // Other necessary typedefs and methods...

    // Constructor
    explicit EigenMatrixAdapter(SharedMatrixRef<MatrixType> matrix_ref)
    : matrix_ref_(matrix_ref)
    {
    }

    // Accessor methods
    inline const Scalar coeff(Index row, Index col) const
    {
        std::cout << "non-const shared matrix ref coeff function being called: about to access value at (" << row << "," << col << "): " << std::endl;
        std::cout << "before we access it, let's print out the value of the shared reference pointer: " << matrix_ref_.get_ptr() << "\n\n\n" << std::endl;
        return matrix_ref_(row, col);
    }

    inline Scalar& coeffRef(Index row, Index col)
    {
        std::cout << "non-const shared matrix ref coeffRef function being called: about to access value at (" << row << "," << col << "): " << std::endl;
        std::cout << "before we access it, let's print out the value of the shared reference pointer: " << matrix_ref_.get_ptr() << "\n\n\n" << std::endl;
        return matrix_ref_(row, col);
    }

    inline Index rows() const
    {
        return matrix_ref_.rows();
    }
    
    inline Index cols() const
    {
        return matrix_ref_.columns();
    }

    SharedMatrixRef<MatrixType> get_matrix_ref()const
    {
        return matrix_ref_;
    }

private:

    SharedMatrixRef<MatrixType> matrix_ref_;
};

//-------------------------------------------------------------------
/**
 * @class EigenConstMatrixAdapter
 * @brief A wrapper class to integrate ConstSharedMatrixRef with Eigen.
 *
 * Adapts ConstSharedMatrixRef for use with Eigen's operations and algorithms.
 *
 * @tparam MatrixType The matrix type wrapped by ConstSharedMatrixRef.
 */
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>
         
class EigenConstMatrixAdapter : public Eigen::MatrixBase<EigenConstMatrixAdapter<MatrixType>>
{
public:
    using Scalar = typename MatrixType::value_type;
    using Index = Eigen::Index;

    static constexpr int RowsAtCompileTime = Eigen::Dynamic;
    static constexpr int ColsAtCompileTime = Eigen::Dynamic;
    static constexpr int MaxRowsAtCompileTime = Eigen::Dynamic;
    static constexpr int MaxColsAtCompileTime = Eigen::Dynamic;
    
    using Nested = EigenMatrixAdapter<MatrixType>;
    using NestedExpression = EigenMatrixAdapter<MatrixType>;

    enum { CoeffReadCost = Eigen::NumTraits<Scalar>::ReadCost, Flags = Eigen::RowMajor };
    
    // Other necessary typedefs and methods...

    // Constructor
    explicit EigenConstMatrixAdapter(ConstSharedMatrixRef<MatrixType> matrix_ref)
    : matrix_ref_(matrix_ref)
    {
    }

    // Accessor methods
    inline const Scalar coeff(Index row, Index col) const
    {
        std::cout << "onst shared matrix ref coeff function being called: about to access value at (" << row << "," << col << "): " << std::endl;
        std::cout << "before we access it, let's print out the value of the shared reference pointer: " << matrix_ref_.get_ptr() << "\n\n\n" << std::endl;
        return matrix_ref_(row, col);
    }

    inline Index rows() const
    {
        return matrix_ref_.rows();
    }
    
    inline Index cols() const
    {
        return matrix_ref_.columns();
    }

    ConstSharedMatrixRef<MatrixType> get_matrix_ref()const
    {
        return matrix_ref_;
    }

private:

    ConstSharedMatrixRef<MatrixType> matrix_ref_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function to wrap a LazyMatrix reference with an appropriate Eigen wrapper
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

auto eigen_wrap(ReferenceType ref)
{
    if constexpr (has_non_const_access<ReferenceType>::value)
    {
        return EigenMatrixAdapter<typename ReferenceType::matrix_type>(ref);
    }
    else
    {
        return EigenConstMatrixAdapter<typename ReferenceType::matrix_type>(ref);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Traits to integrate the matrix wrapprs with eigen library
//-------------------------------------------------------------------
namespace Eigen {
namespace internal {



template<typename MatrixType>
struct traits<LazyMatrix::EigenMatrixAdapter<MatrixType>> : public Eigen::internal::traits<Eigen::Matrix<typename MatrixType::value_type, Eigen::Dynamic, Eigen::Dynamic>>
{
    using Scalar = typename MatrixType::value_type;
    using Index = Eigen::Index;

    enum
    {
        Flags = Eigen::RowMajor | Eigen::LvalueBit
    };
};



template<typename MatrixType>
struct traits<LazyMatrix::EigenConstMatrixAdapter<MatrixType>> : public Eigen::internal::traits<Eigen::Matrix<typename MatrixType::value_type, Eigen::Dynamic, Eigen::Dynamic>>
{
    using Scalar = typename MatrixType::value_type;
    using Index = Eigen::Index;

    enum
    {
        Flags = Eigen::RowMajor
    };
};



template <typename MatrixType>
struct evaluator<LazyMatrix::EigenMatrixAdapter<MatrixType>> : evaluator_base<LazyMatrix::EigenMatrixAdapter<MatrixType>>
{
    using XprType = LazyMatrix::EigenMatrixAdapter<MatrixType>;

    explicit evaluator(XprType xpr)
    : expression_(xpr)
    {
    }

    typename XprType::Scalar coeff(Index row, Index col) const
    {
        return expression_(row, col);
    }

    typename XprType::Scalar& coeffRef(Index row, Index col)
    {
        return expression_(row, col);
    }

    Index rows() const { return expression_.rows(); }
    Index cols() const { return expression_.cols(); }
    Index size() const { return expression_.size(); }

private:

    XprType expression_;
};



template <typename MatrixType>
struct evaluator<LazyMatrix::EigenConstMatrixAdapter<MatrixType>> : evaluator_base<LazyMatrix::EigenConstMatrixAdapter<MatrixType>>
{
    using XprType = LazyMatrix::EigenConstMatrixAdapter<MatrixType>;

    explicit evaluator(XprType xpr)
    : expression_(xpr)
    {
    }

    typename XprType::Scalar coeff(Index row, Index col) const
    {
        return expression_(row, col);
    }

    Index rows() const { return expression_.rows(); }
    Index cols() const { return expression_.cols(); }
    Index size() const { return expression_.size(); }

private:

    XprType expression_;
};



} // namespace internal
} // namespace Eigen
//-------------------------------------------------------------------



#endif // INCLUDE_EIGEN_WRAPPERS_HPP_
