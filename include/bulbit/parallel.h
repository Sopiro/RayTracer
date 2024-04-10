#pragma once

#include "common.h"

#include <condition_variable>
#include <mutex>
#include <optional>
#include <thread>

namespace bulbit
{

class ThreadPool;

class ParallelJob
{
public:
    virtual ~ParallelJob() = default;

    virtual bool HaveWork() const = 0;
    virtual void RunStep(std::unique_lock<std::mutex>* lock) = 0;

    bool Finished() const
    {
        return !HaveWork() && active_workers == 0;
    }

protected:
    friend class ThreadPool;
    ThreadPool* thread_pool;

private:
    // Active threads working on this job
    int32 active_workers = 0;

    // Links
    ParallelJob* prev = nullptr;
    ParallelJob* next = nullptr;
};

class ThreadPool
{
public:
    explicit ThreadPool(int32 worker_count);
    ~ThreadPool();

    void WorkOrWait(std::unique_lock<std::mutex>* lock);
    bool WorkOrReturn();

    std::unique_lock<std::mutex> AddJob(ParallelJob* job);
    void RemoveJob(ParallelJob* job);

    void ForEachThread(std::function<void(void)> func);

    size_t WorkerCount() const
    {
        return threads.size() + 1;
    }

private:
    void Worker();

    bool shutdown = false;

    std::vector<std::thread> threads;
    std::mutex mutex;
    std::condition_variable job_list_condition;

    ParallelJob* job_list = nullptr;
};

inline std::unique_ptr<ThreadPool> g_thread_pool = nullptr;

} // namespace bulbit
