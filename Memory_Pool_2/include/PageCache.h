#pragma once

#include <map>
#include <mutex>

namespace RayCacheMemPool {

class PageCache {
  public:
    static const size_t PAGE_SIZE = 4096;

    static PageCache *getInstance() {
        static PageCache instance;
        return &instance;
    }

    void *allocSpan(size_t n_pages);
    void deallocSpan(void *p, size_t n_pages);

    PageCache(const PageCache &) = delete;
    PageCache(PageCache &&) = delete;
    PageCache &operator=(const PageCache &) = delete;
    PageCache &operator=(PageCache &&) = delete;

  private:
    PageCache() = default;

    void *sysAlloc(size_t n_pages);

  private:
    struct Span {
        void *pageAddr;
        size_t numPages;
        Span *next;
    };

    std::map<size_t, Span *> free_spans_;
    std::map<void *, Span *> span_map_;

    std::mutex mutex_;
};
} // namespace RayCacheMemPool