#include "ThreadCache.h"
#include "CentralCache.h"

namespace RayCacheMemPool {

void *ThreadCache::alloc(size_t size) {
    if (size == 0) { size = ALIGNMENT; }

    if (size > MAX_BYTES) { return malloc(size); }

    size_t index = SizeRounded::getIndex(size);

    // check local free list
    if (void *ptr = free_lists_[index]) {
        free_lists_[index] = *reinterpret_cast<void **>(ptr);
        return ptr;
    }

    return fetchCentralCache(size);
}

void ThreadCache::dealloc(void *p, size_t size) {
    if (size > MAX_BYTES) { return free(p); }

    size_t index = SizeRounded::getIndex(size);
    // p is now point to the old free list head
    *reinterpret_cast<void **>(p) = free_lists_[index];
    // set free list head
    free_lists_[index] = p;
}

void *ThreadCache::fetchCentralCache(size_t index) {
    void *p = CentralCache::getInstance()->fetchPage(index);
    if (p == nullptr) { return nullptr; }

    void *result = p;
    free_lists_[index] = reinterpret_cast<void **>(p);

    return result;
}

void ThreadCache::returnCentralCache(void *p, size_t size, size_t unit_size) {
    size_t index = SizeRounded::getIndex(size);

    size_t batch_num = size / unit_size;
    if (batch_num < 1) { return; }

    char *cur = static_cast<char *>(p);
    size_t keep_num = std::max(batch_num / 4, size_t(1));
    size_t return_num = batch_num - keep_num;

    char *split_addr = cur + keep_num * unit_size;
    void *next = *reinterpret_cast<void **>(split_addr);
    *reinterpret_cast<void **>(split_addr) = nullptr;  // point to nullptr

    free_lists_[index] = p;

    if (return_num > 0) {
        char *return_start = static_cast<char *>(next);
        CentralCache::getInstance()->returnPage(
            return_start, return_num * unit_size);
    }
}

} // namespace RayCacheMemPool