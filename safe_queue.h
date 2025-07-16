#pragma once
#include <mutex>
#include <vector>
#include <functional>

class SafeQueue {
private:
    std::mutex mutex_;
    std::vector<std::function<void()>> callbacks_;

public:
    void enqueue(const std::function<void()>& callback) {
        std::lock_guard<std::mutex> lock(mutex_);
        callbacks_.push_back(callback);
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
