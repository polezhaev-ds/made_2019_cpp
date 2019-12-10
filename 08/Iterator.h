//
// Created by admin2 on 09.12.2019.
//

#ifndef HW8_ITERATOR_H
#define HW8_ITERATOR_H

#include <cstdint>
#include <iterator>
#include "Vector.h"
#include "Allocator.h"

template <class T, class Alloc = Allocator<T>>
class Vector;

template <class T>
class Iterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = std::size_t;
    using difference_type = std::int64_t;

    explicit Iterator(pointer ptr):
            pointer_(ptr)
    {
    }

    Iterator(const Iterator& it) = default;

    Iterator(Iterator&& it) noexcept:
        pointer_(it.pointer_)
    {
        it.pointer_ = nullptr;
    }

    Iterator& operator = (const Iterator& it) = default;

    Iterator& operator = (Iterator&& it) noexcept {
        if (this == &it)
            return *this;
        pointer_ = it.pointer_;
        it.pointer_ = nullptr;
        return *this;
    };

    reference operator * () {
        return *pointer_;
    }

    const_reference operator * () const {
        return *pointer_;
    }

    pointer operator -> () {
        return pointer_;
    }

    const_pointer operator -> () const {
        return pointer_;;
    }

    Iterator& operator ++ () {
        pointer_++;
        return *this;
    }

    Iterator operator ++ (int) {
        Iterator it = *this;
        pointer_++;
        return it;
    }

    Iterator& operator -- () {
        pointer_--;
        return *this;
    }

    Iterator operator -- (int) {
        Iterator it = *this;
        pointer_--;
        return it;
    }

    bool operator == (const Iterator& it) const {
        return pointer_ == it.pointer_;
    }

    bool operator != (const Iterator& it) const {
        return pointer_ != it.pointer_;
    }

    bool operator < (const Iterator& it) const {
        return pointer_ < it.pointer_;
    }

    bool operator > (const Iterator& it) const {
        return pointer_ > it.pointer_;
    }

    bool operator <= (const Iterator& it) const {
        return pointer_ <= it.pointer_;
    }

    bool operator >= (const Iterator& it) const {
        return pointer_ >= it.pointer_;
    }

    Iterator operator + (difference_type n) const {
        Iterator it(*this);
        it += n;
        return it;
    }

    Iterator operator - (difference_type n) const {
        Iterator it(*this);
        it -= n;
        return it;
    }

    difference_type operator - (const Iterator& it) const {
        return pointer_ - it.pointer_;
    }

    Iterator& operator += (difference_type n) {
        pointer_ += n;
        return *this;
    }

    Iterator& operator -= (difference_type n) {
        pointer_ -= n;
        return *this;
    }

    reference operator[] (difference_type n) {
        return *(*this + n);
    }

    const_reference operator[] (difference_type n) const {
        return *(*this + n);
    }

    void swap(Iterator& it)
    {
        std::swap(pointer_, it.pointer_);
    }

    const_pointer data_pointer() const {
        return pointer_;
    }

private:
    T* pointer_;;
};

template <class T>
void swap(Iterator<T>& x, Iterator<T>& y) {
    x.swap(y);
}

#endif //HW8_ITERATOR_H
