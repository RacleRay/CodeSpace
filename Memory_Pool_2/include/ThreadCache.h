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

  private:
    std::array<void *, FREE_LIST_SIZE> free_list_;
};

} // namespace RayCacheMemPool