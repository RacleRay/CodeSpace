#include "CacheMemoryPool.h"
#include "ThreadCache.h"

namespace RayCacheMemPool {

void *MemoryPool::alloc(size_t size) {
    return ThreadCache::getInstance()->alloc(size);
}

void MemoryPool::dealloc(void *p, size_t size) {
    return ThreadCache::getInstance()->dealloc(p, size);
}

} // namespace RayCacheMemPool