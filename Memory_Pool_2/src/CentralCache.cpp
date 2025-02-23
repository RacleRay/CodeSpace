#include <thread>

#include "CentralCache.h"
#include "PageCache.h"

namespace RayCacheMemPool {

constexpr size_t SPAN_PER_PAGE = 8;

void *CentralCache::fetchPage(size_t index) {
    if (index >= FREE_LIST_SIZE) { return nullptr; }

    while (spin_locks_[index].test_and_set(std::memory_order_acquire)) {
        std::this_thread::yield();
    }

    void *result = nullptr;
    try {
        result = free_lists_[index].load(std::memory_order_relaxed);

        // alloc new block from page cache
        if (result == nullptr) {
            size_t size = (index + 1) * ALIGNMENT;
            result = fetchPageCache(size);

            if (result == nullptr) {
                spin_locks_[index].clear(std::memory_order_release);
                return result;
            }

            char *start = static_cast<char *>(result);
            size_t block_num = (SPAN_PER_PAGE * PageCache::PAGE_SIZE) / size;

            if (block_num > 1) {
                for (size_t i = 1; i < block_num; i++) {
                    void *cur = start + (i - 1) * size;
                    void *next = start + i * size;
                    *reinterpret_cast<void **>(cur) = next;
                }
                *reinterpret_cast<void **>(start + (block_num - 1) * size) =
                    nullptr;

                void *next = *reinterpret_cast<void **>(result);
                *reinterpret_cast<void **>(result) = nullptr;
                free_lists_[index].store(next, std::memory_order_release);
            }
        } else {
            void *next = *reinterpret_cast<void **>(result);
            *reinterpret_cast<void **>(result) = nullptr;
            free_lists_[index].store(next, std::memory_order_release);
        }
    } catch (...) {
        spin_locks_[index].clear(std::memory_order_release);
        throw;
    }

    spin_locks_[index].clear(std::memory_order_release);
    return result;
}

void *CentralCache::fetchRange(size_t index, size_t batch_size) {
    if (index >= FREE_LIST_SIZE || batch_size == 0) { return nullptr; }

    while (spin_locks_[index].test_and_set(std::memory_order_acquire)) {
        std::this_thread::yield();
    }

    void *result = nullptr;
    try {
        result = free_lists_[index].load(std::memory_order_relaxed);

        if (result == nullptr) {
            size_t size = (index + 1) * ALIGNMENT;
            result = fetchPageCache(size);

            if (result == nullptr) {
                spin_locks_[index].clear(std::memory_order_release);
                return result;
            }

            char *start = static_cast<char *>(result);
            size_t block_num = (SPAN_PER_PAGE * PageCache::PAGE_SIZE) / size;
            size_t alloc_blocks = std::min(batch_size, block_num);

            if (alloc_blocks > 1) {
                for (size_t i = 1; i < alloc_blocks; i++) {
                    void *cur = start + (i - 1) * size;
                    void *next = start + i * size;
                    *reinterpret_cast<void **>(cur) = next;
                }
                *reinterpret_cast<void **>(start + (alloc_blocks - 1) * size) =
                    nullptr;
            }

            if (block_num > alloc_blocks) {
                void *remain_p_start = start + alloc_blocks * size;
                for (size_t i = alloc_blocks; i < block_num; i++) {
                    void *cur = start + (i - 1) * size;
                    void *next = start + i * size;
                    *reinterpret_cast<void **>(cur) = next;
                }
                *reinterpret_cast<void **>(start + (block_num - 1) * size) =
                    nullptr;
                free_lists_[index].store(
                    remain_p_start, std::memory_order_release);
            }
        } else {
            void *cur = result;
            void *prev = nullptr;
            size_t cnt = 0;

            while (cur && cnt < batch_size) {
                prev = cur;
                cur = *reinterpret_cast<void **>(cur);
                cnt++;
            }

            if (prev) { *reinterpret_cast<void **>(prev) = nullptr; }

            free_lists_[index].store(cur, std::memory_order_release);
        }
    }

    catch (...) {
        spin_locks_[index].clear(std::memory_order_release);
        throw;
    }

    spin_locks_[index].clear(std::memory_order_release);
    return result;
}

void CentralCache::returnPage(void *p, size_t index) {
    if (p == nullptr || index >= FREE_LIST_SIZE) { return; }

    while (spin_locks_[index].test_and_set(std::memory_order_acquire)) {
        std::this_thread::yield();
    }

    try {
        void *cur = free_lists_[index].load(std::memory_order_relaxed);
        *reinterpret_cast<void **>(p) = cur;
        free_lists_[index].store(p, std::memory_order_release);
    } catch (...) {
        spin_locks_[index].clear(std::memory_order_release);
        throw;
    }

    spin_locks_[index].clear(std::memory_order_release);
}

void *CentralCache::fetchPageCache(size_t size) {
    size_t page_needed =
        (size + PageCache::PAGE_SIZE - 1) / PageCache::PAGE_SIZE;

    if (size <= SPAN_PER_PAGE * PageCache::PAGE_SIZE) {
        return PageCache::getInstance()->allocSpan(SPAN_PER_PAGE);
    } else {
        return PageCache::getInstance()->allocSpan(page_needed);
    }
}

} // namespace RayCacheMemPool
