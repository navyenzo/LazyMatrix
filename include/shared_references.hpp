//-------------------------------------------------------------------
/**
 * @file shared_references.hpp
 * @brief Defines the smart shared references that can be used instead
 *        of raw references to eliminate the danger of dangling references.
 *        These smart references are used throughout the LazyMatrix library
 *        to build complex matrix expressions with lazy evaluation
 * 
 * The classes defined in here act like smart pointers, but allow users to
 * access functions of the pointed to objects using "." instead of "->".
 * They are smart references to 2d-matrix-like and 3d-matrix-like objects
 * 
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef SHARED_REFERENCES_HPP
#define SHARED_REFERENCES_HPP



//-------------------------------------------------------------------
#include <memory>
#include <type_traits>

#include "base_matrix.hpp"
#include "base_matrix3d.hpp"
#include "numerical_constants.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Trait to check if type allows for non-const access
 * @tparam T The type in question
 */
template<typename T>
struct has_non_const_access : std::false_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Trait to check if a reference type is a shared matrix ref type.
 * @tparam T The reference type (templated with MatrixType) to check.
 */
//-------------------------------------------------------------------
template<typename T>
struct is_matrix_reference : std::false_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Trait to check if a reference type is a shared 3d matrix ref type.
 * @tparam T The reference type (templated with MatrixType) to check.
 */
//-------------------------------------------------------------------
template<typename T>
struct is_matrix3d_reference : std::false_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class ConstSharedMatrixRef
 * @brief A smart pointer-like wrapper for shared references to matrix objects (with const access only).
 * 
 * @tparam MatrixType The type of the underlying matrix object.
 * 
 * ConstSharedMatrixRef acts as a proxy to MatrixType, forwarding member function calls
 * to the underlying object. It manages the lifetime of the matrix objects, allowing
 * for safe and efficient lazy evaluation in matrix operations. This only
 * allows for const access and does not allow non-const access.
 */
//-------------------------------------------------------------------
template <typename MatrixType>

class ConstSharedMatrixRef
{
public:

    // Type of value that is stored in the matrix
    using value_type = typename MatrixType::value_type;
    using matrix_type = MatrixType;

    /**
     * @brief Constructs a SharedMatrixRef object.
     * @param ptr A shared pointer to the underlying matrix object.
     */
    explicit ConstSharedMatrixRef(std::shared_ptr<MatrixType> ptr = nullptr) : ptr_(ptr)
    {
    }

    /**
     * @brief Default copy constructor
     */
    ConstSharedMatrixRef(const ConstSharedMatrixRef<MatrixType>& shared_matrix_ref) = default;

    /**
     * @brief Default assignment operator
     */
    ConstSharedMatrixRef& operator=(const ConstSharedMatrixRef<MatrixType>&) = default;

    /**
     * @brief Bool operator to check if reference is valid
     */
    explicit operator bool() const
    {
        return static_cast<bool>(ptr_);
    }

    /**
     * @brief Dereference operator to access the underlying matrix object.
     * @return A reference to the underlying matrix object.
     */
    MatrixType& operator*()
    {
        return *ptr_;
    }

    /**
     * @brief Member access operator to access members of the underlying matrix object.
     * @return A pointer to the underlying matrix object.
     */
    MatrixType* operator->()
    {
        return ptr_.get();
    }

    /**
     * @brief Forwards the call to the rows() method of the underlying matrix.
     * @return The number of rows in the matrix.
     */
    uintptr_t rows() const
    {
        return ptr_->rows();
    }

    /**
     * @brief Forwards the call to the columns() method of the underlying matrix.
     * @return The number of columns in the matrix.
     */
    uintptr_t columns() const
    {
        return ptr_->columns();
    }

    /**
     * @brief Forwards the call to the size() method of the underlying matrix.
     * @return The total number of elements in the matrix.
     */
    uintptr_t size() const
    {
        return ptr_->size();
    }

    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A copy of the value of the element at the specified position.
     */
    value_type at(int64_t row, int64_t column) const
    {
        return ptr_->at(row, column);
    }

    /**
     * @brief Forwards the call to the at() method of the underlying matrix.
     * @param index The data index (treats 2d matrix as 1d vector).
     * @return A copy of the value of the element at the specified position.
     */
    value_type at(int64_t index) const
    {
        return ptr_->at(index);
    }

