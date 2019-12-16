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
        return std::unique_ptr<AbstractTask>();
    auto task_ptr = std::move(queue_.front());
    queue_.pop();
    return task_ptr;
}

void ThreadPool::threadFunction() {
    while (keep_working_) {
        if (keep_working_ && !queue_.empty()) {
            auto task = threadSafeDequeue();
            if (task && keep_working_)
                task->run();
        }

        std::unique_lock<std::mutex> lock(task_ready_mutex_);
        while (keep_working_ && queue_.empty())
            task_ready_conditional_variable_.wait(lock);
    }
}