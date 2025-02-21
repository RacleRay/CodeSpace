#pragma once

#include <atomic>
#include <mutex>

namespace RayMemPool {

#define DEFAULT_BLOCK_SIZE 4096
#define MAX_SLOT_SIZE 512  // should not be too large
#define MEMORY_POOL_NUM 64
#define SLOT_BASE_SIZE 8

struct Slot {
    std::atomic<Slot *> next;
};

size_t padptr(void *p, size_t align);

class MemoryPool {
  public:
    MemoryPool(size_t block_size = DEFAULT_BLOCK_SIZE) noexcept;
    ~MemoryPool() noexcept;

    void init(size_t);
    void *alloc();
    void dealloc(void *p);

  private:
    void allocBlock();

    bool pushFreeList(Slot *slot);
    Slot *popFreeList();

  private:
    size_t block_size;
    size_t slot_size;

    Slot *first_block;             // first memory block
    Slot *cur_slot;                // first used slot
    std::atomic<Slot *> free_list; // freed slot which had been used
    Slot *last_slot;               // position which shows you should alloc new block
    std::mutex block_mutex;        // in case of mutiple threads alloc block simultaneously
};

class HashBucket {
  public:
    static void init();

    static MemoryPool &getAMemoryPool(int index);

    static void *alloc(size_t size);
    static void dealloc(void *p, size_t size);

    // template<typename T, typename... Args>
    // friend T *newElement(Args &&...args);

    // template<typename T>
    // friend void deleteElement(T *p);
};

template<typename T, typename... Args>
T *newElement(Args &&...args) {
    T *p = nullptr;

    p = reinterpret_cast<T *>(HashBucket::alloc(sizeof(T)));
    if (p != nullptr) { new (p) T(std::forward<Args>(args)...); }

    return p;
}

template<typename T>
void deleteElement(T *p) {
    if (p == nullptr) { return; }
    p->~T();
    HashBucket::dealloc(reinterpret_cast<void *>(p), sizeof(T));
}

} // namespace RayMemPool