    /**
     * @brief Forwards the call to the operator() method of the underlying matrix.
     * @param row The row index.
     * @param column The column index.
     * @return A copy of the value of the element at the specified position.
     */
    value_type operator()(int64_t row, int64_t column) const
    {
        return ptr_->at(row, column);
    }

    /**
     * @brief Forwards the call to the operator() method of the underlying matrix.
     * @param index The data index (treats 2d matrix as 1d vector).
     * @return A copy of the value of the element at the specified position.
     */
    value_type operator()(int64_t index) const
    {
        return ptr_->at(index);
    }

    /**
     * @brief Forwards the call to the circ_at() method of the underlying matrix for circular access.
     * @param row The row index.
     * @param column The column index.
     * @return A copy of the value of the element at the specified position.
     */
    value_type circ_at(int64_t row, int64_t column) const
    {
        return ptr_->circ_at(row, column);
    }

    /**
     * @brief Forwards the call to the circ_at() method of the underlying matrix for circular access.
     * @param index The data index (treats 2d matrix as 1d vector).
     * @return A copy of the value of the element at the specified position.
     */
    value_type circ_at(int64_t index) const
    {
        return ptr_->circ_at(index);
    }

    /**
     * @brief Return underlying shared_ptr pointer
     */
    std::shared_ptr<MatrixType> get_ptr()const
    {
        return ptr_;
    }



protected:

    std::shared_ptr<MatrixType> ptr_; ///< A shared pointer to the underlying matrix object.
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Let compiler know that ConstSharedMatrixRef does not allow
// non-const access
//-------------------------------------------------------------------
template<typename MatrixType>

struct has_non_const_access< ConstSharedMatrixRef<MatrixType> > : std::false_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Let compiler know that ConstSharedMatrixRef is a shared matrix
 *        reference type
 * @tparam T The reference type (templated with MatrixType) to check.
 */
//-------------------------------------------------------------------
template<typename MatrixType>
struct is_matrix_reference<ConstSharedMatrixRef<MatrixType>> : is_type_a_matrix<MatrixType>
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class SharedMatrixRef
 * @brief A smart pointer-like wrapper for shared references to matrix objects.
 * 
 * @tparam MatrixType The type of the underlying matrix object.
 * 
 * SharedMatrixRef acts as a proxy to MatrixType, forwarding member function calls
 * to the underlying object. It manages the lifetime of the matrix objects, allowing
 * for safe and efficient lazy evaluation in matrix operations.
 */
//-------------------------------------------------------------------
template <typename MatrixType>

class SharedMatrixRef : public ConstSharedMatrixRef<MatrixType>
{
public:

    // Type of value that is stored in the matrix
    using value_type = typename MatrixType::value_type;
    using matrix_type = MatrixType;

    // Bring const access member functions into scope
    using ConstSharedMatrixRef<MatrixType>::at;
    using ConstSharedMatrixRef<MatrixType>::circ_at;
    using ConstSharedMatrixRef<MatrixType>::operator();

    /**
     * @brief Constructs a SharedMatrixRef object.
     * @param ptr A shared pointer to the underlying matrix object.
     */
    explicit SharedMatrixRef(std::shared_ptr<MatrixType> ptr = nullptr)
    : ConstSharedMatrixRef<MatrixType>(ptr)
    {
    }

    /**
     * @brief Default copy constructor
     */
    SharedMatrixRef(const SharedMatrixRef<MatrixType>& shared_matrix_ref) = default;

    /**
     * @brief Default assignment operator
     */
    SharedMatrixRef& operator=(const SharedMatrixRef<MatrixType>&) = default;

    /**
     * @brief Forwards the call to the at() method of the underlying matrix.
     * @param row The row index.
     * @param column The column index.
     * @return A reference to the matrix element at the specified position.
     */
    value_type& at(int64_t row, int64_t column)
    {
        return this->ptr_->at(row, column);
    }

    /**
     * @brief Forwards the call to the at() method of the underlying matrix.
     * @param index The data index (treats 2d matrix as 1d vector).
     * @return A const reference to the matrix element at the specified position.
     */
    value_type& at(int64_t index)
    {
        return this->ptr_->at(index);
    }

