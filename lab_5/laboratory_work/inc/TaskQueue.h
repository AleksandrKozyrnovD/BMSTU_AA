#ifndef __TASK_QUEUE_H__
#define __TASK_QUEUE_H__

#include "General/SafeQueue.h"
#include "scrapper.h"

using Type = Scrapped;

template<>
class SafeQueue<Type>
{
private:
    struct Node
    {
        Type data;
        std::shared_ptr<Node> next;
        explicit Node(Type data) : data(data), next(nullptr) {}

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

    void enqueue(Type data);
    std::optional<Type> dequeue();

    void operator=(const SafeQueue&);
    void operator=(SafeQueue&&);

    int get_size() const;

    bool empty() const;
    operator bool() const;
};

#include "TaskQueue.hpp"

#endif
