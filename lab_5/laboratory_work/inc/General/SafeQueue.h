#ifndef __SAFE_QUEUE_H__
#define __SAFE_QUEUE_H__

#include <memory>
#include <optional>
#include <mutex>
#include <condition_variable>

// Thread-safe queue with condition variable support
template <typename T>
class SafeQueue
{
private:
    struct Node
    {
        T data;
        std::shared_ptr<Node> next;
        explicit Node(T data) : data(std::move(data)), next(nullptr) {}

        void operator=(Node&& other)
        {
            data = other.data;
            next = std::move(other.next);
        }
    };

    std::shared_ptr<Node> head;
    Node* tail;
    mutable std::mutex queue_mutex;
    std::condition_variable cv;
    int size;

public:
    SafeQueue();
    ~SafeQueue() = default;

    void enqueue(T data);
    std::optional<T> dequeue();

    void operator=(const SafeQueue&);
    void operator=(SafeQueue&&);

    int get_size() const;

    bool empty() const;
    operator bool() const;
};

#include "SafeQueue.hpp"

#endif