    /**
     * @brief Forwards the call to the operator() method of the underlying matrix.
     * @param row The row index.
     * @param column The column index.
     * @return A reference to the matrix element at the specified position.
     */
    value_type& operator()(int64_t row, int64_t column)
    {
        return this->ptr_->at(row, column);
    }

    /**
     * @brief Forwards the call to the operator() method of the underlying matrix.
     * @param index The data index (treats 2d matrix as 1d vector).
     * @return A const reference to the matrix element at the specified position.
     */
    value_type& operator()(int64_t index)
    {
        return this->ptr_->at(index);
    }

    /**
     * @brief Forwards the call to the circ_at() method of the underlying matrix for circular access.
     * @param row The row index.
     * @param column The column index.
     * @return A reference to the matrix element at the circularly adjusted position.
     */
    value_type& circ_at(int64_t row, int64_t column)
    {
        return this->ptr_->circ_at(row, column);
    }

    /**
     * @brief Forwards the call to the circ_at() method of the underlying matrix for circular access.
     * @param index The data index (treats 2d matrix as 1d vector).
     * @return A reference to the matrix element at the circularly adjusted position.
     */
    value_type& circ_at(int64_t index)
    {
        return this->ptr_->circ_at(index);
    }
    
    /**
     * @brief This function is used for python bindings, it sets the value at
     *        the specified position (using circular indexing).
     * 
     * @tparam ValueType 
     * @param row 
     * @param column 
     * @param value 
     */
    void set_circ_at(int64_t row, int64_t column, value_type value)
    {
        this->ptr_->set_circ_at(row, column, value);
    }

    /**
     * @brief Resizes the underlying matrix expression (if possible).
     * 
     * @param rows 
     * @param columns 
     * @return std::error_code 
     */
    std::error_code resize(uintptr_t rows, uintptr_t columns)
    {
        return this->ptr_->resize(rows, columns);
    }
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Let compiler know that ConstSharedMatrixRef allows non-const access
//-------------------------------------------------------------------
template<typename MatrixType>

struct has_non_const_access< SharedMatrixRef<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Let compiler know that SharedMatrixRef is a shared matrix reference type
 * @tparam T The reference type (templated with MatrixType) to check.
 */
//-------------------------------------------------------------------
template<typename T>
struct is_matrix_reference<SharedMatrixRef<T>> : is_type_a_matrix<T>
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class ConstSharedMatrix3DRef
 * @brief A smart pointer-like wrapper for shared references to 3D matrix objects (with const access only).
 * 
 * @tparam MatrixType The type of the underlying 3D matrix object.
 * 
 * ConstSharedMatrix3DRef acts as a proxy to MatrixType, forwarding member function calls
 * to the underlying object. It manages the lifetime of the 3D matrix objects, allowing
 * for safe and efficient lazy evaluation in 3D matrix operations. This only
 * allows for const access and does not allow non-const access.
 */
//-------------------------------------------------------------------
template <typename MatrixType>

class ConstSharedMatrix3DRef
{
public:

    // Type of value that is stored in the matrix
    using value_type = typename MatrixType::value_type;
    using matrix_type = MatrixType;

    /**
     * @brief Constructs a SharedMatrix3DRef object.
     * @param ptr A shared pointer to the underlying matrix object.
     */
    explicit ConstSharedMatrix3DRef(std::shared_ptr<MatrixType> ptr = nullptr) : ptr_(ptr)
    {
    }

    /**
     * @brief Default copy constructor
     */
    ConstSharedMatrix3DRef(const ConstSharedMatrix3DRef<MatrixType>& shared_matrix3d_ref) = default;

    /**
     * @brief Default assignment operator
     */
    ConstSharedMatrix3DRef& operator=(const ConstSharedMatrix3DRef<MatrixType>&) = default;

    /**
     * @brief Bool operator to check if reference is valid
     */
    explicit operator bool() const
    {
        return static_cast<bool>(ptr_);
    }
    
    /**
     * @brief Dereference operator to access the underlying 3d matrix object.
     * @return A reference to the underlying matrix object.
     */
    MatrixType& operator*()
    {
        return *ptr_;
    }

    /**
     * @brief Member access operator to access members of the underlying 3d matrix object.
     * @return A pointer to the underlying matrix object.
     */
    MatrixType* operator->()
    {
        return ptr_.get();
    }

