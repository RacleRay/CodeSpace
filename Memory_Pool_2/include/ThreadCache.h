#pragma once

#include <array>

#include "Structs.h"

namespace RayCacheMemPool {

class ThreadCache {
  public:
    static ThreadCache *getInstance() {
        static thread_local ThreadCache instance;
        return &instance;
    }

    void *alloc(size_t size);
    void dealloc(void *p, size_t size);

    ThreadCache(const ThreadCache &) = delete;
    ThreadCache(ThreadCache &&) = delete;
    ThreadCache &operator=(const ThreadCache &) = delete;
    ThreadCache &operator=(ThreadCache &&) = delete;

  private:
    ThreadCache() = default;

    void *fetchCentralCache(size_t size);
    void returnCentralCache(void *p, size_t size, size_t index);

    size_t getBatchSize(size_t size);

  private:
    // for size      : 1~8B, 8B~16B, 16B~24B, ...
    // index         :  0      1       2      ...
    // free list head: head   head    head    ...
    //                  |      |       |
    //                 next   next    next
    std::array<void *, FREE_LIST_SIZE> free_lists_;
};

} // namespace RayCacheMemPool