//
// Created by admin2 on 07.12.2019.
//

#ifndef HW8_VECTOR_H
#define HW8_VECTOR_H

#include <iterator>
#include "Iterator.h"
#include "Allocator.h"

template <class T, class Alloc>
class Vector
{
public:
    using iterator = Iterator<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using size_type = size_t;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using allocator_type = Alloc;
    using pointer = T*;
    using const_pointer = const T*;


    explicit Vector(size_type count = 0):
       Vector(count, T())
    {
    }

    explicit Vector(size_type count, const value_type& default_value):
        alloc_(Alloc()),
        capacity_(count),
        size_(count),
        buffer_(nullptr)
    {
        if (size_ > 0) {
            buffer_ = alloc_.allocate(capacity_);
            for (size_type i = 0; i < size_; ++i)
                new(buffer_ + i) T(default_value);
        }
    }

    Vector(const std::initializer_list<value_type>& init):
        alloc_(Alloc()),
        capacity_(init.size()),
        size_(init.size()),
        buffer_(nullptr)
    {
        if (size_ > 0) {
            buffer_ = alloc_.allocate(capacity_);
            size_type i = 0;
            for (auto it = init.begin(); it != init.end(); ++it, ++i)
                new(buffer_ + i) T(*it);
        }
    }

    Vector(const Vector& obj):
        alloc_(obj.alloc_),
        capacity_(obj.capacity_),
        size_(obj.size_),
        buffer_(nullptr)
    {
        allocate_and_copy_from(obj);
    }

    Vector(Vector&& obj) noexcept
    {
        move_from(std::move(obj));
    }

    ~Vector() {
        free_buffer();
    }

    Vector& operator = (const Vector& obj);

    Vector& operator = (Vector&& obj) noexcept;

    bool operator == (const Vector& obj) const;
    bool operator != (const Vector& obj) const;

    iterator begin() {
        return Iterator<T>(buffer_);
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }
    iterator end() {
        return Iterator<T>(buffer_ + size_);
    }
    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    T& operator [] (size_type index) {
        return buffer_[index];
    }
    const T& operator [] (size_type index) const {
        return buffer_[index];
    }

    T& at(size_type index) {
        if (index >= size_)
            throw std::out_of_range("index is out of range");
        return buffer_[index];
    }

    const T& at(size_type index) const {
        if (index >= size_)
            throw std::out_of_range("index is out of range");
        return buffer_[index];
    }

    void push_back(value_type&& value);

    void push_back(const value_type& value);

    void pop_back();

    [[nodiscard]] std::size_t size() const {
        return size_;
    }

    [[nodiscard]] bool empty() const {
        return size_ == 0;
    }

    void reserve(size_type count) {
        if (count > capacity_)
            move_to_new_buffer_of_size(count);
    }

    [[nodiscard]] size_type capacity() const {
        return capacity_;
    }

    void resize(size_type new_size) {
        resize(new_size, T());
    }
    void resize(size_type new_size, const value_type& default_value);

    void clear() {
        resize(0);
    }

    const_pointer buffer_pointer() const {
        return buffer_;
    }

private:
    Alloc alloc_;
    size_type capacity_;
    size_type size_;
    T* buffer_;

    void move_from(Vector&& obj);

    void allocate_and_copy_from(const Vector& obj);

    void move_to_new_buffer_of_size(size_type new_capacity);

    void grow_memory() {
        move_to_new_buffer_of_size(std::max<size_type>(capacity_ * 2, 1));
    }

    void free_buffer();

};

template<class T, class Alloc>
Vector<T, Alloc>& Vector<T, Alloc>::operator = (const Vector& obj) {
    if (this == &obj)
        return *this;

    free_buffer();
    allocate_and_copy_from(obj);

    return *this;
}

template<class T, class Alloc>
Vector<T, Alloc>& Vector<T, Alloc>::operator = (Vector&& obj) noexcept {
    if (this == &obj)
        return *this;

    free_buffer();
    move_from(std::move(obj));

    return *this;
}

template<class T, class Alloc>
bool Vector<T, Alloc>::operator == (const Vector& obj) const {
    if (size_ != obj.size_)
        return false;
    for(size_type i = 0; i < size_; ++i)
        if (!(buffer_[i] == obj.buffer_[i]))
            return false;
    return true;
}

template<class T, class Alloc>
bool Vector<T, Alloc>::operator != (const Vector& obj) const {
    return !(*this == obj);
}

template<class T, class Alloc>
void Vector<T, Alloc>::push_back(const value_type& value) {
    if (size_ == capacity_)
        grow_memory();

    new (buffer_ + size_) T(value);
    size_++;
}

template<class T, class Alloc>
void Vector<T, Alloc>::push_back(value_type&& value) {
    if (size_ == capacity_)
        grow_memory();

    new (buffer_ + size_) T(value);
    size_++;
}

template<class T, class Alloc>
void Vector<T, Alloc>::pop_back() {
    if (empty())
        throw std::underflow_error("Vector is empty");
    (buffer_ + size_)->~T();
    size_--;
}

template<class T, class Alloc>
void Vector<T, Alloc>::resize(Vector::size_type new_size, const value_type& default_value) {
    if (new_size < size_) {
        for (size_type i = new_size; i < size_; ++i)
            (buffer_ + i)->~T();
        size_ = new_size;
    } else if (new_size > size_) {
        if (new_size > capacity_)
            move_to_new_buffer_of_size(new_size);
        for (size_type i = size_; i < new_size; ++i)
            new (buffer_ + i) T(default_value);
        size_ = new_size;
    }
}

template<class T, class Alloc>
void Vector<T, Alloc>::move_from(Vector&& obj) {
    alloc_ = std::move(obj.alloc_);
    capacity_ = obj.capacity_;
    size_ = obj.size_;
    buffer_ = obj.buffer_;
    obj.capacity_ = 0;
    obj.size_ = 0;
    obj.buffer_ = nullptr;
}

template<class T, class Alloc>
void Vector<T, Alloc>::allocate_and_copy_from(const Vector& obj) {
    alloc_ = obj.alloc_;
    capacity_ = obj.capacity_;
    size_ = obj.size_;

    buffer_ = alloc_.allocate(capacity_);
    for (size_type i = 0; i < size_; ++i)
        new (buffer_ + i) T(obj.buffer_[i]);
}

template<class T, class Alloc>
void Vector<T, Alloc>::move_to_new_buffer_of_size(Vector::size_type new_capacity) {
    if (new_capacity == capacity_)
        return;

    size_type new_size_ = std::min(size_, new_capacity);

    if (new_capacity != 0) {
        T* new_buffer = alloc_.allocate(new_capacity);
        for (size_type i = 0; i < new_size_; ++i) {
            new (new_buffer + i) T(buffer_[i]);
            (buffer_ + i)->~T();
        }
        free_buffer();
        buffer_ = new_buffer;
    } else {
        free_buffer();
    }
    capacity_ = new_capacity;
    size_ = new_size_;
}

template<class T, class Alloc>
void Vector<T, Alloc>::free_buffer() {
    resize(0);
    alloc_.deallocate(buffer_, capacity_);
    capacity_ = 0;
    buffer_ = nullptr;
}


#endif //HW8_VECTOR_H
