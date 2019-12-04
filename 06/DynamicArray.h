//
// Created by admin2 on 25.11.2019.
//

#ifndef HW6_DYNAMICARRAY_H
#define HW6_DYNAMICARRAY_H

#include <cassert>
#include <algorithm>
#include <cstring>

template <typename T>
class DynamicArray {

public:
    static const std::size_t InitialBufferSize = 1;

    explicit DynamicArray(std::size_t bufferSize=DynamicArray<T>::InitialBufferSize):
            capacity(bufferSize),
            buffer(nullptr),
            size(0)
    {
        if(bufferSize > 0)
            buffer = allocateBuffer(bufferSize);
    }

    DynamicArray(const T* array, size_t length);

    DynamicArray(const DynamicArray& obj)
    {
        allocateMemoryAndCopyFrom(obj);
    }

    DynamicArray(DynamicArray<T>&& obj) noexcept:
            capacity(obj.capacity),
            buffer(obj.buffer),
            size(obj.size)
    {
        obj.buffer = nullptr;
        obj.capacity = 0;
        obj.size = 0;
    }

    ~DynamicArray()
    {
        for (std::size_t i = 0; i < size; i++)
            (buffer + i)->~T();
        freeBuffer(buffer);
    }

    DynamicArray& operator = (const DynamicArray<T>& obj);

    DynamicArray& operator = (DynamicArray<T>&& obj) noexcept;

    [[nodiscard]] std::size_t Capacity() const
    {
        return capacity;
    }

    [[nodiscard]] std::size_t Size() const
    {
        return size;
    }

    T& operator[] (std::size_t index)
    {
        if (index >= size)
            throw std::invalid_argument("Invalid value of index!");

        return buffer[index];
    }

    const T& operator[] (std::size_t index) const
    {
        if (index >= size)
            throw std::invalid_argument("Invalid value of index!");

        return buffer[index];
    }

    void Append(const T& value);

    void InsertAt(std::size_t index, const T& value);

    void DeleteAt(std::size_t index);

    void DeleteLast();

    void Clear()
    {
        size = 0;
        reduceMemory();
    }

    DynamicArray<T> Subarray(std::size_t index, std::size_t count);

    void Reverse();

private:
    std::size_t capacity;
    T* buffer;
    std::size_t size;

    T* allocateBuffer(std::size_t sz) const {
        return static_cast<T*>(::operator new[] (sz * sizeof(T)));
    }

    void freeBuffer(T*& buff) const {
        if (buff != nullptr) {
            ::operator delete[] (static_cast<void*>(buff));
            buff = nullptr;
        }
    }

    void allocateMemoryAndCopyFrom(const DynamicArray& obj);

    void moveDataToNewBufferWithSize(std::size_t newBufferSize);

    void growMemory();

    void reduceMemory();

};

template<typename T>
DynamicArray<T>::DynamicArray(const T* array, size_t length):
        capacity(length),
        buffer(nullptr),
        size(length)
{
    if (array == nullptr)
        throw std::invalid_argument("Argument array should not be nullptr!");

    buffer = allocateBuffer(capacity);
    for (std::size_t i = 0; i < length; i++)
        new(buffer + i) T(array[i]);

}

template<typename T>
DynamicArray<T>& DynamicArray<T>::operator = (const DynamicArray<T>& obj) {
    if (this == &obj)
        return *this;

    freeBuffer(buffer);
    allocateMemoryAndCopyFrom(obj);

    return *this;
}

template<typename T>
DynamicArray<T>& DynamicArray<T>::operator = (DynamicArray<T>&& obj) noexcept {
    if (this == &obj)
        return *this;

    freeBuffer(buffer);

    capacity = obj.capacity;
    buffer = obj.buffer;
    size = obj.size;

    obj.buffer = nullptr;
    obj.capacity = 0;
    obj.size = 0;

    return *this;
}

template<typename T>
void DynamicArray<T>::Append(const T& value) {
    if (size == capacity)
        growMemory();

    new (buffer + size) T(value);
    size++;
}

template<typename T>
void DynamicArray<T>::InsertAt(std::size_t index, const T& value) {
    if (index >= size)
        throw std::invalid_argument("Invalid value of index!");

    if (size == capacity)
        growMemory();

    assert(capacity > size);

    new (buffer + size) T(std::move(buffer[size - 1]));
    for (std::size_t i = size - 1; i > index; i--)
        buffer[i] = std::move(buffer[i - 1]);

    new (buffer + index) T(value);
    size++;
}

template<typename T>
void DynamicArray<T>::DeleteAt(std::size_t index) {
    if (index >= size)
        throw std::invalid_argument("Invalid value of index!");

    (buffer + index)->~T();
    for (std::size_t i = index; i < size - 1; i++)
        buffer[i] = std::move(buffer[i + 1]);

    size--;
    if (size < capacity / 4 )
        reduceMemory();
}

template<typename T>
void DynamicArray<T>::DeleteLast() {
    if (size == 0)
        throw std::underflow_error("Cannot delete last element of empty list!");

    (buffer + size)->~T();
    size--;
    if(size < capacity / 4 )
        reduceMemory();
}

template<typename T>
DynamicArray<T> DynamicArray<T>::Subarray(std::size_t index, std::size_t count) {
    if (index >= size)
        throw std::invalid_argument("Invalid value of index!");

    if (count > size - index )
        count = size - index;

    return DynamicArray(buffer + index, count);
}

template<typename T>
void DynamicArray<T>::Reverse() {
    for (std::size_t i = 0; i < size / 2; i++)
        std::swap(buffer[i], buffer[size - i - 1]);
}

template<typename T>
void DynamicArray<T>::allocateMemoryAndCopyFrom(const DynamicArray& obj) {
    capacity = obj.capacity;
    size = obj.size;

    buffer = allocateBuffer(capacity);
    for (std::size_t i = 0; i < size; i++)
        new (buffer + i) T(obj.buffer[i]);
}

template<typename T>
void DynamicArray<T>::moveDataToNewBufferWithSize(std::size_t newBufferSize) {
    if (newBufferSize == capacity )
        return;

    size = std::min(size, newBufferSize);

    if (newBufferSize != 0) {
        T *tempBuffer = allocateBuffer(newBufferSize);
        for (std::size_t i = 0; i < size; i++) {
            new(tempBuffer + i) T(buffer[i]);
            (buffer + i)->~T();
        }
        freeBuffer(buffer);
        buffer = tempBuffer;
    } else {
        freeBuffer(buffer);
    }

    capacity = newBufferSize;
}

template<typename T>
void DynamicArray<T>::growMemory() {
    std::size_t newBufferSize = std::max<std::size_t>(capacity * 2, std::max<std::size_t>(DynamicArray<T>::InitialBufferSize, 1) );

    assert( newBufferSize > Size() );

    moveDataToNewBufferWithSize(newBufferSize);
}

template<typename T>
void DynamicArray<T>::reduceMemory() {
    std::size_t newBufferSize = std::max(capacity / 2, DynamicArray<T>::InitialBufferSize);

    moveDataToNewBufferWithSize(newBufferSize);
}

template <typename T>
const std::size_t DynamicArray<T>::InitialBufferSize;

#endif //HW6_DYNAMICARRAY_H
