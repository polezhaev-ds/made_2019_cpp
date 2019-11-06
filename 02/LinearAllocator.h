//
// Created by admin2 on 05.11.2019.
//

#ifndef HW2_LINEARALLOCATOR_H
#define HW2_LINEARALLOCATOR_H

#include <cstdlib>
#include <new>

class LinearAllocator
{
public:
    explicit LinearAllocator(size_t maxSize):
        maxSize(maxSize),
        offset(0),
        buffer(nullptr)
    {
        if (maxSize > 0) {
            buffer = (char *) std::malloc(maxSize);
            if (buffer == nullptr)
                throw std::bad_alloc();
        }
    }

    ~LinearAllocator() {
        if (buffer != nullptr)
            std::free(buffer);
    }

    LinearAllocator(const LinearAllocator& obj) = delete;
    LinearAllocator(LinearAllocator&& obj) = default;
    LinearAllocator& operator= (const LinearAllocator& obj) = delete;
    LinearAllocator& operator= (LinearAllocator&& obj) = default;

    char* allocate(size_t size);
    void reset();

    [[nodiscard]] size_t getMaxSize() const;
    [[nodiscard]] size_t getAvailableSize() const;

private:
    size_t maxSize;
    size_t offset;
    char* buffer;

};


#endif //HW2_LINEARALLOCATOR_H
