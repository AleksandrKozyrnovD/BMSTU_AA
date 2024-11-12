#ifndef __STAGE_HPP__
#define __STAGE_HPP__

#include "Stage.h"

#include <iostream>



template <typename InputType, typename OutputType>
TypedStage<InputType, OutputType>::TypedStage(SafeQueue<InputType>& input, SafeQueue<OutputType>& output,
                                             std::function<OutputType(InputType)> func, int threads)
: process(std::move(func)), input_queue(input), output_queue(output),
          thread_count(threads), shutdown_flag(false)
{}


template <typename InputType, typename OutputType>
TypedStage<InputType, OutputType>::TypedStage(std::function<OutputType(InputType)> func, int threads)
    : process(std::move(func)), input_queue(nullptr), output_queue(nullptr),
        thread_count(threads), shutdown_flag(false) {}

template <typename InputType, typename OutputType>
void TypedStage<InputType, OutputType>::start()
{
    // if (!input_queue || !output_queue)
    // {
        // throw std::runtime_error("Stage queues are not linked. Call link_queues() before starting the stage.");
    // }

    std::vector<std::thread> threads;
    for (int i = 0; i < thread_count; ++i)
    {
        threads.emplace_back(&TypedStage::worker, this);
    }
    for (auto& thread : threads)
    {
        thread.join();
    }
}

template <typename InputType, typename OutputType>
void TypedStage<InputType, OutputType>::shutdown()
{
    {
        std::lock_guard<std::mutex> lock(shutdown_mutex);
        shutdown_flag = true;
    }
    shutdown_cv.notify_all();
}

template <typename InputType, typename OutputType>
const std::type_info& TypedStage<InputType, OutputType>::input_type() const
{
    return typeid(InputType);
}

template <typename InputType, typename OutputType>
const std::type_info& TypedStage<InputType, OutputType>::output_type() const
{
    return typeid(OutputType);
}

template <typename InputType, typename OutputType>
SafeQueue<OutputType>* TypedStage<InputType, OutputType>::get_output_queue() const
{
    return output_queue;
}

template <typename InputType, typename OutputType>
void TypedStage<InputType, OutputType>::link_queues(SafeQueue<InputType>* in_queue, SafeQueue<OutputType>* out_queue)
{
    input_queue = in_queue;
    output_queue = out_queue;
}

template<typename InputType, typename OutputType>
void TypedStage<InputType, OutputType>::worker()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(shutdown_mutex);
        if (shutdown_flag)
            break;

        auto item = input_queue.dequeue();
        if (!item)
            continue; // Wait for new data if queue is empty

        OutputType result = process(*item);
        // output_queue.enqueue(std::move(result));
        output_queue.enqueue(result);
    }
}

#endif
