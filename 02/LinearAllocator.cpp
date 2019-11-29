//
// Created by admin2 on 05.11.2019.
//

#include "LinearAllocator.h"

char *LinearAllocator::allocate(size_t size) {
    size_t newSize = offset + size;
    if (newSize <= maxSize && size != 0) {
        char* pointer = buffer + offset;
        offset += size;
        return pointer;
    }
    return nullptr;
}

void LinearAllocator::reset() {
    offset = 0;
}

size_t LinearAllocator::getMaxSize() const {
    return maxSize;
}

size_t LinearAllocator::getAvailableSize() const {
    return maxSize - offset;
}




