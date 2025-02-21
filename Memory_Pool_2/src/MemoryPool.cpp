#include <cassert>

#include "MemoryPool.h"

namespace RayMemPool {

MemoryPool::MemoryPool(size_t block_size) noexcept
    : block_size(block_size)
    , slot_size(0)
    , first_block(nullptr)
    , cur_slot(nullptr)
    , free_list(nullptr)
    , last_slot(nullptr) {
}

MemoryPool::~MemoryPool() noexcept {
    Slot *cur_block = first_block;
    while (cur_block) {
        Slot *next_block = cur_block->next.load();
        operator delete(reinterpret_cast<void *>(cur_block));
        cur_block = next_block;
    }
}

void MemoryPool::init(size_t size) {
    assert(size > 0);

    slot_size = size;
    first_block = nullptr;
    cur_slot = nullptr;
    free_list = nullptr;
    last_slot = nullptr;
}

void *MemoryPool::alloc() {
    // 1. use free list first
    Slot *slot = popFreeList();
    if (slot != nullptr) { return reinterpret_cast<void *>(slot); }

    // 2. task cur slot
    {
        std::lock_guard<std::mutex> lock(block_mutex);
        if (cur_slot >= last_slot) { allocBlock(); }
        slot = cur_slot;
        cur_slot += slot_size / sizeof(Slot); // slot_size may not be equal to sizeof(Slot)
    }

    return slot;
}

void MemoryPool::dealloc(void *p) {
    if (p == nullptr) { return; }
    Slot *slot = reinterpret_cast<Slot *>(p);
    pushFreeList(slot);
}

size_t padptr(void *p, size_t align) {
    return (
        reinterpret_cast<size_t>(p)
        & (align - 1)); // (align - reinterpret_cast<size_t>(p)) % align;
}

void MemoryPool::allocBlock() {
    void *new_block_void = operator new(block_size);
    Slot *new_block = reinterpret_cast<Slot *>(new_block_void);
    new_block->next = first_block;
    first_block = new_block;

    char *unaligned = reinterpret_cast<char *>(new_block) + sizeof(Slot *);
    size_t padd_size = padptr(unaligned, slot_size);
    cur_slot = reinterpret_cast<Slot *>(unaligned + padd_size);

    last_slot =
        reinterpret_cast<Slot *>(reinterpret_cast<char *>(new_block) + block_size - slot_size + 1);
    free_list = nullptr;
}

bool MemoryPool::pushFreeList(Slot *slot) {
    while (true) {
        Slot *head = free_list.load(std::memory_order_relaxed);
        slot->next.store(head, std::memory_order_relaxed);

        if (free_list.compare_exchange_weak(
                head, slot, std::memory_order_release, std::memory_order_relaxed)) {
            return true;
        }
    }
}

Slot *MemoryPool::popFreeList() {
    while (true) {
        Slot *head = free_list.load(std::memory_order_relaxed);
        if (head == nullptr) { return nullptr; }

        Slot *next = head->next.load(std::memory_order_relaxed);

        if (free_list.compare_exchange_weak(
                head, next, std::memory_order_acquire, std::memory_order_relaxed)) {
            return head;
        }
    }
}

} // namespace RayMemPool

namespace RayMemPool {

void HashBucket::init() {
    for (int i = 0; i < MEMORY_POOL_NUM; i++) { getAMemoryPool(i).init((i + 1) * SLOT_BASE_SIZE); }
}

MemoryPool &HashBucket::getAMemoryPool(int index) {
    static MemoryPool memory_pool[MEMORY_POOL_NUM];
    return memory_pool[index];
}

void *HashBucket::alloc(size_t size) {
    if (size <= 0) { return nullptr; }
    if (size > MAX_SLOT_SIZE) { return operator new(size); }

    return getAMemoryPool(((size + SLOT_BASE_SIZE - 1) / SLOT_BASE_SIZE) - 1).alloc();
}

void HashBucket::dealloc(void *p, size_t size) {
    if (p == nullptr) { return; }
    if (size > MAX_SLOT_SIZE) {
        operator delete(p);
        return;
    }

    getAMemoryPool(((size + SLOT_BASE_SIZE - 1) / SLOT_BASE_SIZE) - 1).dealloc(p);
}
} // namespace RayMemPool
