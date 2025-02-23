#include <chrono>
#include <random>
#include <thread>
#include <vector>

#include "CacheMemoryPool.h"
#include "ColorPrint.h"
#include "Timer.h"

using namespace RayCacheMemPool;
using namespace std::chrono;

class PerformanceTest {
  private:
    struct TestState {
        double memPoolTime{0.0};
        double sysTime{0.0};
        size_t totalAllocSize{0};
        size_t totalBytes{0};
    };

  public:
    static void warmup() {
        PRINT_BLUE("Warmup memory pool...");

        std::vector<std::pair<void *, size_t>> warmupMem;
        constexpr size_t WARMUP_TIMES = 1000;
        for (size_t i = 0; i < WARMUP_TIMES; i++) {
            for (size_t size : {8, 16, 32, 64, 128, 256, 512}) {
                void *p = MemoryPool::alloc(WARMUP_TIMES);
                warmupMem.emplace_back(p, size);
            }
        }

        for (const auto &[p, size] : warmupMem) {
            MemoryPool::dealloc(p, size);
        }

        PRINTF_GREEN("warmup {} memory alloc done", WARMUP_TIMES);
    };

    static void testSmallAlloc() {
        constexpr size_t TOTAL_ALLOCS = 1000000;
        constexpr size_t SMALL_SIZE = 128;

        PRINT_BLUE("\nTesting small alloc...");
        PRINTF_BLUE("Allocating {}, size {}", TOTAL_ALLOCS, SMALL_SIZE);

        {
            Timer timer;

            std::vector<void *> ptrs;
            ptrs.reserve(TOTAL_ALLOCS);

            for (size_t i = 0; i < TOTAL_ALLOCS; i++) {
                ptrs.push_back(MemoryPool::alloc(SMALL_SIZE));
                if ((i & 3) == 0) {
                    MemoryPool::dealloc(ptrs.back(), SMALL_SIZE);
                    ptrs.pop_back();
                }
            }

            for (void *p : ptrs) { MemoryPool::dealloc(p, SMALL_SIZE); }

            double mem_pool_time = timer.elapsed();
            PRINTF_GREEN("Memory poll time cost: {} ms", mem_pool_time);
        }

        // sleep 1 s
        std::this_thread::sleep_for(std::chrono::seconds(1));

        {
            Timer timer;

            std::vector<char *> ptrs;
            ptrs.reserve(TOTAL_ALLOCS);

            for (size_t i = 0; i < TOTAL_ALLOCS; i++) {
                ptrs.push_back(new char[SMALL_SIZE]);
                if ((i & 3) == 0) {
                    delete[] ptrs.back();
                    ptrs.pop_back();
                }
            }

            for (char *p : ptrs) { delete[] p; }

            double new_delete_time = timer.elapsed();
            PRINTF_GREEN("New delete time cost: {} ms", new_delete_time);
        }
    };

    static void testMixedSize() {

    };

    static void testMultiThread() {
        constexpr size_t NUM_THREADS = 8;
        constexpr size_t ALLOCS_PER_THREAD = 1000000;
        constexpr size_t MAX_SIZE = 256;

        PRINT_BLUE("\nTesting multi-thread alloc...");
        PRINTF_YELLOW(
            "NUM_THREADS: {}, ALLOCS_PER_THREAD: {}, MAX_SIZE: {}", NUM_THREADS,
            ALLOCS_PER_THREAD, MAX_SIZE);

        auto thread_func = [](bool use_memory_pool) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(8, MAX_SIZE);

            std::vector<std::pair<void *, size_t>> ptrs;
            ptrs.reserve(ALLOCS_PER_THREAD);

            for (size_t i = 0; i < ALLOCS_PER_THREAD; i++) {
                size_t size = dis(gen);
                void *p =
                    use_memory_pool ? MemoryPool::alloc(size) : new char[size];
                ptrs.push_back({p, size});

                if (rand() % 100 < 75) {
                    size_t index = rand() % ptrs.size();
                    if (use_memory_pool) {
                        MemoryPool::dealloc(
                            ptrs[index].first, ptrs[index].second);
                    } else {
                        delete[] static_cast<char *>(ptrs[index].first);
                    }
                    ptrs[index] = ptrs.back();
                    ptrs.pop_back();
                }
            }

            for (const auto &[ptr, size] : ptrs) {
                if (use_memory_pool) {
                    MemoryPool::dealloc(ptr, size);
                } else {
                    delete[] static_cast<char *>(ptr);
                }
            }
        };

        {
            Timer timer;

            std::vector<std::thread> threads;
            for (size_t i = 0; i < NUM_THREADS; i++) {
                threads.emplace_back(thread_func, true);
            }

            for (auto &t : threads) { t.join(); }

            double mem_pool_time = timer.elapsed();
            PRINTF_GREEN("Memory poll time cost: {} ms", mem_pool_time);
        }

        // sleep 1 s
        std::this_thread::sleep_for(std::chrono::seconds(1));

        {
            Timer timer;

            std::vector<std::thread> threads;
            for (size_t i = 0; i < NUM_THREADS; i++) {
                threads.emplace_back(thread_func, false);
            }

            for (auto &t : threads) { t.join(); }

            double new_delete_time = timer.elapsed();
            PRINTF_GREEN("New delete time cost: {} ms", new_delete_time);
        }
    };
};

int main() {
    PRINT_BLUE("Starting performance tests...");

    PerformanceTest::warmup();

    PerformanceTest::testSmallAlloc();

    // std::this_thread::sleep_for(std::chrono::seconds(1));
    PerformanceTest::testMultiThread();
    return 0;
}
