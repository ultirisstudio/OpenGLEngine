#pragma once

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>

template <typename T>
class TSQueue {
private:
    std::queue<T> m_queue;

    std::mutex m_mutex;

    std::condition_variable m_cond;

public:
    // Pushes an element to the queue
    void push(T item)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_queue.push(item);

        m_cond.notify_one();
    }

    T front()
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_cond.wait(lock,
            [this]() { return !m_queue.empty(); });

        T item = m_queue.front();

        return item;
    }

    void pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_cond.wait(lock,
            [this]() { return !m_queue.empty(); });

        m_queue.pop();
    }

    bool empty()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    size_t size()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.size();
    }
};