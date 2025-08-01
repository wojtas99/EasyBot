#pragma once
#include <mutex>
#include <vector>
#include <functional>
#include <future>

class SafeQueue {
private:
    std::mutex mutex_;
    std::vector<std::function<void()>> callbacks_;

public:

    template <typename Func>
    auto enqueue(Func&& f) -> std::future<decltype(f())> {
        using ReturnType = decltype(f());
        auto task = std::make_shared<std::packaged_task<ReturnType()>>(std::forward<Func>(f));
        std::future<ReturnType> result = task->get_future();
        {
            std::lock_guard<std::mutex> lock(mutex_);
            callbacks_.push_back([task]() { (*task)(); });
        }
        return result;
    }
    void execute_all() {
        std::vector<std::function<void()>> temp_callbacks;

        {
            std::lock_guard<std::mutex> lock(mutex_);
            temp_callbacks.swap(callbacks_);
        }

        for (auto& callback : temp_callbacks) {
            callback();
        }
    }
};
