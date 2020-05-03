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
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal =  std::true_type;

    template <class U>
    struct rebind {
        using other = Allocator<U>;
    };


    pointer allocate(size_type count);
    void deallocate(pointer ptr, size_type count);

    template <class... Args>
    void construct(pointer ptr, Args&&... args);

    void destroy(pointer ptr);

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

template<class T>
template<class... Args>
void Allocator<T>::construct(Allocator::pointer ptr, Args&&... args) {
    new (ptr) T(std::forward<Args>(args)...);
}

template <class T>
void Allocator<T>::destroy(Allocator::pointer ptr) {
    ptr->~T();
}



template <class T1, class T2>
bool operator == (const Allocator<T1>& lhs, const Allocator<T2>& rhs) noexcept {
    return true;
}

template <class T1, class T2>
bool operator != (const Allocator<T1>& lhs, const Allocator<T2>& rhs) noexcept {
    return false;
}


#endif //HW8_ALLOCATOR_H
