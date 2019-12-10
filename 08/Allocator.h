//
// Created by admin2 on 09.12.2019.
//

#ifndef HW8_ALLOCATOR_H
#define HW8_ALLOCATOR_H

#include <cstdlib>
#include <limits>

template <class T>
class Allocator {
public:
    using value_type = T;
    using pointer = T*;
    using size_type = std::size_t;

    pointer allocate(size_type count);
    void deallocate(pointer ptr, size_type count);

    [[nodiscard]] size_type max_size() const noexcept {
        return std::numeric_limits<std::size_t>::max() / sizeof(T);
    }

};

template<class T>
typename Allocator<T>::pointer Allocator<T>::allocate(Allocator::size_type count) {
    return static_cast<T*>(::operator new[] (count * sizeof(T)));
}

template<class T>
void Allocator<T>::deallocate(Allocator::pointer ptr, Allocator::size_type count) {
    if (ptr != nullptr) {
        ::operator delete[] (static_cast<void*>(ptr));
    }
}

#endif //HW8_ALLOCATOR_H
