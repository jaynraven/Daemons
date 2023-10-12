#ifndef __THREADPOOL_HPP__
#define __THREADPOOL_HPP__

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

class ThreadPool {
public:
    ThreadPool(size_t numThreads) : stop(false), numTasks(0) {
        for (size_t i = 0; i < numThreads; ++i) {
            threads.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this] { return stop || !tasks.empty(); });
                        if (stop && tasks.empty())
                            return;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                    {
                        std::lock_guard<std::mutex> lock(countMutex);
                        --numTasks;
                    }
                    completionCondition.notify_one();
                }
            });
        }
    }

    template <typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        using ReturnType = decltype(f(args...));
        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        std::future<ReturnType> result = task->get_future();
        {
            std::lock_guard<std::mutex> lock(countMutex);
            ++numTasks;
        }
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
        return result;
    }

    void wait() {
        std::unique_lock<std::mutex> lock(countMutex);
        completionCondition.wait(lock, [this] { return numTasks == 0; });
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread& thread : threads)
            thread.join();
    }

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::mutex countMutex;
    std::condition_variable completionCondition;
    bool stop;
    size_t numTasks;
};

// int main() {
//     ThreadPool pool(4);

//     // 使用Lambda表达式演示任务的返回结果
//     std::vector<std::future<int>> results;
//     for (int i = 0; i < 8; ++i) {
//         results.emplace_back(pool.enqueue([i] {
//             std::cout << "Task " << i << " executed in thread " << std::this_thread::get_id() << std::endl;
//             std::this_thread::sleep_for(std::chrono::seconds(1));
//             return i * i;
//         }));
//     }

//     // 等待所有任务执行完毕
//     pool.wait();

//     // 获取任务的返回结果
//     for (auto& result : results) {
//         std::cout << "Task result: " << result.get() << std::endl;
//     }

//     return 0;
// }

#endif