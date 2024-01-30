//-------------------------------------------------------------------
/**
 * @file eigen_wrappers.hpp
 * @brief Provides wrappers for integrating LazyMatrix shared references with Eigen.
 *
 * This file contains the EigenWrapper class and ConstEigenWrapper class, which
 * wrap around SharedMatrixRef and ConstSharedMatrixRef respectively so they can
 * be used in eigen library's algorithms.
 * 
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_EIGEN_WRAPPERS_HPP_
#define INCLUDE_EIGEN_WRAPPERS_HPP_



//-------------------------------------------------------------------
#include <Eigen/Eigen>
#include "shared_references.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class EigenWrapper
 * @brief A wrapper class to integrate SharedMatrixRef references with Eigen.
 *
 * EigenWrapper provides an interface to adapt SharedMatrixRef
 * for use with Eigen's operations and algorithms.
 *
 * @tparam ReferenceType Type of the LazyMatrix reference (SharedReferenceType)
 */
//-------------------------------------------------------------------
template<typename ReferenceType>

class EigenWrapper : public Eigen::MatrixBase<EigenWrapper<ReferenceType>>
{
public:

    // Define necessary Eigen types
    using Scalar = typename ReferenceType::value_type;
    using Index = Eigen::Index;
    static constexpr int RowsAtCompileTime = Eigen::Dynamic;
    static constexpr int ColsAtCompileTime = Eigen::Dynamic;
    static constexpr int MaxRowsAtCompileTime = Eigen::Dynamic;
    static constexpr int MaxColsAtCompileTime = Eigen::Dynamic;

    using Nested = EigenWrapper<ReferenceType>&;
    using NestedExpression = EigenWrapper<ReferenceType>;

    /**
     * @brief Constructs a wrapper around a LazyMatrix reference.
     * @param ref The LazyMatrix reference to wrap.
     */
    EigenWrapper(ReferenceType ref)
    : matrix_(ref)
    {
    }

    // Assignment Operators (if applicable)
    EigenWrapper& operator=(const EigenWrapper& other)
    {
        if (this != &other)
        {
            matrix_ = other.matrix_;
        }

        return *this;
    }

    /**
     * @brief Provides constant access to an element of the matrix.
     * @param row The row index.
     * @param col The column index.
     * @return A constant reference to the matrix element.
     */
    const Scalar coeff(Index row, Index col) const
    {
        return matrix_(row, col);
    }

    /**
     * @brief Provides non-constant access to an element of the matrix.
     *        Enabled only if the reference type allows non-const access.
     * @param row The row index.
     * @param col The column index.
     * @return A reference to the matrix element.
     */
    //template <typename T = ReferenceType, typename = std::enable_if_t<has_non_const_access<T>::value>>
    Scalar& coeffRef(Index row, Index col)
    {
        if constexpr (has_non_const_access<ReferenceType>::value)
        {
            return matrix_(row, col);
        }
        else
        {
            return DummyValueHolder<Scalar>::zero;
        }
    }

    // Size Information
    Index rows() const { return matrix_.rows(); }
    Index cols() const { return matrix_.columns(); }
    Index size() const { return matrix_.size(); }

    const ReferenceType& get_matrix() const { return matrix_; }



private:

    ReferenceType matrix_; ///< The underlying LazyMatrix reference.
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class ConstEigenWrapper
 * @brief A wrapper class to integrate ConstSharedMatrixRef  references with Eigen.
 *
 * ConstEigenWrapper provides an interface to adapt ConstSharedMatrixRef
 * for use with Eigen's operations and algorithms.
 *
 * @tparam ReferenceType Type of the LazyMatrix reference (SharedReferenceType)
 */
//-------------------------------------------------------------------
template<typename ReferenceType>

class ConstEigenWrapper : public Eigen::MatrixBase<ConstEigenWrapper<ReferenceType>>
{
public:

    // Define necessary Eigen types
    using Scalar = typename ReferenceType::value_type;
    using Index = Eigen::Index;
    static constexpr int RowsAtCompileTime = Eigen::Dynamic;
    static constexpr int ColsAtCompileTime = Eigen::Dynamic;
    static constexpr int MaxRowsAtCompileTime = Eigen::Dynamic;
    static constexpr int MaxColsAtCompileTime = Eigen::Dynamic;

    using Nested = ConstEigenWrapper<ReferenceType>&;
    using NestedExpression = ConstEigenWrapper<ReferenceType>;

    /**
     * @brief Constructs a wrapper around a LazyMatrix reference.
     * @param ref The LazyMatrix reference to wrap.
     */
    ConstEigenWrapper(ReferenceType ref)
    : matrix_(ref)
    {
    }

    /**
     * @brief Provides constant access to an element of the matrix.
     * @param row The row index.
     * @param col The column index.
     * @return A constant reference to the matrix element.
     */
    const Scalar coeff(Index row, Index col) const
    {
        return matrix_(row, col);
    }

    // Size Information
    Index rows() const { return matrix_.rows(); }
    Index cols() const { return matrix_.columns(); }
    Index size() const { return matrix_.size(); }

    const ReferenceType& get_matrix() const { return matrix_; }



private:

