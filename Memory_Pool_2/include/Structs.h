#pragma once

#include <algorithm>
#include <cstddef>

namespace RayCacheMemPool {

constexpr size_t ALIGNMENT = 8;
constexpr size_t MAX_BYTES = 256 * 1024; // 256KB
constexpr size_t FREE_LIST_SIZE = MAX_BYTES / ALIGNMENT;

struct BlockHeader {
    size_t size;
    bool inUse;
    BlockHeader *next;
};

class SizeRounded {
  public:
    // round up by alignment
    static size_t roundUp(size_t bytes) {
        return (bytes + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
    }

    // first index of available block chunked by alignment
    static size_t getIndex(size_t bytes) {
        bytes = std::max(bytes, ALIGNMENT);
        return (bytes + ALIGNMENT - 1) / ALIGNMENT - 1;
    }
};
} // namespace RayCacheMemPool