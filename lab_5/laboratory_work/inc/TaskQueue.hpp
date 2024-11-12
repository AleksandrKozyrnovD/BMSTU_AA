#ifndef __TASK_QUEUE_HPP__
#define __TASK_QUEUE_HPP__

#include "TaskQueue.h"
#include "scrapper.h"
#include <iostream>

SafeQueue<Type>::SafeQueue()
: head(nullptr), tail(nullptr), size(0)
{}

void SafeQueue<Type>::enqueue(Type data)
{
    // data.times.push_back({time(nullptr), 0, 0, 0});
    ProcessTime process_time;
    process_time.start = std::chrono::system_clock::now();
    data.times.push_back(process_time);
    auto new_node = std::make_unique<Node>(std::move(data));
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        if (tail)
        {
            tail->next = std::move(new_node);
            tail = tail->next.get();
            // tail->data.times.push_back({time(nullptr), 0});
        }
        else
        {
            head = std::move(new_node);
            tail = head.get();
        }
        // std::cout << "Time inserted, time size = " << tail->data.times.size() << std::endl;
        ++size;
    }
}

std::optional<Type> SafeQueue<Type>::dequeue()
{
    std::unique_lock<std::mutex> lock(queue_mutex);
    // cv.wait(lock, [this] { return head != nullptr; });


    if (!head)
        return std::nullopt;
    Type result = std::move(head->data);
    result.times.back().end = std::chrono::system_clock::now();

    head = std::move(head->next);
    if (!head)
        tail = nullptr;

    --size;

    return result;
}

int SafeQueue<Type>::get_size() const
{
    // std::lock_guard<std::mutex> lock(queue_mutex);
    return size;
}

bool SafeQueue<Type>::empty() const
{
    std::lock_guard<std::mutex> lock(queue_mutex);
    return head == nullptr;
}

SafeQueue<Type>::operator bool() const
{
    return !empty();
}


void SafeQueue<Type>::operator=(const SafeQueue& other)
{
    std::lock_guard<std::mutex> lock(other.queue_mutex);
    head = other.head;
    tail = other.tail;
    size = other.size;
}

void SafeQueue<Type>::operator=(SafeQueue&& other)
{
    std::lock_guard<std::mutex> lock(other.queue_mutex);
    head = std::move(other.head);
    tail = std::move(other.tail);
    size = other.size;
}

#endif