    /**
     * @brief Forwards the call to the pages() method of the underlying matrix.
     * @return The number of rows in the matrix.
     */
    uintptr_t pages() const
    {
        return ptr_->pages();
    }

    /**
     * @brief Forwards the call to the rows() method of the underlying matrix.
     * @return The number of rows in the matrix.
     */
    uintptr_t rows() const
    {
        return ptr_->rows();
    }

    /**
     * @brief Forwards the call to the columns() method of the underlying matrix.
     * @return The number of columns in the matrix.
     */
    uintptr_t columns() const
    {
        return ptr_->columns();
    }

    /**
     * @brief Forwards the call to the size() method of the underlying matrix.
     * @return The total number of elements in the matrix.
     */
    uintptr_t size() const
    {
        return ptr_->size();
    }

    /**
     * @brief Forwards the call to the at() method of the underlying matrix.
     * @param page The page index.
     * @param row The row index.
     * @param column The column index.
     * @return A copy of the value of the element at the specified position.
     */
    value_type at(int64_t page, int64_t row, int64_t column) const
    {
        return ptr_->at(page, row, column);
    }

    /**
     * @brief Forwards the call to the at() method of the underlying matrix.
     * @param index The data index (treats 2d matrix as 1d vector).
     * @return A copy of the value of the element at the specified position.
     */
    value_type at(int64_t index) const
    {
        return ptr_->at(index);
    }

    /**
     * @brief Forwards the call to the operator() method of the underlying matrix.
     * @param page The page index.
     * @param row The row index.
     * @param column The column index.
     * @return A copy of the value of the element at the specified position.
     */
    value_type operator()(int64_t page, int64_t row, int64_t column) const
    {
        return ptr_->at(page, row, column);
    }

    /**
     * @brief Forwards the call to the operator() method of the underlying matrix.
     * @param index The data index (treats 2d matrix as 1d vector).
     * @return A copy of the value of the element at the specified position.
     */
    value_type operator()(int64_t index) const
    {
        return ptr_->at(index);
    }

    /**
     * @brief Forwards the call to the circ_at() method of the underlying matrix for circular access.
     * @param page The page index.
     * @param row The row index.
     * @param column The column index.
     * @return A copy of the value of the element at the specified position.
     */
    value_type circ_at(int64_t page, int64_t row, int64_t column) const
    {
        return ptr_->circ_at(page, row, column);
    }

    /**
     * @brief Forwards the call to the circ_at() method of the underlying matrix for circular access.
     * @param index The data index (treats 2d matrix as 1d vector).
     * @return A copy of the value of the element at the specified position.
     */
    value_type circ_at(int64_t index) const
    {
        return ptr_->circ_at(index);
    }

    /**
     * @brief Return underlying shared_ptr pointer
     */
    std::shared_ptr<MatrixType> get_ptr()const
    {
        return ptr_;
    }



protected:

    std::shared_ptr<MatrixType> ptr_; ///< A shared pointer to the underlying 3D matrix object.
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Let compiler know that SharedMatrix3DRef does not allow non-const access
//-------------------------------------------------------------------
template<typename MatrixType>

struct has_non_const_access< ConstSharedMatrix3DRef<MatrixType> > : std::false_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Let compiler know that ConstSharedMatrix3DRef is a shared 3d matrix reference type
 * @tparam T The reference type (templated with MatrixType) to check.
 */
//-------------------------------------------------------------------
template<typename T>
struct is_matrix3d_reference<ConstSharedMatrix3DRef<T>> : is_type_a_matrix3d<T>
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class SharedMatrix3DRef
 * @brief A smart pointer-like wrapper for shared references to 3D matrix objects.
 * 
 * @tparam MatrixType The type of the underlying 3D matrix object.
 * 
 * SharedMatrix3DRef acts as a proxy to MatrixType, forwarding member function calls
 * to the underlying object. It manages the lifetime of the 3D matrix objects, allowing
 * for safe and efficient lazy evaluation in 3D matrix operations.
 */
//-------------------------------------------------------------------
template <typename MatrixType>

class SharedMatrix3DRef : public ConstSharedMatrix3DRef<MatrixType>
{
public:

    // Type of value that is stored in the matrix
    using value_type = typename MatrixType::value_type;
    using matrix_type = MatrixType;

    // Bring const access member functions into scope
    using ConstSharedMatrix3DRef<MatrixType>::at;
    using ConstSharedMatrix3DRef<MatrixType>::circ_at;
    using ConstSharedMatrix3DRef<MatrixType>::operator();

    /**
     * @brief Constructs a SharedMatrix3DRef object.
     * @param ptr A shared pointer to the underlying matrix object.
     */
    explicit SharedMatrix3DRef(std::shared_ptr<MatrixType> ptr = nullptr)
    : ConstSharedMatrix3DRef<MatrixType>(ptr)
    {
    }

    /**
     * @brief Default copy constructor
     */
    SharedMatrix3DRef(const SharedMatrix3DRef<MatrixType>& shared_matrix3d_ref) = default;

    /**
     * @brief Default assignment operator
     */
    SharedMatrix3DRef& operator=(const SharedMatrix3DRef<MatrixType>&) = default;

    /**
     * @brief Forwards the call to the at() method of the underlying matrix.
     * @param page The page index.
     * @param row The row index.
     * @param column The column index.
     * @return A reference to the matrix element at the specified position.
     */
    value_type& at(int64_t page, int64_t row, int64_t column)
    {
        return this->ptr_->at(page, row, column);
    }

    /**
     * @brief Forwards the call to the at() method of the underlying matrix.
     * @param index The data index (treats 2d matrix as 1d vector).
     * @return A const reference to the matrix element at the specified position.
     */
    value_type& at(int64_t index)
    {
        return this->ptr_->at(index);
    }

    /**
     * @brief Forwards the call to the operator() method of the underlying matrix.
     * @param page The page index.
     * @param row The row index.
     * @param column The column index.
     * @return A reference to the matrix element at the specified position.
     */
    value_type& operator()(int64_t page, int64_t row, int64_t column)
    {
        return this->ptr_->at(page, row, column);
    }

    /**
     * @brief Forwards the call to the operator() method of the underlying matrix.
     * @param index The data index (treats 2d matrix as 1d vector).
     * @return A const reference to the matrix element at the specified position.
     */
    value_type& operator()(int64_t index)
    {
        return this->ptr_->at(index);
    }

    /**
     * @brief Forwards the call to the circ_at() method of the underlying matrix for circular access.
     * @param page The page index.
     * @param row The row index.
     * @param column The column index.
     * @return A reference to the matrix element at the circularly adjusted position.
     */
    value_type& circ_at(int64_t page, int64_t row, int64_t column)
    {
        return this->ptr_->circ_at(page, row, column);
    }

    /**
     * @brief Forwards the call to the circ_at() method of the underlying matrix for circular access.
     * @param index The data index (treats 2d matrix as 1d vector).
     * @return A reference to the matrix element at the circularly adjusted position.
     */
    value_type& circ_at(int64_t index)
    {
        return this->ptr_->circ_at(index);
    }

    /**
     * @brief This function is used for python bindings, it sets the value at
     *        the specified position (using circular indexing).
     * 
     * @tparam ValueType 
     * @param page
     * @param row 
     * @param column 
     * @param value 
     */
    void set_circ_at(int64_t page, int64_t row, int64_t column, value_type value)
    {
        this->ptr_->set_circ_at(page, row, column, value);
    }

    /**
     * @brief Resizes underlying matrix expression (if possible).
     * 
     * @param pages 
     * @param rows 
     * @param columns 
     * @return std::error_code 
     */
    
    std::error_code resize(uintptr_t pages, uintptr_t rows, uintptr_t columns)
    {
        return this->ptr_->resize(pages, rows, columns);
    }
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Let compiler know that SharedMatrix3DRef allows non-const access
//-------------------------------------------------------------------
template<typename MatrixType>

struct has_non_const_access< SharedMatrix3DRef<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Let compiler know that SharedMatrix3DRef is a shared 3d matrix reference type
 * @tparam T The reference type (templated with MatrixType) to check.
 */
//-------------------------------------------------------------------
template<typename T>
struct is_matrix3d_reference<SharedMatrix3DRef<T>> : is_type_a_matrix3d<T>
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif // SHARED_REFERENCES_HPP