    ReferenceType matrix_; ///< The underlying LazyMatrix reference.
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Wraps a given LazyMatrix reference with a EigenWrapper for compatibility with Eigen.
 * @tparam ReferenceType Type of the LazyMatrix reference (SharedReferenceType or ConstSharedReferenceType).
 *        The type must be a valid matrix reference type.
 * @param ref The matrix reference to be wrapped.
 * @return An instance of EigenWrapper wrapping the given matrix reference.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>::value>* = nullptr>

inline auto eigen_wrap(ReferenceType ref)
{
    if constexpr (has_non_const_access<ReferenceType>::value)
    {
        return EigenWrapper<ReferenceType>(ref);
    }
    else
    {
        return ConstEigenWrapper<ReferenceType>(ref);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Specialize Eigen traits for EigenWrapper
//-------------------------------------------------------------------
namespace Eigen {
namespace internal {



template<typename ReferenceType>         
struct traits<LazyMatrix::EigenWrapper<ReferenceType>> : public Eigen::internal::traits<Eigen::Matrix<typename ReferenceType::value_type, Eigen::Dynamic, Eigen::Dynamic>>
{
    using Scalar = typename ReferenceType::value_type;
    using Index = Index;
    
    enum
    {
        CoeffReadCost = Eigen::NumTraits<typename ReferenceType::value_type>::ReadCost,
        Flags = Eigen::internal::traits<Eigen::Matrix<typename ReferenceType::value_type, Eigen::Dynamic, Eigen::Dynamic>>::Flags
    };
};



template<typename ReferenceType>         
struct traits<LazyMatrix::ConstEigenWrapper<ReferenceType>> : public Eigen::internal::traits<Eigen::Matrix<typename ReferenceType::value_type, Eigen::Dynamic, Eigen::Dynamic>>
{
    using Scalar = typename ReferenceType::value_type;
    using Index = Index;
    
    enum
    {
        CoeffReadCost = Eigen::NumTraits<typename ReferenceType::value_type>::ReadCost,
        Flags = Eigen::internal::traits<Eigen::Matrix<typename ReferenceType::value_type, Eigen::Dynamic, Eigen::Dynamic>>::Flags
              & ~Eigen::LvalueBit  // Removes the LvalueBit to indicate read-only
    };
};



// Evaluator for EigenWrapper
template<typename ReferenceType>
struct evaluator<LazyMatrix::EigenWrapper<ReferenceType>> : public evaluator_base<LazyMatrix::EigenWrapper<ReferenceType>>
{
    using XprType = LazyMatrix::EigenWrapper<ReferenceType>;
    
    enum
    {
        CoeffReadCost = Eigen::NumTraits<typename ReferenceType::value_type>::ReadCost,
        Flags = Eigen::internal::traits<Eigen::Matrix<typename ReferenceType::value_type, Eigen::Dynamic, Eigen::Dynamic>>::Flags
    };

    explicit evaluator(XprType xpr)
    : expr_(xpr)
    {
    }

    // Access coefficient at a given row and column
    typename XprType::Scalar coeff(Index row, Index col) const
    {
        return expr_.coeff(row, col);
    }

    // Access coefficient at a given index (for vectors or linear access)
    typename XprType::Scalar coeff(Index index) const
    {
        // Assuming linear access; modify if your matrix uses a different storage order
        return expr_.coeff(index / expr_.cols(), index % expr_.cols());
    }

    // Access coefficient by row and column for non-const access (if applicable)
    typename XprType::Scalar& coeffRef(Index row, Index col)
    {
        return expr_.coeffRef(row, col);
    }

    // Get the number of rows
    Index rows() const { return expr_.rows(); }

    // Get the number of columns
    Index cols() const { return expr_.cols(); }

    // Get the total size (for vectors or linear access)
    Index size() const { return expr_.size(); }

private:

    XprType expr_;
};



// Evaluator for ConstEigenWrapper
template<typename ReferenceType>
struct evaluator<LazyMatrix::ConstEigenWrapper<ReferenceType>> : public evaluator_base<LazyMatrix::ConstEigenWrapper<ReferenceType>>
{
    using XprType = LazyMatrix::ConstEigenWrapper<ReferenceType>;
    
    enum
    {
        CoeffReadCost = Eigen::NumTraits<typename ReferenceType::value_type>::ReadCost,
        Flags = Eigen::internal::traits<Eigen::Matrix<typename ReferenceType::value_type, Eigen::Dynamic, Eigen::Dynamic>>::Flags
              & ~Eigen::LvalueBit  // Removes the LvalueBit to indicate read-only
    };

    explicit evaluator(XprType xpr)
    : expr_(xpr)
    {
    }

    // Access coefficient at a given row and column
    typename XprType::Scalar coeff(Index row, Index col) const
    {
        return expr_.coeff(row, col);
    }

    // Access coefficient at a given index (for vectors or linear access)
    typename XprType::Scalar coeff(Index index) const
    {
        // Assuming linear access; modify if your matrix uses a different storage order
        return expr_.coeff(index / expr_.cols(), index % expr_.cols());
    }

    // Get the number of rows
    Index rows() const { return expr_.rows(); }

    // Get the number of columns
    Index cols() const { return expr_.cols(); }

    // Get the total size (for vectors or linear access)
    Index size() const { return expr_.size(); }

private:

    XprType expr_;
};



} // namespace internal
} // namespace Eigen
//-------------------------------------------------------------------



#endif // INCLUDE_EIGEN_WRAPPERS_HPP_