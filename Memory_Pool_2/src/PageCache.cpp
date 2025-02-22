#include <cstring>
#include <sys/mman.h>

#include "PageCache.h"

namespace RayCacheMemPool {

void *PageCache::allocSpan(size_t n_pages) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = free_spans_.lower_bound(n_pages);
    if (it != free_spans_.end()) {
        Span *span = it->second;

        if (span->next) {
            free_spans_[it->first] = span->next;
        } else {
            free_spans_.erase(it);
        }

        if (span->numPages > n_pages) {
            Span *new_span = new Span;
            new_span->pageAddr =
                static_cast<char *>(span->pageAddr) + n_pages * PAGE_SIZE;
            new_span->numPages = span->numPages - n_pages;
            new_span->next = nullptr;

            Span *list = free_spans_[new_span->numPages];
            new_span->next = list;
            free_spans_[new_span->numPages] = new_span;

            span->numPages = n_pages;
        }

        span_map_[span->pageAddr] = span;
        return span->pageAddr;
    }

    // new alloc
    void *mem_addr = sysAlloc(n_pages);

    if (!mem_addr) { return nullptr; }

    Span *span = new Span;
    span->pageAddr = mem_addr;
    span->numPages = n_pages;
    span->next = nullptr;

    span_map_[mem_addr] = span;
    return mem_addr;
}

void PageCache::deallocSpan(void *p, size_t n_pages) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = span_map_.find(p);
    if (it == span_map_.end()) { return; }

    Span *span = it->second;
    // merge two spans in free list
    void *next_addr = static_cast<char *>(p) + n_pages * PAGE_SIZE;
    auto next_it = span_map_.find(next_addr);
    if (next_it != span_map_.end()) {
        Span *next_span = next_it->second;

        bool found = false;
        auto &next_list = free_spans_[next_span->numPages];
        if (next_list == next_span) {
            next_list = next_span->next;
            found = true;
        } else if (next_list) {
            Span *prev = next_list;
            while (prev->next) {
                if (prev->next == next_span) {
                    prev->next = next_span->next;
                    found = true;
                    break;
                }
                prev = prev->next;
            }
        }

        if (found) {
            span->numPages += next_span->numPages;
            span_map_.erase(next_addr);
            delete next_span;
        }
    }

    Span *list = free_spans_[span->numPages];
    span->next = list;
    free_spans_[span->numPages] = span;
}

void *PageCache::sysAlloc(size_t n_pages) {
    size_t size = n_pages * PAGE_SIZE;

    void *p = mmap(
        nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1,
        0);
    if (p == MAP_FAILED) { return nullptr; }

    memset(p, 0, size);
    return p;
}

} // namespace RayCacheMemPool