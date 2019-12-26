//
// Created by admin2 on 15.12.2019.
//

#include "ThreadPool.h"



void ThreadPool::threadSafeEnqueue(std::unique_ptr<ThreadPool::AbstractTask>&& task) {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    queue_.push(std::move(task));
}

std::unique_ptr<ThreadPool::AbstractTask> ThreadPool::threadSafeDequeue() {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    if (queue_.empty())
        return {};
    auto task_ptr = std::move(queue_.front());
    queue_.pop();
    return task_ptr;
}

bool ThreadPool::threadSafeIsQueueEmpty() {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    return queue_.empty();
}

void ThreadPool::threadFunction() {
    using namespace std::chrono_literals;
    while (keep_working_) {
        if (keep_working_ && !threadSafeIsQueueEmpty()) {
            auto task = threadSafeDequeue();
            if (task && keep_working_)
                task->run();
        }

        std::unique_lock<std::mutex> lock(task_ready_mutex_);
        while (keep_working_ && threadSafeIsQueueEmpty())
            task_ready_conditional_variable_.wait_for(lock, 1s);
    }
}


