#pragma once

#include <cstddef>

namespace RayCacheMemPool {

struct MemoryPool {
    static void *alloc(size_t size);
    static void dealloc(void *p, size_t size);
};

}