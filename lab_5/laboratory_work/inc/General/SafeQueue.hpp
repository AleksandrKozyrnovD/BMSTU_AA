#ifndef __SAFE_QUEUE_HPP__
#define __SAFE_QUEUE_HPP__

#include "SafeQueue.h"

template<typename T>
SafeQueue<T>::SafeQueue()
: head(nullptr), tail(nullptr), size(0)
{}

template<typename T>
void SafeQueue<T>::enqueue(T data)
{
    auto new_node = std::make_unique<Node>(std::move(data));
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        if (tail)
        {
            tail->next = std::move(new_node);
            tail = tail->next.get();
        }
        else
        {
            head = std::move(new_node);
            tail = head.get();
        }
        ++size;
    }
}

template<typename T>
std::optional<T> SafeQueue<T>::dequeue()
{
    std::unique_lock<std::mutex> lock(queue_mutex);
    // cv.wait(lock, [this] { return head != nullptr; });

    if (!head)
        return std::nullopt;
    T result = std::move(head->data);

    head = std::move(head->next);
    if (!head)
        tail = nullptr;

    --size;

    return result;
}

template<typename T>
int SafeQueue<T>::get_size() const
{
    // std::lock_guard<std::mutex> lock(queue_mutex);
    return size;
}

template<typename T>
bool SafeQueue<T>::empty() const
{
    std::lock_guard<std::mutex> lock(queue_mutex);
    return head == nullptr;
}

template<typename T>
SafeQueue<T>::operator bool() const
{
    return !empty();
}


template<typename T>
void SafeQueue<T>::operator=(const SafeQueue& other)
{
    std::lock_guard<std::mutex> lock(other.queue_mutex);
    head = other.head;
    tail = other.tail;
    size = other.size;
}

template<typename T>
void SafeQueue<T>::operator=(SafeQueue&& other)
{
    std::lock_guard<std::mutex> lock(other.queue_mutex);
    head = std::move(other.head);
    tail = std::move(other.tail);
    size = other.size;
}

#endif
