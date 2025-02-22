#include <chrono>

using namespace std::chrono;

class Timer {
    high_resolution_clock::time_point start;

  public:
    Timer() : start(high_resolution_clock::now()) {}

    double elapsed() {
        auto end = high_resolution_clock::now();
        return duration_cast<duration<double>>(end - start).count()
               / 1000.0; // ms
    }
};
