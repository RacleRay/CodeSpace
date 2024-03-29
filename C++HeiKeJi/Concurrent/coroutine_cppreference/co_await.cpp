#include <coroutine>
#include <iostream>
#include <stdexcept>
#include <thread>


auto switch_to_new_thread(std::jthread& out) {
    struct awaitable {
        std::jthread* p_out;
        bool await_ready() {
            return false;
        }
        void await_suspend(std::coroutine_handle<> h) {
            std::jthread& out = *p_out;
            if (out.joinable()) {
                throw std::runtime_error("thread already started, needed to be null, then generated by internal suspend");
            }
            out = std::jthread([h] {h.resume();});
            std::cout << "New thread " << out.get_id() << " started\n";
        }

        void await_resume() {}
    };
    
    return awaitable{&out};
}


struct task {
    struct promise_type {
        task get_return_object() {
            return {};
        }
        std::suspend_never initial_suspend() {
            return {};
        }
        std::suspend_never final_suspend() noexcept {
            return {};
        }

        void return_void() {}

        void unhandled_exception() {}
    };
};


task resuming_on_new_thread(std::jthread& out) {
    std::cout << "coroutine start, current thread " << std::this_thread::get_id() << std::endl;
    co_await switch_to_new_thread(out);
    std::cout << "coroutine end, current thread " << std::this_thread::get_id() << std::endl;
}


int main() {
    std::jthread out;
    resuming_on_new_thread(out);
    return 0;
}