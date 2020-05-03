//
// Created by admin2 on 15.12.2019.
//

#ifndef HW9_THREADPOOL_H
#define HW9_THREADPOOL_H

#include <functional>
#include <array>
#include <queue>
#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <utility>
#include <atomic>

class ThreadPool
{
public:
    explicit ThreadPool(size_t poolSize):
        keep_working_(true)
    {
        if (poolSize == 0) {
            poolSize = std::thread::hardware_concurrency();
            if (poolSize == 0)
                poolSize = 1;
        }

        for (std::size_t i = 0; i < poolSize; ++i)
            threads_.emplace_back(&ThreadPool::threadFunction, this);
    }

    ~ThreadPool()
    {
        keep_working_ = false;
        task_ready_conditional_variable_.notify_all();

        for (auto& thread : threads_)
            thread.join();
    }

    ThreadPool(ThreadPool&& pool) noexcept = default;
    ThreadPool& operator = (ThreadPool&& pool) noexcept = default;

    ThreadPool(const ThreadPool& pool) = delete;
    ThreadPool& operator = (const ThreadPool& pool) = delete;

    // pass arguments by value
    template <class Func, class... Args>
    auto exec(Func func, Args... args);

    [[nodiscard]] std::size_t GetPoolSize() const {
        return threads_.size();
    }

private:

    class AbstractTask {
    public:
        virtual void run() = 0;
        virtual ~AbstractTask() {
        }
    };

    template <class Func, class ReturnType, class... Args>
    class Task : public AbstractTask {
    public:

        explicit Task(std::promise<ReturnType>&& promise, Func&& function, Args&&... args):
                promise_(std::move(promise)),
                function_(std::bind(std::forward<Func>(function), std::forward<Args>(args)... ))
        {
        }

        virtual ~Task() {
        }

        Task(const Task& task) = delete;
        Task& operator = (const Task& task) = delete;

        Task(Task&& task) noexcept = default;
        Task& operator = (Task&& task) noexcept = default;

        void run() override;

    private:
        std::function<ReturnType ()> function_;
        std::promise<ReturnType> promise_;
    };


    std::atomic<bool> keep_working_;

    std::queue<std::unique_ptr<AbstractTask>> queue_;
    std::mutex queue_mutex_;

    std::mutex task_ready_mutex_;
    std::condition_variable task_ready_conditional_variable_;

    std::vector<std::thread> threads_;


    void threadSafeEnqueue(std::unique_ptr<AbstractTask>&& task);

    std::unique_ptr<AbstractTask> threadSafeDequeue();
    bool threadSafeIsQueueEmpty();

    void threadFunction();
};

template<class Func, class... Args>
auto ThreadPool::exec(Func func, Args... args) {
    using ReturnType = decltype(func(args...));
    std::promise<ReturnType> promise;
    std::future<ReturnType> future = promise.get_future();
    threadSafeEnqueue(std::unique_ptr<AbstractTask>(new Task<Func, ReturnType, Args...>(std::move(promise), std::move(func), std::forward<Args>(args)...)));

    task_ready_conditional_variable_.notify_one();

    return future;
}

template<class Func, class ReturnType, class... Args>
void ThreadPool::Task<Func, ReturnType, Args...>::run() {
    try {
        if constexpr (std::is_same_v<ReturnType, void>) {
            function_();
            promise_.set_value();
        } else {
            promise_.set_value(function_());
        }
    }
    catch (...) {
        promise_.set_exception(std::current_exception());
    }
}



#endif //HW9_THREADPOOL_H
