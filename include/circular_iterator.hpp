//-------------------------------------------------------------------
/**
 * @file circular_iterator.hpp
 * @brief Defines a circular iterator for containers in LazyMatrix library.
 *
 * This file contains the CircularIterator template class, which provides an iterator 
 * that can iterate over a container in a circular manner for a specified number of rotations.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_CIRCULAR_ITERATOR_HPP_
#define INCLUDE_CIRCULAR_ITERATOR_HPP_



//-------------------------------------------------------------------
#include <iterator>
#include <vector>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief A circular iterator for iterating over containers.
 * 
 * @tparam ContainerType The type of container over which to iterate.
 */
//-------------------------------------------------------------------
template<typename ContainerType>

class CircularIterator
{
public:

    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;

    using pointer = typename std::remove_reference<decltype(std::begin(std::declval<ContainerType>()))>::type;
    using value_type = typename std::remove_reference<decltype(*std::begin(std::declval<ContainerType>()))>::type;
    using reference = std::reference_wrapper<value_type>;



    /**
     * @brief Constructs a circular iterator for a given container.
     * 
     * @param container Reference to the container to iterate over.
     * @param maximum_number_of_rotations The maximum number of complete rotations to make over the container.
     */
    CircularIterator(ContainerType& container, std::ptrdiff_t maximum_number_of_rotations)
    : container_(container),
      current_pointer_(std::begin(container)),
      begin_pointer_(std::begin(container)),
      end_pointer_(std::end(container)),
      maximum_number_of_rotations_(std::abs(maximum_number_of_rotations))
    {
    }



    // Comparison operators

    bool operator==(const CircularIterator& circular_iterator)
    {
        return current_pointer_ == circular_iterator.get_current_pointer();
    }

    bool operator!=(const CircularIterator& circular_iterator)
    {
        return current_pointer_ != circular_iterator.get_current_pointer();
    }



    // Conversion to boolean
    explicit operator bool()const
    {
        return current_pointer_;
    }



    // Dereference operators
    reference operator*(){return (*current_pointer_);}
    const reference operator*()const{return (*current_pointer_);}
    pointer operator->(){return current_pointer_;}



    // Accessor methods
    pointer get_current_pointer()const { return current_pointer_; }
    pointer get_begin_pointer()const { return begin_pointer_; }
    pointer get_end_pointer()const { return end_pointer_; }

    int get_current_number_of_rotations()const { return current_number_of_rotations_; }
    int get_maximum_number_of_rotations()const { return maximum_number_of_rotations_; }



    // Set the iterator to the beginning or the end of its cycle
    void set_to_begin()
    {
        current_pointer_ = begin_pointer_;
        current_number_of_rotations_ = 0;
    }

    void set_to_end()
    {
        current_pointer_ = end_pointer_;
        current_number_of_rotations_ = maximum_number_of_rotations_;
    }



    // Methods to get iterators to the beginning or end
    CircularIterator<ContainerType> begin()const
    {
        CircularIterator<ContainerType> copied_iterator = (*this);
        copied_iterator.set_to_begin();
        return copied_iterator;
    }

    CircularIterator<ContainerType> end()const
    {
        CircularIterator<ContainerType> copied_iterator = (*this);
        copied_iterator.set_to_end();
        return copied_iterator;
    }



    // Returns the size of the underlying container
    std::ptrdiff_t size()const
    {
        return container_.size();
    }


    
    // Increment and decrement operators
    CircularIterator<ContainerType>& operator++()
    {
        if(current_pointer_ == end_pointer_)
            return (*this);

        ++current_pointer_;
        ++current_position_accumulative_;
        ++current_position_in_container_;

        current_number_of_rotations_ = current_position_accumulative_ / this->size();

        if(current_number_of_rotations_ >= maximum_number_of_rotations_)
        {
            current_pointer_ = end_pointer_;
        }
        else if(current_pointer_ == end_pointer_)
        {
            current_pointer_ = begin_pointer_;
            current_position_in_container_ = 0;
        }
        
        return (*this);
    }

    CircularIterator<ContainerType>& operator--()
    {
        if(current_pointer_ == end_pointer_)
            return (*this);
    
        if(current_pointer_ == begin_pointer_)
        {
            current_pointer_ = end_pointer_;
            --current_pointer_;
            --current_position_accumulative_;
            current_position_in_container_ = this->size() - 1;
        }
        else
        {
            --current_pointer_;
            --current_position_accumulative_;
            --current_position_in_container_;
        }

        current_number_of_rotations_ = current_position_accumulative_ / this->size();

        if(current_number_of_rotations_ <= -maximum_number_of_rotations_)
            current_pointer_ = end_pointer_;
        
        return (*this);
    }



    // Compound assignment operators
    CircularIterator<ContainerType>& operator+=(std::ptrdiff_t movement)
    {
        return this->advance(movement);
    }

    CircularIterator<ContainerType>& operator-=(std::ptrdiff_t movement)
    {
        return this->advance(-movement);
    }



private: // Private functions

    // Advances the iterator by a specified number of steps
    CircularIterator<ContainerType>& advance(std::ptrdiff_t movement)
    {
        if(current_pointer_ == end_pointer_)
            return (*this);

        current_position_accumulative_ += movement;
        current_number_of_rotations_ = current_position_accumulative_ / this->size();

        if(current_number_of_rotations_ >= maximum_number_of_rotations_ || current_number_of_rotations_ <= -maximum_number_of_rotations_)
        {
            current_pointer_ = end_pointer_;
            return (*this);
        }

        std::ptrdiff_t desired_position_in_container = (this->size() + current_position_accumulative_ % this->size()) % this->size();
        
        if(std::abs(desired_position_in_container - current_position_in_container_) <= (this->size() / 2))
        {
            std::advance(current_pointer_, desired_position_in_container - current_position_in_container_);
        }
        else if(desired_position_in_container < current_position_in_container_)
        {
            current_pointer_ = begin_pointer_;
            std::advance(current_pointer_, desired_position_in_container);
        }
        else
        {
            current_pointer_ = end_pointer_;
            std::advance(current_pointer_, desired_position_in_container - this->size());
        }

        current_position_in_container_ = desired_position_in_container;

        return (*this);
    }



private: // Private variables

    ContainerType& container_;

    pointer current_pointer_ = nullptr;
    pointer begin_pointer_ = nullptr;
    pointer end_pointer_ = nullptr;

    std::ptrdiff_t current_position_accumulative_ = 0;
    std::ptrdiff_t current_position_in_container_ = 0;

    std::ptrdiff_t current_number_of_rotations_ = 0;
    std::ptrdiff_t maximum_number_of_rotations_ = 1;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Utility functions to facilitate creating a circular iterator
//-------------------------------------------------------------------
template<typename ContainerType>

inline auto get_circulat_iterator(ContainerType& container, std::ptrdiff_t maximum_number_of_rotations)
{
    return CircularIterator<ContainerType>(container, maximum_number_of_rotations);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_CIRCULAR_ITERATOR_HPP_
