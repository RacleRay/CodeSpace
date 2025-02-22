#include <iostream>
#include <thread>
#include <vector>

#include "MemoryPool.h"

// 测试用例
class P1 {
    int id_;
};

class P2 {
    int id_[5];
};

class P3 {
    int id_[10];
};

class P4 {
    int id_[20];
};

void BenchmarkMemoryPool(size_t ntimes, size_t nworks, size_t rounds) {
    std::vector<std::thread> vthread(nworks);
    size_t total_costtime = 0;
    for (size_t k = 0; k < nworks; ++k) {
        vthread[k] = std::thread([&]() {
            for (size_t j = 0; j < rounds; ++j) {
                size_t begin1 = clock();
                for (size_t i = 0; i < ntimes; i++) {
                    P1 *p1 = RayMemPool::newElement<P1>();
                    RayMemPool::deleteElement<P1>(p1);
                    P2 *p2 = RayMemPool::newElement<P2>();
                    RayMemPool::deleteElement<P2>(p2);
                    P3 *p3 = RayMemPool::newElement<P3>();
                    RayMemPool::deleteElement<P3>(p3);
                    P4 *p4 = RayMemPool::newElement<P4>();
                    RayMemPool::deleteElement<P4>(p4);
                }
                size_t end1 = clock();

                total_costtime += end1 - begin1;
            }
        });
    }
    for (auto &t : vthread) { t.join(); }
    printf(
        "%lu threads %lu rounds, per newElement&deleteElement %lu times. total cost：%lu ms\n",
        nworks, rounds, ntimes, total_costtime);
}

void BenchmarkNew(size_t ntimes, size_t nworks, size_t rounds) {
    std::vector<std::thread> vthread(nworks);
    size_t total_costtime = 0;
    for (size_t k = 0; k < nworks; ++k) {
        vthread[k] = std::thread([&]() {
            for (size_t j = 0; j < rounds; ++j) {
                size_t begin1 = clock();
                for (size_t i = 0; i < ntimes; i++) {
                    P1 *p1 = new P1;
                    delete p1;
                    P2 *p2 = new P2;
                    delete p2;
                    P3 *p3 = new P3;
                    delete p3;
                    P4 *p4 = new P4;
                    delete p4;
                }
                size_t end1 = clock();

                total_costtime += end1 - begin1;
            }
        });
    }
    for (auto &t : vthread) { t.join(); }
    printf(
        "%lu threads %lu rounds，per malloc&free %lu times. total cost：%lu ms\n",
        nworks, rounds, ntimes, total_costtime);
}

int main() {
    RayMemPool::HashBucket::init();
    // BenchmarkMemoryPool(100, 1, 10);
    // BenchmarkMemoryPool(100, 2, 10);
    // BenchmarkMemoryPool(100, 4, 10);
    BenchmarkMemoryPool(100, 8, 10);
    std::cout << "============================================================="
              << std::endl;
    BenchmarkNew(100, 1, 10);
    // BenchmarkNew(100, 2, 10);
    // BenchmarkNew(100, 4, 10);
    // BenchmarkNew(100, 8, 10);
    return 0;
}
