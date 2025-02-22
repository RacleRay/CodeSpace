#pragma once

#include <array>
#include <atomic>

#include "Structs.h"

namespace RayCacheMemPool {

class CentralCache {
  public:
    static CentralCache *getInstance() {
        static CentralCache instance;
        return &instance;
    }

    void *fetchPage(size_t index);
    void returnPage(void *p, size_t index);

  private:
    CentralCache() {
        for (auto &aptr : free_lists_) { aptr.store(nullptr); }
        for (auto &flag : spin_locks_) { flag.clear(); }
    }

    void *fetchPageCache(size_t size);

  private:
    std::array<std::atomic<void *>, FREE_LIST_SIZE> free_lists_;
    std::array<std::atomic_flag, FREE_LIST_SIZE> spin_locks_;
};
} // namespace RayCacheMemPool